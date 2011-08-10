#include "DisplayedGuides.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

DisplayedGuides::DisplayedGuides():
	mDirtyBit( true )
{
}

DisplayedGuides::~DisplayedGuides()
{
}

void DisplayedGuides::build( const GuidesCurrentPositions & aGuidesCurrentPositions, 
	const FrameSegments & aFrameSegments, const SelectedGuides & aSelectedGuides )
{
	mDirtyBit = false;
}

void DisplayedGuides::selectionRebuild( const SelectedGuides & aSelectedGuides, bool aHighlight )
{
	mDirtyBit = false;
}

void DisplayedGuides::draw() const
{
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble