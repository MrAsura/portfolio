#include "Graph.h"

#include <cassert>

const Vertex::color Graph::INIT_COLOR = 0; //0 is the value cur color shouldn't have outside of changeColor and reset.

Graph::Graph(): Graph( std::vector<Vertex::vertex_ptr>(0) )
{
}


Graph::Graph( std::vector<Vertex::vertex_ptr> vertices ): cur_color_(INIT_COLOR), vertices_(vertices)
{
	changeColor();
}


Graph::~Graph()
{
	//The graph is in charge of deleting its vertices
	//for( Vertex::vertex_ptr v : vertices_ )
	//{
	//	if( v != nullptr )
	//	{
	//		delete v;
	//	}
	//}

	vertices_.clear();
}

Vertex::color Graph::curColor() const
{
	return cur_color_;
}

Vertex::color Graph::nextColor() const
{
	return cur_color_ + 1;
}

Vertex::color Graph::prevColor() const
{
	return cur_color_ - 1;
}

Vertex::color Graph::changeColor()
{
	//Increase the value of cur_color_ by one store old value
	Vertex::color old = cur_color_++;

	//Check that the new value has not overflown
	if( old > cur_color_ )
	{
		//Overflow. Reset vertices
		//cur_color_ should be 0
		resetVertexColors();
		
		//Increase color again so reset vertices don't register as found etc.
		++cur_color_;
	}

	return cur_color_;
}

Vertex::vertex_ptr Graph::first() const
{
	return vertices_.front();
}

Vertex::vertex_ptr Graph::last() const
{
	return vertices_.back();
}

Vertex::vertex_ptr Graph::at( std::size_t ind ) const
{
	return vertices_.at(ind);
}

std::size_t Graph::size() const
{
	return vertices_.size();
}

bool Graph::isGraphSameColor( Vertex::color c ) const
{
	for( Vertex::vertex_ptr v : vertices_ )
	{
		assert( v->compareColor( c ) );
		//Check if matching color
		if( !v->compareColor( c ) )
		{
			return false;
		}
	}

	return true;
}

void Graph::resetVertexColors()
{
	for( Vertex::vertex_ptr v : vertices_ )
	{
		//Set color to current color
		v->colorVertex( cur_color_ );
	}
}
