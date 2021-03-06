/***************************************

    Number String Manager in hexadecimal

    Copyright (c) 1995-2017 by Rebecca Ann Heineman <becky@burgerbecky.com>

    It is released under an MIT Open Source license. Please see LICENSE for
    license details. Yes, you can use it in a commercial title without paying
    anything, just give me a credit.

    Please? It's not like I'm asking you for money!

***************************************/

#ifndef __BRNUMBERSTRINGHEX_H__
#define __BRNUMBERSTRINGHEX_H__

#ifndef __BRTYPES_H__
#include "brtypes.h"
#endif

#ifndef __BRSTRINGFUNCTIONS_H__
#include "brstringfunctions.h"
#endif

/* BEGIN */
namespace Burger {
class NumberStringHex {
private:
	char m_Data[32];		///< String buffer
public:
	NumberStringHex() BURGER_NOEXCEPT { m_Data[0] = 0; }
	NumberStringHex(uint32_t uInput);
	NumberStringHex(uint32_t uInput,uint_t uFormat);
	NumberStringHex(uint64_t uInput);
	NumberStringHex(uint64_t uInput, uint_t uFormat);
	NumberStringHex(float fInput);
	NumberStringHex(double dInput);
	NumberStringHex & operator = (uint32_t uInput);
	NumberStringHex & operator = (uint64_t uInput);
	NumberStringHex & operator = (float fInput);
	NumberStringHex & operator = (double dInput);
	BURGER_INLINE operator const char *() const BURGER_NOEXCEPT { return m_Data; }
	BURGER_INLINE const char *GetPtr(void) const BURGER_NOEXCEPT { return m_Data; }
    BURGER_INLINE const char* c_str(void) const BURGER_NOEXCEPT { return m_Data; }
	BURGER_INLINE char operator[](uintptr_t uIndex) const BURGER_NOEXCEPT { return m_Data[uIndex]; }
	BURGER_INLINE const char &operator[](uintptr_t uIndex) BURGER_NOEXCEPT { return m_Data[uIndex]; }
	BURGER_INLINE void Clear(void) BURGER_NOEXCEPT { m_Data[0] = 0; }
	BURGER_INLINE uint_t IsEmpty(void) const BURGER_NOEXCEPT { return !m_Data[0]; }
	BURGER_INLINE uintptr_t GetLength(void) const BURGER_NOEXCEPT { return Burger::StringLength(m_Data); }
};
}
/* END */

#endif

