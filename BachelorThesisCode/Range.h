#pragma once
// Range class as proposed by U. Acar et al.
// Define a set of neighbors (edges) of the vertex that can be iterated over applying the given function for each neighbor
// Supports splitting on weight

#include "Vertex.h"
#include <functional>

class Range
{
public:
	Range();
	Range( Vertex::vertex_ptr v, std::size_t beg = 0, std::size_t end = 0);
	~Range();

	//Return the weight of the range. Same as the number of edges in the range
	std::size_t weight();

	//Split range on weight to a empty range given as a parameter.
	void splitAt( std::size_t weight, Range& other );

	//Iterate over nb edges (and pop) and apply the given function that takes the vertex and a ind of a edge as a parameter.
	//Return number of succesfully handled items
	typedef std::function<void( Vertex::vertex_ptr, std::size_t )> iterFunc_t;
	std::size_t iterPopOverNb( std::size_t nb, iterFunc_t func );

private:
	//Vertex whose neighbors are ranged over
	Vertex::vertex_ptr vertex_;
	//Beg and end inds for the vertices of the outgoing edges
	std::size_t beg_; 
	std::size_t end_; //the index after the final vertex included so at most vertex_.degree()

	void invariant(); //An invariant that needs to hold always
};

