#pragma once

#include "../../blowbox/memory/allocator.h"

namespace blowbox
{
	/**
	* @class blowbox::PoolAllocator
	* @brief A pool allocator is an allocator that functions allows allocation and deallocation (in arbitrary order) of fixed size blocks
	* @author Riko Ophorst
	*/
	class PoolAllocator : public Allocator
	{
	public:
		/**
		* @brief Default PoolAllocator constructor
		* @param[in] object_size (const size_t&) the size of the object that this pool should consist of
		* @param[in] object_alignment (uint8_t) the alignment of the object that this pool should consist of
		* @param[in] size (const size_t&) the size of the memory block this allocator uses
		* @param[in] start (void*) a pointer to the start of the memory block this allocator uses
		*/
		PoolAllocator(const size_t& object_size, uint8_t object_alignment, const size_t& size, void* start);

		/**
		* @brief Default PoolAllocator destructor
		*/
		~PoolAllocator();

		/**
		* @brief Allocates a block in the pool allocator
		* @param[in] size (const size_t&) the size of the block to be allocated
		* @param[in] alignment (uint8_t) the alignment of the block to be allocated
		*/
		void* Allocate(const size_t& size, uint8_t alignment) override;

		/**
		* @brief Deallocates a block in the Pool allocator.
		* @param[in] ptr (void*) the pointer to the memory to be deallocated
		*/
		void Deallocate(void* ptr) override;

	private:
		size_t object_size_; //<! the size of each object in the pool
		uint8_t object_alignment_; //<! the alignment of each object in the pool
		void** free_list_; //<! the list of free objects in the pool
	};
}