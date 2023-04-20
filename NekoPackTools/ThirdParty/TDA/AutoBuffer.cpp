#include "AutoBuffer.h"
#include "FileX.h"

#include <fstream>

namespace TDA
{
	AutoBuffer::AutoBuffer() : m_pBuffer(nullptr), m_szMaxAlloc(0)
	{

	}

	AutoBuffer::AutoBuffer(const AutoBuffer& buffer)
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

	AutoBuffer::AutoBuffer(AutoBuffer&& buffer) noexcept
	{
		this->m_pBuffer = buffer.m_pBuffer;
		this->m_szMaxAlloc = buffer.m_szMaxAlloc;

		buffer.m_pBuffer = nullptr;
		buffer.m_szMaxAlloc = 0;
	}

	AutoBuffer::AutoBuffer(std::wstring wsFile) : m_pBuffer(nullptr), m_szMaxAlloc(0)
	{
		LoadFile(wsFile);
	}

	AutoBuffer::~AutoBuffer()
	{
		Clear();
	}

	uint8_t* AutoBuffer::ReSize(size_t szRes)
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

	void AutoBuffer::Clear()
	{
		m_szMaxAlloc = 0;

		if (m_pBuffer)
		{
			delete[] m_pBuffer;
		}

		m_pBuffer = nullptr;
	}

	size_t AutoBuffer::LoadFile(std::wstring wsFile)
	{
		std::ifstream ifs(wsFile, std::ios::binary);
		if (!ifs.is_open()) return 0;

		size_t sizeFile = static_cast<size_t>(FileX::GetFileSize(ifs));
		ifs.read((char*)ReSize(sizeFile), sizeFile);

		return sizeFile;
	}
}