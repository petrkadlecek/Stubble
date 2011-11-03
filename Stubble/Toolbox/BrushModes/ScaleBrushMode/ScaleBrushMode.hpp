#ifndef STUBBLE_SCALE_BRUSH_MODE_HPP
#define STUBBLE_SCALE_BRUSH_MODE_HPP

#include "../BrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// The class implementing the scale brush transformations.
///----------------------------------------------------------------------------------------------------
class ScaleBrushMode :
	public BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// The method which implements the actual brush transformations.
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( HairTask *aTask );

private:
	///----------------------------------------------------------------------------------------------------
	/// Calculates new length of hair segments
	///
	/// \param aMouseX	Mouse movement in the x axis
	/// \param aLength	Current segment length
	/// \return			New segment length
	///----------------------------------------------------------------------------------------------------
	inline double getNewSegmentLength ( Real aMouseX, Real aLength );
};

inline double ScaleBrushMode::getNewSegmentLength ( Real aMouseX, Real aLength )
{
	Real length = aLength + aMouseX;
	return (length < 0.0) ? 0.0 : length;
}

} // namespace Toolbox

} // namespace Stubble

#endif