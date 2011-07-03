#ifndef STUBBLE_TRIANGLE_CONST_ITERATOR_HPP
#define STUBBLE_TRIANGLE_CONST_ITERATOR_HPP

#include "Triangle.hpp"

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// Triangle iterator. 
///----------------------------------------------------------------------------------------------------
class TriangleConstIterator
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Constructor. 
	///
	/// \param	aBegin	the begin internal iterator of triangles. 
	/// \param	aEnd	the end internal iterator of triangles. 
	///----------------------------------------------------------------------------------------------------
	inline TriangleConstIterator( Triangles::const_iterator aBegin, Triangles::const_iterator aEnd );

	///----------------------------------------------------------------------------------------------------
	/// Pre increment operator. 
	///
	/// \return	Incremented iterator. 
	///----------------------------------------------------------------------------------------------------
	inline TriangleConstIterator & operator++ ();

	///----------------------------------------------------------------------------------------------------
	/// Gets the triangle identifier. 
	///
	/// \return	The triangle identifier. 
	///----------------------------------------------------------------------------------------------------
	inline unsigned int getTriangleID() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the triangle. 
	///
	/// \return	The triangle. 
	///----------------------------------------------------------------------------------------------------
	inline const Triangle & getTriangle() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the triangles count. 
	///
	/// \return	The triangles count. 
	///----------------------------------------------------------------------------------------------------
	inline unsigned int getTrianglesCount() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the end. 
	/// 		
	/// \return	true if iteration is over, false if it is not. 
	///----------------------------------------------------------------------------------------------------
	inline bool end() const;

	///----------------------------------------------------------------------------------------------------
	/// Resets iterator. 
	///----------------------------------------------------------------------------------------------------
	inline void reset();

	///----------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///----------------------------------------------------------------------------------------------------
	inline ~TriangleConstIterator();
	
private:

	Triangles::const_iterator mBegin; ///< The internal iterator begin

	Triangles::const_iterator mEnd; ///< The internal iterator end

	Triangles::const_iterator mIter; ///< The internal iterator
};

///----------------------------------------------------------------------------------------------------
/// Constructor. 
///
/// \param	aBegin	the begin internal iterator of triangles. 
/// \param	aEnd	the end internal iterator of triangles. 
///----------------------------------------------------------------------------------------------------
TriangleConstIterator::TriangleConstIterator( Triangles::const_iterator aBegin, Triangles::const_iterator aEnd ):
	mBegin( aBegin ),
	mEnd( aEnd ),
	mIter( aBegin )
{
}

///----------------------------------------------------------------------------------------------------
/// Pre increment operator.
///
/// \return	Incremented iterator. 
///----------------------------------------------------------------------------------------------------
TriangleConstIterator & TriangleConstIterator::operator++ ()
{
	++mIter;
	return *this;
}

///----------------------------------------------------------------------------------------------------
/// Gets the triangle identifier. 
///
/// \return	The triangle identifier. 
///----------------------------------------------------------------------------------------------------
unsigned int TriangleConstIterator::getTriangleID() const
{
	return static_cast< unsigned int >( mIter - mBegin );
}

///----------------------------------------------------------------------------------------------------
/// Gets the triangle. 
///
/// \return	The triangle. 
///----------------------------------------------------------------------------------------------------
const Triangle & TriangleConstIterator::getTriangle() const
{
	return *mIter;
}

///----------------------------------------------------------------------------------------------------
/// Gets the triangles count. 
///
/// \return	The triangles count. 
///----------------------------------------------------------------------------------------------------
unsigned int TriangleConstIterator::getTrianglesCount() const
{
	return static_cast< unsigned int >( mEnd - mBegin );
}

///----------------------------------------------------------------------------------------------------
/// Gets the end. 
///	
/// \return	true if iteration is over, false if it is not. 
///----------------------------------------------------------------------------------------------------
inline bool TriangleConstIterator::end() const
{
	return mIter == mEnd;
}

///----------------------------------------------------------------------------------------------------
/// Resets iterator. 
///----------------------------------------------------------------------------------------------------
void TriangleConstIterator::reset()
{
	mIter = mBegin;
}

///----------------------------------------------------------------------------------------------------
/// Finaliser. 
///----------------------------------------------------------------------------------------------------
ConstTriangleIterator::~ConstTriangleIterator2()
{
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_TRIANGLE_CONST_ITERATOR_HPP