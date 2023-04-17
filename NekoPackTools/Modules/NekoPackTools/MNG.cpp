#include "MNG.h"
#include "../../ThirdParty/TDA/FileX.h"

#include <fstream>


namespace NekoPackTools
{
	namespace MNG
	{
		static const uint32_t MHDR = 0x4D484452;
		static const uint32_t MEND = 0x4D454E44;
		static const uint32_t IHDR = 0x49484452;
		static const uint32_t IDAT = 0x49444154;
		static const uint32_t IEND = 0x49454E44;

		static uint8_t MNG_Signature[0x8] = { 0x8A, 0x4D, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
		static uint8_t PNG_Signature[0x8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

		inline uint32_t SwapBytes(uint32_t uiValue)
		{
			return (uiValue & 0xFF000000) >> 24 | (uiValue & 0x00FF0000) >> 8 | (uiValue & 0x0000FF00) << 8 | (uiValue & 0x000000FF) << 24;
		}

		inline uint32_t BigEndian(uint32_t uiValue)
		{
			return (uiValue & 0xFF000000) >> 24 | (uiValue & 0x00FF0000) >> 8 | (uiValue & 0x0000FF00) << 8 | (uiValue & 0x000000FF) << 24;
		}


		MNG::MNG()
		{

		}

		MNG::~MNG()
		{

		}

		bool MNG::GetChunk_Info(std::wstring wsMNG)
		{
			std::ifstream ifs(wsMNG, std::ios::binary);
			if (!ifs.is_open()) return false;

			size_t sizeMNG = static_cast<size_t>(TDA::FileX::GetFileSize(ifs));
			uint8_t* pMNG = m_MNG_Buffer.GetBuffer(sizeMNG);
			ifs.read((char*)(pMNG), sizeMNG);

			Chunk_Info chunk = { 0 };
			for (size_t iteData = 8; iteData < sizeMNG;)
			{
				chunk.uiChunkSize = SwapBytes(*(uint32_t*)(pMNG + iteData + 4 * 0));
				chunk.uiChunkName = SwapBytes(*(uint32_t*)(pMNG + iteData + 4 * 1));
				chunk.uiChunkCrc = SwapBytes(*(uint32_t*)(pMNG + iteData + 4 * 2 + chunk.uiChunkSize));

				if (chunk.uiChunkSize != 0)
				{
					chunk.pChunkData = (pMNG + iteData + 4 * 2);
				}
				else
				{
					chunk.pChunkData = nullptr;
				}

				m_listChunk_Info.emplace_back(chunk);

				iteData += chunk.uiChunkSize + 12;
			}

			return true;
		}

		bool MNG::GetPNG_Range()
		{
			PNG_Range png_range;

			for (std::list<Chunk_Info>::iterator ite = m_listChunk_Info.begin(); ite != m_listChunk_Info.end(); ite++)
			{
				if (ite->uiChunkName == IHDR)
				{
					png_range.iteIHDR = ite;
					continue;
				}

				if (ite->uiChunkName == IEND)
				{
					png_range.iteIEND = ite;

					m_vecPNG_Range.emplace_back(png_range);
				}
			}

			return true;
		}

		bool MNG::ExtractPNG(std::wstring wsPNG, PNG_Range Range)
		{
			std::ofstream ofs(wsPNG, std::ios::binary);
			if (!ofs.is_open()) return false;

			ofs.write((char*)PNG_Signature, sizeof(PNG_Signature));

			Range.iteIEND++;

			for (auto& ite = Range.iteIHDR; ite != Range.iteIEND; ite++)
			{
				//if ((ite->uiChunkName != IHDR) && (ite->uiChunkName != IDAT)) { continue; }

				uint32_t size_r = SwapBytes(ite->uiChunkSize);
				uint32_t name_r = SwapBytes(ite->uiChunkName);
				uint8_t* pData = ite->pChunkData;
				uint32_t crc_r = SwapBytes(ite->uiChunkCrc);

				ofs.write((char*)(&size_r), 4);
				ofs.write((char*)(&name_r), 4);
				ofs.write((char*)pData, ite->uiChunkSize);
				ofs.write((char*)(&crc_r), 4);
			}

			ofs.flush();

			return true;
		}

		bool MNG::ExtractMultiplePNG(std::wstring wsMNG)
		{
			if (!GetChunk_Info(wsMNG)) { return false; }

			if (!GetPNG_Range()) { return false; }

			size_t count = 0;
			std::wstring fileName = wsMNG.substr(0, wsMNG.size() - 4);
			for (auto& rang : m_vecPNG_Range)
			{
				ExtractPNG(fileName + L"-" + std::to_wstring(count) + L".png", rang);

				count++;
			}

			return true;
		}

	}
}