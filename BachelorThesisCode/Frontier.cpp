#include "Frontier.h"

#include <cassert>

Frontier::Frontier(std::size_t chunk_size): first_(), last_()
{
	vertices_ = std::make_shared<WeightedQueue<Vertex::vertex_ptr>>( chunk_size );
}


Frontier::~Frontier()
{
}

void Frontier::pushEdgesOf( Vertex::vertex_ptr vertex )
{
	if( vertex->degree() != 0 )
	{
		vertices_->push_back( vertex, vertex->degree() );
	}
}

Range Frontier::fullRange( Vertex::vertex_ptr vertex )
{
	return Range(vertex,0,vertex->degree());
}

std::size_t Frontier::numOfEdges()
{
	return first_.weight() + vertices_->weight() + last_.weight();
}

bool Frontier::isEmpty()
{
	return numOfEdges() == 0;
}

void Frontier::split( Frontier & other )
{
	assert( other.isEmpty() );
	//Give half of the available work
	//Check if first_ or last_ range contains enough work
	//if not split vertices_ (assume other is empty)
	std::size_t half = static_cast<std::size_t>( ( numOfEdges() + 1 ) / 2 );
	std::size_t edges = numOfEdges();
	std::size_t w1 = vertices_->weight();
	
	if( half <= first_.weight() )
	{
		first_.splitAt( half, other.first_ );
	}
	else if( half <= last_.weight() )
	{
		last_.splitAt( half, other.first_ );
	}
	else
	{
		// Give last_ to other and split vertices_ evenly as possible
		other.last_ = last_;

		half -= first_.weight();

		Vertex::vertex_ptr v = nullptr;
		vertices_->splitAt( half, v, *other.vertices_ );
		std::size_t w2 = vertices_->weight();
		std::size_t w3 = other.vertices_->weight();
		last_ = fullRange( v );
		last_.splitAt( half - vertices_->weight(), other.first_ );
	}

	assert( edges == numOfEdges() + other.numOfEdges() );
}

void Frontier::merge( Frontier & other )
{
	vertices_->concat( *other.vertices_ );
}

std::size_t Frontier::iterPopOverNb( std::size_t nb, Range::iterFunc_t func )
{
	std::size_t left = nb; //How many iterations left

	//First iterate over firts_ then vertices_ and finally last_ if there are still iterations left
	left -= first_.iterPopOverNb( left, func );

	while( left > 0 && !( vertices_->isEmpty() ) )
	{
		Vertex::vertex_ptr vertex = vertices_->pop_front();
		std::size_t deg = vertex->degree();

		if( deg <= left )
		{
			left -= fullRange( vertex ).iterPopOverNb( deg, func );
		}
		else
		{
			first_ = fullRange( vertex );
			left -= first_.iterPopOverNb( left, func );
			return nb - left;
		}
	}

	left -= last_.iterPopOverNb( left, func );

	return nb - left;
}
