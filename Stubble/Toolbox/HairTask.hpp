#ifndef STUBBLE_HAIR_TASK_HPP
#define STUBBLE_HAIR_TASK_HPP

#include "../HairShape/UserInterface/HairShape.hpp"
#include "BrushModes/BrushMode.hpp"
#include "../Primitives/Vector3D.hpp"
#include "../HairShape/HairComponents/SegmentsUG.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Structure for passing the brush information to the asynchronous loop performing the actual
/// operation. One should _not_ call delete upon the pointer members as they belong to other class
/// and their disposal is done elsewhere!
///----------------------------------------------------------------------------------------------------
struct HairTask
{
	inline HairTask (HairShape::HairShape *aParentHairShape, ToolShape *aToolShape, BrushMode *aBrushMode, const Vector3D< double > &aDx);

	HairShape::HairShape *mParentHairShape; ///< The fluffy object the changes will be made upon
	ToolShape *mToolShape; ///< Tool shape for selecting guides affected by this operation
	BrushMode *mBrushMode; ///< Brush state containing the brush operation method - points to an effective singleton, no deletion takes place!
	Vector3D< double > mDx; ///< Cursor change projected into the eye coordinates
};

inline HairTask::HairTask (HairShape::HairShape *aParentHairShape, ToolShape *aToolShape, BrushMode *aBrushMode, const Vector3D< double > &aDx) :
	mParentHairShape(aParentHairShape),
	mToolShape(aToolShape),
	mBrushMode(aBrushMode),
	mDx(aDx)
{
	//empty
}

} // namespace Toolbox

} // namespace Stubble

#endif
