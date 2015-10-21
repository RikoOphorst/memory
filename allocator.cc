#include "../../blowbox/memory/allocator.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	Allocator::Allocator(const size_t& size, void* start) :
		start_(start),
		size_(size),
		used_memory_(0),
		num_allocations_(0)
	{
		
	}

	//------------------------------------------------------------------------------------------------------
	Allocator::~Allocator()
	{
		// @TODO assert(num_allocations_ == 0 && used_memory == 0)

		start_ = nullptr;
		size_ = 0;
	}

	//------------------------------------------------------------------------------------------------------
	void* Allocator::Start() const
	{
		return start_;
	}

	//------------------------------------------------------------------------------------------------------
	const size_t& Allocator::Size() const
	{
		return size_;
	}

	//------------------------------------------------------------------------------------------------------
	const size_t& Allocator::UsedMemory() const
	{
		return used_memory_;
	}

	//------------------------------------------------------------------------------------------------------
	const size_t& Allocator::NumAllocations() const
	{
		return num_allocations_;
	}
}