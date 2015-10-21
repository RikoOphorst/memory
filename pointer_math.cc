#include "pointer_math.h"

namespace blowbox
{
	namespace PointerMath
	{
		//------------------------------------------------------------------------------------------------------
		void* AlignForward(void* address, uint8_t alignment)
		{
			return (void*)((reinterpret_cast<uintptr_t>(address)+static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
		};

		//------------------------------------------------------------------------------------------------------
		const void* AlignForward(const void* address, uint8_t alignment)
		{
			return (void*)((reinterpret_cast<uintptr_t>(address)+static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
		}

		//------------------------------------------------------------------------------------------------------
		void* AlignBackward(void* address, uint8_t alignment)
		{
			return (void*)(reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(~(alignment - 1)));
		}

		//------------------------------------------------------------------------------------------------------
		const void* AlignBackward(const void* address, uint8_t alignment)
		{
			return (void*)(reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(~(alignment - 1)));
		}

		//------------------------------------------------------------------------------------------------------
		uint8_t AlignForwardAdjustment(const void* address, uint8_t alignment)
		{
			uint8_t adjustment = alignment - (reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(alignment - 1));

			if (adjustment == alignment)
				return 0;

			return adjustment;
		}

		//------------------------------------------------------------------------------------------------------
		uint8_t AlignForwardAdjustmentWithHeader(const void* address, uint8_t alignment, uint8_t headerSize)
		{
			uint8_t adjustment = AlignForwardAdjustment(address, alignment);

			uint8_t neededSpace = headerSize;

			if (adjustment < neededSpace)
			{
				neededSpace -= adjustment;

				adjustment += alignment * (neededSpace / alignment);

				if (neededSpace % alignment > 0)
					adjustment += alignment;
			}

			return adjustment;
		}

		//------------------------------------------------------------------------------------------------------
		uint8_t AlignBackwardAdjustment(const void* address, uint8_t alignment)
		{
			uint8_t adjustment = reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(alignment - 1);

			if (adjustment == alignment)
			{
				return 0;
			}

			return adjustment;
		}

		//------------------------------------------------------------------------------------------------------
		void* Add(void* p, size_t x)
		{
			return (void*)(reinterpret_cast<uintptr_t>(p)+x);
		}

		//------------------------------------------------------------------------------------------------------
		const void* Add(const void* p, size_t x)
		{
			return (const void*)(reinterpret_cast<uintptr_t>(p)+x);
		}

		//------------------------------------------------------------------------------------------------------
		void* Subtract(void* p, size_t x)
		{
			return (void*)(reinterpret_cast<uintptr_t>(p)-x);
		}

		//------------------------------------------------------------------------------------------------------
		const void* Subtract(const void* p, size_t x)
		{
			return (const void*)(reinterpret_cast<uintptr_t>(p)-x);
		}
	}
}