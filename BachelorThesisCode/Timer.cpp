#include "Timer.h"

#include <sstream>
#include <fstream>

Timer::Timer(): Timer(30)
{
}

Timer::Timer( unsigned int reps ): reps_(reps), timer_groups_(), group_names_(0)
{
}


Timer::~Timer()
{
}

void Timer::setReps( unsigned int n )
{
	reps_ = n;
}


void Timer::saveResults( std::string file )
{
	//Try to open file. Create if doesn't exist
	std::fstream fs;
	fs.open( file, std::fstream::out );

	if( fs )
	{
		fs << makeTable();
	}

	fs.close();
}

std::string Timer::makeTable()
{
	if( group_names_.empty() )
	{
		return std::string();
	}

	//Use to construct the table
	std::stringstream ss;
	ss << ( group_names_.at( 0 ) + "_label" + ";"+ group_names_.at( 0 ) + "_result" );
	
	std::size_t max_row = timer_groups_.at( group_names_.at( 0 ) ).size(); //store maximum number of results
	std::string last_name = group_names_.at( 0 );

	//Make group name row
	for( int ind = 1; ind < group_names_.size(); ++ind )
	{
		ss << ";" << group_names_.at( ind ) + "_label" + ";" + group_names_.at(ind) + "_result";

		if( max_row < timer_groups_.at( group_names_.at( ind ) ).size() )
		{
			max_row = timer_groups_.at( group_names_.at( ind ) ).size();
		}
		
		last_name = group_names_.at( ind );
	}

	ss << std::endl;

	//Make other rows
	for( int ind = 0; ind < max_row; ind++ )
	{
		//Add a line of each of the greaps
		for( std::string g_name : group_names_ )
		{
			if( timer_groups_.at( g_name ).size() <= ind )
			{
				//filler
				ss << "" << ";" << -1;
			}
			else
			{
				ss << timer_groups_.at( g_name ).at( ind ).first << ";" << timer_groups_.at( g_name ).at( ind ).second;
			}
			if( g_name != last_name )
			{
				ss << ";";
			}
		}

		ss << std::endl; 
	}

	return ss.str();
}

void Timer::addTime( std::string group, timer_val time )
{
	std::vector<timer_val> timers(0);

	//Try to find group
	try
	{
		timers = timer_groups_.at( group );
		timers.push_back( time );
	}
	catch( std::out_of_range er )
	{
		//No existing group -> add
		timers.push_back( time );
		timer_groups_.insert( std::make_pair(group, timers) );
		group_names_.push_back( group );

		return;
	}

	//Add updated timers to container
	std::swap( timer_groups_.at( group ), timers );

}
