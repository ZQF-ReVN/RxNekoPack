#pragma once
#include <list>
#include <vector>
#include <string>
#include <cstdint>

#include "../../ThirdParty/TDA/AutoBuffer.h"


namespace NekoPackTools
{
	namespace MNG
	{
		struct Chunk_Info
		{
			uint32_t uiChunkSize;
			uint32_t uiChunkName;
			uint8_t* pChunkData;
			uint32_t uiChunkCrc;
		};

		struct PNG_Range
		{
			std::list<Chunk_Info>::iterator iteIHDR;
			std::list<Chunk_Info>::iterator iteIEND;
		};

		class MNG
		{
		private:
			TDA::AutoBuffer m_MNG_Buffer;
			std::vector<PNG_Range> m_vecPNG_Range;
			std::list<Chunk_Info> m_listChunk_Info;

		public:
			MNG();
			~MNG();

			bool ExtractMultiplePNG(std::wstring wsMNG);

		private:
			bool GetChunk_Info(std::wstring wsMNG);
			bool GetPNG_Range();
			bool ExtractPNG(std::wstring wsPNG, PNG_Range Range);

		};
	}
}