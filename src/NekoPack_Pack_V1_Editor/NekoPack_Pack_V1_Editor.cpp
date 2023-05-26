#include <iostream>

#include "../../lib/Rxx/File.h"
#include "../../lib/NekoPackTools/NEKOPACK_V1.h"


int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		std::cout << "Put the program in the same directory as pack file and Dragging file to the program icon\n";
		std::cout << "Or Use Cmd : NekoPack_Pack_V1_Editor.exe system.dat\n\n";
	}

	if (argc == 2)
	{
		std::string pack_name = Rut::FileX::PathGetFileName(argv[1]);
		NekoPackTools::Pack::NEKOPACK_V1 editor(pack_name);
		editor.Extract();
	}

	system("pause");
}