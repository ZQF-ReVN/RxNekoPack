#include <Windows.h>

#include "../../Modules/NekoPackTools/FileHook.h"


static DWORD g_dwExeBase = (DWORD)GetModuleHandleW(NULL);


VOID StartHook()
{
	//ノストラダムスに聞いてみろ♪
	//NekoPackTools::Pack::SetFileHook(g_dwExeBase + 0x348C8);
	//NekoPackTools::Pack::SetFileDump(g_dwExeBase + 0x33D6C);

	//Stellar☆Theater patch 102
    //NekoPackTools::Pack::SetFileHook(g_dwExeBase + 0xA8388);
    //NekoPackTools::Pack::SetFileDump(g_dwExeBase + 0xA78A4);

	//Stellar☆Theater encore
    //NekoPackTools::Pack::SetFileHook(g_dwExeBase + 0x211108);
    //NekoPackTools::Pack::SetFileDump(g_dwExeBase + 0x20F6AC);

	//少女マイノリティ-慰めの愛- patch 101
	//NekoPackTools::Pack::SetFileHook(g_dwExeBase + 0x7CE4C);
	//NekoPackTools::Pack::SetFileDump(g_dwExeBase + 0x7AA60);

	//少女アクティビティ patch 102
	//NekoPackTools::Pack::SetFileHook(g_dwExeBase + 0xF3684);
	//NekoPackTools::Pack::SetFileDump(g_dwExeBase + 0xF13E8);

	//ソラコイ
	NekoPackTools::Pack::SetFileHook(g_dwExeBase + 0xF3BA8);
	//NekoPackTools::Pack::SetFileDump(g_dwExeBase + 0xF190C);
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