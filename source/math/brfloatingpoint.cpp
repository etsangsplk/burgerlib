/***************************************

    Floating point math functions

    Copyright (c) 1995-2017 by Rebecca Ann Heineman <becky@burgerbecky.com>

    It is released under an MIT Open Source license. Please see LICENSE for
    license details. Yes, you can use it in a commercial title without paying
    anything, just give me a credit.

    Please? It's not like I'm asking you for money!

***************************************/

#include "brfloatingpoint.h"
#include "brendian.h"
#include "brfixedpoint.h"
#include <math.h>

//
// Note: The Xbox 360 PPC compiler has a bug. It's not acknowledging
// that fcmpu modifies the condition code register, so the inline assembler
// reorder optimizer will move the mfcr instruction BEFORE the fcmpu
// instruction, causing this function to fail.
//
// The workaround is to turn off the feature for this function
//

#if defined(BURGER_XBOX360)
#pragma optimize("z", off)
#endif

//
// Constant used by Intel assembly for the 8087 specific code
//

#if defined(BURGER_X86)
#if defined(BURGER_MSVC)
#pragma warning(disable : 4725) // Disable Intel instruction not accurate on
                                // some CPUs (FPREM1)
#endif

struct Word80ToLongDouble {
    union {
        uint8_t w[10]; ///< Value in binary
        long double l; ///< Value as a 80 bit float
    };
    BURGER_INLINE operator long double() const
    {
        return l;
    }
};

// Pi * 2 in 80 bit format
extern const Burger::Float80Bit Pi280bit = {
    0x35, 0xc2, 0x68, 0x21, 0xa2, 0xda, 0x0f, 0xc9, 0x00, 0x40};
#endif

#if defined(BURGER_X86) || defined(DOXYGEN)

const uint16_t Burger::g_X86RoundDownFlag =
    0x0F7F; // 8087 control word to round down with 64 bit precision
const Burger::Word32ToFloat Burger::g_X86OneAndNegOne[2] = {
    0x3F000000U, 0xBF000000U};

#endif

/*! ************************************

    \typedef Burger::Float80Bit

    This is a type to abstract the 80-bit floating point data type found in the
    x8087 and 680x0 math co-processors. Currently, this type is hardly used
    anymore with the exception of the audio frame rate used in AIFF files.

    Burgerlib only has functions to read and write this format into double and
    float.

    \note Use of this data type for runtime math is discouraged and only used
        for x86 code and AIFF audio file support.

***************************************/

/*! ************************************

    \struct Burger::Word32ToFloat
    \brief Class to create float values with integer constants

    Some "C" compilers are not 100% accurate in creating floating point
    constants. To get around this limitation, once the binary representation of
    a floating point number is determined, it will be used to assign the value
    so the binary representation does not change from compiler to compiler.

    \code
    // Binary representation for Single Epsilon
    Burger::Word32ToFloat Epsilon = {0x34000000U};

    void code(void)
    {
        // Fetch as a float
        float fEpsilon = Epsilon;

        // Set a new floating point value
        Epsilon = 1.5f;

        // Get the binary value as a 32 bit integer
        uint32_t uEpsilon = Epsilon.GetWord();

        // Set the binary value as a 32 bit integer
        Epsilon.SetWord(0x34000000U);
    }
    \endcode

    \sa Word64ToDouble

***************************************/

/*! ************************************

    \fn Burger::Word32ToFloat::operator float() const
    \brief Accessor to load the floating point value

    \sa Word32ToFloat::operator = (float)

***************************************/

/*! ************************************

    \fn Burger::Word32ToFloat::operator = (float fInput)
    \brief Update with a new floating point value

    \param fInput 32 bit floating point number to store in the union
    \sa Word32ToFloat::operator float() const

***************************************/

/*! ************************************

    \fn Burger::Word32ToFloat::GetWord(void) const
    \brief Accessor to load the 32 bit value in binary

    \return Floating point value as a binary representation in an integer
    \sa SetWord(uint32_t)

***************************************/

/*! ************************************

    \fn Burger::Word32ToFloat::SetWord(uint32_t uInput)
    \brief Set the floating point value with a binary representation in an
        integer

    \param uInput 32 bit integer to store in the union
    \sa GetWord(void) const

***************************************/

/*! ************************************

    \struct Burger::Word64ToDouble
    \brief Class to create double values with integer constants

    Some "C" compilers are not 100% accurate in creating floating point
    constants. To get around this limitation, once the binary representation of
    a floating point number is determined, it will be used to assign the value
    so the binary representation does not change from compiler to compiler.

    \code
    // Binary representation for Double Epsilon
    Burger::Word64ToDouble Epsilon = {0x3CB0000000000000ULL};

    void code(void)
    {
        // Fetch as a float
        double dEpsilon = Epsilon;

        // Set a new floating point value
        Epsilon = 1.5;

        // Get the binary value as a 64 bit integer
        uint64_t uEpsilon = Epsilon.GetWord();

        // Set the binary value as a 64 bit integer
        Epsilon.SetWord(0x3CB0000000000000ULL);

        // Get the binary value as 32 bit integers
        uint32_t uLowEpsilon = Epsilon.GetLowWord();
        uint32_t uHiEpsilon = Epsilon.GetHiWord();

        // Set the binary value as 32 bit integers
        Epsilon.SetLowWord(0x00000000U);
        Epsilon.SetHiWord(0x3CB00000U);
    }
    \endcode

    \sa Word32ToFloat

***************************************/

/*! ************************************

    \fn Burger::Word64ToDouble::operator double() const
    \brief Accessor to load the double precision floating point value

    \sa Word64ToDouble::operator = (double)

***************************************/

/*! ************************************

    \fn Burger::Word64ToDouble::operator = (double dInput)
    \brief Update with a new floating point value

    \param dInput 64 bit floating point number to store in the union
    \sa Word64ToDouble::operator double() const

***************************************/

/*! ************************************

    \fn Burger::Word64ToDouble::GetWord(void) const
    \brief Accessor to load the 64 bit value in binary

    \return Floating point value as a binary representation in an integer
    \sa SetWord(uint64_t), SetLowWord(uint32_t) or SetHiWord(uint32_t)

***************************************/

/*! ************************************

    \fn Burger::Word64ToDouble::SetWord(uint64_t uInput)
    \brief Set the floating point value with a binary representation in an
        integer

    \param uInput 64 bit integer to store in the union
    \sa GetWord(void) const, GetLowWord(void) const or GetHiWord(void) const

***************************************/

/*! ************************************

    \fn Burger::Word64ToDouble::GetLowWord(void) const
    \brief Accessor to load the low 32 bits of the 64 bit word

    \return Lower 32 bits of the 64 bit word
    \sa GetHiWord(void) const or SetLowWord(uint32_t)

***************************************/

/*! ************************************

    \fn Burger::Word64ToDouble::GetHiWord(void) const
    \brief Accessor to load the upper 32 bits of the 64 bit word

    \return Upper 32 bits of the 64 bit word
    \sa GetLowWord(void) const or SetHiWord(uint32_t)

***************************************/

/*! ************************************

    \fn Burger::Word64ToDouble::SetLowWord(uint32_t uInput)
    \brief Accessor to load the low 32 bits of the 64 bit word

    \param uInput 32 bit integer to store in the low 32 bits of the double
    \sa SetHiWord(uint32_t) or GetLowWord(void) const

***************************************/

/*! ************************************

    \fn Burger::Word64ToDouble::SetHiWord(uint32_t uInput)
    \brief Accessor to load the low 32 bits of the 64 bit word

    \param uInput 32 bit integer to store in the upper 32 bits of the double
    \sa SetLowWord(uint32_t) or GetHiWord(void) const

***************************************/

/*! ************************************

    \struct Burger::Vector_128Float64
    \brief Structure to create a 128 vector with 64 bit floating point constants

    There is no standard for initializing 128 bit vectors at compile time. This
    structure fixes the problem by creating a union between the 128 bit value
    and an array of data that maps directly over it.

    \code
    // Initialize with 2 64 bit floats
    const Burger::Vector_128Float64 One_Vector = {{1.0,-55.0}};

    \endcode

    \sa Vector_128Word64, Vector_128Float32, Vector_128Word32, Vector_128Word16,
        Vector_128Word8, Vector_128Int32

***************************************/

/*! ************************************

    \fn Burger::Vector_128Float64::operator Vector_128() const
    \brief Accessor to load the 128 bit vector value

    \sa Vector_128Float64 or Vector_128

***************************************/

/*! ************************************

    \struct Burger::Vector_128Float32
    \brief Structure to create a 128 vector with 32 bit floating point constants

    There is no standard for initializing 128 bit vectors at compile time. This
    structure fixes the problem by creating a union between the 128 bit value
    and an array of data that maps directly over it.

    \code
    // Initialize with 4 32 bit floats
    const Burger::Vector_128Float32 One_Vector = {{1.0f,1.0f,1.0f,1.0f}};

    \endcode

    \sa Vector_128Word32, Vector_128Word16, Vector_128Word8, Vector_128Int32

***************************************/

/*! ************************************

    \fn Burger::Vector_128Float32::operator Vector_128() const
    \brief Accessor to load the 128 bit vector value

    \sa Vector_128Float32 or Vector_128

***************************************/

/*! ************************************

    \struct Burger::Vector_128Word64
    \brief Structure to create a 128 vector with 64 bit unsigned integer
        constants

    There is no standard for initializing 128 bit vectors at compile time. This
    structure fixes the problem by creating a union between the 128 bit value
    and an array of data that maps directly over it.

    \code
    // Initialize with 2 64 bit unsigned integers
    const Burger::Vector_128Word64 One_Vector = {{1,0x123456789ABCDEFULL}};

    \endcode

    \sa Vector_128Float64, Vector_128Word32, Vector_128Word16, Vector_128Word8,
        Vector_128Int32

***************************************/

/*! ************************************

    \fn Burger::Vector_128Word64::operator Vector_128() const
    \brief Accessor to load the 128 bit vector value

    \sa Vector_128Word64 or Vector_128

***************************************/

/*! ************************************

    \struct Burger::Vector_128Word32
    \brief Structure to create a 128 vector with 32 bit unsigned integer
        constants

    There is no standard for initializing 128 bit vectors at compile time. This
    structure fixes the problem by creating a union between the 128 bit value
    and an array of data that maps directly over it.

    \code
    // Initialize with 4 32 bit unsigned integers
    const Burger::Vector_128Word32 One_Vector = {{1,1,1,0x12345678}};

    \endcode

    \sa Vector_128Float32, Vector_128Word16, Vector_128Word8, Vector_128Int32

***************************************/

/*! ************************************

    \fn Burger::Vector_128Word32::operator Vector_128() const
    \brief Accessor to load the 128 bit vector value

    \sa Vector_128Word32 or Vector_128

***************************************/

/*! ************************************

    \struct Burger::Vector_128Word16
    \brief Structure to create a 128 vector with 16 bit unsigned integer
        constants

    There is no standard for initializing 128 bit vectors at compile time. This
    structure fixes the problem by creating a union between the 128 bit value
    and an array of data that maps directly over it.

    \code
    // Initialize with 8 16 bit unsigned integers
    const Burger::Vector_128Word16 One_Vector = {{1,2,3,4,5,6,7,0x1234}};

    \endcode

    \sa Vector_128Float32, Vector_128Word32, Vector_128Word8, Vector_128Int32

***************************************/

/*! ************************************

    \fn Burger::Vector_128Word16::operator Vector_128() const
    \brief Accessor to load the 128 bit vector value

    \sa Vector_128Word16 or Vector_128

***************************************/

/*! ************************************

    \struct Burger::Vector_128Word8
    \brief Structure to create a 128 vector with 8 bit unsigned integer
        constants

    There is no standard for initializing 128 bit vectors at compile time. This
    structure fixes the problem by creating a union between the 128 bit value
    and an array of data that maps directly over it.

    \code
    // Initialize with 16 8 bit unsigned integers
    const Burger::Vector_128Word8 One_Vector =
        {{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}};

    \endcode

    \sa Vector_128Float32, Vector_128Word32, Vector_128Word16, Vector_128Int32

***************************************/

/*! ************************************

    \fn Burger::Vector_128Word8::operator Vector_128() const
    \brief Accessor to load the 128 bit vector value

    \sa Vector_128Word8 or Vector_128

***************************************/

/*! ************************************

    \struct Burger::Vector_128Int32
    \brief Structure to create a 128 vector with 32 bit signed integer constants

    There is no standard for initializing 128 bit vectors at compile time. This
    structure fixes the problem by creating a union between the 128 bit value
    and an array of data that maps directly over it.

    \code
    // Initialize with 4 32 bit signed integers
    const Burger::Vector_128Int32 One_Vector = {{-1,1,-1,0x12345678}};

    \endcode

    \sa Vector_128Float32, Vector_128Word32, Vector_128Word16, Vector_128Word8

***************************************/

/*! ************************************

    \fn Burger::Vector_128Int32::operator Vector_128() const
    \brief Accessor to load the 128 bit vector value

    \sa Vector_128Int32 or Vector_128

***************************************/

const Burger::Word64ToDouble Burger::g_dOne = {0x3FF0000000000000ULL};
const Burger::Word64ToDouble Burger::g_dNegOne = {0xBFF0000000000000ULL};
const Burger::Word64ToDouble Burger::g_dHalf = {0x3FE0000000000000ULL};
const Burger::Word64ToDouble Burger::g_dNegHalf = {0xBFE0000000000000ULL};
const Burger::Word64ToDouble Burger::g_dInf = {0x7FF0000000000000ULL};
const Burger::Word64ToDouble Burger::g_dNegInf = {0xFFF0000000000000ULL};
const Burger::Word64ToDouble Burger::g_dNan = {0x7FF7FFFFFFFFFFFFULL};
const Burger::Word64ToDouble Burger::g_dNegNan = {0xFFF7FFFFFFFFFFFFULL};
const Burger::Word64ToDouble Burger::g_dQNan = {0x7FFFFFFFFFFFFFFFULL};
const Burger::Word64ToDouble Burger::g_dNegQNan = {0xFFFFFFFFFFFFFFFFULL};
const Burger::Word64ToDouble Burger::g_dMin = {0x0010000000000000ULL};
const Burger::Word64ToDouble Burger::g_dNegMin = {0x8010000000000000ULL};
const Burger::Word64ToDouble Burger::g_dMax = {0x7FEFFFFFFFFFFFFFULL};
const Burger::Word64ToDouble Burger::g_dNegMax = {0xFFEFFFFFFFFFFFFFULL};
const Burger::Word64ToDouble Burger::g_dMinNoInteger = {0x4330000000000000ULL};
const Burger::Word64ToDouble Burger::g_dNegMinNoInteger = {
    0xC330000000000000ULL};
const Burger::Word64ToDouble Burger::g_dEpsilon = {0x3CB0000000000000ULL};
const Burger::Word64ToDouble Burger::g_dNegEpsilon = {0xBCB0000000000000ULL};
const Burger::Word64ToDouble Burger::g_dQuarterPi = {0x3FE921FB54442D18ULL};
const Burger::Word64ToDouble Burger::g_dNegQuarterPi = {0xBFE921FB54442D18ULL};
const Burger::Word64ToDouble Burger::g_dHalfPi = {0x3FF921FB54442D18ULL};
const Burger::Word64ToDouble Burger::g_dNegHalfPi = {0xBFF921FB54442D18ULL};
const Burger::Word64ToDouble Burger::g_dPi = {0x400921FB54442D18ULL};
const Burger::Word64ToDouble Burger::g_dNegPi = {0xC00921FB54442D18ULL};
const Burger::Word64ToDouble Burger::g_dPi2 = {0x401921FB54442D18ULL};
const Burger::Word64ToDouble Burger::g_dNegPi2 = {0xC01921FB54442D18ULL};
const Burger::Word64ToDouble Burger::g_dPi4 = {0x402921FB54442D18ULL};
const Burger::Word64ToDouble Burger::g_dNegPi4 = {0xC02921FB54442D18ULL};
const Burger::Word64ToDouble Burger::g_dCosPiDiv6 = {0x3FEBB67AE8584CABULL};
const Burger::Word64ToDouble Burger::g_dNegCosPiDiv6 = {0xBFEBB67AE8584CABULL};
const Burger::Word64ToDouble Burger::g_dReciprocalHalfPi = {
    0x3FE45F306DC9C883ULL};
const Burger::Word64ToDouble Burger::g_dNegReciprocalHalfPi = {
    0xBFE45F306DC9C883ULL};
const Burger::Word64ToDouble Burger::g_dReciprocalPi = {0x3FD45F306DC9C883ULL};
const Burger::Word64ToDouble Burger::g_dNegReciprocalPi = {
    0xBFD45F306DC9C883ULL};
const Burger::Word64ToDouble Burger::g_dReciprocalPi2 = {0x3FC45F306DC9C883ULL};
const Burger::Word64ToDouble Burger::g_dNegReciprocalPi2 = {
    0xBFC45F306DC9C883ULL};
const Burger::Word64ToDouble Burger::g_dRadiansToDegrees = {
    0x404CA5DC1A63C1F8ULL};
const Burger::Word64ToDouble Burger::g_dNegRadiansToDegrees = {
    0xC04CA5DC1A63C1F8ULL};
const Burger::Word64ToDouble Burger::g_dDegreesToRadians = {
    0x3F91DF46A2529D39ULL};
const Burger::Word64ToDouble Burger::g_dNegDegreesToRadians = {
    0xBF91DF46A2529D39ULL};
const Burger::Word64ToDouble Burger::g_dSqrt2 = {0x3FF6A09E667F3BCDULL};
const Burger::Word64ToDouble Burger::g_dNegSqrt2 = {0xBFF6A09E667F3BCDULL};
const Burger::Word64ToDouble Burger::g_dLN2 = {0x3FE62E42FEFA39EFULL};
const Burger::Word64ToDouble Burger::g_dNegLN2 = {0xBFE62E42FEFA39EFULL};
const Burger::Word64ToDouble Burger::g_dLN10 = {0x40026BB1BBB55516ULL};
const Burger::Word64ToDouble Burger::g_dNegLN10 = {0xC0026BB1BBB55516ULL};

const Burger::Word32ToFloat Burger::g_fOne = {0x3F800000U};
const Burger::Word32ToFloat Burger::g_fNegOne = {0xBF800000U};
const Burger::Word32ToFloat Burger::g_fHalf = {0x3F000000U};
const Burger::Word32ToFloat Burger::g_fNegHalf = {0xBF000000U};
const Burger::Word32ToFloat Burger::g_fInf = {0x7F800000U};
const Burger::Word32ToFloat Burger::g_fNegInf = {0xFF800000U};
const Burger::Word32ToFloat Burger::g_fNan = {0x7FBFFFFFU};
const Burger::Word32ToFloat Burger::g_fNegNan = {0xFFBFFFFFU};
const Burger::Word32ToFloat Burger::g_fQNan = {0x7FFFFFFFU};
const Burger::Word32ToFloat Burger::g_fNegQNan = {0xFFFFFFFFU};
const Burger::Word32ToFloat Burger::g_fMin = {0x00800000U};
const Burger::Word32ToFloat Burger::g_fNegMin = {0x80800000U};
const Burger::Word32ToFloat Burger::g_fMax = {0x7F7FFFFFU};
const Burger::Word32ToFloat Burger::g_fNegMax = {0xFF7FFFFFU};
const Burger::Word32ToFloat Burger::g_fMinNoInteger = {0x4B000000};
const Burger::Word32ToFloat Burger::g_fNegMinNoInteger = {0xCB000000};
const Burger::Word32ToFloat Burger::g_fEpsilon = {0x34000000U};
const Burger::Word32ToFloat Burger::g_fNegEpsilon = {0xB4000000U};
const Burger::Word32ToFloat Burger::g_fQuarterPi = {0x3F490FDB};
const Burger::Word32ToFloat Burger::g_fNegQuarterPi = {0xBF490FDB};
const Burger::Word32ToFloat Burger::g_fHalfPi = {0x3FC90FDB};
const Burger::Word32ToFloat Burger::g_fNegHalfPi = {0xBFC90FDB};
const Burger::Word32ToFloat Burger::g_fPi = {0x40490FDB};
const Burger::Word32ToFloat Burger::g_fNegPi = {0xC0490FDB};
const Burger::Word32ToFloat Burger::g_fPi2 = {0x40C90FDB};
const Burger::Word32ToFloat Burger::g_fNegPi2 = {0xC0C90FDB};
const Burger::Word32ToFloat Burger::g_fPi4 = {0x41490FDB};
const Burger::Word32ToFloat Burger::g_fNegPi4 = {0xC1490FDB};
const Burger::Word32ToFloat Burger::g_fCosPiDiv6 = {0x3F5DB3D7};
const Burger::Word32ToFloat Burger::g_fNegCosPiDiv6 = {0xBF5DB3D7};
const Burger::Word32ToFloat Burger::g_fReciprocalHalfPi = {0x3F22F983};
const Burger::Word32ToFloat Burger::g_fNegReciprocalHalfPi = {0xBF22F983};
const Burger::Word32ToFloat Burger::g_fReciprocalPi = {0x3EA2F983};
const Burger::Word32ToFloat Burger::g_fNegReciprocalPi = {0xBEA2F983};
const Burger::Word32ToFloat Burger::g_fReciprocalPi2 = {0x3E22F983};
const Burger::Word32ToFloat Burger::g_fNegReciprocalPi2 = {0xBE22F983};
const Burger::Word32ToFloat Burger::g_fRadiansToDegrees = {0x42652EE1};
const Burger::Word32ToFloat Burger::g_fNegRadiansToDegrees = {0xC2652EE1};
const Burger::Word32ToFloat Burger::g_fDegreesToRadians = {0x3C8EFA35};
const Burger::Word32ToFloat Burger::g_fNegDegreesToRadians = {0xBC8EFA35};
const Burger::Word32ToFloat Burger::g_fSqrt2 = {0x3FB504F3};
const Burger::Word32ToFloat Burger::g_fNegSqrt2 = {0xBFB504F3};
const Burger::Word32ToFloat Burger::g_fLN2 = {0x3F317218};
const Burger::Word32ToFloat Burger::g_fNegLN2 = {0xBF317218};
const Burger::Word32ToFloat Burger::g_fLN10 = {0x40135D8E};
const Burger::Word32ToFloat Burger::g_fNegLN10 = {0xC0135D8E};

/*! ************************************

    \enum Burger::e8087Precision
    \brief 32 bit Intel floating point precision

    On Intel 32 bit processors, there's a special floating point register to
    control the precision of the 80387 compatible FPU

    \note This enumeration only exists on 32 bit Intel compatible CPU targets

    \sa Set8087Precision(e8087Precision)

***************************************/

/*! ************************************

    \fn Burger::e8087Precision BURGER_API
        Burger::Set8087Precision(e8087Precision eInput)

    \brief Change the floating point precision

    On Intel 32 bit processors, there's a special floating point register to
    control the precision of the 80387 compatible FPU. This function allows the
    modification of this register for FPU precision. Lower precision yields more
    speed.

    \note This function only exists on 32 bit Intel compatible
        CPU targets.
    \note If Direct3D is enabled, Windows sets the precision to 24 bit
        on 32 bit platforms. This function will undo the change if
        that's not desired.

    \param eInput New enumeration state
    \return Previous precision state
    \sa e8087Precision or Get8087Precision(void)

***************************************/

#if defined(BURGER_X86) && \
    (defined(BURGER_WATCOM) || defined(BURGER_METROWERKS) || \
        defined(BURGER_MSVC))

BURGER_DECLSPECNAKED Burger::e8087Precision BURGER_API Burger::Set8087Precision(
    e8087Precision /* eInput */)
{
    // clang-format off
    BURGER_ASM {

#if defined(BURGER_WATCOM)
        sub esp, 12             // Space for the status word
        mov[esp + 8], ecx       // ecx must be preserved for Watcom
        mov ecx, eax
#else
        sub esp, 8              // Space for the status word
#endif
        fnstcw[esp]             // Get the current status word
        mov[esp + 4], edx
        mov eax, dword ptr[esp] // Get the current value for return
        shl ecx, 8              // Move to the Pentium bits area
        mov edx, eax            // Get the bits to retain for write
        and eax, 0x0300         // Mask for return value
        and edx, 0xFCFF         // Mask off unused bits
        shr eax, 8              // Convert to enum
        or edx, ecx             // Blend in the bits
        mov dword ptr[esp], edx // Store in memory
        fldcw[esp]              // Save the new status register
        mov edx, [esp + 4]
#if defined(BURGER_WATCOM)
        mov ecx, [esp + 8]
        add esp, 12             // Fix stack
#else
        add esp, 8              // Fix stack
#endif
        ret
    }
    // clang-format on
}

#elif defined(BURGER_X86) && defined(BURGER_MACOSX)

// __ZN6Burger16Set8087PrecisionENS_14e8087PrecisionE = Burger::e8087Precision
// BURGER_API Burger::Set8087Precision(e8087Precision /* eInput */)
__asm__(
    "	.align	4,0x90\n"
    "	.globl __ZN6Burger16Set8087PrecisionENS_14e8087PrecisionE\n"
    "__ZN6Burger16Set8087PrecisionENS_14e8087PrecisionE:\n"
    "	subl	%esp,12\n"      // Space for the status word
    "	movl	%ecx,8(%esp)\n" // ecx must be preserved for Watcom
    "	movl	%eax,%ecx\n"
    "	fnstcw	(%esp)\n" // Get the current status word
    "	movl	%edx,4(%esp)\n"
    "	movl	(%esp),%eax\n"  // Get the current value for return
    "	shll	$8,%ecx\n"      // Move to the Pentium bits area
    "	movl	%eax,%edx\n"    // Get the bits to retain for write
    "	andl	$0x0300,%eax\n" // Mask for return value
    "	andl	$0xFCFF,%edx\n" // Mask off unused bits
    "	shrl	$8,%eax\n"      // Convert to enum
    "	orl		%ecx,%edx\n"    // Blend in the bits
    "	movl	%edx,(%esp)\n"  // Store in memory
    "	fldcw	(%esp)\n"       // Save the new status register
    "	movl	4(%esp),%edx\n"
    "	movl	8(%esp),%ecx\n"
    "	addl	%esp,12\n" // Fix stack
    "	ret\n");

#endif

/*! ************************************

    \fn Burger::e8087Precision BURGER_API Burger::Get8087Precision(void)
    \brief Get the floating point precision

    On Intel 32 bit processors, there's a special floating point register to
    control the precision of the 80387 compatible FPU. This function returns the
    current setting.

    \note This function only exists on 32 bit Intel compatible
        CPU targets.

    \return Current precision state
    \sa e8087Precision or Set8087Precision(e8087Precision)

***************************************/

#if defined(BURGER_X86) && \
    (defined(BURGER_WATCOM) || defined(BURGER_METROWERKS) || \
        defined(BURGER_MSVC) || defined(BURGER_INTEL_COMPILER))

BURGER_DECLSPECNAKED Burger::e8087Precision BURGER_API Burger::Get8087Precision(
    void)
{
    // clang-format off
    BURGER_ASM
    {
        push eax    // Space for the status word
        fnstcw[esp] // Get the current status word
        pop eax     // Get the current value for return
        shr eax, 8  // Shift the bits of interest down
        and eax, 3  // Mask to enumeration
        ret
    }
    // clang-format on
}

#elif defined(BURGER_X86) && defined(BURGER_MACOSX)

// __ZN6Burger16Get8087PrecisionEv = Burger::e8087Precision BURGER_API
// Burger::Get8087Precision(void)
__asm__(
    "	.align	4,0x90\n"
    "	.globl __ZN6Burger16Get8087PrecisionEv\n"
    "__ZN6Burger16Get8087PrecisionEv:\n"
    "	pushl	%eax\n"         // Space for the status word
    "	fnstcw	(%esp)\n"       // Get the current status word
    "	popl	%eax\n"         // Get the current value for return
    "	shrl	$8,%eax\n"      // Shift the bits of interest down
    "	andl	$0x0003,%eax\n" // Mask to enumeration
    "	ret\n");
#endif

/*! ************************************

    \enum Burger::e8087Rounding
    \brief 32 bit Intel floating point rounding mode

    On Intel 32 bit processors, there's a special floating point register to
    control the rounding mode from float to integer conversions in the 80387
    compatible FPU

    \note This enumeration only exists on 32 bit Intel compatible CPU targets

    \sa Set8087Rounding(e8087Rounding)

***************************************/

/*! ************************************

    \fn Burger::e8087Rounding BURGER_API
        Burger::Set8087Rounding(e8087Rounding eInput)
    \brief Change the floating point precision

    On Intel 32 bit processors, there's a special floating point register to
    control the rounding of the 80387 compatible FPU. This function allows the
    modification of this register for FPU rounding.

    \note This function only exists on 32 bit Intel compatible
        CPU targets.

    \param eInput New enumeration state
    \return Previous rounding state
    \sa e8087Rounding or Get8087Rounding(void)

***************************************/

#if defined(BURGER_X86) && \
    (defined(BURGER_WATCOM) || defined(BURGER_METROWERKS) || \
        defined(BURGER_MSVC) || defined(BURGER_INTEL_COMPILER))

BURGER_DECLSPECNAKED Burger::e8087Rounding BURGER_API Burger::Set8087Rounding(
    e8087Rounding /* eInput */)
{
    // clang-format off
    BURGER_ASM
    {
#if defined(BURGER_WATCOM)
        sub esp, 12         // Space for the status word
        mov[esp + 8], ecx   // ecx must be preserved for Watcom
        mov ecx, eax
#else
        sub esp, 8          // Space for the status word
#endif
        fnstcw[esp]         // Get the current status word
        mov[esp + 4], edx
        mov eax, dword ptr[esp] // Get the current value for return
        shl ecx, 10         // Move to the Pentium bits area
        mov edx, eax        // Get the bits to retain for write
        and eax, 0x0C00     // Mask for return value
        and edx, 0xF3FF     // Mask off unused bits
        shr eax, 10         // Convert to enum
        or edx, ecx         // Blend in the bits
        mov dword ptr[esp], edx // Store in memory
        fldcw [esp]         // Save the new status register
        mov edx, [esp + 4]
#if defined(BURGER_WATCOM)
        mov ecx, [esp + 8]
        add esp, 12         // Fix stack
#else
        add esp, 8      // Fix stack
#endif
        ret
    }
    // clang-format on
}

#elif defined(BURGER_X86) && defined(BURGER_MACOSX)

// __ZN6Burger15Set8087RoundingENS_13e8087RoundingE = Burger::e8087Rounding
// BURGER_API Burger::Set8087Rounding(e8087Rounding /* eInput */)
__asm__(
    "	.align	4,0x90\n"
    "	.globl __ZN6Burger15Set8087RoundingENS_13e8087RoundingE\n"
    "__ZN6Burger15Set8087RoundingENS_13e8087RoundingE:\n"
    "	subl	%esp,12\n"      // Space for the status word
    "	movl	%ecx,8(%esp)\n" // ecx must be preserved for Watcom
    "	movl	%eax,%ecx\n"
    "	fnstcw	(%esp)\n" // Get the current status word
    "	movl	%edx,4(%esp)\n"
    "	movl	(%esp),%eax\n"  // Get the current value for return
    "	shll	$10,%ecx\n"     // Move to the Pentium bits area
    "	movl	%eax,%edx\n"    // Get the bits to retain for write
    "	andl	$0x0C00,%eax\n" // Mask for return value
    "	andl	$0xF3FF,%edx\n" // Mask off unused bits
    "	shrl	$10,%eax\n"     // Convert to enum
    "	orl		%ecx,%edx\n"    // Blend in the bits
    "	movl	%edx,(%esp)\n"  // Store in memory
    "	fldcw	(%esp)\n"       // Save the new status register
    "	movl	4(%esp),%edx\n"
    "	movl	8(%esp),%ecx\n"
    "	addl	%esp,12\n" // Fix stack
    "	ret\n");

#endif

/*! ************************************

    \fn Burger::e8087Rounding BURGER_API Burger::Get8087Rounding(void)
    \brief Get the floating point rounding setting

    On Intel 32 bit processors, there's a special floating point register to
    control the rounding behavior of the 80387 compatible FPU. This function
    returns the current setting.

    \note This function only exists on 32 bit Intel compatible
        CPU targets.

    \return Current precision state
    \sa e8087Rounding or Set8087Rounding(e8087Rounding)

***************************************/

#if defined(BURGER_X86) && \
    (defined(BURGER_WATCOM) || defined(BURGER_METROWERKS) || \
        defined(BURGER_MSVC) || defined(BURGER_INTEL_COMPILER))

BURGER_DECLSPECNAKED Burger::e8087Rounding BURGER_API Burger::Get8087Rounding(
    void)
{
    // clang-format off
    BURGER_ASM
    {
        push eax        // Space for the status word
        fnstcw [esp]    // Get the current status word
        pop eax         // Get the current value for return
        shr eax, 10     // Shift the bits of interest down
        and eax, 3      // Mask to enumeration
        ret
    }
    // clang-format on
}

#elif defined(BURGER_X86) && defined(BURGER_MACOSX)

// __ZN6Burger15Get8087RoundingEv = Burger::e8087Rounding BURGER_API
// Burger::Get8087Rounding(void)
__asm__(
    "	.align	4,0x90\n"
    "	.globl __ZN6Burger15Get8087RoundingEv\n"
    "__ZN6Burger15Get8087RoundingEv:\n"
    "	pushl	%eax\n"         // Space for the status word
    "	fnstcw	(%esp)\n"       // Get the current status word
    "	popl	%eax\n"         // Get the current value for return
    "	shrl	$10,%eax\n"     // Shift the bits of interest down
    "	andl	$0x0003,%eax\n" // Mask to enumeration
    "	ret\n");
#endif

/*! ************************************

    \enum Burger::ePowerPCRounding
    \brief PowerPC floating point rounding mode

    On PowerPC processors, there's a special floating point register to control
    the rounding mode when precision is lost.

    \note This enumeration only exists on PowerPC compatible
        CPU targets

    \sa SetPowerPCRounding(ePowerPCRounding)

***************************************/

/*! ************************************

    \fn Burger::ePowerPCRounding BURGER_API
        Burger::SetPowerPCRounding(ePowerPCRounding eInput)
    \brief Change the floating point precision

    On PowerPC processors, there's a special floating point register to control
    the rounding. This function allows the modification of this register for FPU
    rounding.

    \note This function only exists on PowerPC compatible CPU targets.

    \param eInput New enumeration state
    \return Previous rounding state
    \sa ePowerPCRounding or GetPowerPCRounding(void)

***************************************/

#if defined(BURGER_XBOX360) || \
    (defined(BURGER_PPC) && defined(BURGER_METROWERKS))

BURGER_DECLSPECNAKED Burger::ePowerPCRounding
    BURGER_API Burger::SetPowerPCRounding(ePowerPCRounding /* eInput */)
{
    // clang-format off
#if defined(BURGER_XBOX360)
    BURGER_ASM
    {
#endif
        mffs fp0            // Get the floating point control register
        stfd fp0, -8(r1)    // Store it in memory
        lwz r4, -4(r1)      // Load into an integer register
        extrwi r5, r4, 2, 30 // Get the return value, mask off the lowest 2 bits (AND & 3)
        rlwimi r4, r3, 0, 30, 31 // Move the lowest 2 bits into the existing value
        stw r4, -4(r1)      // Store it in memory
        lfd fp0, -8(r1)     // Reload the fp register
        mtfsf 255, fp0      // Set the flags
        mr r3, r5           // Return the previous value
        blr                 // Exit
#if defined(BURGER_XBOX360)
    }
#endif
    // clang-format on 
}

#elif defined(BURGER_PPC) && defined(BURGER_MACOSX)

__asm__(
    "	.align	2,0\n"
    "	.globl	__ZN6Burger18SetPowerPCRoundingENS_16ePowerPCRoundingE\n"
    "__ZN6Burger18SetPowerPCRoundingENS_16ePowerPCRoundingE:\n"
    "	mffs	f0\n"            // Get the floating point control register
    "	stfd	f0,-8(r1)\n"     // Store it in memory
    "	lwz		r4,-4(r1)\n"     // Load into an integer register
    "	extrwi	r5,r4,2,30\n"    // Get the return value, mask off the lowest 2
                                 // bits (AND & 3)
    "	rlwimi	r4,r3,0,30,31\n" // Move the lowest 2 bits into the existing
                                 // value
    "	stw		r4,-4(r1)\n"     // Store it in memory
    "	lfd		f0,-8(r1)\n"     // Reload the fp register
    "	mtfsf	255,f0\n"        // Set the flags
    "	mr		r3,r5\n"         // Return the previous value
    "	blr\n"                   // Exit
);

#endif

/*! ************************************

    \fn Burger::ePowerPCRounding BURGER_API Burger::GetPowerPCRounding(void)
    \brief Get the floating point rounding setting

    On PowerPC processors, there's a special floating point register to control
    the rounding behavior. This function returns the current setting.

    \note This function only exists on PowerPC compatible CPU targets.

    \return Current precision state
    \sa ePowerPCRounding or SetPowerPCRounding(ePowerPCRounding)

***************************************/

#if defined(BURGER_XBOX360) || \
    (defined(BURGER_PPC) && defined(BURGER_METROWERKS))

BURGER_DECLSPECNAKED Burger::ePowerPCRounding BURGER_API Burger::GetPowerPCRounding(void)
{
    // clang-format off
#if defined(BURGER_XBOX360)
    BURGER_ASM
    {
#endif
        mffs fp0            // Get the floating point control register
        stfd fp0, -8(r1)    // Store it in memory
        lwz r3, -4(r1)      // Load into an integer register
        extrwi r3, r3, 2, 30  // Mask off the lowest 2 bits (AND & 3)
        blr                 // Exit
#if defined(BURGER_XBOX360)
    }
#endif
    // clang-format on
}

#elif defined(BURGER_PPC) && defined(BURGER_MACOSX)

__asm__(
    "	.align	2,0\n"
    "	.globl	__ZN6Burger18GetPowerPCRoundingEv\n"
    "__ZN6Burger18GetPowerPCRoundingEv:\n"
    "	mffs	f0\n"         // Get the floating point control register
    "	stfd	f0,-8(r1)\n"  // Store it in memory
    "	lwz		r3,-4(r1)\n"  // Load into an integer register
    "	extrwi	r3,r3,2,30\n" // Mask off the lowest 2 bits (AND & 3)
    "	blr\n"                // Exit
);

#endif

/*! ************************************

    \fn Burger::Sqr(float fInput);
    \brief Get the square value of a float

    Return the squared value of the floating point input.

    \note -Inf will return +Inf. NaN is undefined

    \param fInput A valid single precision floating point number.
    \return The squared value of the input.
    \sa Sqr(double)

***************************************/

/*! ************************************

    \fn Burger::Sqr(double dInput);
    \brief Get the square value of a double

    Return the squared value of the 64 bit floating point input.

    \note -Inf will return +Inf. NaN is undefined

    \param dInput A valid double precision floating point number.
    \return The squared value of the input.
    \sa Sqr(float)

***************************************/

/*! ************************************

    \fn Burger::Abs(float fInput);
    \brief Get the absolute value of a float

    Return the absolute value of the floating point input.
    This is usually done by inline assembly.

    \note -Inf will return +Inf. NaN is undefined

    \param fInput A valid single precision floating point number.
    \return The absolute value of the input.
    \sa Abs(int32_t), Abs(int64_t) and Abs(double)

***************************************/

#if defined(BURGER_WATCOM) || defined(BURGER_INTEL_COMPILER)
#elif defined(BURGER_METROWERKS) && defined(BURGER_PPC)
#elif defined(BURGER_METROWERKS) && defined(BURGER_X86)
#elif defined(BURGER_XBOX360) && !defined(DOXYGEN)
#elif defined(BURGER_PS3) || defined(BURGER_PS4) || defined(BURGER_VITA)
#elif defined(BURGER_MSVC) && (defined(BURGER_INTEL) || defined(BURGER_ARM))
#elif defined(BURGER_MACOSX) || defined(BURGER_IOS) || defined(BURGER_LINUX)
#else
float BURGER_API Burger::Abs(float fInput) BURGER_NOEXCEPT
{
    return fInput >= 0 ? fInput : -fInput;
}
#endif

/*! ************************************

    \fn Burger::Abs(double dInput);
    \brief Get the absolute value of a double

    Return the absolute value of the floating point input.
    This is usually done by inline assembly.

    \note -Inf will return +Inf. NaN is undefined

    \param dInput A valid double precision floating point number.
    \return The absolute value of the input.
    \sa Abs(int32_t), Abs(int64_t) and Abs(float)

***************************************/

#if defined(BURGER_WATCOM) || defined(BURGER_INTEL_COMPILER)
#elif defined(BURGER_METROWERKS) && defined(BURGER_PPC)
#elif defined(BURGER_METROWERKS) && defined(BURGER_X86)
#elif defined(BURGER_XBOX360) && !defined(DOXYGEN)
#elif defined(BURGER_PS3) || defined(BURGER_PS4) || defined(BURGER_VITA)
#elif defined(BURGER_MSVC) && (defined(BURGER_INTEL) || defined(BURGER_ARM))
#elif defined(BURGER_MACOSX) || defined(BURGER_IOS) || defined(BURGER_LINUX)
#else
double BURGER_API Burger::Abs(double dInput) BURGER_NOEXCEPT
{
    return dInput >= 0 ? dInput : -dInput;
}
#endif

/*! ************************************

    \fn Burger::Sqrt(float fInput);
    \brief Get the square root value of a float

    Return the square root of the floating point input.
    This is usually done by inline assembly.

    \param fInput A valid single precision floating point number.
    \return The square root of the input.
    \sa Sqrt(double), Sqrt(uint32_t), and Sqrt(Fixed32)

***************************************/

#if defined(BURGER_WATCOM) || defined(BURGER_INTEL_COMPILER)
#elif defined(BURGER_METROWERKS) && defined(BURGER_PPC)

//
// Lookup table for sqrt initial guess and iteration slope
// 2 tables, first is the even entries, followed by the odd entries
// Each entry is a pair, the first is the average sqrt, and second
// is the reciprocal average / 2
//

static const Burger::Word32ToFloat g_PPCSqrtGuess[2][256][2] = {
    {{{0x3F353229}, {0x3F34D7D4}}, {{0x3F358C7F}, {0x3F347DD8}},
        {{0x3F35E6A7}, {0x3F342462}}, {{0x3F3640A3}, {0x3F33CB71}},
        {{0x3F369A73}, {0x3F337303}}, {{0x3F36F416}, {0x3F331B17}},
        {{0x3F374D8E}, {0x3F32C3AC}}, {{0x3F37A6DA}, {0x3F326CC0}},
        {{0x3F37FFFB}, {0x3F321652}}, {{0x3F3858F0}, {0x3F31C062}},
        {{0x3F38B1BB}, {0x3F316AEE}}, {{0x3F390A5B}, {0x3F3115F4}},
        {{0x3F3962D1}, {0x3F30C174}}, {{0x3F39BB1D}, {0x3F306D6D}},
        {{0x3F3A133F}, {0x3F3019DC}}, {{0x3F3A6B37}, {0x3F2FC6C3}},
        {{0x3F3AC305}, {0x3F2F741E}}, {{0x3F3B1AAB}, {0x3F2F21EE}},
        {{0x3F3B7227}, {0x3F2ED030}}, {{0x3F3BC97B}, {0x3F2E7EE5}},
        {{0x3F3C20A6}, {0x3F2E2E0B}}, {{0x3F3C77A9}, {0x3F2DDDA0}},
        {{0x3F3CCE84}, {0x3F2D8DA5}}, {{0x3F3D2537}, {0x3F2D3E18}},
        {{0x3F3D7BC2}, {0x3F2CEEF7}}, {{0x3F3DD226}, {0x3F2CA043}},
        {{0x3F3E2863}, {0x3F2C51FA}}, {{0x3F3E7E78}, {0x3F2C041B}},
        {{0x3F3ED467}, {0x3F2BB6A5}}, {{0x3F3F2A2F}, {0x3F2B6997}},
        {{0x3F3F7FD0}, {0x3F2B1CF1}}, {{0x3F3FD54C}, {0x3F2AD0B1}},
        {{0x3F402AA1}, {0x3F2A84D7}}, {{0x3F407FD1}, {0x3F2A3961}},
        {{0x3F40D4DB}, {0x3F29EE50}}, {{0x3F4129BF}, {0x3F29A3A1}},
        {{0x3F417E7E}, {0x3F295954}}, {{0x3F41D319}, {0x3F290F69}},
        {{0x3F42278E}, {0x3F28C5DE}}, {{0x3F427BDE}, {0x3F287CB3}},
        {{0x3F42D00B}, {0x3F2833E7}}, {{0x3F432412}, {0x3F27EB78}},
        {{0x3F4377F6}, {0x3F27A367}}, {{0x3F43CBB6}, {0x3F275BB3}},
        {{0x3F441F52}, {0x3F27145A}}, {{0x3F4472CA}, {0x3F26CD5C}},
        {{0x3F44C61F}, {0x3F2686B8}}, {{0x3F451951}, {0x3F26406E}},
        {{0x3F456C60}, {0x3F25FA7D}}, {{0x3F45BF4C}, {0x3F25B4E3}},
        {{0x3F461215}, {0x3F256FA1}}, {{0x3F4664BB}, {0x3F252AB5}},
        {{0x3F46B73F}, {0x3F24E61F}}, {{0x3F4709A1}, {0x3F24A1DE}},
        {{0x3F475BE1}, {0x3F245DF2}}, {{0x3F47ADFF}, {0x3F241A5A}},
        {{0x3F47FFFC}, {0x3F23D715}}, {{0x3F4851D7}, {0x3F239422}},
        {{0x3F48A390}, {0x3F235181}}, {{0x3F48F528}, {0x3F230F31}},
        {{0x3F49469F}, {0x3F22CD31}}, {{0x3F4997F5}, {0x3F228B82}},
        {{0x3F49E92B}, {0x3F224A22}}, {{0x3F4A3A40}, {0x3F220910}},
        {{0x3F4A8B34}, {0x3F21C84D}}, {{0x3F4ADC08}, {0x3F2187D7}},
        {{0x3F4B2CBC}, {0x3F2147AD}}, {{0x3F4B7D50}, {0x3F2107D0}},
        {{0x3F4BCDC4}, {0x3F20C83F}}, {{0x3F4C1E18}, {0x3F2088F8}},
        {{0x3F4C6E4D}, {0x3F2049FC}}, {{0x3F4CBE62}, {0x3F200B4A}},
        {{0x3F4D0E58}, {0x3F1FCCE2}}, {{0x3F4D5E2F}, {0x3F1F8EC2}},
        {{0x3F4DADE7}, {0x3F1F50EA}}, {{0x3F4DFD80}, {0x3F1F135A}},
        {{0x3F4E4CFA}, {0x3F1ED611}}, {{0x3F4E9C56}, {0x3F1E990F}},
        {{0x3F4EEB93}, {0x3F1E5C53}}, {{0x3F4F3AB2}, {0x3F1E1FDC}},
        {{0x3F4F89B3}, {0x3F1DE3AB}}, {{0x3F4FD896}, {0x3F1DA7BE}},
        {{0x3F50275B}, {0x3F1D6C15}}, {{0x3F507602}, {0x3F1D30B0}},
        {{0x3F50C48C}, {0x3F1CF58D}}, {{0x3F5112F8}, {0x3F1CBAAE}},
        {{0x3F516146}, {0x3F1C8010}}, {{0x3F51AF78}, {0x3F1C45B4}},
        {{0x3F51FD8C}, {0x3F1C0B98}}, {{0x3F524B84}, {0x3F1BD1BE}},
        {{0x3F52995E}, {0x3F1B9824}}, {{0x3F52E71C}, {0x3F1B5EC9}},
        {{0x3F5334BD}, {0x3F1B25AE}}, {{0x3F538241}, {0x3F1AECD1}},
        {{0x3F53CFAA}, {0x3F1AB433}}, {{0x3F541CF6}, {0x3F1A7BD3}},
        {{0x3F546A26}, {0x3F1A43B0}}, {{0x3F54B739}, {0x3F1A0BCA}},
        {{0x3F550431}, {0x3F19D421}}, {{0x3F55510E}, {0x3F199CB4}},
        {{0x3F559DCE}, {0x3F196582}}, {{0x3F55EA73}, {0x3F192E8C}},
        {{0x3F5636FD}, {0x3F18F7D1}}, {{0x3F56836B}, {0x3F18C151}},
        {{0x3F56CFBE}, {0x3F188B0A}}, {{0x3F571BF6}, {0x3F1854FD}},
        {{0x3F576813}, {0x3F181F2A}}, {{0x3F57B415}, {0x3F17E98F}},
        {{0x3F57FFFD}, {0x3F17B42D}}, {{0x3F584BC9}, {0x3F177F03}},
        {{0x3F58977C}, {0x3F174A11}}, {{0x3F58E314}, {0x3F171556}},
        {{0x3F592E91}, {0x3F16E0D2}}, {{0x3F5979F4}, {0x3F16AC85}},
        {{0x3F59C53E}, {0x3F16786E}}, {{0x3F5A106D}, {0x3F16448C}},
        {{0x3F5A5B82}, {0x3F1610E1}}, {{0x3F5AA67E}, {0x3F15DD6A}},
        {{0x3F5AF160}, {0x3F15AA29}}, {{0x3F5B3C28}, {0x3F15771B}},
        {{0x3F5B86D7}, {0x3F154442}}, {{0x3F5BD16C}, {0x3F15119D}},
        {{0x3F5C1BE8}, {0x3F14DF2B}}, {{0x3F5C664B}, {0x3F14ACEC}},
        {{0x3F5CB095}, {0x3F147AE0}}, {{0x3F5CFAC6}, {0x3F144906}},
        {{0x3F5D44DE}, {0x3F14175F}}, {{0x3F5D8EDE}, {0x3F13E5E9}},
        {{0x3F5DD8C4}, {0x3F13B4A4}}, {{0x3F5E2292}, {0x3F138391}},
        {{0x3F5E6C48}, {0x3F1352AE}}, {{0x3F5EB5E5}, {0x3F1321FC}},
        {{0x3F5EFF6A}, {0x3F12F17A}}, {{0x3F5F48D7}, {0x3F12C128}},
        {{0x3F5F922B}, {0x3F129106}}, {{0x3F5FDB68}, {0x3F126112}},
        {{0x3F60248C}, {0x3F12314E}}, {{0x3F606D99}, {0x3F1201B8}},
        {{0x3F60B68E}, {0x3F11D251}}, {{0x3F60FF6B}, {0x3F11A318}},
        {{0x3F614831}, {0x3F11740C}}, {{0x3F6190DF}, {0x3F11452E}},
        {{0x3F61D976}, {0x3F11167D}}, {{0x3F6221F6}, {0x3F10E7F9}},
        {{0x3F626A5E}, {0x3F10B9A2}}, {{0x3F62B2B0}, {0x3F108B77}},
        {{0x3F62FAEA}, {0x3F105D78}}, {{0x3F63430D}, {0x3F102FA5}},
        {{0x3F638B1A}, {0x3F1001FD}}, {{0x3F63D30F}, {0x3F0FD481}},
        {{0x3F641AEE}, {0x3F0FA730}}, {{0x3F6462B6}, {0x3F0F7A09}},
        {{0x3F64AA68}, {0x3F0F4D0D}}, {{0x3F64F203}, {0x3F0F203B}},
        {{0x3F653988}, {0x3F0EF393}}, {{0x3F6580F7}, {0x3F0EC715}},
        {{0x3F65C850}, {0x3F0E9AC0}}, {{0x3F660F92}, {0x3F0E6E95}},
        {{0x3F6656BE}, {0x3F0E4292}}, {{0x3F669DD4}, {0x3F0E16B8}},
        {{0x3F66E4D5}, {0x3F0DEB06}}, {{0x3F672BC0}, {0x3F0DBF7D}},
        {{0x3F677294}, {0x3F0D941C}}, {{0x3F67B954}, {0x3F0D68E2}},
        {{0x3F67FFFD}, {0x3F0D3DD0}}, {{0x3F684691}, {0x3F0D12E5}},
        {{0x3F688D10}, {0x3F0CE821}}, {{0x3F68D37A}, {0x3F0CBD84}},
        {{0x3F6919CE}, {0x3F0C930E}}, {{0x3F69600D}, {0x3F0C68BE}},
        {{0x3F69A637}, {0x3F0C3E94}}, {{0x3F69EC4B}, {0x3F0C1490}},
        {{0x3F6A324B}, {0x3F0BEAB1}}, {{0x3F6A7836}, {0x3F0BC0F8}},
        {{0x3F6ABE0C}, {0x3F0B9764}}, {{0x3F6B03CD}, {0x3F0B6DF6}},
        {{0x3F6B497A}, {0x3F0B44AC}}, {{0x3F6B8F12}, {0x3F0B1B87}},
        {{0x3F6BD496}, {0x3F0AF286}}, {{0x3F6C1A05}, {0x3F0AC9A9}},
        {{0x3F6C5F5F}, {0x3F0AA0F0}}, {{0x3F6CA4A6}, {0x3F0A785B}},
        {{0x3F6CE9D8}, {0x3F0A4FEA}}, {{0x3F6D2EF6}, {0x3F0A279C}},
        {{0x3F6D73FF}, {0x3F09FF71}}, {{0x3F6DB8F5}, {0x3F09D769}},
        {{0x3F6DFDD7}, {0x3F09AF83}}, {{0x3F6E42A5}, {0x3F0987C1}},
        {{0x3F6E875F}, {0x3F096021}}, {{0x3F6ECC05}, {0x3F0938A2}},
        {{0x3F6F1097}, {0x3F091146}}, {{0x3F6F5516}, {0x3F08EA0C}},
        {{0x3F6F9981}, {0x3F08C2F3}}, {{0x3F6FDDD9}, {0x3F089BFC}},
        {{0x3F70221D}, {0x3F087526}}, {{0x3F70664E}, {0x3F084E71}},
        {{0x3F70AA6C}, {0x3F0827DD}}, {{0x3F70EE76}, {0x3F080169}},
        {{0x3F71326D}, {0x3F07DB16}}, {{0x3F717651}, {0x3F07B4E3}},
        {{0x3F71BA22}, {0x3F078ED1}}, {{0x3F71FDE0}, {0x3F0768DE}},
        {{0x3F72418B}, {0x3F07430C}}, {{0x3F728523}, {0x3F071D59}},
        {{0x3F72C8A8}, {0x3F06F7C5}}, {{0x3F730C1B}, {0x3F06D251}},
        {{0x3F734F7B}, {0x3F06ACFB}}, {{0x3F7392C8}, {0x3F0687C5}},
        {{0x3F73D602}, {0x3F0662AD}}, {{0x3F74192B}, {0x3F063DB5}},
        {{0x3F745C40}, {0x3F0618DA}}, {{0x3F749F43}, {0x3F05F41E}},
        {{0x3F74E234}, {0x3F05CF80}}, {{0x3F752513}, {0x3F05AB00}},
        {{0x3F7567DF}, {0x3F05869D}}, {{0x3F75AA9A}, {0x3F056259}},
        {{0x3F75ED42}, {0x3F053E32}}, {{0x3F762FD8}, {0x3F051A28}},
        {{0x3F76725C}, {0x3F04F63B}}, {{0x3F76B4CE}, {0x3F04D26C}},
        {{0x3F76F72E}, {0x3F04AEB9}}, {{0x3F77397D}, {0x3F048B23}},
        {{0x3F777BB9}, {0x3F0467AA}}, {{0x3F77BDE4}, {0x3F04444D}},
        {{0x3F77FFFE}, {0x3F04210C}}, {{0x3F784206}, {0x3F03FDE7}},
        {{0x3F7883FC}, {0x3F03DADF}}, {{0x3F78C5E0}, {0x3F03B7F2}},
        {{0x3F7907B4}, {0x3F039521}}, {{0x3F794976}, {0x3F03726B}},
        {{0x3F798B26}, {0x3F034FD1}}, {{0x3F79CCC5}, {0x3F032D52}},
        {{0x3F7A0E53}, {0x3F030AEE}}, {{0x3F7A4FD0}, {0x3F02E8A6}},
        {{0x3F7A913C}, {0x3F02C678}}, {{0x3F7AD297}, {0x3F02A465}},
        {{0x3F7B13E1}, {0x3F02826C}}, {{0x3F7B5519}, {0x3F02608E}},
        {{0x3F7B9641}, {0x3F023ECA}}, {{0x3F7BD758}, {0x3F021D21}},
        {{0x3F7C185E}, {0x3F01FB91}}, {{0x3F7C5954}, {0x3F01DA1B}},
        {{0x3F7C9A38}, {0x3F01B8BF}}, {{0x3F7CDB0C}, {0x3F01977D}},
        {{0x3F7D1BD0}, {0x3F017654}}, {{0x3F7D5C83}, {0x3F015545}},
        {{0x3F7D9D25}, {0x3F01344F}}, {{0x3F7DDDB7}, {0x3F011372}},
        {{0x3F7E1E39}, {0x3F00F2AE}}, {{0x3F7E5EAA}, {0x3F00D204}},
        {{0x3F7E9F0B}, {0x3F00B171}}, {{0x3F7EDF5B}, {0x3F0090F8}},
        {{0x3F7F1F9C}, {0x3F007097}}, {{0x3F7F5FCC}, {0x3F00504E}},
        {{0x3F7F9FEC}, {0x3F00301E}}, {{0x3F7FDFFC}, {0x3F001006}}},
    {{{0x3F801FF8}, {0x3EFFC030}}, {{0x3F805FD8}, {0x3EFF40EF}},
        {{0x3F809F99}, {0x3EFEC26B}}, {{0x3F80DF39}, {0x3EFE44A2}},
        {{0x3F811EBB}, {0x3EFDC793}}, {{0x3F815E1D}, {0x3EFD4B3C}},
        {{0x3F819D61}, {0x3EFCCF9B}}, {{0x3F81DC85}, {0x3EFC54AE}},
        {{0x3F821B8B}, {0x3EFBDA74}}, {{0x3F825A72}, {0x3EFB60EA}},
        {{0x3F82993C}, {0x3EFAE810}}, {{0x3F82D7E6}, {0x3EFA6FE4}},
        {{0x3F831674}, {0x3EF9F864}}, {{0x3F8354E3}, {0x3EF9818E}},
        {{0x3F839334}, {0x3EF90B61}}, {{0x3F83D168}, {0x3EF895DB}},
        {{0x3F840F7F}, {0x3EF820FC}}, {{0x3F844D79}, {0x3EF7ACC0}},
        {{0x3F848B56}, {0x3EF73927}}, {{0x3F84C916}, {0x3EF6C62F}},
        {{0x3F8506B9}, {0x3EF653D8}}, {{0x3F854440}, {0x3EF5E21E}},
        {{0x3F8581AA}, {0x3EF57102}}, {{0x3F85BEF8}, {0x3EF50081}},
        {{0x3F85FC2A}, {0x3EF4909A}}, {{0x3F863941}, {0x3EF4214C}},
        {{0x3F86763B}, {0x3EF3B295}}, {{0x3F86B31A}, {0x3EF34475}},
        {{0x3F86EFDD}, {0x3EF2D6E9}}, {{0x3F872C85}, {0x3EF269F1}},
        {{0x3F876912}, {0x3EF1FD8B}}, {{0x3F87A584}, {0x3EF191B5}},
        {{0x3F87E1DB}, {0x3EF12670}}, {{0x3F881E17}, {0x3EF0BBB9}},
        {{0x3F885A39}, {0x3EF0518F}}, {{0x3F889640}, {0x3EEFE7F1}},
        {{0x3F88D22D}, {0x3EEF7EDE}}, {{0x3F890E00}, {0x3EEF1654}},
        {{0x3F8949B8}, {0x3EEEAE53}}, {{0x3F898557}, {0x3EEE46D9}},
        {{0x3F89C0DC}, {0x3EEDDFE5}}, {{0x3F89FC47}, {0x3EED7977}},
        {{0x3F8A3799}, {0x3EED138C}}, {{0x3F8A72D1}, {0x3EECAE24}},
        {{0x3F8AADF0}, {0x3EEC493D}}, {{0x3F8AE8F5}, {0x3EEBE4D8}},
        {{0x3F8B23E2}, {0x3EEB80F1}}, {{0x3F8B5EB6}, {0x3EEB1D8A}},
        {{0x3F8B9971}, {0x3EEABA9F}}, {{0x3F8BD413}, {0x3EEA5832}},
        {{0x3F8C0E9D}, {0x3EE9F63F}}, {{0x3F8C490E}, {0x3EE994C7}},
        {{0x3F8C8368}, {0x3EE933C9}}, {{0x3F8CBDA8}, {0x3EE8D342}},
        {{0x3F8CF7D1}, {0x3EE87334}}, {{0x3F8D31E2}, {0x3EE8139C}},
        {{0x3F8D6BDB}, {0x3EE7B479}}, {{0x3F8DA5BC}, {0x3EE755CB}},
        {{0x3F8DDF86}, {0x3EE6F791}}, {{0x3F8E1938}, {0x3EE699C9}},
        {{0x3F8E52D3}, {0x3EE63C74}}, {{0x3F8E8C56}, {0x3EE5DF8F}},
        {{0x3F8EC5C3}, {0x3EE5831B}}, {{0x3F8EFF18}, {0x3EE52715}},
        {{0x3F8F3856}, {0x3EE4CB7E}}, {{0x3F8F717E}, {0x3EE47055}},
        {{0x3F8FAA8E}, {0x3EE41598}}, {{0x3F8FE389}, {0x3EE3BB47}},
        {{0x3F901C6C}, {0x3EE36160}}, {{0x3F905539}, {0x3EE307E4}},
        {{0x3F908DF0}, {0x3EE2AED2}}, {{0x3F90C691}, {0x3EE25628}},
        {{0x3F90FF1B}, {0x3EE1FDE5}}, {{0x3F913790}, {0x3EE1A60A}},
        {{0x3F916FEE}, {0x3EE14E94}}, {{0x3F91A837}, {0x3EE0F784}},
        {{0x3F91E06A}, {0x3EE0A0D9}}, {{0x3F921888}, {0x3EE04A92}},
        {{0x3F92508F}, {0x3EDFF4AD}}, {{0x3F928882}, {0x3EDF9F2B}},
        {{0x3F92C05F}, {0x3EDF4A0B}}, {{0x3F92F827}, {0x3EDEF54B}},
        {{0x3F932FDA}, {0x3EDEA0EC}}, {{0x3F936777}, {0x3EDE4CED}},
        {{0x3F939F00}, {0x3EDDF94C}}, {{0x3F93D674}, {0x3EDDA609}},
        {{0x3F940DD3}, {0x3EDD5324}}, {{0x3F94451E}, {0x3EDD009B}},
        {{0x3F947C54}, {0x3EDCAE6E}}, {{0x3F94B375}, {0x3EDC5C9D}},
        {{0x3F94EA82}, {0x3EDC0B27}}, {{0x3F95217B}, {0x3EDBBA0B}},
        {{0x3F95585F}, {0x3EDB6948}}, {{0x3F958F2F}, {0x3EDB18DE}},
        {{0x3F95C5EB}, {0x3EDAC8CC}}, {{0x3F95FC94}, {0x3EDA7911}},
        {{0x3F963328}, {0x3EDA29AE}}, {{0x3F9669A9}, {0x3ED9DAA1}},
        {{0x3F96A015}, {0x3ED98BE9}}, {{0x3F96D66F}, {0x3ED93D87}},
        {{0x3F970CB4}, {0x3ED8EF79}}, {{0x3F9742E6}, {0x3ED8A1BF}},
        {{0x3F977905}, {0x3ED85458}}, {{0x3F97AF10}, {0x3ED80744}},
        {{0x3F97E509}, {0x3ED7BA82}}, {{0x3F981AEE}, {0x3ED76E12}},
        {{0x3F9850C0}, {0x3ED721F3}}, {{0x3F98867F}, {0x3ED6D624}},
        {{0x3F98BC2B}, {0x3ED68AA5}}, {{0x3F98F1C4}, {0x3ED63F76}},
        {{0x3F99274B}, {0x3ED5F495}}, {{0x3F995CBE}, {0x3ED5AA03}},
        {{0x3F999220}, {0x3ED55FBF}}, {{0x3F99C76E}, {0x3ED515C7}},
        {{0x3F99FCAB}, {0x3ED4CC1D}}, {{0x3F9A31D4}, {0x3ED482BE}},
        {{0x3F9A66EC}, {0x3ED439AC}}, {{0x3F9A9BF1}, {0x3ED3F0E4}},
        {{0x3F9AD0E4}, {0x3ED3A867}}, {{0x3F9B05C5}, {0x3ED36034}},
        {{0x3F9B3A95}, {0x3ED3184B}}, {{0x3F9B6F52}, {0x3ED2D0AC}},
        {{0x3F9BA3FD}, {0x3ED28955}}, {{0x3F9BD896}, {0x3ED24246}},
        {{0x3F9C0D1E}, {0x3ED1FB7F}}, {{0x3F9C4194}, {0x3ED1B4FF}},
        {{0x3F9C75F9}, {0x3ED16EC6}}, {{0x3F9CAA4C}, {0x3ED128D3}},
        {{0x3F9CDE8D}, {0x3ED0E327}}, {{0x3F9D12BD}, {0x3ED09DBF}},
        {{0x3F9D46DC}, {0x3ED0589D}}, {{0x3F9D7AE9}, {0x3ED013BF}},
        {{0x3F9DAEE6}, {0x3ECFCF26}}, {{0x3F9DE2D1}, {0x3ECF8AD0}},
        {{0x3F9E16AB}, {0x3ECF46BD}}, {{0x3F9E4A74}, {0x3ECF02ED}},
        {{0x3F9E7E2D}, {0x3ECEBF60}}, {{0x3F9EB1D4}, {0x3ECE7C14}},
        {{0x3F9EE56B}, {0x3ECE390A}}, {{0x3F9F18F1}, {0x3ECDF641}},
        {{0x3F9F4C66}, {0x3ECDB3B9}}, {{0x3F9F7FCB}, {0x3ECD7171}},
        {{0x3F9FB31F}, {0x3ECD2F6A}}, {{0x3F9FE662}, {0x3ECCEDA1}},
        {{0x3FA01996}, {0x3ECCAC18}}, {{0x3FA04CB8}, {0x3ECC6ACD}},
        {{0x3FA07FCB}, {0x3ECC29C1}}, {{0x3FA0B2CD}, {0x3ECBE8F3}},
        {{0x3FA0E5BF}, {0x3ECBA862}}, {{0x3FA118A1}, {0x3ECB680F}},
        {{0x3FA14B73}, {0x3ECB27F8}}, {{0x3FA17E35}, {0x3ECAE81D}},
        {{0x3FA1B0E8}, {0x3ECAA87F}}, {{0x3FA1E38A}, {0x3ECA691D}},
        {{0x3FA2161C}, {0x3ECA29F5}}, {{0x3FA2489F}, {0x3EC9EB09}},
        {{0x3FA27B12}, {0x3EC9AC57}}, {{0x3FA2AD75}, {0x3EC96DE0}},
        {{0x3FA2DFC9}, {0x3EC92FA3}}, {{0x3FA3120D}, {0x3EC8F19F}},
        {{0x3FA34441}, {0x3EC8B3D4}}, {{0x3FA37667}, {0x3EC87642}},
        {{0x3FA3A87D}, {0x3EC838E9}}, {{0x3FA3DA83}, {0x3EC7FBC8}},
        {{0x3FA40C7B}, {0x3EC7BEDE}}, {{0x3FA43E63}, {0x3EC7822D}},
        {{0x3FA4703C}, {0x3EC745B2}}, {{0x3FA4A205}, {0x3EC7096E}},
        {{0x3FA4D3C0}, {0x3EC6CD61}}, {{0x3FA5056C}, {0x3EC6918A}},
        {{0x3FA53709}, {0x3EC655E9}}, {{0x3FA56897}, {0x3EC61A7E}},
        {{0x3FA59A16}, {0x3EC5DF48}}, {{0x3FA5CB87}, {0x3EC5A447}},
        {{0x3FA5FCE9}, {0x3EC5697A}}, {{0x3FA62E3C}, {0x3EC52EE2}},
        {{0x3FA65F80}, {0x3EC4F47E}}, {{0x3FA690B6}, {0x3EC4BA4D}},
        {{0x3FA6C1DD}, {0x3EC48051}}, {{0x3FA6F2F6}, {0x3EC44687}},
        {{0x3FA72400}, {0x3EC40CF0}}, {{0x3FA754FD}, {0x3EC3D38C}},
        {{0x3FA785EA}, {0x3EC39A5A}}, {{0x3FA7B6CA}, {0x3EC3615A}},
        {{0x3FA7E79B}, {0x3EC3288C}}, {{0x3FA8185E}, {0x3EC2EFEF}},
        {{0x3FA84913}, {0x3EC2B783}}, {{0x3FA879BA}, {0x3EC27F48}},
        {{0x3FA8AA53}, {0x3EC2473E}}, {{0x3FA8DADD}, {0x3EC20F64}},
        {{0x3FA90B5A}, {0x3EC1D7BA}}, {{0x3FA93BC9}, {0x3EC1A040}},
        {{0x3FA96C2A}, {0x3EC168F6}}, {{0x3FA99C7E}, {0x3EC131DB}},
        {{0x3FA9CCC3}, {0x3EC0FAEE}}, {{0x3FA9FCFB}, {0x3EC0C431}},
        {{0x3FAA2D25}, {0x3EC08DA2}}, {{0x3FAA5D42}, {0x3EC05741}},
        {{0x3FAA8D51}, {0x3EC0210E}}, {{0x3FAABD53}, {0x3EBFEB09}},
        {{0x3FAAED47}, {0x3EBFB531}}, {{0x3FAB1D2D}, {0x3EBF7F87}},
        {{0x3FAB4D06}, {0x3EBF4A0A}}, {{0x3FAB7CD2}, {0x3EBF14B9}},
        {{0x3FABAC91}, {0x3EBEDF95}}, {{0x3FABDC42}, {0x3EBEAA9D}},
        {{0x3FAC0BE6}, {0x3EBE75D1}}, {{0x3FAC3B7D}, {0x3EBE4130}},
        {{0x3FAC6B07}, {0x3EBE0CBC}}, {{0x3FAC9A83}, {0x3EBDD872}},
        {{0x3FACC9F3}, {0x3EBDA454}}, {{0x3FACF956}, {0x3EBD7060}},
        {{0x3FAD28AB}, {0x3EBD3C98}}, {{0x3FAD57F4}, {0x3EBD08F9}},
        {{0x3FAD8730}, {0x3EBCD585}}, {{0x3FADB65F}, {0x3EBCA23A}},
        {{0x3FADE581}, {0x3EBC6F19}}, {{0x3FAE1496}, {0x3EBC3C22}},
        {{0x3FAE439F}, {0x3EBC0954}}, {{0x3FAE729B}, {0x3EBBD6AF}},
        {{0x3FAEA18A}, {0x3EBBA433}}, {{0x3FAED06D}, {0x3EBB71E0}},
        {{0x3FAEFF43}, {0x3EBB3FB5}}, {{0x3FAF2E0D}, {0x3EBB0DB2}},
        {{0x3FAF5CCA}, {0x3EBADBD7}}, {{0x3FAF8B7B}, {0x3EBAAA24}},
        {{0x3FAFBA1F}, {0x3EBA7898}}, {{0x3FAFE8B7}, {0x3EBA4734}},
        {{0x3FB01743}, {0x3EBA15F7}}, {{0x3FB045C2}, {0x3EB9E4E1}},
        {{0x3FB07435}, {0x3EB9B3F2}}, {{0x3FB0A29C}, {0x3EB98329}},
        {{0x3FB0D0F7}, {0x3EB95287}}, {{0x3FB0FF45}, {0x3EB9220A}},
        {{0x3FB12D88}, {0x3EB8F1B4}}, {{0x3FB15BBE}, {0x3EB8C184}},
        {{0x3FB189E9}, {0x3EB89179}}, {{0x3FB1B807}, {0x3EB86194}},
        {{0x3FB1E619}, {0x3EB831D3}}, {{0x3FB21420}, {0x3EB80238}},
        {{0x3FB2421B}, {0x3EB7D2C2}}, {{0x3FB27009}, {0x3EB7A370}},
        {{0x3FB29DEC}, {0x3EB77443}}, {{0x3FB2CBC3}, {0x3EB7453A}},
        {{0x3FB2F98F}, {0x3EB71655}}, {{0x3FB3274F}, {0x3EB6E794}},
        {{0x3FB35503}, {0x3EB6B8F7}}, {{0x3FB382AB}, {0x3EB68A7D}},
        {{0x3FB3B048}, {0x3EB65C27}}, {{0x3FB3DDD9}, {0x3EB62DF4}},
        {{0x3FB40B5F}, {0x3EB5FFE4}}, {{0x3FB438D9}, {0x3EB5D1F7}},
        {{0x3FB46648}, {0x3EB5A42D}}, {{0x3FB493AB}, {0x3EB57685}},
        {{0x3FB4C103}, {0x3EB54900}}, {{0x3FB4EE50}, {0x3EB51B9C}}}};

//
// Computer sqrtf optimized for the PowerPC, lovingly based on
// Computation of elementary functions on the IBM RISC System/6000 processor
// by P. W. Markstein.
//
// Newton-Raphson docs
//
// http://www.sosmath.com/calculus/diff/der07/der07.html
//

float BURGER_API Burger::Sqrt(float fInput) BURGER_NOEXCEPT
{

    // 0.5f + epsilon
    const float g_fHalfPlusEpsilon = 0.50000006f;

    // Only calculate if non-zero positive
    // Also will not branch if NaN

    if (fInput > 0.0f) {

        // Not infinity?
        if (fInput != g_fInf) {

            // Get the binary form of the sqrt
            // fInputMem is used to keep fInput from being mapped to memory
            Word32ToFloat fInputMem;
            fInputMem = fInput;
            uint32_t uInput = fInputMem.GetWord();

            // Set the denormalization fixup to "inert" (Mul by 1 means nop)
            float fNormalFix = 1.0f;

            // Denormalized number?
            if (!(uInput & 0x7F800000U)) {

                // Convert to a normal number, convert and then convert
                // back to denormal on exit
                fInput = fInput * 281474976710656.0f;

                // Sync uInput to the normalized value
                fInputMem = fInput;
                uInput = fInputMem.GetWord();

                // Set the fixup value
                fNormalFix = 5.9604644775390625e-8f;
            }

            // Normalize the exponent so the square root is calculated in
            // the range of 0.0f to 3.99999f

            uint32_t uInputNormalized = (uInput & 0x00FFFFFFU) | 0x3F000000U;

            // Convert it back into a float
            float fInputNormalized =
                reinterpret_cast<const float*>(&uInputNormalized)[0];

            // Look up in a table to generate the initial guess
            // 23 bits mantissa, 1 for even/odd, 8 bits for 256 entries
            const float* pGuess = &g_PPCSqrtGuess[0][0][0].f +
                ((uInput >> ((23 - 8) - 1)) & 0x3FEU);

            // Get the initial guess
            float fGuess = pGuess[0];

            // Get the reciprocal of guess * 2 (1/(guess*2))
            float fGuessReciprocal = pGuess[1];

            // There needs to be three iterations of Newton-Raphson
            // to get enough bits of accuracy to get the correct
            // answer

            // Square the guess and test against the normal number.
            float fDelta = -__builtin_fmaf(fGuess, fGuess, -fInputNormalized);

            // Get the base exponent for the resulting sqrt (For final fixup)
            fInputMem.SetWord(((uInput + 0x40000000U) >> 1U) & 0x7F800000);

            // Pre-calc the reciprocal * 2
            float fGuessReciprocal2 = fGuessReciprocal + fGuessReciprocal;

            // We are 16 bits accurate here, not quite there yet
            fGuess = __builtin_fmaf(fGuessReciprocal, fDelta, fGuess);

            // Get new sqrt divided by the initial guess (Subtract 0.5 + epsilon
            // to ensure rounding
            float f2ndDelta =
                -__builtin_fmaf(fGuessReciprocal, fGuess, -g_fHalfPlusEpsilon);

            // Get the input value's adjusted exponent
            float fNextExponent = fInputMem;

            // 2nd iteration, get the delta
            fDelta = -__builtin_fmaf(fGuess, fGuess, -fInputNormalized);

            fGuessReciprocal =
                __builtin_fmaf(f2ndDelta, fGuessReciprocal2, fGuessReciprocal);

            float fNextNormalized = fInputNormalized * fNextExponent;

            // New sqrt, accurate to 23 bits, but we need to fix the last bits
            // due to rounding
            fGuess = __builtin_fmaf(fGuessReciprocal, fDelta, fGuess);

            // Pre-calc the reciprocal * 2 again
            fGuessReciprocal2 = fGuessReciprocal + fGuessReciprocal;

            // Apply the exponent to the normalized sqrt
            float fFixedGuess = fGuess * fNextExponent;

            // Last iteration to get the final bits of precision
            f2ndDelta =
                -__builtin_fmaf(fGuessReciprocal, fGuess, -g_fHalfPlusEpsilon);
            float fFinalDelta =
                -__builtin_fmaf(fFixedGuess, fGuess, -fNextNormalized);
            fGuessReciprocal =
                __builtin_fmaf(f2ndDelta, fGuessReciprocal2, fGuessReciprocal);

            // Multiply the final bits and add in the sqrt to apply the rounding
            fInput =
                __builtin_fmaf(fGuessReciprocal, fFinalDelta, fFixedGuess) *
                fNormalFix;
        }

        // Was it negative?
    } else if (fInput < 0.0f) {
        // You are a doofus. Return NaN
        fInput = g_fQNan;
    }
    // Return the result
    return fInput;
}

#elif defined(BURGER_METROWERKS) && defined(BURGER_X86)
#elif defined(BURGER_XBOX360) && !defined(DOXYGEN)
#elif defined(BURGER_PS3) || defined(BURGER_PS4) || defined(BURGER_VITA)
#elif defined(BURGER_MSVC) && (defined(BURGER_INTEL) || defined(BURGER_ARM))
#elif defined(BURGER_MACOSX) || defined(BURGER_IOS) || defined(BURGER_LINUX)
#else

float BURGER_API Burger::Sqrt(float fInput) BURGER_NOEXCEPT
{
    // Convert to binary
    Word32ToFloat Convert;
    Convert = fInput;
    uint32_t uInput = Convert.GetWord();

    // Inf or NaN?
    if ((uInput & 0x7f800000U) == 0x7f800000U) {

        // Cute trick to return this truth table
        // NaN -> Nan
        // +Inf -> +Inf
        // -Inf -> NaN
        fInput = fInput * fInput + fInput;

        // 0 or negative?
    } else if (static_cast<int32_t>(uInput) <= 0) {
        if (uInput & 0x7FFFFFFFU) {
            // Negative number, eh?
            fInput = g_fQNan;
        }
    } else {

        // It's a positive non zero number, perform the hard work

        // Extract the exponent
        int32_t iExponent = static_cast<int32_t>(uInput >> 23U);

        // Subnormal?
        if (!iExponent) {

            // Force to a normal
            iExponent = 1;
            if (!(uInput & 0x00800000U)) {
                // Keep shifting until it's normalized
                do {
                    uInput <<= 1U;
                    --iExponent;
                } while (!(uInput & 0x00800000U));
            }
        }

        // The number is normalized at this point

        // Convert the exponent to the signed value
        iExponent -= 127;

        // Convert the mantissa into a 24 bit integer
        uInput = (uInput & 0x007FFFFFU) | 0x00800000U;

        // Convert to even exponent value
        if (iExponent & 1U) {
            uInput += uInput;
        }

        // Div the exponent by 2 for the sqrt of the exponent
        iExponent >>= 1U;

        // Perform an integer sqrt()
        uInput += uInput;

        // Here is where the fun begins

        uint32_t uResult = 0;
        uint32_t uRemainder = 0;
        uint32_t uMask = 0x01000000U;
        do {
            uint32_t uToTest = uMask + uRemainder;
            if (uInput >= uToTest) {
                uRemainder = uToTest + uMask;
                uInput -= uToTest;
                uResult += uMask;
            }
            uInput += uInput;
            uMask >>= 1U;
        } while (uMask);

        // Was there any remainder?
        if (uInput) {
            // Need to round up, but use the rounding direction
            // of the generic FPU architecture

            const float fSmall = 1.0e-30f;
            float fTest = g_fOne - fSmall;
            if (fTest >= g_fOne) {
                fTest = g_fOne + fSmall;
                if (fTest > g_fOne) {
                    uResult += 2U;
                } else {
                    // Round it!
                    uResult += (uResult & 1U);
                }
            }
        }

        // Insert the new mantissa into the resulting float
        uInput = (uResult >> 1U) + 0x3F000000U;

        // Restore the exponent (Adjusted)

        Convert.SetWord(uInput + (iExponent << 23));
        fInput = Convert;
    }
    return fInput;
}
#endif

/*! ************************************

    \fn Burger::Sqrt(double dInput);
    \brief Get the square root value of a double

    Return the square root of the floating point input.
    This is usually done by inline assembly.

    \param dInput A valid double precision floating point number.
    \return The square root of the input.
    \sa Sqrt(float), Sqrt(uint32_t), and Sqrt(Fixed32)

***************************************/

#if defined(BURGER_WATCOM) || defined(BURGER_INTEL_COMPILER)
#elif defined(BURGER_METROWERKS) && defined(BURGER_PPC)

//
// Computer sqrt optimized for the PowerPC, lovingly based on
// Computation of elementary functions on the IBM RISC System/6000 processor
// by P. W. Markstein.
//
// Newton-Raphson docs
//
// http://www.sosmath.com/calculus/diff/der07/der07.html
//

double BURGER_API Burger::Sqrt(double dInput) BURGER_NOEXCEPT
{
    // 0.5 + epsilon
    const double g_dHalfPlusEpsilon = 0.5000000000000001;

    // Only calculate if non-zero positive
    // Also will not branch if NaN

    if (dInput > 0.0) {

        // Not infinity?
        if (dInput != g_dInf) {

            // Get the binary form of the sqrt
            // fInputMem is used to keep fInput from being mapped to memory
            Word64ToDouble dInputMem;
            dInputMem = dInput;
            uint32_t uInputHi = dInputMem.GetHiWord();

            // Set the denormalization fixup to "inert" (Mul by 1 means nop)
            double dNormalFix = 1.0;

            // Denormalized number?
            if (!(uInputHi & 0x7FF00000U)) {

                // Convert to a normal number, convert and then convert
                // back to denormal on exit
                dInput = dInput * 3.245185536584267269e+32;

                // Sync uInput to the normalized value
                dInputMem = dInput;
                uInputHi = dInputMem.GetHiWord();

                // Set the fixup value
                dNormalFix = 5.551115123125782702e-17;
            }

            // Normalize the exponent so the square root is calculated in
            // the range of 0.0f to 3.99999f

            uint32_t uInputNormalized = (uInputHi & 0x001FFFFFU) | 0x3FE00000U;

            // Convert it back into a double (Only the upper 32 bits are
            // modified)
            dInputMem.SetHiWord(uInputNormalized);
            double dInputNormalized = dInputMem;

            // Look up in a table to generate the initial guess
            // 52 bits mantissa, 32 bits since it's the high word, 1 for
            // even/odd, 8 bits for 256 entries
            const float* pGuess = &g_PPCSqrtGuess[0][0][0].f +
                ((uInputHi >> (((52 - 32) - 8) - 1)) & 0x3FEU);

            // Get the initial guess
            double dGuess = pGuess[0];

            // Get the reciprocal of guess * 2 (1/(guess*2))
            double dGuessReciprocal = pGuess[1];

            // There needs to be three iterations of Newton-Raphson
            // to get enough bits of accuracy to get the correct
            // answer

            // Square the guess and test against the normal number.
            double dDelta = -__builtin_fma(dGuess, dGuess, -dInputNormalized);

            // Get the base exponent for the resulting sqrt (For final fixup)
            uint32_t uNextExponent =
                ((uInputHi + 0x40000000U) >> 1U) & 0x7FF00000U;

            // Pre-calc the reciprocal * 2
            double dGuessReciprocal2 = dGuessReciprocal + dGuessReciprocal;

            // We are 16 bits accurate here, not quite there yet
            dGuess = __builtin_fma(dGuessReciprocal, dDelta, dGuess);

            // Get new sqrt divided by the initial guess (Subtract 0.5 + epsilon
            // to ensure rounding
            double d2ndDelta =
                -__builtin_fma(dGuessReciprocal, dGuess, -g_dHalfPlusEpsilon);

            // Get the input value's adjusted exponent
            dInputMem.SetLowWord(0);
            dInputMem.SetHiWord(uNextExponent);
            double dNextExponent = dInputMem;

            // 2nd iteration, get the delta
            dDelta = -__builtin_fma(dGuess, dGuess, -dInputNormalized);
            dGuessReciprocal =
                __builtin_fma(d2ndDelta, dGuessReciprocal2, dGuessReciprocal);

            double dNextNormalized = dInputNormalized * dNextExponent;

            // New sqrt, accurate to 23 bits, but we need to fix the last bits
            // due to rounding
            dGuess = __builtin_fma(dGuessReciprocal, dDelta, dGuess);

            // Pre-calc the reciprocal * 2 again
            dGuessReciprocal2 = dGuessReciprocal + dGuessReciprocal;

            // One more pass for DOUBLE precision
            d2ndDelta =
                -__builtin_fma(dGuessReciprocal, dGuess, -g_dHalfPlusEpsilon);
            dDelta = -__builtin_fma(dGuess, dGuess, -dInputNormalized);
            dGuessReciprocal =
                __builtin_fma(d2ndDelta, dGuessReciprocal2, dGuessReciprocal);
            dGuess = __builtin_fma(dGuessReciprocal, dDelta, dGuess);
            dGuessReciprocal2 = dGuessReciprocal + dGuessReciprocal;

            // Apply the exponent to the normalized sqrt
            double dFixedGuess = dGuess * dNextExponent;

            // Last iteration to get the final bits of precision
            d2ndDelta =
                -__builtin_fma(dGuessReciprocal, dGuess, -g_dHalfPlusEpsilon);
            double dFinalDelta =
                -__builtin_fma(dFixedGuess, dGuess, -dNextNormalized);
            dGuessReciprocal =
                __builtin_fma(d2ndDelta, dGuessReciprocal2, dGuessReciprocal);

            // Multiply the final bits and add in the sqrt to apply the rounding
            dInput = __builtin_fma(dGuessReciprocal, dFinalDelta, dFixedGuess) *
                dNormalFix;
        }
        // Was it negative?
    } else if (dInput < 0.0) {
        // You are a doofus. Return NaN
        dInput = g_dQNan;
    }
    // Return the result
    return dInput;
}

#elif defined(BURGER_METROWERKS) && defined(BURGER_X86)
#elif defined(BURGER_XBOX360) && !defined(DOXYGEN)
#elif defined(BURGER_PS3) || defined(BURGER_PS4) || defined(BURGER_VITA)
#elif defined(BURGER_MSVC) && (defined(BURGER_INTEL) || defined(BURGER_ARM))
#elif defined(BURGER_MACOSX) || defined(BURGER_IOS) || defined(BURGER_LINUX)
#else

double BURGER_API Burger::Sqrt(double dInput) BURGER_NOEXCEPT
{
    // Convert to binary
    Word64ToDouble Converter;
    Converter = dInput;
    uint32_t uInputLow = Converter.GetLowWord();
    uint32_t uInputHigh = Converter.GetHiWord();

    // Inf or NaN?
    if ((uInputHigh & 0x7ff00000U) == 0x7ff00000U) {
        // Cute trick to return this truth table
        // NaN -> Nan
        // +Inf -> +Inf
        // -Inf -> NaN
        dInput = dInput * dInput + dInput;

        // 0 or negative?
    } else if (static_cast<int64_t>(Converter.GetWord()) <= 0) {
        if ((uInputHigh & 0x7FFFFFFFU) | uInputLow) {
            // Negative number, eh?
            dInput = g_dNan;
        }
    } else {

        // It's a positive non zero number, perform the hard work

        // Extract the exponent
        int32_t iExponent = static_cast<int32_t>(uInputHigh >> 20U);

        // Subnormal?
        if (!iExponent) {

            // Force to a normal
            iExponent = 1;

            // Perform normalization with full 64 bits

            if (!uInputHigh) {
                do {
                    // Update the exponent for 21 bits
                    iExponent -= 21;
                    // 64 bit shift
                    uInputHigh |= uInputLow >> (static_cast<uint_t>(32 - 21));
                    uInputLow <<= 21U;
                } while (!uInputHigh);
            }

            // uInputHigh is nonzero, so the normal bit will be found soon

            uint_t uShiftCount = 0;
            if (!(uInputHigh & 0x00100000U)) {
                // Keep shifting until it's normalized
                do {
                    uInputHigh <<= 1U;
                    ++uShiftCount;
                } while (!(uInputHigh & 0x00100000U));
            }
            // Update the remaining bits
            iExponent -= uShiftCount;
            uInputHigh |= (uInputLow >> (32 - uShiftCount));
            uInputLow <<= uShiftCount;
        }

        // The number is normalized at this point

        // Convert the exponent to the signed value
        iExponent -= 1023;

        // Convert the mantissa into a 56 bit integer
        uInputHigh = (uInputHigh & 0x000FFFFFU) | 0x00100000U;

        // Convert to even exponent value
        if (iExponent & 1) {
            // 64 bit mul by 2
            uInputHigh += uInputHigh + (uInputLow >> 31U);
            uInputLow += uInputLow;
        }

        // Div the exponent by 2 for the sqrt of the exponent
        iExponent >>= 1U;

        // Perform an integer sqrt()
        // 64 bit mul by 2
        uInputHigh += uInputHigh + (uInputLow >> 31U);
        uInputLow += uInputLow;

        // Here is where the fun begins

        uint32_t uResult = 0;
        uint32_t uRemainder = 0;
        uint32_t uMask = 0x00200000U;
        do {
            uint32_t uToTest = uMask + uRemainder;
            if (uInputHigh >= uToTest) {
                uRemainder = uToTest + uMask;
                uInputHigh -= uToTest;
                uResult += uMask;
            }
            // 64 bit shift
            uInputHigh += uInputHigh + (uInputLow >> 31U);
            uInputLow += uInputLow;
            uMask >>= 1U;
        } while (uMask);

        uint32_t uResult2 = 0;
        uint32_t uRemainder2 = 0;
        uMask = 0x80000000U;
        do {
            uint32_t uToTest2 = uRemainder2 + uMask;
            uint32_t uToTest = uRemainder;
            if ((uToTest < uInputHigh) ||
                ((uToTest == uInputHigh) && (uToTest2 <= uInputLow))) {
                uRemainder2 = uToTest2 + uMask;
                if ((uToTest2 & 0x80000000U) &&
                    (!(uRemainder2 & 0x80000000U))) {
                    uRemainder += 1;
                }
                // Perform the addition and subtraction at 64 bit accuracy
                uInputHigh -= uToTest;
                if (uInputLow < uToTest2) {
                    uInputHigh -= 1;
                }
                uInputLow -= uToTest2;
                uResult2 += uMask;
            }
            // 64 bit mul by 2
            uInputHigh += uInputHigh + (uInputLow >> 31U);
            uInputLow += uInputLow;
            uMask >>= 1U;
        } while (uMask);

        // Was there any remainder?
        if ((uInputHigh | uInputLow) != 0) {
            // Need to round up, but use the rounding direction
            // of the generic FPU architecture

            const double dSmall = 1.0e-300;
            double dTest = g_dOne - dSmall;
            if (dTest >= g_dOne) {
                dTest = g_dOne + dSmall;
                if (uResult2 == 0xFFFFFFFFU) {
                    uResult2 = 0;
                    uResult += 1;
                } else if (dTest > g_dOne) {
                    if (uResult2 == 0xFFFFFFFEU) {
                        uResult += 1;
                    }
                    uResult2 += 2;
                } else {
                    uResult2 += (uResult2 & 1);
                }
            }
        }

        // Insert the new mantissa into the resulting double
        uInputHigh = (uResult >> 1U) + 0x3fe00000;
        uInputLow = uResult2 >> 1U;
        if (uResult & 1) {
            uInputLow |= 0x80000000U;
        }

        // Restore the exponent (Adjusted)
        Converter.SetLowWord(uInputLow);
        Converter.SetHiWord(uInputHigh + (iExponent << 20));
        dInput = Converter;
    }
    return dInput;
}

#endif

/*! ************************************

    \fn Burger::IntToFloat(int32_t iInput)
    \brief 32 bit integer to floating point conversion

    \param iInput 32 bit integer to convert
    \return Floating point representation of the integer input

    \sa IntToFloat(const int32_t *), IntToFloat(float *,int32_t) or
        IntToFloat(float*,const int32_t *)

***************************************/

/*! ************************************

    \fn Burger::IntToFloat(const int32_t *pInput)
    \brief 32 bit integer to floating point conversion

    \param pInput Pointer to a 32 bit integer to convert
    \return Floating point representation of the integer input

    \sa IntToFloat(int32_t), IntToFloat(float *,int32_t) or
        IntToFloat(float*,const int32_t *)

***************************************/

/*! ************************************

    \fn Burger::IntToFloat(float *pOutput,int32_t iInput)
    \brief 32 bit integer to floating point conversion

    \param iInput 32 bit integer to convert
    \param pOutput Pointer to a 32 bit float to receive the converted integer

    \sa IntToFloat(int32_t), IntToFloat(const int32_t *) or
        IntToFloat(float *,const int32_t *)

***************************************/

/*! ************************************

    \fn Burger::IntToFloat(float *pOutput,const int32_t *pInput)
    \brief 32 bit integer to floating point conversion

    \note This function is fast on the Xbox 360 due to direct use of VMX128
        instructions

    \param pInput Pointer to a 32 bit integer to convert
    \param pOutput Pointer to a 32 bit float to receive the converted integer

    \sa IntToFloat(int32_t), IntToFloat(const int32_t *) or
        IntToFloat(float*,int32_t)

***************************************/

/*! ************************************

    \fn Burger::FixedToFloat(Fixed32 iInput)
    \brief 32 bit 16.16 fixed point integer to floating point conversion

    \param iInput 32 bit 16.16 fixed point integer to convert
    \return Floating point representation of the integer input

    \sa FixedToFloat(const Fixed32 *), FixedToFloat(float *,Fixed32) or
        FixedToFloat(float *,const Fixed32 *)

***************************************/

/*! ************************************

    \fn Burger::FixedToFloat(const Fixed32 *pInput)
    \brief 32 bit 16.16 fixed point integer to floating point conversion

    \param pInput Pointer to a 32 bit 16.16 fixed point integer to convert
    \return Floating point representation of the integer input

    \sa FixedToFloat(Fixed32), FixedToFloat(float *,Fixed32) or
        FixedToFloat(float *,const Fixed32 *)

***************************************/

/*! ************************************

    \fn Burger::FixedToFloat(float *pOutput,Fixed32 iInput)
    \brief 32 bit 16.16 fixed point integer to floating point conversion

    \param iInput 32 bit 16.16 fixed point integer to convert
    \param pOutput Pointer to a 32 bit float to receive the converted integer

    \sa FixedToFloat(Fixed32), FixedToFloat(const Fixed32 *) or
        FixedToFloat(float *,const Fixed32 *)

***************************************/

/*! ************************************

    \fn Burger::FixedToFloat(float *pOutput,const Fixed32 *pInput)
    \brief 32 bit 16.16 fixed point integer to floating point conversion

    \note This function is fast on the Xbox 360 due to direct use of VMX128
        instructions

    \param pInput Pointer to a 32 bit 16.16 fixed point integer to convert
    \param pOutput Pointer to a 32 bit float to receive the converted integer

    \sa FixedToFloat(Fixed32), FixedToFloat(const Fixed32 *) or
        FixedToFloat(float *,Fixed32)

***************************************/

/*! ************************************

    \fn Burger::Interpolate(float fFrom,float fTo,float fFactor)
    \brief Interpolate between two floating point values

    Using a factor that's clamped from 0.0f to 1.0f, return the value between
    fFrom and fTo inclusive along a linear scale where 0.0f returns fFrom
    and 1.0f returns fTo. Order of the fFrom and fTo values are unimportant.

    \note NaN and Inf as input will yield undefined behavior

    \param fFrom Value to return if the interpolation factor is 0.0f
    \param fTo Value to return if the interpolation factor is 1.0f
    \param fFactor Interpolation between 0.0f and 1.0f inclusive
    \return Value between fFrom and fTo

    \sa Interpolate(double,double,double)

***************************************/

/*! ************************************

    \fn Burger::Interpolate(double dFrom,double dTo,double dFactor)
    \brief Interpolate between two floating point values

    Using a factor that's clamped from 0.0 to 1.0, return the value between
    dFrom and dTo inclusive along a linear scale where 0.0 returns dFrom and 1.0
    returns dTo. Order of the dFrom and dTo values are unimportant.

    \note NaN and Inf as input will yield undefined behavior

    \param dFrom Value to return if the interpolation factor is 0.0
    \param dTo Value to return if the interpolation factor is 1.0
    \param dFactor Interpolation between 0.0 and 1.0 inclusive
    \return Value between dFrom and dTo

    \sa Interpolate(float,float,float)

***************************************/

/*! ************************************

    \fn float Burger::Square(float fInput)
    \brief Return the square of the input

    Square the number and return the result.

    \param fInput Value to square
    \return The square of the input

    \sa Square(double)

***************************************/

/*! ************************************

    \fn double Burger::Square(double dInput)
    \brief Return the square of the input

    Square the number and return the result.

    \param dInput Value to square
    \return The square of the input

    \sa Square(float)

***************************************/

/*! ************************************

    \fn float Burger::Sign(float fInput)
    \brief Return sign constant

    If the input is less than zero, return negative one, if the input is greater
    than zero, return one, otherwise return zero.

    \param fInput Value to test
    \return The sign constant of 1, -1 or 0

    \sa Sign(double)

***************************************/

/*! ************************************

    \fn double Burger::Sign(double dInput)
    \brief Return sign constant

    If the input is less than zero, return negative one, if the input is greater
    than zero, return one, otherwise return zero.

    \param dInput Value to test
    \return The sign constant of 1, -1 or 0

    \sa Sign(float)

***************************************/

/*! ************************************

    \fn float Burger::Clamp(float fInput,float fMin,float fMax)
    \brief Clamp the input between a bounds

    If the input value is less than the minimum, return the minimum or if the
    input value is greater than the maximum, return the maximum, otherwise
    return the input value. No checking is performed to determine if the minimum
    is less than the maximum.

    \param fInput First value to test
    \param fMin Minimum allowed value
    \param fMax Maximum allowed value
    \return The value clamped between the bounds

    \sa Clamp(double,double,double)

***************************************/

/*! ************************************

    \fn double Burger::Clamp(double dInput,double dMin,double dMax)
    \brief Clamp the input between a bounds

    If the input value is less than the minimum, return the minimum or if the
    input value is greater than the maximum, return the maximum, otherwise
    return the input value. No checking is performed to determine if the minimum
    is less than the maximum.

    \param dInput First value to test
    \param dMin Minimum allowed value
    \param dMax Maximum allowed value
    \return The value clamped between the bounds

    \sa Clamp(float,float,float)

***************************************/

/*! ************************************

    \brief Return \ref TRUE if the value is a NaN

    Test for QNan and SNan and return \ref TRUE if so.

    \param fInput A 32 bit floating point number.
    \return \ref TRUE if Nan, \ref FALSE if not.
    \sa IsNan(double), IsInf(float), IsFinite(float), IsNormal(float) and
        SignBit(float)

***************************************/

#if defined(BURGER_XBOX360) || \
    (defined(BURGER_PPC) && defined(BURGER_METROWERKS))

BURGER_DECLSPECNAKED uint_t BURGER_API Burger::IsNan(
    float /* fInput */) BURGER_NOEXCEPT
{
    // clang-format off
#if defined(BURGER_XBOX360)
    BURGER_ASM
    {
#endif
        fcmpu cr0, fp1, fp1 // Perform a NaN test
        mfcr r0             // Move cr0 into r0
        extrwi r3, r0, 1, 3 // (Flags>>28)&1 Grab the "Unordered" flag
        blr                 // Exit
#if defined(BURGER_XBOX360)
    }
#endif
    // clang-format on
}

#elif defined(BURGER_PPC) && defined(BURGER_MACOSX)

__asm__(
    "	.align	2,0\n"
    "	.globl	__ZN6Burger5IsNanEf\n"
    "__ZN6Burger5IsNanEf:\n"
    "	fcmpu	cr0,f1,f1\n" // Perform a NaN test
    "	mfcr	r0\n"        // Move cr0 into r0
    "	extrwi	r3,r0,1,3\n" // (Flags>>28)&1 Grab the "Unordered" flag
    "	blr\n"               // Exit
);

#else

uint_t BURGER_API Burger::IsNan(float fInput) BURGER_NOEXCEPT
{
    uint32_t uInput =
        static_cast<const uint32_t*>(static_cast<const void*>(&fInput))[0];
    // Kill off the high bit
    uInput &= 0x7FFFFFFF;
    // Set the high bit if 0x7F800001-0x7FFFFFFF
    uInput = 0x7F800000 - uInput;
    // Return TRUE or FALSE depending on the test
    return (uInput >> 31U);
}

#endif

/*! ************************************

    \brief Return \ref TRUE if the value is a NaN

    Test for QNan and SNan and return \ref TRUE if so.

    \param dInput A 64 bit floating point number.
    \return \ref TRUE if Nan, \ref FALSE if not.
    \sa IsNan(float), IsInf(double), IsFinite(double), IsNormal(double) and
        SignBit(double)

***************************************/

#if defined(BURGER_XBOX360) || \
    (defined(BURGER_PPC) && defined(BURGER_METROWERKS))

BURGER_DECLSPECNAKED uint_t BURGER_API Burger::IsNan(
    double /* dInput */) BURGER_NOEXCEPT
{
    // clang-format off
#if defined(BURGER_XBOX360)
    BURGER_ASM
    {
#endif
        fcmpu cr0, fp1, fp1 // Perform a NaN test
        mfcr r0             // Move cr0 into r0
        extrwi r3, r0, 1, 3 // (Flags>>28)&1 Grab the "Unordered" flag
        blr                 // Exit
#if defined(BURGER_XBOX360)
    }
#endif
    // clang-format on
}

#elif defined(BURGER_PPC) && defined(BURGER_MACOSX)

__asm__(
    "	.align	2,0\n"
    "	.globl	__ZN6Burger5IsNanEd\n"
    "__ZN6Burger5IsNanEd:\n"
    "	fcmpu	cr0,f1,f1\n" // Perform a NaN test
    "	mfcr	r0\n"        // Move cr0 into r0
    "	extrwi	r3,r0,1,3\n" // (Flags>>28)&1 Grab the "Unordered" flag
    "	blr\n"               // Exit
);

#else

uint_t BURGER_API Burger::IsNan(double dInput) BURGER_NOEXCEPT
{
#if defined(BURGER_64BITCPU)
    uint64_t uInput =
        static_cast<const uint64_t*>(static_cast<const void*>(&dInput))[0];
    // Kill off the high bit
    uInput &= 0x7FFFFFFFFFFFFFFFULL;
    // Set the high bit if 0x7F800001-0x7FFFFFFF
    uInput = 0x7FF0000000000000ULL - uInput;
    // Return TRUE or FALSE depending on the test
    return static_cast<uint_t>(uInput >> 63U);
#else
    return (static_cast<const uint64_t*>(static_cast<const void*>(&dInput))[0]
               << 1U) > (0x7ff0000000000000ULL << 1U);
#endif
}
#endif

/*! ************************************

    \brief Return \ref TRUE if the value is infinity

    Test for infinity and return \ref TRUE if so.

    \param fInput A 32 bit floating point number.
    \return \ref TRUE if Infinity, \ref FALSE if not.
    \sa IsInf(double), IsNan(float), IsFinite(float), IsNormal(float) and
        SignBit(float)

***************************************/

#if defined(BURGER_XBOX360)

uint_t BURGER_API Burger::IsInf(float fInput) BURGER_NOEXCEPT
{
    return (fabs(fInput) == g_fInf);
}

#elif defined(BURGER_PPC) && defined(BURGER_METROWERKS)

BURGER_ASM uint_t BURGER_API Burger::IsInf(float fInput) BURGER_NOEXCEPT
{
    // clang-format off
    lwz r3, g_fInf(RTOC) // Get the address of g_fInf
    fabs fp1, fp1       // Strip the sign
    lfs fp0, 0(r3)      // Fetch fInf
    fcmpu cr0, fp1, fp0 // Compare for equality
    mfcr r0             // Move cr0 into r0
    extrwi r3, r0, 1, 2 // (Flags>>29)&1 Grab the "Equal" flag
    blr                 // Exit
    // clang-format on
}

#elif defined(BURGER_PPC) && defined(BURGER_MACOSX)

__asm__(
    "	.align	2,0\n"
    "	.globl	__ZN6Burger5IsInfEf\n"
    "__ZN6Burger5IsInfEf:\n"
    "	lis		r3,ha16(__ZN6Burger6g_fInfE)\n"     // Get the address of g_fInf
    "	fabs	f1,f1\n"                            // Strip the sign
    "	lfs		f0,lo16(__ZN6Burger6g_fInfE)(r3)\n" // Fetch fInf
    "	fcmpu	cr0,f1,f0\n"                        // Compare for equality
    "	mfcr	r0\n"                               // Move cr0 into r0
    "	extrwi	r3,r0,1,2\n" // (Flags>>29)&1 Grab the "Equal" flag
    "	blr\n"               // Exit
);

#else

uint_t BURGER_API Burger::IsInf(float fInput) BURGER_NOEXCEPT
{
    const uint32_t uInput =
        static_cast<const uint32_t*>(static_cast<const void*>(&fInput))[0];
    const uint32_t uTemp = uInput & 0x7FFFFFFF;
    return uTemp == 0x7F800000;
}

#endif

/*! ************************************

    \brief Return \ref TRUE if the value is infinity

    Test for infinity and return \ref TRUE if so.

    \param dInput A 64 bit floating point number.
    \return \ref TRUE if Infinity, \ref FALSE if not.
    \sa IsInf(float), IsNan(double), IsFinite(double), IsNormal(double) and
        SignBit(double)

***************************************/

#if defined(BURGER_XBOX360)

uint_t BURGER_API Burger::IsInf(double dInput) BURGER_NOEXCEPT
{
    return (fabs(dInput) == g_dInf);
}

#elif defined(BURGER_PPC) && defined(BURGER_METROWERKS)

BURGER_ASM uint_t BURGER_API Burger::IsInf(double dInput) BURGER_NOEXCEPT
{
    // clang-format off
    lwz r3, g_dInf(RTOC)    // Get the address of g_dInf
    fabs fp1, fp1           // Strip the sign
    lfd fp0, 0(r3)          // Fetch dInf
    fcmpu cr0, fp1, fp0     // Compare for equality
    mfcr r0                 // Move cr0 into r0
    extrwi r3, r0, 1, 2     // (Flags>>29)&1 Grab the "Equal" flag
    blr                     // Exit
    // clang-format on
}

#elif defined(BURGER_MACOSX) && defined(BURGER_PPC)

__asm__(
    "	.align	2,0\n"
    "	.globl	__ZN6Burger5IsInfEd\n"
    "__ZN6Burger5IsInfEd:\n"
    "	lis		r3,ha16(__ZN6Burger6g_dInfE)\n"     // Get the address of g_fInf
    "	fabs	f1,f1\n"                            // Strip the sign
    "	lfd		f0,lo16(__ZN6Burger6g_dInfE)(r3)\n" // Fetch fInf
    "	fcmpu	cr0,f1,f0\n"                        // Compare for equality
    "	mfcr	r0\n"                               // Move cr0 into r0
    "	extrwi	r3,r0,1,2\n" // (Flags>>29)&1 Grab the "Equal" flag
    "	blr\n"               // Exit
);

#else

uint_t BURGER_API Burger::IsInf(double dInput) BURGER_NOEXCEPT
{
    return (static_cast<const uint64_t*>(static_cast<const void*>(&dInput))[0]
               << 1U) == (0x7ff0000000000000ULL << 1U);
}

#endif

/*! ************************************

    \brief Return \ref TRUE if the value is finite

    Test for a finite number (Not Nan, Inf) and return \ref TRUE if so.

    \param fInput A 32 bit floating point number.
    \return \ref TRUE if finite, \ref FALSE if not.
    \sa IsFinite(double), IsNan(float), IsInf(float), IsNormal(float) and
        SignBit(float)

***************************************/

#if defined(BURGER_XBOX360)

BURGER_DECLSPECNAKED uint_t BURGER_API Burger::IsFinite(
    float /* fInput */) BURGER_NOEXCEPT
{
    // clang-format off
    BURGER_ASM
    {
        lau r3, g_fInf      // Get the address of g_fInf
        fabs fp1, fp1       // Strip the sign
        lfs fp0, g_fInf(r3) // Fetch fInf
        fcmpu cr0, fp1, fp0 // Compare if less than infinity
        mfcr r0             // Move cr0 into r0
        extrwi r3, r0, 1, 0 // (Flags>>31)&1 Grab the "Less than" flag
        blr                 // Exit
    }
    // clang-format on
}

#elif defined(BURGER_PPC) && defined(BURGER_METROWERKS)

BURGER_ASM uint_t BURGER_API Burger::IsFinite(float fInput) BURGER_NOEXCEPT
{
    // clang-format off
    lwz r3, g_fInf(RTOC) // Get the address of g_fInf
    fabs fp1, fp1       // Strip the sign
    lfs fp0, 0(r3)      // Fetch fInf
    fcmpu cr0, fp1, fp0 // Compare if less than infinity
    mfcr r0             // Move cr0 into r0
    extrwi r3, r0, 1, 0 // (Flags>>31)&1 Grab the "Less than" flag
    blr                 // Exit
    // clang-format on
}

#elif defined(BURGER_MACOSX) && defined(BURGER_PPC)

__asm__(
    "	.align	2,0\n"
    "	.globl __ZN6Burger8IsFiniteEf\n"
    "__ZN6Burger8IsFiniteEf:\n"
    "	lis		r3,ha16(__ZN6Burger6g_fInfE)\n"     // Get the address of g_fInf
    "	fabs	f1,f1\n"                            // Strip the sign
    "	lfs		f0,lo16(__ZN6Burger6g_fInfE)(r3)\n" // Fetch fInf
    "	fcmpu	cr0,f1,f0\n" // Compare if less than infinity
    "	mfcr	r0\n"        // Move cr0 into r0
    "	extrwi	r3,r0,1,0\n" // (Flags>>31)&1 Grab the "Less than" flag
    "	blr\n"               // Exit
);

#else

uint_t BURGER_API Burger::IsFinite(float fInput) BURGER_NOEXCEPT
{
    uint32_t uInput =
        static_cast<const uint32_t*>(static_cast<const void*>(&fInput))[0];
    uint32_t uTemp = uInput & 0x7FFFFFFFU;
    uTemp = uTemp - 0x7F800000U;
    return (uTemp >> 31U);
}

#endif

/*! ************************************

    \brief Return \ref TRUE if the value is finite

    Test for a finite number (Not Nan, Inf) and return \ref TRUE if so.

    \param dInput A 64 bit floating point number.
    \return \ref TRUE if finite, \ref FALSE if not.
    \sa IsFinite(float), IsNan(double), IsInf(double), IsNormal(double) and
        SignBit(double)

***************************************/

#if defined(BURGER_XBOX360)

BURGER_DECLSPECNAKED uint_t BURGER_API Burger::IsFinite(
    double /* dInput */) BURGER_NOEXCEPT
{
    // clang-format off
    BURGER_ASM
    {
        lau r3, g_dInf      // Get the address of g_dInf
        fabs fp1, fp1       // Strip the sign
        lfd fp0, g_dInf(r3) // Fetch dInf
        fcmpu cr0, fp1, fp0 // Compare if less than infinity
        mfcr r0             // Move cr0 into r0
        extrwi r3, r0, 1, 0 // (Flags>>31)&1 Grab the "Less than" flag
        blr                 // Exit
    }
    // clang-format on
}

#elif defined(BURGER_PPC) && defined(BURGER_METROWERKS)

BURGER_ASM uint_t BURGER_API Burger::IsFinite(double dInput) BURGER_NOEXCEPT
{
    // clang-format off
    lwz r3, g_dInf(RTOC) // Get the address of g_dInf
    fabs fp1, fp1       // Strip the sign
    lfd fp0, 0(r3)      // Fetch dInf
    fcmpu cr0, fp1, fp0 // Compare if less than infinity
    mfcr r0             // Move cr0 into r0
    extrwi r3, r0, 1, 0 // (Flags>>31)&1 Grab the "Less than" flag
    blr                 // Exit
    // clang-format on
}

#elif defined(BURGER_MACOSX) && defined(BURGER_PPC)

__asm__(
    "	.align	2,0\n"
    "	.globl	__ZN6Burger8IsFiniteEd\n"
    "__ZN6Burger8IsFiniteEd:\n"
    "	lis		r3,ha16(__ZN6Burger6g_dInfE)\n"     // Get the address of g_fInf
    "	fabs	f1,f1\n"                            // Strip the sign
    "	lfd		f0,lo16(__ZN6Burger6g_dInfE)(r3)\n" // Fetch fInf
    "	fcmpu	cr0,f1,f0\n" // Compare if less than infinity
    "	mfcr	r0\n"        // Move cr0 into r0
    "	extrwi	r3,r0,1,0\n" // (Flags>>31)&1 Grab the "Less than" flag
    "	blr\n"               // Exit
);

#else

uint_t BURGER_API Burger::IsFinite(double dInput) BURGER_NOEXCEPT
{
    uint32_t uInput;
#if defined(BURGER_BIGENDIAN)
    uInput = static_cast<const uint32_t*>(static_cast<const void*>(&dInput))[0];
#else
    uInput = static_cast<const uint32_t*>(static_cast<const void*>(&dInput))[1];
#endif
    uint32_t uTemp = uInput & 0x7FFFFFFFU;
    uTemp = uTemp - 0x7FF00000U;
    return (uTemp >> 31U);
}
#endif

/*! ************************************

    \brief Return \ref TRUE if the value is normal

    Test for a finite normal number (Not Zero, NaN, Inf or denormalized) and
    return \ref TRUE if so.

    \param fInput A 32 bit floating point number.
    \return \ref TRUE if normal, \ref FALSE if not.
    \sa IsNormal(double), IsNan(float), IsInf(float), IsFinite(float) and
        SignBit(float)

***************************************/

#if defined(BURGER_PPC) && \
    (defined(BURGER_METROWERKS) || defined(BURGER_MACOSX))

uint_t BURGER_API Burger::IsNormal(float fInput) BURGER_NOEXCEPT
{
    // Force absolute
    double fInputNew = fabs(static_cast<double>(fInput));
    // If the number is subnormal, set to infinity (2*Max)
    fInputNew = __fsel(static_cast<double>(fInputNew - g_fMin), fInputNew,
        static_cast<double>(g_fMax + g_fMax));
    // If above max, set to Min, if it was set to max, return max
    // which returns true
    return __fsel(g_fMax - fInputNew, static_cast<double>(g_fMax),
               static_cast<double>(g_fMin)) == g_fMax;
}

#else

//
// Note: Xbox 360 and PS3 are better to take the Load/Hit/Store
// instead of the fcmpu stall.
//

uint_t BURGER_API Burger::IsNormal(float fInput) BURGER_NOEXCEPT
{
    uint32_t uInput =
        static_cast<const uint32_t*>(static_cast<const void*>(&fInput))[0];
    uint32_t uTemp = (uInput - 0x00800000U) & 0x7FFFFFFFU;
    uTemp = uTemp - (0x7F800000U - 0x00800000U);
    return (uTemp >> 31U);
}

#endif

/*! ************************************

    \brief Return \ref TRUE if the value is normal

    Test for a finite normal number (Not Zero, NaN, Inf or denormalized) and
    return \ref TRUE if so.

    \param dInput A 64 bit floating point number.
    \return \ref TRUE if normal, \ref FALSE if not.
    \sa IsNormal(float), IsNan(double), IsInf(double), IsFinite(double) and
        SignBit(double)

***************************************/

#if defined(BURGER_PPC) && \
    (defined(BURGER_METROWERKS) || defined(BURGER_MACOSX))

uint_t BURGER_API Burger::IsNormal(double dInput) BURGER_NOEXCEPT
{
    // Force absolute
    dInput = fabs(dInput);
    // If the number is subnormal, set to infinity (2*Max)
    dInput = __fsel(dInput - g_dMin, dInput, g_dMax + g_dMax);
    // If above max, set to Min, if it was set to max, return max
    // which returns true
    return __fsel(g_dMax - dInput, g_dMax, g_dMin) == g_dMax;
}

#else

// Note: Xbox 360 and PS3 are better to take the Load/Hit/Store
// instead of the fcmpu stall.

uint_t BURGER_API Burger::IsNormal(double dInput) BURGER_NOEXCEPT
{
    uint32_t uInput;
#if defined(BURGER_BIGENDIAN)
    uInput = static_cast<const uint32_t*>(static_cast<const void*>(&dInput))[0];
#else
    uInput = static_cast<const uint32_t*>(static_cast<const void*>(&dInput))[1];
#endif
    uint32_t uTemp = (uInput - 0x00100000U) & 0x7FFFFFFFU;
    uTemp = uTemp - (0x7FF00000U - 0x00100000U);
    return (uTemp >> 31U);
}

#endif

/*! ************************************

    \brief Return \ref TRUE if the value is negative

    Test for a negative number and return \ref TRUE if so.

    \param fInput A 32 bit floating point number.
    \return \ref TRUE if negative, \ref FALSE if not.
    \sa SignBit(double), IsNan(float), IsInf(float), IsFinite(float) and
        IsNormal(float)

***************************************/

uint_t BURGER_API Burger::SignBit(float fInput) BURGER_NOEXCEPT
{
    return static_cast<const uint32_t*>(static_cast<const void*>(&fInput))[0] >>
        31U;
}

/*! ************************************

    \brief Return \ref TRUE if the value is negative

    Test for a negative number and return \ref TRUE if so.

    \param dInput A 64 bit floating point number.
    \return \ref TRUE if negative, \ref FALSE if not.
    \sa SignBit(float), IsNan(double), IsInf(double), IsFinite(double) and
        IsNormal(double)

***************************************/

uint_t BURGER_API Burger::SignBit(double dInput) BURGER_NOEXCEPT
{
#if defined(BURGER_BIGENDIAN)
    return static_cast<const uint32_t*>(static_cast<const void*>(&dInput))[0] >>
        31;
#else
    return static_cast<const uint32_t*>(static_cast<const void*>(&dInput))[1] >>
        31;
#endif
}

/*! ************************************

    \brief Return \ref TRUE if the values are less than epsilon apart

    Test if two numbers are less than or equal apart than the epsilon
    of 1e-6f.

    \note The epsilon is not a constant, it's multiplied to scale with the
        magnitude

    \code

    // Example
    float fDelta = Abs(fInput1-fInput2);
    fInput1 = Abs(fInput1);
    fInput2 = Abs(fInput2);
    return (fInput1 > fInput2) ? (fDelta <= (fInput1 * 1e-6f)) : (fDelta <=
        (fInput2 * 1e-6f));

    \endcode

    \param fInput1 A 32 bit floating point number to test
    \param fInput2 Another 32 bit floating point number to test
    \return \ref TRUE if less than epsilon apart, \ref FALSE if not.

    \sa EqualWithEpsilon(float,float,float), EqualWithEpsilon(double,double)

***************************************/

uint_t BURGER_API Burger::EqualWithEpsilon(
    float fInput1, float fInput2) BURGER_NOEXCEPT
{
    const float fDelta = Abs(fInput1 - fInput2);
    fInput1 = Abs(fInput1);
    fInput2 = Abs(fInput2);
    return (fInput1 > fInput2) ? (fDelta <= (fInput1 * 1e-6f)) :
                                 (fDelta <= (fInput2 * 1e-6f));
}

/*! ************************************

    \brief Return \ref TRUE if the values are less than epsilon apart

    Test if two numbers are less than or equal apart than the epsilon
    of 1e-6.

    \note The epsilon is not a constant, it's multiplied to scale with the
        magnitude

    \code

    // Example
    double dDelta = Abs(dInput1-dInput2);
    dInput1 = Abs(dInput1);
    dInput2 = Abs(dInput2);
    return (dInput1 > dInput2) ? (dDelta <= (dInput1 * 1e-6)) : (dDelta <=
        (dInput2 * 1e-6));

    \endcode

    \param dInput1 A 64 bit floating point number to test
    \param dInput2 Another 64 bit floating point number to test
    \return \ref TRUE if less than epsilon apart, \ref FALSE if not.

    \sa EqualWithEpsilon(double,double,double), EqualWithEpsilon(float,float)

***************************************/

uint_t BURGER_API Burger::EqualWithEpsilon(
    double dInput1, double dInput2) BURGER_NOEXCEPT
{
    const double dDelta = Abs(dInput1 - dInput2);
    dInput1 = Abs(dInput1);
    dInput2 = Abs(dInput2);
    return (dInput1 > dInput2) ? (dDelta <= (dInput1 * 1e-6)) :
                                 (dDelta <= (dInput2 * 1e-6));
}

/*! ************************************

    \brief Return \ref TRUE if the values are less than epsilon apart

    Test if two numbers are less than or equal apart than the epsilon
    of 1e-6f.

    \note The epsilon is not a constant, it's multiplied to scale with the
        magnitude

    \code
    // Example
    float fDelta = Abs(fInput1-fInput2);
    fInput1 = Abs(fInput1);
    fInput2 = Abs(fInput2);
    return (fInput1 > fInput2) ? (fDelta <= (fInput1 * fEpsilon)) : (fDelta <=
        (fInput2 * fEpsilon));

    \endcode

    \param fInput1 A 32 bit floating point number to test
    \param fInput2 Another 32 bit floating point number to test
    \param fEpsilon Epsilon to test with
    \return \ref TRUE if less than epsilon apart, \ref FALSE if not.

    \sa EqualWithEpsilon(float,float), EqualWithEpsilon(double,double,double)

***************************************/

uint_t BURGER_API Burger::EqualWithEpsilon(
    float fInput1, float fInput2, float fEpsilon) BURGER_NOEXCEPT
{
    const float fDelta = Abs(fInput1 - fInput2);
    fInput1 = Abs(fInput1);
    fInput2 = Abs(fInput2);
    return (fInput1 > fInput2) ? (fDelta <= (fInput1 * fEpsilon)) :
                                 (fDelta <= (fInput2 * fEpsilon));
}

/*! ************************************

    \brief Return \ref TRUE if the values are less than epsilon apart

    Test if two numbers are less than or equal apart than the epsilon
    of 1e-6.

    \note The epsilon is not a constant, it's multiplied to scale with the
        magnitude

    \code
    // Example
    double dDelta = Abs(dInput1-dInput2);
    dInput1 = Abs(dInput1);
    dInput2 = Abs(dInput2);
    return (dInput1 > dInput2) ? (dDelta <= (dInput1 * dEpsilon)) : (dDelta <=
        (dInput2 * dEpsilon));

    \endcode

    \param dInput1 A 64 bit floating point number to test
    \param dInput2 Another 64 bit floating point number to test
    \param dEpsilon Epsilon to test with
    \return \ref TRUE if less than epsilon apart, \ref FALSE if not.

    \sa EqualWithEpsilon(double,double), EqualWithEpsilon(float,float,float)

***************************************/

uint_t BURGER_API Burger::EqualWithEpsilon(
    double dInput1, double dInput2, double dEpsilon) BURGER_NOEXCEPT
{
    const double dDelta = Abs(dInput1 - dInput2);
    dInput1 = Abs(dInput1);
    dInput2 = Abs(dInput2);
    return (dInput1 > dInput2) ? (dDelta <= (dInput1 * dEpsilon)) :
                                 (dDelta <= (dInput2 * dEpsilon));
}

/*! ************************************

    \brief Return a floating point number with the fraction removed

    If there is any fraction, round the value down to the whole integer.

    \param fInput 32 bit floating point value to round down to the nearest
        integer.
    \return The input converted to a 32 bit floating point value using the
        truth table below.

    \code
    float fResult;
    fResult = Burger::Floor(1.1f);		//1
    fResult = Burger::Floor(1.95f);		//1
    fResult = Burger::Floor(-1.1f);		//-2
    fResult = Burger::Floor(-1.95f);	//-2
    fResult = Burger::Floor(0.1f);		//0
    fResult = Burger::Floor(0.95f);		//0
    fResult = Burger::Floor(-0.1f);		//-1
    fResult = Burger::Floor(-0.95f);	//-1
    \endcode

    \sa Floor(double), Ceil(float), Round(float), RoundToZero(float), or
        FixedToIntFloor(Fixed32)

***************************************/

#if defined(BURGER_XBOX360)

// Do this completely branchless

float BURGER_API Burger::Floor(float fInput) BURGER_NOEXCEPT
{
    // Convert to the input to an integer (Can fail on large numbers)
    double dVar = __fcfid(__fctiwz(fInput));
    // Floor the value to - infinity
    dVar = __fsel(fInput - dVar, dVar, dVar - 1.0f); // Return floored value

    // If the input number was invalid, return the original number
    return __fself(
        fabsf(fInput) - 8388608.0f, fInput, static_cast<float>(dVar));
}

#elif defined(BURGER_PPC) && defined(BURGER_METROWERKS)

BURGER_ASM float BURGER_API Burger::Floor(float fInput) BURGER_NOEXCEPT
{
    // clang-format off
    lwz r3, g_fMinNoInteger
    fabs fp0, fp1   // Get the abs value to test
    lfs fp2, 0(r3)  // Load 8388608.0f
    fcmpu fp0, fp2  // Compare if already floored
    bgelr           // Exit with the value untouched if larger

    mffs fp4        // Save the rounding register
    mtfsfi 7, 0x03  // Set bits 30 and 31 to Round toward -infinity

    fadds fp3, fp1, fp2 // Push the positive number to highest value without fraction
                    // (Removes fraction)
    fsubs fp0, fp1, fp2 // Push the negative number to the lowest value without fraction
                    // (Removes fraction)

    fsubs fp3, fp3, fp2 // Undo the push (Fraction is gone)
    fadds fp0, fp0, fp2 // Undo the push (Fraction is gone)

    fsel fp1, fp1, fp3, fp0 // Which one to use? Positive or negative?
    mtfsf 255, fp4  // Restore rounding
    blr
    // clang-format on
}

#elif defined(BURGER_PPC) && defined(BURGER_MACOSX)

// __ZN6Burger5FloorEf = float BURGER_API Burger::Floor(float fInput)
__asm__(
    "	.align	2,0\n"
    "	.globl	__ZN6Burger5FloorEf\n"
    "__ZN6Burger5FloorEf:\n"
    "	lis		r3,ha16(__ZN6Burger15g_fMinNoIntegerE)\n"
    "	fabs	f0,f1\n" // Get the abs value to test
    "	lfs		f2,lo16(__ZN6Burger15g_fMinNoIntegerE)(r3)\n" // Load 8388608.0f
    "	fcmpu	cr0,f0,f2\n" // Compare if already floored
    "	bgelr\n"             // Exit with the value untouched if larger

    "	mffs	f4\n"     // Save the rounding register
    "	mtfsfi	7,0x03\n" // Set bits 30 and 31 to Round toward -infinity

    "	fadds	f3,f1,f2\n" // Push the positive number to highest value without
                            // fraction (Removes fraction)
    "	fsubs	f0,f1,f2\n" // Push the negative number to the lowest value
                            // without fraction (Removes fraction)

    "	fsubs	f3,f3,f2\n" // Undo the push (Fraction is gone)
    "	fadds	f0,f0,f2\n" // Undo the push (Fraction is gone)

    "	fsel	f1,f1,f3,f0\n" // Which one to use? Positive or negative?
    "	mtfsf	255,f4\n"      // Restore rounding
    "	blr\n");

#elif defined(BURGER_X86) && \
    (defined(BURGER_WATCOM) || defined(BURGER_METROWERKS) || \
        defined(BURGER_MSVC))

BURGER_DECLSPECNAKED float BURGER_API Burger::Floor(
    float /* fInput */) BURGER_NOEXCEPT
{
    // clang-format off
    BURGER_ASM
    {
        mov eax, [esp + 4]      // Load the value in an integer register
        fld dword ptr[esp + 4]  // Load the same value in the FPU
        and eax, 0x7FFFFFFF     // Mask off the sign
        cmp eax, 0x4B000000     // Compare to 8388608.0f
        jae FloorExit           // Out of range, return original value
        frndint                 // Convert the integer to float (It's in range)
        xor eax, eax            // Clear ax for flags
        fcom dword ptr[esp + 4] // Compare values for difference (Pop stack)
        fnstsw ax               // Get the 8087 condition flags
        and ax, 0x4100          // Was the new greater than or equal the original?
        jnz FloorExit
        fsub dword ptr[g_fOne]  // Subtract 1.0f to round down
    FloorExit:
        ret 4                   // Clean up and exit
    }
    // clang-format on
}

#elif defined(BURGER_X86) && (defined(BURGER_MACOSX) || defined(BURGER_IOS))

// __ZN6Burger5FloorEf = float BURGER_API Burger::Floor(float /* fInput */)
__asm__(
    "	.align	4,0x90\n"
    "	.globl __ZN6Burger5FloorEf\n"
    "__ZN6Burger5FloorEf:\n"
    "	movl	4(%esp),%eax\n"     // Load the value in an integer register
    "	flds	4(%esp)\n"          // Load the same value in the FPU
    "	andl	$0x7FFFFFFF,%eax\n" // Mask off the sign
    "	cmpl	$0x4B000000,%eax\n" // Compare to 8388608.0f
    "	jae		1f\n"               // Out of range, return original value
    "	frndint\n"              // Convert the integer to float (It's in range)
    "	xorl	%eax,%eax\n"    // Clear ax for flags
    "	fcoms	4(%esp)\n"      // Compare values for difference (Pop stack)
    "	fnstsw	%ax\n"          // Get the 8087 condition flags
    "	andl	$0x4100,%eax\n" // Was the new greater than or equal the
                                // original?
    "	jnz		1f\n"
    "	fsubs	(__ZN6Burger6g_fOneE)\n" // Subtract 1.0f to round down
    "1:	ret\n"                           // Clean up and exit
);

#else

float BURGER_API Burger::Floor(float fInput) BURGER_NOEXCEPT
{
    // Note : 8388608 is the first floating point number
    // that cannot have a fraction. Therefore this routine can't
    // affect it. I need to handle the special case since I need to
    // convert the float to an int and it's possible to have a float out
    // of bounds. This is a bad thing.

    if ((fInput < 8388608.0f) && (fInput > -8388608.0f)) { // Within bounds?
        const int iVar =
            static_cast<int>(fInput); // Convert to int but rounded!
        float fVar = static_cast<float>(iVar);
        if (fVar > fInput) { // Did I round up?
            --fVar;          // Fix it
        }
        fInput = fVar; // Return floored value
    }
    return fInput;
}
#endif

/*! ************************************

    \brief Return a floating point number with the fraction removed

    If there is any fraction, round the value down to the whole integer.

    \param dInput 64 bit floating point value to round down to the nearest
        integer.
    \return The input converted to a 64 bit floating point value using the
        truth table below.

    \code
    double dResult;
    dResult = Burger::Floor(1.1);	//1
    dResult = Burger::Floor(1.95);	//1
    dResult = Burger::Floor(-1.1);	//-2
    dResult = Burger::Floor(-1.95);	//-2
    dResult = Burger::Floor(0.1);	//0
    dResult = Burger::Floor(0.95);	//0
    dResult = Burger::Floor(-0.1);	//-1
    dResult = Burger::Floor(-0.95);	//-1
    \endcode

    \sa Floor(float), Ceil(double), Round(double), RoundToZero(double), or
        FixedToIntFloor(Fixed32)

***************************************/

#if defined(BURGER_XBOX360)

// Do this completely branchless

double BURGER_API Burger::Floor(double dInput) BURGER_NOEXCEPT
{
    // Convert to the input to an integer (Can fail on large numbers)
    double dVar = __fcfid(__fctidz(dInput));
    // Floor the value to - infinity
    dVar = __fsel(dInput - dVar, dVar, dVar - 1.0); // Return floored value

    // If the input number was invalid, return the original number
    return __fsel(fabs(dInput) - 4503599627370496.0, dInput, dVar);
}

#elif defined(BURGER_PPC) && defined(BURGER_METROWERKS)

BURGER_ASM double BURGER_API Burger::Floor(double dInput) BURGER_NOEXCEPT
{
    // clang-format off
    lwz r3, g_dMinNoInteger
    fabs fp0, fp1   // Get the abs value to test
    lfd fp2, 0(r3)  // Load 4503599627370496.0
    fcmpu fp0, fp2  // Compare if already floored
    bgelr           // Exit with the value untouched if larger

    mffs fp4        // Save the rounding register
    mtfsfi 7, 0x03  // Set bits 30 and 31 to Round toward -infinity

    fadd fp3, fp1, fp2  // Push the positive number to highest value without fraction
                        // (Removes fraction)
    fsub fp0, fp1, fp2  // Push the negative number to the lowest value without fraction
                        // (Removes fraction)

    fsub fp3, fp3, fp2  // Undo the push (Fraction is gone)
    fadd fp0, fp0, fp2  // Undo the push (Fraction is gone)

    fsel fp1, fp1, fp3, fp0 // Which one to use? Positive or negative?
    mtfsf 255, fp4          // Restore rounding
    blr
    // clang-format on
}

#elif defined(BURGER_PPC) && defined(BURGER_MACOSX)

// __ZN6Burger5FloorEd = double BURGER_API Burger::Floor(double dInput)
__asm__(
    "	.align	2,0\n"
    "	.globl	__ZN6Burger5FloorEd\n"
    "__ZN6Burger5FloorEd:\n"
    "	lis		r3,ha16(__ZN6Burger15g_dMinNoIntegerE)\n"
    "	fabs	f0,f1\n" // Get the abs value to test
    "	lfd		f2,lo16(__ZN6Burger15g_dMinNoIntegerE)(r3)\n" // Load
                                                              // 4503599627370496.0
    "	fcmpu	cr0,f0,f2\n" // Compare if already floored
    "	bgelr\n"             // Exit with the value untouched if larger

    "	mffs	f4\n"     // Save the rounding register
    "	mtfsfi	7,0x03\n" // Set bits 30 and 31 to Round toward -infinity

    "	fadd	f3,f1,f2\n" // Push the positive number to highest value without
                            // fraction (Removes fraction)
    "	fsub	f0,f1,f2\n" // Push the negative number to the lowest value
                            // without fraction (Removes fraction)

    "	fsub	f3,f3,f2\n" // Undo the push (Fraction is gone)
    "	fadd	f0,f0,f2\n" // Undo the push (Fraction is gone)

    "	fsel	f1,f1,f3,f0\n" // Which one to use? Positive or negative?
    "	mtfsf	255,f4\n"      // Restore rounding
    "	blr\n");

#elif defined(BURGER_X86) && \
    (defined(BURGER_WATCOM) || defined(BURGER_METROWERKS) || \
        defined(BURGER_MSVC))

BURGER_DECLSPECNAKED double BURGER_API Burger::Floor(
    double /* dInput */) BURGER_NOEXCEPT
{
    // clang-format off
    BURGER_ASM
    {
        mov eax, [esp + 8]      // Load the value in an integer register
        fld qword ptr[esp + 4]  // Load the same value in the FPU
        and eax, 0x7FFFFFFF     // Mask off the sign
        cmp eax, 0x43300000     // Compare to 4503599627370496.0
        jae FloorExit           // Out of range, return original value
        frndint                 // Convert the integer to float (It's in range)
        xor eax, eax            // Clear ax for flags
        fcom qword ptr[esp + 4] // Compare values for difference (Pop stack)
        fnstsw ax               // Get the 8087 condition flags
        and ax, 0x4100          // Was the new greater than or equal the original?
        jnz FloorExit
        fsub dword ptr[g_fOne]  // Subtract 1.0f to round down
    FloorExit:
        ret 8                   // Clean up and exit
    }
    // clang-format on
}

#elif defined(BURGER_X86) && (defined(BURGER_MACOSX) || defined(BURGER_IOS))

// __ZN6Burger5FloorEd = double BURGER_API Burger::Floor(double /* dInput */)
__asm__(
    "	.align	4,0x90\n"
    "	.globl __ZN6Burger5FloorEd\n"
    "__ZN6Burger5FloorEd:\n"
    "	movl	8(%esp),%eax\n"     // Load the value in an integer register
    "	fldl	4(%esp)\n"          // Load the same value in the FPU
    "	andl	$0x7FFFFFFF,%eax\n" // Mask off the sign
    "	cmpl	$0x43300000,%eax\n" // Compare to 4503599627370496.0
    "	jae		1f\n"               // Out of range, return original value
    "	frndint\n"              // Convert the integer to float (It's in range)
    "	xorl	%eax,%eax\n"    // Clear ax for flags
    "	fcoml	4(%esp)\n"      // Compare values for difference (Pop stack)
    "	fnstsw	%ax\n"          // Get the 8087 condition flags
    "	andl	$0x4100,%eax\n" // Was the new greater than or equal the
                                // original?
    "	jnz		1f\n"
    "	fsubs	(__ZN6Burger6g_fOneE)\n" // Subtract 1.0f to round down
    "1:	ret\n"                           // Clean up and exit
);

#else

double BURGER_API Burger::Floor(double dInput) BURGER_NOEXCEPT
{
    // Note : 4503599627370496.0 is the first floating point number
    // that cannot have a fraction. Therefore this routine can't
    // affect it. I need to handle the special case since I need to
    // convert the double to an int and it's possible to have a double out
    // of bounds. This is a bad thing.
    if (dInput < 4503599627370496.0 &&
        dInput > -4503599627370496.0) { // Within bounds?
        const int64_t iVar =
            static_cast<int64_t>(dInput); // Convert to int but rounded!
        double dVar = static_cast<double>(iVar);
        if (dVar > dInput) { // Did I round up?
            --dVar;          // Fix it
        }
        dInput = dVar; // Return floored value
    }
    return dInput;
}
#endif

/*! ************************************

    \brief Return a floating point number rounded up

    If there is any fraction, round the value up to the
    next whole integer.

    \param fInput 32 bit floating point value to round up to the nearest
        integer.
    \return The input converted to a 32 bit floating point value using the
        truth table below.

    \code
    float fResult;
    fResult = Burger::Ceil(1.1f);	//2
    fResult = Burger::Ceil(1.95f);	//2
    fResult = Burger::Ceil(-1.1f);	//-1
    fResult = Burger::Ceil(-1.95f);	//-1
    fResult = Burger::Ceil(0.1f);	//1
    fResult = Burger::Ceil(0.95f);	//1
    fResult = Burger::Ceil(-0.1f);	//0
    fResult = Burger::Ceil(-0.95f);	//0
    \endcode

    \sa Ceil(double), Floor(float), Round(float), RoundToZero(float), or
        FixedToIntCeil(Fixed32)

***************************************/

#if defined(BURGER_XBOX360)

// Do this completely branchless

float BURGER_API Burger::Ceil(float fInput) BURGER_NOEXCEPT
{
    // Convert to the input to an integer (Can fail on large numbers)
    double dVar = __fcfid(__fctiwz(fInput));
    // Ceiling the value to + infinity
    dVar = __fsel(dVar - fInput, dVar, dVar + 1.0f); // Return ceiling value

    // If the input number was invalid, return the original number
    return __fself(
        fabsf(fInput) - 8388608.0f, fInput, static_cast<float>(dVar));
}

#elif defined(BURGER_PPC) && defined(BURGER_METROWERKS)

BURGER_ASM float BURGER_API Burger::Ceil(float fInput) BURGER_NOEXCEPT
{
    // clang-format off
    lwz r3, g_fMinNoInteger
    fabs fp0, fp1           // Get the abs value to test
    lfs fp2, 0(r3)          // Load 8388608.0f
    fcmpu fp0, fp2          // Compare if already floored
    bgelr                   // Exit with the value untouched if larger

    mffs fp4                // Save the rounding register
    mtfsfi 7, 0x02          // Set bits 30 and 31 to Round toward +infinity

    fadds fp3, fp1, fp2     // Push the positive number to highest value without fraction
                            // (Removes fraction)
    fsubs fp0, fp1, fp2     // Push the negative number to the lowest value without fraction
                            // (Removes fraction)

    fsubs fp3, fp3, fp2     // Undo the push (Fraction is gone)
    fadds fp0, fp0, fp2     // Undo the push (Fraction is gone)

    fsel fp1, fp1, fp3, fp0 // Which one to use? Positive or negative?
    mtfsf 255, fp4          // Restore rounding
    blr
    // clang-format on
}

#elif defined(BURGER_PPC) && defined(BURGER_MACOSX)

// __ZN6Burger4CeilEf = float BURGER_API Burger::Ceil(float fInput)
__asm__(
    "	.align	2,0\n"
    "	.globl	__ZN6Burger4CeilEf\n"
    "__ZN6Burger4CeilEf:\n"
    "	lis		r3,ha16(__ZN6Burger15g_fMinNoIntegerE)\n"
    "	fabs	f0,f1\n" // Get the abs value to test
    "	lfs		f2,lo16(__ZN6Burger15g_fMinNoIntegerE)(r3)\n" // Load 8388608.0f
    "	fcmpu	cr0,f0,f2\n" // Compare if already floored
    "	bgelr\n"             // Exit with the value untouched if larger

    "	mffs	f4\n"     // Save the rounding register
    "	mtfsfi	7,0x02\n" // Set bits 30 and 31 to Round toward +infinity

    "	fadds	f3,f1,f2\n" // Push the positive number to highest value without
                            // fraction (Removes fraction)
    "	fsubs	f0,f1,f2\n" // Push the negative number to the lowest value
                            // without fraction (Removes fraction)

    "	fsubs	f3,f3,f2\n" // Undo the push (Fraction is gone)
    "	fadds	f0,f0,f2\n" // Undo the push (Fraction is gone)

    "	fsel	f1,f1,f3,f0\n" // Which one to use? Positive or negative?
    "	mtfsf	255,f4\n"      // Restore rounding
    "	blr\n");

#elif defined(BURGER_X86) && \
    (defined(BURGER_WATCOM) || defined(BURGER_METROWERKS) || \
        defined(BURGER_MSVC))

BURGER_DECLSPECNAKED float BURGER_API Burger::Ceil(
    float /* fInput */) BURGER_NOEXCEPT
{
    // clang-format off
    BURGER_ASM
    {
        mov eax, [esp + 4]      // Load the value in an integer register
        fld dword ptr[esp + 4]  // Load the same value in the FPU
        and eax, 0x7FFFFFFF     // Mask off the sign
        cmp eax, 0x4B000000     // Compare to 8388608.0f
        jae CeilExit            // Out of range, return original value
        frndint                 // Convert the integer to float (It's in range)
        xor eax, eax            // Clear ax for flags
        fcom dword ptr[esp + 4] // Compare values for difference (Pop stack)
        fnstsw ax
        and eax, 0x100          // Was the new less than the original?
        jz CeilExit
        fadd dword ptr[g_fOne]  // Add 1.0f to round up
    CeilExit:
        ret 4                   // Clean up and exit
    }
    // clang-format on
}

#elif defined(BURGER_X86) && (defined(BURGER_MACOSX) || defined(BURGER_IOS))

// __ZN6Burger4CeilEf = float BURGER_API Burger::Ceil(float /* fInput */)
__asm__(
    "	.align	4,0x90\n"
    ".globl __ZN6Burger4CeilEf\n"
    "__ZN6Burger4CeilEf:\n"
    "	movl	4(%esp),%eax\n"     // Load the value in an integer register
    "	flds	4(%esp)\n"          // Load the same value in the FPU
    "	andl	$0x7FFFFFFF,%eax\n" // Mask off the sign
    "	cmpl	$0x4B000000,%eax\n" // Compare to 8388608.0f
    "	jae		1f\n"               // Out of range, return original value
    "	frndint\n"           // Convert the integer to float (It's in range)
    "	xorl	%eax,%eax\n" // Clear ax for flags
    "	fcoms	4(%esp)\n"   // Compare values for difference (Pop stack)
    "	fnstsw	%ax\n"
    "	andl	$0x100,%eax\n" // Was the new less than the original?
    "	jz		1f\n"
    "	fadds	(__ZN6Burger6g_fOneE)\n" // Add 1.0f to round up
    "1:	ret\n");

#else

float BURGER_API Burger::Ceil(float fInput) BURGER_NOEXCEPT
{
    // Note : 8388608 is the first floating point number
    // that cannot have a fraction. Therefore this routine can't
    // affect it. I need to handle it special case since I need to
    // convert the float to an int and it's possible to have a float out
    // of bounds. This is a bad thing.

    if ((fInput < 8388608.0f) && (fInput > -8388608.0f)) { // Within bounds?
        const int iVar = static_cast<int>(fInput);         // Convert to an int
        float fVar = static_cast<float>(iVar);
        if (fVar < fInput) { // Was there a change?
            ++fVar;          // Round up
        }
        fInput = fVar; // Return the result
    }
    return fInput; // Return the input (Can't change it)
}
#endif

/*! ************************************

    \brief Return a floating point number rounded up

    If there is any fraction, round the value up to the next whole integer.

    \param dInput 64 bit floating point value to round up to the nearest
        integer.
    \return The input converted to a 64 bit floating point value using the
        truth table below.

    \code
    double dResult;
    dResult = Burger::Ceil(1.1);	//2
    dResult = Burger::Ceil(1.95);	//2
    dResult = Burger::Ceil(-1.1);	//-1
    dResult = Burger::Ceil(-1.95);	//-1
    dResult = Burger::Ceil(0.1);	//1
    dResult = Burger::Ceil(0.95);	//1
    dResult = Burger::Ceil(-0.1);	//0
    dResult = Burger::Ceil(-0.95);	//0
    \endcode

    \sa Ceil(float), Floor(double), Round(double), RoundToZero(double), or
        FixedToIntCeil(Fixed32)

***************************************/

#if defined(BURGER_XBOX360)

// Do this completely branchless

double BURGER_API Burger::Ceil(double dInput) BURGER_NOEXCEPT
{
    // Convert to the input to an integer (Can fail on large numbers)
    double dVar = __fcfid(__fctidz(dInput));
    // Ceiling the value to + infinity
    dVar = __fsel(dVar - dInput, dVar, dVar + 1.0); // Return ceilinged value

    // If the input number was invalid, return the original number
    return __fsel(fabs(dInput) - 4503599627370496.0, dInput, dVar);
}

#elif defined(BURGER_PPC) && defined(BURGER_METROWERKS)

BURGER_ASM double BURGER_API Burger::Ceil(double dInput) BURGER_NOEXCEPT
{
    // clang-format off
    lwz r3, g_dMinNoInteger
    fabs fp0, fp1       // Get the abs value to test
    lfd fp2, 0(r3)      // Load 4503599627370496.0
    fcmpu fp0, fp2      // Compare if already floored
    bgelr               // Exit with the value untouched if larger

    mffs fp4            // Save the rounding register
    mtfsfi 7, 0x02      // Set bits 30 and 31 to Round toward +infinity

    fadd fp3, fp1, fp2  // Push the positive number to highest value without fraction
                        // (Removes fraction)
    fsub fp0, fp1, fp2  // Push the negative number to the lowest value without fraction
                        // (Removes fraction)

    fsub fp3, fp3, fp2  // Undo the push (Fraction is gone)
    fadd fp0, fp0, fp2  // Undo the push (Fraction is gone)

    fsel fp1, fp1, fp3, fp0 // Which one to use? Positive or negative?
    mtfsf 255, fp4      // Restore rounding
    blr
    // clang-format on
}

#elif defined(BURGER_PPC) && defined(BURGER_MACOSX)

// __ZN6Burger4CeilEd = double BURGER_API Burger::Ceil(double dInput)
__asm__(
    "	.align	2,0\n"
    "	.globl	__ZN6Burger4CeilEd\n"
    "__ZN6Burger4CeilEd:\n"
    "	lis		r3,ha16(__ZN6Burger15g_dMinNoIntegerE)\n"
    "	fabs	f0,f1\n" // Get the abs value to test
    "	lfd		f2,lo16(__ZN6Burger15g_dMinNoIntegerE)(r3)\n" // Load
                                                              // 4503599627370496.0
    "	fcmpu	cr0,f0,f2\n" // Compare if already floored
    "	bgelr\n"             // Exit with the value untouched if larger

    "	mffs	f4\n"     // Save the rounding register
    "	mtfsfi	7,0x02\n" // Set bits 30 and 31 to Round toward +infinity

    "	fadd	f3,f1,f2\n" // Push the positive number to highest value without
                            // fraction (Removes fraction)
    "	fsub	f0,f1,f2\n" // Push the negative number to the lowest value
                            // without fraction (Removes fraction)

    "	fsub	f3,f3,f2\n" // Undo the push (Fraction is gone)
    "	fadd	f0,f0,f2\n" // Undo the push (Fraction is gone)

    "	fsel	f1,f1,f3,f0\n" // Which one to use? Positive or negative?
    "	mtfsf	255,f4\n"      // Restore rounding
    "	blr\n");

#elif defined(BURGER_X86) && \
    (defined(BURGER_WATCOM) || defined(BURGER_METROWERKS) || \
        defined(BURGER_MSVC))

BURGER_DECLSPECNAKED double BURGER_API Burger::Ceil(
    double /* dInput */) BURGER_NOEXCEPT
{
    // clang-format off
    BURGER_ASM
    {
        mov eax, [esp + 8]      // Load the value in an integer register
        fld qword ptr[esp + 4]  // Load the same value in the FPU
        and eax, 0x7FFFFFFF     // Mask off the sign
        cmp eax, 0x43300000     // Compare to 4503599627370496.0
        jae CeilExit            // Out of range, return original value
        frndint                 // Convert the integer to float (It's in range)
        xor eax, eax            // Clear ax for flags
        fcom qword ptr[esp + 4] // Compare values for difference (Pop stack)
        fnstsw ax
        and eax, 0x100          // Was the new less than the original?
        jz CeilExit
        fadd dword ptr[g_fOne]  // Add 1.0f to round up
    CeilExit:
        ret 8                   // Clean up and exit
    }
    // clang-format on
}

#elif defined(BURGER_X86) && (defined(BURGER_MACOSX) || defined(BURGER_IOS))

// __ZN6Burger4CeilEd = double BURGER_API Burger::Ceil(double /* dInput */)
__asm__(
    "	.align	4,0x90\n"
    ".globl __ZN6Burger4CeilEd\n"
    "__ZN6Burger4CeilEd:\n"
    "	movl	8(%esp),%eax\n"     // Load the value in an integer register
    "	fldl	4(%esp)\n"          // Load the same value in the FPU
    "	andl	$0x7FFFFFFF,%eax\n" // Mask off the sign
    "	cmpl	$0x43300000,%eax\n" // Compare to 4503599627370496.0
    "	jae		1f\n"               // Out of range, return original value
    "	frndint\n"           // Convert the integer to float (It's in range)
    "	xorl	%eax,%eax\n" // Clear ax for flags
    "	fcoml	4(%esp)\n"   // Compare values for difference (Pop stack)
    "	fnstsw	%ax\n"
    "	andl	$0x100,%eax\n" // Was the new less than the original?
    "	jz		1f\n"
    "	fadds	(__ZN6Burger6g_fOneE)\n" // Add 1.0f to round up
    "1:	ret\n");

#else

double BURGER_API Burger::Ceil(double dInput) BURGER_NOEXCEPT
{
    // Note : 4503599627370496.0 is the first floating point number
    // that cannot have a fraction. Therefore this routine can't
    // affect it. I need to handle it special case since I need to
    // convert the double to an int and it's possible to have a double out
    // of bounds. This is a bad thing.

    if ((dInput < 4503599627370496.0) &&
        (dInput > -4503599627370496.0)) {                  // Within bounds?
        const int64_t iVar = static_cast<int64_t>(dInput); // Convert to an int
        double dVar = static_cast<double>(iVar);
        if (dVar < dInput) { // Was there a change?
            ++dVar;          // Round up
        }
        dInput = dVar; // Return the result
    }
    return dInput; // Return the input (Can't change it)
}
#endif

/*! ************************************

    \brief Return a floating point number rounded to the nearest integer

    If there is any fraction, round the value to the whole integer with fraction
    going away from zero. This is a functional equivalent to the "C" library
    function round().

    \note This uses "Round half away from zero" rounding

    \param fInput 32 bit floating point value to round to the nearest integer.
    \return The input converted to a 32 bit floating point value using the truth
        table below.

    \code
    float fResult;
    fResult = Burger::Round(1.1f);		//1
    fResult = Burger::Round(1.95f);		//2
    fResult = Burger::Round(-1.1f);		//-1
    fResult = Burger::Round(-1.95f);	//-2
    fResult = Burger::Round(0.1f);		//0
    fResult = Burger::Round(0.95f);		//1
    fResult = Burger::Round(-0.1f);		//0
    fResult = Burger::Round(-0.95f);	//-1
    \endcode

    \sa Round(double), Floor(float), Ceil(float), RoundToZero(float), or
        FixedToIntNearest(Fixed32)

***************************************/

#if defined(BURGER_X86) && \
    (defined(BURGER_WATCOM) || defined(BURGER_METROWERKS) || \
        defined(BURGER_MSVC))

// x86 version

BURGER_DECLSPECNAKED float BURGER_API Burger::Round(float /* fInput */) BURGER_NOEXCEPT
{
    // clang-format off
    BURGER_ASM
    {
        mov eax, dword ptr[esp + 4] // Get the sign bit
        fld dword ptr[esp + 4]      // Load into the FPU
        shr eax, 31                 // 1 for negative, 0 for positive
        fstcw[esp + 4]              // Save state
        fldcw word ptr[g_X86RoundDownFlag] // Set round down
        fadd dword ptr[g_X86OneAndNegOne + eax * 4] // Add or subtract 0.5f
        frndint                     // Round
        fldcw[esp + 4]              // Restore rounding
        ret 4                       // Clean up and exit
    }
    // clang-format on
}

#elif defined(BURGER_AMD64)

float BURGER_API Burger::Round(float fInput) BURGER_NOEXCEPT
{
    // Convert to SSE register
    __m128 vInput = _mm_set_ss(fInput);

    // Load a mask value (0x80000000)
    __m128 vSignMask = _mm_set_ss(-0.0f);

    // Extract the sign
    __m128 vSign = _mm_and_ps(vSignMask, vInput);

    // Get the abs value of the input (& 0x7FFFFFFF)
    __m128 vAbs = _mm_andnot_ps(vSignMask, vInput);

    // Convert the input into an integer and convert back to float
    __m128 vResult = _mm_cvt_si2ss(vAbs, _mm_cvtt_ss2si(vAbs));

    // Get the fraction
    __m128 vDiff = _mm_sub_ss(vAbs, vResult);

    // Is the fraction above or equal to 0.5f
    vSignMask = _mm_set_ss(g_fHalf);
    __m128 vTest = _mm_cmple_ps(vSignMask, vDiff);

    // Mask 0.5f (Add it twice to get 1.0f)
    vTest = _mm_and_ps(vTest, vSignMask);
    vResult = _mm_add_ss(vResult, vTest);
    vResult = _mm_add_ss(vResult, vTest);

    // Blend the sign back
    vResult = _mm_or_ps(vResult, vSign);

    // Is the number out of bounds? Toss out the rounded value
    // and return the original
    vAbs = _mm_cmple_ss(vAbs, _mm_set_ss(g_fMinNoInteger));

    // Do an __fsel and pick which one to keep
    vResult = _mm_and_ps(vResult, vAbs);
    vAbs = _mm_andnot_ps(vAbs, vInput);
    return _mm_cvtss_f32(_mm_xor_ps(vResult, vAbs));
}

#elif defined(BURGER_XBOX360)

float BURGER_API Burger::Round(float fInput) BURGER_NOEXCEPT
{
    // Get the absolute value
    double dAbs = fabs(fInput);

    // Convert to the input to an integer (Can fail on large numbers)
    double dVar = __fcfid(__fctiwz(dAbs));

    // Get the fraction
    double dFraction = dAbs - dVar;

    // Test for rounding and add 1 if it needs to round up
    dVar += __fsel(dFraction - 0.5f, 1.0f, 0.0f);

    // Restore the sign
    dVar = __fsel(fInput, dVar, -dVar);

    // If the input number was invalid, return the original number
    return static_cast<float>(__fsel(dAbs - 8388608.0f, fInput, dVar));
}

#elif defined(BURGER_PPC) && defined(BURGER_METROWERKS)

BURGER_ASM float BURGER_API Burger::Round(float fInput) BURGER_NOEXCEPT
{
    // clang-format off
    lwz r3, g_fMinNoInteger
    fabs fp0, fp1   // Get the abs value to test
    lfs fp2, 0(r3)  // Load 8388608.0f
    fcmpu fp0, fp2  // Compare if already out of bounds
    bgelr           // Exit with the value untouched if larger

    lwz r3, g_fHalf // Pointer to 0.5f
    fadds fp5, fp0, fp2 // Push the positive number to highest value without fraction
                    // (Removes fraction)
    lfs fp3, 0(r3)  // Load 0.5f
    fsubs fp5, fp5, fp2 // Undo the push (Fraction is gone)
    fsubs fp6, fp0, fp5 // Subtract original from rounded to get the fraction
    fsubs fp6, fp6, fp3 // Test against 0.5
    fadds fp3, fp3, fp3 // Set to one
    fsubs fp4, fp5, fp5 // Set to zero
    fsel fp0, fp6, fp3, fp4 // Set to zero or one depending on the test
    fadds fp5, fp5, fp0 // Add 0 for no rounding, 1 for round up
    fnabs fp2, fp5      // Get the negative value
    fsel fp1, fp1, fp5, fp2 // Which one to use? Positive or negative?
    blr
    // clang-format on
}

#else

float BURGER_API Burger::Round(float fInput) BURGER_NOEXCEPT
{
    Word32ToFloat Converter;
    Converter.f = fInput;

    // Get the binary form of the input
    uint32_t uInput = Converter.GetWord();

    // Extract the exponent
    int32_t iExponent = static_cast<int32_t>((uInput >> 23U) & 0xFFU) - 127;

    // Already an integer? (Or Nan/Infinity)
    if (iExponent < 23) {

        // Less than 1?
        if (iExponent < 0) {

            // Only answers available are 1.0f, 0.0f, -0.0f and -1.0f

            // Turn to zero, keep sign
            uInput &= 0x80000000U;
            // 0.5f?
            if (iExponent == -1) {
                // Convert to 1.0 or -1.0f
                uInput |= 0x3F800000;
            }
            // Convert back to floating point
            Converter.SetWord(uInput);
            fInput = Converter;

        } else {
            // Create mask for fraction
            uint32_t uFractionMask =
                0x007FFFFFU >> static_cast<uint_t>(iExponent);

            // Is there a fraction?
            if (uInput & uFractionMask) {
                // Perform the round (Away from zero)
                uInput += (0x00400000U >> static_cast<uint_t>(iExponent));
                // Hack off the fraction
                uInput &= (~uFractionMask);

                // Convert to floating point
                Converter.SetWord(uInput);
                fInput = Converter;
            }
        }
    }
    // All done!
    return fInput;
}

#endif

/*! ************************************

    \brief Return a floating point number rounded to the nearest integer

    If there is any fraction, round the value to the
    whole integer.

    \param dInput 64 bit floating point value to round to the nearest integer.
    \return The input converted to a 64 bit floating point value using the truth
        table below.

    \code
    double dResult;
    dResult = Burger::Round(1.1);	//1
    dResult = Burger::Round(1.95);	//2
    dResult = Burger::Round(-1.1);	//-1
    dResult = Burger::Round(-1.95);	//-2
    dResult = Burger::Round(0.1);	//0
    dResult = Burger::Round(0.95);	//1
    dResult = Burger::Round(-0.1);	//0
    dResult = Burger::Round(-0.95);	//-1
    \endcode

    \sa Round(float), Floor(double), Ceil(double), RoundToZero(double), or
        FixedToIntNearest(Fixed32)

***************************************/

#if defined(BURGER_X86) && \
    (defined(BURGER_WATCOM) || defined(BURGER_METROWERKS) || \
        defined(BURGER_MSVC))

// x86 version

BURGER_DECLSPECNAKED double BURGER_API Burger::Round(double /* fInput */) BURGER_NOEXCEPT
{
    // clang-format off
    BURGER_ASM
    {
        mov eax, dword ptr[esp + 8] // Get the sign bit
        fld qword ptr[esp + 4]      // Load into the FPU
        shr eax, 31                 // 1 for negative, 0 for positive
        fstcw[esp + 4]              // Save state
        fldcw word ptr[g_X86RoundDownFlag] // Set round down
        fadd dword ptr[g_X86OneAndNegOne + eax * 4] // Add or subtract 0.5f
        frndint                     // Round
        fldcw[esp + 4]              // Restore rounding
        ret 8                       // Clean up and exit
    }
    // clang-format on
}

// Note: VS 2005 and earlier doesn't support the _mm_cvtsd_si64() intrinsic
#elif defined(BURGER_AMD64) && !(defined(BURGER_MSVC) && (_MSC_VER < 1500))

double BURGER_API Burger::Round(double dInput) BURGER_NOEXCEPT
{
    // Convert to SSE register
    __m128d vInput = _mm_set_sd(dInput);

    // Load a mask value (0x80000000)
    __m128d vSignMask = _mm_set_sd(-0.0f);

    // Extract the sign
    __m128d vSign = _mm_and_pd(vSignMask, vInput);

    // Get the abs value of the input (& 0x7FFFFFFFFFFFFFFF)
    __m128d vAbs = _mm_andnot_pd(vSignMask, vInput);

    // Convert the input into an integer and convert back to float
    __m128d vResult = _mm_cvtsi64_sd(vAbs, _mm_cvtsd_si64(vAbs));

    // Get the fraction
    __m128d vDiff = _mm_sub_sd(vAbs, vResult);

    // Is the fraction above or equal to 0.5
    vSignMask = _mm_set_sd(g_dHalf);
    __m128d vTest = _mm_cmple_pd(vSignMask, vDiff);

    // Mask 0.5 (Add it twice to get 1.0)
    vTest = _mm_and_pd(vTest, vSignMask);
    vResult = _mm_add_sd(vResult, vTest);
    vResult = _mm_add_sd(vResult, vTest);

    // Blend the sign back
    vResult = _mm_or_pd(vResult, vSign);

    // Is the number out of bounds? Toss out the rounded value
    // and return the original
    vAbs = _mm_cmple_sd(vAbs, _mm_set_sd(g_dMinNoInteger));

    // Do an __fsel and pick which one to keep
    vResult = _mm_and_pd(vResult, vAbs);
    vAbs = _mm_andnot_pd(vAbs, vInput);
    return _mm_cvtsd_f64(_mm_xor_pd(vResult, vAbs));
}

#elif defined(BURGER_XBOX360)

double BURGER_API Burger::Round(double dInput) BURGER_NOEXCEPT
{
    // Get the absolute value
    double dAbs = fabs(dInput);

    // Convert to the input to an integer (Can fail on large numbers)
    double dVar = __fcfid(__fctidz(dAbs));

    // Get the fraction
    double dFraction = dAbs - dVar;

    // Test for rounding and add 1 if it needs to round up
    dVar += __fsel(dFraction - 0.5f, 1.0f, 0.0f);

    // Restore the sign
    dVar = __fsel(dInput, dVar, -dVar);

    // If the input number was invalid, return the original number
    return __fsel(dAbs - 4503599627370496.0, dInput, dVar);
}

#elif defined(BURGER_PPC) && defined(BURGER_METROWERKS)

BURGER_ASM double BURGER_API Burger::Round(double dInput) BURGER_NOEXCEPT
{
    // clang-format off
    lwz r3, g_dMinNoInteger
    fabs fp0, fp1   // Get the abs value to test
    lfd fp2, 0(r3)  // Load 4503599627370496.0
    fcmpu fp0, fp2  // Compare if already out of bounds
    bgelr           // Exit with the value untouched if larger

    lwz r3, g_fHalf     // Pointer to 0.5f
    fadd fp5, fp0, fp2  // Push the positive number to highest value without fraction
                        // (Removes fraction)
    lfs fp3, 0(r3)      // Load 0.5f
    fsub fp5, fp5, fp2  // Undo the push (Fraction is gone)
    fsub fp6, fp0, fp5  // Subtract original from rounded to get the fraction
    fsub fp6, fp6, fp3  // Test against 0.5
    fadd fp3, fp3, fp3  // Set to one
    fsub fp4, fp5, fp5  // Set to zero
    fsel fp0, fp6, fp3, fp4 // Set to zero or one depending on the test
    fadd fp5, fp5, fp0  // Add 0 for no rounding, 1 for round up
    fnabs fp2, fp5      // Get the negative value
    fsel fp1, fp1, fp5, fp2 // Which one to use? Positive or negative?
    blr
    // clang-format on
}

#else

double BURGER_API Burger::Round(double dInput) BURGER_NOEXCEPT
{
    Word64ToDouble Converter;
    Converter.d = dInput;

    // Get the binary form of the input
    uint32_t uInputHi = Converter.GetHiWord();
    uint32_t uInputLow = Converter.GetLowWord();

    // Extract the exponent
    int32_t iExponent = static_cast<int32_t>((uInputHi >> 20U) & 0x7FFU) - 1023;

    // Already an integer? (Or Nan/Infinity)
    if (iExponent < 20) {

        // Less than 1?
        if (iExponent < 0) {

            // Only answers available are 1.0, 0.0, -0.0 and -1.0

            // Turn to zero, keep sign
            uInputHi &= 0x80000000U;
            // 0.5?
            if (iExponent == -1) {
                // Convert to 1.0 or -1.0
                uInputHi |= 0x3FF00000U;
            }
            // Convert back to floating point
            Converter.SetHiWord(uInputHi);
            Converter.SetLowWord(0);
            dInput = Converter;

        } else {
            // Create mask for fraction
            uint32_t uFractionMask =
                0x000FFFFFU >> static_cast<uint_t>(iExponent);

            // Is there a fraction?
            if ((uInputHi & uFractionMask) | uInputLow) {
                // Perform the round (Away from zero)
                uInputHi += (0x00080000U >> static_cast<uint_t>(iExponent));
                // Hack off the fraction
                // Convert to floating point
                Converter.SetHiWord(uInputHi & (~uFractionMask));
                Converter.SetLowWord(0);
                dInput = Converter;
            }
        }

    } else if (iExponent <= 51) {
        uint32_t uFractionMask =
            0xFFFFFFFFU >> static_cast<uint_t>(iExponent - 20);
        // Is there a fraction?
        if (uInputLow & uFractionMask) {
            // Perform a 64 bit mask with 32 bit registers
            uint32_t uCarryTest =
                uInputLow + (1U << static_cast<uint_t>(51 - iExponent));
            if (uCarryTest < uInputLow) {
                ++uInputHi;
            }
            Converter.SetLowWord(uCarryTest & (~uFractionMask));
            Converter.SetHiWord(uInputHi);
            dInput = Converter;
        }
    }
    return dInput;
}

#endif

/*! ************************************

    \brief Return a floating point number rounded to zero

    If there is any fraction, round the value to the whole integer towards zero

    \param fInput 32 bit floating point value to round to zero.
    \return The input converted to a 32 bit floating point value using the truth
        table below.

    \code
    float fResult;
    fResult = Burger::RoundToZero(1.1f);	//1
    fResult = Burger::RoundToZero(1.95f);	//1
    fResult = Burger::RoundToZero(-1.1f);	//-1
    fResult = Burger::RoundToZero(-1.95f);	//-1
    fResult = Burger::RoundToZero(0.1f);	//0
    fResult = Burger::RoundToZero(0.95f);	//0
    fResult = Burger::RoundToZero(-0.1f);	//0
    fResult = Burger::RoundToZero(-0.95f);	//0
    \endcode

    \sa RoundToZero(double), Floor(float), Ceil(float), Round(float), or
        FixedToInt(Fixed32)

***************************************/

float BURGER_API Burger::RoundToZero(float fInput) BURGER_NOEXCEPT
{
    if (fInput < 0.0f) {
        fInput = Ceil(fInput);
    } else {
        fInput = Floor(fInput);
    }
    return fInput;
}

/*! ************************************

    \brief Return a floating point number rounded to zero

    If there is any fraction, round the value to the whole integer towards zero

    \param dInput 64 bit floating point value to round to zero.
    \return The input converted to a 64 bit floating point value using the truth
        table below.

    \code
    double dResult;
    dResult = Burger::RoundToZero(1.1);		//1
    dResult = Burger::RoundToZero(1.95);	//1
    dResult = Burger::RoundToZero(-1.1);	//-1
    dResult = Burger::RoundToZero(-1.95);	//-1
    dResult = Burger::RoundToZero(0.1);		//0
    dResult = Burger::RoundToZero(0.95);	//0
    dResult = Burger::RoundToZero(-0.1);	//0
    dResult = Burger::RoundToZero(-0.95);	//0
    \endcode

    \sa RoundToZero(float), Floor(double), Ceil(double), Round(double), or
        FixedToInt(Fixed32)

***************************************/

double BURGER_API Burger::RoundToZero(double dInput) BURGER_NOEXCEPT
{
    if (dInput < 0.0) {
        dInput = Ceil(dInput);
    } else {
        dInput = Floor(dInput);
    }
    return dInput;
}

float BURGER_API Burger::Tan(float fInput) BURGER_NOEXCEPT
{
// Watcom and Mac Class StdC do not have the fast version
#if defined(BURGER_WATCOM) || (defined(BURGER_MAC) && defined(__MATH_H__))
    return static_cast<float>(tan(fInput));
#else
    return tanf(fInput);
#endif
}

double BURGER_API Burger::Tan(double dInput) BURGER_NOEXCEPT
{
    return tan(dInput);
}

float BURGER_API Burger::ASin(float fInput) BURGER_NOEXCEPT
{
// Watcom and Mac Class StdC do not have the fast version
#if defined(BURGER_WATCOM) || (defined(BURGER_MAC) && defined(__MATH_H__))
    return static_cast<float>(asin(fInput));
#else
    return asinf(fInput);
#endif
}

double BURGER_API Burger::ASin(double dInput) BURGER_NOEXCEPT
{
    return asin(dInput);
}

float BURGER_API Burger::ACos(float fInput) BURGER_NOEXCEPT
{
// Watcom and Mac Class StdC do not have the fast version
#if defined(BURGER_WATCOM) || (defined(BURGER_MAC) && defined(__MATH_H__))
    return static_cast<float>(acos(fInput));
#else
    return acosf(fInput);
#endif
}

double BURGER_API Burger::ACos(double dInput) BURGER_NOEXCEPT
{
    return acos(dInput);
}

float BURGER_API Burger::ATan(float fInput) BURGER_NOEXCEPT
{
// Watcom and Mac Class StdC do not have the fast version
#if defined(BURGER_WATCOM) || (defined(BURGER_MAC) && defined(__MATH_H__))
    return static_cast<float>(atan(fInput));
#else
    return atanf(fInput);
#endif
}

double BURGER_API Burger::ATan(double dInput) BURGER_NOEXCEPT
{
    return atan(dInput);
}

float BURGER_API Burger::ATan2(float fSin, float fCos) BURGER_NOEXCEPT
{
// Watcom and Mac Class StdC do not have the fast version
#if defined(BURGER_WATCOM) || (defined(BURGER_MAC) && defined(__MATH_H__))
    return static_cast<float>(atan2(fSin, fCos));
#else
    return atan2f(fSin, fCos);
#endif
}

double BURGER_API Burger::ATan2(double dSin, double dCos) BURGER_NOEXCEPT
{
    return atan2(dSin, dCos);
}

float BURGER_API Burger::Pow(float fX, float fY) BURGER_NOEXCEPT
{
// Watcom and Mac Class StdC do not have the fast version
#if defined(BURGER_WATCOM) || (defined(BURGER_MAC) && defined(__MATH_H__))
    return static_cast<float>(pow(fX, fY));
#else
    return powf(fX, fY);
#endif
}

double BURGER_API Burger::Pow(double dX, double dY) BURGER_NOEXCEPT
{
    return pow(dX, dY);
}

float BURGER_API Burger::Exp(float fInput) BURGER_NOEXCEPT
{
// Watcom and Mac Class StdC do not have the fast version
#if defined(BURGER_WATCOM) || (defined(BURGER_MAC) && defined(__MATH_H__))
    return static_cast<float>(exp(fInput));
#else
    return expf(fInput);
#endif
}

double BURGER_API Burger::Exp(double dInput) BURGER_NOEXCEPT
{
    return exp(dInput);
}

float BURGER_API Burger::Log(float fInput) BURGER_NOEXCEPT
{
// Watcom and Mac Class StdC do not have the fast version
#if defined(BURGER_WATCOM) || (defined(BURGER_MAC) && defined(__MATH_H__))
    return static_cast<float>(log(fInput));
#else
    return logf(fInput);
#endif
}

double BURGER_API Burger::Log(double dInput) BURGER_NOEXCEPT
{
    return log(dInput);
}

static const float LN_2 = 1.44269504088896340736f;

float BURGER_API Burger::Log2(float fInput) BURGER_NOEXCEPT
{
    return Log(fInput) * LN_2;
}

double BURGER_API Burger::Log2(double dInput) BURGER_NOEXCEPT
{
    return Log(dInput) * 1.44269504088896340736;
}

float BURGER_API Burger::Log10(float fInput) BURGER_NOEXCEPT
{
// Watcom and Mac Class StdC do not have the fast version
#if defined(BURGER_WATCOM) || (defined(BURGER_MAC) && defined(__MATH_H__))
    return static_cast<float>(log10(fInput));
#else
    return log10f(fInput);
#endif
}

double BURGER_API Burger::Log10(double dInput) BURGER_NOEXCEPT
{
    return log10(dInput);
}

float BURGER_API Burger::Modf(float fInput, float* pInteger) BURGER_NOEXCEPT
{
// Watcom and Mac Class StdC do not have the fast version
#if defined(BURGER_WATCOM) || (defined(BURGER_MAC) && defined(__MATH_H__))
    double dInteger;
    float fResult = static_cast<float>(modf(fInput, &dInteger));
    if (pInteger) {
        pInteger[0] = static_cast<float>(dInteger);
    }
    return fResult;
#else
    return modff(fInput, pInteger);
#endif
}

double BURGER_API Burger::Modf(double dInput, double* pInteger) BURGER_NOEXCEPT
{
    return modf(dInput, pInteger);
}

float BURGER_API Burger::Fmod(float fInput, float fDivisor) BURGER_NOEXCEPT
{
// Watcom and Mac Class StdC do not have the fast version
#if defined(BURGER_WATCOM) || (defined(BURGER_MAC) && defined(__MATH_H__))
    return static_cast<float>(fmod(fInput, fDivisor));
#else
    return fmodf(fInput, fDivisor);
#endif
}

double BURGER_API Burger::Fmod(double dInput, double dDivisor) BURGER_NOEXCEPT
{
    return fmod(dInput, dDivisor);
}

/*! ************************************

    \brief Load a big endian 80 bit float as a double

    Convert an 80 bit float stored in memory in big
    endian format into a double in native endian

    \param pInput Pointer to the 80 bit floating point number
    \return Value in the form of a double
    \sa LittleEndianLoadExtended(const Float80Bit)

***************************************/

double BURGER_API Burger::BigEndianLoadExtended(const Float80Bit pInput) BURGER_NOEXCEPT
{
    // Union to expose the double
    union {
        double m_dDouble;
        uint8_t m_Bytes[8];
    } Result;

    // Obtain the exponent and sign
    uint_t uExponent =
        BigEndian::Load(reinterpret_cast<const uint16_t*>(pInput));
    // Extract the sign
    uint_t uSign = (uExponent >> 8U) & 0x80U;
    // Normalize the exponent to the range of a double
    uExponent = (uExponent - (0x0400C - 0x040C)) & 0x7FFU;

    // Swap endian version
#if defined(BURGER_LITTLEENDIAN)

    // Set the exponent and sign in the double
    Result.m_Bytes[7] = static_cast<uint8_t>((uExponent >> 4U) | uSign);
    Result.m_Bytes[6] = static_cast<uint8_t>(uExponent << 4U);

    uSign = 2;
    do {
        uExponent = reinterpret_cast<const uint8_t*>(pInput)[uSign];
        if (uSign == 2) {
            uExponent &= 0x7FU;
        }
        Result.m_Bytes[8 - uSign] |= static_cast<uint8_t>(uExponent >> 3U);
        if (uSign != 8) {
            Result.m_Bytes[7 - uSign] = static_cast<uint8_t>(uExponent << 5U);
        }
    } while (++uSign < 9);

#else
    // Return IEEE double in big endian format

    Result.m_Bytes[0] = static_cast<uint8_t>((uExponent >> 4U) | uSign);
    Result.m_Bytes[1] = static_cast<uint8_t>(uExponent << 4U);

    uSign = 2;
    do {
        uExponent = reinterpret_cast<const uint8_t*>(pInput)[uSign];
        if (uSign == 2) {
            uExponent &= 0x7f;
        }
        Result.m_Bytes[uSign - 1] |= static_cast<uint8_t>(uExponent >> 3U);
        if (uSign != 8) {
            Result.m_Bytes[uSign] = static_cast<uint8_t>(uExponent << 5U);
        }
    } while (++uSign < 9);
#endif
    return Result.m_dDouble;
}

/*! ************************************

    \brief Load a little endian 80 bit float as a double

    Convert an 80 bit float stored in memory in little
    endian format into a double in native endian

    \param pInput Pointer to the 80 bit floating point number
    \return Value in the form of a double
    \sa BigEndianLoadExtended(const Float80Bit)

***************************************/

double BURGER_API Burger::LittleEndianLoadExtended(const Float80Bit pInput) BURGER_NOEXCEPT
{
    // Union to expose the double
    union {
        double m_dDouble;
        uint8_t m_Bytes[8];
    } Result;

    // Obtain the exponent and sign
    uint_t uExponent =
        LittleEndian::Load(&reinterpret_cast<const uint16_t*>(pInput)[4]);
    // Extract the sign
    uint_t uSign = (uExponent >> 8U) & 0x80U;
    // Normalize the exponent to the range of a double
    uExponent = (uExponent - (0x0400C - 0x040C)) & 0x7FFU;

    // Swap endian version
#if defined(BURGER_LITTLEENDIAN)

    // Set the exponent and sign in the double
    Result.m_Bytes[7] = static_cast<uint8_t>((uExponent >> 4U) | uSign);
    Result.m_Bytes[6] = static_cast<uint8_t>(uExponent << 4U);

    uSign = 7;
    do {
        uExponent = reinterpret_cast<const uint8_t*>(pInput)[uSign];
        if (uSign == 7) {
            uExponent &= 0x7FU;
        }
        Result.m_Bytes[uSign - 1] |= static_cast<uint8_t>(uExponent >> 3U);
        if (uSign != 1) {
            Result.m_Bytes[uSign - 2] = static_cast<uint8_t>(uExponent << 5U);
        }
    } while (--uSign);

#else
    // Return IEEE double in big endian format

    Result.m_Bytes[0] = static_cast<uint8_t>((uExponent >> 4U) | uSign);
    Result.m_Bytes[1] = static_cast<uint8_t>(uExponent << 4U);

    uSign = 7;
    do {
        uExponent = reinterpret_cast<const uint8_t*>(pInput)[uSign];
        if (uSign == 7) {
            uExponent &= 0x7f;
        }
        Result.m_Bytes[8 - uSign] |= static_cast<uint8_t>(uExponent >> 3U);
        if (uSign != 1) {
            Result.m_Bytes[7 - uSign] = static_cast<uint8_t>(uExponent << 5U);
        }
    } while (--uSign);
#endif
    return Result.m_dDouble;
}

/*! ************************************

    \brief Convert a linear volume to decibels

    DirectSound requires volume inputs to be set in decibels
    from 0 being full volume to -10000 being silent.

    Input values higher than 255 will return 0 (Max volume)

    \param uInput Volume from 0 to 255 as a percentage of volume in 256 steps
    \return Value in the form of 0 to -10000 in the scale needed for DirectSound
    \sa ConvertToDirectSoundVolume(float)

***************************************/

long BURGER_API Burger::ConvertToDirectSoundVolume(uint_t uInput) BURGER_NOEXCEPT
{
    long lResult;
    // Anything softer than this is pretty much silence
    if (!uInput) {
        lResult = -10000;
        // Anything louder than 1.0f is maximum
    } else if (uInput >= 255) {
        lResult = 0;
    } else {
        // Log(1.0/.0000000001) yields 10, which is the largest number I'm
        // interested in Normalize 0-255 to 0-1.0f
        float fInput =
            static_cast<float>(static_cast<int>(uInput)) * (1.0f / 255.0f);
        lResult = static_cast<long>(Log(1.0f / fInput) * -1000.0f);
    }
    return lResult;
}

/*! ************************************

    \brief Convert a linear volume to decibels

    DirectSound requires volume inputs to be set in decibels
    from 0 being full volume to -10000 being silent.

    Input values lower than 0.0f will return -10000 (Silence)
    and higher than 1.0f will return 0 (Max volume)

    \param fInput Volume from 0.0f to 1.0f as a percentage of volume
    \return Value in the form of 0 to -10000 in the scale needed for DirectSound
    \sa ConvertToDirectSoundVolume(uint_t)

***************************************/

long BURGER_API Burger::ConvertToDirectSoundVolume(float fInput) BURGER_NOEXCEPT
{
    long lResult;
    // Anything softer than this is pretty much silence
    if (fInput <= 0.0001f) {
        lResult = -10000;
        // Anything louder than 1.0f is maximum
    } else if (fInput >= 1.0f) {
        lResult = 0;
    } else {
        // Log(1.0/.0000000001) yields 10, which is the largest number I'm
        // interested in
        lResult = static_cast<long>(Log(1.0f / fInput) * -1000.0f);
    }
    return lResult;
}

/*! ************************************

    \brief Convert a linear pan to decibels

    DirectSound requires pan inputs to be set in decibels from -10000 being full
    left, 0 being center and 10000 being full right.

    Input values lower than 2 will return -10000 (Full left) and higher than
    0xFFFEU will return 10000 (Full right)

    \param uInput Pan from 0x0000 to 0xFFFFU as a pan value
    \return Value in the form of -10000 to -10000 in the scale needed for
        DirectSound
    \sa ConvertToDirectSoundPan(float)

***************************************/

long BURGER_API Burger::ConvertToDirectSoundPan(uint_t uInput) BURGER_NOEXCEPT
{
    long lResult;
    if (uInput == 0x8000U) {
        lResult = 0;
    } else if (uInput < 3) {
        lResult = -10000;
    } else if (uInput >= 0xFFFEU) {
        lResult = 10000;
    } else {
        // Normalize the pan of 0x0000 (Left) 0x8000 (Center) 0xFFFF (Right)
        // to -1.0 - 1.0f
        const float fValue =
            static_cast<float>(static_cast<int32_t>(uInput - 0x8000U)) *
            (1.0f / 32767.0f);
        lResult =
            static_cast<long>(Log(1.0f / (1.0f - Abs(fValue))) * -1000.0f);
        if (uInput >= 0x8000U) {
            lResult = -lResult;
        }
    }
    return lResult;
}

/*! ************************************

    \brief Convert a linear pan to decibels

    DirectSound requires pan inputs to be set in decibels from -10000 being full
    left, 0 being center and 10000 being full right.

    Input values lower than -1.0f will return -10000 (Full left) and higher
    than 1.0f will return 10000 (Full right)

    \param fInput Pan from -1.0f to 1.0f as a pan value
    \return Value in the form of -10000 to -10000 in the scale needed for
        DirectSound
    \sa ConvertToDirectSoundPan(uint_t)

***************************************/

long BURGER_API Burger::ConvertToDirectSoundPan(float fInput) BURGER_NOEXCEPT
{
    // Get the absolute value
    const float fAbs = Abs(fInput);

    // Close to the center?
    long lResult;
    if (fAbs <= 0.0001f) {
        lResult = 0; // No panning

    } else {

        // Leftmost or rightmost?
        if (fAbs >= 0.9999f) {
            // DirectSound left (Will negate if right)
            lResult = -10000;
        } else {

            // Calculate the value assuming left
            // and the input is clamped at 0.0f to 1.0f

            lResult = static_cast<long>(Log(1.0f / (1.0f - fAbs)) * -1000.0f);
        }
        // If positive then negate (Make positive)
        if (fInput >= 0.0f) {
            lResult = -lResult;
        }
    }
    return lResult;
}

/*! ************************************

    \brief Convert a linear volume to 0.0f to 1.0f

    Clamp the volume to 0-255 and then convert the value
    into floating point.

    \param uInput Volume from 0 to 255. Numbers higher than 255 will be clamped.
    \return Value in the form of 0.0f to 1.0f in the scale needed for Mac OSX /
        iOS AudioUnit
    \sa ConvertToAudioUnitPan(uint_t)

***************************************/

float BURGER_API Burger::ConvertToAudioUnitVolume(uint_t uInput) BURGER_NOEXCEPT
{
    float fResult;
    if (!uInput) {
        fResult = 0.0f;
    } else if (uInput < 255U) {
        fResult =
            static_cast<float>(static_cast<int>(uInput)) * (1.0f / 255.0f);
    } else {
        fResult = 1.0f;
    }
    return fResult;
}

/*! ************************************

    \brief Convert a linear pan to 0.0f to 1.0f

    Convert the pan from 0-65535 into 0.0f to 1.0f.

    \param uInput Pan from 0 to 65535. Numbers higher than 65535 will be
        clamped.
    \return Value in the form of 0.0f to 1.0f in the scale needed for Mac
        OSX / iOS AudioUnit

    \sa ConvertToAudioUnitVolume(uint_t)

***************************************/

float BURGER_API Burger::ConvertToAudioUnitPan(uint_t uInput) BURGER_NOEXCEPT
{
    float fResult;
    if (uInput >= 0xFFFFU) {
        fResult = 1.0f;
    } else {
        fResult = static_cast<float>(static_cast<int>(uInput) - 32768) *
            (1.0f / 32768.0f);
    }
    return fResult;
}

/*! ************************************

    \brief Convert a DirectSound decibel value to an XAudio2 Amplitude
        multiplier

    Convert the decibels from -10000 to 0 to 0 to 1.0f

    \param fDecibels DirectSound decibels from -10000 to 0.
    \return Value in the form of 0.0f to 1.0f in the scale needed for XAudio2
        SetVolume()
    \sa AmplitudeRatioToDecibels(float)

***************************************/

float BURGER_API Burger::DecibelsToAmplitudeRatio(
    float fDecibels) BURGER_NOEXCEPT
{
    return Pow(10.0f, fDecibels * (1.0f / 20.0f));
}

/*! ************************************

    \brief Convert a XAudio2 Amplitude multiplier to a DirectSound decibel value

    Convert the volume of 0.0f to 1.0f into decibels from -10000 to 0

    \param fVolume in the form of 0.0f to 1.0f in the scale needed for XAudio2
        GetVolume()

        \return DirectSound decibels from -10000 to 0.
    \sa DecibelsToAmplitudeRatio(float)

***************************************/

float BURGER_API Burger::AmplitudeRatioToDecibels(float fVolume) BURGER_NOEXCEPT
{
    if (fVolume == 0.0f) {
        fVolume = g_fNegMax; // Smallest value (Negative)
    } else {
        fVolume = Log10(fVolume) * 20.0f;
    }
    return fVolume;
}
