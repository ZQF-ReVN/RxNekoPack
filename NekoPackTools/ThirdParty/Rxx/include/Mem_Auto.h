#pragma once
#include <cstdint>
#include <string>

#include "File.h"

//Ria's Utility Library X
namespace Rut
{
	namespace MemX
	{
		class AutoMem
		{
		private:
			uint8_t* m_pBuffer;
			size_t   m_szMaxAlloc;

		public:
			AutoMem();
			AutoMem(const AutoMem& buffer);
			AutoMem(AutoMem&& buffer) noexcept;
			AutoMem(const std::wstring& wsFile);
			AutoMem(const std::wstring& wsFile, size_t szFile);
			~AutoMem();

			uint8_t* ReSize(size_t szRes);
			size_t   GetMaxSize() { return m_szMaxAlloc; }
			uint8_t* GetPointer() { return m_pBuffer; }
			void Clear();

			uint8_t* LoadFile(const std::wstring& wsFile);
			uint8_t* LoadFileViaSize(const std::wstring& wsFile, size_t szFile);
			void     SaveToFile(const std::wstring& wsFile);
		};
	}
}