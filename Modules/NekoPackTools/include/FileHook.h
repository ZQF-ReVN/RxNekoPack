#pragma once
#include <cstdint>


namespace NekoPackTools
{
	namespace Pack
	{
		void SetHookFolder(const char* cpFolder);
		void SetReplaceFolder(const char* cpFolder);
		void SetDumpFolder(const char* cpFolder);
		bool SetFileHook_V1(uint32_t fnFindResEntry_V1);
		bool SetFileHook_V2(uint32_t fnLoadFile_V2);
		bool SetFileDump_V2(uint32_t fnLoadFile_V2);
	}
}