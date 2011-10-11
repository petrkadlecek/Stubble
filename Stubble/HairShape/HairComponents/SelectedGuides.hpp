#ifndef STUBBLE_SELECTED_GUIDES_HPP
#define STUBBLE_SELECTED_GUIDES_HPP

#include "Common\CommonTypes.hpp"
#include "HairShape\HairComponents\GuidePosition.hpp"
#include "HairShape\HairComponents\Segments.hpp"
#include "HairShape\Mesh\MeshPoint.hpp"
#include "Toolbox\ToolShapes\ToolShape.hpp"
#include "Toolbox\ToolShapes\CircleToolShape\CircleToolShape.hpp"

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

typedef Toolbox::ToolShape * SelectionMask;

///-------------------------------------------------------------------------------------------------
/// Additional information about the segment for tools (brush tool, cut tool etc.) 
///-------------------------------------------------------------------------------------------------
struct OneSegmentAdditionalInfo
{
	inline OneSegmentAdditionalInfo();

	bool mSelected;	///< True if the vertex can be selected and is selected via a selection tool

	bool mInsideBrush;	///< True if the vertex is selected and inside a brush

	bool mIsColliding;	///< True if the vertex collide with the mesh

	Vector3D<Real> mClosestPointOnMesh;	///< In case of collision the closest point on mesh

	Real mFallOff;	///< The fall off of brushing transformation
};

inline OneSegmentAdditionalInfo::OneSegmentAdditionalInfo() :
	mSelected(false),
	mInsideBrush(false),
	mIsColliding(false),
	mFallOff(0)
{
	// empty
}

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the segments additional info .
///-------------------------------------------------------------------------------------------------
typedef std::vector< OneSegmentAdditionalInfo > SegmentsAdditionalInfo;

///-------------------------------------------------------------------------------------------------
/// Selected guide. 
///-------------------------------------------------------------------------------------------------
struct SelectedGuide
{
	GuideCurrentPosition mPosition; ///< The guide current position

	GuideId mGuideId; ///< The guide segments id

	OneGuideSegments mGuideSegments; ///< Pointer to segments of selected guide ( represented as vertices )

	SegmentsAdditionalInfo mSegmentsAdditionalInfo; ///< Information describing the segments additional params

	Vector3D<Real> mNormal; ///< Normal of guide

	bool mDirtyRedrawFlag; ///< True if segments has been changed and needs to be redrawn

	bool mDirtyFlag;	///< True if selected guide has been changed in any way and must be stored..
};

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the selected guides .
///-------------------------------------------------------------------------------------------------
typedef std::vector< SelectedGuide * > SelectedGuides;

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_SELECTED_GUIDES_HPP