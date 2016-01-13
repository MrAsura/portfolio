#include "WorkBalancer.h"
#include <cassert>
#include <chrono>


WorkBalancer::WorkBalancer( unsigned int idle_threshold, std::function<void( void )> termination_func ) : THRESHOLD( idle_threshold ), term_func_( termination_func ), lock_(), exit_lock_(), term_lock_(), term_check_(), term_thread_(), idle_num_(0), terminal_(false), work_requests_(0)
{
	//Start the termination checking thread
	startTermChecking();
}

WorkBalancer::~WorkBalancer()
{
	exitProcedure();
	term_thread_.join();
}

void WorkBalancer::getWork( Frontier & work_cont )
{
	//Increase idle count
	idle_num_++;

	if( terminate() )
	{
		term_check_.notify_all();
	}

	createWorkRequest( &work_cont );

	idle_num_--;
}

bool WorkBalancer::hasWorkRequests()
{
	return idle_num_.load() > 0;
}

void WorkBalancer::sendWork( std::function<void( Frontier& )> share_func )
{
	//Get a work item and service it
	work_item_t item = getWorkRequest();

	//If first value is nullptr, no work requests
	if( std::get<0>( item ) == nullptr ) return;

	Frontier* shared = std::get<0>( item );
	
	//assert( shared->isEmpty() );
	//assert( work_requests_.size() < THRESHOLD );
	
	share_func( *shared );

	//Work shared, notify requester
	std::shared_ptr<bool> done = std::get<2>( item );
	*done = true;
	std::shared_ptr<std::condition_variable> cond = std::get<1>( item );
	cond->notify_one();
}

void WorkBalancer::reset()
{
	if( terminal_.load() )
	{
		idle_num_.store( 0 );
		terminal_.store( false );
		work_requests_.clear();
		startTermChecking();
	}
}

inline bool WorkBalancer::terminate()
{
	return THRESHOLD <= idle_num_.load();
}


void WorkBalancer::createWorkRequest( Frontier* shared )
{
	std::unique_lock<std::mutex> work_lock( lock_ );
	
	//Create work item
	std::shared_ptr<std::condition_variable> cond = std::make_shared<std::condition_variable>();
	std::shared_ptr<bool> done = std::make_shared<bool>( false ); //Tells if service is done. Server should update.
	work_requests_.emplace_back(shared,cond,done);
	
	//assert( shared->isEmpty() );
	//assert( work_requests_.size() < THRESHOLD );
	
	//Wait for service
	while( !terminal_.load() && !*done )
	{
		cond->wait( work_lock );
	}
}

WorkBalancer::work_item_t WorkBalancer::getWorkRequest()
{
	std::lock_guard<std::mutex> get_lock( lock_ );

	//If nothing in queue, return dummy data
	if( work_requests_.empty() ) return work_item_t(nullptr, std::make_shared<std::condition_variable>(), std::make_shared<bool>( false ));

	//Retreave the next work item and return it
	work_item_t item = work_requests_.front();
	work_requests_.pop_front();
	return item;
}

void WorkBalancer::checkTermination()
{
	std::unique_lock<std::mutex> lock( term_lock_ );
	while( !terminal_ )
	{
		term_check_.wait_for( lock, std::chrono::milliseconds( WAIT ) );

		if( terminate() && THRESHOLD <= work_requests_.size() )
		{
			exitProcedure();
			return;
		}
	}
}

void WorkBalancer::startTermChecking()
{
	term_thread_ = std::thread( [this] {
		this->checkTermination();
	});
}

void WorkBalancer::exitProcedure()
{
	std::unique_lock<std::mutex> lock( exit_lock_, std::defer_lock );
	if( lock.try_lock() )
	{
		term_func_();
		terminal_.store( true );

		std::lock_guard<std::mutex> guard( lock_ );

		//Release remaining work requesters
		for( work_item_t item : work_requests_ )
		{
			std::shared_ptr<bool> done = std::get<2>( item );
			*done = true;
			std::shared_ptr<std::condition_variable> cond = std::get<1>( item );
			cond->notify_all();
		}

		//work_requests_.clear();
		term_check_.notify_all();
	}
}
