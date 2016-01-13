#pragma once
//A list that supports weighted splitting based on AA trees
//Only supports popping from the front and pushing to the back
//Tree operations are simplified. Each pushed item is treated as having a greater value that any other so far (always added to the right most part)
//Uses chunks to amortize some operations
//For example giving weights of 1 to all items produces a orderstatistical tree

#include <memory>
#include <deque>
#include <utility>

template< class T >
class WeightedQueue
{
public:
	WeightedQueue(std::size_t chunk_size = 1);
	~WeightedQueue();

	void push_back(T item, std::size_t weight ); //Add item to the end of the queue
	T pop_front(); //Pop first item from queue

	//Debugfunction
	void print() const; //Prints a visual reprecentation of the graph to cout

	//Check if queue is empty
	bool isEmpty() const;

	//Return total weight
	std::size_t weight() const;

	//Split and combine queues
	//Split splits the queue so that this.weight() <= weight and this.weight() + item.weight > weight
	void splitAt( std::size_t weight, T& item,  WeightedQueue& other );
	void concat( WeightedQueue& other );

	//Swap the content of two queues (note chunk_sizes are not swapped)
	void swap( WeightedQueue& other );

private:

	struct Node;
	typedef std::shared_ptr<Node> node_ptr;
	typedef std::pair<std::size_t, T> chunk_item_t;
	
	const std::size_t CHUNK_SIZE; //Size of chunks

	//The chunk that contains individual data items 
	struct Chunk 
	{
		std::deque<chunk_item_t> items; //Contain a list of the items and associated weights
	};

	//struct that represents the vertex in the AA  tree
	struct Node
	{
		node_ptr left; //Points to left child
		node_ptr right; //Points to right child
		node_ptr parent; //Points to parent node
		std::size_t level; //Level of node. Leaf nodes have level one

		//Weight information
		std::size_t node_weight;
		std::size_t weight_left; //Total weight in left child node
		std::size_t weight_right; //Total weiht in right child node

		Chunk data; //Contain some data type T that is associated with each node
	};


	//Pointers to root and first/last node
	node_ptr root_;
	node_ptr first_;
	node_ptr last_;

	//Functions for fixing the tree so that the invariants hold
	void fixup( node_ptr node ); //Fix the tree starting from node
	void popfixup( node_ptr node ); //Fix after item popped
	node_ptr skew( node_ptr node ); //Fix a node with horizontal left link. Return the node that replaced the original node
	node_ptr split( node_ptr node ); //Fix two horizontal right links. Return the node that replaced the original node

	//Add node to end of list. Added node will be pointed to by last_ 
	void pushBackNode( const Chunk& data ); //Add new node with data to end
	void pushBackNode( node_ptr node ); //Add existing node
	node_ptr popFrontNode(); //Remove first
	node_ptr makeNode(const Chunk& data); //Construct a floating node with data inside

	Chunk makeChunk( const T& item, const std::size_t& weight ) const; //Construct a chunk with a initial item given as parameters.
	std::size_t chunkWeight( const Chunk& data ) const; //Calculate total weight of chunk

	//Functions for handling node weights
	void weightFixup( node_ptr node ); //Fix weights starting from node and moving to root
	void updateWeights( node_ptr node ); //Update weights of node
	std::size_t fullWeight( const node_ptr node ) const; //Calculate the total weight of  node

	//Functions for checking if chunk is full or empty
	//Precondition isEmpty() == false
	bool chunkFull( const Chunk& data ) const;
	bool chunkEmpty( const Chunk& data ) const;
	bool chunkFull( node_ptr node ) const;
	bool chunkEmpty( node_ptr node ) const;

	//Functions for adding and removing items from the chunk of the given node
	void chunkPushBack( node_ptr node, const T& item, const std::size_t& weight );
	void chunkPushBack( node_ptr node, const chunk_item_t& item );
	chunk_item_t chunkPopFront( node_ptr node );

	//Split chunk on weight
	//Return a node containing the elements before item. node will contain the elements after item.
	//item will contain the split item
	node_ptr splitChunkOnWeight( std::size_t weight, chunk_item_t& item, node_ptr node );

	node_ptr findWeight( std::size_t& weight ) const; //Find the node for which holds: weight_before <= weight and weight_before + n_weight > weight. weight to split on is updated to the one required of the node split. Weight will be updated to the value the node chunk may need to be split on to acheave the perfect split.

	//Helper debug function
	void getNodesInLevels(std::deque<std::deque<node_ptr>>& levels, const node_ptr start) const;

	//Dont allow copy or insertion (need to implement them properly)
	WeightedQueue & operator=( const WeightedQueue& ) = delete;
	WeightedQueue( const WeightedQueue& ) = delete;
};

