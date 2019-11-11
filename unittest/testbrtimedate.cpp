/***************************************

    Unit tests for the Date/Time manager

    Copyright (c) 1995-2020 by Rebecca Ann Heineman <becky@burgerbecky.com>

    It is released under an MIT Open Source license. Please see LICENSE for
    license details. Yes, you can use it in a commercial title without paying
    anything, just give me a credit.

    Please? It's not like I'm asking you for money!

***************************************/

#include "testbrtimedate.h"
#include "brcapturestdout.h"
#include "brfloatingpoint.h"
#include "brnumberstringhex.h"
#include "brstdouthelpers.h"
#include "brtimedate.h"
#include "common.h"

using namespace Burger;

/***************************************

    Test the clear function

***************************************/

static uint_t BURGER_API TestBurgerTimeDateClear(void)
{
    Burger::TimeDate_t Buffer[3];

    BlastBuffer(Buffer, sizeof(Buffer));
    Buffer[1].Clear();
    uint_t uFailure =
        VerifyBuffer(Buffer, sizeof(Buffer), &Buffer[1], sizeof(Buffer[1]));
    if (Buffer[1].m_uYear || Buffer[1].m_bMonth || Buffer[1].m_bDay ||
        Buffer[1].m_bDayOfWeek || Buffer[1].m_bHour || Buffer[1].m_bMinute ||
        Buffer[1].m_bSecond || Buffer[1].m_usMilliseconds) {
        uFailure = TRUE;
    }
    ReportFailure("Burger::TimeDate_t::Clear() didn't set all entries to zero.",
        uFailure);
    return uFailure;
}

//
// Perform all the tests for the Burgerlib Time Manager
//

int BURGER_API TestDateTime(uint_t uVerbose)
{
    uint_t uResult; // Assume no failures

    if (uVerbose & VERBOSE_MSG) {
        Message("Running Date/Time tests");
    }

    // Test compiler switches
    uResult = TestBurgerTimeDateClear();

    if (!uResult && (uVerbose & VERBOSE_TIME)) {
        char Buffer[256];
        Burger::TimeDate_t MyDate;
        MyDate.GetTime();
        MyDate.DateToStringVerbose(Buffer);
        Message("TimeDate_t::DateToStringVerbose() %s", Buffer);
        MyDate.DateToString(Buffer);
        Message("TimeDate_t::DateToString() %s", Buffer);
        MyDate.TimeToString(Buffer);
        Message("TimeDate_t::TimeToString() %s", Buffer);
        MyDate.TimeToStringPM(Buffer);
        Message("TimeDate_t::TimeToStringPM() %s", Buffer);
    }
    return static_cast<int>(uResult);
}

//
// Test PrintHexDigit(uint_t)
//

static uint_t BURGER_API TestPrintHexDigit(void)
{
    // Capture the string output
    Burger::CaptureStdout Capture;
    Capture.Init(512);
    Burger::PrintHexDigit(0x12345678);
    String CapturedString;
    Capture.GetCapture(&CapturedString);
    Capture.Shutdown();

    const char* pExpected = "8";
    uint_t uTest = Burger::StringCompare(CapturedString.GetPtr(), pExpected) != 0;
    ReportFailure("Burger::PrintHexDigit(0x12345678) = %s, expected %s", uTest,
        CapturedString.GetPtr(), pExpected);
    return uTest;
}

//
// Test PrintHex(uint8_t)
//

static uint_t BURGER_API TestPrintHexWord8(void)
{
    // Capture the string output
    Burger::CaptureStdout Capture;
    Capture.Init(512);
    Burger::PrintHex(static_cast<uint8_t>(0x12));
    String CapturedString;
    Capture.GetCapture(&CapturedString);
    Capture.Shutdown();

    const char* pExpected = "12";
    uint_t uTest = Burger::StringCompare(CapturedString.GetPtr(), pExpected) != 0;
    ReportFailure(
        "Burger::PrintHex(static_cast<uint8_t>(0x12)) = %s, expected %s", uTest,
        CapturedString.GetPtr(), pExpected);
    return uTest;
}

//
// Test PrintHex(uint16_t)
//

static uint_t BURGER_API TestPrintHexWord16(void)
{
    // Capture the string output
    Burger::CaptureStdout Capture;
    Capture.Init(512);
    Burger::PrintHex(static_cast<uint16_t>(0x1234));
    String CapturedString;
    Capture.GetCapture(&CapturedString);
    Capture.Shutdown();

    const char* pExpected = "1234";
    uint_t uTest = Burger::StringCompare(CapturedString.GetPtr(), pExpected) != 0;
    ReportFailure(
        "Burger::PrintHex(static_cast<uint16_t>(0x1234)) = %s, expected %s",
        uTest, CapturedString.GetPtr(), pExpected);
    return uTest;
}

//
// Test PrintHex(uint32_t)
//

static uint_t BURGER_API TestPrintHexWord32(void)
{
    // Capture the string output
    Burger::CaptureStdout Capture;
    Capture.Init(512);
    Burger::PrintHex(static_cast<uint32_t>(0x12345678));
    String CapturedString;
    Capture.GetCapture(&CapturedString);
    Capture.Shutdown();

    const char* pExpected = "12345678";
    uint_t uTest = Burger::StringCompare(CapturedString.GetPtr(), pExpected) != 0;
    ReportFailure(
        "Burger::PrintHex(static_cast<uint32_t>(0x12345678)) = %s, expected %s",
        uTest, CapturedString.GetPtr(), pExpected);
    return uTest;
}

//
// Test PrintHex(uint64_t)
//

static uint_t BURGER_API TestPrintHexWord64(void)
{
    // Capture the string output
    Burger::CaptureStdout Capture;
    Capture.Init(512);
    Burger::PrintHex(static_cast<uint64_t>(0x123456789ABCDEFULL));
    String CapturedString;
    Capture.GetCapture(&CapturedString);
    Capture.Shutdown();

    const char* pExpected = "0123456789ABCDEF";
    uint_t uTest = Burger::StringCompare(CapturedString.GetPtr(), pExpected) != 0;
    ReportFailure(
        "Burger::PrintHex(static_cast<uint64_t>(0x123456789ABCDEFULL)) = %s, expected %s",
        uTest, CapturedString.GetPtr(), pExpected);
    return uTest;
}

//
// Test PrintHex(float)
//

struct PrintFloat_t {
    uint32_t m_uData;
    const char* m_pData;
};

static const PrintFloat_t PrintHexFloatTestTable[] = {
    {0x7FFFFFFF, "7FFFFFFF"}, {0x3F800000, "3F800000"}};

static uint_t BURGER_API TestPrintHexFloat(void)
{
    uint_t uFailure = FALSE;
    const PrintFloat_t* pWork = PrintHexFloatTestTable;
    uintptr_t uCount = BURGER_ARRAYSIZE(PrintHexFloatTestTable);
    do {
        // Capture the string output
        Burger::CaptureStdout Capture;
        Capture.Init(512);
        Burger::PrintHex(static_cast<const float*>(
            static_cast<const void*>(&pWork->m_uData))[0]);
        String CapturedString;
        Capture.GetCapture(&CapturedString);
        Capture.Shutdown();

        uint_t uTest =
            Burger::StringCompare(CapturedString.GetPtr(), pWork->m_pData) != 0;
        ReportFailure(
            "Burger::PrintHex(static_cast<float>(0x%08X)) = %s, expected %s",
            uTest, pWork->m_uData, CapturedString.GetPtr(), pWork->m_pData);
        uFailure |= uTest;
        ++pWork;
    } while (--uCount);
    return uFailure;
}

//
// Test PrintHex(double)
//

struct PrintDouble_t {
    uint64_t m_uData;
    const char* m_pData;
};

static const PrintDouble_t PrintHexDoubleTestTable[] = {
    {0x7FFFFFFFFFFFFFFFULL, "7FFFFFFFFFFFFFFF"},
    {0x3FF0000000000000ULL, "3FF0000000000000"}};

static uint_t BURGER_API TestPrintHexDouble(void)
{
    uint_t uFailure = FALSE;
    const PrintDouble_t* pWork = PrintHexDoubleTestTable;
    uintptr_t uCount = BURGER_ARRAYSIZE(PrintHexDoubleTestTable);
    do {
        // Capture the string output
        Burger::CaptureStdout Capture;
        Capture.Init(512);
        Burger::PrintHex(static_cast<const double*>(
            static_cast<const void*>(&pWork->m_uData))[0]);
        String CapturedString;
        Capture.GetCapture(&CapturedString);
        Capture.Shutdown();

        uint_t uTest =
            Burger::StringCompare(CapturedString.GetPtr(), pWork->m_pData) != 0;
        if (uTest) {
            Burger::NumberStringHex Input(pWork->m_uData);
            ReportFailure(
                "Burger::PrintHex(static_cast<double>(0x%s)) = %s, expected %s",
                uTest, Input.GetPtr(), CapturedString.GetPtr(), pWork->m_pData);
        }
        uFailure |= uTest;
        ++pWork;
    } while (--uCount);
    return uFailure;
}

//
// Test for generic types for proper remapping
//

static void TestPrintHexes(void)
{
    Burger::CaptureStdout Capture;
    Capture.Init(1024);

    Burger::PrintHex('a');
#if defined(BURGER_HAS_WCHAR_T)
    Burger::PrintHex(L'a');
#endif

    Burger::PrintHex(1);
    Burger::PrintHex(1U);
    Burger::PrintHex(1LL);
    Burger::PrintHex(1ULL);
    Burger::PrintHex(1.0);
    Burger::PrintHex(1.0f);

    Burger::PrintHex(static_cast<char>(1));
    Burger::PrintHex(static_cast<signed char>(1));
    Burger::PrintHex(static_cast<unsigned char>(1));

    Burger::PrintHex(static_cast<short>(1));
    Burger::PrintHex(static_cast<signed short>(1));
    Burger::PrintHex(static_cast<unsigned short>(1));

    Burger::PrintHex(static_cast<int>(1));
    Burger::PrintHex(static_cast<unsigned int>(1));
    Burger::PrintHex(static_cast<signed char>(1));

    Burger::PrintHex(static_cast<long>(1));
    Burger::PrintHex(static_cast<unsigned long>(1));
    Burger::PrintHex(static_cast<signed long>(1));

    Burger::PrintHex(static_cast<long long>(1));
    Burger::PrintHex(static_cast<unsigned long long>(1));
    Burger::PrintHex(static_cast<signed long long>(1));
}

//
// Perform all the tests for the Burgerlib Stdouthelpers Manager
//

int BURGER_API TestStdoutHelpers(uint_t uVerbose)
{
    uint_t uResult = FALSE; // Assume no failures

    if (uVerbose & VERBOSE_MSG) {
        Message("Running Stdout Helpers tests");
    }

    uResult |= TestPrintHexDigit();
    uResult |= TestPrintHexWord8();
    uResult |= TestPrintHexWord16();
    uResult |= TestPrintHexWord32();
    uResult |= TestPrintHexWord64();
    uResult |= TestPrintHexFloat();
    uResult |= TestPrintHexDouble();
    TestPrintHexes();

    if (!uResult && (uVerbose & VERBOSE_MSG)) {
        Message("Passed all Stdout Helpers tests!");
    }
    return static_cast<int>(uResult);
}
