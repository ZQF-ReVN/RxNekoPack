#include <iostream>

#include "../../Modules/NekoPackTools/Text.h"


int wmain(int argc, wchar_t* argv[])
{
	if (argc == 1)
	{
		std::cout
			<< "Command: E Extract , I Insert, [CP_Read, CP_Insert] The Script CodePage\n"
			<< "Example:\n"
			<< "NekoPackTextEditor.exe E CS01_A_01.txt CP_Read 932\n"
			<< "NekoPackTextEditor.exe I CS01_A_01.txt CP_Read 932 CP_Insert 936\n\n"
			<< "Please run in CMD! Not Double Click!\n" << std::endl;

		system("pause");
		return 0;
	}

	switch (*argv[1])
	{
	case L'E':
	{
		if (argc < 5) { std::cout << "Command Error!"; }

		NekoPackTools::Text::Editor editor;
		editor.SetCodePage(std::stoi(argv[4]), 932);
		editor.Extract(argv[2]);
	}
	break;

	case L'I':
	{
		if (argc < 7) { std::cout << "Command Error!"; }

		NekoPackTools::Text::Editor editor;
		editor.SetCodePage(std::stoi(argv[4]), std::stoi(argv[6]));
		editor.Insert(argv[2]);
	}
	break;
	}
}