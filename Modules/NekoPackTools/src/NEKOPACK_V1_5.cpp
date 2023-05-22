#include "NEKOPACK_V1_5.h"

namespace NekoPackTools
{
	namespace Pack
	{
		// buffer requires 8 bytes of alignment
		void DecBuffer(uint32_t* pEnc, uint32_t* pTable, uint32_t nKey, uint32_t nSize)
		{
			uint32_t count = nSize / 4;
			while (count--)
			{
				nKey += 0xEB0974C3;
				nKey <<= 0x17;
				nKey >>= 0x17;
				pEnc[0] ^= *(uint32_t*)((uint32_t)pTable + nKey);
				nKey += pEnc++[0];
			}
		}
	}
}