#include <Windows.h>

#include "../../lib/Rxx/INI.h"
#include "../../lib/Rxx/Str.h"
#include "../../lib/Rxx/File.h"
#include "../../lib/Rxx/Hook.h"
#include "../../lib/NekoPackTools/FileHook.h"


static DWORD g_dwExeBase = NULL;
static DWORD g_dwDllBase = NULL;

using namespace Rcf::INI;
using namespace Rut::StrX;
using namespace Rut::FileX;
using namespace Rut::HookX;
using namespace NekoPackTools::Pack;


char* SaveString(const std::string& msStr)
{
	char* pStr = new char[msStr.size() + 1];
	memcpy(pStr, msStr.c_str(), msStr.size() + 1);
	return pStr;
}

VOID StartHook()
{
	std::wstring dll_name_noext = PathRemoveExtension(GetModuleNameViaBaseW((uint32_t)g_dwDllBase));

	try
	{
		INI_File ini(dll_name_noext + L".ini");
		bool is_hook_file = ini[L"Control"][L"HookFile"];
		bool is_dump_file = ini[L"Control"][L"DumpFile"];
		bool is_hook_font = ini[L"Control"][L"HookFont"];
		std::wstring game_select = ini[L"Control"][L"GameSelect"];
		std::uint32_t version = ini[game_select][L"Version"];

		if (is_hook_font) 
		{ 
			HookCreateFontIndirectA(ini[L"Font"][L"Charset"], SaveString(ini[L"Font"][L"FontName"]));
		}

		if (version == 3)
		{
			uint32_t fn_load_file_rva = ini[game_select][L"LoadFile"];

			if (is_hook_file == true)
			{ 
				SetHookFolder(SaveString(ini[L"Folder_V3"][L"HookFolder"]));
				SetReplaceFolder(SaveString(ini[L"Folder_V3"][L"HookFolder"]));
				SetFileHook_V3(g_dwExeBase + fn_load_file_rva);
				return;
			}

			if (is_dump_file == true)
			{ 
				SetDumpFolder(SaveString(ini[L"Folder_V3"][L"DumpFolder"]));
				SetFileDump_V3(g_dwExeBase + fn_load_file_rva);
				return; 
			}
		}

		if (version == 1)
		{
			uint32_t fn_find_entry_rva = ini[game_select][L"FindEntry"];

			SetHookFolder(SaveString(ini[L"Folder_V1"][L"HookFolder"]));
			SetReplaceFolder(SaveString(ini[L"Folder_V1"][L"ReplaceFolder"]));
			SetFileHook_V1(g_dwExeBase + fn_find_entry_rva); 
			return;
		}
	}
	catch (const std::runtime_error& err)
	{
		MessageBoxA(NULL, err.what(), "NekoPack_FileHook", NULL);
		exit(-1);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_dwExeBase = (DWORD)GetModuleHandleW(NULL);
		g_dwDllBase = (DWORD)hModule;
		StartHook();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" VOID __declspec(dllexport) Dir_A() {}