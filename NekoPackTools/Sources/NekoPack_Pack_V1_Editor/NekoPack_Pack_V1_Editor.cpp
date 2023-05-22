#include <iostream>

#include "../../Modules/NekoPackTools/include/NEKOPACK_V1.h"


int main()
{
	NekoPackTools::Pack::NEKOPACK_V1 editor("system.dat");
	editor.Extract();
}