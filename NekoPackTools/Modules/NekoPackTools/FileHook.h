#pragma once
#include <Windows.h>

namespace NekoPackTools
{
	namespace FileHook
	{
		bool SetFileHook(DWORD dwPackResLoadRaw);
		bool SetFileDump(DWORD dwPackResReadRaw);
	}
}