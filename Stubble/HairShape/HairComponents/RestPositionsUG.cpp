#include "RestPositionsUG.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

RestPositionsUG::RestPositionsUG():
	mDirtyBit( true )
{
}

RestPositionsUG::~RestPositionsUG()
{
}

void RestPositionsUG::build( const GuidesRestPositions & aGuidesRestPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	mDirtyBit = false;
}

void RestPositionsUG::getNClosestGuides( const Vector3D< Real > & aPosition, unsigned int aInterpolationGroupId,
		unsigned int aN, ClosestGuidesIds & aClosestGuidesIds ) const
{
}

void RestPositionsUG::exportToFile( std::ostream & aOutputStream ) const
{
}

void RestPositionsUG::importFromFile( std::istream & aInputStream )
{
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble