/***************************************

    String handlers for Win1252 support

    Copyright (c) 1995-2020 by Rebecca Ann Heineman <becky@burgerbecky.com>

    It is released under an MIT Open Source license. Please see LICENSE for
    license details. Yes, you can use it in a commercial title without paying
    anything, just give me a credit.

    Please? It's not like I'm asking you for money!

***************************************/

#include "brwin1252.h"

/*! ************************************

    \class Burger::Win1252
    \brief Conversion routines to support the Win1252 text encoding scheme.

    Win1252 is a high ASCII encoding that's used by the Windows file system for
    Windows XP, NT, etc. This class will allow the conversion of other character
    mappings to Win1252 for use in calling Win32 API functions.

    The character map for Win1252 [looks like this. Click
    here.](windows1252.htm)

***************************************/

/*! ************************************

    \var Burger::Win1252::BAD
    \brief Value returned if a routine failed.

    This is the value returned in the event of a function error condition.
    Test for this value to see if a function failed.

***************************************/

/*! ************************************

    \var const uint8_t Burger::Win1252::UpperCaseTable[256]
    \brief Table to convert all characters to upper case.

    Using Win1252 mapping, this table will convert all 256 codes into their
    upper case equivalents.

***************************************/

const uint8_t BURGER_ALIGN(Burger::Win1252::UpperCaseTable[256], 16) = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F, // 0x00
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,
    0x1C, 0x1D, 0x1E, 0x1F, // 0x10
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B,
    0x2C, 0x2D, 0x2E, 0x2F, // 0x20
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
    0x3C, 0x3D, 0x3E, 0x3F, // 0x30
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B,
    0x4C, 0x4D, 0x4E, 0x4F, // 0x40
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B,
    0x5C, 0x5D, 0x5E, 0x5F, // 0x50
    0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B,
    0x4C, 0x4D, 0x4E, 0x4F, // 0x60
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x7B,
    0x7C, 0x7D, 0x7E, 0x7F, // 0x70
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B,
    0x8C, 0x8D, 0x8E, 0x8F, // 0x80
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B,
    0x9C, 0x9D, 0x9E, 0x9F, // 0x90
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB,
    0xAC, 0xAD, 0xAE, 0xAF, // 0xA0
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB,
    0xBC, 0xBD, 0xBE, 0xBF, // 0xB0
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB,
    0xCC, 0xCD, 0xCE, 0xCF, // 0xC0
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB,
    0xDC, 0xDD, 0xDE, 0xDF, // 0xD0
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB,
    0xCC, 0xCD, 0xCE, 0xCF, // 0xE0
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xF7, 0xD8, 0xD9, 0xDA, 0xDB,
    0xDC, 0xDD, 0xFE, 0xFF // 0xF0
};

/*! ************************************

    \var const uint8_t Burger::Win1252::LowerCaseTable[256]
    \brief Table to convert all characters to lower case.

    Using Win1252 mapping, this table will convert all 256 codes into their
    lower case equivalents.

***************************************/

const uint8_t BURGER_ALIGN(Burger::Win1252::LowerCaseTable[256], 16) = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F, // 0x00
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,
    0x1C, 0x1D, 0x1E, 0x1F, // 0x10
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B,
    0x2C, 0x2D, 0x2E, 0x2F, // 0x20
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
    0x3C, 0x3D, 0x3E, 0x3F, // 0x30
    0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B,
    0x6C, 0x6D, 0x6E, 0x6F, // 0x40
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x5B,
    0x5C, 0x5D, 0x5E, 0x5F, // 0x50
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B,
    0x6C, 0x6D, 0x6E, 0x6F, // 0x60
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B,
    0x7C, 0x7D, 0x7E, 0x7F, // 0x70
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B,
    0x8C, 0x8D, 0x8E, 0x8F, // 0x80
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B,
    0x9C, 0x9D, 0x9E, 0x9F, // 0x90
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB,
    0xAC, 0xAD, 0xAE, 0xAF, // 0xA0
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB,
    0xBC, 0xBD, 0xBE, 0xBF, // 0xB0
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB,
    0xEC, 0xED, 0xEE, 0xEF, // 0xC0
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xD7, 0xF8, 0xF9, 0xFA, 0xFB,
    0xFC, 0xFD, 0xDE, 0xDF, // 0xD0
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB,
    0xEC, 0xED, 0xEE, 0xEF, // 0xE0
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB,
    0xFC, 0xFD, 0xFE, 0xFF // 0xF0
};

/*! ************************************

    \var const uint8_t Burger::Win1252::ToUTF8Table[128][4]
    \brief Table to convert Win1252 to UTF8.

    This 128x4 array holds the 128 high ascii codes for Win1252 converted to
    UTF8. Some unprintable codes don't exist and are marked as themselves for
    the pattern. These are codes 0x81, 0x8D, 0x8F, 0x90 and 0x9D.

    The character map for Win1252 [looks like this. Click
    here.](windows1252.htm)

***************************************/

const uint8_t BURGER_ALIGN(Burger::Win1252::ToUTF8Table[128][4], 16) = {
    {0xE2, 0x82, 0xAC, 0x00}, // 0x80 -> 0x20AC
    {0xC2, 0x81, 0x00, 0x00}, // 0x81 -> 0x0000 / 0x0081
    {0xE2, 0x80, 0x9A, 0x00}, // 0x82 -> 0x201A
    {0xC6, 0x92, 0x00, 0x00}, // 0x83 -> 0x0192
    {0xE2, 0x80, 0x9E, 0x00}, // 0x84 -> 0x201E
    {0xE2, 0x80, 0xA6, 0x00}, // 0x85 -> 0x2026
    {0xE2, 0x80, 0xA0, 0x00}, // 0x86 -> 0x2020
    {0xE2, 0x80, 0xA1, 0x00}, // 0x87 -> 0x2021
    {0xCB, 0x86, 0x00, 0x00}, // 0x88 -> 0x02C6
    {0xE2, 0x80, 0xB0, 0x00}, // 0x89 -> 0x2030
    {0xC5, 0xA0, 0x00, 0x00}, // 0x8A -> 0x0160
    {0xE2, 0x80, 0xB9, 0x00}, // 0x8B -> 0x2039
    {0xC5, 0x92, 0x00, 0x00}, // 0x8C -> 0x0152
    {0xC2, 0x8D, 0x00, 0x00}, // 0x8D -> 0x0000 / 0x008D
    {0xC5, 0xBD, 0x00, 0x00}, // 0x8E -> 0x017D
    {0xC2, 0x8F, 0x00, 0x00}, // 0x8F -> 0x0000 / 0x008F
    {0xC2, 0x90, 0x00, 0x00}, // 0x90 -> 0x0000 / 0x0090
    {0xE2, 0x80, 0x98, 0x00}, // 0x91 -> 0x2018
    {0xE2, 0x80, 0x99, 0x00}, // 0x92 -> 0x2019
    {0xE2, 0x80, 0x9C, 0x00}, // 0x93 -> 0x201C
    {0xE2, 0x80, 0x9D, 0x00}, // 0x94 -> 0x201D
    {0xE2, 0x80, 0xA2, 0x00}, // 0x95 -> 0x2022
    {0xE2, 0x80, 0x93, 0x00}, // 0x96 -> 0x2013
    {0xE2, 0x80, 0x94, 0x00}, // 0x97 -> 0x2014
    {0xCB, 0x9C, 0x00, 0x00}, // 0x98 -> 0x02DC
    {0xE2, 0x84, 0xA2, 0x00}, // 0x99 -> 0x2122
    {0xC5, 0xA1, 0x00, 0x00}, // 0x9A -> 0x0161
    {0xE2, 0x80, 0xBA, 0x00}, // 0x9B -> 0x203A
    {0xC5, 0x93, 0x00, 0x00}, // 0x9C -> 0x0153
    {0xC2, 0x9D, 0x00, 0x00}, // 0x9D -> 0x0000 / 0x009D
    {0xC5, 0xBE, 0x00, 0x00}, // 0x9E -> 0x017E
    {0xC5, 0xB8, 0x00, 0x00}, // 0x9F -> 0x0178
    {0xC2, 0xA0, 0x00, 0x00}, // 0xA0 -> 0x00A0
    {0xC2, 0xA1, 0x00, 0x00}, // 0xA1 -> 0x00A1
    {0xC2, 0xA2, 0x00, 0x00}, // 0xA2 -> 0x00A2
    {0xC2, 0xA3, 0x00, 0x00}, // 0xA3 -> 0x00A3
    {0xC2, 0xA4, 0x00, 0x00}, // 0xA4 -> 0x00A4
    {0xC2, 0xA5, 0x00, 0x00}, // 0xA5 -> 0x00A5
    {0xC2, 0xA6, 0x00, 0x00}, // 0xA6 -> 0x00A6
    {0xC2, 0xA7, 0x00, 0x00}, // 0xA7 -> 0x00A7
    {0xC2, 0xA8, 0x00, 0x00}, // 0xA8 -> 0x00A8
    {0xC2, 0xA9, 0x00, 0x00}, // 0xA9 -> 0x00A9
    {0xC2, 0xAA, 0x00, 0x00}, // 0xAA -> 0x00AA
    {0xC2, 0xAB, 0x00, 0x00}, // 0xAB -> 0x00AB
    {0xC2, 0xAC, 0x00, 0x00}, // 0xAC -> 0x00AC
    {0xC2, 0xAD, 0x00, 0x00}, // 0xAD -> 0x00AD
    {0xC2, 0xAE, 0x00, 0x00}, // 0xAE -> 0x00AE
    {0xC2, 0xAF, 0x00, 0x00}, // 0xAF -> 0x00AF
    {0xC2, 0xB0, 0x00, 0x00}, // 0xB0 -> 0x00B0
    {0xC2, 0xB1, 0x00, 0x00}, // 0xB1 -> 0x00B1
    {0xC2, 0xB2, 0x00, 0x00}, // 0xB2 -> 0x00B2
    {0xC2, 0xB3, 0x00, 0x00}, // 0xB3 -> 0x00B3
    {0xC2, 0xB4, 0x00, 0x00}, // 0xB4 -> 0x00B4
    {0xC2, 0xB5, 0x00, 0x00}, // 0xB5 -> 0x00B5
    {0xC2, 0xB6, 0x00, 0x00}, // 0xB6 -> 0x00B6
    {0xC2, 0xB7, 0x00, 0x00}, // 0xB7 -> 0x00B7
    {0xC2, 0xB8, 0x00, 0x00}, // 0xB8 -> 0x00B8
    {0xC2, 0xB9, 0x00, 0x00}, // 0xB9 -> 0x00B9
    {0xC2, 0xBA, 0x00, 0x00}, // 0xBA -> 0x00BA
    {0xC2, 0xBB, 0x00, 0x00}, // 0xBB -> 0x00BB
    {0xC2, 0xBC, 0x00, 0x00}, // 0xBC -> 0x00BC
    {0xC2, 0xBD, 0x00, 0x00}, // 0xBD -> 0x00BD
    {0xC2, 0xBE, 0x00, 0x00}, // 0xBE -> 0x00BE
    {0xC2, 0xBF, 0x00, 0x00}, // 0xBF -> 0x00BF
    {0xC3, 0x80, 0x00, 0x00}, // 0xC0 -> 0x00C0
    {0xC3, 0x81, 0x00, 0x00}, // 0xC1 -> 0x00C1
    {0xC3, 0x82, 0x00, 0x00}, // 0xC2 -> 0x00C2
    {0xC3, 0x83, 0x00, 0x00}, // 0xC3 -> 0x00C3
    {0xC3, 0x84, 0x00, 0x00}, // 0xC4 -> 0x00C4
    {0xC3, 0x85, 0x00, 0x00}, // 0xC5 -> 0x00C5
    {0xC3, 0x86, 0x00, 0x00}, // 0xC6 -> 0x00C6
    {0xC3, 0x87, 0x00, 0x00}, // 0xC7 -> 0x00C7
    {0xC3, 0x88, 0x00, 0x00}, // 0xC8 -> 0x00C8
    {0xC3, 0x89, 0x00, 0x00}, // 0xC9 -> 0x00C9
    {0xC3, 0x8A, 0x00, 0x00}, // 0xCA -> 0x00CA
    {0xC3, 0x8B, 0x00, 0x00}, // 0xCB -> 0x00CB
    {0xC3, 0x8C, 0x00, 0x00}, // 0xCC -> 0x00CC
    {0xC3, 0x8D, 0x00, 0x00}, // 0xCD -> 0x00CD
    {0xC3, 0x8E, 0x00, 0x00}, // 0xCE -> 0x00CE
    {0xC3, 0x8F, 0x00, 0x00}, // 0xCF -> 0x00CF
    {0xC3, 0x90, 0x00, 0x00}, // 0xD0 -> 0x00D0
    {0xC3, 0x91, 0x00, 0x00}, // 0xD1 -> 0x00D1
    {0xC3, 0x92, 0x00, 0x00}, // 0xD2 -> 0x00D2
    {0xC3, 0x93, 0x00, 0x00}, // 0xD3 -> 0x00D3
    {0xC3, 0x94, 0x00, 0x00}, // 0xD4 -> 0x00D4
    {0xC3, 0x95, 0x00, 0x00}, // 0xD5 -> 0x00D5
    {0xC3, 0x96, 0x00, 0x00}, // 0xD6 -> 0x00D6
    {0xC3, 0x97, 0x00, 0x00}, // 0xD7 -> 0x00D7
    {0xC3, 0x98, 0x00, 0x00}, // 0xD8 -> 0x00D8
    {0xC3, 0x99, 0x00, 0x00}, // 0xD9 -> 0x00D9
    {0xC3, 0x9A, 0x00, 0x00}, // 0xDA -> 0x00DA
    {0xC3, 0x9B, 0x00, 0x00}, // 0xDB -> 0x00DB
    {0xC3, 0x9C, 0x00, 0x00}, // 0xDC -> 0x00DC
    {0xC3, 0x9D, 0x00, 0x00}, // 0xDD -> 0x00DD
    {0xC3, 0x9E, 0x00, 0x00}, // 0xDE -> 0x00DE
    {0xC3, 0x9F, 0x00, 0x00}, // 0xDF -> 0x00DF
    {0xC3, 0xA0, 0x00, 0x00}, // 0xE0 -> 0x00E0
    {0xC3, 0xA1, 0x00, 0x00}, // 0xE1 -> 0x00E1
    {0xC3, 0xA2, 0x00, 0x00}, // 0xE2 -> 0x00E2
    {0xC3, 0xA3, 0x00, 0x00}, // 0xE3 -> 0x00E3
    {0xC3, 0xA4, 0x00, 0x00}, // 0xE4 -> 0x00E4
    {0xC3, 0xA5, 0x00, 0x00}, // 0xE5 -> 0x00E5
    {0xC3, 0xA6, 0x00, 0x00}, // 0xE6 -> 0x00E6
    {0xC3, 0xA7, 0x00, 0x00}, // 0xE7 -> 0x00E7
    {0xC3, 0xA8, 0x00, 0x00}, // 0xE8 -> 0x00E8
    {0xC3, 0xA9, 0x00, 0x00}, // 0xE9 -> 0x00E9
    {0xC3, 0xAA, 0x00, 0x00}, // 0xEA -> 0x00EA
    {0xC3, 0xAB, 0x00, 0x00}, // 0xEB -> 0x00EB
    {0xC3, 0xAC, 0x00, 0x00}, // 0xEC -> 0x00EC
    {0xC3, 0xAD, 0x00, 0x00}, // 0xED -> 0x00ED
    {0xC3, 0xAE, 0x00, 0x00}, // 0xEE -> 0x00EE
    {0xC3, 0xAF, 0x00, 0x00}, // 0xEF -> 0x00EF
    {0xC3, 0xB0, 0x00, 0x00}, // 0xF0 -> 0x00F0
    {0xC3, 0xB1, 0x00, 0x00}, // 0xF1 -> 0x00F1
    {0xC3, 0xB2, 0x00, 0x00}, // 0xF2 -> 0x00F2
    {0xC3, 0xB3, 0x00, 0x00}, // 0xF3 -> 0x00F3
    {0xC3, 0xB4, 0x00, 0x00}, // 0xF4 -> 0x00F4
    {0xC3, 0xB5, 0x00, 0x00}, // 0xF5 -> 0x00F5
    {0xC3, 0xB6, 0x00, 0x00}, // 0xF6 -> 0x00F6
    {0xC3, 0xB7, 0x00, 0x00}, // 0xF7 -> 0x00F7
    {0xC3, 0xB8, 0x00, 0x00}, // 0xF8 -> 0x00F8
    {0xC3, 0xB9, 0x00, 0x00}, // 0xF9 -> 0x00F9
    {0xC3, 0xBA, 0x00, 0x00}, // 0xFA -> 0x00FA
    {0xC3, 0xBB, 0x00, 0x00}, // 0xFB -> 0x00FB
    {0xC3, 0xBC, 0x00, 0x00}, // 0xFC -> 0x00FC
    {0xC3, 0xBD, 0x00, 0x00}, // 0xFD -> 0x00FD
    {0xC3, 0xBE, 0x00, 0x00}, // 0xFE -> 0x00FE
    {0xC3, 0xBF, 0x00, 0x00}  // 0xFF -> 0x00FF
};

/*! ************************************

    \brief Convert a single UTF8 stream character into a Win1252 8 bit char.

    Take up 3 bytes from a UTF8 stream and return the unsigned 8 bit value of
    the Win1252 character. Codes 0 through 0x7f are considered ASCII while codes
    0x80 through 0xFF are mapped to Win1252. If the UTF8 character cannot be
    mapped to Win1252 encoding, Burger::Win1252::BAD will be returned instead.

    \note This function will not return the number of bytes decoded. Use
        Burger::UTF8::NextToken(const char *) to get the pointer to the next
        UTF8 entry.

    \param pInput Pointer to UTF8 buffer that contains the 1 to 3 byte buffer to
        convert. nullptr will page fault.

    \return The unsigned 8 bit character code (0x00-0xFF) or
        Burger::Win1252::BAD if the UTF8 value wasn't low ASCII and couldn't be
        mapped to Win1252.

    \sa Burger::UTF8::NextToken(const char *) or
        Burger::UTF8::GetTokenSize(const char *)

***************************************/

uint_t BURGER_API Burger::Win1252::FromUTF8(const char* pInput) BURGER_NOEXCEPT
{
    // Get a UTF-8 char
    const uint_t uFirst = reinterpret_cast<const uint8_t*>(pInput)[0];
    if (uFirst < 0x80U) {
        return uFirst; // Easy!!! I like it!
    }
    // Great, it's not low ASCII. Do it the hard way.
    uint_t uCounter = 128; // Number of attempts

    // Get the pointer to the translation table

    const uint8_t(*pTable)[4] = ToUTF8Table;

    // Cache the second char (The table is of 2 or 3 byte tokens only)

    const uint_t uSecond = reinterpret_cast<const uint8_t*>(pInput)[1];
    do {
        if ((uFirst == pTable[0][0]) && // So far, so goo?
            (uSecond == pTable[0][1])) {
            const uint_t uTemp = pTable[0][2]; // Is this a 3 byte pattern?
            if (!uTemp ||                      // Nope, I'm good
                (reinterpret_cast<const uint8_t*>(pInput)[2] ==
                    uTemp)) {          // Match?
                return 256 - uCounter; // Return 0x80-0xFF
            }
        }
        ++pTable; // Next 4 byte entry
    } while (--uCounter);
    return static_cast<uint_t>(Win1252::BAD); // Crud
}

/*! ************************************

    \brief Convert a UTF8 stream into a Win1252 "C" string.

    Take a "C" string that is using UTF8 encoding and convert it into a Win1252
    encoded "C" string. The function will return the size of the string after
    encoding. This size is valid, even if it exceeded the output buffer size.
    The output pointer and size can be nullptr to have this routine calculate
    the size of the possible output so the application can allocate a buffer
    large enough to hold it.

    \note This function will ensure that the string is always zero terminated,
        even if truncation is necessary to get it to fit in the output buffer.
        Under no circumstances will the output buffer be overrun.

    \param pOutput Pointer to byte buffer to receive the Win1252 encoded string.
        nullptr is okay if uOutputSize is zero, otherwise it will page fault.
    \param uOutputSize Size of the output buffer in bytes.
    \param pInput A UTF8 encoded "C" string. nullptr will page fault.

    \return Burger::StringLength() of the potential output. It is valid, even if
        the output buffer wasn't large enough to contain everything.

***************************************/

uint_t BURGER_API Burger::Win1252::FromUTF8(
    char* pOutput, uintptr_t uOutputSize, const char* pInput) BURGER_NOEXCEPT
{
    uint8_t* pWorkPtr = reinterpret_cast<uint8_t*>(pOutput);

    // If nonzero, then I append a 0 to the string.
    const uintptr_t bAddZero = uOutputSize;
    if (uOutputSize) { // Valid?
        --uOutputSize;
    }
    // This is the end of the valid buffer
    const uint8_t* pEndPtr = pWorkPtr + uOutputSize;

    // Let's convert the string
    uint_t uFirst = reinterpret_cast<const uint8_t*>(pInput)[0];
    ++pInput;     // Accept it
    if (uFirst) { // Can I start the loop?
        do {

            // Get a char
            if (uFirst < 0x80U) {         // ASCII?
                if (pWorkPtr < pEndPtr) { // Can I write?
                    pWorkPtr[0] = static_cast<uint8_t>(uFirst);
                }
                ++pWorkPtr;               // Increment the output
            } else if (uFirst >= 0xC2U) { // Valid multibyte opcode?
                // Make a copy of the pointer
                const uint8_t(*pTranslateTable2)[4] = ToUTF8Table;

                // Great, it's not low ASCII. Do it the hard way.
                uint_t uCounter = 128; // Number of attempts
                const uint_t uSecond =
                    reinterpret_cast<const uint8_t*>(pInput)[0];
                do {
                    if ((uFirst == pTranslateTable2[0][0]) &&
                        (uSecond == pTranslateTable2[0][1])) {
                        const uint_t uTemp = pTranslateTable2[0][2];
                        if (!uTemp) {
                            ++pInput; // 2 byte code
                            break;
                        }
                        if ((reinterpret_cast<const uint8_t*>(pInput)[1] ==
                                uTemp)) {
                            pInput += 2; // 3 byte code
                            break;
                        }
                    }
                    ++pTranslateTable2; // Next 4 byte entry
                } while (--uCounter);
                if (uCounter) {               // Did I get a valid code?
                    if (pWorkPtr < pEndPtr) { // Write it out
                        pWorkPtr[0] = static_cast<uint8_t>(256 - uCounter);
                    }
                    ++pWorkPtr;
                }
            }
            uFirst = reinterpret_cast<const uint8_t*>(pInput)[0];
            ++pInput; // Accept it
        } while (uFirst);
    }
    // Can I add a trailing zero?
    if (bAddZero) {
        // Write it, but don't add it to the strlen()
        pWorkPtr[0] = 0; 
    }

    // Return the equivalent of strlen()
    return static_cast<uint_t>(reinterpret_cast<char*>(pWorkPtr) - pOutput);
}

/*! ************************************

    \brief Convert a UTF8 stream into a Win1252 byte array.

    Take a byte array that is using UTF8 encoding and convert it to a Win1252
    encoded "C" string. The function will return the size of the string after
    encoding. This size is valid, even if it exceeded the output buffer size.
    The output pointer and size can be nullptr to have this routine calculate
    the size of the possible output so the application can allocate a buffer
    large enough to hold it.

    \note This function will ensure that the string is always zero terminated,
        even if truncation is necessary to get it to fit in the output buffer.
        Under no circumstances will the output buffer be overrun.

    \note Zeros can be encoded into the stream. This function will not early out
        if a zero was parsed. Zeros will be placed in the Win1252 stream as is.

    \param pOutput Pointer to a byte buffer to receive the Win1252 string.
        nullptr is okay if uOutputSize is zero, otherwise a page fault will
        occur.
    \param uOutputSize Size of the output buffer in bytes.
    \param pInput UTF8 encoded byte array. nullptr is okay if uInputSize is
        zero.
    \param uInputSize Size of the input byte array.

    \return Burger::StringLength() of the potential output. It is valid, even if
        the output buffer wasn't large enough to contain everything.

***************************************/

uint_t BURGER_API Burger::Win1252::FromUTF8(char* pOutput,
    uintptr_t uOutputSize, const char* pInput,
    uintptr_t uInputSize) BURGER_NOEXCEPT
{
    uint8_t* pWorkPtr = reinterpret_cast<uint8_t*>(pOutput);
    const uintptr_t bAddZero = uOutputSize; // Can I save a zero?
    if (uOutputSize) {                      // Valid for anything?
        --uOutputSize;                      // Make room for the zero
    }
    // Can't write, ever
    const uint8_t* pEndPtr = pWorkPtr + uOutputSize;

    // Let's convert the string

    if (uInputSize) { // Sanity check
        do {
            const uint_t uFirst = reinterpret_cast<const uint8_t*>(pInput)[0];
            ++pInput; // Accept it

            // Get a char

            if (uFirst < 0x80U) {         // ASCII?
                if (pWorkPtr < pEndPtr) { // Can I write?
                    pWorkPtr[0] = static_cast<uint8_t>(uFirst);
                }
                ++pWorkPtr; // Increment the output
                // Must have at least 2 bytes pending
            } else if ((uFirst >= 0xC2U) && (uInputSize >= 2)) {

                // Make a copy of the pointer
                const uint8_t(*pTranslateTable2)[4] = ToUTF8Table;
                const uint_t uSecond =
                    reinterpret_cast<const uint8_t*>(pInput)[0];

                // Great, it's not low ASCII. Do it the hard way.
                uint_t uCounter = 128; // Number of attempts
                do {
                    if ((uFirst == pTranslateTable2[0][0]) &&
                        (uSecond == pTranslateTable2[0][1])) {
                        const uint_t uTemp = pTranslateTable2[0][2];
                        if (!uTemp) {
                            ++pInput; // Accept it
                            --uInputSize;
                            break;
                        }
                        if ((uInputSize >= 3) &&
                            (reinterpret_cast<const uint8_t*>(pInput)[1] ==
                                uTemp)) {
                            // Accept the three bytes
                            pInput += 2;
                            uInputSize -= 2;
                            break;
                        }
                    }
                    ++pTranslateTable2; // Next 4 byte entry
                } while (--uCounter);
                // Did I get a valid code?
                if (uCounter) {
                    // Write it out
                    if (pWorkPtr < pEndPtr) {
                        pWorkPtr[0] = static_cast<uint8_t>(256 - uCounter);
                    }
                    ++pWorkPtr;
                }
            }
        } while (--uInputSize);
    }
    // Can I add a trailing zero?
    if (bAddZero) {
        // Write it, but don't add it to the strlen()
        pWorkPtr[0] = 0;
    }

    // Return the equivalent of strlen()
    return static_cast<uint_t>(reinterpret_cast<char*>(pWorkPtr) - pOutput);
}
