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
	// For every rest position
	for ( Positions::iterator posIt = mGuidesRestPositions.begin(); cIt != aGuidesRestPositions.end(); 
		++cIt, ++posIt )
	{
		const Vector3D< Real > & pos = cIt->mPosition.getPosition(); // Select pos
		posIt->mPosition = Vector3D< float >( static_cast< float >( pos.x ), static_cast< float >( pos.y ), 
			static_cast< float >( pos.z )); // Copy to float position
		posIt->mUCoordinate = cIt->mPosition.getUCoordinate(); // Copy U
		posIt->mVCoordinate = cIt->mPosition.getVCoordinate(); // Copy V
	}
	// Builds UG
	innerBuild( aInterpolationGroups );
}

void RestPositionsUG::getNClosestGuides( const Vector3D< Real > & aPosition, unsigned __int32 aInterpolationGroupId,
		unsigned __int32 aN, ClosestGuidesIds & aClosestGuidesIds ) const
{
	// Convert position to float
	Vector3D< float > pos( static_cast< float >( aPosition.x ), static_cast< float >( aPosition.y ), 
			static_cast< float >( aPosition.z ));
	if ( mKdForest[ aInterpolationGroupId ].GetNumPoints() < 1 )
	{
		aClosestGuidesIds.clear();
		return;
	}
	// Init query
	KdTree::CKNNQuery query( aN );
	query.Init( pos , aN, 1e20f );
	// Execute query
    mKdForest[ aInterpolationGroupId ].KNNQuery(query, mKdForest[ aInterpolationGroupId ].truePred);
	// Fill results
	aClosestGuidesIds.resize( query.found );
	for( int i = 0; i < query.found; ++i )
		aClosestGuidesIds[ i ] = IdAndDistance( query.indeces[ i + 1 ], query.dist2[ i + 1 ] );
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
		aOutputStream << it->mPosition;
		aOutputStream.write( reinterpret_cast< const char * >( &it->mUCoordinate ), sizeof( Real ) );
		aOutputStream.write( reinterpret_cast< const char * >( &it->mVCoordinate ), sizeof( Real ) );
	}
}

void RestPositionsUG::importFromFile( std::istream & aInputStream, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	// First import rest positions size
	unsigned __int32 size;
	aInputStream.read( reinterpret_cast< char * >( &size ), sizeof( unsigned __int32 ) );
	// Prepare rest position vector
	mGuidesRestPositions.resize( size );
	// For every rest position
	for ( Positions::iterator it = mGuidesRestPositions.begin(); 
		it != mGuidesRestPositions.end(); ++it )
	{
		aInputStream >> it->mPosition;
		aInputStream.read( reinterpret_cast< char * >( &it->mUCoordinate ), sizeof( Real ) );
		aInputStream.read( reinterpret_cast< char * >( &it->mVCoordinate ), sizeof( Real ) );
	}
	// Builds UG
	innerBuild( aInterpolationGroups );
}

void RestPositionsUG::innerBuild( const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	// Prepare group ids
	GroupIds groupIds( mGuidesRestPositions.size() );
	GroupIds::iterator gIt = groupIds.begin();
	// Prepare group counts
	GroupCounts groupCounts( aInterpolationGroups.getGroupsCount(), 0 );   
	// For every rest position checks group id
	for ( Positions::const_iterator posIt = mGuidesRestPositions.begin(); posIt != mGuidesRestPositions.end(); 
		++gIt, ++posIt )
	{
		// Get interpolation group id
		unsigned __int32 groupId = aInterpolationGroups.getGroupId( posIt->mUCoordinate, posIt->mVCoordinate );
		*gIt = groupId; // Select group id
		++groupCounts[ groupId ]; // Increase count for current group
	}
	// Allocate forest
	delete[] mKdForest;
	mForestSize = static_cast< unsigned __int32 >( groupCounts.size() );
	mKdForest = new KdTree[ mForestSize ];
	// Reserve trees
	for( unsigned __int32 i = 0; i < mForestSize; ++i)
	{
		mKdForest[ i ] = KdTree();
		mKdForest[ i ].Reserve( groupCounts[ i ] );
	}
	// Add items to trees
	gIt = groupIds.begin();
	for( unsigned __int32 i = 0; i < mGuidesRestPositions.size(); ++i, ++gIt )
	{
		mKdForest[ *gIt ].AddItem( &mGuidesRestPositions[ i ].mPosition, i );
	}
	// building all trees
	for( unsigned __int32 i = 0; i < mForestSize; ++i )
	{
		mKdForest[ i ].BuildUp();
	}
	// Build is done
	mDirtyBit = false;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble