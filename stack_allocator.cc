#include "stack_allocator.h"

#include "../../blowbox/memory/pointer_math.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	StackAllocator::StackAllocator(const size_t& size, void* start) :
		Allocator(size, start),
		current_pos_(start)
	{
		// @TODO assert (size > 0)

#if _DEBUG
		prev_position_ = nullptr;
#endif
	}

	//------------------------------------------------------------------------------------------------------
	StackAllocator::~StackAllocator()
	{
#if _DEBUG
		prev_position_ = nullptr;
#endif

		current_pos_ = nullptr;
	}

	//------------------------------------------------------------------------------------------------------
	void* StackAllocator::Allocate(const size_t& size, uint8_t alignment)
	{
		// @TODO assert(size != 0)

		// Calculate the correct adjustment
		uint8_t adjustment = PointerMath::AlignForwardAdjustmentWithHeader(current_pos_, alignment, sizeof(AllocationHeader));
	
		// Does this stack allocator still have enough unallocated memory
		if (used_memory_ + adjustment + size > size_)
		{
			// If not, return a nullptr
			return nullptr;
		}

		// Align the address properly
		void* aligned_address = PointerMath::Add(current_pos_, adjustment);

		// Create & save the header, with the correct adjustment saved in it (used in deallocation)
		AllocationHeader* header = (AllocationHeader*)(PointerMath::Subtract(aligned_address, sizeof(AllocationHeader)));
		header->adjustment = adjustment;

#if _DEBUG
		header->prev_address = prev_position_; // save the previous position in the header
		prev_position_ = aligned_address; // save the previous position in the allocator
#endif

		// Set the current position to the top of the stack
		current_pos_ = PointerMath::Add(aligned_address, size);
		
		// Increase the memory as necessary
		used_memory_ += size + adjustment;
		num_allocations_++;

		// Return the pointer
		return aligned_address;
	}

	//------------------------------------------------------------------------------------------------------
	void StackAllocator::Deallocate(void* ptr)
	{
		// @TODO assert (ptr == prev_position_)

#if _DEBUG
		if (ptr != prev_position_)
		{
			__debugbreak();
		}
#endif

		// Get the header of the block of memory
		AllocationHeader* header = (AllocationHeader*)(PointerMath::Subtract(ptr, sizeof(AllocationHeader)));

		// Decrease allocated memory by the correct amount
		used_memory_ -= (uintptr_t)current_pos_ - (uintptr_t)ptr + header->adjustment;
		num_allocations_--;

		// Change the current position the the correct one
		current_pos_ = PointerMath::Subtract(ptr, header->adjustment);
#if _DEBUG
		prev_position_ = header->prev_address; // If in debug, set the previous address as well
#endif
	}
}