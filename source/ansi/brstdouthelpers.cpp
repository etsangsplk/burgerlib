/***************************************

    Helper functions for printf and the like

    Copyright (c) 1995-2020 by Rebecca Ann Heineman <becky@burgerbecky.com>

    It is released under an MIT Open Source license. Please see LICENSE for
    license details. Yes, you can use it in a commercial title without paying
    anything, just give me a credit.

    Please? It's not like I'm asking you for money!

***************************************/

#include "brstdouthelpers.h"
#include "brnumberto.h"
#include "brstringfunctions.h"
#include <stdio.h>

/*! ************************************

    \brief Print a hex character to standard out

    Print a single hex digit from the lowest 4 bits of the input
    value to stdout

    \param uInput Integer value (Only lowest 4 bits used) to print in HEX to
        stdout

    \sa PrintHex(uint8_t), PrintHex(uint16_t), PrintHex(uint32_t), or
        PrintHex(uint64_t)

***************************************/

void BURGER_API Burger::PrintHexDigit(uint_t uInput)
{
    // Print the char
    putchar(g_NibbleToAsciiUppercase[uInput & 0xFU]);
}

/*! ************************************

    \brief Print an 8 bit value in hex to standard out

    Print two hex digits in the form of 00 to stdout

    \param uInput 8 bit value to print in HEX to stdout
    \sa PrintHexDigit(uint_t), PrintHex(uint16_t), PrintHex(uint32_t), or
        PrintHex(uint64_t)

***************************************/

void BURGER_API Burger::PrintHex(uint8_t uInput)
{
    // Convert to a string
    char temp_string[(sizeof(uint8_t) * 2) + 1];
    NumberToAsciiHex(temp_string, uInput);

    // Send the string to stdout
    fputs(temp_string, stdout);
}

/*! ************************************

    \brief Print a 16 bit value in hex to standard out

    Print four hex digits in the form of 0000 to stdout

    \param uInput 16 bit value to print in HEX to stdout
    \sa PrintHexDigit(uint_t), PrintHex(uint8_t), PrintHex(uint32_t), or
        PrintHex(uint64_t)

***************************************/

void BURGER_API Burger::PrintHex(uint16_t uInput)
{
    // Convert to a string
    char temp_string[(sizeof(uint16_t) * 2) + 1];
    NumberToAsciiHex(temp_string, uInput);

    // Send the string to stdout
    fputs(temp_string, stdout);
}

/*! ************************************

    \brief Print a 32 bit value in hex to standard out

    Print eight hex digits in the form of 00000000 to stdout

    \param uInput 32 bit value to print in HEX to stdout
    \sa PrintHexDigit(uint_t), PrintHex(uint8_t), PrintHex(uint16_t), or
        PrintHex(uint64_t)

***************************************/

void BURGER_API Burger::PrintHex(uint32_t uInput)
{
    // Convert to a string
    char temp_string[(sizeof(uint32_t) * 2) + 1];
    NumberToAsciiHex(temp_string, uInput);

    // Send the string to stdout
    fputs(temp_string, stdout);
}

/*! ************************************

    \brief Print a 64 bit value in hex to standard out

    Print sixteen hex digits in the form of 0000000000000000 to stdout

    \param uInput 64 bit value to print in HEX to stdout
    \sa PrintHexDigit(uint_t), PrintHex(uint8_t), PrintHex(uint16_t), or
        PrintHex(uint32_t)

***************************************/

void BURGER_API Burger::PrintHex(uint64_t uInput)
{
    // Convert to a string
    char temp_string[(sizeof(uint64_t) * 2) + 1];
    NumberToAsciiHex(temp_string, uInput);

    // Send the string to stdout
    fputs(temp_string, stdout);
}

/*! ************************************

    \brief Print a 32 bit floating pointer number in hex to standard out

    Print eight hex digits in the form of 00000000 to stdout

    \param fInput Floating point number to print in HEX to stdout
    \sa PrintHex(uint32_t), or PrintHex(double)

***************************************/

void BURGER_API Burger::PrintHex(float fInput)
{
    // Convert to a string
    char temp_string[(sizeof(float) * 2) + 1];
    NumberToAsciiHex(temp_string, fInput);

    // Send the string to stdout
    fputs(temp_string, stdout);
}

/*! ************************************

    \brief Print a 64 bit floating point number in hex to standard out

    Print sixteen hex digits in the form of 0000000000000000 to stdout

    \param dInput 64 bit floating point number to print in HEX to stdout
    \sa PrintHex(uint64_t), or PrintHex(float)

***************************************/

void BURGER_API Burger::PrintHex(double dInput)
{
    // Convert to a string
    char temp_string[(sizeof(double) * 2) + 1];
    NumberToAsciiHex(temp_string, dInput);

    // Send the string to stdout
    fputs(temp_string, stdout);
}

/*! ************************************

    \brief Clear the text console

    For platforms that support a text console, clear out the text console by
    sending a line feed or system appropriate set of calls to perform the same
    action the ends up with a clear text screen and the output cursor set to the
    upper left corner.

    This is an equivalent of the Borland C library function clrscr().

***************************************/

#if !(defined(BURGER_WINDOWS) || defined(BURGER_MACOSX)) || defined(DOXYGEN)
void BURGER_API Burger::ClearConsole(void)
{
    // Send a line feed to clear the screen
    putchar(12);
}
#endif
