#pragma once
#include <list>
#include <vector>
#include <string>
#include <cstdint>

#include "MNG_Chunk.h"


namespace NekoPackTools
{
	namespace Picture
	{
		class MNG_Editor
		{
		private:
			std::wstring m_wsMNG;
			std::wstring m_wsFolder;
			std::list<Chunk> m_lsChunk;

			ChunkItor FindIEND(ChunkItor iteBeg);
			ChunkItor FindIHDR(ChunkItor iteBeg);

			void LoadPNG(const std::wstring& wsPNG, std::list<Chunk>& lsPNG);
			void SavePNG(const std::wstring& wsPNG, std::vector<ChunkItor>& vcItePNG);

			size_t ClearPNG();

		public:
			MNG_Editor(const std::wstring& wsMNG);

			void LoadMNG();
			void SaveMNG();
			void ClearMNG();

			void Create();
			void Extract();

		};
	}
}