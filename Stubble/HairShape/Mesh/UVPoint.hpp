#ifndef STUBBLE_UV_POINT_HPP
#define STUBBLE_UV_POINT_HPP

#include "Common\CommonTypes.hpp"

#include <ostream>
#include <istream>

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// Class that holds UV coordinates.
///----------------------------------------------------------------------------------------------------
class UVPoint
{
	friend inline std::ostream & operator<<( std::ostream & aStreamOut, const UVPoint & aUVPoint );
	friend inline std::istream & operator>>( std::istream & aStreamIn, UVPoint & aUVPoint );
public:

	///----------------------------------------------------------------------------------------------------
	/// Default constructor
	///----------------------------------------------------------------------------------------------------
	inline UVPoint();

	///----------------------------------------------------------------------------------------------------
	/// Constructor
	///----------------------------------------------------------------------------------------------------
	inline UVPoint( Real aU, Real AV, unsigned __int32 aTriangleID );

	///----------------------------------------------------------------------------------------------------
	/// Gets the U part.
	///----------------------------------------------------------------------------------------------------
	inline Real getU() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the V part.
	///----------------------------------------------------------------------------------------------------
	inline Real getV() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets ID of the triangle.
	///----------------------------------------------------------------------------------------------------
	inline unsigned __int32 getTriangleID() const;

private:
	Real mU; ///< The U coordinate.

	Real mV; ///< The V coordinate.

	unsigned __int32 mTriangleID; ///< ID of triangle.
};

///----------------------------------------------------------------------------------------------------
/// Default constructor
///----------------------------------------------------------------------------------------------------
inline UVPoint::UVPoint()
{
}

///----------------------------------------------------------------------------------------------------
/// Constructor
///----------------------------------------------------------------------------------------------------
inline UVPoint::UVPoint( Real aU, Real aV, unsigned __int32 aTriangleID ):
	mU(aU),
	mV(aV),
	mTriangleID(aTriangleID)
{
}

///----------------------------------------------------------------------------------------------------
/// Gets the U part.
///----------------------------------------------------------------------------------------------------
inline Real UVPoint::getU() const
{
	return mU;
}

///----------------------------------------------------------------------------------------------------
/// Gets the V part.
///----------------------------------------------------------------------------------------------------
inline Real UVPoint::getV() const
{
	return mV;
}

///----------------------------------------------------------------------------------------------------
/// Gets ID of the triangle.
///----------------------------------------------------------------------------------------------------
inline unsigned __int32 UVPoint::getTriangleID() const
{
	return mTriangleID;
}

inline std::ostream & operator<<( std::ostream &aStreamOut, const UVPoint & aUVPoint )
{
	aStreamOut.write( reinterpret_cast< const char * >( &aUVPoint.mU ), sizeof( Real ) );
	aStreamOut.write( reinterpret_cast< const char * >( &aUVPoint.mV ), sizeof( Real ) );
	aStreamOut.write( reinterpret_cast< const char * >( &aUVPoint.mTriangleID ), sizeof( unsigned __int32 ) );
	return aStreamOut;
}

inline std::istream & operator>>( std::istream & aStreamIn, UVPoint & aUVPoint )
{
	aStreamIn.read( reinterpret_cast< char * >( &aUVPoint.mU ), sizeof( Real ) );
	aStreamIn.read( reinterpret_cast< char * >( &aUVPoint.mV ), sizeof( Real ) );
	aStreamIn.read( reinterpret_cast< char * >( &aUVPoint.mTriangleID ), sizeof( unsigned __int32 ) );
	return aStreamIn;
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_UV_POINT_HPP