#pragma once

#include "../../blowbox/memory/allocator.h"

namespace blowbox
{
	/**
	* @class blowbox::LinearAllocator
	* @brief A linear allocator allocates memory in a linear fashion, meaning it cannot be deallocated
	* @author Riko Ophorst
	*/
	class LinearAllocator : public Allocator
	{
	public:
		/**
		* @brief Default LinearAllocator constructor
		* @param[in] size (const size_t&) the size of the memory block this allocator uses
		* @param[in] start (void*) a pointer to the start of the memory block this allocator uses
		*/
		LinearAllocator(const size_t& size, void* start);

		/**
		* @brief Default LinearAllocator destructor
		*/
		~LinearAllocator();

		/**
		* @brief Allocates a block in the linear allocator
		* @param[in] size (const size_t&) the size of the block to be allocated
		* @param[in] alignment (uint8_t) the alignment of the block to be allocated
		*/
		void* Allocate(const size_t& size, uint8_t alignment) override;

		/**
		* @brief Deallocates a block in the linear allocator. HOWEVER, the linear allocator doesn't deallocate at all.
		* @param[in] ptr (void*) the pointer to the memory
		*/
		void Deallocate(void* ptr) override;

		/**
		* @brief Clears the allocator, meaning it resets itself. NOTE: Things do NOT get actually deallocated, they still
		* exist and all pointers cached still function, but they should NOT be used anymore when the allocator they're stored
		* in is cleared, because they can be reassigned at ANY moment.
		*/
		void Clear();
	private:
		void* current_pos_; //!< a pointer to the last "allocated" block of memory
	};
}