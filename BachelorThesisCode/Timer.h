#pragma once
#define TIMER_H

//Class for mesuring the time it takes to execute the given function

#include <string>
#include <chrono>
#include <utility>
#include <map>
#include <vector>
#include <functional>

class Timer
{
public:
	Timer();
	Timer( unsigned int reps );
	virtual ~Timer();

	void setReps( unsigned int n ); //set how many times the function is executed. Time is the average of all the executions.

	//Measure the execution time of the given function (avarage over reps_ repetitions). Save result to internal data structure
	//Can specify a named group the time is added to and a label for the measurement.
	template< class T, class ...Args > void timer( std::string group, std::string label, T func, Args&& ...args );
	
	//Save results to a file
	//Format is a ';' delimited table.
	//Times are given in milliseconds
	//If groups have differing lengths, smaller groups are appended with "" and -1
	void saveResults( std::string file );
	std::string makeTable(); //Construct a table from timer_groups_

private:
	unsigned int reps_; //How many times functions are executed to get a more accurate time

	//Hold the different time measurements and respective groups
	//A measurement consists of a label and a duration. Measurements are groupded by a string identifier.
	typedef std::pair<std::string, long long> timer_val;
	std::map<std::string, std::vector<timer_val>> timer_groups_;
	std::vector<std::string> group_names_; //Names of the groups (keys) contained in timer_groups_

	void addTime( std::string group, timer_val time );
};


//******************************************************************************************************************************
//
//
//Template functions have to be defined here
template<class T, class ...Args>
void Timer::timer( std::string group, std::string label, T func, Args&& ...args )
{
	auto final_time = std::chrono::high_resolution_clock::duration::zero();

	//Take the avarage time over reps_ executions
	for( unsigned int n = reps_; n > 0; --n )
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		func( args... );
		auto t2 = std::chrono::high_resolution_clock::now();

		final_time += ( t2 - t1 );
	}

	long long ms = std::chrono::duration_cast<std::chrono::milliseconds>( final_time ).count()/reps_;

	addTime( group, std::make_pair(label, ms) );
}
