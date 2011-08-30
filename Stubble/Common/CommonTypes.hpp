#ifndef STUBBLE_COMMON_TYPES_HPP
#define STUBBLE_COMMON_TYPES_HPP

#include "CommonFunctions.hpp"

#include <stdint.h>

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
typedef Real Real3[3];
typedef Real Real4[3];

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the dimensions in 3D.
///-------------------------------------------------------------------------------------------------
typedef unsigned __int32 Dimensions3[3];

} // namespace Stubble

#endif // STUBBLE_COMMON_TYPES_HPP