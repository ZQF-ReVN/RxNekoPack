#pragma once
#include <Windows.h>

namespace NekoPackTools
{
	namespace Pack
	{
		bool SetFileHook(DWORD dwPackResLoadRaw);
		bool SetFileDump(DWORD dwPackResReadRaw);
	}
}