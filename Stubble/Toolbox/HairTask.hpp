#ifndef STUBBLE_HAIR_TASK_HPP
#define STUBBLE_HAIR_TASK_HPP

#include "BrushModes/BrushMode.hpp"
#include "../Primitives/Vector3D.hpp"
#include "../HairShape/HairComponents/SegmentsUG.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Structure for passing the brush information to the asynchronous loop performing the actual
/// operation
///----------------------------------------------------------------------------------------------------
struct HairTask
{
	BrushMode *mBrushMode; ///< Brush state containing the brush operation method - points to an effective singleton, no deletion takes place!
	Vector3D< double > mDx; ///< Cursor change projected into the eye coordinates
	HairShape::HairComponents::SegmentsUG *mSelectedGuides; ///< Uniform grid built over selected guides subject to brushing operation
};

} // namespace Toolbox

} // namespace Stubble

#endif