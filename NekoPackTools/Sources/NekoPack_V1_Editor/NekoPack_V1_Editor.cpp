#include <iostream>

#include "../../Modules/NekoPackTools/NEKOPACK_V1.h"


int main()
{
	NekoPackTools::NEKOPACK_V1::NEKOPACK_Editor editor("script.dat");
	editor.Init();
	editor.Extract();
}