#pragma once
#include <cstdint>


namespace NekoPackTools
{
	namespace Picture
	{
		static const uint32_t MHDR = 0x4D484452;
		static const uint32_t MEND = 0x4D454E44;
		static const uint32_t DEFI = 0x44454649;
		static const uint32_t oFFs = 0x6F464673;
		static const uint32_t tEXt = 0x74455874;
		static const uint32_t IHDR = 0x49484452;
		static const uint32_t IDAT = 0x49444154;
		static const uint32_t IEND = 0x49454E44;

		static const uint8_t MNG_Signature[0x8] = { 0x8A, 0x4D, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
		static const uint8_t PNG_Signature[0x8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
		static const uint8_t MNG_END[0xC] = { 0x00, 0x00, 0x00, 0x00, 0x4D, 0x45, 0x4E, 0x44, 0x21, 0x20, 0xF7, 0xD5 };
		static const uint8_t PNG_END[0xC] = { 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82 };

#pragma pack(1)
		struct MHDR_Chunk
		{
			uint32_t uiFrame_Width;
			uint32_t uiFrame_Height;
			uint32_t uiTicks_Per_Second;
			uint32_t Nominal_Layer_Count;
			uint32_t Nominal_Frame_Count;
			uint32_t Nominal_Play_Time;
			uint32_t Simplicity_Profile;
		};

		struct DEFI_Chunk
		{
			uint16_t usObject_ID;
			uint8_t ucDo_Not_Show;
			uint8_t ucConcrete_Flag;
			uint32_t uiX_Location;
			uint32_t uiY_Location;
			uint32_t uiLeft_CB;
			uint32_t uiRight_CB;
			uint32_t uiTop_CB;
			uint32_t uiBottom_CB;
		};

		struct IHDR
		{
			uint32_t uiWidth;
			uint32_t uiHeight;
			uint8_t ucBit_Depth;
			uint8_t ucColor_Type;
			uint8_t ucCompresion_Method;
			uint8_t ucFilter_Method;
			uint8_t ucInterlace_Method;
		};
#pragma pack()
	}
}