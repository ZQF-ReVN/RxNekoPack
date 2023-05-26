#pragma once
#include <cstdint>


namespace NekoPackTools
{
	namespace Pack
	{
		void DecodeBuffer(uint32_t* pEnc, uint32_t nKey, size_t nSize);
	}
}