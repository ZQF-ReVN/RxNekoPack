#include <Windows.h>

#include "../../Modules/NekoPackTools/FileHook.h"


static DWORD g_dwExeBase = (DWORD)GetModuleHandleW(NULL);


VOID StartHook()
{
	//少女マイノリティ-慰めの愛- patch 101
	NekoPackTools::FileHook::SetFileHook(g_dwExeBase + 0x7CE4C);
	//NekoPackTools::FileHook::SetFileDump(g_dwExeBase + 0x7AA60);

	//少女アクティビティ patch 102
	//NekoPackTools::FileHook::SetFileHook(g_dwExeBase + 0xF3684);
	//NekoPackTools::FileHook::SetFileDump(g_dwExeBase + 0xF13E8);

	//ソラコイ
	//NekoPackTools::FileHook::SetFileHook(g_dwExeBase + 0xF3BA8);
	//NekoPackTools::FileHook::SetFileDump(g_dwExeBase + 0xF190C);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
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