#pragma once

#include <stdint.h>

namespace blowbox
{
	class Allocator
	{
	public:
		/**
		* @brief Default Allocator constructor
		* @param[in] size (const size_t&) the size in bits of the allocator
		* @param[in] start (void*) the start of the memory
		*/
		Allocator(const size_t& size, void* start);

		/**
		* @brief Default Allocator destructor
		*/
		virtual ~Allocator();

		/**
		* @brief Allocates a block of memory by a certain alignment and returns a pointer of to that newly created block of memory
		* @param[in] size (const size_t&) the size of the block to be allocated
		* @param[in] alignment (const int& alignment) the alignment for the block to be allocated
		*/
		virtual void* Allocate(const size_t& size, uint8_t alignment = 4) = 0;

		/**
		* @brief Deallocates a pointer which is being held by the allocator
		* @param[in] ptr (void*) the pointer that should be deallocated
		*/
		virtual void Deallocate(void* ptr) = 0;

		/**
		* @brief Returns a pointer to the start of the memory block from this allocator
		*/
		void* Start() const;

		/**
		* @brief Returns the size of the of memory block from this allocator
		*/
		const size_t& Size() const;

		/**
		* @brief Returns the memory that is currently in use by this allocator
		*/
		const size_t& UsedMemory() const;

		/**
		* @brief Returns the number of allocations in this allocator
		*/
		const size_t& NumAllocations() const;
	protected:
		void* start_; //!< A pointer to the start of the memory block this allocator uses
		size_t size_; //!< The size of the memory block this allocator uses

		size_t used_memory_; //!< The memory that is currently in use by this allocator
		size_t num_allocations_; //!< The total number of allocations this allocator has
	};
}