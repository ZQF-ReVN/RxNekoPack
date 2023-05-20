#include <Windows.h>

#include "../../ThirdParty/Rxx/include/INI.h"
#include "../../ThirdParty/Rxx/include/Str.h"
#include "../../ThirdParty/Rxx/include/File.h"
#include "../../ThirdParty/Rxx/include/Hook.h"
#include "../../Modules/NekoPackTools/FileHook.h"


static DWORD g_dwExeBase = NULL;
static DWORD g_dwDllBase = NULL;

using namespace Rcf::INI;
using namespace Rut::StrX;
using namespace Rut::FileX;
using namespace Rut::HookX;
using namespace NekoPackTools::Pack;


char* SaveString(const std::wstring& wsString)
{
	std::string str = WStrToStr(wsString, CP_ACP);
	char* pStr = new char[str.size() + 1];
	memcpy(pStr, str.c_str(), str.size() + 1);
	return pStr;
}


VOID StartHook()
{
	std::wstring dll_name_noext = PathRemoveExtension(GetFileNameViaBaseW((uint32_t)g_dwDllBase));

	try
	{
		INI_File ini(dll_name_noext + L".ini");
		auto& neko_hook = ini[L"NekoPack_FileHook"];
		auto& neko_config = ini[L"Config"];

		uint32_t version = neko_hook[L"Version"];

		if (version == 2)
		{
			uint32_t fn_load_file_rva = neko_hook[L"LoadFile"];

			bool isHook = neko_config[L"SetHook"];
			bool isDump = neko_config[L"SetDump"];

			if (isHook == true) 
			{ 
				char* cs_hook_folder = SaveString(neko_config[L"HookFolder"]);
				SetHookFolder(cs_hook_folder);
				SetReplaceFolder(cs_hook_folder);
				SetFileHook_V2(g_dwExeBase + fn_load_file_rva);
				return;
			}

			if (isDump == true) 
			{ 
				SetDumpFolder(SaveString(neko_config[L"DumpFolder"]));
				SetFileDump_V2(g_dwExeBase + fn_load_file_rva);
				return; 
			}
		}

		if (version == 1)
		{
			bool is_hook_font = neko_config[L"SetFontHook"];
			if (is_hook_font)
			{
				HookCreateFontIndirectA(neko_config[L"Charset"], SaveString(neko_config[L"FontName"]));
			}

			uint32_t fn_find_entry_rva = neko_hook[L"FindEntry"];

			SetHookFolder(SaveString(neko_config[L"HookFolder"]));
			SetReplaceFolder(SaveString(neko_config[L"ReplaceFolder"]));
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