#include "memory.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	Memory::Memory()
	{

	}

	//------------------------------------------------------------------------------------------------------
	Memory::~Memory()
	{

	}

	//------------------------------------------------------------------------------------------------------
	Memory* Memory::Instance()
	{
		static Memory* ptr = new Memory();
		return ptr;
	}

	//------------------------------------------------------------------------------------------------------
	void Memory::StartUp(int max_memory)
	{
		max_memory_ = max_memory;
		all_memory_ = malloc(max_memory_);

		all_allocators_ = new FreeListAllocator(max_memory_, all_memory_);
		singleton_allocator_ = CreateFreeListAllocator(100000);
	}

	//------------------------------------------------------------------------------------------------------
	void Memory::ShutDown()
	{
		// Check if there is still memory marked as in use

		delete all_allocators_;
		
		free(all_memory_);
		max_memory_ = 0;
	}

	//------------------------------------------------------------------------------------------------------
	LinearAllocator* Memory::CreateLinearAllocator(const size_t& size)
	{
		// Allocate the allocator in the free list allocator
		void* ptr = all_allocators_->Allocate(size + sizeof(LinearAllocator), __alignof(LinearAllocator));

		// Return the allocator while also constructing it through placement-new
		return new (ptr)LinearAllocator(size, PointerMath::Add(ptr, sizeof(LinearAllocator)));
	}

	//------------------------------------------------------------------------------------------------------
	FreeListAllocator* Memory::CreateFreeListAllocator(const size_t& size)
	{
		// Allocate the allocator in the free list allocator
		void* ptr = all_allocators_->Allocate(size + sizeof(FreeListAllocator), __alignof(FreeListAllocator));

		// Return the allocator while also constructing it through placement-new
		return new (ptr)FreeListAllocator(size, PointerMath::Add(ptr, sizeof(FreeListAllocator)));
	}

	//------------------------------------------------------------------------------------------------------
	StackAllocator* Memory::CreateStackAllocator(const size_t& size)
	{
		// Allocate the allocator in the free list allocator
		void* ptr = all_allocators_->Allocate(size + sizeof(StackAllocator), __alignof(StackAllocator));

		// Return the allocator while also constructing it through placement-new
		return new (ptr)StackAllocator(size, PointerMath::Add(ptr, sizeof(StackAllocator)));
	}
}