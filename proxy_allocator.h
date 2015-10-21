#pragma once

#include "../../blowbox/memory/allocator.h"

namespace blowbox
{
	/**
	* @class blowbox::ProxyAllocator
	* @brief Allows another allocator to be proxied by the ProxyAllocator, which aids in debugging
	* @author Riko Ophorst
	*/
	class ProxyAllocator : public Allocator
	{
	public:
		/**
		* @brief Default ProxyAllocator constructor
		* @param[in] allocator (Allocator*) the allocator that should be proxied
		*/
		ProxyAllocator(Allocator* allocator);
		
		/**
		* @brief Default ProxyAllocator destructor
		*/
		~ProxyAllocator();

		/**
		* @brief Proxy-allocates a block of memory in the hidden allocator
		* @param[in] size (const size_t&) the size of the block of memory that is being allocated
		* @param[in] alignment (uint8_t) the alignment of the block
		*/
		void* Allocate(const size_t& size, uint8_t alignment) override;

		/**
		* @brief Proxy de-allocates a block of memory
		* @param[in] ptr (void*) a pointer to the block of memory that has to be de-allocated
		*/
		void Deallocate(void* ptr) override;
	private:
		Allocator* allocator_; //<! The allocator that is being proxied by this allocator
	};
}