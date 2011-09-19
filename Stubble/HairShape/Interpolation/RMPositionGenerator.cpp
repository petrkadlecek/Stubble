#include "Common\CommonConstants.hpp"

#include "RMPositionGenerator.hpp"

#include <fstream>
#include <bzip2stream.hpp>

using namespace std;

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
		bzip2_stream::bzip2_istream unzipper( file );
		char fileid[20];
		// Read file id
		unzipper.read( fileid, VOXEL_FILE_ID_SIZE );
		if ( memcmp( reinterpret_cast< const void * >( fileid ), reinterpret_cast< const void * >( VOXEL_FILE_ID ), 
			VOXEL_FILE_ID_SIZE ) != 0 )
		{
			throw StubbleException(" RMPositionGenerator::RMPositionGenerator : wrong file format ! ");
		}
		// Read hair start index
		unzipper.read( reinterpret_cast< char * >( &mStartIndex ), sizeof( unsigned __int32 ) );
		// Read hair count
		unzipper.read( reinterpret_cast< char * >( &mCount ), sizeof( unsigned __int32 ) );
		// Read rest pose mesh
		mRestPoseMesh = new Mesh( unzipper, false );
		// Read current mesh
		mCurrentMesh = new Mesh( unzipper, false );
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
