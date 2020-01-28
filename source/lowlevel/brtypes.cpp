/***************************************

    Determine which compiler is being used and create standardized typedefs and
    macros so generic code can be created cross platform

    Copyright (c) 1995-2020 by Rebecca Ann Heineman <becky@burgerbecky.com>

    It is released under an MIT Open Source license. Please see LICENSE for
    license details. Yes, you can use it in a commercial title without paying
    anything, just give me a credit.

    Please? It's not like I'm asking you for money!

***************************************/

#if defined(DOXYGEN)
#define _DEBUG
#define NDEBUG

#define BURGER_WATCOM __WATCOMC__
#define BURGER_MRC __MRC__
#define BURGER_APPLE_SC __SC__
#define BURGER_DJGPP ((__DJGPP__ * 100) + __DJGPP_MINOR__)
#define BURGER_SNSYSTEMS __SN_VER__
#define BURGER_GHS __GHS_VERSION_NUMBER
#define BURGER_INTEL_COMPILER \
    ((__INTEL_COMPILER * 10) + __INTEL_COMPILER_UPDATE)
#define BURGER_MINGW
#define BURGER_CLANG \
    ((__clang_major__ * 10000) + (__clang_minor__ * 100) + __clang_patchlevel__)
#define BURGER_GNUC \
    ((__GNUC__ * 10000) + (__GNUC_MINOR__ * 100) + __GNUC_PATCHLEVEL__)
#define BURGER_METROWERKS __MWERKS__
#define BURGER_ARM_COMPILER __ARMCC_VERSION
#define BURGER_MSVC _MSC_FULL_VER
#define BURGER_COMPILER_NAME "The name of the compiler"
#define BURGER_COMPILER_VERSION 1234

#define BURGER_X86
#define BURGER_AMD64
#define BURGER_ITANIUM
#define BURGER_ARM32
#define BURGER_ARM64
#define BURGER_MIPS32
#define BURGER_MIPS64
#define BURGER_POWERPC
#define BURGER_POWERPC64
#define BURGER_68K
#define BURGER_RISCV
#define BURGER_AVR
#define BURGER_MSP430
#define BURGER_SPARC32
#define BURGER_SPARC64
#define BURGER_CPU_NAME "The name of the CPU"

#define BURGER_3DNOW

#define BURGER_XBOX
#define BURGER_XBOX360
#define BURGER_XBOXONE
#define BURGER_MSDOS
#define BURGER_DOS4G
#define BURGER_X32
#define BURGER_WIN32
#define BURGER_WIN64
#define BURGER_PS1
#define BURGER_PS2
#define BURGER_PS3
#define BURGER_PS4
#define BURGER_PSP
#define BURGER_VITA
#define BURGER_ANDROID
#define BURGER_SHIELD
#define BURGER_OUYA
#define BURGER_GBA
#define BURGER_DS
#define BURGER_3DS
#define BURGER_GAMECUBE
#define BURGER_WII
#define BURGER_WIIU
#define BURGER_SWITCH
#define BURGER_BEOS
#define BURGER_SYMBIAN
#define BURGER_NGAGE
#define BURGER_AMIGA
#define BURGER_MAC
#define BURGER_CFM
#define BURGER_MACCLASSIC
#define BURGER_MACCARBON
#define TARGET_API_MAC_CARBON 1
#define BURGER_IOS
#define BURGER_MACOSX
#define BURGER_LINUX
#define BURGER_ARDUINO
#define BURGER_PLATFORM_NAME "Name of operating system"

#define BURGER_LITTLEENDIAN

#define BURGER_FASTCALLENABLED
#define BURGER_STRUCT_ALIGN
#define BURGER_STRUCT_PACK
#define BURGER_STRUCT_PACKPUSH
#define BURGER_NO_ALIGN

#define nullptr 0
#define __underlying_type(x) int

#define BURGER_HAS_WCHAR_T
#define BURGER_INT_NOT_IN_STDINT
#define BURGER_NO_USING_NAMESPACE

/*! ************************************

    \page pageintroduction Introduction to Burgerlib.

    \section intro_sec Introduction

    BurgerLib is a set of subroutines that are designed to work the same across
    multiple target platforms. Proper use of this library will allow rapid
    development and deployment of game and application software across many
    different operating systems and computers.

    All aspects of the virtual OS are standardized so most interaction with the
    host OS an be done transparently so the application needs minimal work for
    platform localization.

    The library was not designed to be a game engine. It was designed to be a
    set of foundation functions that will allow the creation of game engines
    that are cross platform and will take advantage of most features of the
    underlying hardware without being aware that the features are even present.

    Compilers supported under a Win32 host:

    * * <a href="http://www.openwatcom.com"
        target="_blank">Open Watcom 1.9</a> for <a
        href="http://www.dosextender.com/" target="_blank">FlashTek X-32</a>
        Intel.

    * * <a href="http://www.openwatcom.com" target="_blank">Open Watcom 1.9</a>
        for <a href="http://www.tenberry.com/dos4g/watcom/"
        target="_blank">DOS/4GW</a> Intel.

    * * <a href="http://www.openwatcom.com" target="_blank">Open
        Watcom 1.9</a> for <a href="http://msdn.microsoft.com"
        target="_blank">Win32</a> Intel.

    * * <a href="http://www.intel.com/software/products/compilers/cwin/"
        target="_blank">Intel C++ 10.0</a> for <a
        href="http://msdn.microsoft.com" target="_blank">Win32</a> Intel.

    * * <a href="http://msdn2.microsoft.com/en-us/vstudio/aa973782.aspx"
        target="_blank">Visual Studio .NET 2005</a> for <a
        href="http://msdn.microsoft.com" target="_blank">Win32</a> Intel.

    * * <a href="http://msdn2.microsoft.com/en-us/vstudio/aa973782.aspx"
        target="_blank">Visual Studio .NET 2005</a> for <a
        href="http://msdn.microsoft.com" target="_blank">Win64</a> AMD64.

    * * <a href="http://msdn2.microsoft.com/en-us/vstudio/aa700867.aspx"
        target="_blank">Visual Studio .NET 2003</a> for <a
        href="http://www.xbox.com" target="_blank">XBox</a> Intel Celeron.

    * * <a href="http://msdn2.microsoft.com/en-us/vstudio/aa973782.aspx"
        target="_blank">Visual Studio .NET 2005</a> for <a
        href="http://www.xbox.com" target="_blank">XBox 360</a> PowerPC.

    * * <a
        href="http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=CW-SONY-PS2"
        target="_blank">Metrowerks CodeWarrior 3.8.1</a> for <a
        href="http://www.playstation.com/" target="_blank">Sony Playstation
        2</a> MIPS R5900.

    * * <a href="http://www.snsys.com/PlayStation2/ProDG.htm"
        target="_blank">SNSystems GCC</a> for <a
        href="http://www.playstation.com/" target="_blank">Sony Playstation
        2</a> MIPS R5900.

    * * <a href="http://www.lukasz.dk/ps2toolchain.html"
        target="_blank">PS2Dev</a> for <a href="http://www.playstation.com/"
        target="_blank">Sony Playstation 2</a> MIPS R5900.

    * * <a
        href="http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=CW-NINTENDO-GC"
        target="_blank">Metrowerks CodeWarrior 2.7</a> for <a
        href="http://www.nintendo.com/systemsgcn" target="_blank">Nintendo
        GameCube</a> PowerPC Gekko.

    * * <a href="http://www.snsys.com/GameCube/ProDG.htm"
        target="_blank">SNSystems GCC</a> for <a
        href="http://www.nintendo.com/systemsgcn" target="_blank">Nintendo
        GameCube</a> PowerPC Gekko.

    * * <a
        href="http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=CW-NINTENDODS"
        target="_blank">Metrowerks CodeWarrior 2.0</a> for <a
        href="http://www.nintendo.com/systemsds" target="_blank">Nintendo DS</a>
        ARM 9.

    * * <a href="http://www.warioworld.com" target="_blank">Metrowerks
        CodeWarrior 3.0</a> for <a href="http://www.nintendo.com/systemswii"
        target="_blank">Nintendo Wii</a> PowerPC Gekko 2.

    Compilers supported under a MacOSX host:

    * * <a href="http://developer.apple.com/tools/mpw-tools/"
        target="_blank">MrC 5.0d3 MPW</a> for <a
        href="https://en.wikipedia.org/wiki/Mac_OS_9" target="_blank">Mac OS
        Classic</a> PowerPC.

    * * <a
        href="http://www.metrowerks.com/MW/Develop/Desktop/Windows/Professional/Default.htm"
        target="_blank">Metrowerks CodeWarrior 9.6</a> for <a
        href="http://msdn.microsoft.com" target="_blank">Win32</a> Intel.

    * * <a
        href="http://www.metrowerks.com/MW/Develop/Desktop/Macintosh/Professional/Mac9.htm"
        target="_blank">Metrowerks CodeWarrior 6.0</a> for <a
        href="https://en.wikipedia.org/wiki/Mac_OS_9" target="_blank">MacOS
        Classic 8.6/9.2</a> Motorola 68K.

    * * <a
        href="http://www.metrowerks.com/MW/Develop/Desktop/Macintosh/Professional/Mac9.htm"
        target="_blank">Metrowerks CodeWarrior 10.0</a> for <a
        href="https://en.wikipedia.org/wiki/Mac_OS_9" target="_blank">MacOS
        Classic 8.6/9.2</a> PowerPC.

    * * <a
        href="http://www.metrowerks.com/MW/Develop/Desktop/Macintosh/Professional/Mac9.htm"
        target="_blank">Metrowerks CodeWarrior 10.0</a> for <a
        href="https://www.apple.com/macos/high-sierra/" target="_blank">MacOS
        Carbon 8.6/9.2/X</a> PowerPC.

    * * <a
        href="http://www.metrowerks.com/MW/Develop/Desktop/Macintosh/Professional/Mac9.htm"
        target="_blank">Metrowerks CodeWarrior 10.0</a> for <a
        href="https://www.apple.com/macos/high-sierra/" target="_blank">MacOS
        X</a> Mach PowerPC.

    * * <a href="https://developer.apple.com/xcode/" target="_blank">Apple
        XCode 3.0</a> for <a href="http://www.apple.com/macosx/"
        target="_blank">MacOS X</a> Mach PowerPC.

    * * <a href="https://developer.apple.com/xcode/" target="_blank">Apple
        XCode 10.0</a> for <a href="https://www.apple.com/macos/high-sierra/"
        target="_blank">MacOS X</a> Mach Intel.

***************************************/

/*! ************************************

    \file burger.h
    \brief The master all inclusive header.

    This header contains the base types used in all of Burgerlib. It will also
    try to determine via compiler defines exactly what features are supported in
    the compiler and what target OS you're building for.

***************************************/

/*! ************************************

    \def _DEBUG
    \brief Defined to enable the creation of debug code.

    If this define exists, you can assume that you're compiling a debug build.
    This is used to flag whether debug code is to be included in the build
    and/or \ref BURGER_ASSERT is to operate. Only \ref _DEBUG or \ref NDEBUG can
    exist at a time. Specifying both or neither is considered a fatal compile
    error. This define is not supplied by Burgerlib, it must be defined by the
    build project.

    \note This flag is not used to determine if optimizations have been
        disabled. It's only used to enable the generation of debugging code.

    \code
    #if defined(_DEBUG)
        CheckMemoryHeap();
    #endif
    \endcode

***************************************/

/*! ************************************

    \def NDEBUG
    \brief Defined to disable the creation of debug code.

    If this define exists, then you are creating a release build. This is only
    present to force \ref BURGER_ASSERT to vanish. Only \ref _DEBUG or \ref
    NDEBUG can exist at a time. Specifying both or neither is considered a fatal
    compile error. This define is not supplied by Burgerlib, it must be defined
    by the build project.

    \note This flag is not used to determine if optimizations have been enabled.
    It's only used to disable the generation of debugging code.

    \code
    #if defined(NDEBUG)
        CrashGameIfDataNotFound();
    #endif
    \endcode

    \sa _DEBUG

***************************************/

/*! ************************************

    \def BURGER_CPP89
    \brief Define to determine if compiler has feature level C++89 (ANSI C)

    If this define exists, then you are creating code with a compiler that has a
    minimum feature set found in ANSI C++ compilers.

    \note Burgerlib requires this as a baseline. It will likely not compile
    using a compiler that isn't at least ANSI C89 level of compatibility.

    \sa BURGER_STDCPP_NAME, BURGER_CPP98, BURGER_CPP11, BURGER_CPP14,
        BURGER_CPP17, and BURGER_CPP20

***************************************/

/*! ************************************

    \def BURGER_CPP98
    \brief Define to determine if compiler has feature level C++98

    If this define exists, then you are creating code with a compiler that has a
    minimum feature set found in C++98 compilers.

    \sa BURGER_STDCPP_NAME, BURGER_CPP89, BURGER_CPP11, BURGER_CPP14,
        BURGER_CPP17, and BURGER_CPP20

***************************************/

/*! ************************************

    \def BURGER_CPP11
    \brief Define to determine if compiler has feature level C++11

    If this define exists, then you are creating code with a compiler that has a
    minimum feature set found in C++11 compilers.

    \sa BURGER_STDCPP_NAME, BURGER_CPP89, BURGER_CPP98, BURGER_CPP14,
        BURGER_CPP17, and BURGER_CPP20

***************************************/

/*! ************************************

    \def BURGER_CPP14
    \brief Define to determine if compiler has feature level C++14

    If this define exists, then you are creating code with a compiler that has a
    minimum feature set found in C++14 compilers.

    \sa BURGER_STDCPP_NAME, BURGER_CPP89, BURGER_CPP98, BURGER_CPP11,
        BURGER_CPP17, and BURGER_CPP20

***************************************/

/*! ************************************

    \def BURGER_CPP17
    \brief Define to determine if compiler has feature level C++17

    If this define exists, then you are creating code with a compiler that has a
    minimum feature set found in C++17 compilers.

    \sa BURGER_STDCPP_NAME, BURGER_CPP89, BURGER_CPP98, BURGER_CPP11,
        BURGER_CPP14, and BURGER_CPP20

***************************************/

/*! ************************************

    \def BURGER_CPP20
    \brief Define to determine if compiler has feature level C++20

    If this define exists, then you are creating code with a compiler that has a
    minimum feature set found in C++20 compilers.

    \sa BURGER_STDCPP_NAME, BURGER_CPP89, BURGER_CPP98, BURGER_CPP11,
        BURGER_CPP14, and BURGER_CPP17

***************************************/

/*! ************************************

    \def BURGER_STDCPP_NAME
    \brief String of the C++ feature level.

    This define is of a string naming the compiler feature level.

    \sa BURGER_CPP89, BURGER_CPP98, BURGER_CPP11, BURGER_CPP14, BURGER_CPP17, or
        BURGER_CPP20

***************************************/

/*! ************************************

    \def BURGER_WATCOM
    \brief Define to determine if code is being built using Open Watcom
        compilers.

    If this define exists, then you are creating code with the [Open Watcom
        compiler](http://www.openwatcom.com "Open Watcom Project")

    \sa BURGER_INTEL_COMPILER, BURGER_METROWERKS, BURGER_MSVC and BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_MRC
    \brief Define to determine if code is being built using Apple/Motorola MrC
        compiler.

    If this define exists, then you are creating code with the [MrC compiler]
    (https://staticky.com/mirrors/ftp.apple.com/developer/Tool_Chest/Core_Mac_OS_Tools/MPW_Website_Downloads/
    "MrC Compiler download page")


    \sa BURGER_GNUC, BURGER_METROWERKS, BURGER_APPLE_SC and BURGER_POWERPC

***************************************/

/*! ************************************

    \def BURGER_APPLE_SC
    \brief Define to determine if code is being built using Apple/Motorola SC
        compiler.

    If this define exists, then you are creating code with the [Apple SC
    compiler]
    (https://staticky.com/mirrors/ftp.apple.com/developer/Tool_Chest/Core_Mac_OS_Tools/MPW_Website_Downloads/
    "Apple SC Compiler download page")

    \sa BURGER_GNUC, BURGER_METROWERKS, BURGER_MRC and BURGER_68K

***************************************/

/*! ************************************

    \def BURGER_DJGPP
    \brief Define to determine if code is being built using DJGPP compiler.

    If this define exists, then you are creating code with the [DJGPP compiler]
    (http://www.delorie.com/djgpp/ "DJGPP Compiler home page")

    \sa BURGER_MSDOS, or BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_SNSYSTEMS
    \brief Define to determine if code is being built using Green Hills
        compiler.

    If this define exists, then you are creating code with the
    [SN Systems compiler](http://www.snsys.com/ "SN Systems home page")

    \sa BURGER_METROWERKS, BURGER_POWERPC, BURGER_MIPS32 and BURGER_AMD64

***************************************/

/*! ************************************

    \def BURGER_GHS
    \brief Define to determine if code is being built using SN Systems compiler.

    If this define exists, then you are creating code with the
    [Green Hills Software compiler](https://www.ghs.com/ "Green Hills Software
    home page")

    \sa BURGER_METROWERKS, BURGER_POWERPC, BURGER_WIIU and BURGER_WII

***************************************/

/*! ************************************

    \def BURGER_INTEL_COMPILER
    \brief Define to determine if code is being built using Intel compiler.

    If this define exists, then you are creating code with the [Intel
    compiler](https://software.intel.com/en-us/c-compilers "Intel Compiler Home
    page")

    \sa BURGER_MSVC, BURGER_METROWERKS, BURGER_AMD64 and BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_MINGW
    \brief Define to determine if code is being built using MinGW compilers.

    If this define exists, then you are creating code with the [MinGW compiler]
    (http://www.mingw.org/ "MinGW home page")

    \sa BURGER_WATCOM, BURGER_INTEL_COMPILER, BURGER_METROWERKS, BURGER_MSVC and
        BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_CLANG
    \brief Define to determine if code is being built using the clang compiler.

    If this define exists, then you are creating code with the [clang compiler]
    (https://clang.llvm.org/ "Clang home page")

    \sa BURGER_GNUC, BURGER_METROWERKS, BURGER_POWERPC, BURGER_ARM32,
        BURGER_AMD64 and BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_GNUC
    \brief Define to determine if code is being built using the GNU C family of
        compilers.

    If this define exists, then you are creating code with the [GNUC compiler]
    (https://gcc.gnu.org/ "GCC home page")

    \sa BURGER_CLANG, BURGER_METROWERKS, BURGER_POWERPC, BURGER_ARM32,
        BURGER_AMD64 and BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_METROWERKS
    \brief Define to determine if code is being built using Freescale/Metrowerks
        family of compilers.

    If this define exists, then you are creating code with the [Freescale /
    Metrowerks compiler]
    (https://www.nxp.com/support/developer-resources/software-development-tools/codewarrior-development-tools:CW_HOME)

    \sa BURGER_MRC, BURGER_POWERPC, BURGER_MIPS32 and BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_ARM_COMPILER
    \brief Define to determine if code is being built using
        Advanced RISC Machines family of compilers.

    If this define exists, then you are creating code with the [ARM compiler]
    (https://developer.arm.com/products/software-development-tools/compilers/arm-compiler")

    \sa BURGER_MRC, BURGER_POWERPC, BURGER_MIPS32 and BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_MSVC
    \brief Define to determine if code is being built using Microsoft Visual C
        family of compilers.

    If this define exists, then you are creating code with the [Microsoft
    Visual C compiler.](https://visualstudio.microsoft.com/)

    \sa BURGER_INTEL_COMPILER, BURGER_METROWERKS, BURGER_ARM32, BURGER_AMD64 and
        BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_COMPILER_NAME
    \brief String of the name of the C++ compiler.

    This define is of a string naming the compiler.

    \sa BURGER_COMPILER_VERSION, BURGER_INTEL_COMPILER, BURGER_MSVC,
        BURGER_WATCOM, BURGER_CLANG, BURGER_GNUC, or BURGER_METROWERKS

***************************************/

/*! ************************************

    \def BURGER_COMPILER_VERSION
    \brief Integer of the version of the current compiler.

    This define is of an integer for the version of the compiler.

    The value is compiler specific, and must be checked for the specific
    compiler before testing this number against a constant.

    \sa BURGER_COMPILER_NAME, BURGER_INTEL_COMPILER, BURGER_MSVC,
        BURGER_WATCOM, BURGER_CLANG, BURGER_GNUC, or BURGER_METROWERKS

***************************************/

/*! ************************************

    \def __has_builtin
    \brief Clang feature macro.

    On non-clang compilers, this compiles to 0.

    \sa BURGER_CLANG, __has_feature, __has_extension, __has_cpp_attribute, or
        __has_declspec_attribute

***************************************/

/*! ************************************

    \def __has_feature
    \brief Clang feature macro.

    On non-clang compilers, this compiles to 0.

    \sa BURGER_CLANG, __has_builtin, __has_extension, __has_cpp_attribute, or
        __has_declspec_attribute

***************************************/

/*! ************************************

    \def __has_extension
    \brief Clang feature macro.

    On non-clang compilers, this compiles to 0.

    \sa BURGER_CLANG, __has_builtin, __has_feature, __has_cpp_attribute, or
        __has_declspec_attribute

***************************************/

/*! ************************************

    \def __has_cpp_attribute
    \brief Clang feature macro.

    On non-clang compilers, this compiles to 0.

    \sa BURGER_CLANG, __has_builtin, __has_feature, __has_extension, or
        __has_declspec_attribute

***************************************/

/*! ************************************

    \def __has_declspec_attribute
    \brief Clang feature macro.

    On non-clang compilers, this compiles to 0.

    \sa BURGER_CLANG, __has_builtin, __has_feature, __has_extension, or
        __has_cpp_attribute

***************************************/

/*! ************************************

    \def BURGER_X86
    \brief Define to determine if code is being built for 32 bit Intel
        processors.

    If this define exists, then you are creating code that runs on the Intel,
    AMD and other compatible processors. The Microsoft XBox, Mac OSX Intel and
    Win32 platforms which are running on Pentium, 386, 486, and AMD CPUs will
    have this define present.

    \sa BURGER_CPU_NAME, BURGER_INTEL, BURGER_AMD64, BURGER_WIN32, BURGER_BEOS,
        BURGER_MSDOS, BURGER_MACOSX or BURGER_XBOX

***************************************/

/*! ************************************

    \def BURGER_AMD64
    \brief Define to determine if code is being built for AMD64 (Intel 64 bit)
        compatible processors.

    If this define exists, then you are creating code that runs on the AMD 64
    and other compatible processors. The Microsoft Win64 and Mac OSX 64
    platforms which are running on Pentium and AMD CPUs in 64 bit mode will have
    this define present.

    \sa BURGER_CPU_NAME, BURGER_INTEL, BURGER_WIN64, BURGER_MACOSX,
        BURGER_XBOXONE, BURGER_PS4 or BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_ITANIUM
    \brief Define to determine if code is being built for Intel Itanium
        compatible processors.

    If this define exists, then you are creating code that runs on the Intel
    Itanium compatible processors. The Microsoft Win64 and Linux which are
    running on Itanium compatible processors will have this define present.

    \sa BURGER_CPU_NAME, BURGER_WIN64 or BURGER_LINUX

***************************************/

/*! ************************************

    \def BURGER_ARM32
    \brief Define to determine if code is being built for 32 bit Advanced RISC
        Machine processors.

    If this define exists, then you are creating code that runs on the Advanced
    RISC Machines 32 bit line of processors. The Gameboy Advanced, Nintendo DS,
    Nokia NGage, Apple iPad/iPhone/iPod and certain cell phones will have this
    define present.

    \sa BURGER_CPU_NAME, BURGER_ARM64, BURGER_ARM, BURGER_GBA, BURGER_ANDROID,
        BURGER_SHIELD, BURGER_OUYA, BURGER_DS, BURGER_3DS, BURGER_IOS,
        BURGER_NGAGE, or BURGER_SYMBIAN

***************************************/

/*! ************************************

    \def BURGER_ARM64
    \brief Define to determine if code is being built for 64 bit Advanced RISC
        Machine processors.

    If this define exists, then you are creating code that runs on the Advanced
    RISC Machines 64 bit line of processors. Apple iPad/iPhone/iPod and certain
    cell phones will have this define present.

    \sa BURGER_CPU_NAME, BURGER_ARM32, BURGER_ARM, BURGER_ANDROID or BURGER_IOS

***************************************/

/*! ************************************

    \def BURGER_MIPS32
    \brief Define to determine if code is being built for MIPS 32 bit
        processors.

    If this define exists, then you are creating code that runs on the MIPS 32
    bit line of processors. R5900 (PS2), R3300 (PS1) and R4400 (PSP) all will
    have this define present.

    \sa BURGER_CPU_NAME, BURGER_MIPS64, BURGER_PS2, BURGER_PSP, BURGER_PS1 or
        BURGER_MIPS

***************************************/

/*! ************************************

    \def BURGER_MIPS64
    \brief Define to determine if code is being built for MIPS 64 bit
        processors.

    If this define exists, then you are creating code that runs on the MIPS 64
    bit line of 64 bit processors.

    \sa BURGER_CPU_NAME, BURGER_MIPS32, BURGER_MIPS

***************************************/

/*! ************************************

    \def BURGER_POWERPC
    \brief Define to determine if code is being built for 32 bit PowerPC
        processors.

    If this define exists, then you are creating code that runs on a 32 bit
    PowerPC processor. The Nintendo GameCube, Power Macintosh and the Nintendo
    Wii all will have this define present.

    \sa BURGER_CPU_NAME, BURGER_PPC, BURGER_WIIU, BURGER_WII, BURGER_MAC,
        BURGER_MACOSX, BURGER_BEOS or BURGER_GAMECUBE

***************************************/

/*! ************************************

    \def BURGER_POWERPC64
    \brief Define to determine if code is being built for 64 bit PowerPC
        processors.

    If this define exists, then you are creating code that runs on a 64 bit
    PowerPC processor. The G5 Power Macintosh, Sony Playstation 3 and Microsoft
    XBox 360 all will have this define present.

    \sa BURGER_CPU_NAME, BURGER_PPC, BURGER_XBOX360, BURGER_PS3 or BURGER_MACOSX

***************************************/

/*! ************************************

    \def BURGER_68K
    \brief Define to determine if code is being built for Motorola 680x0
        processors.

    If this define exists, then you are creating code that runs on the Motorola
    680x0 line of processors. The classic Macintosh and certain PDAs will have
    this define present.

    \sa BURGER_CPU_NAME, BURGER_68881, BURGER_MAC or BURGER_CFM

***************************************/

/*! ************************************

    \def BURGER_RISCV
    \brief Define to determine if code is being built for 32 bit RISC-V
    processors.

    If this define exists, then you are creating code that runs on the RISC-V
    line of processors. Micro controllers will have this define present.

    \sa BURGER_CPU_NAME, BURGER_MSP430, BURGER_ARM64, BURGER_ARM32 or BURGER_AVR

***************************************/

/*! ************************************

    \def BURGER_AVR
    \brief Define to determine if code is being built for 32 bit Atmel AVR
    processors.

    If this define exists, then you are creating code that runs on the Atmel
    AVR line of processors. Micro controllers will have this define present.

    \sa BURGER_CPU_NAME, BURGER_ARM64, BURGER_ARM32 or BURGER_RISCV

***************************************/

/*! ************************************

    \def BURGER_MSP430
    \brief Define to determine if code is being built for 16 bit MSP430
    processors from Texas Instruments.

    If this define exists, then you are creating code that runs on the MSP430
    line of 16 bit processors. Micro controllers will have this define present.

    \sa BURGER_CPU_NAME, BURGER_RISCV, BURGER_ARM32 or BURGER_AVR

***************************************/

/*! ************************************

    \def BURGER_SPARC32
    \brief Define to determine if code is being built for 32 bit Sparc
    processors from Sun.

    If this define exists, then you are creating code that runs on the Sparc
    line of 32 bit processors. Sun servers will have this define present.

    \sa BURGER_CPU_NAME, BURGER_LINUX, or BURGER_SPARC64

***************************************/

/*! ************************************

    \def BURGER_SPARC64
    \brief Define to determine if code is being built for 64 bit Sparc
    processors from Sun.

    If this define exists, then you are creating code that runs on the Sparc
    line of 64 bit processors. Sun servers will have this define present.

    \sa BURGER_CPU_NAME, BURGER_LINUX, or BURGER_SPARC32

***************************************/

/*! ************************************

    \def BURGER_CPU_NAME
    \brief String of the name of the CPU.

    "C" string of the name of the CPU.

    \sa BURGER_X86, BURGER_AMD64, BURGER_ARM32, or BURGER_SPARC32

***************************************/

/*! ************************************

    \def BURGER_INTEL
    \brief Define to determine if the CPU is either X86 or AMD64

    On \ref BURGER_AMD64 or \ref BURGER_X86 CPUs, this define will also be
    present to allow Intel Architecture instructions. Some instructions may or
    may not be present, so call \ref Burger::CPUID(CPUID_t *) to test for all
    features and determine which functions to use for performance.

    \sa BURGER_XBOXONE, BURGER_PS4, BURGER_WINDOWS, BURGER_MACOSX, BURGER_X86,
        or BURGER_AMD64

***************************************/

/*! ************************************

    \def BURGER_PPC
    \brief Define to determine if code is being built for PowerPC processors.

    If this define exists, then you are creating code that runs on the PowerPC
    line of processors. The Nintendo GameCube, Power Macintosh, Nintendo Wii,
    Sony Playstation 3 and Microsoft XBox 360 all will have this define present.

    \sa BURGER_POWERPC, BURGER_POWERPC64, BURGER_XBOX360,
        BURGER_WII, BURGER_PS3, BURGER_MAC, BURGER_MACOSX, BURGER_BEOS or
        BURGER_GAMECUBE

***************************************/

/*! ************************************

    \def BURGER_ARM
    \brief Define to determine if code is being built for Advanced RISC Machine
        processors.

    If this define exists, then you are creating code that runs on the Advanced
    RISC Machines line of processors.

    \sa BURGER_ARM32 or BURGER_ARM64

***************************************/

/*! ************************************

    \def BURGER_MIPS
    \brief Define to determine if code is being built for MIPS
        processors.

    If this define exists, then you are creating code that runs on the MIPS line
    of processors.

    \sa BURGER_MIPS32 or BURGER_MIPS64

***************************************/

/*! ************************************

    \def BURGER_SPARC
    \brief Define to determine if code is being built for SPARC
        processors.

    If this define exists, then you are creating code that runs on the SPARC
line of processors.

    \sa BURGER_SPARC32 or BURGER_SPARC64

***************************************/

/*! ************************************

    \def BURGER_64BITCPU
    \brief Define to determine if the CPU has 64 bit integer registers.

    On \ref BURGER_XBOXONE, \ref BURGER_PS4, \ref BURGER_AMD64, \ref BURGER_PS3
    and \ref BURGER_XBOX360 platforms, the CPU has integer registers that are
    64-bits wide. Using this define, code can be written that takes advantage of
    this. Mostly useful with \ref Fixed32 and other high precision fixed point
    calculations.

    \sa BURGER_XBOXONE, BURGER_PS4, BURGER_PS3, BURGER_XBOX360, or BURGER_AMD64

***************************************/

/*! ************************************

    \def BURGER_NEON

    \brief Define to determine if code is being built for Advanced RISC Machines
        NEON instruction set extensions.

    If this define exists, then you are creating code that runs on the ARM line
    of processors with NEON instructions.

    \sa BURGER_ARM, BURGER_ARM32, or BURGER_ARM64

***************************************/

/*! ************************************

    \def BURGER_68881

    \brief Define to determine if code is being built for Motorola 680x0
        processor with a 68881 floating point unit.

    If this define exists, then you are creating code that runs on the Motorola
    680x0 line of processors with floating point hardware.

    \sa BURGER_68K

***************************************/

/*! ************************************

    \def BURGER_ALTIVEC

    \brief Define to determine if code is being built for PowerPC
        Altivec instruction set extensions.

    If this define exists, then you are creating code that runs on the PowerPC
    line of processors with Altivec instructions.

    \sa BURGER_PPC, BURGER_POWERPC, or BURGER_POWERPC64

***************************************/

/*! ************************************

    \def BURGER_3DNOW

    \brief Define to determine if code is being built for AMD
        3D Now! instruction set extensions.

    If this define exists, then you are creating code that runs on the AMD
    line of processors with 3D Now! instructions.

    [Click here to read the 3D Now! instruction set](3dnow.pdf)

    \sa BURGER_SSE, BURGER_SSE2, BURGER_AVX, BURGER_AVX2, BURGER_INTEL,
        BURGER_X86, or BURGER_AMD64

***************************************/

/*! ************************************

    \def BURGER_SSE

    \brief Define to determine if code is being built for SSE instruction set
        extensions.

    If this define exists, then you are creating code that runs a processor that
        has SSE instructions.

    [Click here to read the SSE through AVX2 instruction set](avx.pdf)

    \sa BURGER_3DNOW, BURGER_SSE2, BURGER_AVX, BURGER_AVX2, BURGER_INTEL,
        BURGER_X86, or BURGER_AMD64

***************************************/

/*! ************************************

    \def BURGER_SSE2

    \brief Define to determine if code is being built for SSE2 instruction set
        extensions.

    If this define exists, then you are creating code that runs a processor that
        has SSE2 instructions.

    [Click here to read the SSE through AVX2 instruction set](avx.pdf)

    \sa BURGER_3DNOW, BURGER_SSE, BURGER_AVX, BURGER_AVX2, BURGER_INTEL,
        BURGER_X86, or BURGER_AMD64

***************************************/

/*! ************************************

    \def BURGER_AVX

    \brief Define to determine if code is being built for AVX instruction set
        extensions.

    If this define exists, then you are creating code that runs a processor that
        has AVX instructions.

    [Click here to read the SSE through AVX2 instruction set](avx.pdf)

    \sa BURGER_3DNOW, BURGER_SSE, BURGER_SSE2, BURGER_AVX2, BURGER_INTEL,
        BURGER_X86, or BURGER_AMD64

***************************************/

/*! ************************************

    \def BURGER_AVX2

    \brief Define to determine if code is being built for AVX2 instruction set
        extensions.

    If this define exists, then you are creating code that runs a processor that
        has AVX2 instructions.

    [Click here to read the SSE through AVX2 instruction set](avx.pdf)

    \sa BURGER_3DNOW, BURGER_SSE, BURGER_SSE2, BURGER_AVX, BURGER_INTEL,
        BURGER_X86, or BURGER_AMD64

***************************************/

/*! ************************************

    \def BURGER_LITTLEENDIAN
    \brief Define to flag code running on a little endian machine.

    This define or \ref BURGER_BIGENDIAN will be present to allow the programmer
    to create code that can properly parse endian specific data. Only one macro
    will be defined.

    If this macro is present, the machine the code is being built for is little
    endian.

    \sa BURGER_BIGENDIAN, BURGER_ENDIANINDEX_LOW or BURGER_ENDIANINDEX_HIGH

***************************************/

/*! ************************************

    \def BURGER_BIGENDIAN
    \brief Define to flag code running on a big endian machine.

    This define or \ref BURGER_LITTLEENDIAN will be present to allow the
    programmer to create code that can properly parse endian specific data. Only
    one macro will be defined.

    If this macro is present, the machine the code is being built for is big
    endian.

    \sa BURGER_LITTLEENDIAN, BURGER_ENDIANINDEX_LOW or BURGER_ENDIANINDEX_HIGH

***************************************/

/*! ************************************

    \def BURGER_ENDIANINDEX_LOW
    \brief Index to the "low" word of a 64 bit value as a 32 value

    On little endian machines, this value is 0, otherwise it's 1.

    \sa BURGER_ENDIANINDEX_HIGH, BURGER_LITTLEENDIAN or BURGER_BIGENDIAN

***************************************/

/*! ************************************

    \def BURGER_ENDIANINDEX_HIGH
    \brief Index to the "high" word of a 64 bit value as a 32 value

    On little endian machines, this value is 1, otherwise it's 0.

    \sa BURGER_ENDIANINDEX_LOW, BURGER_LITTLEENDIAN or BURGER_BIGENDIAN

***************************************/

/*! ************************************

    \def BURGER_WIN32
    \brief Define to determine if code is being built for Microsoft 32 bit
        Windows.

    If this define exists, then you are creating code that runs on Microsoft
    Windows for 32 bit CPUs.

    \note You may need to check the processor define, since future versions of
    Windows may be running on non-Pentium type processors.

    \sa BURGER_WINDOWS,BURGER_WIN64, BURGER_MSDOS and BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_WIN64
    \brief Define to determine if code is being built for Microsoft 64 bit
        Windows.

    If this define exists, then you are creating code that runs on Microsoft
    Windows for 64 bit CPUs.

    \note You may need to check the processor define, since future versions of
    Windows may be running on non-Pentium type processors.

    \sa BURGER_WINDOWS, BURGER_WIN32, BURGER_MSDOS and BURGER_AMD64

***************************************/

/*! ************************************

    \def BURGER_WINDOWS
    \brief Define to determine if code is being built for Microsoft 32 or 64 bit
        Windows.

    If this define exists, then you are creating code that runs on Microsoft
    Windows for either 32 or 64 bit CPUs.

    \note You may need to check the processor define, since future versions of
    Windows may be running on non-Pentium type processors.

    \sa BURGER_WIN32, BURGER_WIN64, BURGER_MSDOS and BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_MSDOS
    \brief Define to determine if code is being built for Microsoft MS-DOS.

    If this define exists, then you are creating code that runs under classic
    MS-DOS.

    Check for the defines BURGER_X32 or BURGER_DOS4G if you wish to determine
    which dos extender you are using.

    \sa BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_X32
    \brief Define to determine if MS-DOS code is being built with the X32 DOS
        extender.

    If this define exists, then you are creating code that runs under classic
    MS-DOS with the X32 DOS extender.

    This define is declared by the project, not by Burgerlib. Either BURGER_X32
    or BURGER_DOS4G must be present.

    \sa BURGER_MSDOS or BURGER_DOS4G

***************************************/

/*! ************************************

    \def BURGER_DOS4G
    \brief Define to determine if MS-DOS code is being built with the DOS4G DOS
        extender.

    If this define exists, then you are creating code that runs under classic
    MS-DOS with the DOS4G DOS extender.

    This define is declared by the project, not by Burgerlib. Either
    BURGER_DOS4G or BURGER_X32 must be present.

    \sa BURGER_MSDOS or BURGER_X32

***************************************/

/*! ************************************

    \def BURGER_MAC
    \brief Define to determine if code is being built for Apple Mac OS 8.6
        through 9.2.

    If this define exists, then you are creating code that runs under Apple
    MacOS version 8.6 through 9.2. Since MacOS runs on the 680x0 or the PowerPC
    architectures, you may need to check \ref BURGER_68K or \ref BURGER_POWERPC
    for any CPU specific code or features. Also, on 680x0 platforms, you may be
    compiling under the Code Fragment Manager format, and to detect that, check
    for the presence of \ref BURGER_CFM.

    \note To compile for Carbon under a PowerPC CPU, your project must define
    \ref TARGET_API_MAC_CARBON so the MacOS headers will properly be adjusted
    for Carbon. If you are using Burgerlib for Carbon, you have access to MacOS
    X features if your application is running on a MacOS X system.

    \code
    // Include this define in your CodeWarrior C/C++ Preprocessor preferences
    // settings to build for MacOS 9 Carbon.

    #define TARGET_API_MAC_CARBON 1
    \endcode

    \sa BURGER_MACOS, BURGER_MACOSX, BURGER_MACCLASSIC, BURGER_MACCARBON,
        BURGER_68K, BURGER_POWERPC or TARGET_API_MAC_CARBON

***************************************/

/*! ************************************

    \def BURGER_MACOSX
    \brief Define to determine if code is being built exclusively for Apple Mac
        OS X.

    If this define exists, then you are creating code that runs under Apple
    MacOS X using Mach-O. There is no support for pre-MacOS X operating systems
    under this build target. The CPU can be either \ref BURGER_POWERPC, \ref
    BURGER_AMD64 or \ref BURGER_X86 for this platform, so no CPU specific code
    should be written without checking for the CPU type.

    \sa BURGER_POWERPC, BURGER_AMD64, BURGER_X86, BURGER_MACOS or BURGER_MAC

***************************************/

/*! ************************************

    \def BURGER_MACCARBON
    \brief Define to determine if code is being built for both Apple Mac OS9 and
        MacOS X.

    If this define exists, then you are creating code that runs under Apple
    MacOS 9 and for Apple MacOS X. Only the PowerPC is supported. Since MacOS
    Carbon support is enabled, accessing the low memory operating system
    variables is <b>NOT</b> allowed.

    \sa BURGER_POWERPC, BURGER_MACOSX, BURGER_MACOS or BURGER_MAC

***************************************/

/*! ************************************

    \def BURGER_MACCLASSIC
    \brief Define to determine if code is being built exclusively for Apple Mac
        OS9.

    If this define exists, then you are creating code that runs under Apple
    MacOS 9 for either PowerPC or 68K. There is no support for MacOS X operating
    systems under this build target. The CPU can be either \ref BURGER_POWERPC
    or \ref BURGER_68K for this platform, so no CPU specific code should be
    written without checking for the CPU type. Since no MacOS Carbon support is
    enabled, accessing the low memory operating system variables is allowed.

    \sa BURGER_POWERPC, BURGER_68K, BURGER_MACOS or BURGER_MAC

***************************************/

/*! ************************************

    \def BURGER_MACOS
    \brief Define to determine if code is being built for any Apple Mac OS
        platform.

    If this define exists, then you are creating code that runs under Apple
    MacOS. Currently, 68K, PowerPC and Intel are supported. Since it's not known
    which version of MacOS is currently being targeted, accessing the low memory
    operating system variables is <b>NOT</b> allowed.

    \sa BURGER_POWERPC, BURGER_X86, BURGER_68K, BURGER_MACOSX,
        BURGER_MACCLASSIC, BURGER_MACCARBON or BURGER_MAC

***************************************/

/*! ************************************

    \def TARGET_API_MAC_CARBON
    \brief Define to determine if Mac OS 9 code is being built with the Carbon
        libraries.

    If this define exists, then you are creating code that runs under classic
    Mac OS with the Carbon libraries.

    This define is declared by the project, not by Burgerlib. If this define is
    missing, the code will default to classic MacOS 8.6 - 9.2.

    \sa BURGER_MAC, BURGER_CFM or BURGER_MACOSX

***************************************/

/*! ************************************

    \def BURGER_CFM
    \brief Define to determine if Mac OS 9 code is being built in Code Fragment
        Manger format.

    If this define exists, then you are creating code in which the final binary
    is in PEF format for use with the Code Fragment Manager. All Power PC code
    for Classic or Carbon is in this format and 68k also can be compiled this
    way.

    \note To be able to use DrawSprocket in 68k code, you must build your
    application using CFM. The classic format can't link to DrawSprocket 68k.

    \note Power PC code can be built in Mach-O format, but if that is the case,
    then the code can only run in Mac OS X.

    \sa BURGER_MAC, BURGER_68K or BURGER_MACOSX

***************************************/

/*! ************************************

    \def BURGER_IOS
    \brief Define to determine if code is being built for Apple iOS devices.

    If this define exists, then you are creating code that runs on Apple iOS
    devices.

    \sa BURGER_ANDROID, BURGER_ARM32 or BURGER_MACOSX

***************************************/

/*! ************************************

    \def BURGER_ANDROID
    \brief Define to determine if code is being built for Google Android
        devices.

    If this define exists, then you are creating code that runs on Google
    Android devices.

    \sa BURGER_IOS, BURGER_SHIELD, BURGER_OUYA, BURGER_ARM32 or BURGER_MACOSX

***************************************/

/*! ************************************

    \def BURGER_SHIELD
    \brief Define to determine if code is being built for nVidia SHIELD devices.

    If this define exists, then you are creating code that runs on nVidia SHIELD
    Android devices.

    \sa BURGER_IOS, BURGER_ANDROID, BURGER_OUYA, BURGER_ARM32 or BURGER_MACOSX

***************************************/

/*! ************************************

    \def BURGER_OUYA
    \brief Define to determine if code is being built for the OUYA.

    If this define exists, then you are creating code that runs on OUYA Android
    devices.

    \sa BURGER_IOS, BURGER_ANDROID, BURGER_SHIELD, BURGER_ARM32 or BURGER_MACOSX

***************************************/

/*! ************************************

    \def BURGER_NGAGE
    \brief Define to determine if code is being built for the Nokia Ngage.

    If this define exists, then you are creating code that runs on the Nokia
    NGage.

    \note There is no floating point support on this platform.

    \sa BURGER_ARM32, or BURGER_SYMBIAN

***************************************/

/*! ************************************

    \def BURGER_SYMBIAN
    \brief Define to determine if code is being built for the Symbian OS.

    If this define exists, then you are creating code that runs under the
    Symbian OS, usually Nokia brand cell phones and PDAs.

    \note There is no floating point support on this platform.

    \sa BURGER_ARM32, or BURGER_NGAGE

***************************************/

/*! ************************************

    \def BURGER_PS1
    \brief Define to determine if code is being built for the Sony Playstation.

    If this define exists, then you are creating code that runs on the Sony
    Playstation.

    \note There is no floating point support on this platform.

    \sa BURGER_MIPS32, BURGER_PS2, BURGER_PS3, or BURGER_PS4

***************************************/

/*! ************************************

    \def BURGER_PS2
    \brief Define to determine if code is being built for the Emotion Engine CPU
        for a PS2.

    If this define exists, then you are creating code that runs on the main
    processor for a Sony Playstation 2.

    \sa BURGER_MIPS32, BURGER_PS1, BURGER_PS3, or BURGER_PS4

***************************************/

/*! ************************************

    \def BURGER_PS3
    \brief Define to determine if code is being built for the PS3.

    If this define exists, then you are creating code that runs on the main
    processor for a Sony Playstation 3.

    \note This define is only present for compiling code on the primary CPU.
    Code targeted for the cell units must be written and compiled separately.

    \sa BURGER_POWERPC, BURGER_PS1, BURGER_PS2, or BURGER_PS4

***************************************/

/*! ************************************

    \def BURGER_PS4
    \brief Define to determine if code is being built for the PS4.

    If this define exists, then you are creating code that runs on the main
    processor for a Sony Playstation 4.

    \sa BURGER_AMD64, BURGER_PS1, BURGER_PS2, or BURGER_PS3

***************************************/

/*! ************************************

    \def BURGER_PSP
    \brief Define to determine if code is being built for the Sony PSP.

    If this define exists, then you are creating code that runs on the Sony
    Playstation Portable (PSP).

    \sa BURGER_MIPS32, BURGER_VITA, BURGER_PS1, BURGER_PS2, BURGER_PS3, or
        BURGER_PS4

***************************************/

/*! ************************************

    \def BURGER_VITA
    \brief Define to determine if code is being built for the Sony VITA.

    If this define exists, then you are creating code that runs on the Sony
    Playstation Portable 2 (PSP2 / Vita).

    \sa BURGER_ARM32, BURGER_PSP, BURGER_PS1, BURGER_PS2, BURGER_PS3, or
        BURGER_PS4

***************************************/

/*! ************************************

    \def BURGER_GAMECUBE
    \brief Define to determine if code is being built for the Nintendo GameCube.

    If this define exists, then you are creating code that runs on the Gekko
    processor for a Nintendo GameCube.

    \sa BURGER_POWERPC, BURGER_WII or BURGER_WIIU

***************************************/

/*! ************************************

    \def BURGER_WII
    \brief Define to determine if code is being built for the Nintendo Wii.

    If this define exists, then you are creating code that runs on the Broadway
    processor for a Nintendo Wii.

    \sa BURGER_SWITCH, BURGER_POWERPC, BURGER_WIIU or BURGER_GAMECUBE

***************************************/

/*! ************************************

    \def BURGER_WIIU
    \brief Define to determine if code is being built for the Nintendo WiiU.

    If this define exists, then you are creating code that runs on the Broadway
    processor for a Nintendo WiiU.

    \sa BURGER_SWITCH, BURGER_POWERPC, BURGER_WII or BURGER_GAMECUBE

***************************************/

/*! ************************************

    \def BURGER_SWITCH
    \brief Define to determine if code is being built for the Nintendo Switch.

    If this define exists, then you are creating code that runs on the Arm64
    processor for a Nintendo Switch.

    \sa BURGER_ARM64, BURGER_WII, BURGER_WII or BURGER_3DS

***************************************/

/*! ************************************

    \def BURGER_GBA
    \brief Define to determine if code is being built for the Nintendo Gameboy
    Advanced.

    If this define exists, then you are creating code that runs on the Nintendo
    Gameboy Advanced.

    \note In addition to defining \ref NDEBUG or \ref _DEBUG, you need to define
    SDK_DEBUG in your project if you wish for the Nintendo GBA SDK to generate
    debugging information

    \sa BURGER_ARM32, BURGER_DS or BURGER_3DS

***************************************/

/*! ************************************

    \def BURGER_DS
    \brief Define to determine if code is being built for the Nintendo DS.

    If this define exists, then you are creating code that runs on the Nintendo
    DS.

    \note In addition to defining \ref NDEBUG or \ref _DEBUG, you need to define
    SDK_DEBUG in your project if you wish for the Nintendo DS SDK to generate
    debugging information

    \sa BURGER_ARM32, BURGER_3DS or BURGER_GBA

***************************************/

/*! ************************************

    \def BURGER_3DS
    \brief Define to determine if code is being built for the Nintendo 3DS.

    If this define exists, then you are creating code that runs on the Nintendo
    3DS.

    \note In addition to defining \ref NDEBUG or \ref _DEBUG, you need to define
    SDK_DEBUG in your project if you wish for the Nintendo 3DS SDK to generate
    debugging information

    \sa BURGER_ARM32, BURGER_DS or BURGER_GBA

***************************************/

/*! ************************************

    \def BURGER_XBOX
    \brief Define to determine if code is being built for the Microsoft XBox.

    If this define exists, then you are creating code that runs on the Intel
    Celeron for a Microsoft XBox.

    \sa BURGER_X86, BURGER_XBOX360, or BURGER_XBOXONE

***************************************/

/*! ************************************

    \def BURGER_XBOX360
    \brief Define to determine if code is being built for the Microsoft XBox
        360.

    If this define exists, then you are creating code that runs on the Microsoft
    XBox360.

    \note You can use Burgerlib functions from any thread, but caution should be
    exercised to prevent thread stalls and multi threading problems. Burgerlib's
    higher level classes may not be reentrant. See the documentation for higher
    level classes to determine multi-threading safety.

    \sa BURGER_POWERPC, BURGER_XBOX, or BURGER_XBOXONE

***************************************/

/*! ************************************

    \def BURGER_XBOXONE
    \brief Define to determine if code is being built for the Microsoft XBox
        One.

    If this define exists, then you are creating code that runs on the Microsoft
    XBox One.

    \note You can use Burgerlib functions from any thread, but caution should be
    exercised to prevent thread stalls and multi threading problems. Burgerlib's
    higher level classes may not be reentrant. See the documentation for higher
    level classes to determine multi-threading safety.

    \sa BURGER_AMD64, BURGER_XBOX, or BURGER_XBOX360

***************************************/

/*! ************************************

    \def BURGER_BEOS
    \brief Define to determine if code is being built for Be OS.

    If this define exists, then you are creating code that runs on Be OS for
    Intel or PowerPC.

    \sa BURGER_POWERPC or BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_AMIGA
    \brief Define to determine if code is being built for the Commodore Amiga.

    If this define exists, then you are creating code that runs on the Amiga for
    68K or PowerPC.

    \sa BURGER_POWERPC or BURGER_68K

***************************************/

/*! ************************************

    \def BURGER_LINUX
    \brief Define to determine if code is being built for Linux.

    If this define exists, then you are creating code that runs on Linux.

    \sa BURGER_POWERPC, BURGER_AMD64 or BURGER_X86

***************************************/

/*! ************************************

    \def BURGER_PLATFORM_NAME
    \brief String with the operating system's name.

    This string is of the operating system the code is being compiled for.

    \sa BURGER_WINDOWS, BURGER_LINUX or BURGER_MACOS

***************************************/

/*! ************************************

    \def TRUE
    \brief Define of the number 1 for boolean operations.

    \note Since this a common \#define, it will first check if it's already
    present before defining.

    \sa FALSE or NULL

***************************************/

/*! ************************************

    \def FALSE
    \brief Define of the number 0 for boolean operations.

    \note Since this a common \#define, it will first check if it's already
    present before defining.

    \sa TRUE or NULL

***************************************/

/*! ************************************

    \def NULL
    \brief Define of the number 0 for pointer invalidation.

    \note Since this a common \#define, it will first check if it's already
    present before defining.

    \sa FALSE or TRUE

***************************************/

/*! ************************************

    \def BURGER_MININT
    \brief Minimum value of a signed integer.

    This is a replacement for the ANSI macro MIN_INT. Normally, this is
    (-0x7FFFFFFF)-1, but it could be a 64 or 128 bit value on future processors.

    \sa BURGER_MININT64 or BURGER_MAXINT

***************************************/

/*! ************************************

    \def BURGER_MAXINT
    \brief Maximum value of a signed integer.

    This is a replacement for the ANSI macro MAX_INT. Normally, this is
    0x7FFFFFFF, but it could be a 64 or 128 bit value on future processors.

    \sa BURGER_MININT, BURGER_MAXINTPTR or BURGER_MAXUINT

***************************************/

/*! ************************************

    \def BURGER_MAXUINT
    \brief Maximum value of an unsigned integer.

    This is a replacement for the ANSI macro MAX_UINT. Normally, this is
    0xFFFFFFFFU, but it could be a 64 or 128 bit value on future processors.

    \sa BURGER_MAXINTPTR or BURGER_MAXINT

***************************************/

/*! ************************************

    \def BURGER_MININT64
    \brief Minimum value of a \ref Int64.

    This is a replacement for the ANSI macro MIN_INT but it's meant for 64 bit
    values defined as \ref Int64. Normally, this is ((-0x7FFFFFFFFFFFFFFFLL)-1).

    \sa BURGER_MAXINT64, BURGER_MAXUINT64, or BURGER_MAXINT

***************************************/

/*! ************************************

    \def BURGER_MAXINT64
    \brief Maximum value of a \ref Int64.

    This is a replacement for the ANSI macro MAX_INT but it's meant for 64 bit
    values defined as \ref Int64. Normally, this is 0x7FFFFFFFFFFFFFFFLL.

    \sa BURGER_MININT64, BURGER_MAXUINT64, or BURGER_MAXINT

***************************************/

/*! ************************************

    \def BURGER_MAXUINT64
    \brief Maximum value of a \ref Word64.

    This is a replacement for the ANSI macro MAX_UINT but it's meant for 64 bit
    values defined as \ref Word64. Normally, this is 0xFFFFFFFFFFFFFFFFULL.

    \sa BURGER_MAXINT64, or BURGER_MAXUINT

***************************************/

/*! ************************************

    \def BURGER_MAXINTPTR
    \brief Maximum value of a signed address space integer.

    This is a constant value of the largest allowable address. Normally, this is
    0x7FFFFFFF on 32 bit CPUs and 0x7FFFFFFFFFFFFFFFLL on 64 bit CPUs. It may be
    a 128 bit value on future processors.

    \sa BURGER_MAXINT or BURGER_MAXUINT

***************************************/

/*! ************************************

    \def BURGER_INLINE
    \brief Define to create inline code.

    Since pre-C++ compilers created different keywords to force inlining of a
    function, this define standardizes the keyword. Normally, it will evaluate
    to inline, but it can become `__inline` or `__forceinline` or some other
    token that is compiler specific.

    In all cases, the behavior of the C++ inline keyword is what is expected.

    \sa BURGER_NO_INLINE

***************************************/

/*! ************************************

    \def BURGER_NO_INLINE
    \brief Define to disable inline code.

    Since pre-C++ compilers created different keywords to disable inlining of a
    function, this define standardizes the keyword. Normally, it will evaluate
    to `__declspec(noinline)`, but it can become `__attribute__((noinline))`
    or some other token that is compiler specific.

    Some compilers don't support this keyword, for those compilers, this define
    resolves into nothing.

    \sa BURGER_INLINE

***************************************/

/*! ************************************

    \def BURGER_API
    \brief Default parameter passing type for Burgerlib functions.

    On Intel platforms, there are several methods of passing parameters.
    Burgerlib uses the `__fastcall` method and this define will evaluate to
    `__fastcall` on compilers that support it. Otherwise, this will evaluate
    to nothing on non-Intel platforms.

    For non Intel platforms, this resolves into a blank \#define.

    \sa BURGER_ANSIAPI or BURGER_FASTCALLENABLED

***************************************/

/*! ************************************

    \def BURGER_ANSIAPI
    \brief Default parameter passing type for ANSI library functions.

    On Intel platforms, there are several methods of passing parameters. ANSI
    uses the __cdecl keyword, so any ANSI function like qsort() or atexit() that
    needs a callback function should have this keyword before the user supplied
    callback. It will ensure that the linkage will be correct. This will
    evaluate to nothing on non-Intel platforms.

    \code
    static int BURGER_ANSIAPI Compare(const void *a,const void *b)
    {
        return strcmp((char *)a,(char *)b);
    }

    void SortData(void)
    {
        qsort(array,arraycount,sizeof(array[0]),Compare);
    }

    \endcode

    \sa BURGER_API

***************************************/

/*! ************************************

    \def BURGER_ASM
    \brief "C" compiler keyword for inline assembly.

    No one could agree on what would the keyword be to start a block of inline
    assembly. This will map to asm or _asm depending on the compiler.

    It's preferred that this keyword is used for Intel assembly.

    \sa BURGER_DECLSPECNAKED or BURGER_ANSIAPI

***************************************/

/*! ************************************

    \def BURGER_FASTCALLENABLED
    \brief Flag for determining Intel assembly generation

    This flag is defined only if the target platform is Intel and the parameter
    passing is in the "__fastcall" format.

    Some targets require parameters to be passed in the __cdecl format, so this
    will allow the selection of the proper assembly header and footer without
    having to create complicated \#ifdefs

    \sa BURGER_API, BURGER_ANSIAPI or BURGER_DECLSPECNAKED

***************************************/

/*! ************************************

    \def BURGER_DECLSPECNAKED
    \brief "C" compiler keyword for a pure assembly function.

    No one could agree on what would the keyword be to declare a "C" function
    that is 100% assembly. This will map to asm or __declspec(naked) depending
    on the compiler.

    It's preferred that this keyword is used for Intel assembly only.

    \note Use of this keyword implies that there is no "C" glue code so setup,
    tear down and exiting must all be present in the assembly code.

    \code
    BURGER_DECLSPECNAKED Word32 BURGER_API SwapEndian(Word32 Val)
    {
        BURGER_ASM {
            mov		eax,ecx
            bswap	eax
            ret
        }
    }
    \endcode

    \sa BURGER_X86 or BURGER_FASTCALLENABLED

***************************************/

/*! ************************************

    \def BURGER_ALIGN
    \brief Macro to force data alignment.

    Some data needs to be aligned differently than native alignment. Use this
    macro to have a compiler agnostic way to ensure data is aligned in a method
    that is desired by the programmer.

    \note The size must be a power of two. Acceptable sizes are 1, 2, 4, 8, 16
    and 32. Larger are acceptable, as long as they are powers of two.

    \code
        extern int BURGER_ALIGN(foo,8);
        CoolClass BURGER_ALIGN(MyClass,1);
        int BURGER_ALIGN(DataArray[4],8) = {1,2,3,4};
    \endcode

    \sa BURGER_NO_ALIGN, BURGER_PREALIGN and BURGER_POSTALIGN

***************************************/

/*! ************************************

    \def BURGER_PREALIGN
    \brief First half of a macro to force data alignment.

    Some data needs to be aligned differently than native alignment. However,
    it's sometimes impractical to use the \ref BURGER_ALIGN macro so this macro
    and its compliment are used to align complex objects like classes and
    structures. Use the code example below for proper use.

    \note The size must be a power of two. Acceptable sizes are 1, 2, 4, 8, 16
    and 32. Larger are acceptable, as long as they are powers of two.

    \code
    BURGER_PREALIGN(32) class foo {
    public:
        foo();
        int m_Data;		// This class is 32 byte aligned
    } BURGER_POSTALIGN(32);
    \endcode

    \sa BURGER_NO_ALIGN, BURGER_ALIGN and BURGER_POSTALIGN

***************************************/

/*! ************************************

    \def BURGER_POSTALIGN
    \brief Second half of a macro to force data alignment.

    Some data needs to be aligned differently than native alignment. However,
    it's sometimes impractical to use the \ref BURGER_ALIGN macro so this macro
    and its compliment are used to align complex objects like classes and
    structures. Use the code example below for proper use.

    \note The size must be a power of two. Acceptable sizes are 1, 2, 4, 8, 16
    and 32. Larger are acceptable, as long as they are powers of two.

    \code
    BURGER_PREALIGN(32) class foo {
    public:
        foo();
        int m_Data;		// This class is 32 byte aligned
    } BURGER_POSTALIGN(32);
    \endcode

    \sa BURGER_NO_ALIGN, BURGER_PREALIGN and BURGER_ALIGN

***************************************/

/*! ************************************

    \def BURGER_NO_ALIGN
    \brief Per object alignment is not supported.

    On older compilers, the ability to set the alignment of individual object is
    not supported. This macro exists on those compilers to alert code to be
    written to handle this case.

    \sa BURGER_PREALIGN, BURGER_POSTALIGN and BURGER_ALIGN

***************************************/

/*! ************************************

    \def BURGER_STRUCT_ALIGN
    \brief Defined if the compiler uses Macintosh style data alignment commands.

    Compilers can be set to change the default alignment of data structures. If
    alignment for a specific header needs to be set, you will need to insert
    this code as a prefix and a postfix to make sure that the compiler settings
    are overridden.

    This is useful for data structures that are mapped to the contents of a
    loaded file.

    \note Care must be exercised when changing alignment to byte or short. Some
    CPUs will generate a page fault if a 32 bit value is not 4 byte aligned.

    \code

    // Prefix for 4 byte alignment
    #if defined(BURGER_STRUCT_ALIGN)
    #pragma options align=power
    #elif defined(BURGER_STRUCT_PACKPUSH)
    #pragma pack(push,4)
    #elif defined(BURGER_STRUCT_PACK)
    #pragma pack(4)
    #endif

    // Postfix to restore compiler setting alignment
    #if defined(BURGER_STRUCT_ALIGN)
    #pragma options align=reset
    #elif defined(BURGER_STRUCT_PACKPUSH)
    #pragma pack(pop)
    #elif defined(BURGER_STRUCT_PACK)
    #pragma pack()
    #endif

    \endcode

    \sa BURGER_STRUCT_PACK or BURGER_STRUCT_PACKPUSH

***************************************/

/*! ************************************

    \def BURGER_STRUCT_PACK
    \brief Defined if the compiler uses GNU style data alignment commands.

    Compilers can be set to change the default alignment of data structures. If
    alignment for a specific header needs to be set, you will need to insert
    this code as a prefix and a postfix to make sure that the compiler settings
    are overridden.

    This is useful for data structures that are mapped to the contents of a
    loaded file.

    \note Care must be exercised when changing alignment to byte or short. Some
    CPUs will generate a page fault if a 32 bit value is not 4 byte aligned.

    \code

    // Prefix for 4 byte alignment
    #if defined(BURGER_STRUCT_ALIGN)
    #pragma options align=power
    #elif defined(BURGER_STRUCT_PACKPUSH)
    #pragma pack(push,4)
    #elif defined(BURGER_STRUCT_PACK)
    #pragma pack(4)
    #endif

    // Postfix to restore compiler setting alignment
    #if defined(BURGER_STRUCT_ALIGN)
    #pragma options align=reset
    #elif defined(BURGER_STRUCT_PACKPUSH)
    #pragma pack(pop)
    #elif defined(BURGER_STRUCT_PACK)
    #pragma pack()
    #endif

    \endcode

    \sa BURGER_STRUCT_ALIGN or BURGER_STRUCT_PACKPUSH

***************************************/

/*! ************************************

    \def BURGER_STATIC_ASSERT(x)
    \brief Test a compile time condition and if it's false, force a compiler
        error

    Use either typedef int ThisIsABogusTypeDef ## __LINE__ [(x) * 2 - 1] or
    static_assert() to determine if a conditional that can be determined at
    compile time should trigger an assert. This macro does not generate any
    runtime code.

    \note This macro will always invoke the conditional in all builds. The
    conditional must be one that is resolved at compile time.

    \param x A boolean that evaluates to \ref FALSE to force a compile time
        error

    \code
    // Will succeed
    BURGER_STATIC_ASSERT(sizeof(Word32)==sizeof(Int32));
    // Will fail
    BURGER_STATIC_ASSERT(sizeof(char)==sizeof(int));
    // Will fail if sizeof(eType) != sizeof(int)
    enum eType {
        TEST
    };
    BURGER_STATIC_ASSERT(sizeof(eType)==sizeof(int));

    // Use BURGER_ASSERT(x) instead, since this example is resolved at runtime
    BURGER_STATIC_ASSERT(Burger::StringLength("EpicFail")==8);
    \endcode

***************************************/

/*! ************************************

    \def BURGER_CONSTEXPR
    \brief Defined to nothing if constexpr is not supported.

    On compilers that support the keyword constexpr, this define will
    be set to constexpr, otherwise this define will be defined to nothing.

    \code

    // Foo is code that can generate a runtime constant
    // with this keyword. If the input values are
    // constants, the entire function will be optimized
    // away and a new constant will be created

    BURGER_CONSTEXPR int Foo(int a,int b)
    {
        return a*b;
    }

    void Sample(void)
    {
        // This will compile to iNewConstant = 6 on C++14 compilers
        // or a call to Foo(2,3) on older compilers that
        // don't perform Link Time Code Generation

        int iNewConstant = Foo(2,3);
    }

    \endcode

    \sa BURGER_CPP11, BURGER_FINAL, BURGER_OVERRIDE or nullptr

***************************************/

/*! ************************************

    \def BURGER_NOEXCEPT
    \brief Defined to nothing if noexcept is not supported.

    On compilers that support the keyword noexcept, this define will
    be set to noexcept, otherwise this define will be defined to nothing.

    \sa BURGER_CPP11, BURGER_FINAL, BURGER_OVERRIDE or nullptr

***************************************/

/*! ************************************

    \def nullptr
    \brief Defined to nothing if the feature is not available.

    On compilers that support nullptr, this macro does not exist. To support
    this feature on older compilers, this macro exists to simulate the feature

    \sa BURGER_CPP11, BURGER_OVERRIDE or BURGER_CONSTEXPR

***************************************/

/*! ************************************

    \def __underlying_type
    \brief Defined if __underlying_type is not available.

    If the feature __underlying_type isn't supported by the compiler, this
    macro will be defined to `int` to allow code to compile on older
    compilers.

    \sa BURGER_CPP11, nullptr or BURGER_CONSTEXPR

***************************************/

/*! ************************************

    \def BURGER_OVERRIDE
    \brief Defined to nothing if the feature is not available.

    On compilers that support override, this macro is set to override. To
    support this feature on older compilers, this macro is set to nothing.

    \sa BURGER_CPP11, nullptr, BURGER_FINAL or BURGER_CONSTEXPR

***************************************/

/*! ************************************

    \def BURGER_FINAL
    \brief Defined to nothing if the feature is not available.

    On compilers that support final, this macro is set to final. To
    support this feature on older compilers, this macro is set to nothing.

    Microsoft Visual C can set this to sealed, or for GNU compilers it is
    set to __final.

    \sa BURGER_CPP11, nullptr, BURGER_OVERRIDE or BURGER_CONSTEXPR

***************************************/

/*! ************************************

    \def BURGER_RVALUE_REFERENCES
    \brief Defined if move semantics are available.

    On compilers that support move constructors, this define
    will exist.

    \sa BURGER_CPP11, nullptr, BURGER_OVERRIDE or BURGER_CONSTEXPR

***************************************/

/*! ************************************

    \def BURGER_ADDRESS_SANITIZER
    \brief Detect if address sanitization is active.

    If the code is being built with Address Sanitization, this macro will exist.

    \sa BURGER_DISABLE_ASAN or BURGER_CLANG

***************************************/

/*! ************************************

    \def BURGER_DISABLE_ASAN
    \brief Disable address sanitization.

    If the code is being built with Address Sanitization, this macro will
    disable testing any function that is marked. It's to work around false
    positives.

    \sa BURGER_ADDRESS_SANITIZER, BURGER_DISABLE_MSAN or BURGER_CLANG

***************************************/

/*! ************************************

    \def BURGER_MEMORY_SANITIZER
    \brief Detect if memory sanitization is active.

    If the code is being built with Memory Sanitization, this macro will exist.

    \sa BURGER_DISABLE_MSAN or BURGER_CLANG

***************************************/

/*! ************************************

    \def BURGER_DISABLE_MSAN
    \brief Disable memory sanitization.

    If the code is being built with Memory Sanitization, this macro will disable
    testing any function that is marked. It's to work around false positives.

    \sa BURGER_MEMORY_SANITIZER, BURGER_DISABLE_ASAN or BURGER_CLANG

***************************************/

/*! ************************************

    \def BURGER_MAYBE_UNUSED
    \brief Mark with [[maybe_unused]]

    For C++17 or higher compilers, or those that support the equivalent, insert
    the [[maybe_unused]] attribute to the code.

    \sa BURGER_NODISCARD, BURGER_USED or BURGER_FALLTHROUGH

***************************************/

/*! ************************************

    \def BURGER_NODISCARD
    \brief Mark with [[nodiscard]]

    For C++17 or higher compilers, or those that support the equivalent, insert
    the [[nodiscard]] attribute to the code.

    \sa BURGER_MAYBE_UNUSED, BURGER_USED or BURGER_FALLTHROUGH

***************************************/

/*! ************************************

    \def BURGER_FALLTHROUGH
    \brief Mark with [[fallthrough]]

    For C++17 or higher compilers, or those that support the equivalent, insert
    the [[fallthrough]] attribute to the code.

    \sa BURGER_NODISCARD or BURGER_MAYBE_UNUSED

***************************************/

/*! ************************************

    \def BURGER_USED
    \brief Mark with __attribute__((used))

    Some compilers remove functions and variables that are not used by the
    program. To force a function or variable to be compiled and linked into the
    final product, mark it with this macro.

    \sa BURGER_NODISCARD or BURGER_MAYBE_UNUSED

***************************************/

/*! ************************************

    \def BURGER_STRUCT_PACKPUSH
    \brief Defined if the compiler uses Microsoft style data alignment commands.

    Compilers can be set to change the default alignment of data structures. If
    alignment for a specific header needs to be set, you will need to insert
    this code as a prefix and a postfix to make sure that the compiler settings
    are overridden.

    This is useful for data structures that are mapped to the contents of a
    loaded file.

    \note Care must be exercised when changing alignment to byte or short. Some
    CPUs will generate a page fault if a 32 bit value is not 4 byte aligned.

    \code

    // Prefix for 4 byte alignment
    #if defined(BURGER_STRUCT_ALIGN)
    #pragma options align=power
    #elif defined(BURGER_STRUCT_PACKPUSH)
    #pragma pack(push,4)
    #elif defined(BURGER_STRUCT_PACK)
    #pragma pack(4)
    #endif

    // Postfix to restore compiler setting alignment
    #if defined(BURGER_STRUCT_ALIGN)
    #pragma options align=reset
    #elif defined(BURGER_STRUCT_PACKPUSH)
    #pragma pack(pop)
    #elif defined(BURGER_STRUCT_PACK)
    #pragma pack()
    #endif

    \endcode

    \sa BURGER_STRUCT_ALIGN or BURGER_STRUCT_PACK

***************************************/

/*! ************************************

    \def BURGER_NO_USING_NAMESPACE
    \brief Define to disable using namespace Burger;

    If this define exists, then code found in burger.h "using namespace Burger;"
    will be disabled. The default behavior when using burgerlib is to assume the
    namespace Burger is enabled. For some codebases, there may be name
    collisions and this is to remove the collisions.

    \note Define this manually BEFORE the inclusion of burger.h or define it in
    the compiler command line.

    \code
    // Code in burger.h
    #if !defined(BURGER_NO_USING_NAMESPACE)
    using namespace Burger;
    #endif

    // Code in a user C file
    #define BURGER_NO_USING_NAMESPACE
    #include <burger.h>

    // From here on out, use Burgerlib by invoking the Burger namespace
    \endcode

***************************************/

/*! ************************************

    \def BURGER_OPENGL
    \brief Define for the presence of OpenGL

    If this define exists, then OpenGL is supported.
    \sa BURGER_OPENGLES, or BURGER_VULKAN

***************************************/

/*! ************************************

    \def BURGER_OPENGLES
    \brief Define for the presence of OpenGL ES

    If this define exists, then OpenGL ES is supported.
    \sa BURGER_OPENGL, or BURGER_VULKAN

***************************************/

/*! ************************************

    \def BURGER_VULKAN
    \brief Define for the presence of Vulkan

    If this define exists, then Vulkan is supported.
    \sa BURGER_OPENGL, or BURGER_OPENGLES

***************************************/

/*! ************************************

    \def BURGER_TCPIP
    \brief Define for the presence of TCP/IP

    If this define exists, then the TCP/IP networking protocol
    is supported.

    \sa BURGER_APPLETALK, BURGER_IPX, BURGER_STEAM, or BURGER_XBOXLIVE

***************************************/

/*! ************************************

    \def BURGER_APPLETALK
    \brief Define for the presence of AppleTalk

    If this define exists, then the AppleTalk networking protocol
    is supported. This is usually found on older MacOS computers and
    some Windows XP platforms.

    \sa BURGER_TCPIP, BURGER_IPX, BURGER_STEAM, or BURGER_XBOXLIVE

***************************************/

/*! ************************************

    \def BURGER_IPX
    \brief Define for the presence of IPX/SPX

    If this define exists, then the IPX/SPX networking protocol
    is supported. It's supported on Windows and older MacOS platforms.

    \sa BURGER_TCPIP, BURGER_APPLETALK, BURGER_STEAM, or BURGER_XBOXLIVE

***************************************/

/*! ************************************

    \def BURGER_STEAM
    \brief Define for the presence of Valve's Steam platform

    If this define exists, then the Steam platform is supported.
    TCP/IP can be assumed to be available with this define present.

    \sa BURGER_TCPIP, BURGER_APPLETALK, BURGER_IPX, or BURGER_XBOXLIVE

***************************************/

/*! ************************************

    \def BURGER_XBOXLIVE
    \brief Define for the presence of Microsoft's Xbox Live platform.

    If this define exists, then the Xbox Live platform is supported.
    TCP/IP can be assumed to be available with this define present.

    \sa BURGER_TCPIP, BURGER_APPLETALK, BURGER_IPX, or BURGER_STEAM

***************************************/

/*! ************************************

    \def BURGER_CONSOLE
    \brief Define for the presence of a text console.

    If this define exists, then the platform supports a terminal console such as
    CMD.COM, bash, sh etc. Test for this macro if the code requires a console,
    such as code for a command line tool.

    \sa BURGER_MSDOS, BURGER_LINUX, BURGER_MACOS or BURGER_WINDOWS

***************************************/

/*! ************************************

    \def BURGER_OFFSETOF
    \brief Define to return the offset of a member variable.

    Return the byte offset of a member variable from a class or struct.

    \param __type Name of the class / struct type
    \param __member Name of the member in the type to determine the offset of.

    \code
    struct foo {
        int m_member;
        int m_member2;
    };

    // Will equal to 4 (Usually)
    int offsetofmember2 = BURGER_OFFSETOF(foo,m_member2);

    \endcode

    \sa BURGER_GET_BASE_PTR

***************************************/

/*! ************************************

    \def BURGER_GET_BASE_PTR
    \brief Define to return the base pointer of a class from a class member

    Return the pointer to the parent class a member variable resides.

    \param x Pointer to the member variable that needs fixup
    \param __type Name of the class / struct type
    \param __member Name of the member in the type to determine the offset of.

    \code
    struct foo {
        int m_member;
        int m_member2;
    };

    struct bar {
        int m_Temp;
        int m_Temp2;
        foo m_Foo;
    };

    // Structure to work with
    bar Test;

    // Get pointer to a member variable
    foo *pFoo = &Test;

    // Use the macro to convert the member pointer back into a bar *
    bar *pBar = BURGER_GET_BASE_PTR(pFoo,bar,m_Foo);

    \endcode

    \sa BURGER_OFFSETOF, BURGER_CONST_GET_BASE_PTR

***************************************/

/*! ************************************

    \def BURGER_CONST_GET_BASE_PTR
    \brief Define to return the const base pointer of a class from a class
        member

    Return the const pointer to the parent class a member variable resides.

    \param x Const pointer to the member variable that needs fixup
    \param __type Name of the class / struct type
    \param __member Name of the member in the type to determine the offset of.

    \code
    struct foo {
        int m_member;
        int m_member2;
    };

    struct bar {
        int m_Temp;
        int m_Temp2;
        foo m_Foo;
    };

    // Structure to work with
    bar Test;

    // Get pointer to a member variable
    const foo *pFoo = &Test;

    // Use the macro to convert the member pointer back into a bar *
    const bar *pBar = BURGER_CONST_GET_BASE_PTR(pFoo,bar,m_Foo);

    \endcode

    \sa BURGER_OFFSETOF, BURGER_GET_BASE_PTR

***************************************/

/*! ************************************

    \def BURGER_ARRAYSIZE
    \brief Define to return the number of elements in an array.

    For arrays where the size is known at compile time, this macro will return
    the number of elements found in the array

    \param x Name of the array to determine the size

    \code
    int Foo[100];

    // Will equal to 100
    int ElementsInFoo = BURGER_ARRAYSIZE(Foo);

    \endcode
    \sa _BurgerArraySize

***************************************/

/*! ************************************

    \fn const char(*_BurgerArraySize(T(&)[N]))[N]
    \brief Helper for \ref BURGER_ARRAYSIZE

    Non-existent array for determining array element size.

    \tparam T Data type of the elements of the array/
    \tparam N Number of elements in the array

    \sa BURGER_ARRAYSIZE

***************************************/

/*! ************************************

    \def BURGER_UNUSED
    \brief Define to disable the unused variable warning.

    In cases where a variable is defined, but not used, such as in testing error
    conditions with asserts, this macro will alert the compiler that the
    existence of this variable was intentional.

    \param x Name of the variable to allow to be unused

    \code

    // Ignore errors on release builds
    #if defined(NDEBUG)
    #define I_FEAR_NOTHING(x)
    #else
    #define I_FEAR_NOTHING(x) x
    #endif

    int iError = FunctionThatCanFail();
    // Disable unused variable warning
    BURGER_UNUSED(iError);

    I_FEAR_NOTHING(printf("Error code %d",iError));

    \endcode

***************************************/

/*! ************************************

    \def BURGER_STRINGIZE
    \brief Convert a macro into a string literal.

    Using deep magic in the form of \ref BURGER_STRINGIZE2,
    convert the macro into an 8 bit quoted string literal.

    \param x Name of the macro to turn into a string

    \code
    #define foo 4
    printf(BURGER_STRINGIZE(foo));

    // Prints "4" (Without the quotes)

    \endcode
    \sa BURGER_STRINGIZE2

***************************************/

/*! ************************************

    \def BURGER_STRINGIZE2
    \brief Convert the parameter into a string literal.

    Using the # token, convert the macro parameter into an 8 bit quoted string
    literal.

    \param x Name of the item to turn into a string

    \code
    int foo = 4;
    printf(BURGER_STRINGIZE2(foo));

    // Prints "foo" (Without the quotes)

    \endcode

    \sa BURGER_STRINGIZE

***************************************/

/*! ************************************

    \def BURGER_JOIN
    \brief Concatenates two strings, even if they are macros.

    Using deep magic in the form of \ref BURGER_JOIN2 and \ref BURGER_JOIN3,
    join the two macros into one.

    \param x The first macro to join.
    \param y The second macro to join.

    \sa BURGER_JOIN2 or BURGER_JOIN3

***************************************/

/*! ************************************

    \def BURGER_JOIN2
    \brief Invokes \ref BURGER_JOIN3

    Invoke \ref BURGER_JOIN3 to join macros with ##.

    \param x The first macro to join.
    \param y The second macro to join.

    \sa BURGER_JOIN or BURGER_JOIN3

***************************************/

/*! ************************************

    \def BURGER_JOIN3
    \brief Combine two macros with ##.

    Use ## to join two macros.

    \param x The first macro to join.
    \param y The second macro to join.

    \sa BURGER_JOIN or BURGER_JOIN2

***************************************/

/*! ************************************

    \def BURGER_LEFT_PARENTHESIS
    \brief The character (

    If a macro contains a parenthesis, it will be treated as an enclosure for a
    parameter list. Use this macro inside another macro to output a left
    parenthesis without actually invoking parameters.

    \sa BURGER_RIGHT_PARENTHESIS

***************************************/

/*! ************************************

    \def BURGER_RIGHT_PARENTHESIS
    \brief The character )

    If a macro contains a parenthesis, it will be treated as an enclosure for a
    parameter list. UUse this macro inside another macro to output a right
    parenthesis without actually invoking parameters.

    \sa BURGER_LEFT_PARENTHESIS

***************************************/

/*! ************************************

    \def BURGER_DISABLE_COPY
    \brief Declares copy constructors to disable invocation at runtime.

    Some classes by their nature shouldn't be or can't be copied, by declaring
    this macro, the x(x const &) constructor and the x &operator = (x const &)
    operator are declared. If compiled with C++11 or higher, the x(x &&) and
    x& operator = (x &&) are also declared as '= delete'.

    \note This macro MUST be invoked in a private section of a class or it will
    inadvertently declare that there are copy constructors in existence which
    could lead to mysterious link errors.

    \param x Name of the class that the macro resides in

    \code
    class DontCopyMe {
        // Disallow copying of this class
        BURGER_DISABLE_COPY(DontCopyMe);
    public:
        void DoStuff(void);

    \endcode

    \note This macro will set the class setting to "private"

    \sa BURGER_EQUALS_DELETE

***************************************/

/*! ************************************

    \def BURGER_EQUALS_DELETE
    \brief Declare "= delete" on C++ 11 compilers

    Some class functions can be disabled in C++11 by the use of the
    term "= delete" suffix. Of the compiler supports this feature, this
    macro will map to "= delete", otherwise the macro will be blank.

    \code
    class DontCopyMe {
        // Disallow copying of this class
        DontCopyMe(const &) BURGER_EQUALS_DELETE;
        DontCopyMe& operator=(DontCopyMe const&) BURGER_EQUALS_DELETE;
    public:
        void DoStuff(void);

    \endcode

    \sa BURGER_DISABLE_COPY, or BURGER_EQUALS_DEFAULT

***************************************/

/*! ************************************

    \def BURGER_EQUALS_DEFAULT
    \brief Declare "= default" on C++ 11 compilers

    Some default constructors can be created in C++11 by the use of the
    term "= default" suffix. Of the compiler supports this feature, this
    macro will map to "= default", otherwise the macro will be blank.

    \code
    class DefaultClass {
        // Default constructor
        DefaultClass() BURGER_EQUALS_DEFAULT;

    public:
        void DoStuff(void);

    \endcode

    \sa BURGER_EQUALS_DELETE or BURGER_DEFAULT_DESTRUCTOR

***************************************/

/*! ************************************

    \def BURGER_DEFAULT_DESTRUCTOR
    \brief Declare "= default" on C++ 11 compilers

    For a default destructor, "= default" is the preferred method for C++ 11 or
    higher compilers, while earlier compilers prefer {} for default destructors.
    This macro will expand to the preferred format for creating default
    destructors.

    \code
    class DefaultClass {
        // Default destructor
        ~DefaultClass() BURGER_DEFAULT_DESTRUCTOR;

    public:
        void DoStuff(void);

    \endcode

    \sa BURGER_EQUALS_DEFAULT or BURGER_EQUALS_DELETE

***************************************/

/*! ************************************

    \def BURGER_ENUMSTART(x,y)
    \brief Creates a cross platform "enum class : int {" instance

    This macro maps to "namespace x { enum Type {" on older compilers.

    Newer compilers support strong enum types where it's impossible to test one
    enum type with another due to the implicit conversion to integer. This is
    called "enum class". This macro offers this support while still offering
    backwards compatibility with C89 and C99 compilers. The solution was to use
    "enum class" on compilers that support it, and use a namespace to
    encapsulate the enum members on older compilers (Or compilers that have
    extensions turned off).

    There were compromises made, enums need to be generated with a start and end
    macro so they could ensure the members are either inside a namespace or an
    enum class. However, this caused a difference in the final type, with it
    either being a global type name or a name embedded in a namespace. To get
    around this, a unified type was generated called "???Type" where the ??? is
    replaces with the name of the new enum and it's declared as a typedef to
    either the enum class or "enum Type" that resides in the namespace.

    \note Always use the declared enum type to reference the members and ???
    Type to declare the enum as a member variable or parameter. Secondly, the
    size of the data is only enforced on compilers that offer "enum class foo :
    char" or "enum foo : char" type declarations, otherwise, the compiler will
    likely map the enum to a signed integer.

    \param x Name of the new enum
    \param y Desired data type for the new enum

    \code

    // Declare eTestEnum members and the eTestEnumType

    BURGER_ENUMSTART(eTestEnum,uint_t) {
        value0,
        value1,
        value2
    } BURGER_ENUMEND(eTestEnum)

    // Call this function with the global enum type of eTestEnumType

    void foo(eTestEnumType uValue)
    {
        // Test each value with eTestEnum:: prefixes
        switch (uValue) {
        case eTestEnum::value0:
            printf("value0\n");
            break;
        case eTestEnum::value1:
            printf("value1\n");
            break;
        case eTestEnum::value0:
            printf("value2\n");
            break;
        }
    }

    \endcode

    \sa BURGER_ENUMEND and BURGER_ENUMFLAGSEND

***************************************/

/*! ************************************

    \def BURGER_ENUMEND(x)
    \brief Finish a cross platform "enum class : int {" instance

    This macro maps to "}; } typedef x::Type x ## Type;" on older compilers.

    \param x Name of the new enum (Must match the name used in preceding \ref
        BURGER_ENUMSTART(x)

    \note Read the description of \ref BURGER_ENUMSTART for full details.

    \code

    // Declare eTestEnum members and the eTestEnumType

    BURGER_ENUMSTART(eTestEnum,uint_t) {
        value0,
        value1,
        value2
    } BURGER_ENUMEND(eTestEnum)

    // Call this function with the global enum type of eTestEnumType

    void foo(eTestEnumType uValue)
    {
        // Test each value with eTestEnum:: prefixes
        switch (uValue) {
        case eTestEnum::value0:
            printf("value0\n");
            break;
        case eTestEnum::value1:
            printf("value1\n");
            break;
        case eTestEnum::value0:
            printf("value2\n");
            break;
        }
    }

    \endcode

    \sa BURGER_ENUMSTART and BURGER_ENUMFLAGSEND

***************************************/

/*! ************************************

    \def BURGER_ENUMFLAGSEND(x,y)
    \brief Finish a cross platform "enum class : int {" instance for flags.

    \details This macro will invoke \ref BURGER_ENUMEND and then add a list of
    operator overloads so the bit values can be merged and tested with strong
    type safety.

    Binary operators that are considered valid for flags are AND (&), OR (|) and
    XOR (^) and unary operations NOT (!) and bitwise XOR (~).

    The operation overload will enforce the data type declared for the enum, so
    operators such as bitwise XOR (~) will only be as wide as the declared
    underlying type.

    \note Read the description of \ref BURGER_ENUMSTART for full details.

    \param x Name of the new enum (Must match the name used in preceding \ref
        BURGER_ENUMSTART(z)

    \param y Desired data type for the new enum

    \code

    // Declare eTestFlags members and the eTestFlagsType

    BURGER_ENUMSTART(eTestFlags,Byte) {
        None=0,					// No flag present
        Flag1 = 0x01,			// First bit flag
        Flag2 = 0x02,			// Second bit flag
        Flag3 = 0x04			// You get the drift
    } BURGER_ENUMFLAGSEND(eTestFlags,Byte)

    // Call this function with the global enum type of eTestFlagsType

    void foo(eTestFlagsType uFlags)
    {
        // A flag was set?
        if (uFlags!=eTestFlags::None) {
            // Test the flags and print
            if ((uFlags&eTestFlags::Flag1)!=eTestFlags::None) {
                printf("Flag1 found\n");
            }
            if ((uFlags&eTestFlags::Flag2)!=eTestFlags::None) {
                printf("Flag2 found\n");
            }
            if ((uFlags&eTestFlags::Flag3)!=eTestFlags::None) {
                printf("Flag3 found\n");
            }
            if
((uFlags&(eTestFlags::Flag2|eTestFlags::Flag3))!=eTestFlags::None) {
                printf("Flag2 or Flag3 found\n");
            }
            if
((uFlags&(eTestFlags::Flag2|eTestFlags::Flag3))==(eTestFlags::Flag2|eTestFlags::Flag3))
{ printf("Flag2 and Flag3 found\n");
            }
        }
    }

    void bar(void)
    {
        foo(eTestFlags::None);
        foo(eTestFlags::Flag2);
        foo(eTestFlags::Flag2|eTestFlags::Flag3);
    }

    \endcode

    \sa BURGER_ENUMSTART and BURGER_ENUMFLAGSEND

***************************************/

/*! ************************************

    \def BURGER_SIZEOF_INT
    \brief Size in bytes of the intrinsic int

    Define that has the number of bytes an int occupies. This value can be 2, 4
    or 8. Default is 4

    \sa BURGER_SIZEOF_LONG

***************************************/

/*! ************************************

    \def BURGER_SIZEOF_LONG
    \brief Size in bytes of the intrinsic long

    Define that has the number of bytes a long occupies. This value can be 4
    or 8. Default is 4.

    \sa BURGER_SIZEOF_INT

***************************************/

/*! ************************************

    \def BURGER_LONGLONG
    \brief Signed 64 bit integer type specific to the current compiler.

    Define that has the compiler keyword that defines a 64 bit signed integer.

    \sa BURGER_ULONGLONG

***************************************/

/*! ************************************

    \def BURGER_ULONGLONG
    \brief Unsigned 64 bit integer type specific to the current compiler.

    Define that has the compiler keyword that defines a 64 bit unsigned integer.

    \sa BURGER_LONGLONG

***************************************/

/*! ************************************

    \def BURGER_HAS_WCHAR_T
    \brief Data type wchar_t is native

    If this define is present, wchar_t is a native type for the compiler,
    otherwise, it's a typedef cast from an unsigned short, which may cause
    collisions for classes that want to treat wchar_t and Word16 as unique data
    types.

***************************************/

/*! ************************************

    \def BURGER_INT_NOT_IN_STDINT
    \brief Is `int` used in stdint.h

    If this define is present, `int` isn't used in the int32_t family of data
    types, so if a series of functions wish to allow a function to be
    automatically called using `int` as a parameter, a function specifically
    using `int` as an input must be declared.

    \sa BURGER_LONG_NOT_IN_STDINT

***************************************/

/*! ************************************

    \def BURGER_LONG_NOT_IN_STDINT
    \brief Is `long` used in stdint.h

    If this define is present, `long` isn't used in the int32_t family of
    data types, so if a series of functions wish to allow a function to be
    automatically called using `long` as a parameter, a function specifically
    using `long` as an input must be declared.

    \sa BURGER_INT_NOT_IN_STDINT

***************************************/

/*! ************************************

    \def BURGER_HAS_64_BIT_SUPPORT
    \brief Computer supports 64 bit integers.

    Most compilers support 64 bit integers, but older compilers only support 32
    bit integers. To allow limited 64 bit integer support, this define exists
    and `Burger::longlong_t` and `Burger::ulonglong_t` are used to preform 64
    bit integer math.

    \sa Burger::longlong_t, or Burger::ulonglong_t

***************************************/

/*! ************************************

    \def BURGER_HAS_SFINAE
    \brief Compiler supports SFINAE

    Most compilers support "Substitution Failure Is Not An Error" (SFINAE), for
    those few old compilers that don't support it, this macro doesn't exist.

    \sa BURGER_EMPTY_TEMPLATE_DECLARATION

***************************************/

/*! ************************************

    \def BURGER_EMPTY_TEMPLATE_DECLARATION
    \brief Declares an empty template parameter list

    Apple's MPW compilers, MrC and SCpp, do not support the C++ term
    "template <>", but if the term is removed, template overrides can be
    declared. To help ease the development of template overrides, this macro
    will remove the template<> keyword to allow code to compile properly on MPW
    compilers.

    Example of use:

    \code
    template<class T>
    T Square(T Value)
    {
        return Value*Value;
    }

    BURGER_EMPTY_TEMPLATE_DECLARATION
    float Square(float Value)
    {
        return Value*Value;
    }

    \endcode

    \sa BURGER_HAS_SFINAE

***************************************/

/*! ************************************

    \typedef Word8
    \brief 8 bit unsigned integer.

    This integer can contain the number 0 through 255. It is compiler switch
    setting safe.

    \sa Int8

***************************************/

/*! ************************************

    \typedef Int8
    \brief 8 bit signed integer.

    This integer can contain the number -128 through 127. It is compiler switch
    setting safe.

    \sa Word8

***************************************/

/*! ************************************

    \typedef Word16
    \brief 16bit unsigned integer.

    This integer can contain the number 0 through 65535. It is compiler switch
    setting safe.

    \sa Int16

***************************************/

/*! ************************************

    \typedef Int16
    \brief 16 bit signed integer.

    This integer can contain the number -32768 through 32767. It is compiler
    switch setting safe.

    \sa Word16

***************************************/

/*! ************************************

    \typedef Int32
    \brief 32 bit signed integer.

    This integer can contain the number -2,147,483,648 through 2,147,483,647. It
    is compiler switch setting safe.

    \note This is cast as a signed int on the Playstation 2.

    \sa Word32 or Word

***************************************/

/*! ************************************

    \typedef Word32
    \brief 32 bit unsigned integer.

    This integer can contain the number 0 through 4,294,967,295. It is compiler
    switch setting safe.

    \note This is cast as an unsigned int on the Playstation 2.

    \sa Int32 or Int

***************************************/

/*! ************************************

    \typedef Int64
    \brief 64 bit signed integer.

    This integer can contain the number -9,223,372,036,854,775,808 through
    9,223,372,036,854,775,807. It is compiler switch setting safe.

    \note This is cast as a signed long on the Playstation 2.

    \note Some processors like the 680x0 do not have registers this large and as
    a result it will incur a performance penalty. Use this type carefully.

    \sa Word64

***************************************/

/*! ************************************

    \typedef Word64
    \brief 64 bit unsigned integer.

    This integer can contain the number 0 through 18,446,744,073,709,551,615. It
    is compiler switch setting safe.

    \note This is cast as an unsigned long on the Playstation 2.

    \note Some processors like the 680x0 do not have registers this large and as
    a result it will incur a performance penalty. Use this type carefully.

    \sa Int64

***************************************/

/*! ************************************

    \struct Vector_128
    \brief 128 bit vector intrinsic.

    This integer is used for register remapping to the vector units on several
    CPUs. It is not to be used for 128 bit integer math.	Currently, only
    processors with vector units like the Playstation 2, 3, 4, Xbox 360, XBox
    ONE, Pentium with SSE, ARM with NEON and PowerPC with Alti-Vec can use this
    intrinsic data type.

    \sa Burger::Vector_128Float32, Burger::Vector_128Word32,
        Burger::Vector_128Int32, Burger::Vector_128Word16,
        Burger::Vector_128Word8, BURGER_ARM32, BURGER_POWERPC or BURGER_X86

***************************************/

/*! ************************************

    \typedef WordPtr
    \brief Unsigned integer that matches in size to a void *.

    Since address pointers can be 64 bit on some platforms, this type allows the
    casting and storing of this type into an unsigned integer.

    \note Caution should be used when using this type in data structures due to
    it being 64 or 32 bit on different platforms.

    \sa IntPtr

***************************************/

/*! ************************************

    \typedef IntPtr
    \brief Signed integer that matches in size to a void *.

    Since address pointers can be 64 bit on some platforms, this type allows the
    casting and storing of this type into an signed integer.

    \note Caution should be used when using this type in data structures due to
    it being 64 or 32 bit on different platforms.

    \sa WordPtr

***************************************/

/*! ************************************

    \typedef Bool
    \brief Unsigned 8 bit integer for boolean operations.

    When data storage is at a premium, this data type can be used to alert
    programmers that only a true or not true can be stored inside. Using the
    Word8 data type could imply that the data has a wider range.

    \sa Word8 or Int8.

***************************************/

/*! ************************************

    \typedef Fixed32
    \brief Signed 32 bit integer for fixed point operations.

    All 16.16 fixed point integer math uses this data type to alert the
    programmer of the special nature of the 32 bits. The upper 16 bits is an
    integer of the range of -32768 through 32767 and the lower 16 bits is a
    fraction of x/65536.

    \sa \ref FLOATTOFIXED, \ref FIXEDTOFLOAT, Int32 or Frac32.

***************************************/

/*! ************************************

    \typedef Frac32
    \brief Signed 32 bit integer for fixed point operations.

    All 2.30 fixed point integer math uses this data type to alert the
    programmer of the special nature of the 32 bits. The upper 2 bits is an
    integer of the range of -2 through 1 and the lower 30 bits is a fraction of
    x/1,073,741,824.

    \sa Int32 or Fixed32.

***************************************/

/*! ************************************

    \typedef Word
    \brief Unsigned integer.

    This is the most common data type in BurgerLib. On most platforms, this is
    an unsigned 32 bit integer but it can be 64 bit if the processor handles 64
    bit data more efficiently than 32 bit. To date, all CPUs handle 32 bit data
    with no penalty so it's locked at 32 bits.

    \sa Int

***************************************/

/*! ************************************

    \typedef Int
    \brief Signed integer.

    This is the second most common data type in BurgerLib. On most platforms,
    this is an signed 32 bit integer but it can be 64 bit if the processor
    handles 64 bit data more efficiently than 32 bit. To date, all CPUs handle
    32 bit data with no penalty so it's locked at 32 bits.

    \sa Word

***************************************/

/*! ************************************

    \typedef uint_t
    \brief Unsigned integer.

    This is the most common data type in BurgerLib. On most platforms, this is
    an unsigned 32 bit integer but it can be 64 bit if the processor handles 64
    bit data more efficiently than 32 bit. To date, all CPUs handle 32 bit data
    with no penalty so it's locked at 32 bits.

    \sa int_t

***************************************/

/*! ************************************

    \typedef int_t
    \brief Signed integer.

    This is the second most common data type in BurgerLib. On most platforms,
    this is an signed 32 bit integer but it can be 64 bit if the processor
    handles 64 bit data more efficiently than 32 bit. To date, all CPUs handle
    32 bit data with no penalty so it's locked at 32 bits.

    \sa uint_t

***************************************/

/*! ************************************

    \typedef int2int_t
    \brief `int` to int??_t.

    Data type to convert native `int` to the `int32_t`, `int16_t`
    or other compatible type that reflects the proper size.

    \sa uint2uint_t

***************************************/

/*! ************************************

    \typedef uint2uint_t
    \brief `unsigned int` to uint??_t.

    Data type to convert native `unsigned int` to the `uint32_t`,
    `uint16_t` or other compatible type that reflects the proper size.

    \sa int2int_t

***************************************/

/*! ************************************

    \typedef long2int_t
    \brief `long` to int??_t.

    Data type to convert native `long` to the `int32_t`, `int64_t`
    or other compatible type that reflects the proper size.

    \sa ulong2uint_t

***************************************/

/*! ************************************

    \typedef ulong2uint_t
    \brief `unsigned long` to uint??_t.

    Data type to convert native `unsigned long` to the `uint32_t`,
    `uint64_t` or other compatible type that reflects the proper size.

    \sa long2int_t

***************************************/

/*! ************************************

    \fn void Burger::SwapVariables(T *pA,T *pB)
    \brief Swap two variables of the same kind

    Given pointers to two like variables, swap the contents with each other.

    \note This template will fail if the T value is of a class that doesn't
    allow copying.

    \param pA Pointer to the first variable to swap.
    \param pB Pointer to the second variable to swap.

***************************************/

#endif
