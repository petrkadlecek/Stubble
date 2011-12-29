#ifndef STUBBLE_SELECTED_GUIDES_HPP
#define STUBBLE_SELECTED_GUIDES_HPP

#include "Common\CommonTypes.hpp"
#include "HairShape\HairComponents\GuidePosition.hpp"
#include "HairShape\HairComponents\Segments.hpp"
#include "HairShape\Mesh\MeshPoint.hpp"
#include "Toolbox\ToolShapes\ToolShape.hpp"
#include "Toolbox\ToolShapes\CircleToolShape\CircleToolShape.hpp"
#include "Toolbox\ToolShapes\SphereToolShape\SphereToolShape.hpp"
#include "Toolbox\ToolShapes\CylinderToolShape\CylinderToolShape.hpp"

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

///-------------------------------------------------------------------------------------------------
/// Additional information about the segment for tools (brush tool, cut tool etc.) 
/// By segment we mean one hair vertex.
///-------------------------------------------------------------------------------------------------
struct OneSegmentAdditionalInfo
{
	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	inline OneSegmentAdditionalInfo();

	bool mSelected;	///< True if the vertex was selected via a selection tool and so can be selected by brush

	bool mInsideBrush;	///< True if the vertex was selected by brush

	bool mIsColliding;	///< True if the vertex collide with the mesh

	Vector3D< Real > mClosestPointOnMesh;	///< In case of collision the closest point on mesh

	Vector3D< Real > mSurfaceDirection; ///< For collision detection purposes - direction to the surface

	Vector3D< Real > mOriginalPosition; ///< Original endpoint positions prior to any changes

	Real mFallOff;	///< The fall off of brushing transformation = 1 - d, where d is the distance of the vertex from the brush center
};

inline OneSegmentAdditionalInfo::OneSegmentAdditionalInfo() :
	mSelected( false ),
	mInsideBrush( false ),
	mIsColliding( false ),
	mFallOff( 0 )
{
	// empty
}

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the array of the segments additional info .
///-------------------------------------------------------------------------------------------------
typedef std::vector< OneSegmentAdditionalInfo > SegmentsAdditionalInfo;

///-------------------------------------------------------------------------------------------------
/// Structure holding one selected guide. 
/// Stores all data needed for applying tools to this guide (brush tool, cut tool etc.). 
///-------------------------------------------------------------------------------------------------
struct SelectedGuide
{
	GuideCurrentPosition mPosition; ///< The guide's root position on current mesh

	GuideId mGuideId; ///< The guide segments id

	OneGuideSegments mGuideSegments; ///< Segments of selected guide ( represented as vertices )

	SegmentsAdditionalInfo mSegmentsAdditionalInfo; ///< Information describing the segments additional params

	Uint mCollisionsCount; ///< How many guide vertices are colliding?

	Vector3D<Real> mNormal; ///< Normal of guide hair on current mesh

	bool mDirtyRedrawFlag; ///< True if segments has been changed and needs to be redrawn

	bool mDirtyFlag;	///< True if selected guide has been changed in any way and must be stored..
};

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the array of pointers to selected guides.
///-------------------------------------------------------------------------------------------------
typedef std::vector< SelectedGuide * > SelectedGuides;

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_SELECTED_GUIDES_HPP