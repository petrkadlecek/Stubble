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

} // namespace Stubble

#endif // STUBBLE_COMMON_FUNCTIONS_HPP