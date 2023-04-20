#include <iostream>

#include "../../Modules/NekoPackTools/MNG.h"


int wmain(int argc, wchar_t* argv[])
{
	if (argc == 1)
	{
		std::cout
			<< "Command: E Extract , R Repack\n"
			<< "Example:\n"
			<< "MNG_Editor.exe E title.mng\n"
			<< "MNG_Editor.exe R title.mng\n\n"
			<< "Please run in CMD! Not Double Click!\n" << std::endl;

		system("pause");
		return 0;
	}

	if (argc > 3)
	{
		std::cout << "Too many parameters" << std::endl;
		return 0;
	}

	switch (*argv[1])
	{
	case L'E': 
	{
		NekoPackTools::MNG::MNG_Editor mng(argv[2]);
		mng.LoadMNG();
		mng.ExtractMultipleFrame();
	}
	break;

	case L'R':
	{
		NekoPackTools::MNG::MNG_Editor mng(argv[2]);
		mng.BuildMNG();
		mng.SaveMNG();
	}
	break;

	}

}