#pragma once
#include <string>

#include "String.h"
#include "File_Enum.h"
#include "File_Stream.hpp"

//Ria's Utility Library X
namespace Rut
{
	namespace FileX
	{
		void         SaveFileViaPath(const char* pwPath, void* pData, size_t nBytes);
		void         SaveFileViaPath(const wchar_t* pcPath, void* pData, size_t nBytes);
		void         SaveFileViaPath(const std::wstring& wsPath, void* pData, size_t nBytes);
		void         SaveFileViaPath(const std::string& msPath, void* pData, size_t nBytes);

		std::string  GetFileNameViaBaseA(uint32_t uiBase);
		std::wstring GetFileNameViaBaseW(uint32_t uiBase);

		size_t       GetCurrentDirectoryFolder(char* pPath);
		size_t       GetCurrentDirectoryFolder(wchar_t* pPath);
		size_t       GetCurrentDirectoryFolder(std::string& msPath);
		size_t       GetCurrentDirectoryFolder(std::wstring& wsPath);
		std::string  GetCurrentDirectoryFolder_RETA();
		std::wstring GetCurrentDirectoryFolder_RETW();


		char*		 BackSlash(char* pPath);
		wchar_t*	 BackSlash(wchar_t* pPath);
		void         BackSlash(const char* pPath, std::string& msPath);
		void         BackSlash(const wchar_t* pPath, std::wstring& wsPath);
		std::string  BackSlash_RET(const char* pPath);
		std::wstring BackSlash_RET(const wchar_t* pPath);


		bool         PathRemoveFileName(std::string& msPath, std::string& msRemoved);
		bool         PathRemoveFileName(std::wstring& wsPath, std::wstring& wsRemoved);
		std::string  PathRemoveFileName_RET(std::string& msPath);
		std::wstring PathRemoveFileName_RET(std::wstring& wsPath);


		std::wstring PathRemoveExtension(const std::wstring& wsPath);


		size_t       PathGetFileName(wchar_t* pPath);
		size_t       PathGetFileName(char* pPath);
		void         PathGetFileName(std::string& msPath, std::string& msFileName);
		void         PathGetFileName(std::wstring& msPath, std::wstring& wsFileName);
		std::string  PathGetFileName_RET(std::string& msPath);
		std::wstring PathGetFileName_RET(std::wstring& wsPath);
	}
}

