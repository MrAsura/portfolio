#include "Range.h"
#include <algorithm>
#include <cassert>

Range::Range(): Range(nullptr)
{
}

Range::Range( Vertex::vertex_ptr v, std::size_t beg, std::size_t end ): vertex_(v), beg_(beg), end_(end)
{
	invariant();
}

Range::~Range()
{
}

std::size_t Range::weight()
{
	return end_-beg_;
}

void Range::splitAt( std::size_t weight, Range & other )
{
	invariant();
	other.invariant();

	other.vertex_ = vertex_;
	other.beg_ = beg_ + weight;
	other.end_ = end_;
	end_ = beg_ + weight;

	invariant();
	other.invariant();
}

std::size_t Range::iterPopOverNb( std::size_t nb, iterFunc_t func )
{
	invariant();

	if( nb == 0 )
	{
		return 0;
	}

	nb = std::min( nb, weight() );
	std::size_t stop = beg_ + nb;
	std::size_t calls = 0;


	for( std::size_t ind = beg_; ind < stop; ind++ )
	{
		func( vertex_, ind ); calls++;
	}

	assert( calls == nb );

	beg_ = stop;

	invariant();

	return nb;
}

void Range::invariant()
{
	assert( beg_ <= end_ );
	assert( vertex_==nullptr || weight() <= vertex_->degree() );
	assert( vertex_==nullptr || vertex_->degree() > 0 );
}
