#include "NEKOPACK_V1.h"
#include "../../ThirdParty/Rut/MemX.h"
#include "../../ThirdParty/Rut/FileX.h"

#include <fstream>
#include <direct.h>

namespace NekoPackTools
{
	namespace Pack
	{
		bool NEKOPACK_V1::LoadIndex()
		{
			std::ifstream ifs_Pack(m_msPack, std::ios::binary);
			if (!ifs_Pack.is_open()) return false;

			ifs_Pack.read((char*)&m_Header, sizeof(m_Header));

			uint32_t aTable[625] = { 0 };
			MakeTable(aTable, 0x9999);

			NEKOPACK_V1_Entry entry = { 0 };
			for (size_t iteRes = 0; iteRes < m_Header.uiResCount; iteRes++)
			{
				ifs_Pack.read((char*)&entry.ucKey, 1);
				ifs_Pack.read((char*)&entry.ucResNameLen, 1);
				ifs_Pack.read((char*)entry.aResName, entry.ucResNameLen);
				ifs_Pack.read((char*)&entry.uiResOffset, 4);
				ifs_Pack.read((char*)&entry.uiResSize, 4);

				DeocdeBuffer(aTable, entry.aResName, entry.ucResNameLen);

				m_vecIndex.emplace_back(entry);

				memset(&entry, 0, sizeof(entry));
			}

			return true;
		}

		bool NEKOPACK_V1::Extract()
		{
			if (!LoadIndex()) { return false; }

			std::ifstream ifs_Pack(m_msPack, std::ios::binary);
			if (!ifs_Pack.is_open()) return false;

			std::string folder = m_msPack.substr(0, m_msPack.size() - 4) + "\\";

			uint32_t aTable[625] = { 0 };

			Rut::MemX::AutoMem buffer;
			for (const auto& entry : m_vecIndex)
			{
				uint8_t* pBuffer = buffer.ReSize(entry.uiResSize);

				ifs_Pack.seekg(entry.uiResOffset, std::ios::beg);
				ifs_Pack.read((char*)pBuffer, entry.uiResSize);

				if (entry.ucKey)
				{
					MakeTable(aTable, 0x9999 + entry.ucKey);
					DeocdeBuffer(aTable, pBuffer, entry.uiResSize);
				}

				Rut::FileX::SaveFileViaPath((folder + (char*)entry.aResName).c_str(), pBuffer, entry.uiResSize);
			}

			return true;
		}

		bool NEKOPACK_V1::Create()
		{
			return false;
		}

		void NEKOPACK_V1::MakeTable(uint32_t* pBuffer, int32_t nMagic)
		{
			uint32_t* buffer = pBuffer;

			for (size_t iteTable = 0; iteTable < 624; iteTable++)
			{
				nMagic = nMagic * 0x10DCD;
				buffer++[0] = nMagic;
			}

			pBuffer[624] = 0;
		}

		void NEKOPACK_V1::DeocdeByte(uint32_t* pTable, uint8_t* pByte)
		{
			if (pTable[624] >= 624) { MakeTable(pTable, pTable[623]); }

			int32_t v2 = pTable[624];
			pTable[624] += 1;
			uint32_t v3 = 
				(((((pTable[v2] >> 11) ^ pTable[v2]) << 7) & 
				0x31518A63 ^ (pTable[v2] >> 11) ^ pTable[v2]) << 15) &
				0x17F1CA43 ^ (((pTable[v2] >> 11) ^ pTable[v2]) << 7) & 
				0x31518A63 ^ (pTable[v2] >> 11) ^ pTable[v2];

			*pByte = *pByte ^ (v3 >> 18) ^ v3;
		}

		void NEKOPACK_V1::DeocdeBuffer(uint32_t* pTable, uint8_t* pBuffer, uint32_t uiBufferSize)
		{
			for (size_t iteBuffer = 0; iteBuffer < uiBufferSize; iteBuffer++)
			{
				DeocdeByte(pTable, pBuffer + iteBuffer);
			}
		}

		void NEKOPACK_V1::EncodeBuffer(uint32_t* pTable, uint8_t* pBuffer, uint32_t uiBufferSize)
		{
			DeocdeBuffer(pTable, pBuffer, uiBufferSize);
		}
	}
}