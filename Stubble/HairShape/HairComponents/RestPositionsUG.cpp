#include "RestPositionsUG.hpp"

#include <vector>
#include <assert.h>

using namespace std;

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

RestPositionsUG::RestPositionsUG():
	mDirtyBit( true ),
	mKdForest( 0 ),
	mForestSize( 0 )
{
	/* TODO */
}

RestPositionsUG::~RestPositionsUG()
{
	delete[] mKdForest;
}

void RestPositionsUG::build( const GuidesRestPositions & aGuidesRestPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	// Copies only positions to local store
	mGuidesRestPositions.resize( aGuidesRestPositions.size() );
	GuidesRestPositions::const_iterator cIt = aGuidesRestPositions.begin();

	vector< unsigned int > groupIdentifiers;

	for ( Positions::iterator it = mGuidesRestPositions.begin();  it != mGuidesRestPositions.end(); ++it, ++cIt )
	{
		* it = cIt->mPosition.getPosition();
		unsigned int groupId = aInterpolationGroups.getGroupId( cIt->mUVPoint.getU(), cIt->mUVPoint.getV() );
		groupIdentifiers.push_back( groupId );
	}

	// Builds UG
	innerBuild( aInterpolationGroups, groupIdentifiers );
}

void RestPositionsUG::getNClosestGuides( const Vector3D< Real > & aPosition, unsigned __int32 aInterpolationGroupId,
		unsigned __int32 aN, ClosestGuidesIds & aClosestGuidesIds ) const
{
	/* TODO */
}

vector< const Vector3D< Real > * > RestPositionsUG::getNClosestGuides( const Vector3D< Real > & aPosition, unsigned __int32 aInterpolationGroupId,
		unsigned __int32 aN ) const
{
	assert( aInterpolationGroupId < mForestSize );

	KdTree::CKNNQuery query( aN );

	query.Init( aPosition , aN, 1e20 );
    mKdForest[ aInterpolationGroupId ].KNNQuery(query, mKdForest[ aInterpolationGroupId ].truePred);

	vector< const Vector3D< Real > *> foundPoints;

	for(unsigned int i = 1; i <= query.found; ++i)
		foundPoints.push_back( query.index[ i ] );

	return foundPoints;
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

void RestPositionsUG::innerBuild( const Interpolation::InterpolationGroups & aInterpolationGroups, const vector< unsigned int > &groupIdentifiers )
{
	delete[] mKdForest;
	mKdForest = new KdTree[ aInterpolationGroups.getGroupsCount() ];

	for(unsigned int i = 0; i < aInterpolationGroups.getGroupsCount(); ++i)
		mKdForest[ i ] = KdTree();

	// filling data structure
	assert( mGuidesRestPositions.size() == groupIdentifiers.size() );

	vector< unsigned int >::const_iterator gIt = groupIdentifiers.begin();
	for(Positions::iterator it = mGuidesRestPositions.begin(); it != mGuidesRestPositions.end(); ++it, ++gIt )
		mKdForest[ *gIt ].AddItem( & ( *it ) );

	// building all trees
	for(unsigned int i = 0; i < aInterpolationGroups.getGroupsCount(); ++i)
		mKdForest[ i ].BuildUp();

	mForestSize = aInterpolationGroups.getGroupsCount();
	mDirtyBit = false;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble