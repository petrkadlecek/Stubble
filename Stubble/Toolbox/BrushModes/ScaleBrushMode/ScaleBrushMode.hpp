#ifndef STUBBLE_SCALE_BRUSH_MODE_HPP
#define STUBBLE_SCALE_BRUSH_MODE_HPP

#include "../BrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Class implementing the scale brush transformation, i.e. the brush changes the scale of affected
/// guide hair by increasing/decreasing the length of hair segments.
///----------------------------------------------------------------------------------------------------
class ScaleBrushMode :
	public BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Rescales all supplied guide hair to a new desired length calculated from provided move vector.
	/// Takes into account zero sized hair and prevents division by zero to occur. As a consequence zero
	/// sized hair can be "popped" back.
	///
	/// \param aTask Hair task object containing transformation details, see HairTask.hpp
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( HairTask *aTask );

private:

	///----------------------------------------------------------------------------------------------------
	/// Calculates new length of hair segments
	///
	/// \param aMeasure Measure of the rotation (typically movement of the mouse along the X axis)
	/// \param aLength Current segment length
	///
	/// \return New segment length
	///----------------------------------------------------------------------------------------------------
	inline double getNewSegmentLength ( Real aMeasure, Real aLength );

	static const Real SCALE_SENSITIVITY; ///< To damp too sensitive brush
};

inline double ScaleBrushMode::getNewSegmentLength ( Real aMeasure, Real aLength )
{
	Real length = aLength + aMeasure * (aLength + EPSILON) * ScaleBrushMode::SCALE_SENSITIVITY;
	return (length < 0.0) ? 0.0 : length;
}

} // namespace Toolbox

} // namespace Stubble

#endif
