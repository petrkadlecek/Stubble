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
	// Copies only positions to local store
	mGuidesRestPositions.resize( aGuidesRestPositions.size() );
	GuidesRestPositions::const_iterator cIt = aGuidesRestPositions.begin();
	for ( Positions::iterator it = mGuidesRestPositions.begin(); 
		it != mGuidesRestPositions.end(); ++it, ++cIt )
	{
		* it = cIt->mPosition.getPosition();
	}
	// Builds UG
	innerBuild( aInterpolationGroups );
}

void RestPositionsUG::getNClosestGuides( const Vector3D< Real > & aPosition, unsigned __int32 aInterpolationGroupId,
		unsigned __int32 aN, ClosestGuidesIds & aClosestGuidesIds ) const
{
	/* TODO */
}

void RestPositionsUG::exportToFile( std::ostream & aOutputStream ) const
{
	// First export rest positions size
	unsigned __int32 size = static_cast< unsigned __int32 >( mGuidesRestPositions.size() );
	aOutputStream.write( reinterpret_cast< const char * >( &size ), sizeof( unsigned __int32 ) );
	// For every rest position
	for ( Positions::const_iterator it = mGuidesRestPositions.begin(); 
		it != mGuidesRestPositions.end(); ++it )
	{
		aOutputStream << *it;
	}
}

void RestPositionsUG::importFromFile( std::istream & aInputStream, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	// First import rest positions size
	unsigned __int32 size = static_cast< unsigned __int32 >( mGuidesRestPositions.size() );
	aInputStream.read( reinterpret_cast< char * >( &size ), sizeof( unsigned __int32 ) );
	// Prepare rest position vector
	mGuidesRestPositions.resize( size );
	// For every rest position
	for ( Positions::iterator it = mGuidesRestPositions.begin(); 
		it != mGuidesRestPositions.end(); ++it )
	{
		aInputStream >> *it; // Only position is imported
	}
}

void RestPositionsUG::innerBuild( const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	/* TODO */
	mDirtyBit = false;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble