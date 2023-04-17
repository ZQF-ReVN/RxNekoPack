#pragma once
#include <cstdint>


namespace NekoPackTools
{
	namespace MNG
	{
#pragma pack(1)
		static struct MHDR_Chunk
		{
			uint32_t uiFrame_Width;
			uint32_t uiFrame_Height;
			uint32_t uiTicks_Per_Second;
			uint32_t Nominal_Layer_Count;
			uint32_t Nominal_Frame_Count;
			uint32_t Nominal_Play_Time;
			uint32_t Simplicity_Profile;
		};

		static struct DEFI_Chunk
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

		static struct IHDR
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