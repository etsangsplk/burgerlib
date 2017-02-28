/***************************************

	Random number generator base class

	Copyright (c) 1995-2017 by Rebecca Ann Heineman <becky@burgerbecky.com>

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!

***************************************/

#include "brrandombase.h"
#include "brtimedate.h"
#include "brtick.h"
#include "brstringfunctions.h"

/*! ************************************

	\class Burger::RandomBase
	\brief A random number generator base class.
	
	This class is what all random number generators derive
	from so that many random number generator algorithms
	can share common code.

	\note It's permissible to make binary copies of this class.
	
***************************************/

#if !defined(DOXYGEN)
BURGER_CREATE_STATICRTTI_PARENT(Burger::RandomBase,Burger::Base);
#endif

/*! ************************************

	\var const Burger::StaticRTTI Burger::RandomBase::g_StaticRTTI
	\brief The global description of the class

	This record contains the name of this class and a
	reference to the parent

***************************************/


/*! ************************************

	\fn void Burger::RandomBase::SetSeed(Word32 uNewSeed)
	\brief Seed the random number generator.
	
	Set the random number generator to a specific seed.
	This allows altering the random number flow in a
	controlled manner.
	
	\param uNewSeed 32 bit seed value.
	\sa Get(void) or GetSeed(void) const

***************************************/


/*! ************************************

	\fn Word32 Burger::RandomBase::Get(void)
	\brief Return a 32 bit pseudo random number.
	
	Get a pseudo random number using the current algorithm. 
	Return a 32 bit unsigned value.

	\return A 32 bit pseudo random number.
	\sa SetSeed(Word32) or GetWord(Word32)

***************************************/

/*! ************************************

	\fn Word32 Burger::RandomBase::GetSeed(void) const
	\brief Return the random number seed.
	
	Get seed value for this pseudo random number
	generator.

	\return A 32 bit pseudo random number.
	\sa SetSeed(Word32)

***************************************/




/*! ************************************

	\brief Seed with a random value.

	Init the random number generator with an "Anything goes"
	policy so programs will power up in an unknown state.
	Do NOT use this if you wish your title to have recordable demos.

	This function will start with the current time as a seed and then
	it will use a formula that assumes that the tick
	timer runs at a constant time base but the machine in question
	does not. As a result. The number of times Get() is
	called is anyone's guess.
	
	\sa SetSeed(Word32) or Get(void).

***************************************/

void BURGER_API Burger::RandomBase::RandomSeed(void)
{
	// Read the current time to get a seed
	TimeDate_t Date;
	Date.GetTime();
	WordPtr uTimeT;
	Date.StoreTimeT(&uTimeT);

	// Init the seed with the current time
	SetSeed(static_cast<Word32>(uTimeT));					
	Word32 uTickMark = Tick::Read();	// Get a current tick mark
	do {
		Get();							// Discard a number from the stream
	} while (Tick::Read()==uTickMark);	// Same time?
}

/*! ************************************

	\brief Return a 32 bit random number.
	
	Get a random number. Return a number between 0
	through (Range-1) inclusive.
	
	\param uRange 0 means return 32 bits as is, anything else
		means return 0 through (Range-1) inclusive.
	\return A random number in the specified range.

***************************************/

Word32 BURGER_API Burger::RandomBase::GetWord(Word32 uRange)
{
	Word32 uNewVal = Get();
	// Return as is
	if (uRange) {
		
		// A ranged random number is requested,
		// apply a linear scale
		
		if (uRange>0x10000U) {
			// Do it with a modulo, slow
			// but accurate
			uNewVal = uNewVal%uRange;
		} else {
			// Can do it the quick way with fixed
			// point math
			
			uNewVal&=0xFFFFU;		// Make sure they are shorts!
			uNewVal = ((uNewVal*uRange)>>16U);
		}
	}
	return uNewVal;
}

/*! ************************************

	\brief Return a dice roll
	
	Given the number of dice and the size of the dice, 
	"roll" the dice and return the result.

	Examples:
	GetDice(1,4) will yield 1-4 evenly spread
	GetDice(2,4) will yield 2-8 with 5 having the highest probability based on the curve

	If either input value is 0, the result is zero.
	If the dice roll exceeds a \ref Word32, \ref BURGER_MAXUINT is returned
	
	\param uDiceCount Number of dice to roll.
	\param uDiceSize Number of sides on each die.
	\return A random number generated by the dice roll.

***************************************/

Word32 BURGER_API Burger::RandomBase::GetDice(Word32 uDiceCount,Word32 uDiceSize)
{
	Word32 uResult = 0;
	if (uDiceCount && uDiceSize) {

		// Prime the value with the dice count so 
		// there is no need to do a +1 per iteration
		uResult = uDiceCount;
		do {
			Word32 uTemp = GetWord(uDiceSize)+uResult;
			// Test for overflow
			// Likely will never happen, but you never know.
			if (uTemp<uResult) {
				uResult = BURGER_MAXUINT;
				break;
			}
			uResult = uTemp;
		} while (--uDiceCount);
	}
	return uResult;
}

/*! ************************************

	\brief Return a signed value in a specific range.
	
	Return a random number between -Range and +Range (Inclusive)
	and it's a SIGNED value.
	If Range = 3, then the value returned is -2 to 2 inclusive.
	0, and numbers higher than 0x7FFFFFFFU are illegal.
	
	\param uRange Range from 1 to MAX_INT-1.
	\return Signed value from -uRange to uRange (Inclusive)
	\sa Get() and GetFloat().

***************************************/

Int32 BURGER_API Burger::RandomBase::GetSigned(Word32 uRange)
{
	return static_cast<Int32>(GetWord(uRange<<1U)-uRange);		/* Get the random number */
}

/*! ************************************

	\brief Return a float from 0.0f to 0.99999f.
	
	Returns a random number in the range of 0.0f
	to 0.999999f. The numbers are spread evenly.
	
	\return Random float from 0.0 to 0.9999999999f
	\sa GetSigned(), GetSymmetricFloat() and Get().

***************************************/

float BURGER_API Burger::RandomBase::GetFloat(void)
{
	Int32 iValue = static_cast<Int32>(Get())&0x7FFFFFFF;		// Max 32 bit int
	// Convert to float
	return static_cast<float>(iValue)*
		(1.0f/static_cast<float>(0x80000000U));
}

/*! ************************************

	\brief Return a float from 0.0f to fRange.
	
	Returns a random number in the range of 0.0f
	to fRange. The numbers are spread evenly.
	
	\return Random float from 0.0 to fRange
	\sa GetFloat(void), GetSigned(Word32), GetSymmetricFloat(float) and GetWord(Word32).

***************************************/

float BURGER_API Burger::RandomBase::GetFloat(float fRange)
{
	Int32 iValue = static_cast<Int32>(Get())&0x7FFFFFFF;		// Max 32 bit int
	// Convert to float
	return fRange*static_cast<float>(iValue)*
		(1.0f/static_cast<float>(0x80000000U));
}

/*! ************************************

	\brief Return a float from -.0.99999f to 0.99999f.
	
	Returns a random number in the range of -.0.99999f
	to 0.999999f. The numbers are spread evenly.
	
	\return Random float from -.0.99999f to 0.9999999999f
	\sa GetSigned(), GetFloat() and Get().

***************************************/

float BURGER_API Burger::RandomBase::GetSymmetricFloat(void)
{
	Int32 iValue = static_cast<Int32>(Get());		// Max 32 bit int
	// Convert to float
	float fValue = static_cast<float>(iValue&0x7FFFFFFF)*
		(1.0f/static_cast<float>(0x80000000U));
	if (iValue&0x80000000) {
		fValue *= -1.0f;
	}
	return fValue;
}

/*! ************************************

	\brief Return a float from -fRange to fRange
	
	Returns a random number in the range of -fRange
	to fRange. The numbers are spread evenly.

	\return Random float from -fRange to fRange
	\sa GetSymmetricFloat(void), GetSigned(void), GetFloat(float) and GetWord(Word32).

***************************************/

float BURGER_API Burger::RandomBase::GetSymmetricFloat(float fRange)
{
	Int32 iValue = static_cast<Int32>(Get());		// Max 32 bit int
	// Convert to float
	float fValue = static_cast<float>(iValue&0x7FFFFFFF)*
		(1.0f/static_cast<float>(0x80000000U));
	fValue*=fRange;
	if (iValue&0x80000000) {
		fValue *= -1.0f;
	}
	return fValue;
}

/*! ************************************

	\brief Return a double from 0.0 to 0.999999999.
	
	Returns a random number in the range of 0.0
	to 0.999999. The numbers are spread evenly with 53 bit resolution.
	
	\return Random double from 0.0 to 0.9999999999
	\sa GetFloat() and Get().

***************************************/

double BURGER_API Burger::RandomBase::GetDouble(void)
{
	Int32 iUpper=static_cast<Int32>(Get()>>5U);
	Int32 iLower=static_cast<Int32>(Get()>>6U);

	// 0x20000000000000 = 9007199254740992.0
	return ((static_cast<double>(iUpper)*67108864.0)+static_cast<double>(iLower))*(1.0/9007199254740992.0);
}