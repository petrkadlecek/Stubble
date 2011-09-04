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

} // namespace Stubble

#endif // STUBBLE_COMMON_FUNCTIONS_HPP