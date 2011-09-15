#include "HairProperties.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

HairProperties::HairProperties():
	mDensityTexture( 0 ),
	mInterpolationGroups( 0 ),
	mInterpolationGroupsTexture( 0 ),
	mNumberOfGuidesToInterpolateFrom( 3 ),
	mGuidesRestPositionsUG( 0 ),
	mGuidesSegments( 0 )
{
}

HairProperties::~HairProperties()
{
	delete mDensityTexture;

	delete mInterpolationGroups;

	delete mInterpolationGroupsTexture;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble