#pragma once

#include <cstdint>

namespace blowbox
{
	namespace PointerMath
	{
		void* AlignForward(void* address, uint8_t alignment);
		const void* AlignForward(const void* address, uint8_t alignment);

		void* AlignBackward(void* address, uint8_t alignment);
		const void* AlignBackward(const void* address, uint8_t alignment);

		uint8_t AlignForwardAdjustment(const void* address, uint8_t alignment);
		uint8_t AlignForwardAdjustmentWithHeader(const void* address, uint8_t alignment, uint8_t header);

		uint8_t AlignBackwardAdjustment(const void* address, uint8_t alignment);

		void* Add(void* ptr, size_t x);
		const void* Add(const void* ptr, size_t x);

		void* Subtract(void* ptr, size_t x);
		const void* Subtract(const void* ptr, size_t x);
	}
}