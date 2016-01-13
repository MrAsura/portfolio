#pragma once
//Class for generating the catan style board given in the mat-72006 programming exercise
//Made by Jaakko Laitinen - 229785

#include <vector>
#include <array>

#define TYPENUM 5

class CatanBoard
{
public:
	CatanBoard();
	~CatanBoard();

	//Function that returns the type of the nth tile
	int nthTileType( unsigned int n );
	std::vector<int> printTilesT(); //Prints the types of all the tiles currently stored. Used for debugging

private:

	//Convinience typedefs
	typedef std::size_t ind_t;
	typedef unsigned int level_t;

	const int UNDEFINED_TYPE = -1;
	const std::array<int, TYPENUM> TYPES{ { 1,2,3,4,5 } };
	const std::array<int, TYPENUM> INITDISTR{ { 0,0,0,0,0 } };

	//Custom type for representig a tile
	struct Tile
	{
		int type; //Type of tile (1-5) or undefined
		level_t level; //Level/ring the tile is a part of. First center tile is level 0. The tiles surrounding it are level 1 etc.
		bool is_diagonal; //Diagonal tiles have one of their edges alligned with the center tile. (In a straight line).

		//Contain all the neighbors (tiles touching this one) that would have been added before this one
		// and thus determine the type
		std::vector<ind_t> neighbors; 
	};

	//Make a new tile and add it to tiles_. This becomes the next tile (nth tile, where n is tiles_.size()+1. Index is tiles_size()).
	void makeTile();

	level_t getLevel( ind_t ind ); //Return the level of a tile with given ind. Calculates the level based on the ind. Use Tile.level if Tile knows its level

	bool isDiagonal( ind_t ind, level_t level ); //Return whether ind is diagonal or not. Use Tile.is_diagonal if defined. Need to know tiles level.
	
	ind_t indInLeve( ind_t ind, level_t level ); //Return the index in the tiles level for given ind. ind has to be in level
	
	ind_t fromLevelInd( ind_t level_ind, level_t level); //Return the actual ind from level ind 
	
	ind_t tilesInLevel( level_t level ); //Return the number of tile that fit in given level
	
	ind_t tilesBeforeLevel( level_t level ); //Return number of tiles in all levels before level
	
	ind_t nonDiagBefore( ind_t level_ind, level_t level ); //Return the number of non diagonal tiles before level_ind in level.


	std::vector<ind_t> findNeighbors( ind_t ind, level_t level, bool is_diag ); //Calculate the neighbors for the given parameters. Use Tile.neighbors if defined.

	//Return the type of the tile in tiles_.at(ind). If type is undefined, defines it.
	//Use this instead of Tile.type
	int typeOf( ind_t ind );
	int evaluateType( ind_t ind ); //Evalueates what the type should be

	//Vector containing tiles needed thus far
	std::vector<Tile> tiles_;
	std::array<int, TYPENUM> typeDistribution_; //Contains the numbers of different types of tiles
};

