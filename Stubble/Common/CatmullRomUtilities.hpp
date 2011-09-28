#ifndef STUBBLE_CATMULL_ROM_UTILITIES_HPP
#define STUBBLE_CATMULL_ROM_UTILITIES_HPP

#include "Primitives/Vector3D.hpp"

namespace Stubble 
{

///-------------------------------------------------------------------------------------------------
/// Evalulates catmull-rom spline at given t param. Only works for 4 points section of curve.
///
/// \param [in,out]	aOutput	The evaluated point. 
/// \param	aPoints			The curve 4 points. 
/// \param	aTParam			The t parameter. 
///-------------------------------------------------------------------------------------------------
template< typename tType >
inline void catmullRomEval( Vector3D< tType > & aOutput, const Vector3D< tType > * aPoints, tType aTParam )
{
	// Catmull-rom evaluation
	aOutput = ( ( aPoints[ 1 ] * 2 ) + ( ( - aPoints[ 0 ] + aPoints[ 2 ] ) +
			 ( ( aPoints[ 0 ] * 2 - aPoints[ 1 ] * 5 + aPoints[ 2 ] * 4 - aPoints[ 3 ] ) +
			 ( - aPoints[ 0 ] + aPoints[ 1 ] * 3 - aPoints[ 2 ] * 3 + aPoints[ 3 ] ) * aTParam ) 
			 * aTParam ) * aTParam ) * 0.5;
}

} // namespace Stubble

#endif // STUBBLE_CATMULL_ROM_UTILITIES_HPP