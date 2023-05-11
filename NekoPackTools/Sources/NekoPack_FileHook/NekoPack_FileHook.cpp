#include <Windows.h>

#include "../../ThirdParty/Rcf/INI.hpp"
#include "../../ThirdParty/Rut/FileX.h"
#include "../../Modules/NekoPackTools/FileHook.h"

static DWORD g_dwExeBase = NULL;
static DWORD g_dwDllBase = NULL;

using namespace Rut::FileX;
using namespace NekoPackTools::Pack;

VOID StartHook()
{
	std::wstring dll_name_noext = PathRemoveExtension(GetFileNameViaBaseW((uint32_t)g_dwDllBase));

	try
	{
		Rcf::INI::INI_File ini(dll_name_noext + L".ini");
		bool isHook = ini.Get(L"NekoPack_FileHook", L"SetHook");
		bool isDump = ini.Get(L"NekoPack_FileHook", L"SetDump");
		int hook_rva = ini.Get(L"NekoPack_FileHook", L"HookRVA");
		int dump_rva = ini.Get(L"NekoPack_FileHook", L"DumpRVA");

		if (isHook == true) { SetFileHook(g_dwExeBase + hook_rva); return; }
		if (isDump == true) { SetFileDump(g_dwExeBase + dump_rva); return; }
	}
	catch (const std::runtime_error& err)
	{
		std::ofstream ofs(dll_name_noext + L".log");
		ofs << err.what() << std::endl;
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