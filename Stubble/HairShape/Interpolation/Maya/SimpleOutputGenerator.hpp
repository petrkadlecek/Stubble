#ifndef STUBBLE_SIMPLE_OUTPUT_GENERATOR_HPP
#define STUBBLE_SIMPLE_OUTPUT_GENERATOR_HPP

#include "Common/CommonTypes.hpp"
#include "../OutputGenerator.hpp"
#include <ri.h>

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

namespace Maya
{

///-------------------------------------------------------------------------------------------------
/// Simple output generator types.
/// These types are used only for bounding box calculation, they corresponds to RenderMan types.
///-------------------------------------------------------------------------------------------------
struct SimpleTypes
{
	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of 
	/// the 3D position.
	///-------------------------------------------------------------------------------------------------
	typedef RtFloat PositionType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of the color.
	///-------------------------------------------------------------------------------------------------
	typedef RtFloat ColorType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of 
	/// the normal.
	///-------------------------------------------------------------------------------------------------
	typedef RtFloat NormalType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store width.
	///-------------------------------------------------------------------------------------------------
	typedef RtFloat WidthType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of 
	/// the opacity.
	///-------------------------------------------------------------------------------------------------
	typedef RtFloat OpacityType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one of the 2 u v coordinates.
	///-------------------------------------------------------------------------------------------------
	typedef RtFloat UVCoordinateType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store hair and strand index.
	///-------------------------------------------------------------------------------------------------
	typedef unsigned __int32 IndexType;

};

///-------------------------------------------------------------------------------------------------
/// This generator does not generate any hair, it only serves for bounding box calculation.
///-------------------------------------------------------------------------------------------------
class SimpleOutputGenerator : public OutputGenerator< SimpleTypes >, public SimpleTypes
{
	/* EMPTY */
};

} // namespace Maya

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_SIMPLE_OUTPUT_GENERATOR_HPP