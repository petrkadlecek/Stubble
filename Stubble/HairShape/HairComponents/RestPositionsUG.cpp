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
	groupIdentifiers.resize( aGuidesRestPositions.size() );
	vector< unsigned int >::iterator gIt = groupIdentifiers.begin();

	unsigned int *groupCounts = new unsigned int[ aInterpolationGroups.getGroupsCount() ];
	memset( groupCounts, 0, aInterpolationGroups.getGroupsCount() * sizeof( unsigned int ) );

	for ( unsigned int i = 0; i < aGuidesRestPositions.size(); ++i, ++cIt, ++gIt )
	{
		unsigned int groupId = aInterpolationGroups.getGroupId( cIt->mUVPoint.getU(), cIt->mUVPoint.getV() );

		mGuidesRestPositions[ i ] = cIt->mPosition.getPosition();
		groupIdentifiers[ i ] = groupId;
		++groupCounts[ groupId ];
	}

	// Builds UG
	innerBuild( aInterpolationGroups, groupIdentifiers, groupCounts );
}

void RestPositionsUG::getNClosestGuides( const Vector3D< Real > & aPosition, unsigned __int32 aInterpolationGroupId,
		unsigned __int32 aN, ClosestGuidesIds & aClosestGuidesIds ) const
{
	KdTree::CKNNQuery query( aN );

	query.Init( aPosition , aN, 1e20 );
    mKdForest[ aInterpolationGroupId ].KNNQuery(query, mKdForest[ aInterpolationGroupId ].truePred);

	aClosestGuidesIds.reserve( query.found );

	for(unsigned int i = 1; i <= query.found; ++i)
		aClosestGuidesIds[ i ] = IdAndDistance( query.indeces[ i ], sqrt( query.dist2[ i ] ) );
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

void RestPositionsUG::innerBuild( const Interpolation::InterpolationGroups & aInterpolationGroups,
	const vector< unsigned int > &groupIdentifiers, unsigned int *groupCounts )
{
	delete[] mKdForest;
	mKdForest = new KdTree[ aInterpolationGroups.getGroupsCount() ];

	for(unsigned int i = 0; i < aInterpolationGroups.getGroupsCount(); ++i)
	{
		mKdForest[ i ] = KdTree();
		mKdForest[ i ].Reserve( groupCounts[ i ] );
	}

	// filling data structure
	assert( mGuidesRestPositions.size() == groupIdentifiers.size() );

	vector< unsigned int >::const_iterator gIt = groupIdentifiers.begin();
	for(unsigned int i = 0; i < mGuidesRestPositions.size(); ++i, ++gIt )
		mKdForest[ *gIt ].AddItem( &mGuidesRestPositions[ i ], i  );

	// building all trees
	for(unsigned int i = 0; i < aInterpolationGroups.getGroupsCount(); ++i)
		mKdForest[ i ].BuildUp();

	mForestSize = aInterpolationGroups.getGroupsCount();
	mDirtyBit = false;

	delete[] groupCounts;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble