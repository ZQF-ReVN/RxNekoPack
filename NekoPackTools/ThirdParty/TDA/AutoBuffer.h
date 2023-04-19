#pragma once
#include <cstdint>
#include <string>

namespace TDA
{
	class AutoBuffer
	{
	private:
		uint8_t* m_pBuffer;
		size_t m_szMaxAlloc;

	public:
		AutoBuffer();
		AutoBuffer(std::wstring wsFile);
		~AutoBuffer();

		uint8_t* ReSize(size_t szRes);
		size_t LoadFile(std::wstring wsFile);

		size_t GetMaxSize() { return m_szMaxAlloc; }
		uint8_t* GetPointer() { return m_pBuffer; }

		void Clear();
	};
}