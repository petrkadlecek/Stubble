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
	mCutTexture = new Texture( unzipper );
	mScaleTexture = new Texture( unzipper );
	mRandScaleTexture = new Texture( unzipper );
	mRootThicknessTexture = new Texture( unzipper );
	mTipThicknessTexture = new Texture( unzipper );
	mDisplacementTexture = new Texture( unzipper );
	mRootOpacityTexture = new Texture( unzipper );
	mTipOpacityTexture = new Texture( unzipper );
	mRootColorTexture = new Texture( unzipper );
	mTipColorTexture = new Texture( unzipper );
	mHueVariationTexture = new Texture( unzipper );
	mValueVariationTexture = new Texture( unzipper );
	mMutantHairColorTexture = new Texture( unzipper );
	mPercentMutantHairTexture = new Texture( unzipper );
	mRootFrizzTexture = new Texture( unzipper );
	mTipFrizzTexture = new Texture( unzipper );
	mFrizzXFrequencyTexture = new Texture( unzipper );
	mFrizzYFrequencyTexture = new Texture( unzipper );
	mFrizzZFrequencyTexture = new Texture( unzipper );
	mFrizzAnimTexture = new Texture( unzipper );
	mFrizzAnimSpeedTexture = new Texture( unzipper );
	mTipKinkTexture = new Texture( unzipper );
	mKinkXFrequencyTexture = new Texture( unzipper );
	mKinkYFrequencyTexture = new Texture( unzipper );
	mKinkZFrequencyTexture = new Texture( unzipper );
	mRootSplayTexture = new Texture( unzipper );
	mTipSplayTexture = new Texture( unzipper );
	mCenterSplayTexture = new Texture( unzipper );
	mTwistTexture = new Texture( unzipper );
	mOffsetTexture = new Texture( unzipper );
	mAspectTexture = new Texture( unzipper );
	mRandomizeStrandTexture = new Texture( unzipper );
	// Read segments count
	mInterpolationGroups = new InterpolationGroups( *mInterpolationGroupsTexture, DEFAULT_SEGMENTS_COUNT );
	mInterpolationGroups->importSegmentsCountFromFile( unzipper );
	// Read non-texture hair properties
	unzipper.read( reinterpret_cast< char * >( & mScale ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mRandScale ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mRootThickness ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mTipThickness ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mDisplacement ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mRootOpacity ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mTipOpacity ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( mRootColor ), 3 * sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( mTipColor ), 3 * sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mHueVariation ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mValueVariation ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( mMutantHairColor ), 3 * sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mPercentMutantHair ), sizeof( Real ) );	unzipper.read( reinterpret_cast< char * >( & mRootFrizz ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mTipFrizz ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mFrizzXFrequency ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mFrizzYFrequency ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mFrizzZFrequency ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mFrizzAnim ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mFrizzAnimSpeed ), sizeof( Real ) );		unzipper >> mFrizzAnimDirection;	unzipper.read( reinterpret_cast< char * >( & mRootKink ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mTipKink ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mKinkXFrequency ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mKinkYFrequency ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mKinkZFrequency ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mMultiStrandCount ), sizeof( unsigned __int32 ) );		unzipper.read( reinterpret_cast< char * >( & mRootSplay ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mTipSplay ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mCenterSplay ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mTwist ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mOffset ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mAspect ), sizeof( Real ) );		unzipper.read( reinterpret_cast< char * >( & mRandomizeStrand ), sizeof( Real ) );
	// Read number of guides to interpolate from
	unzipper.read( reinterpret_cast< char * >( &mNumberOfGuidesToInterpolateFrom ), 
		sizeof( unsigned __int32 ) );
	// Read whether the normals should be calculated 
	unzipper.read( reinterpret_cast< char * >( &mAreNormalsCalculated ), 
		sizeof( bool ) );
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