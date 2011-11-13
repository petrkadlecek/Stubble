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
#include <vector>
#include <string>
#include <sstream>

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
	inline void expand( const Vector3D<Real> &aPoint );

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
	/// Gets the minimum size of bounding box. 
	///
	/// \return	The minimum size. 
	///-------------------------------------------------------------------------------------------------
	inline Real minSize() const;

	///-------------------------------------------------------------------------------------------------
	/// Serialize object.
	///-------------------------------------------------------------------------------------------------
	inline std::string serialize() const;	

	///-------------------------------------------------------------------------------------------------
	/// Deserialize object.	
	///
	/// \param	aStr	String from which to read.
	/// \param	aPos	Position at which to start.
	///-------------------------------------------------------------------------------------------------
	inline size_t deserialize( const std::string &aStr, size_t aPos );

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

inline std::string BoundingBox::serialize() const
{
	std::ostringstream oss;
	oss << mMin.serialize()
		<< mMax.serialize();
	return oss.str();
}

inline size_t BoundingBox::deserialize( const std::string &aStr, size_t aPos )
{
	aPos = mMin.deserialize( aStr, aPos );
	aPos = mMax.deserialize( aStr, aPos );
	return aPos;
}

} // Stubble

#endif // STUBBLE_BOUNDING_BOX_HPP