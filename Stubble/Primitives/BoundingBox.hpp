#ifndef STUBBLE_BOUNDING_BOX_HPP
#define STUBBLE_BOUNDING_BOX_HPP

#include "Common\CommonTypes.hpp"
#include "Common\CommonConstants.hpp"
#include "Common\CommonFunctions.hpp"
#include "Primitives\Vector3D.hpp"

#ifdef MAYA
#include <maya\MBoundingBox.h>
#endif

#include <algorithm>
#include <limits>

namespace Stubble
{

///----------------------------------------------------------------------------------------------------
/// Representation of bounding box.
///----------------------------------------------------------------------------------------------------
class BoundingBox
{ 
public:

	///----------------------------------------------------------------------------------------------------
	/// Creates empty bounding box.
	///----------------------------------------------------------------------------------------------------
	inline BoundingBox();

#ifdef MAYA

	///----------------------------------------------------------------------------------------------------
	/// Maya bounding box constructor
	///----------------------------------------------------------------------------------------------------
	inline BoundingBox( const MBoundingBox &aBoundingBox );

	inline MBoundingBox toMBoundingBox() const;
#endif

	///----------------------------------------------------------------------------------------------------
	/// Expand the bounding box to include the given point.
	///
	/// \param aPoint	new point to include in the bounding box
	///----------------------------------------------------------------------------------------------------
	inline void expand( const Vector3D< Real > &aPoint );

	///-------------------------------------------------------------------------------------------------
	/// Expands the bounding box to include the bounding box. 
	///
	/// \param	aBoundingBox	The new bounding box to include in the bounding box. 
	///-------------------------------------------------------------------------------------------------
	inline void expand( const BoundingBox & aBoundingBox );

	///-------------------------------------------------------------------------------------------------
	/// Clears this object to its blank/initial state. 
	///-------------------------------------------------------------------------------------------------
	inline void clear();

	///----------------------------------------------------------------------------------------------------
	/// Returns the minimum point for the bounding box.
	///----------------------------------------------------------------------------------------------------
	inline Vector3D< Real > min() const;

	///----------------------------------------------------------------------------------------------------
	/// Returns the maximum point for the bounding box.
	///----------------------------------------------------------------------------------------------------
	inline Vector3D< Real > max() const;

	///----------------------------------------------------------------------------------------------------
	/// = casting operator. 
	///
	/// \param	aBoundingBox	a bounding box. 
	///----------------------------------------------------------------------------------------------------
	inline const BoundingBox & operator= ( const BoundingBox & aBoundingBox );

	///-------------------------------------------------------------------------------------------------
	/// Gets the size of the minimum side of bounding box. 
	///
	/// \return	The minimum size. 
	///-------------------------------------------------------------------------------------------------
	inline Real minSize() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the size of diagonal
	///
	/// \return	The diagonal size. 
	///-------------------------------------------------------------------------------------------------
	inline Real diagonal() const;

	///-------------------------------------------------------------------------------------------------
	/// Query if this object contains the given aPoint. 
	///
	/// \param	aPoint	The point to test for containment. 
	///
	/// \return	true if the object is in this bounding box, false if not. 
	///-------------------------------------------------------------------------------------------------
	inline bool contains( const Vector3D< Real > & aPoint ) const;

	///-------------------------------------------------------------------------------------------------
	/// Query if this object contains the given aBoundingBox. 
	///
	/// \param	aBoundingBox	The bounding box to test for containment. 
	///
	/// \return	true if the object is in this bounding box, false if not. 
	///-------------------------------------------------------------------------------------------------
	inline bool contains( const BoundingBox & aBoundingBox ) const;

private:
	Vector3D< Real > mMin; ///<	Minimum coordinate values.

	Vector3D< Real > mMax; ///<	Maximum coordinates values.
};

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the bounding boxes .
///-------------------------------------------------------------------------------------------------
typedef std::vector< BoundingBox > BoundingBoxes;

// inline functions implementation
inline BoundingBox::BoundingBox()
{
	clear();
}

#ifdef MAYA
inline BoundingBox::BoundingBox( const MBoundingBox &aBoundingBox )
{
	mMin = Vector3D< Real >( aBoundingBox.min().x, aBoundingBox.min().y, aBoundingBox.min().z );
	mMax = Vector3D< Real >( aBoundingBox.max().x, aBoundingBox.max().y, aBoundingBox.max().z );
}

inline MBoundingBox BoundingBox::toMBoundingBox() const
{
	return MBoundingBox( mMin.toMayaPoint(), mMax.toMayaPoint() ); 
}

#endif

inline void BoundingBox::expand( const Vector3D<Real> &aPoint )
{
	mMin.x = std::min( aPoint.x, mMin.x );
	mMin.y = std::min( aPoint.y, mMin.y );
	mMin.z = std::min( aPoint.z, mMin.z );
	mMax.x = std::max( aPoint.x, mMax.x );
	mMax.y = std::max( aPoint.y, mMax.y );
	mMax.z = std::max( aPoint.z, mMax.z );
}

inline void BoundingBox::expand( const BoundingBox & aBoundingBox )
{
	mMin.x = std::min( aBoundingBox.mMin.x, mMin.x );
	mMin.y = std::min( aBoundingBox.mMin.y, mMin.y );
	mMin.z = std::min( aBoundingBox.mMin.z, mMin.z );
	mMax.x = std::max( aBoundingBox.mMax.x, mMax.x );
	mMax.y = std::max( aBoundingBox.mMax.y, mMax.y );
	mMax.z = std::max( aBoundingBox.mMax.z, mMax.z );
}

inline void BoundingBox::clear()
{
	Real infinity = std::numeric_limits<Real>::infinity();
	mMin = Vector3D< Real >( infinity, infinity, infinity );
	mMax = Vector3D< Real >( -infinity, -infinity, -infinity );
}


inline Vector3D< Real > BoundingBox::min() const
{
	return mMin;
}

inline Vector3D< Real > BoundingBox::max() const
{
	return mMax;
}

inline const BoundingBox & BoundingBox::operator= ( const BoundingBox & aBoundingBox )
{
	mMin = aBoundingBox.mMin;
	mMax = aBoundingBox.mMax;
	return *this;
}

inline Real BoundingBox::minSize() const
{
	Vector3D< Real > size = mMax - mMin;
	return size.x > size.y ? ( size.x > size.z ? size.x : size.z ) : ( size.y > size.z ? size.y : size.z );
}

inline Real BoundingBox::diagonal() const
{
	return ( mMax - mMin ).size();
}

inline bool BoundingBox::contains( const Vector3D< Real > & aPoint ) const
{
	return	aPoint.x <= mMax.x && 
			aPoint.y <= mMax.y &&
			aPoint.z <= mMax.z &&
			aPoint.x >= mMin.x &&
			aPoint.y >= mMin.y &&
			aPoint.z >= mMin.z;
}

inline bool BoundingBox::contains( const BoundingBox & aBoundingBox ) const
{
	return	aBoundingBox.mMax.x <= mMax.x && 
			aBoundingBox.mMax.y <= mMax.y &&
			aBoundingBox.mMax.z <= mMax.z &&
			aBoundingBox.mMin.x >= mMin.x &&
			aBoundingBox.mMin.y >= mMin.y &&
			aBoundingBox.mMin.z >= mMin.z;
}

} // Stubble

#endif // STUBBLE_BOUNDING_BOX_HPP