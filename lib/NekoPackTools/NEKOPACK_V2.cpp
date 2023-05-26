#include "NEKOPACK_V2.h"
#include "../Rxx/File.h"
#include "../Rxx/Mem.h"
#include "../Rxx/Console.h"

#include <iostream>


namespace NekoPackTools
{
	namespace Pack
	{
		using namespace Rut::MemX;
		using namespace Rut::FileX;
		using namespace Rut::ConsoleX;

		static constexpr uint32_t ResourceType = 3;
		static constexpr uint32_t DecodeTableType = 1;


		NEKOPACK_V2::NEKOPACK_V2(const std::string& msPack) : m_wsPack(msPack), m_pDecTable(nullptr)
		{
			m_ifsPack = OpenFileBinaryStream(msPack);
		}

		NEKOPACK_V2::~NEKOPACK_V2()
		{
			if (m_pDecTable) { delete[] m_pDecTable; }
		}


		void NEKOPACK_V2::Extract()
		{
			ReadTable();
			ReadIndex();

			// The res files in pack are fully 4byts aligned
			AutoMem buffer;
			for (auto& entry : m_vcIndex)
			{
				uint8_t* res_buf = buffer[entry.uiResSize];
				m_ifsPack.seekg(entry.uiResOffset, std::ios::beg);
				m_ifsPack.read((char*)res_buf, entry.uiResSize);
				BufferDecode(ResourceType, (uint32_t*)res_buf, m_pDecTable, entry.uiResKey, entry.uiResSize);
				SaveFileViaPath((char*)entry.aResName, res_buf, entry.uiResSize);
				PrintInfo(entry);
			}
		}

		void NEKOPACK_V2::PrintInfo(const NEKOPACK_V2_Entry& eEntry)
		{
			PutConsoleA("Key:0x%X\n", eEntry.uiResKey);
			PutConsoleA("Name:%s\n", eEntry.aResName);
			PutConsoleA("Size:0x%X\n", eEntry.uiResSize);
			PutConsoleA("Offset:0x%X\n\n", eEntry.uiResOffset);
		}


		void NEKOPACK_V2::ReadTable()
		{
			m_ifsPack.seekg(0xC, std::ios::beg); // mov pointer to decode table

			uint32_t dec_table_key = 0;
			uint8_t* dec_table_buf = new uint8_t[0x400];
			m_ifsPack.read((char*)(&dec_table_key), 0x4);
			m_ifsPack.read((char*)dec_table_buf, 0x400);

			uint32_t dec_round = (dec_table_key % 7) + 3;
			while (dec_round-->0) 
			{ 
				BufferDecode(DecodeTableType, (uint32_t*)dec_table_buf, (uint8_t*)dec_table_buf, dec_table_key, 0x400);
			}

			m_pDecTable = dec_table_buf;
		}

		void NEKOPACK_V2::ReadIndex()
		{
			m_ifsPack.seekg(0x410, std::ios::beg); // mov pointer to index

			uint32_t index_key = 0;
			uint32_t index_size = 0;
			GetKeyAndSize(index_key, index_size);

			uint8_t* index_buf = new uint8_t[index_size];
			m_ifsPack.read((char*)index_buf, index_size);
			BufferDecode(ResourceType, (uint32_t*)index_buf, m_pDecTable, index_key, index_size);

			uint32_t folder_count = *(uint32_t*)index_buf;
			uint8_t* index_ptr = index_buf + 4;
			NEKOPACK_V2_Entry entry = { 0 };
			for (size_t ite_folder = 0; ite_folder < folder_count; ite_folder++) // Iterate folders
			{
				uint8_t folder_name_size = index_ptr[0];
				uint8_t* folder_name_ptr = index_ptr + 1;
				uint32_t res_count = *(uint32_t*)(index_ptr + 1 + folder_name_size);
				index_ptr += 1 + folder_name_size + 4;
				for (size_t ite_res = 0; ite_res < res_count; ite_res++)  // Iterate resources
				{
					if (*index_ptr) { throw std::runtime_error("NEKOPACK_V1_5::ReadIndex: Read Index Error!"); }
					index_ptr += 1;

					uint8_t res_name_size = index_ptr[0];
					uint8_t* res_name_ptr = index_ptr + 1;
					uint32_t res_offset = *(uint32_t*)(index_ptr + 1 + res_name_size);
					index_ptr += 1 + res_name_size + 4;

					memcpy(entry.aResName, folder_name_ptr, folder_name_size);
					entry.aResName[folder_name_size] = '/';
					memcpy(entry.aResName + folder_name_size + 1, res_name_ptr, res_name_size);
					entry.aResName[folder_name_size + res_name_size + 1] = '\0';
					entry.uiResOffset = res_offset + 0x8 + 0x4 + 0x4 + 0x400 + 0x4 + 0x8 + index_size;

					m_ifsPack.seekg(entry.uiResOffset, std::ios::beg);
					GetKeyAndSize(entry.uiResKey, entry.uiResSize);
					entry.uiResOffset += 12;
					FormatSlash_Ptr((char*)entry.aResName, '\\');
					m_vcIndex.push_back(entry);
				}
			}

			delete[] index_buf;
			index_buf = nullptr;
			index_ptr = nullptr;
		}

		// buffer requires 4 bytes of alignment
		void NEKOPACK_V2::BufferDecode(uint32_t nType, uint32_t* pEnc, uint8_t* pTable, uint32_t nKey, uint32_t nSize)
		{
			uint32_t uint = 0;
			uint32_t blocks = nSize / 4;

			for (size_t block = 0; block < blocks; block++)
			{
				nKey += 0xEB0974C3;
				nKey <<= 0x17; nKey >>= 0x17;
				uint = *pEnc ^ *(uint32_t*)(pTable + nKey);

				switch (nType)
				{
				case DecodeTableType: { nKey += *pEnc; *pEnc = uint; } break;
				case ResourceType: { *pEnc = uint; nKey += *pEnc; } break;
				default: throw std::runtime_error("NEKOPACK_V1_5::BufferDecode: Error Type!");
				}

				pEnc++;
			}
		}

		void NEKOPACK_V2::GetKeyAndSize(uint32_t& nKey, uint32_t& nSize)
		{
			uint32_t key = 0;
			uint32_t size[2] = { 0 };
			m_ifsPack.read((char*)(&key), 0x4);
			m_ifsPack.read((char*)(&size), 0x8);

			BufferDecode(3, size, m_pDecTable, key, 8);

			if (size[0] != size[1]) { throw std::runtime_error("NEKOPACK_V1_5::GetKeyAndSize: Error!"); }

			nKey = key;
			nSize = size[0];
		};
	}
}