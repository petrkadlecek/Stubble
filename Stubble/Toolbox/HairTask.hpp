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
	inline HairTask (const M3dView &aView, const Vector3D< double > &aDx, HairShape::HairShape *aParentHairShape,
		HairShape::HairComponents::SelectedGuides *aAffectedGuides,	BrushMode *aBrushMode);

	M3dView mView; ///< View information for the move vector
	Vector3D< double > mDx; ///< Cursor move vector in the camera coordinates
	HairShape::HairShape *mParentHairShape; ///< The fluffy object the changes will be made upon
	HairShape::HairComponents::SelectedGuides *mAffectedGuides; ///< Selection of guides affected by this operation
	BrushMode *mBrushMode; ///< Brush state containing the brush operation method - points to an effective singleton, no deletion takes place!
};

inline HairTask::HairTask (const M3dView &aView, const Vector3D< double > &aDx, HairShape::HairShape *aParentHairShape,
	HairShape::HairComponents::SelectedGuides *aAffectedGuides,	BrushMode *aBrushMode) :
	mView(mView),
	mDx(aDx),
	mParentHairShape(aParentHairShape),
	mAffectedGuides(aAffectedGuides),
	mBrushMode(aBrushMode)
{
	mView = M3dView::active3dView(); //FIXME: M3dView apparently doesn't have copy constructor, or whatever is happenening
	//empty
}

} // namespace Toolbox

} // namespace Stubble

#endif
