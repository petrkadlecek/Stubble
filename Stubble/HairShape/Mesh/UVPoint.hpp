#ifndef STUBBLE_UV_POINT_HPP
#define STUBBLE_UV_POINT_HPP

#include "Common\CommonTypes.hpp"
#include "Common\CommonConstants.hpp"
#include "Common\CommonFunctions.hpp"

#include <sstream>
#include <string>
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

	static const unsigned __int32 NOT_TRIANGLE = 0xFFFFFFFF;

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

inline std::string UVPoint::serialize() const
{
	std::ostringstream oss;		
	oss << Stubble::serialize< Real >( mU )
		<< Stubble::serialize< Real >( mV )
		<< Stubble::serialize< unsigned __int32 >( mTriangleID );
	return oss.str();
}

inline size_t UVPoint::deserialize( const std::string &aStr, size_t aPos )
{	
	mU = Stubble::deserialize< Real >( aStr, aPos );
	mV = Stubble::deserialize< Real >( aStr, aPos );
	mTriangleID = Stubble::deserialize< unsigned __int32 >( aStr, aPos );
	return aPos;	
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_UV_POINT_HPP