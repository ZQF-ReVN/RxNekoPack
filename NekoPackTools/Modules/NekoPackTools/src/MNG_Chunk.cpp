#include "MNG_Chunk.h"
#include "MNG_Struct.h"
#include "Rxx/include/Mem.h"

using namespace Rut::MemX;


namespace NekoPackTools
{
	namespace Picture
	{
		Chunk::Chunk() : m_uiSize(0), m_uiName(0), m_uiCrc(0), m_pData(nullptr) { }

		Chunk::Chunk(const Chunk& refChunk)
		{
			m_uiSize = refChunk.m_uiSize;
			m_uiName = refChunk.m_uiName;
			m_uiCrc = refChunk.m_uiCrc;

			if (m_uiSize != 0)
			{
				m_pData = new uint8_t[m_uiSize];
				memcpy(m_pData, refChunk.m_pData, m_uiSize);
			}
			else
			{
				m_pData = nullptr;
			}
		}

		Chunk::Chunk(Chunk&& refChunk) noexcept
		{
			this->m_uiSize = refChunk.m_uiSize;
			this->m_uiName = refChunk.m_uiName;
			this->m_uiCrc = refChunk.m_uiCrc;
			this->m_pData = refChunk.m_pData;

			refChunk.m_pData = nullptr;
		}

		Chunk::Chunk(const uint8_t* pChunk)
		{
			Read(pChunk);
		}

		Chunk::~Chunk()
		{
			if (m_pData != nullptr) { delete[] m_pData; }
		}

		uint32_t Chunk::SwapBytes(uint32_t uiValue)
		{
			return (uiValue & 0xFF000000) >> 24 | (uiValue & 0x00FF0000) >> 8 | (uiValue & 0x0000FF00) << 8 | (uiValue & 0x000000FF) << 24;
		}

		void Chunk::Read(const uint8_t* pChunk)
		{
			m_uiSize = SwapBytes(*(uint32_t*)(pChunk + 4 * 0));
			m_uiName = SwapBytes(*(uint32_t*)(pChunk + 4 * 1));
			m_uiCrc = SwapBytes(*(uint32_t*)(pChunk + 4 * 2 + m_uiSize));

			if (m_uiSize != 0)
			{
				m_pData = new uint8_t[m_uiSize];
				memcpy(m_pData, (pChunk + 4 * 2), m_uiSize);
			}
			else
			{
				m_pData = nullptr;
			}
		}

		void Chunk::Write(uint8_t* pBuffer)
		{
			uint32_t size_r = SwapBytes(m_uiSize);
			uint32_t name_r = SwapBytes(m_uiName);
			uint32_t crc_r = SwapBytes(m_uiCrc);

			memcpy(pBuffer, &size_r, 4);
			memcpy(pBuffer, &name_r, 4);
			memcpy(pBuffer, m_pData, m_uiSize);
			memcpy(pBuffer, &crc_r, 4);
		}

		void Chunk::Write(std::ofstream& ofStream)
		{
			uint32_t size_r = SwapBytes(m_uiSize);
			uint32_t name_r = SwapBytes(m_uiName);
			uint32_t crc_r = SwapBytes(m_uiCrc);

			ofStream.write(reinterpret_cast<char*>(&size_r), 4);
			ofStream.write(reinterpret_cast<char*>(&name_r), 4);
			ofStream.write(reinterpret_cast<char*>(m_pData), m_uiSize);
			ofStream.write(reinterpret_cast<char*>(&crc_r), 4);
		}

		std::ofstream& operator << (std::ofstream& ofStream, Chunk& cChunk)
		{
			cChunk.Write(ofStream);
			return ofStream;
		}

		void Chunk::LoadChunks(const std::wstring& wsFile, std::list<Chunk>& lsChunk)
		{
			//Init File Buffer
			AutoMem auto_buffer(wsFile);
			size_t file_size = auto_buffer.GetSize();
			uint8_t* file_buffer = auto_buffer.GetPtr();
			if (file_size == 0 || file_buffer == nullptr) { throw std::runtime_error("Chunk::LoadChunks: Init File Buffer Error"); }

			//Read Each Chunk
			for (size_t ite_file = sizeof(MNG_Signature); ite_file < file_size; ite_file += 12)
			{
				Chunk chunk(file_buffer + ite_file);
				ite_file += chunk.GetSize();
				lsChunk.emplace_back(std::move(chunk));
			}
		}
	}
}