#ifndef STUBBLE_COMMON_FUNCTIONS_HPP
#define STUBBLE_COMMON_FUNCTIONS_HPP

#include "StubbleException.hpp"

#include <stdlib.h>
#include <string>

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
/// Minimum from 3 values. 
///
/// \param	x	The first value.
/// \param	y	The second value. 
/// \param	z	The third value. 
///-------------------------------------------------------------------------------------------------
#define MIN3(x,y,z)  ((y) <= (z) ? ((x) <= (y) ? (x) : (y)) : ((x) <= (z) ? (x) : (z)))

///-------------------------------------------------------------------------------------------------
/// Maximum from 3 values. 
///
/// \param	x	The first value.
/// \param	y	The second value. 
/// \param	z	The third value. 
///-------------------------------------------------------------------------------------------------
#define MAX3(x,y,z)  ((y) >= (z) ? ((x) >= (y) ? (x) : (y)) : ((x) >= (z) ? (x) : (z)))

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
	aRGB[ 0 ] *= valueInverse;
	aRGB[ 1 ] *= valueInverse;
	aRGB[ 2 ] *= valueInverse;
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
		aHSV[ 0 ] = 0.0 + 60.0 * ( rgb_g - rgb_b );
		if ( aHSV[ 0 ] < 0.0 ) 
		{
			aHSV[ 0 ] += 360.0;
		}
	} 
	else 
	{
		if ( rgb_max == rgb_g ) 
		{
			aHSV[ 0 ] = 120.0 + 60.0 * ( rgb_b - rgb_r );
		}
	} 
	else // rgb_max == rgb_b 
	{
		aHSV[ 0 ] = 240.0 + 60.0 * ( rgb_r - rgb_g );
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
	/* TODO */
}


} // namespace Stubble

#endif // STUBBLE_COMMON_FUNCTIONS_HPP