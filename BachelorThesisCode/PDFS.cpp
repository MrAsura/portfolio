#include "PDFS.h"
#include "Frontier.h"
#include "WorkBalancer.h"

#include <vector>
#include <thread>
#include <atomic>
#include <cassert>

// Implementation of a parallel DFS as proposed by U. Acar et al.


namespace PDFS
{
	inline namespace
	{
		static unsigned int THREAD_NUM = std::thread::hardware_concurrency()-1; // 12/6
		static std::size_t CHUNK_SIZE = 32; // 32 //
		static std::size_t POLLING_CUTOFF = 10; // How many elements are handeled before checking for work requests
		static std::size_t CUTOFF = 256; // 256 // Minimum amount of work that should be done before splitting

		static std::atomic<bool> traversal_not_complete = true;

		void terminate()
		{
			traversal_not_complete.store(false);
		}

		void pdfs_thread( std::shared_ptr<Frontier> fr, Vertex::color c, WorkBalancer* wb );
	}

	void PDFS( Graph & G, Vertex::vertex_ptr begin )
	{
		//Get target color and initialize First frontier
		Vertex::color c = G.changeColor(); //Change color so graph isn't already colored
		std::shared_ptr<Frontier> fr = std::make_shared<Frontier>( CHUNK_SIZE ); //Initial frontier with only the begin vertex

		//Check begin vertex color and add its edges to the frontier (not parallele yet)
		//This should be the first searched vertex so color it and add its edges to the frontier
		//It can't be visited yet.
		begin->colorVertex( c );
		fr->pushEdgesOf( begin );

		traversal_not_complete.store( true ); //Reset traversal flag for next execution

		//start other threads
		std::vector<std::thread> threads( 0 );

		//Class for handling workbalancing
		//threads plus this thred
		WorkBalancer wb( THREAD_NUM+1, terminate );

		for( unsigned int thread = 1; thread <= THREAD_NUM; thread++ )
		{
			threads.emplace_back( std::thread(pdfs_thread, std::make_shared<Frontier>(CHUNK_SIZE), c, &wb) );
		}

		//Start the search on fr
		pdfs_thread( fr, c, &wb );

		//wait for other threads to terminate
		for( std::thread& thread : threads )
		{
			thread.join();
		}

		//wb.reset();
		
	}

	void setNumOfThreads( unsigned int n )
	{
		THREAD_NUM = n;
	}

	void setChunkSize( std::size_t size )
	{
		CHUNK_SIZE = size;
	}

	void setPollingCO( std::size_t size )
	{
		POLLING_CUTOFF = size;
	}

	void setCutOff( std::size_t size )
	{
		CUTOFF = size;
	}

	
	//Create an anonymous namespace to hide aux. functions and variables 
	inline namespace
	{
		//Main function executed by the threads
		void pdfs_thread( std::shared_ptr<Frontier> fr, Vertex::color c, WorkBalancer* wb )
		{
			std::size_t nb = 0; //Items handeled

			//Loop while graph is not traversed
			while( traversal_not_complete.load() )
			{
				if( fr->isEmpty() )
				{
					//No work so get more
					nb = 0;
					wb->getWork( *fr );
				}
				else
				{
					//Check if work requests exist. If not, continue working
					if( wb->hasWorkRequests() )
					{
						//Make sure there is enoug work to share and atleas cutoff items have been processed
						std::size_t size = fr->numOfEdges();

						if( size > CUTOFF || ( nb > CUTOFF && size > 1 ) )
						{
							wb->sendWork( [&fr]( Frontier& other )
							{
								fr->split( other );
							} );

							nb = 0;
						}
					}

					//Do work until polling cutoff is reached
					nb += fr->iterPopOverNb( POLLING_CUTOFF, [&fr,&c]( Vertex::vertex_ptr parent, std::size_t edge )
					{
						Vertex::vertex_ptr v = parent->edgeAt( edge );

						if( !v->colorCompare( c ) )
						{
							assert( v->colorCompare( c ) );
							fr->pushEdgesOf( v );
						}
					} );
				}
			}

			//If fr is not empty. We need to finnish it (premature termination)
			/*while( !fr->isEmpty() )
			{
				fr->iterPopOverNb( fr->numOfEdges(), [&fr, &c]( Vertex::vertex_ptr parent, std::size_t edge )
				{
					Vertex::vertex_ptr v = parent->edgeAt( edge );

					if( !v->colorCompare( c ) )
					{
						assert( v->colorCompare( c ) );
						fr->pushEdgesOf( v );
					}
				} );
			}*/
			if( !fr->isEmpty() ) throw "Frontier not empty";
			assert( fr->isEmpty() );
		}
	}
}