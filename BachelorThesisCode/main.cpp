//Main function for the project. Run test etc. here

#include <cstdlib>
#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <chrono>

#include "GraphFactory.h"
#include "BFS.h"
#include "DFS.h"
#include "PDFS.h"
#include "Timer.h"
#include "Range.h"
#include "WeightedQueue.h"

//#define NDEBUG
#include <cassert>

//Control test that are ran
#define test
//#define param
//#define other
#define stop

inline void writeToFile( std::size_t num, std::string file )
{
	//Try to open file. Create if doesn't exist
	std::fstream fs;
	fs.open( file, std::fstream::out | std::fstream::app );

	if( fs )
	{
		fs << num;
		fs << ' ';
	}

	fs.close();
}

inline void resetFile( std::string file )
{
	//try to clear file content or create empty file
	std::fstream fs;
	fs.open( file, std::fstream::out | std::fstream::trunc );
	fs.close();
}

inline void runTests( Graph& G, std::string name, std::string sfile, Timer& t )
{
	std::cout << name + ": " << G.size() << " vertices" << std::endl;
	writeToFile( G.size(), sfile );

	t.timer( "BFS", name, BFS::BFS, G, G.first() );
	//BFS::BFS( G, G.first() );
	std::cout << "BFS: " << G.isGraphSameColor( G.curColor() ) << " (" + name + ")" << std::endl;

	t.timer( "DFS", name, DFS::DFS, G, G.first() );
	//DFS::DFS( G, G.first() );
	std::cout << "DFS: " << G.isGraphSameColor( G.curColor() ) << " (" + name + ")" << std::endl;

	t.timer( "PDFS", name, PDFS::PDFS, G, G.first() );
	//PDFS::PDFS( G, G.first() );
	std::cout << "PDFS: " << G.isGraphSameColor( G.curColor() ) << " (" + name + ")" << std::endl;

	assert( G.isGraphSameColor( G.curColor() ) );
}

inline void graphTests()
{
	//int scale = 1;
	Timer t( 20 );
	std::string file = std::string("C:\\Users\\Jaakko\\Documents\\MATLAB\\Kandi\\data.txt");
	std::string sfile = std::string( "C:\\Users\\Jaakko\\Documents\\MATLAB\\Kandi\\sdata.txt" );

	resetFile( sfile );

	//Test graphs and algorithm
	//RandomArity
	unsigned int target = static_cast<unsigned int>(1e6);

	std::unique_ptr<Graph> G( GraphFactory::getInstance().makeRandomArityN( 3, target ) );
	std::string name = "Satunnainen-aste-3";
	runTests( *G, name, sfile, t );

	G = std::unique_ptr<Graph> ( GraphFactory::getInstance().makeRandomArityN( 8, target ) );
	name = "Satunnainen-aste-8";
	runTests( *G, name, sfile, t );

	G = std::unique_ptr<Graph>( GraphFactory::getInstance().makeRandomArityN( 100, target ) );
	name = "Satunnainen-aste-100";
	runTests( *G, name, sfile, t );
	
	//Chain
	target = static_cast<unsigned int>( 1e7 );

	G = std::unique_ptr<Graph>( GraphFactory::getInstance().makeChain( target ) );
	name = "Ketju";
	runTests( *G, name, sfile, t );

	//ParChain
	G = std::unique_ptr<Graph>( GraphFactory::getInstance().makeParChain( 8, target/8 ) );
	name = "Rin-Ketju-8";
	runTests( *G, name, sfile, t );

	G = std::unique_ptr<Graph>( GraphFactory::getInstance().makeParChain( 20, target/20 ) );
	name = "Rin-Ketju-20";
	runTests( *G, name, sfile, t );

	G = std::unique_ptr<Graph>( GraphFactory::getInstance().makeParChain( 100, target/100 ) );
	name = "Rin-Ketju-100";
	runTests( *G, name, sfile, t );

	G = std::unique_ptr<Graph>( GraphFactory::getInstance().makeParChain( 10000, target/10000 ) );
	name = "Rin-Ketju-10k";
	runTests( *G, name, sfile, t );
	
	//TreeX
	target = static_cast<unsigned int>( 1e7 );

	G = std::unique_ptr<Graph>( GraphFactory::getInstance().makeTreeXY( 50000, 0, target/50000 ) );
	name = "Puut-50k";
	runTests( *G, name, sfile, t );

	//TreeXY
	G = std::unique_ptr<Graph>( GraphFactory::getInstance().makeTreeXY( 1000, 1000, target/(1000*1000) ) );
	name = "Puut-1k-1k";
	runTests( *G, name, sfile, t );

	G = std::unique_ptr<Graph>( GraphFactory::getInstance().makeTreeXY( 64, 64, target / ( 64 * 64 ) ) );
	name = "Puut-64-64";
	runTests( *G, name, sfile, t );

	G = std::unique_ptr<Graph>( GraphFactory::getInstance().makeTreeXY( 64, 128, target / ( 64 * 128 ) ) );
	name = "Puut-64-128";
	runTests( *G, name, sfile, t );

	//Binary tree
	target = static_cast<unsigned int>( 1e7 );

	G = std::unique_ptr<Graph>(GraphFactory::getInstance().makeBinTree(static_cast<unsigned int>(log2(target+1))));
	name = "Bin‰‰ripuu";
	runTests( *G, name, sfile, t );

	//Phases-x-d-y
	target = static_cast<unsigned int>( 1e7 );

	G = std::unique_ptr<Graph>( GraphFactory::getInstance().makePhaseXDY( 50, 5, target/50 ) );
	name = "Vaihe-50-d-5";
	runTests( *G, name, sfile, t );
	
	G = std::unique_ptr<Graph>( GraphFactory::getInstance().makePhaseXDY( 10, 2, target / 10 ) );
	name = "Vaihe-10-d-2";
	runTests( *G, name, sfile, t );

	//Kn
	target = static_cast<unsigned int>( 10000 );

	G = std::unique_ptr<Graph>(GraphFactory::getInstance().makeKn( target ));
	name = "$K_{10k}$";
	runTests( *G, name, sfile, t );

	//std::cout << t.makeTable() << std::endl;

	//Save results into a data file
	t.saveResults( file );

/*
	Range r1(G->first(),0,G->first()->degree());
	Range r2;
	r1.splitAt( bin / 2, r2 );

	std::cout << r1.weight() << std::endl << r2.weight() << std::endl;

	std::cout << r1.iterPopOverNb( 5, []( Vertex::vertex_ptr v, std::size_t ind ) {
		std::cout << "Degree: " << v->edgeAt( ind )->degree() << std::endl;
	} ) << std::endl;*/
}

inline void WQTests( std::size_t loops, std::size_t chunk_size = 1 )
{
	WeightedQueue<Vertex::vertex_ptr> wq(chunk_size);

	for( std::size_t n = 1; n <= loops; ++n )
	{
		wq.push_back( Vertex::vertex_ptr(), n );
		wq.print();
	}
	std::cout << "Weight: " << wq.weight() << std::endl;
}

inline void OSTreeTest( std::size_t loops, std::size_t chunk_size = 1 )
{
	WeightedQueue<Vertex::vertex_ptr> wq( chunk_size );

	for( std::size_t n = 1; n <= loops; ++n )
	{
		wq.push_back( Vertex::vertex_ptr(), 1 );
		wq.print();
	}
	std::cout << "Weight: " << wq.weight() << std::endl;
}

inline void WQPopTests( std::size_t loops, std::size_t chunk_size = 1 )
{
	WeightedQueue<Vertex::vertex_ptr> wq( chunk_size );

	for( std::size_t n = 1; n <= loops; ++n )
	{
		wq.push_back( Vertex::vertex_ptr(), n );
		
	}
	wq.print();
	for( std::size_t n = 1; n <= loops; ++n )
	{
		wq.pop_front();
		wq.print();
	}

}

inline void SplitCatTest( std::size_t weight, std::size_t loops, std::size_t chunk_size = 1 )
{
	WeightedQueue<Vertex::vertex_ptr> wq1( chunk_size );
	WeightedQueue<Vertex::vertex_ptr> wq2( chunk_size );

	for( std::size_t n = 1; n <= loops; ++n )
	{
		wq1.push_back( Vertex::vertex_ptr(), n );

	}
	wq1.print();
	std::cout << "Weight1: " << wq1.weight() << std::endl;

	Vertex::vertex_ptr v;
	wq1.splitAt( weight, v, wq2 );

	wq1.print();
	wq2.print();

	std::cout << "Weight1: " << wq1.weight() << std::endl;
	std::cout << "Weight2: " << wq2.weight() << std::endl;

	wq1.concat( wq2 );

	wq1.print();
	wq2.print();

	std::cout << "Weight1: " << wq1.weight() << std::endl;
}

void paramFinder()
{
	//Try find best parameters for the PDFS
	std::size_t chunk_m = 1;
	std::size_t p_cutoff_m = 1;
	std::size_t cutoff_m = 1;
	auto G = GraphFactory::getInstance().makeBinTree(10);
	auto min_time = std::chrono::high_resolution_clock::duration::zero();

	for( size_t i = chunk_m; i <= 64; ++i )
	{
		PDFS::setChunkSize( i );
		for( size_t n = p_cutoff_m; n < 100; n++ )
		{
			PDFS::setPollingCO( n );
			for( size_t k = p_cutoff_m; k < 512; k++ )
			{
				PDFS::setCutOff( k );

				auto t1 = std::chrono::high_resolution_clock::now();
				PDFS::PDFS( *G, G->first() );
				auto t2 = std::chrono::high_resolution_clock::now();

				if( min_time > ( t2 - t1 ) )
				{
					min_time = t2 - t1;
					chunk_m = i;
					p_cutoff_m = n;
					cutoff_m = k;
				}

				std::cout << "Chunk: " << i << ", Polling cutoff: " << n << ", cutoff: " << k << ", time: " << (t2 - t1).count() << std::endl;
			}

		}
	}

	std::cout << "Max Chunk: " << chunk_m << ", Max Polling cutoff: " << p_cutoff_m << ", Max cutoff: " << cutoff_m << std::endl;
}

int main()
{
	std::string msg = "";
#ifdef test

	graphTests();

#endif // test
#ifdef param

	paramFinder();

#endif // param

#ifdef other
	WQTests( 15 );

	std::cout << "next?" << std::endl;
	std::cin >> msg;

	WQTests( 15, 5 );

	std::cout << "next?" << std::endl;
	std::cin >> msg;

	OSTreeTest( 15 );

	std::cout << "next?" << std::endl;
	std::cin >> msg;

	OSTreeTest( 15, 5 );

	std::cout << "next?" << std::endl;
	std::cin >> msg;

	WQPopTests( 13 );

	std::cout << "next?" << std::endl;
	std::cin >> msg;

	WQPopTests( 15, 2 );

	std::cout << "next?" << std::endl;
	std::cin >> msg;

	SplitCatTest(40, 13 );

	std::cout << "next?" << std::endl;
	std::cin >> msg;

	WQTests( 15, 2 );
	SplitCatTest(40, 15, 2 );

#endif // other

#ifdef stop
	std::cout << "Yes?" << std::endl;
	std::cin >> msg;
#endif // stop


	return EXIT_SUCCESS;
}


