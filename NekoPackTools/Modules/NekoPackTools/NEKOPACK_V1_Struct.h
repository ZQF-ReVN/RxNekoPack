#pragma once
#include <cstdint>

namespace NekoPackTools
{
	namespace NEKOPACK_V1
	{
		struct NEKOPACK_V1_Header
		{
			uint8_t  aSignature[0x8]; // "NEKOPACK"
			uint32_t uiVersion;
			uint32_t uiUn1;
			uint32_t uiResCount;
		};

		struct NEKOPACK_V1_Entry
		{
			uint8_t  ucKey;
			uint8_t  ucResNameLen;
			uint8_t  aResName[0x100];
			uint32_t uiResOffset;  //Pack FOA
			uint32_t uiResSize;
		};
	}
}