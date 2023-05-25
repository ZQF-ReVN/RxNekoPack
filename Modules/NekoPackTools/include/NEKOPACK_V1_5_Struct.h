#include <cstdint>


namespace NekoPackTools
{
	namespace Pack
	{
		// Custom Structure
		struct NEKOPACK_V1_5_Entry
		{
			uint8_t  aResName[256];
			uint32_t uiResKey;
			uint32_t uiResSize;
			uint32_t uiResOffset;
		};
	}
}