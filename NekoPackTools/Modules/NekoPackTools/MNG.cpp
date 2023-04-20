#include "MNG.h"
#include "../../ThirdParty/TDA/ConsoleX.h"
#include "../../ThirdParty/TDA/FileX.h"
#include "../../ThirdParty/TDA/AutoBuffer.h"


#include <fstream>
#include <direct.h>
#include <iostream>
#include <algorithm>



namespace NekoPackTools
{
	namespace MNG
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

		//****************
		//*    Chunk    *
		//****************
		Chunk::Chunk() : m_uiChunkSize(0), m_uiChunkName(0), m_uiChunkCrc(0), m_pChunkData(nullptr) {}

		Chunk::Chunk(const Chunk& refChunk)
		{
			m_uiChunkSize = refChunk.m_uiChunkSize;
			m_uiChunkName = refChunk.m_uiChunkName;
			m_uiChunkCrc = refChunk.m_uiChunkCrc;

			if (m_uiChunkSize != 0)
			{
				m_pChunkData = new uint8_t[m_uiChunkSize];
				memcpy(m_pChunkData, refChunk.m_pChunkData, m_uiChunkSize);
			}
			else
			{
				m_pChunkData = nullptr;
			}
		}

		Chunk::Chunk (Chunk&& refChunk) noexcept
		{
			this->m_uiChunkSize = refChunk.m_uiChunkSize;
			this->m_uiChunkName = refChunk.m_uiChunkName;
			this->m_uiChunkCrc = refChunk.m_uiChunkCrc;
			this->m_pChunkData = refChunk.m_pChunkData;

			refChunk.m_pChunkData = nullptr;
		}

		Chunk::Chunk(const uint8_t* pChunk)
		{
			Read(pChunk);
		}

		Chunk::~Chunk()
		{
			if (m_pChunkData != nullptr)
			{
				delete[] m_pChunkData;
			}
		}

		uint32_t Chunk::SwapBytes(uint32_t uiValue)
		{
			return (uiValue & 0xFF000000) >> 24 | (uiValue & 0x00FF0000) >> 8 | (uiValue & 0x0000FF00) << 8 | (uiValue & 0x000000FF) << 24;
		}

		bool Chunk::Read(const uint8_t* pChunk)
		{
			m_uiChunkSize = SwapBytes(*(uint32_t*)(pChunk + 4 * 0));
			m_uiChunkName = SwapBytes(*(uint32_t*)(pChunk + 4 * 1));
			m_uiChunkCrc = SwapBytes(*(uint32_t*)(pChunk + 4 * 2 + m_uiChunkSize));

			if (m_uiChunkSize != 0)
			{
				m_pChunkData = new uint8_t[m_uiChunkSize];
				memcpy(m_pChunkData, (pChunk + 4 * 2), m_uiChunkSize);
			}
			else
			{
				m_pChunkData = nullptr;
			}

			return true;
		}

		bool Chunk::Write(std::ofstream& refStream)
		{
			uint32_t size_r = SwapBytes(m_uiChunkSize);
			uint32_t name_r = SwapBytes(m_uiChunkName);
			uint32_t crc_r = SwapBytes(m_uiChunkCrc);

			refStream.write(reinterpret_cast<char*>(&size_r), 4);
			refStream.write(reinterpret_cast<char*>(&name_r), 4);
			refStream.write(reinterpret_cast<char*>(m_pChunkData), m_uiChunkSize);
			refStream.write(reinterpret_cast<char*>(&crc_r), 4);

			return true;
		}

		bool Chunk::LoadChunks(std::wstring wsFile, std::list<Chunk>& listChunk)
		{
			//Init File Buffer
			TDA::AutoBuffer buffer(wsFile);
			size_t sizeFile = buffer.GetMaxSize();
			uint8_t* pFile = buffer.GetPointer();
			if (sizeFile == 0 || pFile == nullptr) return false;

			//Read Each Chunk
			size_t iteData = 0;
			if (!memcmp(pFile, MNG_Signature, sizeof(MNG_Signature))) { iteData = sizeof(MNG_Signature); }
			if (!memcmp(pFile, PNG_Signature, sizeof(PNG_Signature))) { iteData = sizeof(PNG_Signature); }
			for (; iteData < sizeFile; iteData += 12)
			{
				Chunk chunk(pFile + iteData);
				iteData += chunk.GetSize();

				listChunk.emplace_back(std::move(chunk));
			}

			return true;
		}


		//****************
		//*    Frame    *
		//****************
		bool Frame::SaveFrame(std::wstring wsFrame)
		{
			//Create File
			std::ofstream ofs(wsFrame, std::ios::binary);
			if (!ofs.is_open()) return false;
			std::ofstream ofs_info(wsFrame + L".INFO", std::ios::binary);
			if (!ofs_info.is_open()) return false;

			//Write PNG Signature
			ofs.write(reinterpret_cast<const char*>(PNG_Signature), sizeof(PNG_Signature));

			//Write PNG Data
			auto saveChunk = [&ofs, &ofs_info](Chunk& chunk) 
			{ 
				switch (chunk.GetName())
				{
				case IDAT:
				case IHDR:
				case IEND:
				{
					chunk.Write(ofs);
				}
				break;

				default:
				{
					chunk.Write(ofs_info);
				}
				break;
				}
			};
			std::for_each(GetBeg(), GetEnd(), saveChunk);

			//Clear
			ofs.flush();
			return true;
		}

		bool Frame::FindFrame(std::list<Chunk>& listChunk, std::vector<Frame>& vecFrame)
		{
			Frame frame;
			auto iteBeg = listChunk.end();
			auto iteEnd = listChunk.end();

			for (auto iteChunk = listChunk.begin(); iteChunk != listChunk.end(); iteChunk++)
			{
				switch (iteChunk->GetName())
				{
				case DEFI: { iteBeg = iteChunk; } break;

				case IHDR: 
				{
					if (iteBeg == listChunk.end()) { iteBeg = iteChunk; }
				}
				break;

				case IEND:
				{
					iteEnd = iteChunk;

					if (iteBeg == listChunk.end()) { return false; }

					frame.SetRange(iteBeg, ++iteEnd);
					vecFrame.emplace_back(frame);

					iteBeg = listChunk.end();
					iteEnd = listChunk.end();
				}
				break;

				}
			}

			return true;
		}

		void Frame::SetRange(std::list<Chunk>::iterator iteIHDR, std::list<Chunk>::iterator iteIEND)
		{
			m_iteBeg = iteIHDR;
			m_iteEnd = iteIEND;
		}


        //****************
        //*     PNG      *
        //****************
		bool PNG::Load()
		{
			return Chunk::LoadChunks(m_wsPNG, m_listChunk);
		}

		bool PNG::Merge()
		{
			std::list<Chunk> listInfoChunk;
			if (!Chunk::LoadChunks(m_wsPNG + L".INFO", listInfoChunk)) { return false; }

			//Find IHDR
			auto iteIHDR = m_listChunk.begin();
			for (auto ite = m_listChunk.begin(); ite != m_listChunk.end(); ite++)
			{
				if (ite->GetName() == IHDR) { iteIHDR = ++ite; break; }
			}

			//Insert INFO
			for (auto pChunk = listInfoChunk.begin(); pChunk != listInfoChunk.end();)
			{
				switch (pChunk->GetName())
				{
				case DEFI:
				{
					m_listChunk.emplace_front(std::move(*pChunk));
					pChunk = listInfoChunk.erase(pChunk);
					continue;
				}
				break;

				default: {	m_listChunk.insert(iteIHDR, *pChunk); }

				}

				pChunk++;
			}

			return true;
		}


		//****************
		//*     MNG      *
		//****************
		bool MNG_Editor::LoadMNG()
		{
			if (!Chunk::LoadChunks(m_wsMNG, m_listChunk)) { return false; }

			if (!Frame::FindFrame(m_listChunk, m_vecFrame)) { return false; }

			return true;
		}

		bool MNG_Editor::SaveMNG()
		{
			std::ofstream ofs(m_wsMNG + L".new", std::ios::binary);
			if (!ofs.is_open()) return false;

			ofs.write(reinterpret_cast<const char*>(MNG_Signature), sizeof(MNG_Signature));

			for (auto& chunk : m_listChunk) { chunk.Write(ofs); }

			ofs.flush();
			return true;
		}

		bool MNG_Editor::BuildMNG()
		{
			std::wstring folder = m_wsMNG.substr(0, m_wsMNG.size() - 4) + L"/";
			std::wstring path = folder + m_wsMNG.substr(0, m_wsMNG.size() - 4);

			if (!LoadMHDR(path + L".MHDR")) return false;

			size_t count = 0;
			for (; ; count++)
			{
				PNG png(path + L"-" + std::to_wstring(count) + L".png");
				if (!png.Load()) { break; }
				if (!png.Merge()) { break; }
				m_listChunk.splice(m_listChunk.end(), png.GetChunkList());
			}

			std::cout << "Repack Files:" << count << std::endl;

			m_listChunk.emplace_back(MNG_END);
			
			return true;
		}

		bool MNG_Editor::LoadMHDR(std::wstring wsFile)
		{
			return Chunk::LoadChunks(wsFile, m_listChunk);
		}

		bool MNG_Editor::SaveMHDR(std::wstring wsFile)
		{
			std::ofstream ofs_MHDR(wsFile, std::ios::binary);
			if (!ofs_MHDR.is_open()) return false;

			for (auto& chunk : m_listChunk)
			{
				if (chunk.GetName() == MHDR)
				{
					chunk.Write(ofs_MHDR);
					ofs_MHDR.flush();
					return true;
				}
			}

			return false;
		}

		bool MNG_Editor::ExtractSingleFrame(std::wstring wsPNG, Frame& refFrame)
		{
			if (refFrame.SaveFrame(wsPNG))
			{
				TDA::ConsoleX::PutConsoleW(L"Save Frame: %s\n", wsPNG.c_str());
				return true;
			}
			else
			{
				TDA::ConsoleX::PutConsoleW(L"Failed Save Frame: %s\n", wsPNG.c_str());
				return false;
			}
		}

		bool MNG_Editor::ExtractMultipleFrame()
		{
			std::wstring folder = m_wsMNG.substr(0, m_wsMNG.size() - 4) + L"/";
			std::wstring path = folder + m_wsMNG.substr(0, m_wsMNG.size() - 4);

			(void)_wmkdir(folder.c_str());

			SaveMHDR(path + L".MHDR");

			std::size_t count = 0;
			for (auto& frame : m_vecFrame) 
			{ 
				ExtractSingleFrame(path + L"-" + std::to_wstring(count++) + L".png", frame); 
			}

			return true;
		}
	}
}