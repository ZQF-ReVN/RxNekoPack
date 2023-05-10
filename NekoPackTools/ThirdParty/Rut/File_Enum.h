#pragma once
#include <string>
#include <vector>


//Ria's Utility Library X
namespace Rut
{
	namespace FileX
	{
		bool GetAllFilesPathA(std::string msBasePath, std::vector<std::string>& vecList);
		bool GetAllFileNameA(std::string msBasePath, std::vector<std::string>& vecList);

		bool GetAllFilesPathW(std::wstring wsBasePath, std::vector<std::wstring>& vecList);
		bool GetAllFileNameW(std::wstring wsBasePath, std::vector<std::wstring>& vecList);
	}
}

