#include "Vertex.h"

#include <cassert>

Vertex::Vertex(): Vertex( color() )
{
}

Vertex::Vertex( color c ): m_(), edges_(0)
{
	//Set a specific color for vertex
	color_.store(c);
	degree_.store( static_cast<int>(edges_.size()) );
}


Vertex::~Vertex()
{
}

int Vertex::degree()
{
	return degree_.load();
}

Vertex::vertex_ptr Vertex::edgeAt( std::size_t ind )
{
	return edges_.at(ind);
}

std::vector<Vertex::vertex_ptr> Vertex::edges()
{
	//locked with a mutex
	std::lock_guard<std::mutex> lock( m_ );
	return edges_;
}

void Vertex::addOutEdge( vertex_ptr v )
{
	//locked with a mutex
	std::lock_guard<std::mutex> lock( m_ );
	edges_.push_back( v );
	degree_.store( static_cast<int>( edges_.size() ) );
}

void Vertex::addOutEdges( std::vector<vertex_ptr> V )
{
	//locked with a mutex
	std::lock_guard<std::mutex> lock( m_ );
	edges_.insert( edges_.end(), V.begin(), V.end() );
	degree_.store( static_cast<int>( edges_.size() ) );
}

void Vertex::colorVertex( color c )
{
	//Operation is atomic
	color_.store(c);
}

bool Vertex::compareColor( color c )
{
	//Operation is atomic
	return color_.load() == c;
}

bool Vertex::colorCompare( color c )
{
	color cur = color_.load();
	bool is_same = cur == c;

	while( !is_same && !color_.compare_exchange_strong( cur, c ) )
	{
		is_same = cur == c;
	}

	assert( color_.load() == c );

	return is_same;
}
