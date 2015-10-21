#include "proxy_allocator.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	ProxyAllocator::ProxyAllocator(Allocator* allocator) :
		Allocator(allocator->Size(), allocator->Start()),
		allocator_(allocator)
	{

	}

	//------------------------------------------------------------------------------------------------------
	ProxyAllocator::~ProxyAllocator()
	{

	}

	//------------------------------------------------------------------------------------------------------
	void* ProxyAllocator::Allocate(const size_t& size, uint8_t alignment)
	{
		// @TODO assert(size)
		num_allocations_++;

		size_t mem = allocator_->UsedMemory();
		void* ptr = allocator_->Allocate(size, alignment);

		used_memory_ += allocator_->UsedMemory() - mem;

		return ptr;
	}

	//------------------------------------------------------------------------------------------------------
	void ProxyAllocator::Deallocate(void* ptr)
	{
		num_allocations_--;

		size_t mem = allocator_->UsedMemory();
		allocator_->Deallocate(ptr);

		used_memory_ -= mem - allocator_->UsedMemory();
	}
}