#include "Voxelization.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

namespace Maya
{

Voxelization::Voxelization( const Mesh & aRestPoseMesh, const Texture & aDensityTexture, 
	const Dimensions3 & aResolution )
{
	BoundingBox bbox = aRestPoseMesh.getBoundingBox();
	unsigned __int32 total = aResolution[ 0 ] * aResolution[ 1 ] * aResolution[ 2 ];
	mVoxels.resize( total );
	Vector3D< Real > bsize = ( bbox.max() - bbox.min() ) * 1.001f; // Resize size to avoid voxel id overflow
	Vector3D< Real > voxelSize( bsize.x / aResolution[ 0 ],
		bsize.y / aResolution[ 1 ],
		bsize.z / aResolution[ 2 ] );
	// For each triangle
	for ( TriangleConstIterator it = aRestPoseMesh.getTriangleConstIterator(); !it.end(); ++it )
	{
		const Triangle & t = it.getTriangle();
		// Calculate barycenter of triangle
		Vector3D< double > barycenter = t.getBarycenter();
		// Choose voxel in which barycenter lies
		unsigned __int32 x = static_cast< unsigned __int32 >( floor( ( barycenter.x - bbox.min().x ) / voxelSize.x ) );
		unsigned __int32 y = static_cast< unsigned __int32 >( floor( ( barycenter.y - bbox.min().y ) / voxelSize.y ) );
		unsigned __int32 z = static_cast< unsigned __int32 >( floor( ( barycenter.z - bbox.min().z ) / voxelSize.z ) );
		// Calculate voxel id
		unsigned __int32 id = z + aResolution[ 2 ] * ( y + aResolution[ 1 ] * x );
		Voxel & v = mVoxels[ id ];
		// Add triangle id to voxel
		v.mTrianglesIds.push_back( it.getTriangleID() );
	}
	// For each voxel
	for ( Voxels::iterator it = mVoxels.begin(); it != mVoxels.end(); ++it )
	{
		// No current mesh, yet
		it->mCurrentMesh = 0;
		if ( it->mTrianglesIds.empty() ) // Empty voxel ?
		{
			it->mRestPoseMesh = 0;
			it->mUVPointGenerator = 0;
			it->mHairCount = 0;
		}
		else
		{
			// Generate rest pose mesh only for this voxel
			Triangles triangles;
			aRestPoseMesh.getRequestedTriangles( it->mTrianglesIds, triangles );
			it->mRestPoseMesh = new Mesh( triangles );
			// Create UV point generator for selected triangles
			it->mUVPointGenerator = new UVPointGenerator( aDensityTexture, it->mRestPoseMesh->getTriangleConstIterator(), it->mRandom );
		}
	}
}

void Voxelization::updateVoxels( const MayaMesh & aCurrentMesh, const Interpolation::HairProperties & aHairProperties,
	unsigned __int32 aTotalHairCount )
{
	Real totalDensity = 0;
	// For each voxel => calculate total density
	for ( Voxels::iterator it = mVoxels.begin(); it != mVoxels.end(); ++it )
	{
		if ( it->mUVPointGenerator != 0 )
		{
			totalDensity += it->mUVPointGenerator->getDensity();
		}
	}
	Real inverseDensity = 1 / totalDensity;
	unsigned __int32 index = 0;
	// Calculate hair count and hair index for each voxel
	for ( Voxels::iterator it = mVoxels.begin(); it != mVoxels.end(); ++it )
	{
		if ( it->mUVPointGenerator != 0 )
		{
			it->mHairCount = static_cast< unsigned __int32 >( it->mUVPointGenerator->getDensity() * inverseDensity *
				aTotalHairCount );
			it->mHairIndex = index;
			index += it->mHairCount; // Increase hair index
		}
	}
	// For each voxel -> calculate bounding box
	#pragma omp parallel for schedule( guided )
	for ( int i = 0; i < static_cast< int >( mVoxels.size() ); ++i )
	{
		Voxel & vx = mVoxels[ i ];
		if ( vx.mHairCount != 0 )
		{
			// Generate current mesh only for this voxel
			Triangles triangles;
			aCurrentMesh.getRequestedTriangles( vx.mTrianglesIds, triangles );
			delete vx.mCurrentMesh;
			vx.mCurrentMesh = new Mesh( triangles, true );
			// Create simple hair position generator & output generator
			SimpleOutputGenerator output;
			SimplePositionGenerator posGenerator( *vx.mRestPoseMesh, *vx.mCurrentMesh,
				*vx.mUVPointGenerator, vx.mHairCount, vx.mHairIndex );
			// Interpolate hair in order to calculate bounding box
			HairGenerator< SimplePositionGenerator, SimpleOutputGenerator > generator( posGenerator, output );
			vx.mBoundingBox.clear();
			vx.mRandom.reset();
			generator.calculateBoundingBox( aHairProperties, 1.0f, vx.mBoundingBox );
		}
	}
}

BoundingBox Voxelization::exportVoxel( std::ostream & aOutputStream, unsigned __int32 aVoxelId )
{
	Voxel & voxel = mVoxels[ aVoxelId ];
	// Export hair index
	aOutputStream.write( reinterpret_cast< const char *>( &voxel.mHairIndex ), sizeof( unsigned __int32 ) );
	// Export hair count
	aOutputStream.write( reinterpret_cast< const char *>( &voxel.mHairCount ), sizeof( unsigned __int32 ) );
	// Export rest pose mesh
	voxel.mRestPoseMesh->exportMesh( aOutputStream );
	// Export current mesh
	voxel.mCurrentMesh->exportMesh( aOutputStream );
	// Finally return bbox
	return voxel.mBoundingBox;
}

} // namespace Maya

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble