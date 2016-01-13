#pragma once

#include "Graph.h"

//Header for the Parallel Deapth first search
//Functions are for performance test purposes so they just color the whole graph and don't try to find a specific vertex
//Performs a parallel search. Note: Calling PDFS parallelly (on the same G) is not quaranteet to work. 

namespace PDFS
{
	void PDFS( Graph& G, Vertex::vertex_ptr begin );
	void setNumOfThreads( unsigned int n ); //Set how many threads are used when PDFS is called
	void setChunkSize( std::size_t size ); //Change the size of the chunk used. Only applies to frontiers created after calling this.
	void setPollingCO( std::size_t size );
	void setCutOff( std::size_t size );
}