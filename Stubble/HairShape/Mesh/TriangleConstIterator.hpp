#ifndef STUBBLE_TRIANGLE_CONST_ITERATOR_HPP
#define STUBBLE_TRIANGLE_CONST_ITERATOR_HPP

#include "HairShape\Mesh\Triangle.hpp"

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// Class serving as iterator through all triangles of Mesh object.
/// Iterator can access all triangles data, but can not modify it.
///----------------------------------------------------------------------------------------------------
class TriangleConstIterator
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Constructor. 
	/// Creates iterator from standard vector iterators.
	///
	/// \param	aBegin	The begin internal iterator of triangles. 
	/// \param	aEnd	The end internal iterator of triangles. 
	///----------------------------------------------------------------------------------------------------
	inline TriangleConstIterator( Triangles::const_iterator aBegin, Triangles::const_iterator aEnd );

	///----------------------------------------------------------------------------------------------------
	/// Pre increment operator. 
	/// Moves to next triangle.
	///
	/// \return	Incremented iterator. 
	///----------------------------------------------------------------------------------------------------
	inline TriangleConstIterator & operator++ ();

	///----------------------------------------------------------------------------------------------------
	/// Gets the triangle identifier. 
	///
	/// \return	The triangle identifier. 
	///----------------------------------------------------------------------------------------------------
	inline unsigned __int32 getTriangleID() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the triangle data. 
	///
	/// \return	The triangle data. 
	///----------------------------------------------------------------------------------------------------
	inline const Triangle & getTriangle() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the triangles count. 
	///
	/// \return	The triangles count. 
	///----------------------------------------------------------------------------------------------------
	inline unsigned __int32 getTrianglesCount() const;

	///----------------------------------------------------------------------------------------------------
	/// Returns	true if iteration is over, false if it is not. 
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

TriangleConstIterator::TriangleConstIterator( Triangles::const_iterator aBegin, Triangles::const_iterator aEnd ):
	mBegin( aBegin ),
	mEnd( aEnd ),
	mIter( aBegin )
{
}

TriangleConstIterator & TriangleConstIterator::operator++ ()
{
	++mIter;
	return *this;
}

unsigned __int32 TriangleConstIterator::getTriangleID() const
{
	return static_cast< unsigned __int32 >( mIter - mBegin );
}

const Triangle & TriangleConstIterator::getTriangle() const
{
	return *mIter;
}

unsigned __int32 TriangleConstIterator::getTrianglesCount() const
{
	return static_cast< unsigned __int32 >( mEnd - mBegin );
}

inline bool TriangleConstIterator::end() const
{
	return mIter == mEnd;
}

void TriangleConstIterator::reset()
{
	mIter = mBegin;
}

TriangleConstIterator::~TriangleConstIterator()
{
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_TRIANGLE_CONST_ITERATOR_HPP