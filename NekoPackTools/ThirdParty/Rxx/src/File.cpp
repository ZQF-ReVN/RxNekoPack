#include "../include/File.h"

#include <Windows.h>
#include <shlobj.h>
#include <stdexcept>


namespace Rut
{
	namespace FileX
	{
		void SaveFileViaPath(const wchar_t* pwPath, void* pData, size_t nBytes)
		{
			std::wstring path = GetCurrentDirectoryFolder_RETW() + pwPath;

			SHCreateDirectoryExW(NULL, PathRemoveFileName_RET(path).c_str(), NULL);

			HANDLE hFile = CreateFileW(pwPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) { throw std::runtime_error("SaveFileViaPath: Create File Error!"); }

			WriteFile(hFile, pData, nBytes, NULL, NULL);
			FlushFileBuffers(hFile);
			CloseHandle(hFile);
		}

		void SaveFileViaPath(const char* pcPath, void* pData, size_t nBytes)
		{
			std::wstring wsPath;
			std::string msPath = pcPath;
			StrX::StrToWStr(msPath, wsPath, CP_ACP);
			SaveFileViaPath(wsPath.c_str(), pData, nBytes);
		}

		void SaveFileViaPath(const std::wstring& wsPath, void* pData, size_t nBytes)
		{
			SaveFileViaPath(wsPath.c_str(), pData, nBytes);
		}

		void SaveFileViaPath(const std::string& msPath, void* pData, size_t nBytes)
		{
			SaveFileViaPath(msPath.c_str(), pData, nBytes);
		}


		std::string GetFileNameViaBaseA(uint32_t uiBase)
		{
			std::string name_ms;
			std::wstring name_ws = GetFileNameViaBaseW(uiBase);
			StrX::WStrToStr(name_ws, name_ms, CP_ACP);
			return name_ms;
		}

		std::wstring GetFileNameViaBaseW(uint32_t uiBase)
		{
			std::wstring name;
			name.resize(MAX_PATH);
			GetModuleFileNameW((HMODULE)uiBase, (wchar_t*)name.data(), MAX_PATH);
			name.resize(PathGetFileName((wchar_t*)name.data()));
			return name;
		}


		size_t GetCurrentDirectoryFolder(char* pPath)
		{
			size_t size = GetCurrentDirectoryA(MAX_PATH, pPath);
			memcpy(pPath + size, "\\", 1);
			return ++size;
		}

		size_t GetCurrentDirectoryFolder(wchar_t* pPath)
		{
			size_t size = GetCurrentDirectoryW(MAX_PATH, pPath);
			memcpy(pPath + size, L"\\", 2);
			return ++size;
		}

		size_t GetCurrentDirectoryFolder(std::string& msPath)
		{
			std::wstring path_ws;
			size_t size = GetCurrentDirectoryFolder(path_ws);
			StrX::WStrToStr(path_ws, msPath, CP_ACP);
			return size;
		}

		size_t GetCurrentDirectoryFolder(std::wstring& wsPath)
		{
			size_t len = 0;
			wsPath.resize(MAX_PATH);
			len = GetCurrentDirectoryFolder(const_cast<wchar_t*>(wsPath.data()));
			wsPath.resize(len);
			return len;
		}

		std::string GetCurrentDirectoryFolder_RETA()
		{
			std::string path_ms; std::wstring path_ws;
			GetCurrentDirectoryFolder(path_ws);
			StrX::WStrToStr(path_ws, path_ms, CP_ACP);
			return path_ms;
		}

		std::wstring GetCurrentDirectoryFolder_RETW()
		{
			std::wstring path;
			GetCurrentDirectoryFolder(path);
			return path;
		}


		char* BackSlash(char* pPath)
		{
			for (size_t ite = 0; pPath[ite]; ite++)
			{
				if ((uint8_t)pPath[ite] > 0x7F) { ite++; continue; }

				switch (pPath[ite])
				{
				case '/':
					pPath[ite] = '\\';
					break;
				case '\\':
					pPath[ite] = '/';
					break;
				}
			}

			return pPath;
		}

		wchar_t* BackSlash(wchar_t* pPath)
		{
			for (size_t ite = 0; pPath[ite]; ite++)
			{
				switch (pPath[ite])
				{
				case L'/':
					pPath[ite] = L'\\';
					break;
				case L'\\':
					pPath[ite] = L'/';
					break;
				}
			}

			return pPath;
		}

		void BackSlash(const char* pPath, std::string& msPath)
		{
			msPath = pPath;
			BackSlash(const_cast<char*>(msPath.data()));
		}

		void BackSlash(const wchar_t* pPath, std::wstring& wsPath)
		{
			wsPath = pPath;
			BackSlash(const_cast<wchar_t*>(wsPath.data()));
		}

		std::string BackSlash_RET(const char* pPath)
		{
			std::string msPath = pPath;
			BackSlash(const_cast<char*>(msPath.data()));
			return msPath;
		}

		std::wstring BackSlash_RET(const wchar_t* pPath)
		{
			std::wstring wsPath = pPath;
			BackSlash(const_cast<wchar_t*>(wsPath.data()));
			return wsPath;
		}


		bool PathRemoveFileName(std::string& msPath, std::string& msRemoved)
		{
			size_t pos = msPath.rfind("\\");
			if (pos == std::wstring::npos) { return false; }
			msRemoved = msPath.substr(0, pos + 1);
			return true;
		}

		bool PathRemoveFileName(std::wstring& wsPath, std::wstring& wsRemoved)
		{
			size_t pos = wsPath.rfind(L"\\");
			if (pos == std::wstring::npos) { return false; }
			wsRemoved = wsPath.substr(0, pos + 1);
			return true;
		}

		std::string PathRemoveFileName_RET(std::string& msPath)
		{
			std::string removed;
			PathRemoveFileName(msPath, removed);
			return removed;
		}

		std::wstring PathRemoveFileName_RET(std::wstring& wsPath)
		{
			std::wstring removed;
			PathRemoveFileName(wsPath, removed);
			return removed;
		}


		std::wstring PathRemoveExtension(const std::wstring& wsPath)
		{
			std::size_t pos = wsPath.rfind(L".");
			if (pos == std::wstring::npos) { throw std::runtime_error("PathRemoveExtension: Not Find Extension!"); }
			return wsPath.substr(0, pos);
		}


		size_t PathGetFileName(char* pPath)
		{
			//StrLen
			size_t len = 0;
			while (pPath[++len]);

			for (size_t iteChar = len; iteChar > 0; iteChar--)
			{
				if (pPath[iteChar - 1] != '\\') continue;
				len -= iteChar;
				memcpy(pPath, pPath + iteChar, len + 1);
				return len;
			}

			return 0;
		}

		size_t PathGetFileName(wchar_t* pPath)
		{
			//StrLen
			size_t len = 0;
			while (pPath[++len]);

			for (size_t iteChar = len; iteChar > 0; iteChar--)
			{
				if (pPath[iteChar - 1] != L'\\') continue;
				len -= iteChar;
				memcpy(pPath, pPath + iteChar, (len + 1) * 2);
				return len;
			}

			return 0;
		}

		void PathGetFileName(std::string& msPath, std::string& msFileName)
		{
			msPath.resize(PathGetFileName(const_cast<char*>(msPath.data())));
			msFileName = msPath;
		}

		void PathGetFileName(std::wstring& wsPath, std::wstring& wsFileName)
		{
			wsPath.resize(PathGetFileName(const_cast<wchar_t*>(wsPath.data())));
			wsFileName = wsPath;
		}

		std::string PathGetFileName_RET(std::string& msPath)
		{
			std::string filename;
			PathGetFileName(msPath, filename);
			return filename;
		}

		std::wstring PathGetFileName_RET(std::wstring& wsPath)
		{
			std::wstring filename;
			PathGetFileName(wsPath, filename);
			return filename;
		}
	}
}