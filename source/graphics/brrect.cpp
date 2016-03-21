/***************************************

	Integer Rect handlers

	Copyright (c) 1995-2016 by Rebecca Ann Heineman <becky@burgerbecky.com>

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!

***************************************/

#include "brrect.h"

#if defined(BURGER_MAC)
#include <MacTypes.h>
#elif defined(BURGER_MACOSX)
#include <Carbon/Carbon.h>
#endif

/*! ************************************

	\struct Burger::Rect_t
	\brief Structure describing an integer precision 2D rectangle.
	
	Simple container structure that holds the upper left and
	lower right coordinates in integer precision.
	
	\sa Burger::Vector4D_t or Burger::Point2D_t

***************************************/

/*! ************************************

	\fn Burger::Rect_t::GetWidth(void) const
	\brief Return the width of a rectangle.
	
	Subtract the right value from the left value and returns the
	difference in the form of the rectangle's width.
	
	\return Width of the rectangle
	
	\sa GetHeight() and SetWidth()

***************************************/

/*! ************************************

	\fn Burger::Rect_t::GetHeight(void) const
	\brief Return the height of a rectangle.
	
	Subtract the bottom value from the top value and returns the
	difference in the form of the rectangle's height.
	
	\return Height of the rectangle
	
	\sa GetWidth() and SetHeight()

***************************************/

/*! ************************************

	\fn Burger::Rect_t::GetLeft(void) const
	\brief Return the leftmost X value of a rectangle.
	
	Return the left most value of the rectangle
	
	\return Left X of the rectangle
	
	\sa GetRight() or SetLeft()
	
	\note On unsorted rectangles, this could be a larger value than the rightmost
	value. Do not assume that this value is less than or equal to the rightmost X coordinate.

***************************************/

/*! ************************************

	\fn Burger::Rect_t::GetTop(void) const
	\brief Return the topmost Y value of a rectangle.
	
	Return the top most value of the rectangle
	
	\return Top Y of the rectangle
	
	\sa GetBottom() or SetTop()
	
	\note On unsorted rectangles, this could be a larger value than the bottommost
	value. Do not assume that this value is less than or equal to the bottommost Y coordinate.

***************************************/

/*! ************************************

	\fn Burger::Rect_t::GetRight(void) const
	\brief Return the rightmost X value of a rectangle.
	
	Return the right most value of the rectangle
	
	\return Right X of the rectangle
	
	\sa GetLeft() or SetRight()
	
	\note On unsorted rectangles, this could be a smaller value than the leftmost
	value. Do not assume that this value is greater than or equal to the leftmost X coordinate.

***************************************/

/*! ************************************

	\fn Burger::Rect_t::GetBottom(void) const
	\brief Return the bottommost Y value of a rectangle.
	
	Return the bottom most value of the rectangle
	
	\return Bottom Y of the rectangle
	
	\sa GetTop() or SetBottom()
	
	\note On unsorted rectangles, this could be a smaller value than the top most
	value. Do not assume that this value is greater than or equal to the topmost Y coordinate.

***************************************/

/*! ************************************

	\fn Burger::Rect_t::SetWidth(int)
	\brief Set the width of a rectangle.
	
	Set the right most X coordinate so the rectangle will be the
	requested width. No bounds checking is performed, so unsorted 
	rectangles are possible on overflow or underflow input.
	
	\param iWidth Requested width of the rectangle.
	
	\sa GetWidth() or SetHeight()

***************************************/

/*! ************************************

	\fn Burger::Rect_t::SetHeight(int)
	\brief Set the height of a rectangle.
	
	Set the bottom most Y coordinate so the rectangle will be the
	requested height. No bounds checking is performed, so unsorted 
	rectangles are possible on overflow or underflow input.
	
	\param iHeight Requested height of the rectangle.
	
	\sa GetHeight() or SetWidth()

***************************************/

/*! ************************************

	\fn Burger::Rect_t::SetSize(int,int)
	\brief Set the size of a rectangle.
	
	Set the right most X coordinate and the bottom most Y coordinate so the
	rectangle will be the requested width and height. No bounds checking is
	performed, so unsorted rectangles are possible on overflow or underflow input.
	
	\param iWidth Requested width of the rectangle.
	\param iHeight Requested height of the rectangle.
	
	\sa GetWidth(), GetHeight(), SetWidth() or SetHeight()

***************************************/

/*! ************************************

	\fn Burger::Rect_t::SetLeft(int)
	\brief Set the left most X coordinate of a rectangle.
	
	Set the left most X coordinate to a specific value. No bounds checking is
	performed to prevent unsorted rectangles.
	
	\param iLeft Requested left most X coordinate of the rectangle.
	
	\sa GetLeft() or SetRight()

***************************************/

/*! ************************************

	\fn Burger::Rect_t::SetTop(int)
	\brief Set the top most Y coordinate of a rectangle.
	
	Set the top most X coordinate to a specific value. No bounds checking is
	performed to prevent unsorted rectangles.
	
	\param iTop Requested top most Y coordinate of the rectangle.
	
	\sa GetTop() or SetBottom()

***************************************/

/*! ************************************

	\fn Burger::Rect_t::SetRight(int)
	\brief Set the right most X coordinate of a rectangle.
	
	Set the right most X coordinate to a specific value. No bounds checking is
	performed to prevent unsorted rectangles.
	
	\param iRight Requested right most X coordinate of the rectangle.
	
	\sa GetRight() or SetLeft()

***************************************/

/*! ************************************

	\fn Burger::Rect_t::SetBottom(int)
	\brief Set the bottom most Y coordinate of a rectangle.
	
	Set the bottom most Y coordinate to a specific value. No bounds checking is
	performed to prevent unsorted rectangles.
	
	\param iBottom Requested bottom most Y coordinate of the rectangle.
	
	\sa GetBottom() or SetTop()

***************************************/

/*! ************************************

	\fn Burger::Rect_t::Set(int,int,int,int)
	\brief Set all four coordinates of a rectangle.
	
	Sets the left, top, right and bottom coordinates of the rectangle to specific values.
	No bounds checking is performed to prevent unsorted rectangles.
	
	\param iLeft Requested left most X coordinate of the rectangle.
	\param iTop Requested top most Y coordinate of the rectangle.
	\param iRight Requested right most X coordinate of the rectangle.
	\param iBottom Requested bottom most Y coordinate of the rectangle.
	
	\sa SetSorted() or SetBySize()

***************************************/

/*! ************************************

	\fn Burger::Rect_t::SetBySize(int,int,int,int)
	\brief Set all four coordinates of a rectangle using upper left corner and width and height
	
	Sets the left, top, right and bottom coordinates of the rectangle to using iLeft and iTop
	as the upper left anchor point and iWidth and iHeight to determine what the new bottom right
	coordinate should be.
	No bounds checking is performed to prevent unsorted rectangles, espectially in the cases of negative width
	or height.
	
	\param iLeft Requested left most X coordinate of the rectangle.
	\param iTop Requested top most Y coordinate of the rectangle.
	\param iWidth Requested width of the rectangle.
	\param iHeight Requested height of the rectangle.
	
	\sa SetSorted(),Clear() or Set(int,int,int,int)

***************************************/

/*! ************************************

	\fn Burger::Rect_t::Clear(void)
	\brief Set all four coordinates of a rectangle to zero
	
	Initializes a Burger::Rect_t to an empty rectangle.
	
	\sa SetSorted(), SetBySize() or Set(int,int,int,int)

***************************************/

/*! ************************************

	\fn Burger::Rect_t::Offset(int,int)
	\brief Adjust all four coordinates by a horizontal and vertical offset
	
	Given a new X and Y offset value, adjust the location of the entire rectangle by
	the X and Y offsets. This is performed by addition of the offsets. No bounds
	checking is performed, so the rectangle could "wrap around"
	
	\param iX Number of units to add to the left and right coordinates
	\param iY Number of units to add to the top and bottom coordinates
	
	\sa MoveTo(int,int)

***************************************/

/*! ************************************

	\brief Move a rectangle to a new location
	
	Given an x,y point, "move" the rectangle so that bottom right corner
	maintains the same offset (Width/Height) from the new origin point
	as the previous origin point
	
	\param iX New upper left X coordinate
	\param iY New upper top Y coordinate
	
	\sa Offset(int,int), MoveTo(const Point2D_t *), MoveX(int) or MoveY(int)
	
***************************************/

void Burger::Rect_t::MoveTo(int iX,int iY)
{
	// Get the size of the rectangle
	int iWidth = m_iRight-m_iLeft;
	int iHeight = m_iBottom-m_iTop;
	m_iLeft = iX;		// Set the new upper left corner
	m_iTop = iY;
	iWidth += iX;		// New lower right
	iHeight += iY;
	m_iRight = iWidth;	// Save it
	m_iBottom = iHeight;
}

/*! ************************************

	\brief Move a rectangle to a new location
	
	Given an x,y point, "move" the rectangle so that bottom right corner
	maintains the same offset (Width/Height) from the new origin point
	as the previous origin point
	
	\param pInput Pointer to a valid Point2D_t structure.
	
	\sa Offset(int,int), MoveTo(int,int), MoveX(int) or MoveY(int)
	
***************************************/

void Burger::Rect_t::MoveTo(const Point2D_t *pInput)
{
	int iX = pInput->m_iX;
	int iY = pInput->m_iY;
	// Get the size of the rectangle
	int iWidth = m_iRight-m_iLeft;
	int iHeight = m_iBottom-m_iTop;
	m_iLeft = iX;		// Set the new upper left corner
	m_iTop = iY;
	iWidth += iX;		// New lower right
	iHeight += iY;
	m_iRight = iWidth;	// Save it
	m_iBottom = iHeight;
}

/*! ************************************

	\brief Move a rectangle to a specific x coordinate
	
	\param iX New upper left X coordinate
	\sa MoveTo(int,int), Offset(int,int) or MoveY(int)

***************************************/

void Burger::Rect_t::MoveX(int iX)
{
	int iWidth = m_iRight-m_iLeft;	// Get the width
	m_iLeft = iX;					// Set the new X 
	m_iRight = iX+iWidth;			// Adjust the right side
}

/*! ************************************

	\brief Move a rectangle to a specific y coordinate
	\param iY New upper top Y coordinate
	\sa MoveTo(int,int), Offset(int,int) or MoveX(int)

***************************************/

void Burger::Rect_t::MoveY(int iY)
{
	int iHeight = m_iBottom-m_iTop;	// Get the height
	m_iTop = iY;					// Set the new Y
	m_iBottom = iY+iHeight;			// Adjust the bottom side
}

/*! ************************************

	\brief Return \ref TRUE if the X and Y coordinates are inside of a rectangle

	Given an X and Y coordinate, determine if it is inside of a rect
	if so, then return \ref TRUE, otherwise, return \ref FALSE.
	
	Determination of whether a point is inside is that the value must be greater than
	or equal to the minimum and less than the maximum. If the rectangle has a width or
	height of zero or less, then this function always returns \ref FALSE

	\param iX X coordinate to test
	\param iY Y coordinate to test
	
	\sa IsInside(const Point2D_t *) const or IsInside(const Rect_t *pRect) const

***************************************/

Word Burger::Rect_t::IsInside(int iX,int iY) const
{
	if ((iX >= m_iLeft) && (iX < m_iRight) && (iY >= m_iTop) && (iY < m_iBottom)) {
		return TRUE;	// No intersection!
	}
	return FALSE;		// I intersect!
}

/*! ************************************

	\brief Return \ref TRUE if the Point2D_t is inside of a rectangle

	Given an X and Y coordinate inside of a \ref Point2D_t, determine if it is inside of a rect
	if so, then return \ref TRUE, otherwise, return \ref FALSE.
	
	Determination of whether a point is inside is that the value must be greater than
	or equal to the minimum and less than the maximum. If the rectangle has a width or
	height of zero or less, then this function always returns \ref FALSE

	\param pPoint Pointer to a valid Point2D_t structure.
	
	\sa IsInside(int,int) const or IsInside(const Rect_t *pRect) const

***************************************/

Word Burger::Rect_t::IsInside(const Point2D_t *pPoint) const
{
	int iX = pPoint->m_iX;
	int iY = pPoint->m_iY;
	if ((iX >= m_iLeft) && (iX < m_iRight) && (iY >= m_iTop) && (iY < m_iBottom)) {
		return TRUE;	// No intersection!
	}
	return FALSE;		// I intersect!
}

/*! ************************************

	\brief Return \ref TRUE if the input rectangle is completely inside of the rectangle
	
	Given an Rect_t, test it against the bounds of this rectangle
	and return \ref TRUE if the input rectangle is completely enclosed by
	this rectangle.
	
	\param pInput Pointer to a valid Rect_t to test against
	\sa IsInside(int,int) const or IsInside(const Point2D_t *) const

***************************************/

Word Burger::Rect_t::IsInside(const Rect_t *pInput) const
{
	if ((m_iLeft >= pInput->m_iLeft) && (pInput->m_iRight >= m_iRight) &&
		(m_iTop >= pInput->m_iTop) && (pInput->m_iBottom >= m_iBottom)) {
		return TRUE;		// Input1 is inside Input2
	}
	return FALSE;			// Input1 is outside
}

/*! ************************************

	\brief Ensure that the rectangle is sorted
	
	If a rectangle is sorted (Left<=Right and Top<=Bottom)
	this function does nothing. Otherwise, the Left and Right
	entries and the Top and Bottom entries are sorted
	so they pass the above test. This will fix
	any rectangle that may have reveresed entries.
	
	\sa SetSorted(int,int,int,int)

***************************************/

void Burger::Rect_t::Sort(void)
{
	int iMin = m_iLeft;		// Get the x edges
	int iMax = m_iRight;
	if (iMin > iMax) {		// I swap the x's
		m_iLeft = iMax;
		m_iRight = iMin;
	}
	iMin = m_iTop;			// Get the y edges
	iMax = m_iBottom;
	if (iMin > iMax) {
		m_iTop = iMax;		// I swap the y's
		m_iBottom = iMin;
	}
}

/*! ************************************

	\brief Set all four coordinates of a rectangle and ensure they are sorted.
	
	Sets the left, top, right and bottom coordinates of the rectangle to specific values.
	The Left and Right are sorted so that the Left side is less than or equal to the Right
	and the Top and Bottom sides are sorted as well.
	
	Due to the sorting, there is a slight performance penalty to perform
	the tests. If it's known in advance that the coordinates are sorted, use
	\ref Set(int,int,int,int) instead.
	
	\param iLeft First X coordinate of the rectangle.
	\param iTop First Y coordinate of the rectangle.
	\param iRight Second X coordinate of the rectangle.
	\param iBottom Second Y coordinate of the rectangle.
	
	\sa Set(int,int,int,int) or SetBySize(int,int,int,int)

***************************************/

void Burger::Rect_t::SetSorted(int iLeft,int iTop,int iRight,int iBottom)
{
	int iTemp;
	if (iRight<iLeft) {		// Make sure that iLeft is less than iRight
		iTemp = iLeft;		// Swap the x's!
		iLeft = iRight;
		iRight = iTemp;
	}
	m_iLeft	= iLeft;		// Save the leftmost and rightmost points
	m_iRight = iRight;
	
	if (iBottom<iTop) {		// Make sure that y1 is less than y2
		iTemp = iTop;		// Swap the y's!
		iTop = iBottom;
		iBottom = iTemp;
	}
	m_iTop = iTop;			// Save the highest and lowest points
	m_iBottom = iBottom;
}

/*! ************************************

	\brief Return \ref TRUE if the rect is empty
	
	If either the horizontal or vertical coordinates are equal, this 
	will be considered an empty rectangle.
	
	\sa IsNotEmpty()

***************************************/

Word Burger::Rect_t::IsEmpty(void) const
{
	if ((m_iTop==m_iBottom) || (m_iLeft==m_iRight)) {
		return TRUE;	// At least one point was too small...
	}
	return FALSE;		// Empty rect!
}

/*! ************************************

	\brief Return \ref TRUE if the rect is not empty

	If both the horizontal or vertical coordinates are not equal, this 
	will be considered a valid rectangle.

	\sa IsEmpty()
	
***************************************/

Word Burger::Rect_t::IsNotEmpty(void) const
{
	if ((m_iTop==m_iBottom) || (m_iLeft==m_iRight)) {
		return FALSE;	// It's empty!
	}
	return TRUE;		// Non-empty rect!
}

/*! ************************************

	\brief Return \ref TRUE if the two rectangles are equal

	If all four entries are the same, return \ref TRUE, otherwise
	return \ref FALSE.
	
	\param pInput Pointer to a valid Rect_t to test against
	
	\sa IsNotEqual(const Rect_t *) const or Rect_t::operator==(const Rect_t &) const

***************************************/

Word Burger::Rect_t::IsEqual(const Rect_t *pInput) const
{
	// Perform this branchless
	
	int iResult = (m_iLeft^pInput->m_iLeft) |
		(m_iTop^pInput->m_iTop) |
		(m_iRight^pInput->m_iRight) |
		(m_iBottom^pInput->m_iBottom);
	return iResult==0;
}

/*! ************************************

	\brief Return \ref TRUE if the two rectangles are not equal

	If all four entries are the same, return \ref FALSE, otherwise
	return \ref TRUE.
	
	\param pInput Pointer to a valid Rect_t to test against
	
	\sa IsEqual(const Rect_t *) const or Rect_t::operator!=(const Rect_t &) const

***************************************/

Word Burger::Rect_t::IsNotEqual(const Rect_t *pInput) const
{
	// Perform this branchless

	int iResult = (m_iLeft^pInput->m_iLeft) |
		(m_iTop^pInput->m_iTop) |
		(m_iRight^pInput->m_iRight) |
		(m_iBottom^pInput->m_iBottom);
	return iResult!=0;
}

/*! ************************************

	\fn Burger::Rect_t::operator==(const Rect_t &) const
	\brief Return \ref TRUE if the two rectangles are equal

	If all four entries are the same, return \ref TRUE, otherwise
	return \ref FALSE.
	
	\param rInput Reference to a valid Rect_t to test against
	
	\sa IsEqual(const Rect_t *) const or Rect_t::operator!=(const Rect_t &) const

***************************************/

/*! ************************************

	\fn Burger::Rect_t::operator!=(const Rect_t &) const
	\brief Return \ref TRUE if the two rectangles are not equal

	If all four entries are the same, return \ref FALSE, otherwise
	return \ref TRUE.
	
	\param rInput Reference to a valid Rect_t to test against
	
	\sa IsNotEqual(const Rect_t *) const or Rect_t::operator==(const Rect_t &) const

***************************************/

/*! ************************************

	\brief Shrink or expand a rect by a specific number of units.
	
	If positive offsets are given, the rectangle is shrunk by the number of units.
	The rectangle is grown if the offsets are negative values.
	the coordinates are sorted after adjustment.
	
	\param iX Number of units to shrink the rectangle horizontally
	\param iY Number of units to shrink the rectangle vertically
	
	\sa Offset(int,int)

***************************************/

void Burger::Rect_t::Inset(int iX,int iY)
{
	int iLeft = m_iLeft + iX;	// Move the left towards the center
	int iRight = m_iRight - iX;	// Move the right towards the center
	int iTemp;
	if (iRight<iLeft) {			// Make sure that iLeft is less than iRight
		iTemp = iLeft;			// Swap the x's!
		iLeft = iRight;
		iRight = iTemp;
	}
	m_iLeft	= iLeft;			// Save the leftmost and rightmost points
	m_iRight = iRight;
	
	int iTop = m_iTop + iY;		// Etc... Etc...
	int iBottom = m_iBottom - iY;
	if (iBottom<iTop) {			// Make sure that y1 is less than y2
		iTemp = iTop;			// Swap the y's!
		iTop = iBottom;
		iBottom = iTemp;
	}
	m_iTop = iTop;				// Save the highest and lowest points
	m_iBottom = iBottom;
}

/*! ************************************

	\brief Return \ref TRUE if the two rectangles intersect.
	
	Test the rectangle against this one and if they are intersecting, 
	return \ref TRUE. This function assumes the rectangles are sorted.
	
	\param pInput Pointer to a valid Rect_t
	\sa Intersection(const Rect_t *,const Rect_t *)

***************************************/

Word Burger::Rect_t::IsIntersecting(const Rect_t *pInput) const
{
	if ((m_iLeft < pInput->m_iRight) &&		// Test the left
		(m_iRight >= pInput->m_iLeft) &&
		(m_iTop < pInput->m_iBottom) &&		// Test the left
		(m_iBottom >= pInput->m_iTop)) {
		return TRUE;
	}
	return FALSE;
}

/*! ************************************

	\brief Create the intersection of two rectangles
	
	Create the intersection of two rects and return \ref TRUE
	if they truly intersect.
	
	\param pInput1 Pointer to the first valid Rect_t structure
	\param pInput2 Pointer to the second valid Rect_t structure
	
	\note It is okay to pass the output rectangle as one of the input rectangles
	
	\sa IsIntersecting(const Rect_t *) const or Union(const Rect_t *,const Rect_t *)

***************************************/

Word Burger::Rect_t::Intersection(const Rect_t *pInput1,const Rect_t *pInput2)
{
	int iLeft = pInput1->m_iLeft;	// Get the higher of the two lefts
	int iTemp = pInput2->m_iLeft;
	if (iLeft < iTemp) {
		iLeft = iTemp;
	}

	int iRight = pInput1->m_iRight;	// Get the lower of the two rights
	iTemp = pInput2->m_iRight;
	if (iRight >= iTemp) {
		iRight = iTemp;
	}

	int iTop = pInput1->m_iTop;
	iTemp = pInput2->m_iTop;
	if (iTop < iTemp) {				// Get the lower value for top
		iTop = iTemp;
	}

	int iBottom = pInput1->m_iBottom;
	iTemp = pInput2->m_iBottom;
	if (iBottom >= iTemp) {			// Get the greater value to bottom
		iBottom = iTemp;
	}

	Word uResult = TRUE;			// Assume OK
	if ((iLeft >= iRight) || (iTop >= iBottom)) {	// Non-Empty rect?
		uResult = FALSE;			// This is also FALSE
		iLeft = 0;		// Force an empty rect
		iRight = 0;
		iTop = 0;
		iBottom = 0;
	}
	// Save the resulting rectangle
	m_iLeft = iLeft;
	m_iTop = iTop;
	m_iRight = iRight;
	m_iBottom = iBottom;
	return uResult;		// Return TRUE or FALSE
}

/*! ************************************

	\brief Generate the union between two rects
	
	Create a rectangle that will encompass both input retangles
	
	\param pInput1 Pointer to the first non-empty rectangle
	\param pInput2 Pointer to the second non-empt rectangle
	
	\sa Intersection(const Rect_t *,const Rect_t *);
	
***************************************/

void Burger::Rect_t::Union(const Rect_t *pInput1,const Rect_t *pInput2)
{
	int iTemp = pInput1->m_iLeft;	// Preload left
	int iTemp2 = pInput2->m_iLeft;
	if (iTemp>iTemp2) {				// Use smaller
		iTemp = iTemp2;
	}
	m_iLeft = iTemp;

	iTemp = pInput1->m_iTop;		// Preload top
	iTemp2 = pInput2->m_iTop;
	if (iTemp>iTemp2) {				// Use smaller
		iTemp = iTemp2;
	}
	m_iTop = iTemp;

	iTemp = pInput1->m_iRight;		// Preload right
	iTemp2 = pInput2->m_iRight;
	if (iTemp<iTemp2) {				// Use larger
		iTemp = iTemp2;
	}
	m_iRight = iTemp;

	iTemp = pInput1->m_iBottom;		// Preload bottom
	iTemp2 = pInput2->m_iBottom;
	if (iTemp<iTemp2) {				// Use larger
		iTemp = iTemp2;
	}
	m_iBottom = iTemp;
}

/*! ************************************

	\brief Expand a rectangle to encompass a new point.
	
	If a passed point is outside of the bounds of the current rect,
	expand the rectangle to contain the point.

	If the rectangle is empty, initialize the rectangle to encompass only the point
	with a width and height of one.
	
	\param pInput Pointer to a valid Point2D_t structure
	
	\sa Add(int,int)

***************************************/

void Burger::Rect_t::Add(const Point2D_t *pInput)
{
	int iX = pInput->GetX();
	int iY = pInput->GetY();
	int iLeft = m_iLeft;
	int iTop = m_iTop;
	int iRight = m_iRight;
	int iBottom = m_iBottom;
	if ((iLeft==iRight) || (iTop==iBottom)) {
		m_iLeft = iX;			// Create a single pixel rect!
		m_iRight = iX+1;		// 1 pixel wide
		m_iTop = iY;
		m_iBottom = iY+1;		// 1 pixel tall
	} else {
		if (iX<iLeft) {			// Off the left side?
			m_iLeft = iX;		// Set the new left side
		}
		if (iX>=iRight) {		// Off the right side?
			m_iRight = iX+1;	// Set the new right side?
		}
		if (iY<iTop) {			// Off the top?
			m_iTop = iY;		// Set the new top
		}
		if (iY>=iBottom) {		// Off the bottom?
			m_iBottom = iY+1;	// Set the new bottom
		}
	}
}

/*! ************************************

	\brief Expand a rectangle to encompass a new point.
	
	If a passed point is outside of the bounds of the current rect,
	expand the rectangle to contain the point.

	If the rectangle is empty, initialize the rectangle to encompass only the point
	with a width and height of one.

	\param iX X coordinate of the new point to add
	\param iY Y coordinate of the new point to add
	
	\sa Add(const Point2D_t *)
	
***************************************/

void Burger::Rect_t::Add(int iX,int iY)
{
	int iLeft = m_iLeft;
	int iTop = m_iTop;
	int iRight = m_iRight;
	int iBottom = m_iBottom;
	if ((iLeft==iRight) || (iTop==iBottom)) {
		m_iLeft = iX;			// Create a single pixel rect!
		m_iRight = iX+1;		// 1 pixel wide
		m_iTop = iY;
		m_iBottom = iY+1;		// 1 pixel tall
	} else {
		if (iX<iLeft) {			// Off the left side?
			m_iLeft = iX;		// Set the new left side
		}
		if (iX>=iRight) {		// Off the right side?
			m_iRight = iX+1;	// Set the new right side?
		}
		if (iY<iTop) {			// Off the top?
			m_iTop = iY;		// Set the new top
		}
		if (iY>=iBottom) {		// Off the bottom?
			m_iBottom = iY+1;	// Set the new bottom
		}
	}
}

/*! ************************************

	\brief Clip a rectangle to a bounds rectangle
	
	Clip the rectangle to the boundaries set by the input rectangle.
	If this rectangle is outside of the boundaries, set it to
	an empty rectangle with all entries being set to zero.
	
	\param pInput Pointer to a valid boundary rectangle
	
	\sa Union(const Rect_t *,const Rect_t *)

***************************************/

void Burger::Rect_t::Clip(const Rect_t *pInput)
{
	int iLeft = m_iLeft;
	int iTop = m_iTop;
	int iRight = m_iRight;
	int iBottom = m_iBottom;
	const int iLeft2 = pInput->m_iLeft;
	const int iTop2 = pInput->m_iTop;
	const int iRight2 = pInput->m_iRight;
	const int iBottom2 = pInput->m_iBottom;
	if ((iBottom >= iTop2) &&	// Outside of the clip rect?
		(iTop < iBottom2) &&
		(iRight >= iLeft2) &&
		(iLeft < iRight2)) {
		if (iLeft < iLeft2) {	// Clip the left?
			m_iLeft = iLeft2;
		}
		if (iTop < iTop2) {		// Clip the top?
			m_iTop = iTop2;
		}
		if (iRight2 < iRight) {		// Clip the right
			m_iRight = iRight2;
		}
		if (iBottom2 < iBottom) {	// Clip the bottom
			m_iBottom = iBottom2;
		}
	} else {
		m_iLeft = 0;			// This is an empty rect
		m_iTop = 0;
		m_iRight = 0;
		m_iBottom = 0;
	}
}

/*! ************************************

	\fn void Burger::Rect_t::Get(Rect *pOutput) const
	\brief Convert a Rect_t into a MacOS Rect structure.
	
	Converts the contents of a Rect_t into a MacOS/Quicktime
	for Windows Rect structure. Since it's converting an int into a short,
	data loss could occur if the values exceed 32767 or are less than -32768.
	
	\param pOutput Pointer to an uninitialized MacOS Rect structure.
	\note This function is MacOS or Windows using Quicktime
	only.

***************************************/

/*! ************************************

	\fn void Burger::Rect_t::Set(const Rect *pInput)
	\brief Convert a MacOS Rect structure into a Rect_t
	
	Converts the contents of a MacOS/Quicktime for Windows Rect structure
	into a Rect_t. Since it's converting a short to an int, no
	data loss will occur.
	
	\param pInput Pointer to a valid MacOS Rect structure.
	\note This function is MacOS or Windows using Quicktime
	only.

***************************************/

/*! ************************************

	\fn void Burger::Rect_t::Get(tagRECT *pOutput) const
	\brief Convert a Rect_t into a Windows RECT structure.
	
	Converts the contents of a Rect_t into a Windows RECT structure.
	
	\param pOutput Pointer to an uninitialized Windows RECT structure.
	\note This function is Windows or Xbox 360 only.

***************************************/

/*! ************************************

	\fn void Burger::Rect_t::Set(const tagRECT *pInput)
	\brief Convert a Windows RECT structure into a Rect_t
	
	Converts the contents of a Windows RECT structure
	into a Rect_t.
	
	\param pInput Pointer to a valid Windows RECT structure.
	\note This function is Windows or Xbox 360 only.

***************************************/

#if 0

/***************************************

	Move a rect so that it is encompassed by a larger rect.
	I favor bounding to the top-leftmost edge in the case that the
	bounds rect cannot fit the rect in question.

***************************************/

void BURGER_API LBRectMoveWithinRect(LBRect *Input,const LBRect *Bounds)
{
	if (Input->right > Bounds->right) {
		LBRectMoveX(Input,Bounds->right - LBRectWidth(Input));
	}
	if (Input->left < Bounds->left) {	/* Do this AFTER right to favor left */
		LBRectMoveX(Input,Bounds->left);
	}
	if (Input->bottom > Bounds->bottom) {
		LBRectMoveY(Input,Bounds->bottom - LBRectHeight(Input));
	}
	if (Input->top < Bounds->top) {		/* Do this AFTER bottom to favor top */
		LBRectMoveY(Input,Bounds->top);
	}
}




/***************************************

	Find and return the center position of a rect
	I use this form instead of (left+right)>>1 since it prevents
	overflow errors
	
***************************************/

void BURGER_API LBRectGetCenter(int *x,int *y,const LBRect *Input)
{
	*x = (LBRectWidth(Input)>>1)+Input->left;		/* Get the center X */
	*y = (LBRectHeight(Input)>>1)+Input->top;		/* Get the center Y */
}


/***************************************

	Return the center x position of a rect

***************************************/

int BURGER_API LBRectGetCenterX(const LBRect *Input)
{
	return (LBRectWidth(Input)>>1)+Input->left;		/* Get the center X */
}


/***************************************

	Return the center y position of a rect

***************************************/

int BURGER_API LBRectGetCenterY(const LBRect *Input)
{
	return (LBRectHeight(Input)>>1)+Input->top;		/* Get the center Y */
}

/***************************************

	Find and return the center position of a rect

***************************************/

void BURGER_API LBRectGetCenterPoint(LBPoint *Output,const LBRect *Input)
{
	Output->x = (LBRectWidth(Input)>>1)+Input->left;		/* Get the center X */
	Output->y = (LBRectHeight(Input)>>1)+Input->top;		/* Get the center Y */
}

/***************************************

	Move a rect around a center point

***************************************/

void BURGER_API LBRectCenterAroundPoint(LBRect *Output,const LBPoint *Input)
{
	LBRectMove(Output,
		Input->x-(LBRectWidth(Output)>>1),		/* New X coord */
		Input->y-(LBRectHeight(Output)>>1));	/* New Y coord */
}


/***************************************

	Move a rect around a center x,y

***************************************/

void BURGER_API LBRectCenterAroundXY(LBRect *Output,int x,int y)
{
	LBRectMove(Output,
		x-(LBRectWidth(Output)>>1),		/* New X coord */
		y-(LBRectHeight(Output)>>1));	/* New Y coord */
}


/***************************************

	Move a rect around a center x

***************************************/

void BURGER_API LBRectCenterAroundX(LBRect *Output,int x)
{
	LBRectMoveX(Output,
		x-(LBRectWidth(Output)>>1));	/* New X coord */
}


/***************************************

	Move a rect around a center y

***************************************/

void BURGER_API LBRectCenterAroundY(LBRect *Output,int y)
{
	LBRectMoveY(Output,
		y-(LBRectHeight(Output)>>1));	/* New Y coord */
}


/***************************************

	Move a rect around the center of another rect

***************************************/

void BURGER_API LBRectCenterAroundRectCenter(LBRect *Output,const LBRect *Input)
{
	LBRectCenterAroundXY(Output,
		(LBRectWidth(Input)>>1)+Input->left,		/* Center X */
		(LBRectHeight(Input)>>1)+Input->top);		/* Center Y */
}


/***************************************

	Move a rect around the center of another rect
	but only in the X direction

***************************************/

void BURGER_API LBRectCenterAroundRectCenterX(LBRect *Output,const LBRect *Input)
{
	LBRectCenterAroundX(Output,
		(LBRectWidth(Input)>>1)+Input->left);		/* Center X */
}

/***************************************

	Move a rect around the center of another rect
	but only in the y direction

***************************************/

void BURGER_API LBRectCenterAroundRectCenterY(LBRect *Output,const LBRect *Input)
{
	LBRectCenterAroundY(Output,
		(LBRectHeight(Input)>>1)+Input->top);		/* Center X */
}

/***************************************

	I take the input Point which is in direct relation to the SrcBoundsRect
	and map it to the DestBoundsRect as if the SrcBoundsRect was stretched to
	fit the DestBoundsRect and the Input Point stretches in the same proportions.
	
***************************************/

void BURGER_API LBRectMapPoint(LBPoint *Output,const LBRect *SrcBoundsRect,
	const LBRect *DestBoundsRect,const Burger::Point2D_t *Input)
{
	Fixed32 WidthRatio;
	Fixed32 HeightRatio;
	
	/* First thing i do is get the ratios between the width and heigths */
	/* of the two bounds rects */
	
	WidthRatio = (LBRectWidth(DestBoundsRect)<<16)/LBRectWidth(SrcBoundsRect);
	HeightRatio = (LBRectHeight(DestBoundsRect)<<16)/LBRectHeight(SrcBoundsRect);	

	/* Now I create the source point x and y */
	
	Output->x = Input->x-SrcBoundsRect->left;
	Output->y = Input->y-SrcBoundsRect->top;

	/* Let's scale the point! */
	
	Output->x = Burger::IntMath::FromFixedNearest(Output->x*WidthRatio);
	Output->y = Burger::IntMath::FromFixedNearest(Output->y*HeightRatio);

	/* Add the new origin */
	
	Output->x = Output->x+DestBoundsRect->left;
	Output->y = Output->y+DestBoundsRect->top;
		
	/* Voila!! */
}

/***************************************

	I take the input rect which is in direct relation to the SrcBoundsRect
	and map it to the DestBoundsRect as if the SrcBoundsRect was stretched to
	fit the DestBoundsRect and the Input rect stretches in the same proportions.
	
***************************************/

void BURGER_API LBRectMapRect(LBRect *Output,const LBRect *SrcBoundsRect,
	const LBRect *DestBoundsRect,const LBRect *Input)
{
	Fixed32 WidthRatio;
	Fixed32 HeightRatio;
	
	/* First thing i do is get the ratios between the width and heigths */
	/* of the two bounds rects */
	
	WidthRatio = (LBRectWidth(DestBoundsRect)<<16)/LBRectWidth(SrcBoundsRect);
	HeightRatio = (LBRectHeight(DestBoundsRect)<<16)/LBRectHeight(SrcBoundsRect);	

	/* Now I create the source point x and y */
	
	Output->left = Input->left-SrcBoundsRect->left;
	Output->top = Input->top-SrcBoundsRect->top;
	Output->right = Input->right-SrcBoundsRect->left;
	Output->bottom = Input->bottom-SrcBoundsRect->top;

	/* Let's scale the point! */
	
	Output->left = Burger::IntMath::FromFixedNearest(Output->left*WidthRatio);
	Output->top = Burger::IntMath::FromFixedNearest(Output->top*HeightRatio);
	Output->right = Burger::IntMath::FromFixedNearest(Output->right*WidthRatio);
	Output->bottom = Burger::IntMath::FromFixedNearest(Output->bottom*HeightRatio);

	/* Add the new origin */
	
	Output->left = Output->left+DestBoundsRect->left;
	Output->top = Output->top+DestBoundsRect->top;
	Output->right = Output->right+DestBoundsRect->left;
	Output->bottom = Output->bottom+DestBoundsRect->top;
		
	/* Voila!! */
}

/***************************************

	Convert an OS Rect to a Burgerlib Rect

***************************************/

#if !defined(BURGER_BEOS)
#define ADDER
#else
#define ADDER +1
#endif

void BURGER_API LBRectFromSYSRECT(LBRect *Output,const SYSRECT *Input)
{
	Output->top = (int)Input->top;
	Output->left = (int)Input->left;
	Output->bottom = (int)Input->bottom ADDER;
	Output->right = (int)Input->right ADDER;
}
#undef ADDER

/***************************************

	Convert a Burgerlib Rect to an OS Rect

***************************************/

#if defined(BURGER_MAC)
#define ADDER
#define RTYPE short		/* Macs use shorts */
#elif defined(BURGER_BEOS)
#define ADDER -1
#define RTYPE float
#else
#define ADDER
#define RTYPE long		/* All others use longs */
#endif

void BURGER_API LBRectToSYSRECT(SYSRECT *Output,const LBRect *Input)
{
	Output->top = (RTYPE)Input->top;
	Output->left = (RTYPE)Input->left;
	Output->bottom = (RTYPE)Input->bottom ADDER;
	Output->right = (RTYPE)Input->right ADDER;
}
#undef ADDER
#undef RTYPE

/***************************************

	Read a rect from a file

***************************************/

Word BURGER_API LBRectRead(LBRect *Output,FILE *fp)
{
	Int16 Input[4];
	
	if (fread(&Input[0],1,8,fp)==8) {
		Output->left = Burger::LittleEndian::Load(&Input[0]);
		Output->top = Burger::LittleEndian::Load(&Input[1]);
		Output->right = Burger::LittleEndian::Load(&Input[2]);
		Output->bottom = Burger::LittleEndian::Load(&Input[3]);
		return FALSE;
	}
	return TRUE;
}

/***************************************

	Write a rect to a file

***************************************/

Word BURGER_API LBRectWrite(const LBRect *Input,FILE *fp)
{
	Int16 Output[4];
	
	Output[0] = Burger::LittleEndian::Load(static_cast<Int16>(Input->left));
	Output[1] = Burger::LittleEndian::Load(static_cast<Int16>(Input->top));
	Output[2] = Burger::LittleEndian::Load(static_cast<Int16>(Input->right));
	Output[3] = Burger::LittleEndian::Load(static_cast<Int16>(Input->bottom));
	if (fwrite(&Output[0],1,8,fp)==8) {
		return FALSE;
	}
	return TRUE;
}


/***************************************

	Read a rect from a streamhandle

***************************************/

void BURGER_API LBRectReadStream(LBRect *Output,StreamHandle_t *fp)
{
	Output->left = StreamHandleGetShort(fp);
	Output->top = StreamHandleGetShort(fp);
	Output->right = StreamHandleGetShort(fp);
	Output->bottom = StreamHandleGetShort(fp);
}

/***************************************

	Write a rect to a stream

***************************************/

void BURGER_API LBRectWriteStream(const LBRect *Input,StreamHandle_t *fp)
{
	StreamHandlePutShort(fp,Input->left);
	StreamHandlePutShort(fp,Input->top);
	StreamHandlePutShort(fp,Input->right);
	StreamHandlePutShort(fp,Input->bottom);
}


/***************************************

	Allocate and init a new Rect list
	(Side effect, since all I need to do is blank the records,
	I just allocate zero'd out memory!)

***************************************/

LBRectList * BURGER_API LBRectListNew(void)
{
	return (LBRectList *)Burger::AllocClear(sizeof(LBRectList));
}

/***************************************

	Dispose of a rect list that was allocated

***************************************/

void BURGER_API LBRectListDelete(LBRectList *Input)
{
	if (Input) {			/* Valid input? */
		DeallocAHandle((void **)Input->RectList);	/* Release the CRectList */
		Burger::Free(Input);		/* Dispose of the memory */
	}
}


/***************************************

	Init a LBRectList record

***************************************/

void BURGER_API LBRectListInit(LBRectList *Input)
{
	Input->MaxRects = 0;		/* I assume no entries are valid */
	Input->NumRects = 0;		/* No entries are valid */
	Input->RectList = 0;		/* No memory is valid */
}

/***************************************

	Delete the contents of a LBRectList

***************************************/

void BURGER_API LBRectListDestroy(LBRectList *Input)
{
	if (Input) {
		DeallocAHandle((void **)Input->RectList);	/* Release the CRectList */
		Input->RectList = 0;	/* No entries are valid */
		Input->MaxRects = 0;	/* Empty */
		Input->NumRects = 0;		/* No rects are valid */
	}
}

/***************************************

	Return a rect list of the differance of rects b and t
	Return TRUE if there is an intersection and I modified the LBRectList.
	Note : This routine is recursive!!

***************************************/

Word BURGER_API LBRectListRectClip(LBRectList *Input,const LBRect* b,const LBRect* t)
{
	LBRect temp;		/* First temp rect */

	if ((b->left >= t->left) && (b->right <= t->right) && (b->top >= t->top) && (b->bottom <= t->bottom)) {
		return FALSE;	/* b is completely under t */
	}

	if ((b->right <= t->left) || (b->left >= t->right) || (b->bottom <= t->top) || (b->top >= t->bottom)) {
		LBRectListAppendRect(Input,b);
		return TRUE;	/* b is not at all under t */
	}

	temp = *b;		/* Get the bottom rect... */
	if (temp.top < t->top) {		/* piece above can be seen */
		temp.bottom = t->top;
		LBRectListAppendRect(Input,&temp);
		temp.bottom = b->bottom;
		temp.top = t->top;
	} else if (temp.bottom > t->bottom) { /* piece below can be seen */
		temp.top = t->bottom;
		LBRectListAppendRect(Input,&temp);
		temp.top = b->top;
		temp.bottom = t->bottom;
	} else if (temp.left < t->left) {		/* piece to the left can be seen */
		temp.right = t->left;
		LBRectListAppendRect(Input,&temp);
		temp.right = b->right;
		temp.left = t->left;
	} else { 							/* piece to the right can be seen */
		temp.left = t->right;
		LBRectListAppendRect(Input,&temp);
		temp.left = b->left;
		temp.right = t->right;
	}
	LBRectListRectClip(Input,&temp,t);		/* Call myself to finish subdividing */
	return TRUE;		/* I modified the LBRectlist */
}

/***************************************

	Return a rect list of the differance of rects b and t
	Return TRUE if there is an intersection and I modified the LBRectList

***************************************/

static LBRectList Rtemp;	/* This is static so I can keep a buffer around for */
							/* pretty much enternity. This way I can avoid calling AllocAHandle */
							/* and DeallocAHandle unnessasarly */

void BURGER_API LBRectListClipOutRect(LBRectList *Input,const LBRect *bound)
{
	Word i;				/* Temp */
	LBRect **RectHandle;	/* Handle to rect list */

	Rtemp.NumRects = 0;		/* Discard previous contents */
	RectHandle = Rtemp.RectList;	/* Get the handle */
	if (RectHandle) {		/* Does Handle exist? */
		if (!*RectHandle) {	/* Handle purged? */
			DeallocAHandle((void **)RectHandle);	/* Discard it */
			Rtemp.RectList = 0;	/* No Handles */
			Rtemp.MaxRects = 0;	/* No Handles */
		} else {
			SetHandlePurgeFlag((void **)RectHandle,FALSE);
		}
	}
	i = Input->NumRects;	/* Any source rects? */
	if (i) {
		LBRect *RectPtr;	/* Pointer to the rect list */
		RectPtr = (LBRect *)LockAHandle((void **)Input->RectList);
		do {
			LBRectListRectClip(&Rtemp,RectPtr,bound);	/* Clip the rect */
			++RectPtr;
		} while (--i);		/* All done? */
		UnlockAHandle((void **)Input->RectList);	/* Release the memory */
	}
	LBRectListCopy(Input,&Rtemp);		/* Copy the rect list into the resulting buffer */
	RectHandle = Rtemp.RectList;		/* Does a handle exit? */
	if (RectHandle) {		/* Mark as purgable */
		SetHandlePurgeFlag((void **)RectHandle,TRUE);	/* Mark as purgeable */
	}
}


/***************************************

	Return a rect list of the differance of rects b and t
	Return TRUE if there is an intersection and I modified the LBRectList

***************************************/

void BURGER_API LBRectListClipOutRectList(LBRectList *Input,const LBRectList *list)
{
	Word i;

	i = list->NumRects;		/* Get the rect count */
	if (i) {				/* Any rects? */
		LBRect *RectPtr;	/* Pointer to the rect list */
		RectPtr = (LBRect *)LockAHandle((void **)list->RectList);	/* Get the rect pointer */
		do {
			LBRectListClipOutRect(Input,RectPtr);	/* Clip out the rect */
			++RectPtr;			/* Next rect */
		} while (--i);			/* All scanned? */
		UnlockAHandle((void **)list->RectList);
	}
}

/***************************************

	Add a rect to the end of a LBRectList

***************************************/

void BURGER_API LBRectListAppendRect(LBRectList *Input,const LBRect *rect)
{
	if (Input->NumRects >= Input->MaxRects) {	/* Too tight? */
		void **NewMem;		/* Handle to memory */

		Input->MaxRects+=RECTLISTGROW;		/* Up the memory size */
		NewMem = ResizeAHandle((void **)Input->RectList,sizeof(LBRect)*Input->MaxRects);	/* Get a larger buffer */
		Input->RectList=(LBRect **)NewMem;	/* Save the new pointer */
		if (!NewMem) {		/* Error!?!?! */
			Input->MaxRects = 0;		/* Kill off the rect list */
			Input->NumRects = 0;
			return;			/* Foobar! */
		}
	}
	(*Input->RectList)[Input->NumRects] = *rect;		/* Append a rect */
	Input->NumRects++;		/* +1 to the number of valid rects */
}


/***************************************

	Add a rect list to the end of a LBRectList

***************************************/

void BURGER_API LBRectListAppendRectList(LBRectList *Input,const LBRectList *list)
{
	Word i;

	i = list->NumRects;		/* Get the number of rects to append */
	if (i) {
		LBRect *RectPtr;
		RectPtr = (LBRect *)LockAHandle((void **)list->RectList);	/* Lock it down */
		do {
			LBRectListAppendRect(Input,RectPtr);	/* Pass all the individual rects */
			++RectPtr;
		} while (--i);
		UnlockAHandle((void **)list->RectList);		/* Release it */
	}
}

/***************************************

	This is for speed. I will copy the valid rect count from list
	into Input, if the buffer is big enough, I'll just memcpy the rects as
	well. If not, then I will expand the buffer and do the copy

***************************************/

void BURGER_API LBRectListCopy(LBRectList *Input,const LBRectList *list)
{
	if (Input->MaxRects<list->NumRects) {	/* Buffer too small? */
		void **NewMem;		/* Handle to memory */
		NewMem = ResizeAHandle((void **)Input->RectList,sizeof(LBRect)*(list->NumRects+RECTLISTGROW));	/* Get a larger buffer */
		Input->RectList=(LBRect **)NewMem;	/* Save the new handle */
		if (!NewMem) {		/* Error?!?!? */
			Input->MaxRects = 0;		/* Reset the struct */
			Input->NumRects = 0;
			return;			/* Foobar! */
		}
		Input->MaxRects=list->NumRects+RECTLISTGROW;		/* Up the memory size */
	}
	FastMemCpy(*Input->RectList,*list->RectList,sizeof(Burger::Rect_t)*list->NumRects);
	Input->NumRects = list->NumRects;		/* Set the valid rect count */
}

#endif


