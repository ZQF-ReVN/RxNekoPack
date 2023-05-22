#include "MNG.h"
#include "MNG_Struct.h"
#include "Rxx/include/File.h"

#include <fstream>
#include <direct.h>
#include <algorithm>

using namespace Rut::FileX;


namespace NekoPackTools
{
	namespace Picture
	{
		MNG_Editor::MNG_Editor(const std::wstring& wsMNG)
		{
			m_wsMNG = wsMNG;
			m_wsFolder = m_wsMNG.substr(0, m_wsMNG.size() - 4) + L"/";
		}

		ChunkItor MNG_Editor::FindIHDR(ChunkItor iteBeg)
		{
			for (; iteBeg != m_lsChunk.end(); iteBeg++) { if (iteBeg->GetName() == IHDR) { return iteBeg; } }
			throw std::runtime_error("std::list<Chunk>::iterator MNG_Editor::FindIHDR: Not Find MNG IHDR Chunk");
		}

		ChunkItor MNG_Editor::FindIEND(ChunkItor iteBeg)
		{
			for (; iteBeg != m_lsChunk.end(); iteBeg++) { if (iteBeg->GetName() == IEND) { return iteBeg; } }
			throw std::runtime_error("std::list<Chunk>::iterator MNG_Editor::FindIEND: Not Find MNG IEND Chunk");
		}

		void MNG_Editor::LoadPNG(const std::wstring& wsPNG, std::list<Chunk>& lsPNG)
		{
			Chunk::LoadChunks(wsPNG, lsPNG);
		}

		void MNG_Editor::SavePNG(const std::wstring& wsPNG, std::vector<ChunkItor>& vcItePNG)
		{
			std::ofstream ofs = CreateFileBinaryStream(wsPNG);
			ofs.write((char*)PNG_Signature, sizeof(PNG_Signature));
			for (auto& ite_chunk : vcItePNG) { ofs << *ite_chunk; }
		}

		size_t MNG_Editor::ClearPNG()
		{
			size_t frame_count = 0;

			auto filter = [&frame_count](const Chunk& ck) -> bool
			{
				switch (ck.GetName())
				{
				case IHDR: { frame_count++; } break;
				case IDAT: { return true; } break;
				}
				return false;
			};
			m_lsChunk.remove_if(filter);

			return frame_count;
		}

		void MNG_Editor::LoadMNG()
		{
			Chunk::LoadChunks(m_wsMNG, m_lsChunk);
			(void)_wmkdir(m_wsFolder.c_str());
		}

		void MNG_Editor::SaveMNG()
		{
			std::ofstream ofs = CreateFileBinaryStream(m_wsMNG + L".new");
			ofs.write((char*)MNG_Signature, sizeof(MNG_Signature));
			std::for_each(m_lsChunk.begin(), m_lsChunk.end(), [&ofs](Chunk& ck) { ofs << ck; });
		}

		void MNG_Editor::ClearMNG()
		{
			m_lsChunk.clear();
		}

		void MNG_Editor::Create()
		{
			std::list<Chunk> png_chunks;
			std::size_t frame_count = ClearPNG();

			auto ite_png_beg = m_lsChunk.begin();
			for (size_t ite_frames = 0; ite_frames < frame_count; ite_frames++)
			{
				LoadPNG(m_wsFolder + std::to_wstring(ite_frames) + L".png", png_chunks);
				ite_png_beg = m_lsChunk.insert(m_lsChunk.erase(FindIHDR(ite_png_beg)), *png_chunks.begin());
				m_lsChunk.insert(FindIEND(++ite_png_beg), ++png_chunks.begin(), --png_chunks.end());
				png_chunks.clear();
			}
		}

		void MNG_Editor::Extract()
		{
			std::size_t png_count = 0;
			std::vector<ChunkItor> ite_png_chunks;
			for (auto ite = m_lsChunk.begin(); ite != m_lsChunk.end(); ite++)
			{
				switch (ite->GetName())
				{
				case IDAT:case IHDR: { ite_png_chunks.push_back(ite); } break;
				case IEND:
				{
					ite_png_chunks.push_back(ite);
					SavePNG(m_wsFolder + std::to_wstring(png_count++) + L".png", ite_png_chunks);
					ite_png_chunks.clear();
				} break;
				}
			}
		}
	}
}