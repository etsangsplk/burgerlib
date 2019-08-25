/***************************************

	Intrinsics and subroutines exclusive to the Microsoft Visual Studio compilers

	Copyright (c) 1995-2019 by Rebecca Ann Heineman <becky@burgerbecky.com>

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!

***************************************/

#ifndef __BRVISUALSTUDIO_H__
#define __BRVISUALSTUDIO_H__

#ifndef __BRTYPES_H__
#include "brtypes.h"
#endif

//
// Note: _MSC_VER values for determining specific version.
//
// _MSC_VER == 1920 (Visual Studio 2019)
// _MSC_VER == 1910 (Visual Studio 2017)
// _MSC_VER == 1900 (Visual Studio 2015)
// _MSC_VER == 1800 (Visual Studio 2013)
// _MSC_VER == 1700 (Visual Studio 2012)
// _MSC_VER == 1600 (Visual Studio 2010)
// _MSC_VER == 1500 (Visual Studio 2008)
// _MSC_VER == 1400 (Visual Studio 2005)
// _MSC_VER == 1310 (Visual Studio 2003)
// _MSC_VER == 1300 (Visual Studio 2002)
// _MSC_VER == 1200 (Visual Studio 6)
// _MSC_VER == 1100 (Visual studio 5)
//

/* BEGIN */
#if defined(BURGER_MSVC) && !defined(DOXYGEN)

extern "C" {

extern double __cdecl fabs(double);
extern double __cdecl sqrt(double);
#pragma intrinsic(fabs,sqrt)

#if defined(BURGER_AMD64) || defined(BURGER_ARM)
extern float __cdecl sqrtf(float);
#pragma intrinsic(sqrtf)
#endif

#if defined(BURGER_ARM)
extern float __cdecl fabsf(float);
#pragma intrinsic(fabsf)
#endif

extern unsigned int __cdecl _rotl(unsigned int,int);
extern Word64 __cdecl _rotl64(Word64,int);
extern unsigned int __cdecl _rotr(unsigned int,int);
extern Word64 __cdecl _rotr64(Word64,int);
#pragma intrinsic(_rotl,_rotl64,_rotr,_rotr64)

#if defined(BURGER_INTEL)

// Visual Studio 2005 or higher
#if _MSC_VER>=1400
extern void __cpuid(int [4],int);
#pragma intrinsic(__cpuid)
#else
BURGER_INLINE void __cpuid(int a[4],int b) {
BURGER_ASM {
	mov	esi,a		// Get the pointer to the destination buffer
	mov eax,b		// Command byte
	cpuid			// Invoke CPUID
	mov [esi],eax	// Store the result in the same order as Visual C
	mov [esi+4],ebx
	mov [esi+8],ecx
	mov [esi+12],edx
}
}

BURGER_INLINE void __cpuidex(int a[4],int b,int c) {
BURGER_ASM {
	mov	esi,a		// Get the pointer to the destination buffer
	mov eax,b		// Command byte
	mov ecx,c		// Get the sub command
	cpuid			// Invoke CPUID
	mov [esi],eax	// Store the result in the same order as Visual C
	mov [esi+4],ebx
	mov [esi+8],ecx
	mov [esi+12],edx
}
}
#endif

// Visual studio 2010 or higher

#if _MSC_VER>=1600
extern void __cpuidex(int [4],int,int);
#pragma intrinsic(__cpuidex)
#endif

#endif

Word8 _BitScanForward(unsigned long *Index,unsigned long Mask);
Word8 _BitScanReverse(unsigned long *Index,unsigned long Mask);
#if !defined(BURGER_XBOX360)
#pragma intrinsic(_BitScanForward,_BitScanReverse)
#endif

#if defined(BURGER_X86) && (_MSC_VER>=1700)  // Visual Studio before 2012 doesn't __cdecl these
long __cdecl _InterlockedExchange(long volatile*,long);
long __cdecl _InterlockedExchangeAdd(long volatile*,long);
long __cdecl _InterlockedCompareExchange(long volatile*,long,long);
#else
long _InterlockedExchange(long volatile*,long);
long _InterlockedExchangeAdd(long volatile*,long);
long _InterlockedCompareExchange(long volatile*,long,long);
#endif

long __cdecl _InterlockedIncrement(long volatile*);
long __cdecl _InterlockedDecrement(long volatile*);
#pragma intrinsic(_InterlockedExchange,_InterlockedIncrement,_InterlockedDecrement,_InterlockedExchangeAdd,_InterlockedCompareExchange)

#if defined(BURGER_64BITCPU)
__int64 _InterlockedExchange64(__int64 volatile*,__int64);
__int64 _InterlockedIncrement64(__int64 volatile*);
__int64 _InterlockedDecrement64(__int64 volatile*);
__int64 _InterlockedExchangeAdd64(__int64 volatile*,__int64);
__int64 _InterlockedCompareExchange64(__int64 volatile*,__int64,__int64);
#pragma intrinsic(_InterlockedExchange64,_InterlockedIncrement64,_InterlockedDecrement64,_InterlockedExchangeAdd64,_InterlockedCompareExchange64)

#if defined(BURGER_INTEL)
Word8 _BitScanForward64(unsigned long *Index,Word64 Mask);
Word8 _BitScanReverse64(unsigned long *Index,Word64 Mask);
#pragma intrinsic(_BitScanForward64,_BitScanReverse64)
#endif

// Visual studio 2005 and earlier don't have these SSE type conversions

#if defined(BURGER_AMD64) && (_MSC_VER<1500)
BURGER_INLINE float _mm_cvtss_f32(__m128 vInput) { return vInput.m128_f32[0]; }
BURGER_INLINE double _mm_cvtsd_f64(__m128d vInput) { return vInput.m128d_f64[0]; }
BURGER_INLINE __m128 _mm_castpd_ps(__m128d vInput) { union { __m128d a; __m128 b; } x; x.a = vInput; return x.b; }
BURGER_INLINE __m128i _mm_castpd_si128(__m128d vInput) { union { __m128d a; __m128i b; } x; x.a = vInput; return x.b; }
BURGER_INLINE __m128d _mm_castps_pd(__m128 vInput) { union { __m128 a; __m128d b; } x; x.a = vInput; return x.b; }
BURGER_INLINE __m128i _mm_castps_si128(__m128 vInput) { union { __m128 a; __m128i b; } x; x.a = vInput; return x.b; }
BURGER_INLINE __m128 _mm_castsi128_ps(__m128i vInput) { union { __m128i a; __m128 b; } x; x.a = vInput; return x.b; }
BURGER_INLINE __m128d _mm_castsi128_pd(__m128i vInput) { union { __m128i a; __m128d b; } x; x.a = vInput; return x.b; }
#endif

#endif
}
#endif

// __cpuid() and __cpuidex() intrinsics for other compilers

#if defined(BURGER_INTEL) && (defined(BURGER_CLANG) || defined(BURGER_GNUC))

#if defined(BURGER_X86) && defined(__PIC__)

// If PIC is enabled, EBX / RBX is the pointer to the local data
// so it must be preserved

BURGER_INLINE void __cpuid(int a[4],int b) {
	__asm__ __volatile__(
		"pushl	%%ebx\n" \
		"cpuid\n" \
		"movl	%%ebx,%1\n" \
		"popl	%%ebx"
		: "=a" ((a)[0]), "=r" ((a)[1]), "=c" ((a)[2]), "=d" ((a)[3])
		: "0" (b));
}

BURGER_INLINE void __cpuidex(int a[4],int b,int c) {
	__asm__ __volatile__(
		"pushl	%%ebx\n" \
		"cpuid\n" \
		"movl	%%ebx,%1\n" \
		"popl	%%ebx"
		: "=a" ((a)[0]), "=r" ((a)[1]), "=c" ((a)[2]), "=d" ((a)[3])
		: "0" (b),"2" (c));
}

#elif defined(BURGER_AMD64) && defined(__PIC__)

BURGER_INLINE void __cpuid(int a[4],int b) {
	__asm__ __volatile__(
		"pushq	%%rbx\n" \
		"cpuid\n" \
		"movl	%%ebx,%1\n" \
		"popq	%%rbx"
		: "=a" ((a)[0]), "=r" ((a)[1]), "=c" ((a)[2]), "=d" ((a)[3]) 
		: "0" (b));
}

BURGER_INLINE void __cpuidex(int a[4],int b,int c) {
	__asm__ __volatile__(
		"pushq	%%rbx\n" \
		"cpuid\n" \
		"movl	%%ebx,%1\n" \
		"popq	%%rbx"
		: "=a" ((a)[0]), "=r" ((a)[1]), "=c" ((a)[2]), "=d" ((a)[3])
		: "0" (b),"2" (c));
}

#else

BURGER_INLINE void __cpuid(int a[4],int b) {
	__asm__ __volatile__(
		"cpuid"
		: "=a" ((a)[0]), "=b" ((a)[1]), "=c" ((a)[2]), "=d" ((a)[3])
		: "0" (b));
}

BURGER_INLINE void __cpuidex(int a[4],int b,int c) {
	__asm__ __volatile__(
		"cpuid"
		: "=a" ((a)[0]), "=b" ((a)[1]), "=c" ((a)[2]), "=d" ((a)[3])
		: "0" (b), "2" (c));
}

#endif

BURGER_INLINE Word32 _BitScanForward(unsigned long *Index,unsigned long Mask)
{
	Word8 bZero;
	__asm__("bsf %2, %0 \n \
			setne %1" : "=r" (*Index), "=q" (bZero) : "mr" (Mask));
	return bZero;
}

BURGER_INLINE Word32 _BitScanReverse(unsigned long *Index,unsigned long Mask)
{
	Word8 bZero;
	__asm__("bsr %2, %0 \n \
			setne %1" : "=r" (*Index), "=q" (bZero) : "mr" (Mask));
	return bZero;
}

#elif defined(BURGER_PPC) && defined(BURGER_MSVC)

#define __cntlzw(x) _CountLeadingZeros(x)
#define __cntlzd(x) _CountLeadingZeros64(x)

#elif defined(BURGER_X86) && defined(BURGER_METROWERKS)

BURGER_INLINE void __cpuid(int a[4],int b) {
BURGER_ASM {
	mov	esi,a		// Get the pointer to the destination buffer
	mov eax,b		// Command byte
	cpuid			// Invoke CPUID
	mov [esi],eax	// Store the result in the same order as Visual C
	mov [esi+4],ebx
	mov [esi+8],ecx
	mov [esi+12],edx
}
}

BURGER_INLINE void __cpuidex(int a[4],int b,int c) {
BURGER_ASM {
	mov	esi,a		// Get the pointer to the destination buffer
	mov eax,b		// Command byte
	mov ecx,c		// Get the sub command
	cpuid			// Invoke CPUID
	mov [esi],eax	// Store the result in the same order as Visual C
	mov [esi+4],ebx
	mov [esi+8],ecx
	mov [esi+12],edx
}
}

BURGER_INLINE Word32 _BitScanForward(register unsigned long *Index,register unsigned long Mask)
{
BURGER_ASM {
	mov	eax,Mask
	mov ebx,Index
	bsf	eax,eax
	mov	dword ptr [ebx],eax
	setne al
}
}

BURGER_INLINE Word32 _BitScanReverse(register unsigned long *Index,register unsigned long Mask)
{
BURGER_ASM {
	mov	eax,Mask
	mov ebx,Index
	bsr	eax,eax
	mov	dword ptr [ebx],eax
	setne al
}
}

#elif defined(BURGER_X86) && defined(BURGER_WATCOM)

extern void __cpuid(int a[4],int b);

#pragma aux __cpuid = \
	"cpuid" \
	"mov [esi+0],eax" \
	"mov [esi+4],ebx" \
	"mov [esi+8],ecx" \
	"mov [esi+12],edx" \
	parm [esi] [eax] modify [ebx ecx edx];

extern void __cpuidex(int a[4],int b,int c);

#pragma aux __cpuidex = \
	"cpuid" \
	"mov [esi+0],eax" \
	"mov [esi+4],ebx" \
	"mov [esi+8],ecx" \
	"mov [esi+12],edx" \
	parm [esi] [eax] [ecx] modify [ebx ecx edx];

extern Word32 _BitScanForward(unsigned long *Index,unsigned long Mask);

#pragma aux _BitScanForward = \
	"bsf eax,eax" \
	"mov dword ptr [edx],eax" \
	"setne al" \
	parm [eax] [ecx] value [eax] modify exact [eax];

extern Word32 _BitScanReverse(unsigned long *Index,unsigned long Mask);

#pragma aux _BitScanReverse = \
	"bsr eax,eax" \
	"mov dword ptr [edx],eax" \
	"setne al" \
	parm [eax] [ecx] value [eax] modify exact [eax];

#endif

/* END */

#endif
