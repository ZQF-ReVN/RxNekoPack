#include "Mem_Auto.h"
#include "FileX.h"

#include <fstream>


namespace Rut
{
	namespace MemX
	{
		AutoMem::AutoMem() : m_pBuffer(nullptr), m_szMaxAlloc(0)
		{

		}

		AutoMem::AutoMem(const AutoMem& buffer)
		{
			if (buffer.m_pBuffer != nullptr)
			{
				this->m_szMaxAlloc = buffer.m_szMaxAlloc;
				this->m_pBuffer = new uint8_t[buffer.m_szMaxAlloc];
				memcpy(m_pBuffer, buffer.m_pBuffer, m_szMaxAlloc);
			}
			else
			{
				this->m_pBuffer = nullptr;
				this->m_szMaxAlloc = 0;
			}
		}

		AutoMem::AutoMem(AutoMem&& buffer) noexcept
		{
			this->m_pBuffer = buffer.m_pBuffer;
			this->m_szMaxAlloc = buffer.m_szMaxAlloc;

			buffer.m_pBuffer = nullptr;
			buffer.m_szMaxAlloc = 0;
		}

		AutoMem::AutoMem(const std::wstring& wsFile) : m_pBuffer(nullptr), m_szMaxAlloc(0)
		{
			LoadFile(wsFile);
		}

		AutoMem::AutoMem(const std::wstring& wsFile, size_t szFile) : m_pBuffer(nullptr), m_szMaxAlloc(0)
		{
			LoadFile(wsFile, szFile);
		}

		AutoMem::~AutoMem()
		{
			Clear();
		}

		uint8_t* AutoMem::ReSize(size_t szRes)
		{
			if (!m_szMaxAlloc)
			{
				m_pBuffer = new uint8_t[szRes];
				m_szMaxAlloc = szRes;
			}

			if (szRes > m_szMaxAlloc)
			{
				delete[] m_pBuffer;
				m_pBuffer = new uint8_t[szRes];
				m_szMaxAlloc = szRes;
			}

			return m_pBuffer;
		}

		void AutoMem::Clear()
		{
			m_szMaxAlloc = 0;

			if (m_pBuffer)
			{
				delete[] m_pBuffer;
			}

			m_pBuffer = nullptr;
		}

		uint8_t* AutoMem::LoadFile(const std::wstring& wsFile)
		{
			return LoadFile(wsFile, -1);
		}

		uint8_t* AutoMem::LoadFile(const std::wstring& wsFile, size_t szFile)
		{
			std::ifstream ifs = Rut::FileX::OpenFileBinaryStream(wsFile);
			if (szFile == -1) { szFile = static_cast<size_t>(FileX::GetFileSize(ifs)); }
			ifs.read(reinterpret_cast<char*>(ReSize(szFile)), szFile);
			return GetPointer();
		}

		void AutoMem::SaveToFile(const std::wstring& wsFile)
		{
			std::ofstream ofs(wsFile, std::ios::binary);
			if (ofs.fail()) { throw std::runtime_error("AutoMem::SaveToFile: Create File Error!"); }

			ofs.write(reinterpret_cast<char*>(GetPointer()), GetMaxSize());
			ofs.flush();
		}
	}
}