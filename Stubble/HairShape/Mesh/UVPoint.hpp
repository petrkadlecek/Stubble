#ifndef STUBBLE_UV_POINT_HPP
#define STUBBLE_UV_POINT_HPP

#include "Common\CommonTypes.hpp"
#include "Common\CommonConstants.hpp"
#include "Common\CommonFunctions.hpp"

#include <ostream>
#include <istream>

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// Class that holds triangle id and barycentric coordinates of point inside this triangle.
/// This class can be imported from/exported to binary stream.
///----------------------------------------------------------------------------------------------------
class UVPoint
{
	friend inline std::ostream & operator<<( std::ostream & aStreamOut, const UVPoint & aUVPoint );
	friend inline std::istream & operator>>( std::istream & aStreamIn, UVPoint & aUVPoint );
public:

	///----------------------------------------------------------------------------------------------------
	/// Default empty constructor.
	///----------------------------------------------------------------------------------------------------
	inline UVPoint();

	///----------------------------------------------------------------------------------------------------
	/// Constructor.
	/// Creates class from point on triangle.
	/// 
	/// \param aU			Barycentric coordinate U of point.
	/// \param aV			Barycentric coordinate V of point.
	/// \param aTriangleID	ID of triangle in which point lies.
	///----------------------------------------------------------------------------------------------------
	inline UVPoint( Real aU, Real AV, unsigned __int32 aTriangleID );

	///----------------------------------------------------------------------------------------------------
	/// Gets the barycentric coordinate U of point.
	/// 
	/// \return The barycentric coordinate U of point.
	///----------------------------------------------------------------------------------------------------
	inline Real getU() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the barycentric coordinate V of point.
	/// 
	/// \return The barycentric coordinate V of point.
	///----------------------------------------------------------------------------------------------------
	inline Real getV() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets ID of the triangle.
	/// 
	/// \return ID of triangle.
	///----------------------------------------------------------------------------------------------------
	inline unsigned __int32 getTriangleID() const;

	static const unsigned __int32 NOT_TRIANGLE = 0xFFFFFFFF;	///< The id of non-existing triangle

private:
	Real mU; ///< The barycentric coordinate U.

	Real mV; ///< The barycentric coordinate V.

	unsigned __int32 mTriangleID; ///< ID of triangle.
};

inline UVPoint::UVPoint()
{
}

inline UVPoint::UVPoint( Real aU, Real aV, unsigned __int32 aTriangleID ):
	mU(aU),
	mV(aV),
	mTriangleID(aTriangleID)
{
}

inline Real UVPoint::getU() const
{
	return mU;
}

inline Real UVPoint::getV() const
{
	return mV;
}

inline unsigned __int32 UVPoint::getTriangleID() const
{
	return mTriangleID;
}

///----------------------------------------------------------------------------------------------------
/// Put uv point into stream 
///
/// \param [in,out]	aStreamOut	The stream out. 
/// \param aUVPoint			The uv point. 
///
/// \return	The updated stream. 
///----------------------------------------------------------------------------------------------------
inline std::ostream & operator<<( std::ostream &aStreamOut, const UVPoint & aUVPoint )
{
	aStreamOut.write( reinterpret_cast< const char * >( &aUVPoint.mU ), sizeof( Real ) );
	aStreamOut.write( reinterpret_cast< const char * >( &aUVPoint.mV ), sizeof( Real ) );
	aStreamOut.write( reinterpret_cast< const char * >( &aUVPoint.mTriangleID ), sizeof( unsigned __int32 ) );
	return aStreamOut;
}

///----------------------------------------------------------------------------------------------------
/// Pull uv point from stream.
///
/// \param [in,out]	aStreamIn		The stream in. 
/// \param [in,out]	aUVPoint	The uv point.
///
/// \return	The updated stream. 
///----------------------------------------------------------------------------------------------------
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