#include "Common\CommonConstants.hpp"

#include "RMHairProperties.hpp"

#include <bzip2stream.hpp>

using namespace std;

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{


RMHairProperties::RMHairProperties( const std::string & aFrameFileName )
{
	std::ifstream file( aFrameFileName.c_str(), std::ios::binary );
	bzip2_stream::bzip2_istream unzipper( file );
	char fileid[20];
	// Read file id
	unzipper.read( fileid, FRAME_FILE_ID_SIZE );
	if ( memcmp( reinterpret_cast< const void * >( fileid ), reinterpret_cast< const void * >( FRAME_FILE_ID ), 
		FRAME_FILE_ID_SIZE ) != 0 )
	{
		throw StubbleException(" RMHairProperties::RMHairProperties : wrong file format ! ");
	}
	mDensityTexture = new Texture( unzipper );
	mInterpolationGroupsTexture = new Texture( unzipper );
	mInterpolationGroups = new InterpolationGroups( *mInterpolationGroupsTexture, DEFAULT_SEGMENTS_COUNT );
	mInterpolationGroups->importSegmentsCountFromFile( unzipper );
	// Read number of guides to interpolate from
	unzipper.read( reinterpret_cast< char * >( &mNumberOfGuidesToInterpolateFrom ), 
		sizeof( unsigned __int32 ) );
	// Read rest positions of guides
	mGuidesRestPositionsUGMutable = new HairComponents::RestPositionsUG();
	mGuidesRestPositionsUG = mGuidesRestPositionsUGMutable;
	mGuidesRestPositionsUGMutable->importFromFile( unzipper, *mInterpolationGroups );
	// Import guides count
	unsigned __int32 size;
	unzipper.read( reinterpret_cast< char * >( &size ), sizeof( unsigned __int32 ) );
	mGuidesSegmentsMutable = new HairComponents::GuidesSegments( size );
	mGuidesSegments = mGuidesSegmentsMutable;
	// For each guide
	for ( HairComponents::GuidesSegments::iterator it = mGuidesSegmentsMutable->begin(); 
		it != mGuidesSegmentsMutable->end(); ++it )
	{
		// Import vertices count
		unzipper.read( reinterpret_cast< char * >( &size ), sizeof( unsigned __int32 ) );
		it->mSegments.resize( size );
		// For each hair vertex
		for ( HairComponents::Segments::iterator segIt = it->mSegments.begin(); segIt != it->mSegments.end(); ++segIt )
		{
			unzipper >> *segIt;
		}
	}
	// TODO : import all other properties
	file.close();
}

RMHairProperties::~RMHairProperties()
{
	delete mGuidesSegmentsMutable;

	delete mGuidesRestPositionsUGMutable;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble