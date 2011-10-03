#include "Voxelization.hpp"

namespace Stubble
{

namespace HairShape
{

Voxelization::Voxelization( const Mesh & aRestPoseMesh, const UVPointGenerator & aUVPointGenerator, 
	const Dimensions3 & aDimensions3 )
{
	BoundingBox bbox = aRestPoseMesh.getBoundingBox();
	unsigned __int32 total = aDimensions3[ 0 ] * aDimensions3[ 1 ] * aDimensions3[ 2 ];
	mVoxels.resize( total );
	// Resets density
	for ( Voxels::iterator it = mVoxels.begin(); it != mVoxels.end(); ++it )
	{
		it->mDensity = 0;
	}
	Vector3D< Real > bsize = ( bbox.max() - bbox.min() ) * 1.001f;
	Vector3D< Real > voxelSize( bsize.x / aDimensions3[ 0 ],
		bsize.y / aDimensions3[ 1 ],
		bsize.z / aDimensions3[ 2 ] );
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
		Voxel & v = mVoxels[ z + aDimensions3[ 2 ] * ( y + aDimensions3[ 1 ] * x ) ];
		// Add triangle to voxel
		v.mTrianglesIds.push_back( it.getTriangleID() );
		// Increase density
		v.mDensity += aUVPointGenerator.getTriangleDensity( it.getTriangleID() );
	}
}

BoundingBox Voxelization::exportCurrentVoxel( std::ostream & aOutputStream, const MayaMesh & aCurrentMesh, 
	unsigned __int32 aVoxelId ) const
{
	BoundingBox bbox;
	const TrianglesIds & trianglesIds = mVoxels[ aVoxelId ].mTrianglesIds;
	// Export triangles count
	unsigned __int32 size = static_cast< unsigned __int32 >( trianglesIds.size() );
	aOutputStream.write( reinterpret_cast< const char *>( &size ), sizeof( unsigned __int32 ) );
	// Export triangles
	for ( TrianglesIds::const_iterator it = trianglesIds.begin(); it != trianglesIds.end(); ++it )
	{
		const Triangle & t = aCurrentMesh.getTriangle( *it );
		// Expand bounding box
		bbox.expand( t.getVertex1().getPosition() );
		bbox.expand( t.getVertex2().getPosition() );
		bbox.expand( t.getVertex3().getPosition() );
		// Export vertices
		aOutputStream << t.getVertex1();
		aOutputStream << t.getVertex2();
		aOutputStream << t.getVertex3();
	}
	return bbox;
}

void Voxelization::exportRestPoseVoxel( std::ostream & aOutputStream, const Mesh & aRestPoseMesh, 
	unsigned __int32 aVoxelId ) const
{
	const TrianglesIds & trianglesIds = mVoxels[ aVoxelId ].mTrianglesIds;
	// Export triangles count
	unsigned __int32 size = static_cast< unsigned __int32 >( trianglesIds.size() );
	aOutputStream.write( reinterpret_cast< const char *>( &size ), sizeof( unsigned __int32 ) );
	// Export triangles
	for ( TrianglesIds::const_iterator it = trianglesIds.begin(); it != trianglesIds.end(); ++it )
	{
		const Triangle & t = aRestPoseMesh.getTriangle( *it );
		// Export vertices
		aOutputStream << t.getVertex1();
		aOutputStream << t.getVertex2();
		aOutputStream << t.getVertex3();
	}
}

} // namespace HairShape

} // namespace Stubble