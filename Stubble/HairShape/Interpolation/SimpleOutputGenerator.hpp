#ifndef STUBBLE_SIMPLE_OUTPUT_GENERATOR_HPP
#define STUBBLE_SIMPLE_OUTPUT_GENERATOR_HPP

#include "Common/CommonTypes.hpp"
#include "OutputGenerator.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Simple output generator types.
///-------------------------------------------------------------------------------------------------
struct SimpleTypes
{
	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of 
	/// the 3D position.
	///-------------------------------------------------------------------------------------------------
	typedef Real PositionType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of the color.
	///-------------------------------------------------------------------------------------------------
	typedef Real ColorType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of 
	/// the normal.
	///-------------------------------------------------------------------------------------------------
	typedef Real NormalType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store width.
	///-------------------------------------------------------------------------------------------------
	typedef Real WidthType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of 
	/// the opacity.
	///-------------------------------------------------------------------------------------------------
	typedef Real OpacityType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one of the 2 u v coordinates.
	///-------------------------------------------------------------------------------------------------
	typedef Real UVCoordinateType;

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

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_SIMPLE_OUTPUT_GENERATOR_HPP