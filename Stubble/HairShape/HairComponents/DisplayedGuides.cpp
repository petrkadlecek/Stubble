#include "DisplayedGuides.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

DisplayedGuides::DisplayedGuides():
	mDirtyBit( false )
{
}

DisplayedGuides::~DisplayedGuides()
{
}

void DisplayedGuides::build( const GuidesCurrentPositions & aGuidesCurrentPositions, 
	const FrameSegments & aFrameSegments )
{
	mDirtyBit = false;
}

void DisplayedGuides::rebuild( const SelectedGuides & aSelectedGuides, bool aHighlight  )
{
	mDirtyBit = false;
}

void DisplayedGuides::draw() const
{
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble