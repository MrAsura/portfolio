#pragma once

//Class for representing a Graph.
//Manages vertices belonging to the graph.
//

#include "Vertex.h"

class Graph
{
public:
	Graph();
	Graph( std::vector<Vertex::vertex_ptr> vertices );
	virtual ~Graph();

	//Define a color that can be used to color graphs. The color is a changing value so vertex color needs to be reset only when color overflows. Otherwise, to designate a vertex is visited, use current color. If color differs in a vertex it has an old value and isn't visited yet.
	
	Vertex::color curColor() const; //Return current color value
	Vertex::color nextColor() const; //Return next color but don't change it
	Vertex::color prevColor() const; //Return previous color but don't change it

	Vertex::color changeColor(); //Change to next color and return it

	Vertex::vertex_ptr first() const; //Return first vertex in list
	Vertex::vertex_ptr last() const; //Return last vertex in list
	Vertex::vertex_ptr at( std::size_t ind ) const; //Return indth vertex from list
	std::size_t size() const; //Return the number of vertices in graph

	bool isGraphSameColor( Vertex::color c ) const; //Checks if all elements in graph have the same color c

	static const Vertex::color INIT_COLOR; //Contains the color that curColor() never returns. Can be used to initialize Vertex colors

private:

	Vertex::color cur_color_; //Holds value of current color.

	std::vector<Vertex::vertex_ptr> vertices_;

	void resetVertexColors(); //Reset vertex colors when color overflows

	//Copying this class is a shallow copy. Will strill refer to the same vertices.
	Graph & operator=( const Graph& ) = delete;
	//Graph( const Graph& ) = delete;
};

