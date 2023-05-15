#include <Windows.h>

#include "../../ThirdParty/Rxx/include/INI.h"
#include "../../ThirdParty/Rxx/include/File.h"
#include "../../Modules/NekoPackTools/FileHook.h"

static DWORD g_dwExeBase = NULL;
static DWORD g_dwDllBase = NULL;

using namespace Rcf::INI;
using namespace Rut::FileX;
using namespace NekoPackTools::Pack;


VOID StartHook()
{
	std::wstring dll_name_noext = PathRemoveExtension(GetFileNameViaBaseW((uint32_t)g_dwDllBase));

	try
	{
		INI_File ini(dll_name_noext + L".ini");
		KeysMap& neko_hook = ini[L"NekoPack_FileHook"];
		bool isHook = neko_hook[L"SetHook"];
		bool isDump = neko_hook[L"SetDump"];
		uint32_t hook_rva = neko_hook[L"HookRVA"];
		uint32_t dump_rva = neko_hook[L"DumpRVA"];

		if (isHook == true) { SetFileHook(g_dwExeBase + hook_rva); return; }
		if (isDump == true) { SetFileDump(g_dwExeBase + dump_rva); return; }
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