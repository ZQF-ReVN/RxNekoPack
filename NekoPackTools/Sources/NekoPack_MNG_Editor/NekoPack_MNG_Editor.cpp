#include <iostream>

#include "../../Modules/NekoPackTools/MNG.h"
#include "../../Modules/NekoPackTools/MNG_Struct.h"
#include "../../ThirdParty/Rut/FileX.h"

using namespace NekoPackTools::Picture;
using namespace Rut::FileX;


int wmain(int argc, wchar_t* argv[])
{
	try
	{
		auto extract = [](const std::wstring& wsFile)
		{
			NekoPackTools::Picture::MNG_Editor mng(wsFile);
			mng.LoadMNG(); mng.Extract();
			std::cout << "Extract Completed!" << std::endl;
		};

		auto create = [](const std::wstring& wsFile)
		{
			NekoPackTools::Picture::MNG_Editor mng(wsFile);
			mng.LoadMNG(); mng.Create(); mng.SaveMNG();
			std::cout << "Repack Completed!" << std::endl;
		};

		auto parser = [&argv, &extract, &create](const std::wstring& wsFile) -> bool
		{
			if (*argv[2] == L'E') { extract(wsFile); return true; }
			if (*argv[2] == L'R') { create(wsFile); return true; }
			return false;
		};

		auto info = []()
		{
			std::cout
				<< "Command:\n"
				<< "        NekoPack_MNG_Editor.exe C E title.mng\n"
				<< "        NekoPack_MNG_Editor.exe C R title.mng\n"
				<< "Drag   :\n"
				<< "        drag MNG to exe to extract\n"
				<< "        drag MNG and extract folder to exe to repack\n\n"
				<< "Please run in CMD! Not Double Click!\n"
				<< std::endl;
		};


		std::wstring path; std::wstring file;

		if (argc == 2 || argc == 3) { path = argv[1]; file = PathGetFileName_RET(path); }
		else if (argc == 4) { file = argv[3]; }

		switch (argc)
		{
		case 1: { info(); } break;
		case 2: { extract(file); } break;
		case 3: { create(file); } break;
		case 4: { if (parser(file)) { break; } }
		default: { std::cerr << "parameters error!" << std::endl; }
		}
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << '\n';
	}

	system("pause");
	return 0;
}