/***************************************

    Unit tests for the File Manager library

    Copyright (c) 1995-2017 by Rebecca Ann Heineman <becky@burgerbecky.com>

    It is released under an MIT Open Source license. Please see LICENSE for
    license details. Yes, you can use it in a commercial title without paying
    anything, just give me a credit.

    Please? It's not like I'm asking you for money!

***************************************/

#include "testbrfilemanager.h"
#include "common.h"

#include "brdirectorysearch.h"
#include "brfile.h"
#include "brfilemanager.h"
#include "brfilename.h"
#include "brmemoryansi.h"
#include "brmemoryfunctions.h"

#if defined(BURGER_WINDOWS)
#include <windows.h>
#undef CopyFile
#undef DeleteFile
#endif

#define TM "\xE2\x84\xA2"
#define COPYRIGHT "\xC2\xA9"
#if defined(BURGER_MSDOS)
#define SAILORMOON "Bishojo Senshi Sailor Moon"
#define SAILORMOONSHORT "MOON"
#else
#define SAILORMOON                                                             \
	"\xE7\xBE\x8E\xE5\xB0\x91\xE5\xA5\xB3\xE6\x88"                             \
	"\xA6\xE5\xA3\xAB\xE3\x82\xBB\xE3\x83\xBC\xE3\x83\xA9\xE3\x83\xBC"         \
	"\xE3\x83\xA0\xE3\x83\xBC\xE3\x83\xB3"
#endif
using namespace Burger;

//
// This string it longer than 512 bytes. This is IMPORTANT for certain
// unit tests.
//

static const char g_LongFileName[] =
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename"
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename"
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename"
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename"
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename"
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename"
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename"
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename"
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename"
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename"
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename"
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename"
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename"
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename"
	"ThisIsAReallyLongFilenameThisIsAReallyLongFilenameThisIsAReallyLongFilename";

static const char g_SampleData[] = "\nThis is a test, this only a test.";
static const char g_ShortFileName[] = "Foobar";

/***************************************

	Primary test!

	Verify that the class is exactly 512 bytes in size
	This is so that filenames on local storage don't take up
	too much room on the stack so the stack is filled

	This test MUST pass. Failure of this test will mean
	that assumptions may be invalid as to stack memory usage.

***************************************/

static Word BURGER_API TestFilenameClass(void)
{
	// The structure size test
	Word uFailure = (sizeof(Filename) != 512);
	ReportFailure(
		"sizeof(Filename) = %d / Wanted (512)", uFailure, sizeof(Filename));

	// Test for default string initialization
	Filename TestName;
	const char* pTest = TestName.GetPtr();
	Word uTest = pTest == NULL;
	uFailure |= uTest;
	ReportFailure("Filename.GetPtr() = %p, expected non NULL)", uTest, pTest);

	// Test for default empty string
	// If NULL, the previous test would have reported the failure
	if (pTest) {
		uTest = StringCompare("", pTest) != 0;
		uFailure |= uTest;
		ReportFailure("Filename = \"%s\", expected \"\"", uTest, pTest);
	}
	return uFailure;
}

/***************************************

	Test Filename.Set()

***************************************/

static Word BURGER_API TestFilenameSet(void)
{
	Word uFailure = FALSE;
	// This test will start with a large string and then
	// get smaller.
	Word i;
	const char* pWork;

	{
		Filename TestName;
		i = static_cast<Word>(sizeof(g_LongFileName));
		pWork = g_LongFileName;
		do {
			// Set the string
			TestName.Set(pWork);
			const char* pTest = TestName.GetPtr();
			Word uTest = StringCompare(pWork, pTest) != 0;
			uFailure |= uTest;
			ReportFailure("Filename.Set(%s) = \"%s\", expected \"\"", uTest,
				pTest, pWork);
			++pWork;
		} while (--i);
	}

	i = static_cast<Word>(sizeof(g_LongFileName));
	pWork = g_LongFileName;
	do {
		// Set the string
		Filename TestName(pWork);
		const char* pTest = TestName.GetPtr();
		Word uTest = StringCompare(pWork, pTest) != 0;
		uFailure |= uTest;
		ReportFailure(
			"Filename(%s) = \"%s\", expected \"\"", uTest, pTest, pWork);
		++pWork;
	} while (--i);

	// Do the reverse. Start small, get larger
	{
		i = static_cast<Word>(sizeof(g_LongFileName));
		pWork = g_LongFileName + (sizeof(g_LongFileName) - 1);
		Filename TestName;
		do {
			// Set the string
			TestName.Set(pWork);
			const char* pTest = TestName.GetPtr();
			Word uTest = StringCompare(pWork, pTest) != 0;
			uFailure |= uTest;
			ReportFailure("Filename.Set(%s) = \"%s\", expected \"\"", uTest,
				pTest, pWork);
			--pWork;
		} while (--i);
	}

	i = static_cast<Word>(sizeof(g_LongFileName));
	pWork = g_LongFileName + (sizeof(g_LongFileName) - 1);
	do {
		// Set the string
		Filename TestName(pWork);
		const char* pTest = TestName.GetPtr();
		Word uTest = StringCompare(pWork, pTest) != 0;
		uFailure |= uTest;
		ReportFailure(
			"Filename(%s) = \"%s\", expected \"\"", uTest, pTest, pWork);
		--pWork;
	} while (--i);

	return uFailure;
}

/***************************************

	Test Filename.Clear()

***************************************/

static Word BURGER_API TestFilenameClear(void)
{
	Word uFailure = FALSE;

	// Test if clearing resets the buffer pointer

	Filename TestName;
	// Get the default pointer (Known to be the original buffer)
	const char* pReference = TestName.GetPtr();
	TestName.Clear();
	const char* pTest = TestName.GetPtr();
	Word uTest = pReference != pTest;
	uFailure |= uTest;
	ReportFailure(
		"Filename::Clear() = %p, expected %p", uTest, pTest, pReference);

	// Set a short string, then blank it
	TestName.Set(g_ShortFileName);
	TestName.Clear();
	pTest = TestName.GetPtr();
	uTest = pTest[0] != 0;
	uFailure |= uTest;
	ReportFailure("Filename::Clear() = \"%s\", expected \"\"", uTest, pTest);

	// Set the string, then blank it
	TestName.Set(g_LongFileName);
	TestName.Clear();
	pTest = TestName.GetPtr();
	uTest = pTest[0] != 0;
	uFailure |= uTest;
	ReportFailure("Filename::Clear() = \"%s\", expected \"\"", uTest, pTest);

	// Verify the buffer is the default one after a memory allocation
	// since the previous test created a giant string
	uTest = pReference != pTest;
	uFailure |= uTest;
	ReportFailure(
		"Filename::Clear() = %p, expected %p", uTest, pTest, pReference);

	return uFailure;
}

/***************************************

	Test Filename.IsFullPathname()

***************************************/

struct IsTests_t {
	const char* m_pTest;
	Word m_uFull;
	Word m_uOnly;
	Word m_uPrefix;
};

static const IsTests_t IsFullTests[] = {
	{".d3:foo", TRUE, FALSE, FileManager::PREFIXINVALID},
	{".d31:foo", TRUE, FALSE, FileManager::PREFIXINVALID},
	{".d:foo", FALSE, TRUE, FileManager::PREFIXINVALID},
	{":foo:bar", TRUE, FALSE, FileManager::PREFIXINVALID},
	{".:folder", FALSE, TRUE, FileManager::PREFIXINVALID},
	{"temp.txt", FALSE, TRUE, FileManager::PREFIXINVALID},
	{"temp", FALSE, TRUE, FileManager::PREFIXINVALID}, {"8:", FALSE, FALSE, 8},
	{"12:this:is:a:path", FALSE, FALSE, 12},
	{"20:twenty.txt", FALSE, FALSE, 20}, {"8:foo", FALSE, FALSE, 8},
	{"$:foo", FALSE, FALSE, FileManager::PREFIXSYSTEM},
	{"@:foo", FALSE, FALSE, FileManager::PREFIXPREFS},
	{"*:foo", FALSE, FALSE, FileManager::PREFIXBOOT}};

static Word BURGER_API TestFilenameIs(void)
{
	Word uFailure = FALSE;
	Filename TestName;

	Word i = static_cast<Word>(BURGER_ARRAYSIZE(IsFullTests));
	const IsTests_t* pIsTests = IsFullTests;
	do {
		TestName.Set(pIsTests->m_pTest);
		Word uResult = TestName.IsFullPathname();
		Word uTest = uResult != pIsTests->m_uFull;
		uFailure |= uTest;
		ReportFailure("Filename::IsFullPathname(\"%s\") = %d, expected %d",
			uTest, pIsTests->m_pTest, uResult, pIsTests->m_uFull);

		uResult = TestName.IsFilenameOnly();
		uTest = uResult != pIsTests->m_uOnly;
		uFailure |= uTest;
		ReportFailure("Filename::IsFilenameOnly(\"%s\") = %d, expected %d",
			uTest, pIsTests->m_pTest, uResult, pIsTests->m_uOnly);

		uResult = TestName.ParsePrefixNumber();
		uTest = uResult != pIsTests->m_uPrefix;
		uFailure |= uTest;
		ReportFailure("Filename::ParsePrefixNumber(\"%s\") = %d, expected %d",
			uTest, pIsTests->m_pTest, uResult, pIsTests->m_uPrefix);

		++pIsTests;
	} while (--i);

	return uFailure;
}

/***************************************

	Show directories

***************************************/

static void BURGER_API TestShowDirectories(void)
{
	Filename Test;

	Word uLong = FileManager::AreLongFilenamesAllowed();
	Message("FileManager::AreLongFilenamesAllowed() is %u", uLong);

	// Display the directories
	Test.SetSystemWorkingDirectory();
	Message("Current working directory is %s", Test.GetPtr());
	Test.SetApplicationDirectory();
	Message("Application directory is %s", Test.GetPtr());
	Test.SetMachinePrefsDirectory();
	Message("Machine prefs directory is %s", Test.GetPtr());
	Test.SetUserPrefsDirectory();
	Message("User prefs directory is %s", Test.GetPtr());
}

/***************************************

	Test FileManager.GetVolumeName()

***************************************/

static Word BURGER_API TestGetVolumeName(Word uVerbose)
{
	// No automated error check is possible since this will generate different
	// output depending on the machine this is run on

	if (uVerbose & VERBOSE_FILE) {
		Filename MyFilename;
		Word i = 0;
		do {
			if (!FileManager::GetVolumeName(&MyFilename, i)) {
				Message("Drive %d is named \"%s\"", i, MyFilename.GetPtr());
			}
		} while (++i < 10);
	}
	return 0;
}

/***************************************

	Test FileManager.DefaultPrefixes()

***************************************/

typedef struct PrefixName_t {
	Word m_uPrefix;
	const char* m_pPrefixName;
} PrefixName_t;

static const PrefixName_t PrefixNameTable[] = {
	{FileManager::PREFIXCURRENT, "PREFIXCURRENT"},
	{FileManager::PREFIXAPPLICATION, "PREFIXAPPLICATION"},
	{FileManager::PREFIXBOOT, "PREFIXBOOT"},
	{FileManager::PREFIXPREFS, "PREFIXPREFS"},
	{FileManager::PREFIXSYSTEM, "PREFIXSYSTEM"}};

static Word BURGER_API TestPrefixes(Word uVerbose)
{
	Filename MyFileName;

	// For testing, print out the current prefixes. Useful for debugging
	// on new platforms.

	if (uVerbose & VERBOSE_FILE) {
		Word i = static_cast<Word>(BURGER_ARRAYSIZE(PrefixNameTable));
		const PrefixName_t* pWork = PrefixNameTable;
		do {
			FileManager::GetPrefix(&MyFileName, pWork->m_uPrefix);
			Message("FileManager::%s = \"%s\"", pWork->m_pPrefixName,
				MyFileName.GetPtr());
			++pWork;
		} while (--i);
	}

	// Check if all the other prefixes are empty on startup.

	Word i = 0;
	Word uFailure = FALSE;
	do {
		if ((i != FileManager::PREFIXCURRENT)
			&& (i != FileManager::PREFIXAPPLICATION)) {
			FileManager::GetPrefix(&MyFileName, i);
			const char* pTest = MyFileName.GetPtr();
			Word uTest = pTest[0] != 0;
			uFailure |= uTest;
			ReportFailure(
				"FileManager::GetPrefix(%d) = \"%s\"", uTest, i, pTest);
		}
	} while (++i < FileManager::PREFIXBOOT);
	return uFailure;
}

/***************************************

	Test Filename.DirName()

***************************************/

struct TestDirName_t {
	const char* m_pOriginal;
	const char* m_pExpected;
};

static const TestDirName_t TestDirNames[] = {{":foo:bar", ":foo:"},
	{".d3:foo:bar", ".d3:foo:"}, {":foo:", ":foo:"}, {".d3:", ".d3:"},
	{":foo:bar:", ":foo:"},
	{":one:two:three:four:five", ":one:two:three:four:"},
	{":one:two:three:four:five:", ":one:two:three:four:"}, {"one", "one"},
	{"one:two", "one:"}, {"one:two:", "one:"}};

static Word BURGER_API TestFilenameDirName(void)
{
	Word uFailure = 0;
	const TestDirName_t* pTestDirName = TestDirNames;
	Word i = static_cast<Word>(BURGER_ARRAYSIZE(TestDirNames));
	do {
		Filename Test(pTestDirName->m_pOriginal);
		Test.DirName();
		Word uTest =
			StringCompare(Test.GetPtr(), pTestDirName->m_pExpected) != 0;
		uFailure |= uTest;
		ReportFailure("DirName \"%s\" to \"%s\" but got \"%s\"", uTest,
			pTestDirName->m_pOriginal, pTestDirName->m_pExpected,
			Test.GetPtr());
		++pTestDirName;
	} while (--i);
	return uFailure;
}

/***************************************

	Test FileManager.Append()

***************************************/

struct TestAppend_t {
	const char* m_pOriginal;
	const char* m_pAppend;
	const char* m_pExpected;
};

static const TestAppend_t TestAppends[] = {
	{":foo:bar", "foo:", ":foo:bar:foo:"},
	{".d3:foo:bar", ".d3:foo:", ".d3:foo:bar:.d3:foo:"},
	{":foo:", "foo", ":foo:foo:"}, {".d3:", "bar:", ".d3:bar:"},
	{":foo:bar:", "foo:", ":foo:bar:foo:"},
	{":one:two:three:four:five",
		"one:two:three:four:", ":one:two:three:four:five:one:two:three:four:"},
	{":one:two:three:four:five:", "one:two:three:four:",
		":one:two:three:four:five:one:two:three:four:"},
	{"one", "one", "one:one:"}, {"one:two", "one:", "one:two:one:"},
	{"one:two:", "one:", "one:two:one:"}};

static Word BURGER_API TestFilenameAppend(void)
{
	Word uFailure = 0;
	const TestAppend_t* pTestAppend = TestAppends;
	Word i = static_cast<Word>(BURGER_ARRAYSIZE(TestAppends));
	do {
		Filename Test(pTestAppend->m_pOriginal);
		Test.Append(pTestAppend->m_pAppend);
		Word uTest =
			StringCompare(Test.GetPtr(), pTestAppend->m_pExpected) != 0;
		uFailure |= uTest;
		ReportFailure(
			"Append \"%s\" to \"%s\" but got \"%s\" and expected \"%s\"", uTest,
			pTestAppend->m_pOriginal, pTestAppend->m_pAppend, Test.GetPtr(),
			pTestAppend->m_pExpected);
		++pTestAppend;
	} while (--i);
	return uFailure;
}

/***************************************

	Test Filename.Expand() prepending the current directory

***************************************/

static Word BURGER_API TestExpandFull(const char* pInput)
{
	Word uFailure = FALSE;

	String TempString(pInput);
	Filename MyFilename(pInput);
	Word uTest = StringCompare(pInput, MyFilename.GetPtr()) != 0;
	uFailure |= uTest;
	ReportFailure(
		"TestExpandFull() Original is \"%s\"", uTest, MyFilename.GetPtr());

	MyFilename.Expand();
	TempString += ':';
	uTest = StringCompare(TempString.GetPtr(), MyFilename.GetPtr()) != 0;
	uFailure |= uTest;
	ReportFailure(
		"TestExpandFull() Expand from \"%s\" to \"%s\" but got \"%s\"", uTest,
		pInput, TempString.GetPtr(), MyFilename.GetPtr());
	return uFailure;
}

/***************************************

	Test Filename.Expand() prepending the current directory

***************************************/

static Word BURGER_API TestPrepend(const char* pInput, const Filename* pWorkDir)
{
	Word uFailure = FALSE;

	String TempString;

	Filename MyFilename(pInput);
	Word uTest = StringCompare(pInput, MyFilename.GetPtr()) != 0;
	uFailure |= uTest;
	ReportFailure(
		"TestPrepend() Original is \"%s\"", uTest, MyFilename.GetPtr());

	MyFilename.Expand();
	TempString.Set(pWorkDir->GetPtr());
	TempString += pInput;
	TempString += ':';
	uTest = StringCompare(TempString.GetPtr(), MyFilename.GetPtr()) != 0;
	uFailure |= uTest;
	ReportFailure("Expand from \"%s\" to \"%s\" but got \"%s\"", uTest, pInput,
		TempString.GetPtr(), MyFilename.GetPtr());
	return uFailure;
}

/***************************************

	Test Filename.Expand() popping current directory

***************************************/

static const char g_PeriodName[] = "........:Help me pfx";

static Word BURGER_API TestPopDir(const Filename* pWorkDir)
{
	const Word uPeriodCount = 8;
	Word uFailure = FALSE;
	String TempString;
	Filename MyFilename;
	Word i = uPeriodCount;
	do {
		TempString.Set((g_PeriodName - 1) + i);
		MyFilename.Set(TempString);

		Word uTest =
			StringCompare(TempString.GetPtr(), MyFilename.GetPtr()) != 0;
		uFailure |= uTest;
		ReportFailure(
			"TestPopDir() Original is \"%s\"", uTest, MyFilename.GetPtr());

		MyFilename.Expand();

		Filename TempDir(*pWorkDir);
		Word y = uPeriodCount - i;
		if (y) {
			do {
				TempDir.DirName();
			} while (--y);
		}

		TempDir.Append(g_PeriodName + uPeriodCount + 1);
		uTest = StringCompare(TempDir.GetPtr(), MyFilename.GetPtr()) != 0;
		uFailure |= uTest;
		ReportFailure("Expand from \"%s\" to \"%s\" but got \"%s\"", uTest,
			TempString.GetPtr(), TempDir.GetPtr(), MyFilename.GetPtr());

	} while (--i);
	return uFailure;
}

/***************************************

	Test Filename.Expand() inserting a prefix

***************************************/

static Word BURGER_API TestPrefixDir(const char* pInput, Word uPrefix)
{
	Word uFailure = FALSE;

	Filename MyFilename(pInput);
	Word uTest = StringCompare(pInput, MyFilename.GetPtr()) != 0;
	uFailure |= uTest;
	ReportFailure(
		"TestPrefixDir() Original is \"%s\"", uTest, MyFilename.GetPtr());

	MyFilename.Expand();
	Filename TempString;
	FileManager::GetPrefix(&TempString, uPrefix);
	TempString.Append(StringCharacter(pInput, ':')+1);

	uTest = StringCompare(TempString.GetPtr(), MyFilename.GetPtr()) != 0;
	uFailure |= uTest;
	ReportFailure("Expand from \"%s\" to \"%s\" but got \"%s\"", uTest, pInput,
		TempString.GetPtr(), MyFilename.GetPtr());
	return uFailure;
}

/***************************************

	Test Filename.Expand()

***************************************/

static Word BURGER_API TestFilenameExpand(void)
{
	// Boot volume name is needed for tests to work on multiple platforms
	Filename BootVolume;
	Filename WorkBootDirectory;
	Filename SavedCurrentDir;
	FileManager::GetPrefix(&BootVolume, FileManager::PREFIXBOOT);
	FileManager::GetPrefix(&SavedCurrentDir, FileManager::PREFIXCURRENT);
	WorkBootDirectory = BootVolume;
	WorkBootDirectory.Append("Two:Three:Four:Five");
	FileManager::SetPrefix(
		FileManager::PREFIXCURRENT, WorkBootDirectory.GetPtr());

	Word uFailure = TestExpandFull(".D2:Help me");
	uFailure |= TestExpandFull(":Burger:foo.txt");
	uFailure |= TestExpandFull(":Burger:a:foo.txt");
	uFailure |= TestExpandFull(":Burger:9.txt");

	uFailure |= TestPrepend(".D:Help me", &WorkBootDirectory);
	uFailure |= TestPrepend(".D2x:Help me", &WorkBootDirectory);
	uFailure |= TestPrepend(".Help me start", &WorkBootDirectory);

	uFailure |= TestPopDir(&WorkBootDirectory);
	uFailure |= TestPrefixDir("8:Six:Help me pfx", FileManager::PREFIXCURRENT);
	uFailure |= TestPrefixDir("8:Help me pfx", FileManager::PREFIXCURRENT);
	uFailure |= TestPrefixDir("*:Help me pfx", FileManager::PREFIXBOOT);
	uFailure |= TestPrefixDir("$:Help me pfx", FileManager::PREFIXSYSTEM);
	uFailure |= TestPrefixDir("@:Help me pfx", FileManager::PREFIXPREFS);
	uFailure |= TestPrefixDir("9:Help me pfx", FileManager::PREFIXAPPLICATION);

	// Restore the current directory
	FileManager::SetPrefix(
		FileManager::PREFIXCURRENT, SavedCurrentDir.GetPtr());
	return uFailure;
}

/***************************************

	Test File

***************************************/

static Word BURGER_API TestFile(Word uVerbose)
{
	Word uFailure = 0;
	TimeDate_t Time;
#if defined(BURGER_MSDOS)
	Filename TestName;
	if (FileManager::AreLongFilenamesAllowed()) {
		TestName.Set("9:" SAILORMOON ".txt");
	} else {
		TestName.Set("9:" SAILORMOONSHORT "2.txt");
	}
#else
	Filename TestName("9:" SAILORMOON ".txt");
#endif
	File TestFile(&TestName, File::WRITEONLY);

	Word uTest =
		TestFile.Write("This is a test", sizeof("This is a test") - 1) != 14;
	uFailure |= uTest;
	ReportFailure("File::Write", uTest);

	uTest = TestFile.GetModificationTime(&Time) != File::OKAY;
	uFailure |= uTest;
	ReportFailure("File::GetModificationTime", uTest);
	if (uTest || (uVerbose & VERBOSE_FILE)) {
		char Output[128];
		char Output2[128];
		Time.DateToStringVerbose(Output);
		Time.TimeToStringPM(Output2);
		Message("File modification time = %s %s",Output,Output2);
	}
	WordPtr uSize = TestFile.GetSize();
	uTest = uSize != 14;
	uFailure |= uTest;
	ReportFailure("File::GetSize return %d, expected 14", uTest, uSize);

	uTest = TestFile.Close() != File::OKAY;
	uFailure |= uTest;
	ReportFailure("File::Close", uTest);
	return uFailure;
}

/***************************************

	Create some temp files

***************************************/

static void BURGER_API TestCreateTempFiles(void)
{
	FileManager::SaveFile(
		"9:testfile.txt", g_SampleData, sizeof(g_SampleData) - 1);
	FileManager::SaveFile(
		"9:testfile" TM ".txt", g_SampleData, sizeof(g_SampleData) - 1);
	FileManager::SaveFile(
		"9:" SAILORMOON ".txt", g_SampleData, sizeof(g_SampleData) - 1);
#if defined(BURGER_MACOS)
	FileManager::SetFileAndAuxType("9:testfile.txt", 'TEXT', 'CWIE');
	FileManager::SetFileAndAuxType("9:testfile" TM ".txt", 'TEXT', 'CWIE');
	FileManager::SetFileAndAuxType("9:" SAILORMOON ".txt", 'TEXT', 'CWIE');
#endif
}

/***************************************

	Dispose of the temp files

***************************************/

static void BURGER_API TestDisposeTempFiles(void)
{
	FileManager::DeleteFile("9:testfile.txt");
	FileManager::DeleteFile("9:testfile" TM ".txt");
	FileManager::DeleteFile("9:" SAILORMOON ".txt");
}


/***************************************

	Test FileManager.DoesFileExist()

***************************************/

static Word BURGER_API TestDoesFileExist(void)
{
	Word uReturn = FileManager::DoesFileExist("9:FileNotHere");
	Word uTest = uReturn != 0;
	Word uFailure = uTest;
	ReportFailure(
		"FileManager::DoesFileExist(\"9:FileNotHere\") = %d", uTest, uReturn);

	uReturn = FileManager::DoesFileExist("9:testfile.txt");
	uTest = uReturn == 0;
	uFailure |= uTest;
	ReportFailure(
		"FileManager::DoesFileExist(\"9:testfile.txt\") = %d", uTest, uReturn);

	uReturn = FileManager::DoesFileExist("9:testfile" TM ".txt");
	uTest = uReturn == 0;
	uFailure |= uTest;
	ReportFailure("FileManager::DoesFileExist(\"9:testfile" TM ".txt\") = %d",
		uTest, uReturn);

	uReturn = FileManager::DoesFileExist("9:" SAILORMOON ".txt");
	uTest = uReturn == 0;
	uFailure |= uTest;
	ReportFailure("FileManager::DoesFileExist(\"9:" SAILORMOON ".txt\") = %d",
		uTest, uReturn);

	return uFailure;
}

/***************************************

	Test the load and save commands

***************************************/

static Word BURGER_API TestLoadSave(void)
{
	// Save the file
	Word32 uReturn = FileManager::SaveFile(
		"9:tempfile" COPYRIGHT ".txt", g_SampleData, sizeof(g_SampleData) - 1);
	Word uTest = uReturn == 0;
	Word uFailure = uTest;
	ReportFailure("FileManager::SaveFile(\"9:tempfile" COPYRIGHT
				  ".txt\",g_SampleData,sizeof(g_SampleData)-1) = %d",
		uTest, uReturn);

	uReturn = FileManager::SaveFile(
		"9:" SAILORMOON ".txt", g_SampleData, sizeof(g_SampleData) - 1);
	uTest = uReturn == 0;
	uFailure |= uTest;
	ReportFailure("FileManager::SaveFile(\"9:tempfile" SAILORMOON
				  ".txt\",g_SampleData,sizeof(g_SampleData)-1) = %d",
		uTest, uReturn);

	// Determine if the file is present
	uReturn = FileManager::DoesFileExist("9:tempfile" COPYRIGHT ".txt");
	uTest = uReturn == 0;
	uFailure |= uTest;
	ReportFailure("FileManager::SaveFile(\"9:tempfile" COPYRIGHT
				  ".txt\",g_SampleData,sizeof(g_SampleData)-1 didn't save!",
		uTest);

	// Load the file
	WordPtr uLength;
	void* pReturn =
		FileManager::LoadFile("9:tempfile" COPYRIGHT ".txt", &uLength);
	uTest = (uReturn == NULL) || (uLength != (sizeof(g_SampleData) - 1));
	uFailure |= uTest;
	ReportFailure("FileManager::LoadFile(\"9:tempfile" COPYRIGHT
				  ".txt\") = %08X",
		uTest, static_cast<unsigned int>(uLength));
	if (pReturn) {
		uTest =
			MemoryCompare(pReturn, g_SampleData, sizeof(g_SampleData) - 1) != 0;
		uFailure |= uTest;
		ReportFailure("FileManager::LoadFile(\"9:tempfile" COPYRIGHT
					  ".txt\") return bad data",
			uTest);
		Free(pReturn);
	}

	// Copy the file
	uReturn = FileManager::CopyFile(
		"9:tempfile" TM ".txt", "9:tempfile" COPYRIGHT ".txt");
	uTest = uReturn != 0;
	uFailure = uTest;
	ReportFailure("FileManager::CopyFile(\"9:tempfile" TM
				  ".txt\",\"9:tempfile" COPYRIGHT ".txt\") = %d",
		uTest, uReturn);

	// Determine if the file is present
	uReturn = FileManager::DoesFileExist("9:tempfile" TM ".txt");
	uTest = uReturn == 0;
	uFailure |= uTest;
	ReportFailure("FileManager::CopyFile(\"9:tempfile" TM
				  ".txt\",\"9:tempfile" COPYRIGHT ".txt\") didn't copy!",
		uTest);

	// Rename the file
	uReturn = FileManager::RenameFile(
		"9:tempfile" TM TM ".txt", "9:tempfile" TM ".txt");
	uTest = uReturn != 0;
	uFailure = uTest;
	ReportFailure("FileManager::RenameFile(\"9:tempfile" TM TM
				  ".txt\",\"9:tempfile" TM ".txt\") = %d",
		uTest, uReturn);

	// Determine if the file is present
	uReturn = FileManager::DoesFileExist("9:tempfile" TM TM ".txt");
	uTest = uReturn == 0;
	uFailure |= uTest;
	ReportFailure("FileManager::RenameFile(\"9:tempfile" TM TM
				  ".txt\",\"9:tempfile" TM ".txt\") didn't rename!",
		uTest);

	// Check for fp opening
	FILE* fp = FileManager::OpenFile("9:tempfile" TM TM ".txt", "rb");
	uTest = fp == NULL;
	uFailure |= uTest;
	ReportFailure(
		"FileManager::OpenFile(\"9:tempfile" TM TM ".txt\",\"rb\")", uTest);
	if (fp) {
		fclose(fp);
	}

	// Delete the files
	uReturn = FileManager::DeleteFile("9:tempfile" COPYRIGHT ".txt");
	uTest = uReturn != 0;
	uFailure |= uTest;
	ReportFailure("FileManager::DeleteFile(\"9:tempfile" COPYRIGHT
				  ".txt\") = %d",
		uTest, uReturn);

	uReturn = FileManager::DeleteFile("9:tempfile" TM TM ".txt");
	uTest = uReturn != 0;
	uFailure |= uTest;
	ReportFailure("FileManager::DeleteFile(\"9:tempfile" TM TM ".txt\") = %d",
		uTest, uReturn);

	// This SHOULD fail, because the file is gone. Test for failure case
	uReturn = FileManager::DeleteFile("9:tempfile" COPYRIGHT ".txt");
	uTest = uReturn == 0;
	uFailure |= uTest;
	ReportFailure("FileManager::DeleteFile(\"9:tempfile" COPYRIGHT
				  ".txt\") (Should fail) = %d",
		uTest, uReturn);

	return uFailure;
}

/***************************************

	Test FileManager.DoesFileExist()

***************************************/

static Word BURGER_API TestGetModificationTime(Word uVerbose)
{
	TimeDate_t MyTime;
	char TempBuffer[128];
	char TempBuffer2[128];
	Word32 uReturn = FileManager::GetModificationTime("9:FileNotHere", &MyTime);
	Word uTest = uReturn == 0;
	Word uFailure = uTest;
	ReportFailure("FileManager::GetModificationTime(\"9:FileNotHere\") = %d",
		uTest, uReturn);

	uReturn = FileManager::GetModificationTime("9:testfile.txt", &MyTime);
	uTest = uReturn != 0;
	uFailure |= uTest;
	ReportFailure("FileManager::GetModificationTime(\"9:testfile.txt\") = %d",
		uTest, uReturn);
	if (!uReturn && (uVerbose & VERBOSE_FILE)) {
		MyTime.DateToStringVerbose(TempBuffer);
		MyTime.TimeToStringPM(TempBuffer2);
		Message(
			"9:testfile.txt's modification was %s %s", TempBuffer, TempBuffer2);
	}

	uReturn = FileManager::GetModificationTime("9:testfile" TM ".txt", &MyTime);
	uTest = uReturn != 0;
	uFailure |= uTest;
	ReportFailure("FileManager::GetModificationTime(\"9:testfile" TM
				  ".txt\") = %d",
		uTest, uReturn);
	if (!uReturn && (uVerbose & VERBOSE_FILE)) {
		MyTime.DateToStringVerbose(TempBuffer);
		MyTime.TimeToStringPM(TempBuffer2);
		Message("9:testfile" TM ".txt's modification was %s %s", TempBuffer,
			TempBuffer2);
	}
	return uFailure;
}

/***************************************

	Test FileManager.GetCreationTime()

***************************************/

static Word BURGER_API TestGetCreationTime(Word uVerbose)
{
	TimeDate_t MyTime;
	char TempBuffer[128];
	char TempBuffer2[128];
	Word32 uReturn = FileManager::GetCreationTime("9:FileNotHere", &MyTime);
	Word uTest = uReturn == 0;
	Word uFailure = uTest;
	ReportFailure(
		"FileManager::GetCreationTime(\"9:FileNotHere\") = %d", uTest, uReturn);

	uReturn = FileManager::GetCreationTime("9:testfile.txt", &MyTime);
	uTest = uReturn != 0;
	uFailure |= uTest;
	ReportFailure("FileManager::GetCreationTime(\"9:testfile.txt\") = %d",
		uTest, uReturn);
	if (!uReturn && (uVerbose & VERBOSE_FILE)) {
		MyTime.DateToStringVerbose(TempBuffer);
		MyTime.TimeToStringPM(TempBuffer2);
		Message("9:testfile.txt's creation was %s %s", TempBuffer, TempBuffer2);
	}

	uReturn = FileManager::GetCreationTime("9:testfile" TM ".txt", &MyTime);
	uTest = uReturn != 0;
	uFailure |= uTest;
	ReportFailure("FileManager::GetCreationTime(\"9:testfile" TM ".txt\") = %d",
		uTest, uReturn);
	if (!uReturn && (uVerbose & VERBOSE_FILE)) {
		MyTime.DateToStringVerbose(TempBuffer);
		MyTime.TimeToStringPM(TempBuffer2);
		Message("9:testfile" TM ".txt's creation was %s %s", TempBuffer,
			TempBuffer2);
	}
	return uFailure;
}

/***************************************

	Test FileManager.GetFileType()

***************************************/

static Word BURGER_API TestGetFileType(Word uVerbose)
{
#if defined(BURGER_MACOS)
	Word32 uReturn = FileManager::GetFileType("9:FileNotHere");
	Word uTest = uReturn != 0;
	Word uFailure = uTest;
	ReportFailure(
		"FileManager::GetFileType(\"9:FileNotHere\") = 0x%08X", uTest, uReturn);

	uReturn = FileManager::GetFileType("9:testfile.txt");
	uTest = uReturn == 0;
	uFailure |= uTest;
	ReportFailure("FileManager::GetFileType(\"9:testfile.txt\") = 0x%08X",
		uTest, uReturn);
	if (uReturn && (uVerbose & VERBOSE_FILE)) {
		Message("9:testfile.txt's file type was 0x%08X", uReturn);
	}

	uReturn = FileManager::GetFileType("9:testfile" TM ".txt");
	uTest = uReturn == 0;
	uFailure |= uTest;
	ReportFailure("FileManager::GetFileType(\"9:testfile" TM ".txt\") = 0x%08X",
		uTest, uReturn);
	if (uReturn && (uVerbose & VERBOSE_FILE)) {
		Message("9:testfile" TM ".txt's file type was 0x%08X", uReturn);
	}
	return uFailure;
#else
	BURGER_UNUSED(uVerbose);
	return 0;
#endif
}

/***************************************

	Test FileManager.GetAuxType()

***************************************/

static Word BURGER_API TestGetAuxType(Word uVerbose)
{
#if defined(BURGER_MACOS)
	Word32 uReturn = FileManager::GetAuxType("9:FileNotHere");
	Word uTest = uReturn != 0;
	Word uFailure = uTest;
	ReportFailure(
		"FileManager::GetAuxType(\"9:FileNotHere\") = 0x%08X", uTest, uReturn);

	uReturn = FileManager::GetAuxType("9:testfile.txt");
	uTest = uReturn == 0;
	uFailure |= uTest;
	ReportFailure(
		"FileManager::GetAuxType(\"9:testfile.txt\") = 0x%08X", uTest, uReturn);
	if (uReturn && (uVerbose & VERBOSE_FILE)) {
		Message("9:testfile.txt's aux type was 0x%08X", uReturn);
	}

	uReturn = FileManager::GetAuxType("9:testfile" TM ".txt");
	uTest = uReturn == 0;
	uFailure |= uTest;
	ReportFailure("FileManager::GetAuxType(\"9:testfile" TM ".txt\") = 0x%08X",
		uTest, uReturn);
	if (uReturn && (uVerbose & VERBOSE_FILE)) {
		Message("9:testfile" TM ".txt's aux type was 0x%08X", uReturn);
	}
	return uFailure;
#else
	BURGER_UNUSED(uVerbose);
	return 0;
#endif
}

/***************************************

	Test FileManager.GetFileAndAuxType()

***************************************/

static Word BURGER_API TestGetFileAndAuxType(Word uVerbose)
{
#if defined(BURGER_MACOS)
	Word32 uFileType;
	Word32 uAuxType;
	Word uReturn =
		FileManager::GetFileAndAuxType("9:FileNotHere", &uFileType, &uAuxType);
	Word uTest = uReturn == 0;
	Word uFailure = uTest;
	ReportFailure(
		"FileManager::GetFileAndAuxType(\"9:FileNotHere\") = %d File = 0x%08X, Aux = 0x%08X",
		uTest, uReturn, uFileType, uAuxType);

	uReturn =
		FileManager::GetFileAndAuxType("9:testfile.txt", &uFileType, &uAuxType);
	uTest = uReturn != 0;
	uFailure |= uTest;
	ReportFailure(
		"FileManager::GetFileAndAuxType(\"9:testfile.txt\") = %d File = 0x%08X, Aux = 0x%08X",
		uTest, uReturn, uFileType, uAuxType);
	if (!uReturn && (uVerbose & VERBOSE_FILE)) {
		Message("9:testfile.txt's file was 0x%08X, aux was 0x%08X", uFileType,
			uAuxType);
	}

	uReturn = FileManager::GetFileAndAuxType(
		"9:testfile" TM ".txt", &uFileType, &uAuxType);
	uTest = uReturn != 0;
	uFailure |= uTest;
	ReportFailure("FileManager::GetFileAndAuxType(\"9:testfile" TM
				  ".txt\") = %d File = 0x%08X, Aux = 0x%08X",
		uTest, uReturn, uFileType, uAuxType);
	if (!uReturn && (uVerbose & VERBOSE_FILE)) {
		Message("9:testfile" TM ".txt's file was 0x%08X, aux was 0x%08X",
			uFileType, uAuxType);
	}
	return uFailure;
#else
	BURGER_UNUSED(uVerbose);
	return 0;
#endif
}

/***************************************

	Test FileManager.SetFileType()

***************************************/

static Word BURGER_API TestSetFileType(Word uVerbose)
{
#if defined(BURGER_MACOS)
	BURGER_UNUSED(uVerbose);
	Word32 uReturn = FileManager::SetFileType("9:testfile" TM ".txt", 'ABCD');
	Word uTest = uReturn != 0;
	Word uFailure = uTest;
	ReportFailure("FileManager::SetFileType(\"9:testfile" TM
				  ".txt\",'ABCD') = %d",
		uTest, uReturn);
	if (!uTest) {
		uReturn = FileManager::GetFileType("9:testfile" TM ".txt");
		uTest = uReturn != 'ABCD';
		uFailure |= uTest;
		ReportFailure("FileManager::SetFileType(\"9:testfile" TM
					  ".txt\",'ABCD') = got 0x%08X",
			uTest, uReturn);
	}

	uReturn = FileManager::SetFileType("9:testfile" TM ".txt", 'TEXT');
	uTest = uReturn != 0;
	uFailure |= uTest;
	ReportFailure("FileManager::SetFileType(\"9:testfile" TM
				  ".txt\",'TEXT') = %d",
		uTest, uReturn);
	if (!uTest) {
		uReturn = FileManager::GetFileType("9:testfile" TM ".txt");
		uTest = uReturn != 'TEXT';
		uFailure |= uTest;
		ReportFailure("FileManager::SetFileType(\"9:testfile" TM
					  ".txt\",'TEXT') = got 0x%08X",
			uTest, uReturn);
	}
	return uFailure;
#else
	BURGER_UNUSED(uVerbose);
	return 0;
#endif
}

/***************************************

	Test FileManager.SetAuxType()

***************************************/

static Word BURGER_API TestSetAuxType(Word uVerbose)
{
#if defined(BURGER_MACOS)
	BURGER_UNUSED(uVerbose);
	Word32 uReturn = FileManager::SetAuxType("9:testfile" TM ".txt", 'ABCD');
	Word uTest = uReturn != 0;
	Word uFailure = uTest;
	ReportFailure("FileManager::SetAuxType(\"9:testfile" TM
				  ".txt\",'ABCD') = %d",
		uTest, uReturn);
	if (!uTest) {
		uReturn = FileManager::GetAuxType("9:testfile" TM ".txt");
		uTest = uReturn != 'ABCD';
		uFailure |= uTest;
		ReportFailure("FileManager::SetAuxType(\"9:testfile" TM
					  ".txt\",'ABCD') = got 0x%08X",
			uTest, uReturn);
	}

	uReturn = FileManager::SetAuxType("9:testfile" TM ".txt", 'CWIE');
	uTest = uReturn != 0;
	uFailure |= uTest;
	ReportFailure("FileManager::SetAuxType(\"9:testfile" TM
				  ".txt\",'CWIE') = %d",
		uTest, uReturn);
	if (!uTest) {
		uReturn = FileManager::GetAuxType("9:testfile" TM ".txt");
		uTest = uReturn != 'CWIE';
		uFailure |= uTest;
		ReportFailure("FileManager::SetAuxType(\"9:testfile" TM
					  ".txt\",'CWIE') = got 0x%08X",
			uTest, uReturn);
	}
	return uFailure;
#else
	BURGER_UNUSED(uVerbose);
	return 0;
#endif
}

/***************************************

	Test FileManager.SetFileAndAuxType()

***************************************/

static Word BURGER_API TestSetFileAndAuxType(Word uVerbose)
{
#if defined(BURGER_MACOS)
	BURGER_UNUSED(uVerbose);
	Word32 uFileType;
	Word32 uAuxType;
	Word32 uReturn =
		FileManager::SetFileAndAuxType("9:testfile" TM ".txt", 'ABCD', 'ABCD');
	Word uTest = uReturn != 0;
	Word uFailure = uTest;
	ReportFailure("FileManager::SetFileAndAuxType(\"9:testfile" TM
				  ".txt\",'ABCD','ABCD') = %d",
		uTest, uReturn);
	if (!uTest) {
		uReturn = FileManager::GetFileAndAuxType(
			"9:testfile" TM ".txt", &uFileType, &uAuxType);
		uTest = (uReturn != 0) || (uFileType != 'ABCD') || (uAuxType != 'ABCD');
		uFailure |= uTest;
		ReportFailure("FileManager::SetFileAndAuxType(\"9:testfile" TM
					  ".txt\",'ABCD','ABCD') = got 0x%08X,0x%08X",
			uTest, uReturn);
	}

	uReturn =
		FileManager::SetFileAndAuxType("9:testfile" TM ".txt", 'TEXT', 'CWIE');
	uTest = uReturn != 0;
	uFailure |= uTest;
	ReportFailure("FileManager::SetFileAndAuxType(\"9:testfile" TM
				  ".txt\",'TEXT','CWIE') = %d",
		uTest, uReturn);
	if (!uTest) {
		uReturn = FileManager::GetFileAndAuxType(
			"9:testfile" TM ".txt", &uFileType, &uAuxType);
		uTest = (uReturn != 0) || (uFileType != 'TEXT') || (uAuxType != 'CWIE');
		uFailure |= uTest;
		ReportFailure("FileManager::SetFileAndAuxType(\"9:testfile" TM
					  ".txt\",'TEXT','CWIE') = got 0x%08X,0x%08X",
			uTest, uReturn);
	}
	return uFailure;
#else
	BURGER_UNUSED(uVerbose);
	return 0;
#endif
}

struct DirectoryTests_t {
	const char* m_pName;
	Word m_uDir;
};

static const DirectoryTests_t g_DirectoryFiles[] = {{"apple.txt", FALSE},
	{"berry.txt", FALSE}, {"cat.txt", FALSE}, {"delta", TRUE},
	{TM COPYRIGHT ".txt", FALSE}};

static const char g_DirectoryToTest[] = "9:test";

static Word BURGER_API TestDirectorySearch(Word uVerbose)
{
	FileManager::CreateDirectoryPath(g_DirectoryToTest);
	FileManager::SetPrefix(20, g_DirectoryToTest);
	Word i = 0;
	const DirectoryTests_t* pDirTests = g_DirectoryFiles;
	do {
		Filename TestName("20:");
		TestName.Append(pDirTests->m_pName);
		if (pDirTests->m_uDir) {
			FileManager::CreateDirectoryPath(&TestName);
		} else {
			FileManager::SaveFile(
				&TestName, g_SampleData, sizeof(g_SampleData));
		}
		++pDirTests;
	} while (++i < BURGER_ARRAYSIZE(g_DirectoryFiles));

	// Test directory traversal
	DirectorySearch MyDir;
	Word uTest;
	Word uFailure = FALSE;
	Word uReturn;

#if defined(BURGER_WINDOWS)
	uTest = sizeof(WIN32_FIND_DATAW) != 592;
	uFailure |= uTest;
	ReportFailure(
		"sizeof(MyDir.m_MyFindT) != %d", uTest, sizeof(WIN32_FIND_DATAW));
#endif

	uReturn = MyDir.Open(g_DirectoryToTest);
	uTest = uReturn != 0;
	uFailure |= uTest;
	ReportFailure("MyDir.Open(\"%s\") = %d", uTest, g_DirectoryToTest, uReturn);

	Word uEntries = 0;
	while (!MyDir.GetNextEntry()) {
		if (uEntries < BURGER_ARRAYSIZE(g_DirectoryFiles)) {
			uTest =
				StringCompare(MyDir.m_Name, g_DirectoryFiles[uEntries].m_pName)
				!= 0;
			uFailure |= uTest;
			ReportFailure("MyDir.GetNextEntry() returned %s, was expecting %s",
				uTest, MyDir.m_Name, g_DirectoryFiles[uEntries], uReturn);
			uTest = (g_DirectoryFiles[uEntries].m_uDir != MyDir.m_bDir);
			uFailure |= uTest;
			ReportFailure(
				"MyDir.GetNextEntry() wrong dir flag for %s. expected %d, got %d",
				uTest, MyDir.m_Name, g_DirectoryFiles[uEntries].m_uDir,
				MyDir.m_bDir);
		}
		if (uVerbose & VERBOSE_FILE) {
			Message("File %s found!", MyDir.m_Name);
			char TempBuffer[128];
			char TempBuffer2[128];
			MyDir.m_CreationDate.DateToStringVerbose(TempBuffer);
			MyDir.m_CreationDate.TimeToStringPM(TempBuffer2);
			Message("Creation was %s %s", TempBuffer, TempBuffer2);
			MyDir.m_ModificatonDate.DateToStringVerbose(TempBuffer);
			MyDir.m_ModificatonDate.TimeToStringPM(TempBuffer2);
			Message("Modification was %s %s", TempBuffer, TempBuffer2);
			Message("Size %u, Hidden %d, System %d, Directory %d, Locked %d",
				MyDir.m_uFileSize, MyDir.m_bHidden, MyDir.m_bSystem,
				MyDir.m_bDir, MyDir.m_bLocked);
#if defined(BURGER_MACOS)
			Message("File type 0x%08X, Aux Type 0x%08X", MyDir.m_uFileType,
				MyDir.m_uAuxType);
#endif
		}
		++uEntries;
	}
	MyDir.Close();

	uTest = uEntries == 0;
	uFailure |= uTest;
	ReportFailure("MyDir.GetNextEntry(\"9\") didn't iterate", uTest, uReturn);
	// Disable to run manual tests of modification/creation time difference

	pDirTests = g_DirectoryFiles;
	i = 0;
	do {
		Filename TestName("20:");
		TestName.Append(pDirTests->m_pName);
		FileManager::DeleteFile(&TestName);
		++pDirTests;
	} while (++i < BURGER_ARRAYSIZE(g_DirectoryFiles));
	FileManager::DeleteFile(g_DirectoryToTest);

	return uFailure;
}

/***************************************

	Test if setting the filename explicitly works.

***************************************/

Word BURGER_API TestBrFileManager(Word uVerbose)
{
	Word uTotal = 0;
	if (uVerbose & VERBOSE_FILE) {

		// Test Filename
		if (uVerbose & VERBOSE_MSG) {
			Message("Running Filename tests");
		}

		// Verify Filename
		uTotal |= TestFilenameClass();
		uTotal |= TestFilenameSet();
		uTotal |= TestFilenameClear();
		uTotal |= TestFilenameIs();

		// Test default prefixes and volume names
		TestShowDirectories();
		uTotal |= TestGetVolumeName(uVerbose);
		uTotal |= TestPrefixes(uVerbose);
		uTotal |= TestFilenameDirName();
		uTotal |= TestFilenameAppend();
		uTotal |= TestFilenameExpand();
		uTotal |= TestFile(uVerbose);

		TestCreateTempFiles();
		uTotal |= TestDoesFileExist();

		// Test file manipulation
		uTotal |= TestLoadSave();

		// Test file information
		uTotal |= TestGetModificationTime(uVerbose);
		uTotal |= TestGetCreationTime(uVerbose);
		uTotal |= TestGetFileType(uVerbose);
		uTotal |= TestGetAuxType(uVerbose);
		uTotal |= TestGetFileAndAuxType(uVerbose);
		uTotal |= TestSetFileType(uVerbose);
		uTotal |= TestSetAuxType(uVerbose);
		uTotal |= TestSetFileAndAuxType(uVerbose);
		TestDisposeTempFiles();

		if (uVerbose & VERBOSE_MSG) {
			Message("Running Directory tests");
		}
		uTotal |= TestDirectorySearch(uVerbose);
	}
	return uTotal;
}
