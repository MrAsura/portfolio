//Implementations for the functions in the BFS namespace

#include "BFS.h"

#include <queue>
#include <vector>

namespace BFS
{
	//Main function that performs a BF search on given graph G
	void BFS( Graph& G, Vertex::vertex_ptr begin )
	{
		std::queue<Vertex::vertex_ptr> frontier; //Container to hold the vertices to be colored

		//Change color to make sure vertices are not colored yet
		Vertex::color grey = G.changeColor(); //Save current color to check against the vertex color. Visited color
		Vertex::color black = G.changeColor(); //Checked color

		//Start search from begin
		begin->colorVertex( grey );
		frontier.emplace( begin );

		//Main loop
		while( !frontier.empty() )
		{
			//Get next vertex and check if visited
			Vertex::vertex_ptr v = frontier.front();
			frontier.pop();

			//Add out edges to frontier and color v
			std::vector<Vertex::vertex_ptr> edges = v->edges();
			for( Vertex::vertex_ptr e : edges )
			{
				if( !e->compareColor( grey ) && !e->compareColor( black ) )
				{
					e->colorVertex( grey );
					frontier.push( e );
				}
			}

			v->colorVertex( black );
		}
	}
}