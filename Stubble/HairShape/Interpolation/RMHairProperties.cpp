#include "RMHairProperties.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{


RMHairProperties::RMHairProperties( const std::string & aFrameFileName )
{
	std::ifstream file( aFrameFileName.c_str(), std::ios::binary );
	mDensityTexture = new Texture( file );
	mInterpolationGroupsTexture = new Texture( file );
	mInterpolationGroups = new InterpolationGroups( *mInterpolationGroupsTexture, DEFAULT_SEGMENTS_COUNT );
	mInterpolationGroups->importSegmentsCountFromFile( file );
	// Read number of guides to interpolate from
	file.read( reinterpret_cast< char * >( &mNumberOfGuidesToInterpolateFrom ), 
		sizeof( unsigned __int32 ) );
	// Read rest positions of guides
	mGuidesRestPositionsUGMutable = new HairComponents::RestPositionsUG();
	mGuidesRestPositionsUG = mGuidesRestPositionsUGMutable;
	mGuidesRestPositionsUGMutable->importFromFile( file, *mInterpolationGroups );
	// Import guides count
	unsigned __int32 size;
	file.read( reinterpret_cast< char * >( &size ), sizeof( unsigned __int32 ) );
	mGuidesSegmentsMutable = new HairComponents::GuidesSegments( size );
	mGuidesSegments = mGuidesSegmentsMutable;
	// For each guide
	for ( HairComponents::GuidesSegments::iterator it = mGuidesSegmentsMutable->begin(); 
		it != mGuidesSegmentsMutable->end(); ++it )
	{
		// Import vertices count
		file.read( reinterpret_cast< char * >( &size ), sizeof( unsigned __int32 ) );
		it->mSegments.resize( size );
		// For each hair vertex
		for ( HairComponents::Segments::iterator segIt = it->mSegments.begin(); segIt != it->mSegments.end(); ++segIt )
		{
			file >> *segIt;
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