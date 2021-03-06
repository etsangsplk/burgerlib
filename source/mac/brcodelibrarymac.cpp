/***************************************

    Code library (DLL) manager

    MacOS version

    Copyright (c) 1995-2017 by Rebecca Ann Heineman <becky@burgerbecky.com>

    It is released under an MIT Open Source license. Please see LICENSE for
    license details. Yes, you can use it in a commercial title without paying
    anything, just give me a credit.

    Please? It's not like I'm asking you for money!

***************************************/

#include "brcodelibrary.h"

#if defined(BURGER_MAC)
#include "brstringfunctions.h"
#include <CodeFragments.h>

#if defined(BURGER_MACCARBON)
#include <CFBundle.h>
#include <CFDate.h>
#include <Folders.h>
#endif

/***************************************

    Attempt to load in a shared library or DLL using
    the standard paths. Return NULL if it fails

***************************************/

Word Burger::CodeLibrary::Init(const char* pFilename)
{
    Str255 TempName; // Copy of the "C" string as a PASCAL string
    Str255 ErrStr;   // Returned error code if any
    Ptr EntryPtr;    // Pointer to the fragment entry
    CFragConnectionID ConnID;

    /* This code only works for CFM functions */
    Burger::CStringToPString(TempName, pFilename);
    Word uResult = FALSE;
    if (!GetSharedLibrary(TempName, kCompiledCFragArch, kLoadCFrag, &ConnID,
            &EntryPtr, ErrStr)) {
        m_pLibInstance = ConnID;
        uResult = TRUE;
    }
    return uResult;
}

/***************************************

    Release a shared library

***************************************/

void Burger::CodeLibrary::Shutdown(void)
{
    if (m_pLibInstance) {
        CloseConnection(reinterpret_cast<CFragConnectionID*>(&m_pLibInstance));
        m_pLibInstance = NULL;
    }
}

/***************************************

    Return a function pointer to a procedure or data contained within a shared
    library

***************************************/

void* Burger::CodeLibrary::GetFunction(const char* pFunctionName)
{
    Str255 TempName; // Copy of the "C" string as a PASCAL string
    Ptr ProcPtr;     // Pointer to the function
    void* pFunction = NULL;

    // This code only works for CFM functions
    if (pFunctionName && m_pLibInstance) {
        Burger::CStringToPString(TempName, pFunctionName);
        if (!FindSymbol(static_cast<OpaqueCFragConnectionID*>(m_pLibInstance),
                TempName, &ProcPtr, 0)) {
            pFunction = ProcPtr;
        }
    }
    return pFunction;
}

#endif

#if defined(BURGER_MACCARBON) || defined(DOXYGEN)

/*! ************************************

    \class Burger::CodeFramework
    \brief Manage MacOSX / iOS Framework library files.

    Loading a Framework bundle on MacOSX and iOS is managed with this generic
    class. Initialize it with a call to Init(const char) and then call
    GetFunction(const char *) to extract each and every pointer to the code or
    data contained within.

    If this class is shut down by the Shutdown() call or the class going out of
    scope, all of the pointers will become invalid and should never be used
    again.

    \sa CodeLibrary

***************************************/

/*! ************************************

    \brief Default constructor

***************************************/

Burger::CodeFramework::CodeFramework(): m_pBundle(NULL) {}

/*! ************************************

    \brief Default destructor

    If a Framework was loaded, it will be released

    \sa Shutdown(void)

***************************************/

Burger::CodeFramework::~CodeFramework()
{
    Shutdown();
}

/*! ************************************

    \brief Load a Framework

    Attempt to load in a Framework using the standard paths.

    \param pName Name of the Framework to load
    \return Zero on success, non-zero on failure

***************************************/

Burger::eError BURGER_API Burger::CodeFramework::Init(const char* pName)
{
    // Release any previous instance
    Shutdown();

    // Folder for the framework
    FSRef frameworksFolderRef;

    // Does the framework folder exist? (Only on a MacOS X install)

    eError Result = static_cast<eError>(FSFindFolder(
        kOnAppropriateDisk, kFrameworksFolderType, TRUE, &frameworksFolderRef));

    if (!Result) {

        // Convert the FSRef into a URL

        CFURLRef baseURL = CFURLCreateFromFSRef(0, &frameworksFolderRef);

        Result = kErrorOutOfMemory;
        if (baseURL) {

            // Convert to a string ref

            CFStringRef StringRef =
                CFStringCreateWithCString(0, pName, kCFStringEncodingUTF8);
            CFURLRef bundleURL = CFURLCreateCopyAppendingPathComponent(
                0, baseURL, StringRef, FALSE);

            // Dispose of the string ref
            CFRelease(StringRef);

            if (bundleURL) {
                CFBundleRef bundleRef = CFBundleCreate(0, bundleURL);
                if (bundleRef) {
                    if (!CFBundleLoadExecutable(bundleRef)) {
                        CFRelease(bundleRef);
                        Result = kErrorReadFailure;
                    } else {
                        m_pBundle = bundleRef;
                        Result = kErrorNone;
                    }
                }
                CFRelease(bundleURL);
            }
            CFRelease(baseURL);
        }
    }
    return Result;
}

/*! ************************************

    \brief Release the Framework

    If a Framework was loaded, it will be released

***************************************/

void BURGER_API Burger::CodeFramework::Shutdown(void)
{
    if (m_pBundle) {
        CFRelease(m_pBundle);
        m_pBundle = NULL;
    }
}

/*! ************************************

    \brief Locate a function pointer in a Framework

    Return a function pointer to a procedure or data contained within a shared
    library

    \param pFunctionName Pointer to a "C" string in UTF-8
        encoding of the function to locate.
    \return \ref NULL on failure, or valid pointer on success

***************************************/

void* BURGER_API Burger::CodeFramework::GetFunction(const char* pFunctionName)
{
    // Assume failure
    void* pResult = NULL;

    // Is the lib valid?
    if (m_pBundle) {

        // Convert to a string ref
        CFStringRef StringRef =
            CFStringCreateWithCString(0, pFunctionName, kCFStringEncodingUTF8);

        // Get the function
        pResult = CFBundleGetFunctionPointerForName(m_pBundle, StringRef);

        // Dispose of the string ref
        CFRelease(StringRef);
    }
    return pResult; /* Return 0 or the pointer */
}

/*! ************************************

    \brief Load InterfaceLib for manual linking.

    Load in the shared system library ``InterfaceLib``.

    \return Pointer to Burger::CodeLibrary that has InterfaceLib.

***************************************/

Burger::CodeLibrary* BURGER_API Burger::GetInterfaceLib(void)
{
    // Reference to InterfaceLib
    static CodeLibrary s_Interface;

    // Is InterfaceLib installed?
    if (!s_Interface.IsInitialized()) {
        // Try to get the lib
        s_Interface.Init("InterfaceLib");
    }
    return &s_Interface;
}

/*! ************************************

    \brief Load DriverLoaderLib for manual linking.

    Load in the shared system library ``DriverLoaderLib``.

    \return Pointer to Burger::CodeLibrary that has DriverLoaderLib.

***************************************/

Burger::CodeLibrary* BURGER_API Burger::GetDriverLoaderLib(void)
{
    // Reference to DriverLoaderLib
    static CodeLibrary s_Interface;

    // Is DriverLoaderLib installed?
    if (!s_Interface.IsInitialized()) {
        // Try to get the lib
        s_Interface.Init("DriverLoaderLib");
    }
    return &s_Interface;
}

/*! ************************************

    \brief Load NameRegistryLib for manual linking.

    Load in the shared system library ``NameRegistryLib``.

    \return Pointer to Burger::CodeLibrary that has NameRegistryLib.

***************************************/

Burger::CodeLibrary* BURGER_API Burger::GetNameRegistryLib(void)
{
    // Reference to DriverLoaderLib
    static CodeLibrary s_Interface;

    // Is DriverLoaderLib installed?
    if (!s_Interface.IsInitialized()) {
        // Try to get the lib
        s_Interface.Init("NameRegistryLib");
    }
    return &s_Interface;
}

#endif
