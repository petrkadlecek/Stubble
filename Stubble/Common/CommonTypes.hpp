#ifndef STUBBLE_COMMON_TYPES_HPP
#define STUBBLE_COMMON_TYPES_HPP

#include "CommonFunctions.hpp"

namespace Stubble 
{

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the real number.
///-------------------------------------------------------------------------------------------------
typedef double Real;

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the time .
///-------------------------------------------------------------------------------------------------
typedef double Time;

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the unknown type = the types to be defined later .
///-------------------------------------------------------------------------------------------------
typedef void * UNKNOWN;

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing fixed length arrays.
///-------------------------------------------------------------------------------------------------
typedef double Real3[3];
typedef double Real4[3];

} // namespace Stubble

#endif // STUBBLE_COMMON_TYPES_HPP