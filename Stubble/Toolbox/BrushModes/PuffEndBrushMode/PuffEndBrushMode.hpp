#ifndef STUBBLE_PUFF_END_BRUSH_MODE_HPP
#define STUBBLE_PUFF_END_BRUSH_MODE_HPP

#include "../BrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// The class implementing the puff end brush transformation, i.e. moving hair vertices onto the hair
/// normal.
///----------------------------------------------------------------------------------------------------
class PuffEndBrushMode :
	public BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// The method which implements the actual brush transformation
	///
	/// \param aTask	Hair task object containing transformation details, see HairTask.hpp
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( HairTask *aTask );
};

} // namespace Toolbox

} // namespace Stubble

#endif