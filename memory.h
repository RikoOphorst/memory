#pragma once

#include <new>
#include <cstdarg>

// These are included in the header file because they simplify life for the programmer a lot
// Because he won't need to include these allocators manually, instead simply include this file
#include "../../blowbox/memory/linear_allocator.h"
#include "../../blowbox/memory/free_list_allocator.h"
#include "../../blowbox/memory/stack_allocator.h"
#include "../../blowbox/memory/pool_allocator.h"
#include "../../blowbox/memory/proxy_allocator.h"

#include "../../blowbox/memory/pointer_math.h"

namespace blowbox
{	
	/**
	* @class blowbox::Memory
	* @brief This class manages all the heap allocated memory in the engine
	* @author Riko Ophorst
	*/
	class Memory
	{
	public:
		/**
		* @brief Default Memory constructor
		*/
		Memory();

		/**
		* @brief Default Memory destructor
		*/
		~Memory();

		/**
		* @brief Gets the instance of the Memory manager
		*/
		static Memory* Instance();

		/**
		* @brief Start the Memory manager up
		*/
		void StartUp(int max_memory);

		/**
		* @brief Shuts the Memory manager down
		*/
		void ShutDown();

		/**
		* @brief Creates & allocates a new LinearAllocator
		* @param[in] size (const size_t&) the size of the memory in the allocator
		*/
		LinearAllocator* CreateLinearAllocator(const size_t& size);

		/**
		* @brief Creates & allocates a new FreeListAllocator
		* @param[in] size (const size_t&) the size of the memory in the allocator
		*/
		FreeListAllocator* CreateFreeListAllocator(const size_t& size);

		/**
		* @brief Creates & allocates a new StackAllocator
		* @param[in] size (const size_t&) the size of the memory in the allocator
		*/
		StackAllocator* CreateStackAllocator(const size_t& size);

		/**
		* @brief Creates & allocates a new StackAllocator
		* @param[in] size (const size_t&) the size of the memory in the allocator
		*/
		template<typename T>
		PoolAllocator* CreatePoolAllocator(const size_t& size);

		/**
		* @brief Allocates an object of type T in the given allocator
		* @param[in] allocator (Allocator*) the allocator in which the object should be allocated
		*/
		template<typename T, typename...Args>
		inline T* Allocate(Allocator* allocator, Args...args);

		/**
		* @brief Deallocates an object of type T in the given allocator
		* @param[in] allocator (Allocator*) the allocator in which the pointer is allocated
		* @param[in] ptr (T*) the pointer to be deallocated
		*/
		template<typename T>
		inline void Deallocate(Allocator* allocator, T* ptr);

		/**
		* @brief Allocates a singleton in a special allocator meant for use by singletons only
		*/
		template<typename T>
		inline T* AllocateSingleton();
	private:
		int max_memory_; //<! The maximum amount of heap allocated memory the application can use in bytes
		void* all_memory_; //<! Pointer to "all" heap allocated memory the application is using

		FreeListAllocator* all_allocators_; //<! A free list allocator that allows allocators to be stored in the pre-allocated heap memory (What an inception, eh?)
		FreeListAllocator* singleton_allocator_; //<! A free list allocator that is supposed to store the pointers to singletons
	};

	//------------------------------------------------------------------------------------------------------
	template <typename T, typename...Args>
	T* Memory::Allocate(Allocator* allocator, Args...args)
	{
		return new (allocator->Allocate(sizeof(T), __alignof(T))) T(args...);
	}

	//------------------------------------------------------------------------------------------------------
	template <typename T>
	void Memory::Deallocate(Allocator* allocator, T* ptr)
	{
		ptr->~T();
		allocator->Deallocate(ptr);
	}

	//------------------------------------------------------------------------------------------------------
	template <typename T>
	T* Memory::AllocateSingleton()
	{
		return new (singleton_allocator_->Allocate(sizeof(T), __alignof(T))) T;
	}

	//------------------------------------------------------------------------------------------------------
	template <typename T>
	PoolAllocator* Memory::CreatePoolAllocator(const size_t& size)
	{
		// Allocate the allocator in the free list allocator
		void* ptr = all_allocators_->Allocate(size + sizeof(PoolAllocator), __alignof(PoolAllocator));

		// Return the allocator while also constructing it through placement-new
		return new (ptr)PoolAllocator(sizeof(T), __alignof(T), size, PointerMath::Add(ptr, sizeof(PoolAllocator)));
	}
}