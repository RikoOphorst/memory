#include "linear_allocator.h"

#include "../../blowbox/memory/pointer_math.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	LinearAllocator::LinearAllocator(const size_t& size, void* start) :
		Allocator(size, start),
		current_pos_(start)
	{
		// @TODO assert(size > 0)
	}

	//------------------------------------------------------------------------------------------------------
	LinearAllocator::~LinearAllocator()
	{
		current_pos_ = nullptr;
	}

	//------------------------------------------------------------------------------------------------------
	void* LinearAllocator::Allocate(const size_t& size, uint8_t alignment)
	{
		// @TODO assert(size != 0)

		uint8_t adjustment = PointerMath::AlignForwardAdjustment(current_pos_, alignment);

		if (used_memory_ + adjustment + size > size_)
		{
			return nullptr;
		}

		uintptr_t aligned_address = (uintptr_t)current_pos_ + adjustment;
		
		current_pos_ = (void*)(aligned_address + size);
		used_memory_ += size + adjustment;
		num_allocations_++;

		return (void*)aligned_address;
	}

	//------------------------------------------------------------------------------------------------------
	void LinearAllocator::Deallocate(void* ptr)
	{
		// @TODO assert because deallocation is not possible with linear allocators
	}

	//------------------------------------------------------------------------------------------------------
	void LinearAllocator::Clear()
	{
		num_allocations_ = 0;
		used_memory_ = 0;
		current_pos_ = start_;
	}
}