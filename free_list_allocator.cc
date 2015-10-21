#include "free_list_allocator.h"

#include "../../blowbox/memory/pointer_math.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	FreeListAllocator::FreeListAllocator(const size_t& size, void* start) :
		Allocator(size, start),
		free_blocks_((FreeBlock*)start)
	{
		// @TODO assert(size > 0);

		free_blocks_->size = size;
		free_blocks_->next = nullptr;
	}

	//------------------------------------------------------------------------------------------------------
	FreeListAllocator::~FreeListAllocator()
	{
		free_blocks_ = nullptr;
	}

	//------------------------------------------------------------------------------------------------------
	void* FreeListAllocator::Allocate(const size_t& size, uint8_t alignment)
	{
		// @TODO assert(size != 0 && alignment != 0)

		FreeBlock* prev_free_block = nullptr;
		FreeBlock* free_block = free_blocks_;

		while (free_block != nullptr)
		{
			// Calculate the adjustment needed to align the memory address correctly
			uint8_t adjustment = PointerMath::AlignForwardAdjustmentWithHeader(free_block, alignment, sizeof(AllocationHeader));
			
			// The total size of the memory block
			size_t total_size = size + adjustment;

			// Is the current free block's size big enough to fit the size of the block to be allocated?
			if (free_block->size < total_size)
			{
				// No, so we skip the current free block and move on to the next block in the line
				prev_free_block = free_block;
				free_block = free_block->next;
				continue;
			}

			// Asserts whether the allocation header is bigger than the free block
			static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");

			// Is it possible to create another block of memory in this allocator?
			if (free_block->size - total_size <= sizeof(AllocationHeader))
			{
				// No, so therefore this is the last possible free block in this allocator that is possible

				// Increase the total size of the block to be allocated to the maximum size of the current free block
				total_size = free_block->size;

				if (prev_free_block != nullptr)
				{
					// We set the previous free block's next to the current free block's next
					prev_free_block->next = free_block->next;
				}
				else
				{
					// We set all the free blocks to the current free block's next
					free_blocks_ = free_block->next;
				}
			}
			else
			{
				// Yes, so therefore we "split" the current free block.

				// Create a new pointer to a FreeBlock that is offsetted by the total size of the memory block to be allocated
				FreeBlock* next_block = (FreeBlock*)(PointerMath::Add(free_block, total_size));
				next_block->size = free_block->size - total_size;
				next_block->next = free_block->next;

				if (prev_free_block != nullptr)
				{
					// We set the previous free block's next to the newly created free block
					prev_free_block->next = next_block;
				}
				else
				{
					// We set the start of the free blocks linked list to be the free block we have just created
					free_blocks_ = next_block;
				}
			}

			// Calculate the aligned address of the memory block to be allocated
			uintptr_t aligned_address = (uintptr_t)free_block + adjustment;

			// Create & save the allocation header that is used for the allocation of the memory block
			AllocationHeader* header = (AllocationHeader*)(aligned_address - sizeof(AllocationHeader));
			header->size = total_size;
			header->adjustment = adjustment;

			// Increase the used memory & number of allocations
			used_memory_ += total_size;
			num_allocations_++;

			// @TODO assert(PointerMath::AlignForwardAdjustment((void*)aligned_address, alignment) == 0)

			return (void*)aligned_address;
		}

		// @TODO assert because no block was found to be large enough

		return nullptr;
	}

	//------------------------------------------------------------------------------------------------------
	void FreeListAllocator::Deallocate(void* ptr)
	{
		// @TODO assert(ptr != nullptr)

		// Retrieve the pointer to the header of this block of memory
		AllocationHeader* header = (AllocationHeader*)PointerMath::Subtract(ptr, sizeof(AllocationHeader));
	
		uintptr_t block_start = reinterpret_cast<uintptr_t>(ptr) - header->adjustment; // The start of the block
		size_t block_size = header->size; // The size of the block
		uintptr_t block_end = block_start + block_size; // The end of the block

		FreeBlock* prev_free_block = nullptr; // Previous free block
		FreeBlock* free_block = free_blocks_; // Current free block


		while (free_block != nullptr)
		{
			if ((uintptr_t)free_block >= block_end)
			{
				break;
			}

			prev_free_block = free_block;
			free_block = free_block->next;
		}

		if (prev_free_block == nullptr)
		{
			prev_free_block = (FreeBlock*)block_start;
			prev_free_block->size = block_size;
			prev_free_block->next = free_blocks_;

			free_blocks_ = prev_free_block;
		}
		else if ((uintptr_t)prev_free_block + prev_free_block->size == block_start)
		{
			prev_free_block->size += block_size;
		}
		else
		{
			FreeBlock* temp = (FreeBlock*)block_start;
			temp->size = block_size;
			temp->next = prev_free_block->next;
			
			prev_free_block->next = temp;
			prev_free_block = temp;
		}

		if (free_block != nullptr && (uintptr_t)free_block == block_end)
		{
			prev_free_block->size += free_block->size;
			prev_free_block->next = free_block->next;
		}

		num_allocations_--;
		used_memory_ -= block_size;
	}
}