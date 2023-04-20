#include <iostream>

#include "../../Modules/NekoPackTools/Pack_V1.h"


int main()
{
	NekoPackTools::Pack_V1::Pack_Editor editor("script.dat");
	editor.Init();
	editor.Extract();
}