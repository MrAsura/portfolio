#pragma once

//Class that handeles work balancing for threads
//Allows to query for work and share if work is sent
//Also keeps track of idle (waiting) threads and calls a termination function if a idle threshold is reached
//Works for frontiers

#include "Frontier.h"

#include <functional>
#include <mutex>
#include <atomic>
#include <tuple>
#include <list>
#include <memory>
#include <thread>

class WorkBalancer
{
public:
	WorkBalancer( unsigned int idle_threshold, std::function<void(void)> termination_func );
	~WorkBalancer();

	void getWork( Frontier& work_cont ); //Function that blocks until request is served or rejected. Work_cont is a container for work items to be added to. If termination condition is reached, unblock all threads.
	bool hasWorkRequests(); //Return true if a thread is requesting work
	void sendWork( std::function<void(Frontier&)> share_func ); //Handeles work sharing by executing the given share function. Passes a frontier given in getWork as a parameter.

	void reset(); //Reset number of idle threads after termination.

private:

	inline bool terminate(); //Return true if termination condition is reached

	const unsigned int THRESHOLD; //When termination function is called
	const int WAIT = 10; //Wait 100ms between termination checks
	std::function<void( void )> term_func_; //function that is called if waiting threads exceed THRESHOLD

	typedef std::tuple<Frontier*, std::shared_ptr<std::condition_variable>,std::shared_ptr<bool>> work_item_t; //Type for work request items
	//Contains a pointer to the shared container and a condition_variable for waking up the requester when done

	void createWorkRequest(Frontier* shared); //Make a new work request. Lock until served.
	work_item_t getWorkRequest(); //Return a work request for serving

	void checkTermination(); //Function for checking the termination that can be run in a dedicated thread
	void startTermChecking(); //Start a thread to check termination. Thread is detached.

	//Handle stuff when we terminate
	void exitProcedure();

	std::mutex lock_; //Used as a general lock
	std::mutex exit_lock_; //Used when exiting

	std::mutex term_lock_; //Used by the termination checking thread
	std::condition_variable term_check_; //Used by the termination checking thread
	std::thread term_thread_; //Thread used for termination checking

	std::atomic<unsigned int> idle_num_; //Number of currently idle threads
	std::atomic<bool> terminal_; //Tells if execution has been terminated

	std::list<work_item_t> work_requests_; //Contain current work requests
};

