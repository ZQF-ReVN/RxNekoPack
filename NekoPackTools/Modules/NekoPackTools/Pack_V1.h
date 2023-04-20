#pragma once
#include <string>
#include <vector>
#include <cstdint>

#include "Pack_V1_Struct.h"


namespace NekoPackTools
{
	namespace Pack_V1
	{
		class Pack_Editor
		{
		private:
			std::string m_msPack;
			NEKOPACK_V1_Header m_Header;
			std::vector<NEKOPACK_V1_Entry> m_vecIndex;

		public:
			Pack_Editor(const std::string& msPack) : m_msPack(msPack), m_Header({ 0 }) { }

			bool Init();
			bool Extract();

			static void MakeTable(uint32_t* pBuffer, int32_t nMagic);
			static void DeocdeByte(uint32_t* pTable, uint8_t* pByte);
			static void DeocdeBuffer(uint32_t* pTable, uint8_t* pBuffer, uint32_t uiBufferSize);
			static void EncodeBuffer(uint32_t* pTable, uint8_t* pBuffer, uint32_t uiBufferSize);
		};
	}
}