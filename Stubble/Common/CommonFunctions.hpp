#ifndef STUBBLE_COMMON_FUNCTIONS_HPP
#define STUBBLE_COMMON_FUNCTIONS_HPP

#include "StubbleException.hpp"
#include "CommonConstants.hpp"

#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>

namespace Stubble 
{


///----------------------------------------------------------------------------------------------------
/// Clamp the given value. 
///
/// \param	value	The value. 
/// \param	min		The minimum. 
/// \param	max		The maximum. 
///
/// \return	clamped value. 
///----------------------------------------------------------------------------------------------------
template < typename Type >
inline Type clamp( Type value, Type min, Type max )
{
	return value < min ? min : value > max ? max : value;
}

///----------------------------------------------------------------------------------------------------
/// Circle the given value around min, max interval.
///
/// \param	value	The value. 
/// \param	min		The minimum. 
/// \param	max		The maximum. 
///
/// \return	circled value. 
///----------------------------------------------------------------------------------------------------
template < typename Type >
inline Type circleValue( Type value, Type min, Type max )
{
	return value < min ? max - ( min - value ) : value > max ? ( value - max ) + min : value;
}

///-------------------------------------------------------------------------------------------------
/// Gets an environment variable. 
///
/// \param	aVariableName	Name of a variable. 
///
/// \return	The environment variable value. 
///-------------------------------------------------------------------------------------------------

inline std::string getEnvironmentVariable( const char * aVariableName )
{
	char *pValue = 0;
	size_t len;
	errno_t err = _dupenv_s( &pValue, &len, aVariableName );
	if ( err || pValue == 0 )
	{
		free( pValue );
		throw StubbleException( " getEnvironmentVariable : variable was not found " );
	}
	std::string res( pValue );
	free( pValue );
	return res;
}

///-------------------------------------------------------------------------------------------------
/// Copies data from second entry to first, from last but one to last.
///
/// \param [in,out]	aData	Array data. 
/// \param	aCount			Number of entries in aData. 
///-------------------------------------------------------------------------------------------------
template< typename tType, unsigned __int32 tCompounds >
inline void copyToLastAndFirst( tType * aData, unsigned __int32 aCount )
{
  // Second to first
  memcpy( reinterpret_cast< void * >( aData ),
          reinterpret_cast< const void * >( aData + tCompounds ),
          sizeof( tType ) * tCompounds );
  // Last but one to last
  size_t jmp = tCompounds * ( aCount - 2 );
  memcpy( reinterpret_cast< void * >( aData + jmp + tCompounds ),
          reinterpret_cast< const void * >( aData + jmp ),
          sizeof( tType ) * tCompounds ); 
}

///-------------------------------------------------------------------------------------------------
/// Copies data from second entry to first, from last but one to last. Works only for 1 compound 
/// array entries.
///
/// \param [in,out]	aData	Array data. 
/// \param	aCount			Number of entries in aData. 
///-------------------------------------------------------------------------------------------------
template< typename tType >
inline void copyToLastAndFirst( tType * aData, unsigned __int32 aCount )
{
  // Second to first
  *aData = *( aData + 1 );
  // Last but one to last
  *( aData + aCount - 1 ) = *( aData + aCount - 2 ); 
}

///-------------------------------------------------------------------------------------------------
/// Mix 2 colors. 
///
/// \param [in,out]	aResult	The result color. 
/// \param	aColor1			The first color. 
/// \param	aColor2			The second color. 
///-------------------------------------------------------------------------------------------------
template< typename tOutType, typename tInType1, typename tInType2 >
inline void mixColor( tOutType * aResult, const tInType1 & aColor1, const tInType2 & aColor2 )
{
	aResult[ 0 ] = static_cast< tOutType >( aColor1[ 0 ] * aColor2[ 0 ] );
	aResult[ 1 ] = static_cast< tOutType >( aColor1[ 1 ] * aColor2[ 1 ] );
	aResult[ 2 ] = static_cast< tOutType >( aColor1[ 2 ] * aColor2[ 2 ] );
}

///-------------------------------------------------------------------------------------------------
/// Determines the minimum of the given parameters. 
///
/// \param	a	The first value. 
/// \param	b	The second value. 
///-------------------------------------------------------------------------------------------------
#define MIN( a, b ) ( ( a ) < ( b ) ? ( a ) : ( b ) )

///-------------------------------------------------------------------------------------------------
/// Determines the maximum of the given parameters. 
///
/// \param	a	The first value. 
/// \param	b	The second value. 
///-------------------------------------------------------------------------------------------------
#define MAX( a, b ) ( ( a ) > ( b ) ? ( a ) : ( b ) )

///-------------------------------------------------------------------------------------------------
/// Minimum from 3 values. 
///
/// \param	x	The first value.
/// \param	y	The second value. 
/// \param	z	The third value. 
///-------------------------------------------------------------------------------------------------
#define MIN3( x, y, z )  (( y ) <= ( z ) ? (( x ) <= ( y ) ? ( x ) : ( y )) : (( x ) <= ( z ) ? ( x ) : ( z )))

///-------------------------------------------------------------------------------------------------
/// Maximum from 3 values. 
///
/// \param	x	The first value.
/// \param	y	The second value. 
/// \param	z	The third value. 
///-------------------------------------------------------------------------------------------------
#define MAX3( x, y, z )  (( y ) >= ( z ) ? (( x ) >= ( y ) ? ( x ) : ( y )) : (( x ) >= ( z ) ? ( x ) : ( z )))

///-------------------------------------------------------------------------------------------------
/// Converts RGB to HSV
///
/// \param [in,out]	aHSV	Color in HSV format.
/// \param	aRGB			Color in RGB format.
///-------------------------------------------------------------------------------------------------
template< typename tType >
inline void RGBtoHSV( tType * aHSV, const tType * aRGB )
{
	// First find rgb max and min value
	tType rgb_min, rgb_max, rgb_r, rgb_g, rgb_b;
	rgb_r = aRGB[ 0 ];
	rgb_g = aRGB[ 1 ];
	rgb_b = aRGB[ 2 ];
	rgb_min = MIN3( rgb_r, rgb_g, rgb_b );
	rgb_max = MAX3( rgb_r, rgb_g, rgb_b );
	// Compute value
	aHSV[ 2 ] = rgb_max;
	if ( aHSV[ 2 ] == 0 ) 
	{
		aHSV[ 0 ] = aHSV[ 1 ] = 0;
		return;
	}
	// Normalize value to 1
	tType valueInverse = 1 / aHSV[ 2 ];
	rgb_r *= valueInverse;
	rgb_g *= valueInverse;
	rgb_b *= valueInverse;
	rgb_min = MIN3( rgb_r, rgb_g, rgb_b );
	rgb_max = MAX3( rgb_r, rgb_g, rgb_b );
	// Compute saturation
	aHSV[ 1 ] = rgb_max - rgb_min;
	if ( aHSV[ 1 ] == 0 ) 
	{
		aHSV[ 0 ] = 0;
		return;
	}
	// Normalize saturation to 1
	tType sizeInverse = 1 / ( rgb_max - rgb_min );
	rgb_r = ( rgb_r - rgb_min ) * sizeInverse;
	rgb_g = ( rgb_g - rgb_min ) * sizeInverse;
	rgb_b = ( rgb_b - rgb_min ) * sizeInverse;
	rgb_min = MIN3( rgb_r, rgb_g, rgb_b );
	rgb_max = MAX3( rgb_r, rgb_g, rgb_b );
	// Compute hue
	if ( rgb_max == rgb_r ) 
	{
		aHSV[ 0 ] = 0 + 60 * ( rgb_g - rgb_b );
		if ( aHSV[ 0 ] < 0 ) 
		{
			aHSV[ 0 ] += 360;
		}
	} 
	else 
	{
		if ( rgb_max == rgb_g ) 
		{
			aHSV[ 0 ] = 120 + 60 * ( rgb_b - rgb_r );
		}
		else // rgb_max == rgb_b 
		{
			aHSV[ 0 ] = 240 + 60 * ( rgb_r - rgb_g );
		}
	} 
	
}

///-------------------------------------------------------------------------------------------------
/// Converts HSV to RGB
///
/// \param [in,out]	aRGB	Color in RGB format.
/// \param	aHSV			Color in HSV format.
///-------------------------------------------------------------------------------------------------
template< typename tType >
inline void HSVtoRGB( tType * aRGB, const tType * aHSV )
{
	unsigned __int8 hi = static_cast< unsigned __int8 >( std::floor( aHSV[ 0 ] / 60 ) ) % 6;
	tType f = aHSV[ 0 ] / 60 - hi;
	tType p = aHSV[ 2 ] * ( 1 - aHSV[ 1 ] );
	tType q = aHSV[ 2 ] * ( 1 - f * aHSV[ 1 ] );
	tType t = aHSV[ 2 ] * ( 1 - ( 1 - f ) * aHSV[ 1 ] );
	switch( hi )
	{
		case 0 : aRGB[ 0 ] = aHSV[ 2 ]; aRGB[ 1 ] = t; aRGB[ 2 ] = p; break;
		case 1 : aRGB[ 0 ] = q; aRGB[ 1 ] = aHSV[ 2 ]; aRGB[ 2 ] = p; break;
		case 2 : aRGB[ 0 ] = p; aRGB[ 1 ] = aHSV[ 2 ]; aRGB[ 2 ] = t; break;
		case 3 : aRGB[ 0 ] = p; aRGB[ 1 ] = q; aRGB[ 2 ] = aHSV[ 2 ]; break;
		case 4 : aRGB[ 0 ] = t; aRGB[ 1 ] = p; aRGB[ 2 ] = aHSV[ 2 ]; break;
		case 5 : aRGB[ 0 ] = aHSV[ 2 ]; aRGB[ 1 ] = p; aRGB[ 2 ] = q; break;
	}
}

///-------------------------------------------------------------------------------------------------
/// Serialize a primitive type.
///
/// \param	aVar			Variable to be serialized.
/// \param	aOutputStream	Output stream
///-------------------------------------------------------------------------------------------------
template < typename Type >
inline void serialize( Type aVar, std::ostream & aOutputStream )
{
	aOutputStream.write( reinterpret_cast< const char * >( &aVar ), sizeof( Type ) );
}

///-------------------------------------------------------------------------------------------------
/// Serialize a primitive type (string version).
///
/// \param	aVar			Variable to be serialized.
/// \param	aOutputStream	Output stream
///-------------------------------------------------------------------------------------------------
template <>
inline void serialize( std::string aVar, std::ostream & aOutputStream )
{
	unsigned __int32 length = static_cast< unsigned __int32 >( aVar.length() );
	aOutputStream.write( reinterpret_cast< const char * >( &length ), sizeof( unsigned __int32 ) );
	aOutputStream.write( aVar.c_str(), sizeof( char ) * length );
}

///-------------------------------------------------------------------------------------------------
/// Serialize primitive types from an STL vector.
///
/// \param	aVector			Reference to vector that will be serialized.
/// \param	aOutputStream	Output stream
///-------------------------------------------------------------------------------------------------
template < typename Type >
inline void serializePrimitives( const std::vector< Type > &aVector, std::ostream & aOutputStream )
{
	// Save vector size
	unsigned __int32 size = static_cast< unsigned __int32 >( aVector.size() );
	aOutputStream.write( reinterpret_cast< const char * >( &size ), sizeof( unsigned __int32 ) );
	// For every member
	std::vector< Type >::const_iterator it;
	for ( it = aVector.begin(); it != aVector.end(); ++it ) // store the individual elements
	{		
		serialize( *it, aOutputStream );
	}
}

///-------------------------------------------------------------------------------------------------
/// Serialize objects from an STL vector.
///
/// \param	aVector			Reference to vector that will be serialized.
/// \param	aOutputStream	Output stream
///-------------------------------------------------------------------------------------------------
template < typename Type >
inline void serializeObjects( const std::vector< Type > &aVector, std::ostream & aOutputStream )
{
	// Save vector size
	unsigned __int32 size = static_cast< unsigned __int32 >( aVector.size() );
	aOutputStream.write( reinterpret_cast< const char * >( &size ), sizeof( unsigned __int32 ) );
	// For every member
	std::vector< Type >::const_iterator it;
	for ( it = aVector.begin(); it != aVector.end(); ++it ) // store the individual elements
	{		
		it->serialize( aOutputStream );
	}
}

///-------------------------------------------------------------------------------------------------
/// Deserialize a primitive type.
///
/// \param	aVar			Variable to contain deserialized value.
/// \param	aInputStream	Input stream
///-------------------------------------------------------------------------------------------------
template < typename Type >
inline void deserialize( Type & aVar, std::istream & aInputStream )
{
	aInputStream.read( reinterpret_cast< char * >( &aVar ), sizeof( Type ) );
}

///-------------------------------------------------------------------------------------------------
/// Deserialize a primitive type (string reference version).
///
/// \param	aVar			Variable to contain deserialized value.
/// \param	aInputStream	Input stream
///-------------------------------------------------------------------------------------------------
template <>
inline void deserialize( std::string & aVar, std::istream & aInputStream )
{
	// Read string length
	unsigned __int32 length;
	aInputStream.read( reinterpret_cast< char * >( &length ), sizeof( unsigned __int32 ) );
	// Create buffer
	char * buff = new char[ length + 1 ];
	buff[ length ] = '\0';
	// Fill buffer
	aInputStream.read( buff, sizeof( char ) * length );
	// Copy to string
	aVar = buff;
	// Free buffer
	delete [] buff;
}

///-------------------------------------------------------------------------------------------------
/// Deserialize primitive types into an STL vector.
///
/// \param	aVector			Vector to contain deserialized values.
/// \param	aInputStream	Input stream
///-------------------------------------------------------------------------------------------------
template < typename Type >
inline void deserializePrimitives( std::vector< Type > & aVector, std::istream & aInputStream )
{
	// Load vector size
	unsigned __int32 size;
	aInputStream.read( reinterpret_cast< char * >( &size ), sizeof( unsigned __int32 ) );
	// Resize vector
	aVector.resize( size );
	std::vector< Type >::iterator it;
	for ( it = aVector.begin(); it != aVector.end(); ++it ) // store the individual elements
	{	
		deserialize( *it, aInputStream );
	}
}

///-------------------------------------------------------------------------------------------------
/// Deserialize objects into an STL vector.
///
/// \param	aVector			Vector to contain deserialized objects.
/// \param	aInputStream	Input stream
///-------------------------------------------------------------------------------------------------
template < typename Type >
inline void deserializeObjects( std::vector< Type > & aVector, std::istream & aInputStream )
{
	// Load vector size
	unsigned __int32 size;
	aInputStream.read( reinterpret_cast< char * >( &size ), sizeof( unsigned __int32 ) );
	// Resize vector
	aVector.resize( size );
	std::vector< Type >::iterator it;
	for ( it = aVector.begin(); it != aVector.end(); ++it ) // store the individual elements
	{	
		it->deserialize( aInputStream );
	}
}

} // namespace Stubble

#endif // STUBBLE_COMMON_FUNCTIONS_HPP