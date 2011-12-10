#ifndef STUBBLE_PUFF_ROOT_BRUSH_MODE_HPP
#define STUBBLE_PUFF_ROOT_BRUSH_MODE_HPP

#include "../BrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Class implementing the puff root brush transformation, i.e. moving hair vertices onto the hair
/// normal putting the emphasis to vertices closer to the root.
///----------------------------------------------------------------------------------------------------
class PuffRootBrushMode :
	public BrushMode
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Loops through all guide vertices and pushes them onto the guide normal (local z axis). Effect falls
	/// off with distance from the root node.
	///
	/// \param aTask Hair task object containing transformation details, see HairTask.hpp
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( HairTask *aTask );
};

} // namespace Toolbox

} // namespace Stubble

#endif
