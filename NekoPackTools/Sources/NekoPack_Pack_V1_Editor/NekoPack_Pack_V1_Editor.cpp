#include <iostream>

#include "../../Modules/NekoPackTools/NEKOPACK_V1.h"


int main()
{
	NekoPackTools::Pack::NEKOPACK_V1 editor("system.dat");
	editor.Extract();
}