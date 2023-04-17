#pragma once
#include <cstdint>

namespace TDA
{
	class AutoBuffer
	{
	private:
		uint8_t* m_pBuffer;
		size_t m_szAllocMax;

	public:
		AutoBuffer();
		~AutoBuffer();

		void DelBuffer();
		uint8_t* GetBuffer(size_t szRes);
	};
}