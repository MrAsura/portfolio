#include "GraphFactory.h"

#include <vector>
#include <algorithm>
#include <iterator>
#include <set>
#include <cmath>

GraphFactory::GraphFactory(): gen_()
{
}


GraphFactory::~GraphFactory()
{
}

GraphFactory & GraphFactory::getInstance()
{
	static GraphFactory fac;
	return fac;
}

Graph* GraphFactory::makeBinTree( const unsigned int height )
{
	//Construct a bin tree and return it as a graph
	std::vector<Vertex::vertex_ptr> vertices(0);
	vertices.push_back( makeV() );

	for( unsigned int level = 1; level <= height; level++ )
	{
		//add child to all in this level
		//level inds go from 2^(level-1)-1 to 2^(level)-1
		unsigned int beg = static_cast<unsigned int>(exp2(  level - 1 ) - 1);
		unsigned int end = static_cast<unsigned int>( exp2( level ) - 1 );
		for( unsigned int r_num = beg; r_num < end; r_num++ )
		{
			Vertex::vertex_ptr lc = makeV();
			Vertex::vertex_ptr rc = makeV();
			vertices.at( r_num )->addOutEdge( lc );
			vertices.at( r_num )->addOutEdge( rc );
			vertices.push_back( lc );
			vertices.push_back( rc );
		}
	}
	
	return new Graph( vertices );
}

Graph* GraphFactory::makeKn( const unsigned int n )
{
	std::vector<Vertex::vertex_ptr> vertices( 0 );

	//Add n vertices and connect all of them to each other
	for( unsigned int ind = 0; ind < n; ind++ )
	{
		Vertex::vertex_ptr v = makeV();
		v->addOutEdges( vertices );
		vertices.emplace_back( v );
	}
	//Only Last element is fully connected. Need to connect first added to the rest
	for( unsigned int ind = 0; ind < vertices.size() - 1; ind++ )
	{
		for( unsigned int k = ind + 1; k < vertices.size(); k++ )
		{
			vertices.at( ind )->addOutEdge( vertices.at( k ) );
		}
	}

	return new Graph( vertices );
}

Graph * GraphFactory::makeRandomArityN( const unsigned int n, const unsigned int size )
{
	//If n=0 return an empty graph
	if( n == 0 )
	{
		return new Graph();
	}
	
	//We want a connected graph (no sense for testing purposes otherwise) so each vertex has at leas degree 1. We make a directed graph with the first element being the root.
	//Build a graph with random arity n. We chooce a random degree from the range x=rand([0,m]), where m is such that the expected value of the range is n (thus the average degree should be clode to n). We have E(x) = 0*1/(m+1) + 1*1/(m+1) + ... + m*1/(m+1) = 1/2*m => m = 2*n
	unsigned int m = 2 * n;
	std::uniform_int_distribution<unsigned int> distr( 0, m );
	std::uniform_real_distribution<double> prob( 0, 1 ); //For calculating probabilities

	//Create a permutation vector for choosing random vertices from the current values
	//std::vector<std::size_t> perms( 0 );
	//std::size_t perm_num = 0; //Next index to be added to perms. At most the size of vertices

	std::vector<Vertex::vertex_ptr> vertices( 0 );
	std::size_t v_ind = 0; //index of the vertex being handeled

	//Create first vertex
	vertices.push_back( makeV() );
	//perms.push_back( perm_num++ );

	while( vertices.size() < size || v_ind < vertices.size() )
	{
		Vertex::vertex_ptr cur = vertices.at( v_ind++ );

		unsigned int deg = distr( gen_ ); //Get degree of vertex
		bool has_added_v = false; //Make sure atleast one vertex is added
		//std::size_t perm_ind = 0; //Keep track of used indices
		//std::size_t perm_size_old = perms.size(); //Keep track of edges that existed before cur
		//std::shuffle( perms.begin(), perms.end(), gen_ ); //Suffle the current perms
		std::set<std::size_t> used; //vertices already added as neighbours
		std::size_t old_size = vertices.size(); //Keep track of last ind containing a vertex added before this round
		used.insert( old_size ); //Insert old_size as a stopper

		for( unsigned int ind = 0; ind < deg; ++ind )
		{
			//Either choose an existing vertex to connect to or a vertex that doesn't exist (create new)
			//The probability of chooding a new vertex is 1-vertices.size()/size
			//So that the loop end, atleast one of the added edges has to be to a new vertex
			//Greating parralel edges is also pointless so if the old perms has been used up, force creating a new edge
			double new_prob = static_cast<double>( vertices.size() ) / static_cast<double>( size );
			bool add_v = ( prob( gen_ ) > new_prob );
			bool last_add = ( ind + 1 >= deg && !has_added_v );
			bool no_perms = used.size() >= old_size;//( perm_ind >= perm_size_old );
			bool new_cond = add_v || last_add || no_perms;
			if( new_cond && vertices.size() < size )
			{
				//We create a new vertex as the child of cur
				Vertex::vertex_ptr child = makeV();
				cur->addOutEdge( child );
				vertices.push_back( child );
				//perms.push_back( perm_num++ );
				has_added_v = true;
			}
			else if( !no_perms )
			{
				//Choose an existing vertex checking that it is not in used
				//Choose from 0 to old_size-1
				std::uniform_int_distribution<std::size_t> old_inds( 0, old_size - 1 );
				std::size_t old_ind = old_inds( gen_ );
				auto pos = used.find( old_ind );
				//Go throug used and only use old_ind if it is not in used.
				if( pos != used.end() )
				{
					do
					{
						if( old_ind >= old_size )
						{
							old_ind = 0;
						}

						//If used is less than half full pick a random ind
						if( used.size() <= old_size/2 + 1 )
						{
							old_ind = old_inds( gen_ );
						}
						else
						{
							//else take the next element untill we find a suitable one
							++old_ind;
						}
						pos = used.find( old_ind );

					} while( pos != used.end() );
				}

				cur->addOutEdge( vertices.at( old_ind ) );
				used.insert( old_ind );
			}
			else
			{
				break;
			}
		}
	}

	return new Graph(vertices);
}


void GraphFactory::chain( const unsigned int length, std::vector<Vertex::vertex_ptr>& vertices )
{
	Vertex::vertex_ptr prev = vertices.front();

	//make vertices connected to the previous vertex
	for( unsigned int turn = 0; turn < length; turn++ )
	{
		Vertex::vertex_ptr next = makeV();
		prev->addOutEdge( next );
		vertices.push_back( next );
		prev = next;
	}
}

inline Vertex::vertex_ptr GraphFactory::makeV()
{
	return std::make_shared<Vertex>(Graph::INIT_COLOR);
}

Graph * GraphFactory::makeSGrid( const unsigned int size )
{
	return nullptr;
}

Graph * GraphFactory::makeCGrid( const unsigned int size )
{
	return nullptr;
}

Graph * GraphFactory::makeChain( const unsigned int length )
{
	Vertex::vertex_ptr root = makeV();
	std::vector<Vertex::vertex_ptr> vertices( 0 );
	vertices.push_back( root );

	chain( length, vertices );

	return new Graph(vertices);
}

Graph * GraphFactory::makeParChain( const unsigned int x, const unsigned int length )
{
	Vertex::vertex_ptr root = makeV();
	std::vector<Vertex::vertex_ptr> vertices( 0 );
	vertices.push_back( root );

	for( unsigned int ch = 0; ch < x; ch++ )
	{
		chain( length, vertices );
	}

	return new Graph(vertices);
}

Graph * GraphFactory::makeTreeXY( const unsigned int x, const unsigned int y, const unsigned int num )
{
	std::vector<Vertex::vertex_ptr> vertices( 0 );
	Vertex::vertex_ptr root = makeV();
	vertices.push_back( root );

	//Determine how many levels
	for( unsigned int loop = 0; loop < num; loop++ )
	{
		//Generate x vertices with y children. If y is 0, returns a TreeX
		for( unsigned int n = 0; n < x; n++ )
		{
			Vertex::vertex_ptr new_x = makeV();
			root->addOutEdge( new_x );
			vertices.push_back( new_x );

			for( unsigned int m = 0; m < y; m++ )
			{
				Vertex::vertex_ptr new_y = makeV();
				new_x->addOutEdge( new_y );
				vertices.push_back( new_y );
			}
		}

		//Get new root by taking a random vertex from ys
		if( y > 0 )
		{
			//Ys are in the range [vertices.size() - x*y,vertices.size()-1]
			//how ever if ind-vertices.size() % y+1 == 0, we have found one of the xs so choose ind+1 (xs are first so ind+1 is valid)
			std::uniform_int_distribution<std::size_t> distr( vertices.size() - x*y, vertices.size() - 1 );
			std::size_t root_ind = distr( gen_ );

			if( ( root_ind - vertices.size() ) % ( y + 1 ) == 0 ) 
			{
				root_ind++;
			}

			root = vertices.at( root_ind );
		}
		else
		{
			//Only xs so choose one of them (x last vertices)
			std::uniform_int_distribution<std::size_t> distr( vertices.size() - x, vertices.size() - 1 );
			std::size_t root_ind = distr( gen_ );

			root = vertices.at( root_ind );
		}
	}

	return new Graph(vertices);
}

Graph * GraphFactory::makePhaseXDY( const unsigned int x, const unsigned int y, const unsigned int v_in_f )
{
	

	std::vector<Vertex::vertex_ptr> vertices( 0 );
	Vertex::vertex_ptr root = makeV();
	vertices.push_back( root );

	//If v_in_f == 1 , same as a chain
	if( v_in_f == 1 )
	{
		chain( x, vertices );
		return new Graph( vertices );
	}

	std::vector<std::size_t> perm( v_in_f ); //Store a permutation of unique nums for determining children
	std::size_t perm_ind = 0;
	std::generate( perm.begin(), perm.end(), [&perm_ind]() {return perm_ind++; } );

	//Construct each frontier and connect
	for( unsigned int fr = 0; fr < x; fr++ )
	{
		perm_ind = 0; //reset ind and use later to refer to elements of perm
		std::size_t min_ind = fr*v_in_f + 1; //first ind of prev

		//Construct each vertex 
		for( unsigned int v_num = 0; v_num < v_in_f; v_num++ )
		{
			Vertex::vertex_ptr cur = makeV();
			root->addOutEdge( cur );
			vertices.push_back( cur );

			//if the current frontier is greater than 0, add the edges to the prev frontier. Prev frontier is in the range
			//[(fr-1)*v_in_f + 1, fr*v_in_f]
			if( fr > 0 )
			{
				//choose y vertices from prev fr to connnect to
				for( unsigned int loop = 0; loop < y; ++loop, perm_ind = ( perm.at( perm_ind + 1 ) == perm.back() ) ? 0 : perm_ind + 1 )
				{
					vertices.at( min_ind - v_in_f + perm.at( perm_ind ) )->addOutEdge( cur );
				}
			}
		}

		//get new root
		std::shuffle( perm.begin(), perm.end(), gen_ ); //suffle
		root = vertices.at( min_ind + perm.back() );
	}

	return new Graph( vertices );
}
