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
	/* TODO */
}

SegmentsUG::~SegmentsUG()
{
	/* TODO */
}

void SegmentsUG::build( const GuidesCurrentPositions & aGuidesCurrentPositions, 
	const FrameSegments & aFrameSegments )
{
	/* TODO */
	mDirtyBit = false;
}

void SegmentsUG::select( const SelectionMask & aSelectionMask, SelectedGuides & aResult ) const
{
	/* TODO */
}

void SegmentsUG::build( const SelectedGuides & aSelectedGuides, bool aFullBuild )
{
	/* TODO */
	mDirtyBit = false;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble