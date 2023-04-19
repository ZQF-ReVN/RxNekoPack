#pragma once
#include <list>
#include <vector>
#include <string>
#include <cstdint>


namespace NekoPackTools
{
	namespace MNG
	{
		class Chunk
		{
		private:
			uint32_t m_uiChunkSize;
			uint32_t m_uiChunkName;
			uint32_t m_uiChunkCrc;
			uint8_t* m_pChunkData;

			static uint32_t SwapBytes(uint32_t uiValue);

		public:
			Chunk();
			Chunk(const Chunk& refChunk);
			Chunk(Chunk&& refChunk)  noexcept;
			Chunk(const uint8_t* pChunk);
			~Chunk();

			bool Read(const uint8_t* pChunk);
			bool Write(std::ofstream& refStream);
			static bool LoadChunks(std::wstring wsMNG, std::list<Chunk>& listChunk);

			uint32_t GetSize() { return m_uiChunkSize; }
			uint32_t GetName() { return m_uiChunkName; }
			uint32_t GetCrc()  { return m_uiChunkCrc; }
			uint8_t* GetData() { return m_pChunkData; }
		};


		class Frame
		{
		private:
			std::list<Chunk>::iterator m_iteBeg;
			std::list<Chunk>::iterator m_iteEnd;

		public:
			bool SaveFrame(std::wstring wsFrame);
			void SetRange(std::list<Chunk>::iterator iteBeg, std::list<Chunk>::iterator iteEnd);

			static bool FindFrame(std::list<Chunk>& listChunk, std::vector<Frame>& vecFrame);

			std::list<Chunk>::iterator GetBeg() { return m_iteBeg; }
			std::list<Chunk>::iterator GetEnd() { return m_iteEnd; }
		};


		class PNG
		{
		public:
			std::wstring m_wsPNG;
			std::list<Chunk> m_listChunk;

			PNG(std::wstring wsPNG) : m_wsPNG(wsPNG) {}

			bool LoadPNG();
			bool MergeInfo();

			std::list<Chunk>& GetChunkList() { return m_listChunk; }
		};


		class MNG_Editor
		{
		private:
			std::wstring m_wsMNG;
			std::list<Chunk> m_listChunk;
			std::vector<Frame> m_vecFrame;

		public:
			MNG_Editor(std::wstring wsMNG) { m_wsMNG = wsMNG; }

			bool LoadMNG();
			bool SaveMNG();
			bool BuildMNG();
			bool ExtractMultiplePNG();

		private:
			bool LoadMHDR(std::wstring wsFile);
			bool SaveMHDR(std::wstring wsFile);
			bool ExtractFrame(std::wstring wsPNG, Frame& refFrame);

		};
	}
}