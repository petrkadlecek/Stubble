#ifndef STUBBLE_SEGMENTS_HPP
#define STUBBLE_SEGMENTS_HPP

#include "Common\CommonTypes.hpp"
#include "Primitives\Vector3D.hpp"

#include <vector>

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the guide segments .
///-------------------------------------------------------------------------------------------------
typedef std::vector< Vector3D< Real > > Segments;

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the segments of all guides .
///-------------------------------------------------------------------------------------------------
typedef std::vector< Segments > GuidesSegments;

///-------------------------------------------------------------------------------------------------
/// Segments of all guides in one time frame
///-------------------------------------------------------------------------------------------------
struct FrameSegments
{
	GuidesSegments mSegments;  ///< The segments of all guides

	Time mFrame; ///< The time frame
};

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing all frames segments .
///-------------------------------------------------------------------------------------------------
typedef std::vector< FrameSegments > AllFramesSegments;

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing identifier for the guide .
///-------------------------------------------------------------------------------------------------
typedef unsigned int GuideId;

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing list of identifiers for the guides .
///-------------------------------------------------------------------------------------------------
typedef std::vector< GuideId > GuidesIds;

///-------------------------------------------------------------------------------------------------
/// Partial storage of all segments (used for undo stack).
///-------------------------------------------------------------------------------------------------
struct PartialStorage
{
	AllFramesSegments mFrames;  ///< The frames segments of guides

	GuidesIds mIds; ///< The identifiers of guides
};

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the segments changes stack.
///-------------------------------------------------------------------------------------------------
typedef std::vector< PartialStorage * > SegmentsChangesStack;

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the segments changes stack pointer .
///-------------------------------------------------------------------------------------------------
typedef SegmentsChangesStack::iterator SegmentsChangesStackPointer;

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_SEGMENTS_HPP