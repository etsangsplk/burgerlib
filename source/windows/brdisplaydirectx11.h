/***************************************

    DirectX 11 manager class

    Copyright (c) 1995-2017 by Rebecca Ann Heineman <becky@burgerbecky.com>

    It is released under an MIT Open Source license. Please see LICENSE for
    license details. Yes, you can use it in a commercial title without paying
    anything, just give me a credit.

    Please? It's not like I'm asking you for money!

***************************************/

#ifndef __BRDISPLAYDIRECTX11_H__
#define __BRDISPLAYDIRECTX11_H__

#ifndef __BRDISPLAY_H__
#include "brdisplay.h"
#endif

#if defined(BURGER_WINDOWS) && !defined(__BRWINDOWSTYPES_H__)
#include "brwindowstypes.h"
#endif

/* BEGIN */
namespace Burger {
#if defined(BURGER_WINDOWS) || defined(DOXYGEN)
class DisplayDirectX11 : public Display {
    BURGER_DISABLE_COPY(DisplayDirectX11);
	BURGER_RTTI_IN_CLASS();

public:
	struct DXGISurfaceDesc_t {
		Word m_uWidth;		   ///< Width of the surface in pixels
		Word m_uHeight;		   ///< Height of the surface in pixels
		Word m_uFormat;		   ///< DXGI_FORMAT enumeration of the pixel format
		Word m_uSampleCount;   ///< DXGI_SAMPLE_DESC sample count
		Word m_uSampleQuality; ///< DXGI_SAMPLE_DESC sample quality
	};

protected:
	/** TRUE if the cursor is visible in full screen mode */
	Word m_bShowCursorWhenFullScreen;
	/** TRUE if the cursor is clipped in full screen mode */
	Word m_bClipCursorWhenFullScreen;

	void BURGER_API SetupCursor(void) const;

public:
	static void BURGER_API ClearContext(ID3D11DeviceContext* pDX11Context);
};

extern Word BURGER_API GetDXGI_FORMATColorChannelBits(Word uDXGI_FORMAT);

#endif
}
/* END */

#endif
