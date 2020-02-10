/***************************************

    Shims for hid.dll

    Copyright (c) 1995-2017 by Rebecca Ann Heineman <becky@burgerbecky.com>

    It is released under an MIT Open Source license. Please see LICENSE for
    license details. Yes, you can use it in a commercial title without paying
    anything, just give me a credit.

    Please? It's not like I'm asking you for money!

***************************************/

#include "brwindowstypes.h"
#if defined(BURGER_WINDOWS) || defined(DOXYGEN)

#if !defined(DOXYGEN)
#include "brmemoryfunctions.h"

//
// Handle some annoying defines that some windows SDKs may or may not have
//

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#if !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0501 // Windows XP
#endif

#include <Windows.h>

// #include <ddk\hidsdi.h>

typedef void(APIENTRY* HidD_GetHidGuidPtr)(LPGUID);

// Unit tests for pointers

// HidD_GetHidGuidPtr gHidD_GetHidGuid = ::HidD_GetHidGuid;

#endif

//
// hid.dll
//

/*! ************************************

	\brief Load in hid.dll and call HidD_GetHidGuid

	Manually load hid.dll if needed and call the Windows function
	HidD_GetHidGuid()

	http://msdn.microsoft.com/en-us/library/windows/hardware/ff538924(v=vs.85).aspx

	\windowsonly

	\param pHidGuid Pointer to a caller-allocated GUID buffer that the routine
		uses to return the device interface GUID for HIDClass devices.

***************************************/

void BURGER_API Burger::Windows::HidD_GetHidGuid(GUID* pHidGuid)
{
	void* pHidD_GetHidGuid = LoadFunctionIndex(CALL_HidD_GetHidGuid);
	if (!pHidD_GetHidGuid) {
		MemoryClear(pHidGuid, sizeof(GUID));
	} else {
		static_cast<HidD_GetHidGuidPtr>(pHidD_GetHidGuid)(pHidGuid);
	}
}

#endif
