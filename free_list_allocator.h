#pragma once

#include "../../blowbox/memory/allocator.h"

namespace blowbox
{
	/**
	* @class blowbox::FreeListAllocator
	* @brief Allows you to allocate memory freely
	* @author Riko Ophorst
	*/
	class FreeListAllocator : public Allocator
	{
	public:
		/**
		* @brief Default FreeListAllocator constructor
		* @param[in] size (const size_t&) the size of the memory block this allocator uses
		* @param[in] start (void*) a pointer to the start of the memory block this allocator uses
		*/
		FreeListAllocator(const size_t& size, void* start);

		/**
		* @brief Default FreeListAllocator destructor
		*/
		~FreeListAllocator();

		/**
		* @brief Allocates a block in the free list allocator
		* @param[in] size (const size_t&) the size of the block to be allocated
		* @param[in] alignment (uint8_t) the alignment of the block to be allocated
		*/
		void* Allocate(const size_t& size, uint8_t alignment) override;

		/**
		* @brief Deallocates a block in the free list allocator.
		* @param[in] ptr (void*) the pointer to the memory
		*/
		void Deallocate(void* ptr) override;

	private:
		/**
		* @struct blowbox::FreeListAllocator::AllocationHeader
		* @brief Header to every block of memory allocated in the free list allocator
		* @author Riko Ophorst
		*/
		struct AllocationHeader
		{
			size_t size;
			uint8_t adjustment;
		};

		/**
		* @struct blowbox::FreeListAllocator::FreeBlock
		* @brief Acts as a free block in the free list allocator, is inherently a linked list
		* @author Riko Ophorst
		*/
		struct FreeBlock
		{
			size_t size;
			FreeBlock* next;
		};

		FreeBlock* free_blocks_; //<! A pointer to the start of the free blocks linked list
	};
}