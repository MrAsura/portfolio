//Implementations for the functions in the DFS namespace

#include "DFS.h"

#include <stack>
#include <vector>

namespace DFS
{
	//Main function that performs a DF search on given graph G
	void DFS( Graph& G, Vertex::vertex_ptr begin )
	{
		std::stack<Vertex::vertex_ptr> frontier; //Container to hold the vertices to be colored
		
		//Change color to make sure vertices are not colored yet
		Vertex::color color = G.changeColor(); //Save current color to check against the vertex color. Used for visited nodes
		
		//Start search from begin
		frontier.emplace( begin );

		//Main loop
		while( !frontier.empty() )
		{
			//Get next vertex and check if visited
			Vertex::vertex_ptr v = frontier.top();
			frontier.pop();

			if( !v->compareColor( color ) )
			{
				//Not visited. Add out edges to frontier and color v
				std::vector<Vertex::vertex_ptr> edges = v->edges();
				v->colorVertex( color );

				for( Vertex::vertex_ptr e : edges )
				{
					if(!e->compareColor(color)) frontier.push( e );
				}
			}
		}
	}



}