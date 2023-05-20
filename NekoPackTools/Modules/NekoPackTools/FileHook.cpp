#include "FileHook.h"
#include "../../ThirdParty/Rxx/include/Hook.h"
#include "../../ThirdParty/Rxx/include/File.h"

#include <Windows.h>

using namespace Rut::HookX;
using namespace Rut::FileX;


namespace NekoPackTools
{
	namespace Pack
	{
		static const char* sg_cpHookFolder = nullptr;
		static const char* sg_cpDumpFolder = nullptr;
		static const char* sg_cpReplaceFolder = nullptr;

		struct ResStream
		{
			uint32_t uiExceptionHandler;
			uint32_t uiUn1;
			uint32_t uiUn2;
			uint32_t uiBuffer;
			uint32_t uiSize;
		};

		typedef uint32_t (__cdecl* pLoadFile_V2)(uint32_t* pStaticList, uint32_t* pStruct, const char* cpFolder, const char* cpResName);
		static pLoadFile_V2 sg_fnLoadFile_V2 = nullptr;

		typedef uint32_t(__cdecl* pFindResEntry)(uint32_t* pStruct, const char* cpFolder, const char* cpResName);
		static pFindResEntry sg_fnFindResEntry_V1 = nullptr;



		char* MakeFolder(const char* aBaseFolder, const char* cpFolder)
		{
			static char buffer[MAX_PATH] = { 0 };
			lstrcpyA(buffer, aBaseFolder);
			lstrcatA(buffer, cpFolder);
			return buffer;
		}

		char* MakePath(const char* aBaseFolder, const char* cpFolder, const char* cpResName)
		{
			char* path = MakeFolder(aBaseFolder, cpFolder);
			lstrcatA(path, "/");
			lstrcatA(path, cpResName);
			return path;
		}

		bool FileExist(const char* cpFolder, const char* cpResName)
		{
			return (GetFileAttributesA(MakePath(sg_cpReplaceFolder, cpFolder, cpResName)) == INVALID_FILE_ATTRIBUTES) ? false : true;
		}

		uint32_t HookFile_V2(uint32_t* pStaticList, uint32_t* pStruct, const char* cpFolder, const char* cpResName)
		{
			if (FileExist(cpFolder, cpResName)) { cpFolder = MakeFolder(sg_cpHookFolder, cpFolder); }
			return sg_fnLoadFile_V2(pStaticList, pStruct, cpFolder, cpResName);
		}

		uint32_t HookFile_V1(uint32_t* pStruct, const char* cpFolder, const char* cpResName)
		{
			if (FileExist(cpFolder, cpResName)) { cpFolder = MakeFolder(sg_cpHookFolder, cpFolder); }
			return sg_fnFindResEntry_V1(pStruct, cpFolder, cpResName);
		}

		uint32_t* GetStruct(uint32_t* pStruct)
		{
			uint32_t* strutc_1 = (uint32_t*)pStruct[3];
			if (!strutc_1) return nullptr;
			uint32_t* strutc_2 = (uint32_t*)strutc_1[2];
			if (!strutc_2) return nullptr;
			return strutc_2;
		}

		uint32_t DumpFile_V2(uint32_t* pStaticList, uint32_t* pStruct, const char* cpFolder, const char* cpResName)
		{
			uint32_t read_ret = sg_fnLoadFile_V2(pStaticList, pStruct, cpFolder, cpResName);
			
			uint32_t* struct_x = GetStruct(pStruct);
			if (struct_x)
			{
				uint32_t size = struct_x[4];
				uint32_t data = struct_x[6];
				if (size && data) { SaveFileViaPath(BackSlash(MakePath(sg_cpDumpFolder, cpFolder, cpResName)), (void*)data, size); }
			}

			return read_ret;
		}


		// EXPORT FUNCTION //

		void SetHookFolder(const char* cpFolder)
		{
			sg_cpHookFolder = cpFolder;
		}

		void SetReplaceFolder(const char* cpFolder)
		{
			sg_cpReplaceFolder = cpFolder;
		}

		void SetDumpFolder(const char* cpFolder)
		{
			sg_cpDumpFolder = cpFolder;
		}

		bool SetFileHook_V1(uint32_t fnFindResEntry_V1)
		{
			sg_fnFindResEntry_V1 = (pFindResEntry)fnFindResEntry_V1;
			return DetourAttachFunc(&sg_fnFindResEntry_V1, HookFile_V1);
		}

		bool SetFileHook_V2(uint32_t fnLoadFile_V2)
		{
			sg_fnLoadFile_V2 = (pLoadFile_V2)fnLoadFile_V2;
			return DetourAttachFunc(&sg_fnLoadFile_V2, HookFile_V2);
		}

		bool SetFileDump_V2(uint32_t fnLoadFile_V2)
		{
			sg_fnLoadFile_V2 = (pLoadFile_V2)fnLoadFile_V2;
			return DetourAttachFunc(&sg_fnLoadFile_V2, DumpFile_V2);
		}
	}
}