#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	int res = RUN_ALL_TESTS();

	return res;
}

