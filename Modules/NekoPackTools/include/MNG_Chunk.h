#pragma once
#include <list>
#include <fstream>
#include <cstdint>


namespace NekoPackTools
{
	namespace Picture
	{
		class Chunk
		{
		private:
			uint32_t m_uiSize;
			uint32_t m_uiName;
			uint32_t m_uiCrc;
			uint8_t* m_pData;

			static uint32_t SwapBytes(uint32_t uiValue);

		public:
			Chunk();
			Chunk(const Chunk& refChunk);
			Chunk(Chunk&& refChunk)  noexcept;
			Chunk(const uint8_t* pChunk);
			~Chunk();

			void Read(const uint8_t* pChunk);
			void Write(uint8_t* pBuffer);
			void Write(std::ofstream& ofStream);

			friend std::ofstream& operator << (std::ofstream& ofStream, Chunk& cChunk);

			static void LoadChunks(const std::wstring& wsMNG, std::list<Chunk>& lsChunk);

			uint32_t GetSize() const { return m_uiSize; }
			uint32_t GetName() const { return m_uiName; }
			uint32_t GetCrc()  const { return m_uiCrc; }
			uint8_t* GetData() const { return m_pData; }
		};

		typedef std::list<Chunk>::iterator ChunkItor;
	}
}