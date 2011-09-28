#include "MayaPositionGenerator.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

void MayaPositionGenerator::preGenerate( UVPointGenerator & aUVPointGenerator, const MayaMesh & aCurrentMesh,
	const Mesh & aRestPoseMesh, unsigned __int32 aCount )
{
	// Clear old memory
	delete [] mGeneratedPositions;
	mGeneratedPositions = 0;
	// Copy new hair count
	mCount = aCount;
	// Allocate memory for generated positions
	mGeneratedPositions = new GeneratedPosition[ mCount ];
	// Set pointer to first position
	mCurrentPosition = mGeneratedPositions;
	// Iterates over all positions
	const GeneratedPosition * endIteration = mGeneratedPositions + mCount;
	aUVPointGenerator.reset();
	for ( GeneratedPosition * it = mGeneratedPositions; it != endIteration; ++it )
	{
		it->mUVPoint = aUVPointGenerator.next(); // Generate uv pos
		// Calculate positions
		it->mCurrentPosition = aCurrentMesh.getMeshPoint( it->mUVPoint );
		it->mRestPosition = aRestPoseMesh.getMeshPoint( it->mUVPoint );
	}
}

void MayaPositionGenerator::recalculateCurrentPositions( const MayaMesh & aCurrentMesh )
{
	// Iterates over all positions
	const GeneratedPosition * endIteration = mGeneratedPositions + mCount;
	for ( GeneratedPosition * it = mGeneratedPositions; it != endIteration; ++it )
	{
		// Calculate new current position
		it->mCurrentPosition = aCurrentMesh.getMeshPoint( it->mUVPoint );
	}
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble