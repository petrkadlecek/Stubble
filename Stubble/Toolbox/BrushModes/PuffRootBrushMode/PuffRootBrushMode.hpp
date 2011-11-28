#ifndef STUBBLE_PUFF_ROOT_BRUSH_MODE_HPP
#define STUBBLE_PUFF_ROOT_BRUSH_MODE_HPP

#include "../BrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// The class implementing the puff root brush transformation, i.e. moving hair vertices onto the hair
/// normal putting the emphasis to vertices closer to the root.
///----------------------------------------------------------------------------------------------------
class PuffRootBrushMode :
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