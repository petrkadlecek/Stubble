#include "Voxelization.hpp"

namespace Stubble
{

namespace HairShape
{

namespace RibExport
{

Voxelization::Voxelization( const Mesh & aRestPoseMesh, const UVPointGenerator & aUVPointGenerator, 
	const Dimensions3 & aDimensions3 )
{
}

Voxelization::~Voxelization()
{
}

BoundingBox Voxelization::exportCurrentVoxel( std::ostream & aOutputStream, const MayaMesh & aCurrentMesh, 
	unsigned int aVoxelId ) const
{
	return BoundingBox();
}

void Voxelization::exportRestPoseVoxel( std::ostream & aOutputStream, const Mesh & aRestPoseMesh, 
	unsigned int aVoxelId ) const
{
}

} // namespace RibExport

} // namespace HairShape

} // namespace Stubble