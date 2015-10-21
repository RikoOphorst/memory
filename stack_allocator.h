#pragma once

#include "../../blowbox/memory/allocator.h"

namespace blowbox
{
	/**
	* @class blowbox::StackAllocator
	* @brief A stack allocator is an allocator that functions as a stack (LIFO)
	* @author Riko Ophorst
	*/
	class StackAllocator : public Allocator
	{
	public:
		/**
		* @brief Default StackAllocator constructor
		* @param[in] size (const size_t&) the size of the memory block this allocator uses
		* @param[in] start (void*) a pointer to the start of the memory block this allocator uses
		*/
		StackAllocator(const size_t& size, void* start);

		/**
		* @brief Default StackAllocator destructor
		*/
		~StackAllocator();

		/**
		* @brief Allocates a block in the stack allocator
		* @param[in] size (const size_t&) the size of the block to be allocated
		* @param[in] alignment (uint8_t) the alignment of the block to be allocated
		*/
		void* Allocate(const size_t& size, uint8_t alignment) override;

		/**
		* @brief Deallocates a block in the stack allocator.
		* @param[in] ptr (void*) the pointer to the memory to be deallocated
		*/
		void Deallocate(void* ptr) override;

	private:

		/**
		* @struct blowbox::StackAllocator::AllocationHeader
		* @brief The header for each allocated block of memory in the stack allocator
		* @author Riko Ophorst
		*/
		struct AllocationHeader
		{
#if _DEBUG
			void* prev_address; //<! pointer to the previous allocated address. NOTE: Only active in debug builds
#endif
			uint8_t adjustment; //<! the adjustment that was needed for the allocation
		};

#if _DEBUG
		void* prev_position_; //<! pointer to the previous allocated address. NOTE: Only active in debug builds
#endif

		void*  current_pos_; //<! pointer to the top of the stack
	};
}