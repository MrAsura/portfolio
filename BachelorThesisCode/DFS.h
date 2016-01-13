#pragma once

#include "Graph.h"

//Define available function in the DFS-namespace
//Function used for Depth first search on a given graph
//Functions are for performance test purposes so they just color the whole graph and don't try to find a specific vertex

namespace DFS
{
	void DFS( Graph& G, Vertex::vertex_ptr begin );
}