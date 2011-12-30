#ifndef STUBBLE_HAIR_TASK_HPP
#define STUBBLE_HAIR_TASK_HPP

#include "../HairShape/UserInterface/HairShape.hpp"
#include "BrushModes/BrushMode.hpp"
#include "../Primitives/Vector3D.hpp"
#include "../HairShape/HairComponents/SegmentsDS.hpp"

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
	///----------------------------------------------------------------------------------------------------
	/// Constructor
	/// 
	/// \param aView Viewport information for the move vector
	/// \param aMouseWorld Mouse position in world coordinates
	/// \param aMouseDir Direction to the scene through the mouse world position
	/// \param aDx Cursor move vector in the camera coordinates
	/// \param aParentHairShape The fluffy object the changes will be made upon
	/// \param aAffectedGuides Selection of guides affected by this operation
	/// \param aBrushMode Brush state containing the brush operation method - points to an effective singleton, no deletion takes place!
	///----------------------------------------------------------------------------------------------------
	inline HairTask (const M3dView &aView, Vector3D< double > &aMouseWorld, Vector3D< double > &aMouseDir,
		const Vector3D< double > &aDx, HairShape::HairShape *aParentHairShape,
		HairShape::HairComponents::SelectedGuides *aAffectedGuides,	BrushMode *aBrushMode);

	M3dView mView; ///< View information for the move vector
	Vector3D< double > mMouseWorld; ///< Mouse position in world coordinates
	Vector3D< double > mMouseDir; ///< Direction to the scene through the mouse world position
	Vector3D< double > mDx; ///< Cursor move vector in the camera coordinates
	HairShape::HairShape *mParentHairShape; ///< The fluffy object the changes will be made upon
	HairShape::HairComponents::SelectedGuides *mAffectedGuides; ///< Selection of guides affected by this operation
	BrushMode *mBrushMode; ///< Brush state containing the brush operation method - points to an effective singleton, no deletion takes place!
};

inline HairTask::HairTask (const M3dView &aView, Vector3D< double > &aMouseWorld, Vector3D< double > &aMouseDir,
	const Vector3D< double > &aDx, HairShape::HairShape *aParentHairShape,
	HairShape::HairComponents::SelectedGuides *aAffectedGuides,	BrushMode *aBrushMode) :
	mMouseWorld(aMouseWorld),
	mMouseDir(aMouseDir),
	mDx(aDx),
	mParentHairShape(aParentHairShape),
	mAffectedGuides(aAffectedGuides),
	mBrushMode(aBrushMode)
{
	mView = aView; // M3dView apparently doesn't have copy constructor, or whatever is happening
}

} // namespace Toolbox

} // namespace Stubble

#endif
