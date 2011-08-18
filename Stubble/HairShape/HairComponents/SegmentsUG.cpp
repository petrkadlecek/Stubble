#include "SegmentsUG.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

SegmentsUG::SegmentsUG():
	mDirtyBit( true )
{
}

SegmentsUG::~SegmentsUG()
{
}

void SegmentsUG::build( const GuidesCurrentPositions & aGuidesCurrentPositions, 
	const FrameSegments & aFrameSegments )
{
	mDirtyBit = false;
}

void SegmentsUG::select( const SelectionMask & aSelectionMask, SelectedGuides & aResult ) const
{
}

void SegmentsUG::build( const SelectedGuides & aSelectedGuides, bool aFullBuild )
{
	mDirtyBit = false;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble