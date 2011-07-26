#include "HairGuides.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

HairGuides::HairGuides()
{
}

HairGuides::~HairGuides()
{
}

void HairGuides::applySelection( const SelectionMask & aSelectionMask )
{
}

SelectedGuides & HairGuides::getSelectedGuides() 
{
	/* TO DO */
	SelectedGuides s;
	return s;
}

const RestPositionsUG & HairGuides::getGuidesSegmentsUG()
{
	/* TO DO */
	RestPositionsUG r;
	return r;
}

void HairGuides::updateGuides()
{
}

const SegmentsUG & HairGuides::getGuidesPositionsUG( const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	/* TO DO */
	SegmentsUG s;
	return s;
}

void HairGuides::draw() const
{
}

void HairGuides::importNURBS( void )
{
}

void HairGuides::exportNURBS( void )
{
}

void HairGuides::setCurrentTime( Time aTime )
{
}

void HairGuides::meshUpdate( const MayaMesh & aMayaMesh, bool aTopologyChanged )
{
}

void HairGuides::undo()
{
}

void HairGuides::redo()
{
}

void HairGuides::emptyHistoryStack()
{
}

void HairGuides::generate( UVPointGenerator & aUVPointGenerator, const MayaMesh & aMayaMesh, 
	const Interpolation::InterpolationGroups & aInterpolationGroups, const HairLength::LengthInfo & aLengthInfo, 
	int aCount, bool aInterpolateFromPrevious )
{
}

void HairGuides::updateSegmentsCount( const Interpolation::InterpolationGroups & aInterpolationGroups )
{
}

void HairGuides::updateLength( const HairLength::LengthInfo & aLengthInfo )
{
}

void HairGuides::exportToFile( std::ostream & aOutputStream )
{
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble