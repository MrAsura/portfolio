#pragma once

//A factory class for constructing graphs

#include "Graph.h"

#include <vector>
#include <random>

class GraphFactory
{
public:
	static GraphFactory& getInstance();

	Graph* makeBinTree( const unsigned int height ); //Makes a binary three of the given height
	Graph* makeKn( const unsigned int n ); //Make a complete graph with n vertices
	Graph* makeRandomArityN( const unsigned int n, const unsigned int size ); //Make a graph with avarage degree n and contains size vertices
	Graph* makeSGrid( const unsigned int size ); //Make a square grid graph with a side of size vertices
	Graph* makeCGrid( const unsigned int size ); //Make a cube grid graph with a side of size vertices
	Graph* makeChain( const unsigned int length ); //Make a chain graph of length vertices
	Graph* makeParChain( const unsigned int x, const unsigned int length ); //Make a graph with x parallell chains
	Graph* makeTreeXY( const unsigned int x, const unsigned int y, const unsigned int num ); //Makes a tree structure with x children, who have y children. Repeats num times
	Graph* makePhaseXDY( const unsigned int x, const unsigned int y, const unsigned int v_in_f ); //Make a graph with x frontiers, whose vertices have degree y. v_in_f gives the number vertices in a frontier

private:
	GraphFactory();
	~GraphFactory();

	//make a chain of vertices. Add the chain directly into the given vector using the first element as the root
	void chain( const unsigned int length, std::vector<Vertex::vertex_ptr>& vertices );
	inline Vertex::vertex_ptr makeV(); //Construct a default vertex

	std::default_random_engine gen_;

	GraphFactory( GraphFactory const& ) = delete;
	void operator= ( GraphFactory fac ) = delete;
};

