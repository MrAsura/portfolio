#pragma once

//Implement a general vertex class for use in a graph.
//Vertex is implemented so that it can be accessed concurrently
//This implementation doesn't have data assosiated with the vertex. Inherit this class to add actual content.

#include <vector>
#include <memory>
#include <atomic>
#include <mutex>

class Vertex
{
public:

	//Define a color that can be used to color graphs. The color is a changing value so vertex color needs to be reset only when color overflows. Otherwise, to designate a vertex is visited, use current color. If color differs in a vertex it has an old value and isn't visited yet.
	typedef unsigned int color;
	
	typedef std::shared_ptr<Vertex> vertex_ptr; //Define a pointer type to make it easier to switch to smart pointer if necessary.

	Vertex();
	Vertex( color c );
	virtual ~Vertex();


	int degree(); //Return the number of vertices this vertex connects to.
	vertex_ptr edgeAt( std::size_t ind ); //Return the indth neigboring vertex
	std::vector<vertex_ptr> edges(); //Return the vertices this vertex connects to. Vertices are in the same order they were added
	void addOutEdge( vertex_ptr v ); //Add a new connection to vertex v.
	void addOutEdges( std::vector<vertex_ptr> V ); //Add several edges

	void colorVertex( color c ); //Colors vertex a new color c
	bool compareColor( color c ); //Check if vertex has same color as color c
	bool colorCompare( color c ); //Color vertex c (if not already) and return true if cur color is c and false otherwise

private:
	typedef std::atomic<color> atomic_color; //Define a atomic vertion of color that is quaranteet to be thread safe

	std::vector<vertex_ptr> edges_; //Contain pointers to all the other vertices this vertex is connected to.
	atomic_color color_; //Vetexes current color. Used for algorithms. Color asigned by Graph. Graph specific
	std::atomic_int degree_; //store the degree as a atomic value
	std::mutex m_; //mutex for locking edge adding operations

	//Dont allow copy or insertion
	Vertex & operator=( const Vertex& ) = delete;
	Vertex( const Vertex& ) = delete;
};

