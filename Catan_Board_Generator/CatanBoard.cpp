//Implementation for CatanBoard
//Made by Jaakko Laitinen - 229785

#include "CatanBoard.h"

#include <cmath>
#include <limits>


CatanBoard::CatanBoard(): tiles_(0), typeDistribution_(INITDISTR)
{
	//initialize class by adding the trivial center tile
	Tile tile = { 1,0,true,std::vector<ind_t>( 0 ) };
	tiles_.push_back(tile );
	typeDistribution_.at( 0 ) = 1;
}


CatanBoard::~CatanBoard()
{
}

int CatanBoard::nthTileType( unsigned int n )
{
	//If there are n ore more tile, return type
	if( n <= tiles_.size() )
	{
		return typeOf( n - 1 ); // indexing starts from 0 so nth tile has index n-1
	}

	//Add enough tiles to contain the nth tile
	while( n > tiles_.size() )
	{
		makeTile(); //tiles_.size() should increase every loop
	}

	return typeOf( n -1 );
}

std::vector<int> CatanBoard::printTilesT()
{
	std::vector<int> types(0);

	for( Tile t : tiles_ )
	{
		types.push_back( t.type );
	}

	return types;
}

void CatanBoard::makeTile()
{
	//Initialize tile
	Tile tile;
	tile.type = UNDEFINED_TYPE; //Type is determined lazily when needed. (a pit pointles since all previous values are needed anyway before computing type)

	//Determine basic information from tile
	std::size_t ind = tiles_.size(); //The index of the new tile
	
	level_t level = getLevel( ind );
	tile.level = level;
	
	bool is_diag = isDiagonal( ind, level );
	tile.is_diagonal = is_diag;
	
	tile.neighbors = findNeighbors( ind, level, is_diag);
	
	//Finaly add tile to the list
	tiles_.push_back(tile);

	//To prevent stack overflow from recursion when n is large, determine type right away
	typeOf( ind );
}

CatanBoard::level_t CatanBoard::getLevel( ind_t ind )
{
	//If ind is 0 we return 0
	if( ind == 0 )
	{
		return 0;
	}

	//We can estimate the level by solving the equation 1+3*n*(n-1) = ind (from tilesBeforeLevel)
	//We get n = 0.5+sqrt(12*ind-3)/6
	//If we take the floor it gives us a good starting point
	//The level is the n for which tilesBeforeLevel(n) <= ind < tilesBeforeLevel(n+1)
	
	level_t level = static_cast<level_t>(std::floor( 0.5 + std::sqrt( 12 * ind - 3 ) / 6 ));
	
	while( !(tilesBeforeLevel( level ) <= ind && ind < tilesBeforeLevel( level + 1 )) )
	{
		if( tilesBeforeLevel( level + 1 ) <= ind )
		{
			++level;
		}
		else
		{
			--level;
		}
	}


	return level;
}

bool CatanBoard::isDiagonal( ind_t ind, level_t level )
{
	//Center one is always a diagonal (ind=0)
	if( ind == 0 )
	{
		return true;
	}

	//There are 6 diagonals per level (level>0) at equal intervals and a diagonal has 6*n tiles so we can use mod(ind,n)==0 to check the diagonality.
	//The way a new level starts, the first tile in a level is always the one after a diagonal (since in the first level the only tile adjacent to the end (diagonal) tile and start (diagonal) tile of the level is a non-diagonal tile). The round always end in a diagonal so the next leves first is again the first non-diagonal after the diagonal of that level.
	//The first index of the level is 1 + 3*(level-1)*level (level>0) (indexing starts at 0) and the last 6*level-1 so the last one is a diagonal and mod(ind,level)==0 holds.
	//nth tile in level has index n+3*(level-1)*level so the multibles of level are diagonal. This is consistent with visual inspection

	if( ( ind % level ) == 0 )
	{
		return true;
	}

	return false;
}

CatanBoard::ind_t CatanBoard::indInLeve( ind_t ind, level_t level )
{
	//Ind is ind - number of tiles before inds level
	return ind - tilesBeforeLevel(level);
}

CatanBoard::ind_t CatanBoard::fromLevelInd( ind_t level_ind, level_t level )
{
	//If level is 0 the only possible ind is 0.
	if( level == 0 )
	{
		return 0;
	}

	//Actual ind is the number of tiles before this level plus level ind
	return level_ind + tilesBeforeLevel(level);
}

CatanBoard::ind_t CatanBoard::tilesInLevel( level_t level )
{
	//If level is 0 then number of tiles is 1
	if( level == 0 )
	{
		return 1;
	}

	//Each level has 6 diagonals, since the center tile is hexagonal and has 6 sides.
	//Additionally there are n-1 tiles between adjacent diagonals where n is the level
	//In total nth level has 6+6*(n-1) = 6(1+n-1) = 6n
	
	return static_cast<ind_t>(6*level);
}

CatanBoard::ind_t CatanBoard::tilesBeforeLevel( level_t level )
{
	//if level is 0 return 0
	if( level == 0 )
	{
		return 0;
	}

	//if each level has 6n tiles, the total number tiles before level is the 1 + 6*1 + 6*2 + ... + 6*(level-1)
	//We get 1 + 6*sum(1,2,3,...,level-1) = 1+6*( (level-1)(1+(level-1))/2 ) = 1 + 3*(level-1)*level
	return static_cast<ind_t>(1+3*level*(level-1));
}

CatanBoard::ind_t CatanBoard::nonDiagBefore( ind_t level_ind, level_t level )
{
	//If we divide level_ind (number of tiles before the one pointed to by level_ind (indexing starts from 0)) by level we get level_ind = level*x + (level_ind) % level. x is the number of diagonals before level_ind since a level has 6*level tiles and 6 diagonals so 6 segment of size level with the diagonal being the levelth tile (see isDiagonal() ).
	//Thus x is the number of diagonals and we get the number of non-diagonal tiles as level_ind - (level_ind-((level_ind) % level))/level 
	return level_ind - (level_ind - (level_ind % level ))/level;
}

std::vector<CatanBoard::ind_t> CatanBoard::findNeighbors( ind_t ind, level_t level, bool is_diag)
{
	//A tile has only one preceding neighbor in the same level (ind-1) except the first one which has none
	//For the last diagonal in the level (also last tile), the first tile of the level is a neighbor
	std::vector<ind_t> neighbors(0);
	ind_t level_ind = indInLeve(ind,level);
	
	if( level_ind != 0 )
	{
		neighbors.push_back(ind-1);

		if( level_ind == tilesInLevel( level ) - 1 )
		{
			neighbors.push_back( fromLevelInd( 0, level ) );
		}
	}
	 
	//Other preceding neighbors can only be in the previous level. Non diagonal tiles have two and diagonal tiles one (visualy apparrent).
	//Prev level has 6 tiles less so the indexing has to reflect that. If we count only the non diagonal tiles in level before level_ind we get a value that corresponds to the level_ind of level-1 and is a tile that is adjacent to ind-tile.
	ind_t non_diag_before = nonDiagBefore( level_ind, level ); 
	
	if( is_diag )
	{
		//non_diag_before corresponds to the tile in level-1 that is after the diagonal ind-tile is adjacent to so we use non_diag_before-1 and convert the level ind to the actual ind
		neighbors.push_back( fromLevelInd(non_diag_before-1,level-1) );
	}
	else if( level_ind == 0 )
	{
		//The first tile in the level has the last tile in level-1 as a neighbor so the other index needs to be the one for the last index of level-1
		neighbors.push_back( fromLevelInd(non_diag_before, level-1));
		neighbors.push_back( fromLevelInd( tilesInLevel(level-1)-1, level-1));
	}
	else
	{
		//non_diag_before corresponds to the second of the two adjacent tiles in level-1
		neighbors.push_back( fromLevelInd(non_diag_before-1,level-1) ); 
		neighbors.push_back( fromLevelInd(non_diag_before, level-1));
	}
	
	return neighbors;
}

int CatanBoard::typeOf( ind_t ind )
{
	Tile t = tiles_.at( ind );
	if( t.type != UNDEFINED_TYPE )
	{
		return t.type;
	}

	//Evaluate the type and update typeDistribution
	int type = evaluateType( ind );
	++typeDistribution_.at( type - 1 );

	//Update tile to tiles_
	t.type = type;
	tiles_.at( ind ) = t;

	return type;
}

int CatanBoard::evaluateType( ind_t ind )
{
	//Define vector of possible types. Assume each type is only once in possible and types are in asscending order
	std::vector<int> possible( TYPES.size() ); 
	std::copy( TYPES.begin(), TYPES.end(), possible.begin() );
	
	//Lampda function for removing items from possible whose values match x
	auto remove_match = [&possible]( int x ) {
		for( auto it = possible.begin(); it != possible.end(); ++it )
		{
			if( *it == x )
			{
				possible.erase( it );
				break;
			}
		}
	};

	//Get types we should skip
	for( ind_t tile : tiles_.at( ind ).neighbors )
	{
		int remove_type = typeOf( tile );
		
		//Remove types that match neighbors type
		remove_match( remove_type );
	}

	int min_oc = std::numeric_limits<int>::max();

	//typeDistributions indexes correspond to the tile_type-1 and possible contains possible types
	//Thus we only try to find the min from those types
	for( int possible_ind : possible )
	{
		if( typeDistribution_.at(possible_ind-1) < min_oc )
		{
			min_oc = typeDistribution_.at(possible_ind-1);
		}
	}

	//from those that are still possible we take the ones with the minimum number of occurances
	for( int i = 0; i < typeDistribution_.size(); ++i )
	{
		if( typeDistribution_.at( i ) > min_oc || typeDistribution_.at( i ) < min_oc )
		{
			//Remove types that dont have min_oc number of occurances
			remove_match( i + 1 );
		}
	}

	//Returning the first element of possible should satisfy all the requirements since possible is in asscending order
	return possible.at( 0 );
}

