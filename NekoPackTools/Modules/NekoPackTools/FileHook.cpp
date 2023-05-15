#include "FileHook.h"
#include "../../ThirdParty/Rxx/include/Hook.h"
#include "../../ThirdParty/Rxx/include/File.h"


using namespace Rut::HookX;
using namespace Rut::FileX;


namespace NekoPackTools
{
	namespace Pack
	{
		//*************
		//* Find File *
		//*************
		static CHAR g_aHook_Folder[MAX_PATH] = { 0 };
		static CHAR g_aHook_RelativePath[MAX_PATH] = { 0 };
		BOOL FileExist(LPCSTR lpFolder, LPCSTR lpResName)
		{
			lstrcpyA(g_aHook_Folder, "FileHook/");
			lstrcatA(g_aHook_Folder, lpFolder); 

			lstrcpyA(g_aHook_RelativePath, g_aHook_Folder);
			lstrcatA(g_aHook_RelativePath, "/");
			lstrcatA(g_aHook_RelativePath, lpResName);

			DWORD attri = GetFileAttributesA(g_aHook_RelativePath);
			if ((attri != INVALID_FILE_ATTRIBUTES) && (attri != FILE_ATTRIBUTE_DIRECTORY))
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}


		//********************
		//* Hook PackResLoad *
		//********************
		typedef DWORD(__cdecl* fnPackResLoad)(DWORD dwUn0, DWORD dwUn1, LPSTR pFolder, LPSTR pResName);
		static fnPackResLoad PackResLoadRaw = nullptr;

		DWORD __cdecl PackResLoadNew(DWORD dwUn0, DWORD dwUn1, LPSTR pFolder, LPSTR pResName)
		{
			if (FileExist(pFolder, pResName))
			{
				pFolder = g_aHook_Folder;
			}
			return PackResLoadRaw(dwUn0, dwUn1, pFolder, pResName);
		}

		//********************
        //* Hook PackResRead *
        //********************
		struct ResStream
		{
			DWORD dwExceptionHandler;
			DWORD dwUn1;
			DWORD dwUn2;
			DWORD dwBuffer;
			DWORD dwSize;
		};

		typedef DWORD(__cdecl* pPackResRead)(DWORD dwUn0, LPCSTR lpFolder, LPCSTR lpResName, ResStream* pStream);
		static pPackResRead PackResReadRaw = nullptr;

		static CHAR g_aDump_RelativePath[MAX_PATH] = { 0 };
		DWORD __cdecl PackResReadNew (DWORD dwUn0, LPCSTR lpFolder, LPCSTR lpResName, ResStream* pStream)
		{
			DWORD pInfo = PackResReadRaw(dwUn0, lpFolder, lpResName, pStream);

			if (pInfo)
			{
				lstrcpyA(g_aDump_RelativePath, "Dump/");
				lstrcatA(g_aDump_RelativePath, lpFolder);

				BackSlash(g_aDump_RelativePath);

				lstrcatA(g_aDump_RelativePath, "\\");
				lstrcatA(g_aDump_RelativePath, lpResName);

				SaveFileViaPath(g_aDump_RelativePath, (void*)pStream->dwBuffer, pStream->dwSize);
			}

			return pInfo;
		}

		//***************
		//* Export Func *
		//***************
		bool SetFileHook(DWORD dwPackResLoadRaw)
		{
			PackResLoadRaw = (fnPackResLoad)dwPackResLoadRaw;
			return DetourAttachFunc(&PackResLoadRaw, PackResLoadNew);
		}

		bool SetFileDump(DWORD dwPackResReadRaw)
		{
			PackResReadRaw = (pPackResRead)dwPackResReadRaw;
			return DetourAttachFunc(&PackResReadRaw, PackResReadNew);
		}
	}
}