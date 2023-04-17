#include "AutoBuffer.h"


namespace TDA
{
	AutoBuffer::AutoBuffer() :
		m_pBuffer(nullptr), m_szAllocMax(0)
	{

	}

	AutoBuffer::~AutoBuffer()
	{
		if (m_pBuffer)
		{
			delete[] m_pBuffer;
		}
	}

	uint8_t* AutoBuffer::GetBuffer(size_t szRes)
	{
		if (!m_szAllocMax)
		{
			m_pBuffer = new uint8_t[szRes];
			m_szAllocMax = szRes;
		}

		if (szRes > m_szAllocMax)
		{
			delete[] m_pBuffer;
			m_pBuffer = new uint8_t[szRes];
			m_szAllocMax = szRes;
		}

		return m_pBuffer;
	}

	void AutoBuffer::DelBuffer()
	{
		m_szAllocMax = 0;

		if (m_pBuffer)
		{
			delete[] m_pBuffer;
		}

		m_pBuffer = nullptr;
	}
}