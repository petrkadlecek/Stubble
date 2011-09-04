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
	inline HairTask ();

	inline static HairTask *create (ToolShape *aToolShape, BrushMode *aBrushMode,
		const Vector3D< double > &aDx, HairShape::HairComponents::SegmentsUG *aSelectedGuides);

	bool mValid; ///< Clean way to indicate valid object and not doing it via null pointer checking
	ToolShape *mToolShape; ///< Tool shape for selecting guides affected by this operation
	BrushMode *mBrushMode; ///< Brush state containing the brush operation method - points to an effective singleton, no deletion takes place!
	Vector3D< double > mDx; ///< Cursor change projected into the eye coordinates
	HairShape::HairComponents::SegmentsUG *mSelectedGuidesUG; ///< Uniform grid built over selected guides subject to brushing operation
};

inline HairTask::HairTask () :
	mValid(false),
	mToolShape(0),
	mBrushMode(0),
	mSelectedGuidesUG(0)
{
	mValid = false;
}

inline HairTask *HairTask::create (ToolShape *aToolShape, BrushMode *aBrushMode,
	const Vector3D< double > &aDx, HairShape::HairComponents::SegmentsUG *aSelectedGuides)
{
	HairTask *task = new HairTask();
	task->mValid = true;
	task->mToolShape = aToolShape;
	task->mBrushMode = aBrushMode;
	task->mDx.set(aDx.x, aDx.y, aDx.z);
	task->mSelectedGuidesUG = aSelectedGuides;

	return task;
}

} // namespace Toolbox

} // namespace Stubble

#endif
