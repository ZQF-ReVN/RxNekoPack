#pragma once
#include <cstdint>

namespace NekoPackTools
{
	namespace Pack
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

		/*
		* 2004-06-25 Lass  3days¡«œº¤Á¤Æ¤æ¤¯¿Ì¤Î±Ë·½¤Ç¡«
		* 2005-09-16 Navel Tick! Tack!
		*/
	}
}