#pragma once
//A class to represent the frontier of a graph (the vertices currently being handled) as proposed by U. Acar et al.

#include "Range.h"
#include "Vertex.h"
#include "WeightedQueue.h"


class Frontier
{
public:
	Frontier( std::size_t chunk_size = 1);
	~Frontier();

	//Add new edges to frontier
	void pushEdgesOf( Vertex::vertex_ptr vertex );

	//Static function for returning the full range of a vertex
	static Range fullRange( Vertex::vertex_ptr vertex );

	std::size_t numOfEdges(); //Return total number of edges contained in the frontier
	bool isEmpty(); //Check if frontier is empty

	void split( Frontier& other ); //split frontier and share work with other
	void merge( Frontier& other ); //merge two frontiers

	//Iterate over nb edges (and pop) and apply a function func to each vertex that is iterated over.
	//func returns nothing and takes a vertex and a ind to its edge as parameter
	//Return number of edges handled
	std::size_t iterPopOverNb( std::size_t nb, Range::iterFunc_t func );

private:

	//Holds the first and last vertex that may be partially checked
	Range first_;
	Range last_;

	std::shared_ptr<WeightedQueue<Vertex::vertex_ptr>> vertices_; //Contain vertices to be handled
	//use pointers because WeightedQueue doesn't support proper copy yet.
	//Allow copy and insertion, but only copies the values but ranges and vertices still point to the same vertices (shallow copy).

};

