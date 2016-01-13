#pragma once

#include "Graph.h"

//Define available function in the BFS-namespace
//Function used for Breath first search on a given graph
//Functions are for performance test purposes so they just color the whole graph and don't try to find a specific vertex

namespace BFS
{
	void BFS( Graph& G, Vertex::vertex_ptr begin );
}