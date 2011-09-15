#include "RMPositionGenerator.hpp"

#include <fstream>

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

RMPositionGenerator::RMPositionGenerator( const Texture & aDensityTexture, const std::string & aVoxelFileName ):
	mRestPoseMesh( 0 ),
	mCurrentMesh( 0 ),
	mUVPointGenerator( 0 )
{
	try {
		std::ifstream file( aVoxelFileName.c_str(), std::ios::binary );
		// Read hair count
		file.read( reinterpret_cast< char * >( &mCount ), sizeof( unsigned __int32 ) );
		// Read rest pose mesh
		mRestPoseMesh = new Mesh( file, true );
		// Read current mesh
		mCurrentMesh = new Mesh( file, false );
		// Copy texture coordinates
		mRestPoseMesh->copyTextureCoordinates( * mCurrentMesh );
		// Create uv point generator
		mUVPointGenerator = new UVPointGenerator( aDensityTexture, mRestPoseMesh->getTriangleConstIterator(), randomGenerator );
		// Close file
		file.close();
	}
	catch( ... )
	{
		delete mRestPoseMesh;
		delete mCurrentMesh;
		delete mUVPointGenerator;
	}
	
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble	
