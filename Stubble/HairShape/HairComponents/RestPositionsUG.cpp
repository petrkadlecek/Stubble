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
	/* TODO */
}

RestPositionsUG::~RestPositionsUG()
{
	/* TODO */
}

void RestPositionsUG::build( const GuidesRestPositions & aGuidesRestPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	/* TODO */
	mDirtyBit = false;
}

void RestPositionsUG::getNClosestGuides( const Vector3D< Real > & aPosition, unsigned int aInterpolationGroupId,
		unsigned int aN, ClosestGuidesIds & aClosestGuidesIds ) const
{
	/* TODO */
}

void RestPositionsUG::exportToFile( std::ostream & aOutputStream ) const
{
	/* TODO */
}

void RestPositionsUG::importFromFile( std::istream & aInputStream )
{
	/* TODO */
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble