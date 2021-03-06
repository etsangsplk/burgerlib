/***************************************

	Debug manager

	Copyright (c) 1995-2017 by Rebecca Ann Heineman <becky@burgerbecky.com>

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!

***************************************/

#include "brdebug.h"
#include "brglobals.h"
#include "brstringfunctions.h"
#include "brnumberstring.h"
#include "brfile.h"
#include "brassert.h"
#include "brnumberstringhex.h"
#include <stdarg.h>
#include <stdio.h>

#if !(defined(BURGER_MAC) && !defined(__MSL__)) && !(defined(_MSC_VER) && (_MSC_VER<1400)) && !defined(DOXYGEN)
#define HASVSNPRINTF
#endif

/*! ************************************

	\namespace Burger::Debug
	\brief Group of functions to assist in debugging

	These thread safe functions will print messages to the
	debug console of the attached debugger and
	if a debugger is not available, the output
	will be logged into a text file on applicable
	platforms.

	\sa \ref Globals
	
***************************************/



/*! ************************************

	\brief A fatal error has occurred, print message, then exit.
	
	If the input message pointer is not \ref NULL, then print
	the message string using printf() rules.
	
	This message, if any, will be output through \ref Debug::Message()
	which either outputs it to a log file or a debugger's console (If
	a debugger is present and running).
			
	\note This will call Globals::Shutdown() with a default error code of 1. You
	must call \ref Globals::SetErrorCode() to change the code to something else.
	
	Secondly, this function won't return unless it was called while
	Globals::Shutdown() was already invoked, then this function CAN return, so you must
	add a return statement after calling this function in the case
	that there is a recursion error in progress.
	
	\code
	if (TestError) {
		Debug::Fatal("Fatal error, file %s was not found.",pFileName);
		return;
	}
	\endcode

	\param pMessage Pointer to a string suitable for printf or \ref NULL.
	\return 0 if this function returns. In most cases, this function never returns.

	\sa Debug::Warning, Globals::GetExitFlag() and Globals::Shutdown()
	
***************************************/

Word BURGER_ANSIAPI Burger::Debug::Fatal(const char *pMessage,...)
{
	if (!Globals::GetExitFlag()) {

		if (pMessage) {		// Message to print?
			char TempBuffer[512];

			va_list Args;
			va_start(Args,pMessage);
#if defined(HASVSNPRINTF)
			vsnprintf(TempBuffer,sizeof(TempBuffer),pMessage,Args);
#else
			vsprintf(TempBuffer,pMessage,Args);	// Create the message
#endif
			va_end(Args);

			PrintString(TempBuffer);		// Print it
		}
		Globals::Shutdown();	// Exit to OS
	}
	return 0;					// Return 0 for \ref BURGER_ASSERTTEST()
}

/*! ************************************

	\brief Store a non-fatal error message, then return or exit.
	
	If the input message pointer is not \ref NULL, then store
	the message string into the global error string buffer
	using printf() rules. If it's \ref NULL, do nothing to the error message buffer.
	
	If \ref Globals::SetErrorCode() was called with \ref TRUE, this function
	will then treat this warning as an error and call \ref Debug::Fatal(const char *pFatalMsg,...) and exit.
		
	\code
	if (TestError) {
		Debug::Warning("Warning, file %s was not found.",pFileName);
		return;
	}
	\endcode
	
	\note The standard behavior is to store the text string and immediately
	return. Only if an explicit call to \ref Globals::SetErrorCode() setting
	it \ref TRUE will change the operation of this procedure.
		
	\param pMessage Pointer to a string suitable for printf or \ref NULL.
	
	\sa Debug::Fatal(const char *pFatalMsg,...)
	
***************************************/

void BURGER_ANSIAPI Burger::Debug::Warning(const char *pMessage,...)
{
	char TempBuffer[512];
	// Ensure the buffer is a valid "C" string
	TempBuffer[0] = 0;

	if (pMessage) {						// No message, no error!

		// Should the warning be printed?
		if ((Globals::GetTraceFlag()&Globals::TRACE_WARNINGS) ||
			Globals::GetErrorBombFlag()) {

			va_list Args;
			va_start(Args,pMessage);		// Start parameter passing
#if defined(HASVSNPRINTF)
			vsnprintf(TempBuffer,sizeof(TempBuffer),pMessage,Args);		// Create the message
#else
			vsprintf(TempBuffer,pMessage,Args);		// Create the message
			BURGER_ASSERT(StringLength(TempBuffer)<sizeof(TempBuffer));
#endif
			va_end(Args);					// End parameter passing

			PrintString(TempBuffer);
		}
	}

	// Bomb on ANY Error?
	if (Globals::GetErrorBombFlag()) {	
		// Print the buffer as is, do not apply printf on it.
		// Which is why %s is used.
		Fatal("%s",TempBuffer);			// Force a fatal error with the stored message
	}
}


/*! ************************************

	\brief Print a message to the debugger 
	output stream

	Given a "printf()" message string and parameters, print
	the message to the output stream, which is a console or an
	attached debugger

	\sa Debug::PrintString(const char *), Debug::PrintString(Word32) and Debug::PrintString(Word64)

***************************************/

void BURGER_ANSIAPI Burger::Debug::Message(const char *pMessage,...)
{
	// No message, no output!
	if (pMessage) {

		// Local string buffer
		char TempBuffer[2048];

		va_list Args;
		// Start parameter passing
		va_start(Args,pMessage);
		// Create the message in a single buffer
#if defined(HASVSNPRINTF)
		vsnprintf(TempBuffer,sizeof(TempBuffer),pMessage,Args);
#else
		vsprintf(TempBuffer,pMessage,Args);
		BURGER_ASSERT(StringLength(TempBuffer)<sizeof(TempBuffer));
#endif
		// End the parameter passing
		va_end(Args);

		// Actually print the message to the console
		PrintString(TempBuffer);
	}
}

/*! ************************************

	\brief Print a string to a file
	
	Given a "C" string, stream the data to a text file,
	or if a debugger is attached, to the debugger console.
	
	No parsing is done on the string, it's written as is.
	
	\param pString Pointer to a "C" string to print.

***************************************/

#if !(defined(BURGER_WINDOWS) || defined(BURGER_MACOSX)) || defined(DOXYGEN)
void BURGER_API Burger::Debug::PrintString(const char *pString)
{
	if (pString && !Globals::GetExitFlag()) {
		WordPtr i = StringLength(pString);
		if (i) {

			// MacOS 9/Carbon doesn't have a console,
			// so send the data to a file
#if defined(BURGER_MAC)
			File MyFile;
			if (MyFile.Open("9:LogFile.Txt",File::APPEND)==File::OKAY) {
				MyFile.Write(pString,i);		// Send the string to the log file
				MyFile.Close();
			}
#else
			// Send to stdout on all other platforms
			fwrite(pString,1,i,stdout);
#endif
		}
	}
}
#endif

/*! ************************************

	\brief Print a Word32 to the debug port
	\sa Debug::PrintString(Word64)

***************************************/

void BURGER_API Burger::Debug::PrintString(Word32 uInput)
{
	NumberString TempBuffer(uInput);
	PrintString(TempBuffer);
}

/*! ************************************

	\brief Print a Word32 to the debug port
	\sa Debug::PrintString(Word32)

***************************************/

void BURGER_API Burger::Debug::PrintString(Word64 uInput)
{
	NumberString TempBuffer(uInput);
	PrintString(TempBuffer);
}

/*! ************************************

	\brief Detect if a debugger is attached

	Return \ref TRUE if a debugger is attached

***************************************/

#if !(defined(BURGER_WINDOWS) || defined(BURGER_MACOSX)) || defined(DOXYGEN)
Word BURGER_API Burger::Debug::IsDebuggerPresent(void)
{
	return FALSE;
}
#endif

/*! ************************************

	\brief Print the error message for an OS error code

	Given an error code from the native operating system and print
	it out the \ref Debug messaging system.

	\param uErrorCode Error code from Windows/MacOS/etc...

***************************************/

#if !(defined(BURGER_WINDOWS)) || defined(DOXYGEN)
void BURGER_API Burger::Debug::PrintErrorMessage(Word uErrorCode)
{
	// Print the error string
	PrintString("Error: 0x");

	// Show the error in hex
	NumberStringHex TempBuffer(static_cast<Word32>(uErrorCode));
	PrintString(TempBuffer);

	PrintString("\n");
}
#endif

/*! ************************************

	\brief Display a dialog box
	
	On platforms that support pop up dialogs, display a dialog
	that has an "Okay" button

	On platforms that do not support pop up dialogs, the messages are logged

	\param pMessage Message to print in the center of the dialog box
	\param pTitle Pointer to "C" string or \ref NULL for a message in the title bar
	\sa OkCancelAlertMessage() or Debug::PrintString(const char *)

***************************************/

#if !(defined(BURGER_MAC) || defined(BURGER_WINDOWS) || defined(BURGER_BEOS) || defined(BURGER_MACOSX)) || defined(DOXYGEN)

void BURGER_API Burger::OkAlertMessage(const char *pMessage,const char *pTitle)
{
	Debug::PrintString("Alert message : ");
	if (pTitle) {
		Debug::PrintString(pTitle);
		Debug::PrintString(", ");
	}
	Debug::PrintString(pMessage);
	Debug::PrintString("\n");
}

#endif

/*! ************************************

	\brief Display a dialog to alert the user of a possible error condition or message.

	On platforms that support pop up dialogs, display a dialog
	that has two buttons, one for "Okay" and another for "Cancel"

	On platforms that do not support pop up dialogs, the messages are logged
	and \ref FALSE (Cancel) is always returned

	\param pMessage Pointer to "C" string with the message that asks a question that
	can be answered with Okay or Cancel
	\param pTitle Pointer to "C" string or \ref NULL for a message in the title bar
	\return \ref TRUE if the user pressed "Okay" or \ref FALSE if pressed "Cancel"
	\sa OkAlertMessage() or Debug::PrintString(const char *)

***************************************/

#if !(defined(BURGER_MAC) || defined(BURGER_WINDOWS) || defined(BURGER_BEOS) || defined(BURGER_MACOSX)) || defined(DOXYGEN)

Word BURGER_API Burger::OkCancelAlertMessage(const char *pMessage,const char *pTitle)
{
	Debug::PrintString("Cancel alert message : ");
	if (pTitle) {
		Debug::PrintString(pTitle);
		Debug::PrintString(", ");
	}
	Debug::PrintString(pMessage);
	Debug::PrintString("\n");
	return FALSE;		// Always cancel!
}

#endif


