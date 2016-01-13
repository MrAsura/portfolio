#include "WeightedQueue.h"
#include "Vertex.h"

#include <iostream>
#include <string>
#include <algorithm>

#include <cassert>

template< class T >
WeightedQueue<T>::WeightedQueue(std::size_t chunk_size):CHUNK_SIZE(chunk_size), root_(nullptr), first_(nullptr), last_(nullptr)
{
}


template< class T >
WeightedQueue<T>::~WeightedQueue()
{
	node_ptr node = first_;

	while( node != nullptr )
	{
		if( node->left != nullptr )
		{
			node = node->left;
		}
		else if( node->right != nullptr )
		{
			node = node->right;
		}
		else if( node->parent != nullptr )
		{
			node_ptr parent = node->parent;
			
			if( parent->left != nullptr && parent->left == node )
			{
				parent->left = nullptr;
			}
			else
			{
				parent->right = nullptr;
			}

			node->parent = nullptr;
			node = parent;
		}
		else
		{
			node = nullptr;
		}
	}
}

template<class T>
void WeightedQueue<T>::push_back( typename T item, std::size_t weight )
{
	//try to add chunk to last_ node. If full add a new node.
	if( !isEmpty() && !chunkFull( last_ ) )
	{
		chunkPushBack( last_, item, weight );
		weightFixup( last_->parent ); //node weight chances so update
	}
	else
	{
		pushBackNode( makeChunk( item, weight ) );
	}
}

template<class T>
typename T WeightedQueue<T>::pop_front()
{
	//Take an item from first_ node. If it becomes empty, remove node
	//If empty return default value
	if( isEmpty() )
	{
		return T();
	}

	chunk_item_t item = chunkPopFront(first_);

	if( chunkEmpty( first_ ) )
	{
		popFrontNode();
	}
	else
	{
		//Node Weights changed so update weights
		weightFixup( first_->parent );
	}

	return item.second;
}

template<class T>
void WeightedQueue<T>::print() const
{
	if( isEmpty() ) return;

	std::deque<std::deque<node_ptr>> levels( root_->level );
	getNodesInLevels( levels, root_ );

	std::size_t n = 1;

	//Print each level and add connections
	for( std::deque<node_ptr> level : levels )
	{
		std::cout << "Level " << n << ":" ;
		
		for( node_ptr node : level )
		{
			std::cout << std::string(3*n,' ') << "(" << node->weight_left << "," << node->node_weight << "," << node-> weight_right << ")";

			if( node->right != nullptr && node->level == node->right->level )
			{
				std::cout << " >";
			}
			if( node->parent != nullptr && node == node->parent->left && node->parent->level != node->level )
			{
				if( node->parent->right != nullptr && node->parent->right->level == node->parent->level )
				{
					std::cout << "  ";
				}
			}
		}
		std::cout << std::endl << "        ";

		//Print connections to next level
		for( node_ptr node : level )
		{
			std::cout << std::string( 2*n, ' ' ) << "    ";

			if( node->parent != nullptr && node == node->parent->left && node->parent->level != node->level )
			{
				std::cout << "\\";

				if( node->parent->right != nullptr && node->parent->right->level == node->parent->level )
				{
					std::cout << "  ";
				}
			}
			else if( node->parent != nullptr && node->parent->level != node->level )
			{
				std::cout << "/";
			}
			else
			{
				std::cout << "   ";
			}
			
		}
		std::cout << std::endl;
		++n;
	}

	std::cout << std::endl;
}

template<class T>
bool WeightedQueue<T>::isEmpty() const
{
	return root_ == nullptr;
}

template<class T>
std::size_t WeightedQueue<T>::weight() const
{
	if( isEmpty() ) return 0;
	return fullWeight(root_);
}

template<class T>
void WeightedQueue<T>::splitAt( std::size_t weight, T & item, WeightedQueue & other )
{
	if( isEmpty() ) return;

	std::size_t w = this->weight();

	assert( other.isEmpty() );

	//Find split item and dívide nodes
	node_ptr split_node = findWeight( weight );

	size_t w2 = other.weight() + this->weight();
	assert( w2 == w );

	while( first_ != split_node )
	{
		other.pushBackNode( popFrontNode() );
	}

	w2 = other.weight() + this->weight();
	assert( w2 == w );

	size_t snw = split_node->node_weight;

	chunk_item_t split_item;
	node_ptr end = splitChunkOnWeight( weight, split_item, split_node );


	assert( snw == end->node_weight + split_node->node_weight + split_item.first );
	w2 = other.weight() + this->weight() + end->node_weight + split_item.first;
	assert( w2 == w );

	//Remove possible empty nodes
	if( !chunkEmpty( end ) )
	{
		other.pushBackNode( end ); //splits chunk so that first chunk goes to other
	}
	if( chunkEmpty( split_node ) )
	{
		popFrontNode();
	}

	item = split_item.second;

	w2 = other.weight() + this->weight() + split_item.first;
	assert( w2 == w );

	//this contains the end of the queue and other the front so we swap the queues
	swap( other );
}

template<class T>
void WeightedQueue<T>::concat( WeightedQueue & other )
{
	while( !other.isEmpty() )
	{
		pushBackNode( other.popFrontNode() );
	}
}

template<class T>
void WeightedQueue<T>::swap( WeightedQueue & other )
{
	std::swap( root_, other.root_ );
	std::swap( first_, other.first_ );
	std::swap( last_, other.last_ );
}

template<class T>
void WeightedQueue<T>::fixup( node_ptr node )
{
	//Start from node and move up until root is reached
	while( node != nullptr )
	{
		node = skew( node );
		node = split( node );

		node = node->parent;
	}
}

template<class T>
void WeightedQueue<T>::popfixup( node_ptr node )
{
	//Move to root and decrease level if a child is two levels below (or null and level > 1)
	while( node != nullptr )
	{
		//Levels of the right and left children
		std::size_t rc_level = node->right == nullptr ? 0 : node->right->level;
		std::size_t lc_level = node->left == nullptr ? 0 : node->left->level;


		if( rc_level < node->level - 1 || lc_level < node->level - 1 )
		{
			node->level--;

			if( node->right->level > node->level )
			{
				node->right->level--;
			}

			node = skew( node );
			skew( node->right );
			skew( node->right->right );

			node = split( node );
			split( node->right );
		}
		else
		{
			updateWeights( node ); //Update weights
		}

		node = node->parent;
	}
}

template<class T>
typename WeightedQueue<T>::node_ptr WeightedQueue<T>::skew( node_ptr node )
{
	if( node == nullptr ) return node;

	//Check if skew is needed
	if( !(node->left != nullptr && node->level == node->left->level) )
	{
		//Update weights
		updateWeights( node );
		return node;
	}

	//switch node and its left child and give node the right child of left to node
	//First move node->parent to point to node->left
	node_ptr parent = node->parent;
	node_ptr left = node->left;
	
	if( parent == nullptr )
	{
		//node is root so make left root_
		root_ = left;
	}
	else if( parent->left == node )
	{
		parent->left = left;
	}
	else
	{
		parent->right = left;
	}
	left->parent = parent;

	//Give node lefts right child
	node->left = left->right;
	
	if( left->right != nullptr )
	{
		left->right->parent = node;
	}

	node->parent = left;
	left->right = node;

	//Update weights
	updateWeights( node );
	updateWeights( left );

	return left;
}

template<class T>
typename WeightedQueue<T>::node_ptr WeightedQueue<T>::split( node_ptr node )
{
	if( node == nullptr ) return node;

	//Check if split is needed
	if( !(node->right != nullptr && node->right->right != nullptr && node->level == node->right->right->level) )
	{
		//Update weights
		updateWeights( node );
		return node;
	}

	//raise right child to the next level and give node rights left child
	node_ptr parent = node->parent;
	node_ptr right = node->right;

	if( parent == nullptr )
	{
		//node is root so make right root_
		root_ = right;
	}
	else if( parent->left == node )
	{
		parent->left = right;
	}
	else
	{
		parent->right = right;
	}
	right->parent = parent;

	//give node rights left child
	node->right = right->left;

	if( right->left != nullptr )
	{
		right->left->parent = node;
	}

	node->parent = right;
	right->left = node;
	right->level++; //increase level

	//Update weights
	updateWeights( node );
	updateWeights( right );

	return right;
}

template<class T>
void WeightedQueue<T>::pushBackNode( const Chunk & data )
{
	//Create a node and add it to the end of the queue
	pushBackNode( makeNode(data) );
}

template<class T>
void WeightedQueue<T>::pushBackNode( node_ptr node )
{
	if( root_ == nullptr )
	{
		root_ = node;
		first_ = node;
		node->parent = nullptr;
	}
	else
	{
		node->parent = last_;
		last_->right = node;
	}
	last_ = node;

	node->level = 1; //All leaf nodes have level 1
	
	//Initial weights will be zero, expect node_weight will be same as the total weight of data
	node->weight_left = 0;
	node->weight_right = 0;

	fixup( node->parent );
}

template<class T>
typename WeightedQueue<T>::node_ptr WeightedQueue<T>::popFrontNode()
{
	//Remove first_ node
	node_ptr parent = first_->parent;
	node_ptr node = first_;

	if( parent == nullptr && first_->right == nullptr )
	{
		root_ = nullptr;
		first_ = nullptr;
		last_ = nullptr;

		return node;
	}
	else if( parent == nullptr && first_->right != nullptr )
	{
		//One left 
		root_ = first_->right;
		first_->right = nullptr;
		first_ = root_;

		while( first_->left != nullptr )
		{
			first_ = first_->left;
		}

		root_->parent = nullptr;

		return node;
	}

	first_->parent = nullptr;

	//Since we are removing the first node, it is quaranteed to be a left child
	//We make firsts_ right side parents new left child
	parent->left = first_->right;

	if( first_->right != nullptr )
	{
		first_->right->parent = parent;
		first_->right = nullptr;
		first_ = parent->left;
	}
	else
	{
		first_ = parent;
	}

	popfixup( parent );

	//Return node
	return node;
}

template<class T>
typename WeightedQueue<T>::node_ptr WeightedQueue<T>::makeNode( const Chunk & data )
{
	//Construct a default node (floating eg. not connected to anything
	node_ptr node = std::make_shared<Node>();
	node->left = nullptr;
	node->right = nullptr;
	node->parent = nullptr;
	node->level = 1;

	node->weight_left = 0;
	node->weight_right = 0;
	node->node_weight = chunkWeight( data );
	node->data = data;

	return node;
}

template<class T>
typename WeightedQueue<T>::Chunk WeightedQueue<T>::makeChunk( const T& item, const std::size_t& weight ) const
{
	chunk_item_t c_item = std::make_pair( weight, item );
	std::deque<chunk_item_t> items( 1, c_item );
	Chunk c = { items };

	return c;
}

template<class T>
std::size_t WeightedQueue<T>::chunkWeight( const Chunk & data ) const
{
	std::size_t weight = 0;

	//Just iterate over chunk items
	for( chunk_item_t item : data.items )
	{
		weight += item.first;
	}

	return weight;
}

template<class T>
void WeightedQueue<T>::weightFixup( node_ptr node )
{
	while( node != nullptr )
	{
		updateWeights( node );
		node = node->parent;
	}
}

template<class T>
void WeightedQueue<T>::updateWeights( node_ptr node )
{
	if( node == nullptr ) return;

	//Update left and right weights
	node->weight_left = node->left == nullptr ? 0 : fullWeight( node->left );
	node->weight_right = node->right == nullptr ? 0 : fullWeight( node->right );
}

template<class T>
std::size_t WeightedQueue<T>::fullWeight( const node_ptr node ) const
{
	if( node == nullptr ) return 0;

	//Full weight is just the sum of all the weights of a node
	return node->node_weight + node->weight_left + node->weight_right;
}

template<class T>
bool WeightedQueue<T>::chunkFull( const Chunk & data ) const
{
	return data.items.size() >= CHUNK_SIZE;
}

template<class T>
bool WeightedQueue<T>::chunkEmpty( const Chunk & data ) const
{
	return data.items.empty();
}

template<class T>
bool WeightedQueue<T>::chunkFull( node_ptr node ) const
{
	return chunkFull(node->data);
}

template<class T>
bool WeightedQueue<T>::chunkEmpty( node_ptr node ) const
{
	return chunkEmpty( node->data );
}

template<class T>
void WeightedQueue<T>::chunkPushBack( node_ptr node, const T & item, const std::size_t & weight )
{
	node->node_weight += weight; //Increace node weight
	node->data.items.emplace_back( weight, item );

	//Update tree weights with fixup
	weightFixup( node->parent );
}

template<class T>
void WeightedQueue<T>::chunkPushBack( node_ptr node, const chunk_item_t & item )
{
	node->node_weight += item.first;
	node->data.items.push_back( item );

	weightFixup( node->parent );
}

template<class T>
typename WeightedQueue<T>::chunk_item_t WeightedQueue<T>::chunkPopFront( node_ptr node )
{
	chunk_item_t c_item = node->data.items.front();
	node->data.items.pop_front();
	node->node_weight -= c_item.first; //Decreace node weight
	return c_item;
}

template<class T>
typename WeightedQueue<T>::node_ptr WeightedQueue<T>::splitChunkOnWeight( std::size_t weight, chunk_item_t& item, node_ptr node )
{
	std::size_t total_w = 0;
	Chunk first_chunk = { std::deque<chunk_item_t>( 0 ) };
	node_ptr first = makeNode( first_chunk );
	
	std::size_t w = node->node_weight;

	item = chunk_item_t();

	while( !chunkEmpty(node) )
	{
		chunk_item_t elem = chunkPopFront(node);

		if( total_w <= weight && total_w + elem.first > weight )
		{
			item = elem;
			break;
		}
		else if( chunkEmpty( node ) )
		{
			//elem is the last item but the condition doesn't hold
			//choose the last item as the split item anyway to prevent errors
			item = elem;
			break;
		}

		total_w += elem.first;
		chunkPushBack( first, elem );
	}

	//Items have been popped from node so parents weights need to be updated
	if( node->parent != nullptr )
	{
		weightFixup( node->parent );
	}

	assert( w == first->node_weight + node->node_weight + item.first );

	return first;
}

template<class T>
typename WeightedQueue<T>::node_ptr WeightedQueue<T>::findWeight( std::size_t& weight ) const
{
	if( isEmpty() )
	{
		weight = 0;
		return nullptr;
	}
	node_ptr node = root_;

	//if nodes left weight is greater than weight go there
	//else go right and decrease weight (since left weight is the weight so far, the right node has a greater weight by its weight
	//When left weight is equal to or smaller than weight and node_weight takes it over weight, we have found our split node.
	while( node != nullptr )
	{
		if( node->weight_left <= weight && node->weight_left + node->node_weight > weight )
		{
			break;
		}
		else if( node->weight_left > weight )
		{
			//go left
			if( node->left == nullptr ) break;
			node = node->left;
		}
		else
		{
			//go right
			if( node->right == nullptr ) break;
			weight = weight - node->node_weight - node->weight_left;
			node = node->right;
		}
	}

	//We need to update weight to hold the weight the chunk needs to be split at. Simply remove the weight before node.
	weight -= node->weight_left;

	return node;
}

template<class T>
void WeightedQueue<T>::getNodesInLevels( std::deque<std::deque<node_ptr>>& levels, const node_ptr start ) const
{
	if( start == nullptr )
	{
		return;
	}

	getNodesInLevels( levels, start->left );

	levels.at( start->level - 1 ).push_back( start );

	getNodesInLevels( levels, start->right );
}


//Define used initialization of template to allow compilation
template class WeightedQueue<Vertex::vertex_ptr>;