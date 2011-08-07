#ifndef STUBBLE_HAIR_TASK_HPP
#define STUBBLE_HAIR_TASK_HPP

#include "BrushModes/BrushMode.hpp"
#include "../Primitives/Vector3D.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Structure for passing the brush information to the asynchronous loop performing the actual
/// operation
///----------------------------------------------------------------------------------------------------
typedef struct HairTask
{
	BrushMode *mBrushMode; ///< Brush state containing the brush operation method - points to an effective singleton, no deletion takes place!
	Vector3D< double > mDx; ///< Cursor change projected into the eye coordinates
	//vector< HairGuide > mAffectedGuides; //TODO
} HairTask;

} // namespace Toolbox

} // namespace Stubble

#endif