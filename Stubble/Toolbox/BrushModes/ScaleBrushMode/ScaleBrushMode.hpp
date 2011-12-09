#ifndef STUBBLE_SCALE_BRUSH_MODE_HPP
#define STUBBLE_SCALE_BRUSH_MODE_HPP

#include "../BrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// The class implementing the scale brush transformation, i.e. the brush changes the scale of affected
/// guide hair by increasing/decreasing the length of hair segments.
///----------------------------------------------------------------------------------------------------
class ScaleBrushMode :
	public BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// The method which implements the actual brush transformation
	///
	/// \param aTask	Hair task object containing transformation details, see HairTask.hpp
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( HairTask *aTask );

private:
	///----------------------------------------------------------------------------------------------------
	/// Calculates new length of hair segments
	///
	/// \param aMeasure	Measure of the rotation (typically movement of the mouse along the X axis
	/// \param aLength	Current segment length
	/// \return			New segment length
	///----------------------------------------------------------------------------------------------------
	inline double getNewSegmentLength ( Real aMeasure, Real aLength );
};

inline double ScaleBrushMode::getNewSegmentLength ( Real aMeasure, Real aLength )
{
	Real length = aLength + aMeasure /* aLength*/;
	return (length < 0.0) ? 0.0 : length;
}

} // namespace Toolbox

} // namespace Stubble

#endif