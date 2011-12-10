#ifndef STUBBLE_PUFF_END_BRUSH_MODE_HPP
#define STUBBLE_PUFF_END_BRUSH_MODE_HPP

#include "../BrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Class implementing the puff end brush transformation, i.e. moving hair vertices onto the hair
/// normal.
///----------------------------------------------------------------------------------------------------
class PuffEndBrushMode :
	public BrushMode
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Loops through all guide vertices and pushes them onto the local z axis using linear interpolation
	/// of the current and desired position at the z axis.
	///
	/// \param aTask Hair task object containing transformation details, see HairTask.hpp
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( HairTask *aTask );
};

} // namespace Toolbox

} // namespace Stubble

#endif
