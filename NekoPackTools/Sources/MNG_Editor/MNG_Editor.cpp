#include "../../Modules/NekoPackTools/MNG.h"

int wmain(int argc, wchar_t* argv[])
{
	if (argc < 1) return 0;

	switch (*argv[1])
	{
	case L'E': 
	{
		NekoPackTools::MNG::MNG_Editor mng(argv[2]);
		mng.LoadMNG();
		mng.ExtractMultiplePNG();
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