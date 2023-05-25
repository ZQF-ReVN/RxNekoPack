#include <iostream>

#include "../../Modules/NekoPackTools/include/NEKOPACK_V1.h"
#include "../../Modules/NekoPackTools/include/NEKOPACK_V1_5.h"


int main(int argc, char* argv[])
{
	NekoPackTools::Pack::NEKOPACK_V1 editor(argv[1]);
	editor.Extract();
	//NekoPackTools::Pack::NEKOPACK_V1_5 pack(L"system.dat");
	//pack.Extract();
}