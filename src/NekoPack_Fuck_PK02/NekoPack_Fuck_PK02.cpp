#include <iostream>

#include "../../lib/Rxx/Mem.h"
#include "../../lib/Rxx/File.h"

using namespace Rut::MemX;
using namespace Rut::FileX;


#pragma pack(4)
struct PK02_HDR
{
	uint32_t uiSignatur;       // "PK02"
	uint64_t uiEntryFOA;
};

struct PK02_Folder_Entry
{
	uint64_t uiFolderCount;
	uint64_t uiFileCount;
	uint64_t uiSubEntryFOA;
	uint64_t uiFileIndexFOA;
	//uint8_t  ucFolder[? ? ];
};

//struct PK02_File_Entry
//{
//	uint64_t uiSize;                 // decomp size
//	uint64_t uiFOA;
//	uint8_t	 ucCompFlag;             // Always 0x1 ?
//	//uint8_t	 ucFileName[? ? ];   // NULL END
//};

static uint8_t* sg_uiPackPtr;
static AutoMem sg_amDecompBuf;

class PK02_File
{
public:
	uint32_t m_uiSize;
	uint32_t m_uiFOA;
	uint8_t  m_uiCompFlag;
	std::string m_msFileName;
	std::string m_msFilePath;

private:
	void Decode(uint8_t* pEnc, uint8_t* pDec, uint32_t uiDecSize)
	{
		uint8_t* enc_ptr = pEnc;
		uint8_t* dec_ptr = pDec;

		while (true)
		{
			if ((int32_t)(dec_ptr - pDec) >= (int32_t)uiDecSize) { break; }

			uint8_t token = enc_ptr++[0];
			uint32_t data_size = token >> 4;
			uint32_t copy_size = token & 0xF;

			if (data_size == 0xF)
			{
				uint8_t add_size = 0;
				do
				{
					add_size = enc_ptr++[0];
					data_size += add_size;
				} while (add_size == 0xFF);
			}


			uint8_t* data_ptr = enc_ptr;
			enc_ptr += data_size;
			uint16_t copy_offset = *(uint16_t*)enc_ptr;
			enc_ptr += 2;

			if (copy_size == 0xF)
			{
				uint8_t add_size = 0;
				do
				{
					add_size = enc_ptr++[0];
					copy_size += add_size;
				} while (add_size == 0xFF);
			}

			copy_size += 4;

			memcpy(dec_ptr, data_ptr, data_size);
			dec_ptr += data_size;

			if ((int32_t)(dec_ptr - pDec) >= (int32_t)uiDecSize) { break; }

			for (uint32_t ite_byte = 0; ite_byte < copy_size; ite_byte++)
			{
				dec_ptr[0] = dec_ptr[-copy_offset];
				dec_ptr++;
			}
		}
	}

public:
	PK02_File()
	{
		m_uiSize = 0;
		m_uiFOA = 0;
		m_uiCompFlag = 0;
	}

	PK02_File(uint64_t* pFile, std::string msFolder)
	{
		this->m_uiSize = (uint32_t)pFile[0];
		this->m_uiFOA = (uint32_t)pFile[1];
		pFile += 2;

		this->m_uiCompFlag = *(uint8_t*)pFile;

		this->m_msFileName = ((char*)pFile + 1);
		this->m_msFilePath = msFolder.substr(2) + m_msFileName;

		Decode((uint8_t*)(sg_uiPackPtr + m_uiFOA), sg_amDecompBuf[m_uiSize], m_uiSize);
		SaveFileViaPath(m_msFilePath.c_str(), sg_amDecompBuf, m_uiSize);
		std::cout << "Extract: " << m_msFilePath << '\n';
	}
};

class PK02_Folder
{
public:
	std::string m_msFolderName;
	std::string m_msFolderPath;
	std::vector<PK02_File> m_vecFile;
	std::vector<PK02_Folder> m_vecFolder;


public:
	PK02_Folder(uint64_t* pFolder, const std::string& msFolder)
	{
		uint64_t folder_count = pFolder[0];
		uint64_t folder_index_foa = pFolder[2];
		uint64_t file_count = pFolder[1];
		uint64_t file_index_foa = pFolder[3];
		pFolder += 4;
		m_msFolderName = (char*)pFolder;
		m_msFolderPath = msFolder + m_msFolderName;

		if (folder_count != 0)
		{
			uint32_t next_size = 0;
			for (size_t ite_folder = 0; ite_folder < folder_count; ite_folder++)
			{
				PK02_Folder folder((uint64_t*)(sg_uiPackPtr + folder_index_foa + next_size), m_msFolderPath + "/");
				m_vecFolder.emplace_back(folder);
				next_size += 32 + folder.m_msFolderName.size() + 1;
			}
		}

		if (file_count != 0)
		{
			uint32_t next_size = 0;
			for (size_t ite_file = 0; ite_file < file_count; ite_file++)
			{
				PK02_File file((uint64_t*)(sg_uiPackPtr + file_index_foa + next_size), m_msFolderPath + "/");
				m_vecFile.emplace_back(file);
				next_size += 16 + file.m_msFileName.size() + 2;
			}
		}
	}
};

int main() 
{
	AutoMem pack_buf(L"sorakoi_chs.pk2");
	sg_uiPackPtr = pack_buf;

	PK02_HDR* hdr = (PK02_HDR*)sg_uiPackPtr;
	uint8_t* folder_entry = sg_uiPackPtr + hdr->uiEntryFOA;

	PK02_Folder folder((uint64_t*)folder_entry, "");
}