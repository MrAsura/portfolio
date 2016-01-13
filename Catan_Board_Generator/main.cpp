//Main function for mat-72006 programming exercise
//Made by Jaakko Laitinen - 229785

#include <cstdlib>
#include <iostream>

#include "CatanBoard.h"

#define DEBUG

int main()
{
	//main will wait for input from user
	//if user returns a negative number exit
	//else first number is the number of test inputs
	//following numbers are used as the input for nthTileType(n)

	int val = 0;
	CatanBoard board;

	while( std::cin >> val )
	{
		if( val < 0 )
		{
			break;
		}

		for( int i = val; i > 0; --i )
		{
			int val2 = 0;
			std::cin >> val2;

			if( val2 < 0 )
			{
				continue;
			}

			std::cout << board.nthTileType( val2 ) << std::endl;

			//For debuging
			#ifdef DEBUG	
			for( int type : board.printTilesT() )
			{
				std::cout << type << ' ';
			}

			std::cout << std::endl;
			#endif
		}
	}

	return EXIT_SUCCESS;
}