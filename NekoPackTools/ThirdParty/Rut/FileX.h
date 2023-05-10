#pragma once
#include <string>

#include "File_Enum.h"
#include "File_Stream.hpp"


//Ria's Utility Library X
namespace Rut
{
	namespace FileX
	{
		bool         SaveFileViaPath(const char* pPath, void* pBuffer, size_t nSize);
		bool         SaveFileViaPath(const wchar_t* pPath, void* pBuffer, size_t nSize);

		size_t       GetCurrentDirectoryFolder(char* pPath);
		size_t       GetCurrentDirectoryFolder(wchar_t* pPath);
		size_t       GetCurrentDirectoryFolder(std::string& msPath);
		size_t       GetCurrentDirectoryFolder(std::wstring& wsPath);
		std::string  GetCurrentDirectoryFolder_RETA();
		std::wstring GetCurrentDirectoryFolder_RETW();


		void         BackSlash(char* pPath);
		void         BackSlash(wchar_t* pPath);
		void         BackSlash(const char* pPath, std::string& msPath);
		void         BackSlash(const wchar_t* pPath, std::wstring& wsPath);
		std::string  BackSlash_RET(const char* pPath);
		std::wstring BackSlash_RET(const wchar_t* pPath);


		bool         PathRemoveFileName(std::string& msPath, std::string& msRemoved);
		bool         PathRemoveFileName(std::wstring& wsPath, std::wstring& wsRemoved);
		std::string  PathRemoveFileName_RET(std::string& msPath);
		std::wstring PathRemoveFileName_RET(std::wstring& wsPath);


		size_t       PathGetFileName(wchar_t* pPath);
		size_t       PathGetFileName(char* pPath);
		void         PathGetFileName(std::string& msPath, std::string& msFileName);
		void         PathGetFileName(std::wstring& msPath, std::wstring& wsFileName);
		std::string  PathGetFileName_RET(std::string& msPath);
		std::wstring PathGetFileName_RET(std::wstring& wsPath);

	}
}

