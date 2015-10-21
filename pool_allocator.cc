#include "pool_allocator.h"

#include "../../blowbox/memory/pointer_math.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	PoolAllocator::PoolAllocator(const size_t& object_size, uint8_t object_alignment, const size_t& size, void* start) :
		Allocator(size, start),
		object_size_(object_size),
		object_alignment_(object_alignment)
	{
		// @TODO assert (object_size >= sizeof(void*))

		// Calculate the adjustment needed on all objects for them to be properly aligned
		uint8_t adjustment = PointerMath::AlignForwardAdjustment(start, object_alignment);

		// Create the list of free blocks
		free_list_ = (void**)PointerMath::Add(start, adjustment);

		// The number of objects that can fit at max in the pool
		size_t num_objects = (size - adjustment) / object_size;
		
		// The free list
		void** ptr = free_list_;

		// Initialise the pool's free list
		for (unsigned int i = 0; i < num_objects; i++)
		{
			*ptr = PointerMath::Add(ptr, object_size);
			ptr = (void**)*ptr;
		}

		*ptr = nullptr;
	}

	//------------------------------------------------------------------------------------------------------
	PoolAllocator::~PoolAllocator()
	{
		free_list_ = nullptr;
	}

	//------------------------------------------------------------------------------------------------------
	void* PoolAllocator::Allocate(const size_t& size, uint8_t alignment)
	{
		// @TODO assert(size = object_size_ && alignment == object_alignment)

		if (free_list_ == nullptr)
		{
			return nullptr;
		}

		void* ptr = free_list_;

		free_list_ = (void**)(*free_list_);

		used_memory_ += size;
		num_allocations_++;

		return ptr;
	}

	//------------------------------------------------------------------------------------------------------
	void PoolAllocator::Deallocate(void* ptr)
	{
		*((void**)ptr) = free_list_;

		free_list_ = (void**)ptr;

		used_memory_ -= object_size_;
		num_allocations_--;
	}
}