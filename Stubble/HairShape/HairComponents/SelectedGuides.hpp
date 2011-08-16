#ifndef STUBBLE_SELECTED_GUIDES_HPP
#define STUBBLE_SELECTED_GUIDES_HPP

#include "Common\CommonTypes.hpp"
#include "HairShape\HairComponents\GuidePosition.hpp"
#include "HairShape\HairComponents\Segments.hpp"
#include "HairShape\Mesh\MeshPoint.hpp"

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

/* TO DO : */
typedef UNKNOWN SelectionMask;

///-------------------------------------------------------------------------------------------------
/// Selected guide. 
///-------------------------------------------------------------------------------------------------
struct SelectedGuide
{
	GuideCurrentPosition mPosition; ///< The guide current position

	GuideId mGuideId; ///< The guide segments id

	Segments & mSegments; ///< Reference to segments of selected guide

	/* TO DO */
	UNKNOWN mSelection; ///< Defines the type of selection and selected points

	bool mDirtyBit;	///< True if segments has been changed
};

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the selected guides .
///-------------------------------------------------------------------------------------------------
typedef std::vector< SelectedGuide > SelectedGuides;

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_SELECTED_GUIDES_HPP