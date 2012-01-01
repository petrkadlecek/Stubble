#include "MayaHairProperties.hpp"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MPxSurfaceShape.h>

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

namespace Maya
{

/* MAYA BASIC PROPERTIES */
MObject MayaHairProperties::densityTextureAttr; ///< The density texture attribute
MObject MayaHairProperties::interpolationGroupsTextureAttr; ///< The interpolation groups texture attribute
MObject MayaHairProperties::cutTextureAttr;  ///< The cut texture attribute
MObject MayaHairProperties::segmentsCountAttr; ///< The segments count attribute
MObject MayaHairProperties::interpolationGroupsSelectableAttr; ///< The interpolation groups selectable attribute
MObject MayaHairProperties::interpolationGroupsColorsAttr;   ///< The interpolation groups colors attribute
MObject MayaHairProperties::numberOfGuidesToInterpolateFromAttr; ///< Number of guides to interpolate from attribute
MObject MayaHairProperties::areNormalsCalculatedAttr;	///< The are normals calculated attribute
MObject MayaHairProperties::scaleTextureAttr;	///< The scale texture attribute
MObject MayaHairProperties::scaleAttr;   ///< The scale attribute
MObject MayaHairProperties::randScaleTextureAttr;	///< The rand scale texture attribute
MObject MayaHairProperties::randScaleAttr;   ///< The rand scale attribute
MObject MayaHairProperties::rootThicknessTextureAttr;	///< The root thickness texture attribute
MObject MayaHairProperties::rootThicknessAttr;   ///< The root thickness attribute
MObject MayaHairProperties::tipThicknessTextureAttr;	///< The tip thickness texture attribute
MObject MayaHairProperties::tipThicknessAttr;   ///< The tip thickness attribute
MObject MayaHairProperties::displacementTextureAttr; ///< The displacement texture attribute
MObject MayaHairProperties::displacementAttr; ///< The displacement attribute
MObject MayaHairProperties::skipThresholdAttr;   ///< The skip threshold attribute
/* MAYA COLOR PROPERTIES */
MObject MayaHairProperties::rootOpacityTextureAttr;  ///< The root opacity texture attribute
MObject MayaHairProperties::rootOpacityAttr;  ///< The root opacity attribute
MObject MayaHairProperties::tipOpacityTextureAttr;  ///< The tip opacity texture attribute
MObject MayaHairProperties::tipOpacityAttr;  ///< The tip opacity attribute
MObject MayaHairProperties::rootColorTextureAttr;  ///< The root color texture attribute
MObject MayaHairProperties::rootColorAttr;  ///< The root colorattribute
MObject MayaHairProperties::tipColorTextureAttr;  ///< The tip color texture attribute
MObject MayaHairProperties::tipColorAttr;  ///< The tip color attribute
MObject MayaHairProperties::hueVariationTextureAttr; ///< The hue variation texture attribute
MObject MayaHairProperties::hueVariationAttr; ///< The hue variation attribute
MObject MayaHairProperties::valueVariationTextureAttr; ///< The value variation texture attribute
MObject MayaHairProperties::valueVariationAttr; ///< The value variation attribute
MObject MayaHairProperties::mutantHairColorTextureAttr;  ///< The mutant hair color texture attribute
MObject MayaHairProperties::mutantHairColorAttr;  ///< The mutant hair color attribute
MObject MayaHairProperties::percentMutantHairTextureAttr;	///< The percent mutant hair texture attribute
MObject MayaHairProperties::percentMutantHairAttr;	///< The percent mutant hair texture attribute
/* MAYA FRIZZ PROPERTIES */
MObject MayaHairProperties::rootFrizzTextureAttr;	///< The root frizz texture attribute
MObject MayaHairProperties::rootFrizzAttr;	///< The root frizz attribute
MObject MayaHairProperties::tipFrizzTextureAttr;	///< The tip frizz texture attribute
MObject MayaHairProperties::tipFrizzAttr;	///< The tip frizz attribute
MObject MayaHairProperties::frizzXFrequencyTextureAttr;  ///< The frizz x coordinate frequency texture attribute
MObject MayaHairProperties::frizzXFrequencyAttr;  ///< The frizz x coordinate frequency attribute
MObject MayaHairProperties::frizzYFrequencyTextureAttr;  ///< The frizz y coordinate frequency texture attribute
MObject MayaHairProperties::frizzYFrequencyAttr;  ///< The frizz y coordinate frequency attribute
MObject MayaHairProperties::frizzZFrequencyTextureAttr;  ///< The frizz z coordinate frequency texture attribute
MObject MayaHairProperties::frizzZFrequencyAttr;  ///< The frizz z coordinate frequency attribute
MObject MayaHairProperties::frizzAnimTextureAttr;	///< The frizz animation texture attribute
MObject MayaHairProperties::frizzAnimAttr;	///< The frizz animation attribute
MObject MayaHairProperties::frizzAnimSpeedTextureAttr;	///< The frizz animation speed texture attribute
MObject MayaHairProperties::frizzAnimSpeedAttr;	///< The frizz animation speed attribute
MObject MayaHairProperties::frizzAnimDirectionAttr;  ///< The frizz animation direction attribute
MObject MayaHairProperties::frizzAnimDirectionXAttr;  ///< The frizz animation direction X attribute
MObject MayaHairProperties::frizzAnimDirectionYAttr;  ///< The frizz animation direction Y attribute
MObject MayaHairProperties::frizzAnimDirectionZAttr;  ///< The frizz animation direction Z attribute
/* MAYA KINK PROPERTIES */
MObject MayaHairProperties::rootKinkTextureAttr;	///< The root kink texture attribute
MObject MayaHairProperties::rootKinkAttr;	///< The root kink attribute
MObject MayaHairProperties::tipKinkTextureAttr;	///< The tip kink texture attribute
MObject MayaHairProperties::tipKinkAttr;	///< The tip kink attribute
MObject MayaHairProperties::kinkXFrequencyTextureAttr;  ///< The kink x coordinate frequency texture attribute
MObject MayaHairProperties::kinkXFrequencyAttr;  ///< The kink x coordinate frequency attribute
MObject MayaHairProperties::kinkYFrequencyTextureAttr;  ///< The kink y coordinate frequency texture attribute
MObject MayaHairProperties::kinkYFrequencyAttr;  ///< The kink y coordinate frequency attribute
MObject MayaHairProperties::kinkZFrequencyTextureAttr;  ///< The kink z coordinate frequency texture attribute
MObject MayaHairProperties::kinkZFrequencyAttr;  ///< The kink z coordinate frequency attribute
/* MAYA MULTI STRAND PROPERTIES */
MObject MayaHairProperties::multiStrandCountAttr;	///< The multi strand count attribute
MObject MayaHairProperties::rootSplayTextureAttr;	///< The root splay texture attribute
MObject MayaHairProperties::rootSplayAttr;	///< The root splayattribute
MObject MayaHairProperties::tipSplayTextureAttr;	///< The tip splay texture attribute
MObject MayaHairProperties::tipSplayAttr;	///< The tip splayattribute
MObject MayaHairProperties::centerSplayTextureAttr;	///< The center splay texture attribute
MObject MayaHairProperties::centerSplayAttr;	///< The center splayattribute
MObject MayaHairProperties::twistTextureAttr;	///< The twist texture attribute
MObject MayaHairProperties::twistAttr;	///< The twist attribute
MObject MayaHairProperties::offsetTextureAttr;	///< The offset texture attribute
MObject MayaHairProperties::offsetAttr;	///< The offset attribute
MObject MayaHairProperties::aspectTextureAttr;	///< The aspect texture attribute
MObject MayaHairProperties::aspectAttr;	///< The aspect attribute
MObject MayaHairProperties::randomizeStrandTextureAttr;	///< The randomizeStrand texture attribute
MObject MayaHairProperties::randomizeStrandAttr;	///< The randomizeStrand attribute
/* TEXTURE DIMENSIONS FOR SAMPLING ATTRIBUTES */
/// The density texture sampling dimesion in U attribute
MObject MayaHairProperties::densityTextureSamplingUDimensionAttr;
/// The density texture sampling dimesion in V attribute
MObject MayaHairProperties::densityTextureSamplingVDimensionAttr;
/// The interpolation groups texture sampling dimesion in U attribute
MObject MayaHairProperties::interpolationGroupsTextureSamplingUDimensionAttr;
/// The interpolation groups texture sampling dimesion in V attribute
MObject MayaHairProperties::interpolationGroupsTextureSamplingVDimensionAttr;
/// The cut texture sampling dimesion in U attribute
MObject MayaHairProperties::cutTextureSamplingUDimensionAttr;
/// The cut texture sampling dimesion in V attribute
MObject MayaHairProperties::cutTextureSamplingVDimensionAttr;
/// The scale texture sampling dimension in U atribute
MObject MayaHairProperties::scaleTextureSamplingUDimensionAttr;
/// The scale texture sampling dimension in V atribute
MObject MayaHairProperties::scaleTextureSamplingVDimensionAttr;
/// The rand scale texture sampling dimension in U atribute
MObject MayaHairProperties::randScaleTextureSamplingUDimensionAttr;
/// The rand scale texture sampling dimension in V atribute
MObject MayaHairProperties::randScaleTextureSamplingVDimensionAttr;
/// The root thickness texture sampling dimension in U atribute
MObject MayaHairProperties::rootThicknessTextureSamplingUDimensionAttr;
/// The root thickness texture sampling dimension in V atribute
MObject MayaHairProperties::rootThicknessTextureSamplingVDimensionAttr;
/// The tip thickness texture sampling dimension in U attribute
MObject MayaHairProperties::tipThicknessTextureSamplingUDimensionAttr;
/// The tip thickness texture sampling dimension in V attribute
MObject MayaHairProperties::tipThicknessTextureSamplingVDimensionAttr;
/// The displacement texture sampling dimension in U atribute
MObject MayaHairProperties::displacementTextureSamplingUDimensionAttr;
/// The displacement texture sampling dimension in V atribute
MObject MayaHairProperties::displacementTextureSamplingVDimensionAttr;
/* MAYA COLOR PROPERTIES */                                                                                                 
/// The root opacity texture sampling dimension in U atribute
MObject MayaHairProperties::rootOpacityTextureSamplingUDimensionAttr;
/// The root opacity texture sampling dimension in V atribute
MObject MayaHairProperties::rootOpacityTextureSamplingVDimensionAttr;
/// The tip opacity texture sampling dimension in U atribute
MObject MayaHairProperties::tipOpacityTextureSamplingUDimensionAttr;
/// The tip opacity texture sampling dimension in V atribute
MObject MayaHairProperties::tipOpacityTextureSamplingVDimensionAttr;
/// The root color texture sampling dimension in U atribute
MObject MayaHairProperties::rootColorTextureSamplingUDimensionAttr;
/// The root color texture sampling dimension in V atribute
MObject MayaHairProperties::rootColorTextureSamplingVDimensionAttr;
/// The tip color texture sampling dimension in U atribute
MObject MayaHairProperties::tipColorTextureSamplingUDimensionAttr;
/// The tip color texture sampling dimension in V atribute
MObject MayaHairProperties::tipColorTextureSamplingVDimensionAttr;
/// The hue variation texture sampling dimension in U atribute
MObject MayaHairProperties::hueVariationTextureSamplingUDimensionAttr;
/// The hue variation texture sampling dimension in V atribute
MObject MayaHairProperties::hueVariationTextureSamplingVDimensionAttr;
/// The value variation texture sampling dimension in U atribute
MObject MayaHairProperties::valueVariationTextureSamplingUDimensionAttr;
/// The value variation texture sampling dimension in V atribute
MObject MayaHairProperties::valueVariationTextureSamplingVDimensionAttr;
/// The mutant hair color texture sampling dimension in U atribute
MObject MayaHairProperties::mutantHairColorTextureSamplingUDimensionAttr;
/// The mutant hair color texture sampling dimension in V atribute
MObject MayaHairProperties::mutantHairColorTextureSamplingVDimensionAttr;
/// The percent mutant hair texture sampling dimension in U atribute
MObject MayaHairProperties::percentMutantHairTextureSamplingUDimensionAttr;
/// The percent mutant hair texture sampling dimension in V atribute
MObject MayaHairProperties::percentMutantHairTextureSamplingVDimensionAttr;
/* MAYA FRIZZ PROPERTIES */
/// The root frizz texture sampling dimension in U atribute
MObject MayaHairProperties::rootFrizzTextureSamplingUDimensionAttr;
/// The root frizz texture sampling dimension in V atribute
MObject MayaHairProperties::rootFrizzTextureSamplingVDimensionAttr;
/// The tip frizz texture sampling dimension in U atribute
MObject MayaHairProperties::tipFrizzTextureSamplingUDimensionAttr;
/// The tip frizz texture sampling dimension in V atribute
MObject MayaHairProperties::tipFrizzTextureSamplingVDimensionAttr;
/// The frizz x coordinate frequency texture sampling dimension in U atribute
MObject MayaHairProperties::frizzXFrequencyTextureSamplingUDimensionAttr;
/// The frizz x coordinate frequency texture sampling dimension in V atribute
MObject MayaHairProperties::frizzXFrequencyTextureSamplingVDimensionAttr;
/// The frizz y coordinate frequency texture sampling dimension in U atribute
MObject MayaHairProperties::frizzYFrequencyTextureSamplingUDimensionAttr;
/// The frizz y coordinate frequency texture sampling dimension in V atribute
MObject MayaHairProperties::frizzYFrequencyTextureSamplingVDimensionAttr;
/// The frizz z coordinate frequency texture sampling dimension in U atribute
MObject MayaHairProperties::frizzZFrequencyTextureSamplingUDimensionAttr;
/// The frizz z coordinate frequency texture sampling dimension in V atribute
MObject MayaHairProperties::frizzZFrequencyTextureSamplingVDimensionAttr;
/// The frizz animation texture sampling dimension in U atribute
MObject MayaHairProperties::frizzAnimTextureSamplingUDimensionAttr;
/// The frizz animation texture sampling dimension in V atribute
MObject MayaHairProperties::frizzAnimTextureSamplingVDimensionAttr;
/// The frizz animation speed texture sampling dimension in U atribute
MObject MayaHairProperties::frizzAnimSpeedTextureSamplingUDimensionAttr;
/// The frizz animation speed texture sampling dimension in V atribute
MObject MayaHairProperties::frizzAnimSpeedTextureSamplingVDimensionAttr;
/* MAYA KINK PROPERTIES */
/// The root kink texture sampling dimension in U atribute
MObject MayaHairProperties::rootKinkTextureSamplingUDimensionAttr;
/// The root kink texture sampling dimension in V atribute
MObject MayaHairProperties::rootKinkTextureSamplingVDimensionAttr;
/// The tip kink texture sampling dimension in U atribute
MObject MayaHairProperties::tipKinkTextureSamplingUDimensionAttr;
/// The tip kink texture sampling dimension in V atribute
MObject MayaHairProperties::tipKinkTextureSamplingVDimensionAttr;
/// The kink x coordinate frequency texture sampling dimension in U atribute
MObject MayaHairProperties::kinkXFrequencyTextureSamplingUDimensionAttr;
/// The kink x coordinate frequency texture sampling dimension in V atribute
MObject MayaHairProperties::kinkXFrequencyTextureSamplingVDimensionAttr;
/// The kink y coordinate frequency texture sampling dimension in U atribute
MObject MayaHairProperties::kinkYFrequencyTextureSamplingUDimensionAttr;
/// The kink y coordinate frequency texture sampling dimension in V atribute
MObject MayaHairProperties::kinkYFrequencyTextureSamplingVDimensionAttr;
/// The kink z coordinate frequency texture sampling dimension in U atribute
MObject MayaHairProperties::kinkZFrequencyTextureSamplingUDimensionAttr;
/// The kink z coordinate frequency texture sampling dimension in V atribute
MObject MayaHairProperties::kinkZFrequencyTextureSamplingVDimensionAttr;
/* MAYA MULTI STRAND PROPERTIES */
/// The root splay texture sampling dimension in U atribute
MObject MayaHairProperties::rootSplayTextureSamplingUDimensionAttr;
/// The root splay texture sampling dimension in V atribute
MObject MayaHairProperties::rootSplayTextureSamplingVDimensionAttr;
/// The tip splay texture sampling dimension in U atribute
MObject MayaHairProperties::tipSplayTextureSamplingUDimensionAttr;
/// The tip splay texture sampling dimension in V atribute
MObject MayaHairProperties::tipSplayTextureSamplingVDimensionAttr;
/// The center splay texture sampling dimension in U atribute
MObject MayaHairProperties::centerSplayTextureSamplingUDimensionAttr;
/// The center splay texture sampling dimension in V atribute
MObject MayaHairProperties::centerSplayTextureSamplingVDimensionAttr;
/// The twist texture sampling dimension in U atribute
MObject MayaHairProperties::twistTextureSamplingUDimensionAttr;
/// The twist texture sampling dimension in V atribute
MObject MayaHairProperties::twistTextureSamplingVDimensionAttr;
/// The offset texture sampling dimension in U atribute
MObject MayaHairProperties::offsetTextureSamplingUDimensionAttr;
/// The offset texture sampling dimension in V atribute
MObject MayaHairProperties::offsetTextureSamplingVDimensionAttr;
/// The aspect texture sampling dimension in U atribute
MObject MayaHairProperties::aspectTextureSamplingUDimensionAttr;
/// The aspect texture sampling dimension in V atribute
MObject MayaHairProperties::aspectTextureSamplingVDimensionAttr;
/// The randomizeStrand texture sampling dimension in U atribute
MObject MayaHairProperties::randomizeStrandTextureSamplingUDimensionAttr;
/// The randomizeStrand texture sampling dimension in V atribute
MObject MayaHairProperties::randomizeStrandTextureSamplingVDimensionAttr;

/* METHODS */

void MayaHairProperties::exportToFile( std::ostream & aOutputStream ) const
{	
	// Export textures
	mDensityTexture->exportToFile( aOutputStream );
	mInterpolationGroupsTexture->exportToFile( aOutputStream );
	mCutTexture->exportToFile( aOutputStream );
	mScaleTexture->exportToFile( aOutputStream );
	mRandScaleTexture->exportToFile( aOutputStream );
	mRootThicknessTexture->exportToFile( aOutputStream );
	mTipThicknessTexture->exportToFile( aOutputStream );
	mDisplacementTexture->exportToFile( aOutputStream );
	mRootOpacityTexture->exportToFile( aOutputStream );
	mTipOpacityTexture->exportToFile( aOutputStream );
	mRootColorTexture->exportToFile( aOutputStream );
	mTipColorTexture->exportToFile( aOutputStream );
	mHueVariationTexture->exportToFile( aOutputStream );
	mValueVariationTexture->exportToFile( aOutputStream );
	mMutantHairColorTexture->exportToFile( aOutputStream );
	mPercentMutantHairTexture->exportToFile( aOutputStream );
	mRootFrizzTexture->exportToFile( aOutputStream );
	mTipFrizzTexture->exportToFile( aOutputStream );
	mFrizzXFrequencyTexture->exportToFile( aOutputStream );
	mFrizzYFrequencyTexture->exportToFile( aOutputStream );
	mFrizzZFrequencyTexture->exportToFile( aOutputStream );
	mFrizzAnimTexture->exportToFile( aOutputStream );
	mFrizzAnimSpeedTexture->exportToFile( aOutputStream );
	mRootKinkTexture->exportToFile( aOutputStream );
	mTipKinkTexture->exportToFile( aOutputStream );
	mKinkXFrequencyTexture->exportToFile( aOutputStream );
	mKinkYFrequencyTexture->exportToFile( aOutputStream );
	mKinkZFrequencyTexture->exportToFile( aOutputStream );
	mRootSplayTexture->exportToFile( aOutputStream );
	mTipSplayTexture->exportToFile( aOutputStream );
	mCenterSplayTexture->exportToFile( aOutputStream );
	mTwistTexture->exportToFile( aOutputStream );
	mOffsetTexture->exportToFile( aOutputStream );
	mAspectTexture->exportToFile( aOutputStream );
	mRandomizeStrandTexture->exportToFile( aOutputStream );
	// Write segments count
	mInterpolationGroups->exportSegmentsCountToFile( aOutputStream );
	// Write non-texture hair properties
	aOutputStream.write( reinterpret_cast< const char * >( & mScale ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mRandScale ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mRootThickness ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mTipThickness ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mDisplacement ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mSkipThreshold ), sizeof( Real ) );
	aOutputStream.write( reinterpret_cast< const char * >( & mRootOpacity ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mTipOpacity ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( mRootColor ), 3 * sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( mTipColor ), 3 * sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mHueVariation ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mValueVariation ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( mMutantHairColor ), 3 * sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mPercentMutantHair ), sizeof( Real ) );
	aOutputStream.write( reinterpret_cast< const char * >( & mRootFrizz ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mTipFrizz ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mFrizzXFrequency ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mFrizzYFrequency ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mFrizzZFrequency ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mFrizzAnim ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mFrizzAnimSpeed ), sizeof( Real ) );	
	aOutputStream << mFrizzAnimDirection;
	aOutputStream.write( reinterpret_cast< const char * >( & mRootKink ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mTipKink ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mKinkXFrequency ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mKinkYFrequency ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mKinkZFrequency ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mMultiStrandCount ), sizeof( unsigned __int32 ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mRootSplay ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mTipSplay ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mCenterSplay ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mTwist ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mOffset ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mAspect ), sizeof( Real ) );	
	aOutputStream.write( reinterpret_cast< const char * >( & mRandomizeStrand ), sizeof( Real ) );
	// Write number of guides to interpolate from
	aOutputStream.write( reinterpret_cast< const char * >( &mNumberOfGuidesToInterpolateFrom ), 
		sizeof( unsigned __int32 ) );
	// Write whether the normals should be calculated 
	aOutputStream.write( reinterpret_cast< const char * >( &mAreNormalsCalculated ), 
		sizeof( bool ) );
	// Write rest positions of guides
	mGuidesRestPositionsDS->exportToFile( aOutputStream );
	// Export guides count
	unsigned __int32 size = static_cast< unsigned __int32 >( mGuidesSegments->size() );
	aOutputStream.write( reinterpret_cast< const char *>( &size ), sizeof( unsigned __int32 ) );
	// For each guide
	for ( HairComponents::GuidesSegments::const_iterator it = mGuidesSegments->begin(); 
		it != mGuidesSegments->end(); ++it )
	{
		// Export vertices count
		size = static_cast< unsigned __int32 >( it->mSegments.size() );
		aOutputStream.write( reinterpret_cast< const char *>( &size ), sizeof( unsigned __int32 ) );
		// For each hair vertex
		for ( HairComponents::Segments::const_iterator segIt = it->mSegments.begin(); segIt != it->mSegments.end(); ++segIt )
		{
			aOutputStream << *segIt;
		}
	}
}

MayaHairProperties::MayaHairProperties():
	/* TEXTURE DIMENSIONS FOR SAMPLING ATTRIBUTES */
	mDensityTextureSamplingUDimension(128),
	mDensityTextureSamplingVDimension(128),
	mInterpolationGroupsTextureSamplingUDimension(128),
	mInterpolationGroupsTextureSamplingVDimension(128),
	mCutTextureSamplingUDimension(128),
	mCutTextureSamplingVDimension(128),
	mScaleTextureSamplingUDimension(128),
	mScaleTextureSamplingVDimension(128),
	mRandScaleTextureSamplingUDimension(128),
	mRandScaleTextureSamplingVDimension(128),
	mRootThicknessTextureSamplingUDimension(128),
	mRootThicknessTextureSamplingVDimension(128),
	mTipThicknessTextureSamplingUDimension(128),
	mTipThicknessTextureSamplingVDimension(128),
	mDisplacementTextureSamplingUDimension(128),
	mDisplacementTextureSamplingVDimension(128),
	/* MAYA COLOR PROPERTIES */
	mRootOpacityTextureSamplingUDimension(128),
	mRootOpacityTextureSamplingVDimension(128),
	mTipOpacityTextureSamplingUDimension(128),
	mTipOpacityTextureSamplingVDimension(128),
	mRootColorTextureSamplingUDimension(128),
	mRootColorTextureSamplingVDimension(128),
	mTipColorTextureSamplingUDimension(128),
	mTipColorTextureSamplingVDimension(128),
	mHueVariationTextureSamplingUDimension(128),
	mHueVariationTextureSamplingVDimension(128),
	mValueVariationTextureSamplingUDimension(128),
	mValueVariationTextureSamplingVDimension(128),
	mMutantHairColorTextureSamplingUDimension(128),
	mMutantHairColorTextureSamplingVDimension(128),
	mPercentMutantHairTextureSamplingUDimension(128),
	mPercentMutantHairTextureSamplingVDimension(128),
	/* MAYA FRIZZ PROPERTIES */
	mRootFrizzTextureSamplingUDimension(128),
	mRootFrizzTextureSamplingVDimension(128),
	mTipFrizzTextureSamplingUDimension(128),
	mTipFrizzTextureSamplingVDimension(128),
	mFrizzXFrequencyTextureSamplingUDimension(128),
	mFrizzXFrequencyTextureSamplingVDimension(128),
	mFrizzYFrequencyTextureSamplingUDimension(128),
	mFrizzYFrequencyTextureSamplingVDimension(128),
	mFrizzZFrequencyTextureSamplingUDimension(128),
	mFrizzZFrequencyTextureSamplingVDimension(128),
	mFrizzAnimTextureSamplingUDimension(128),
	mFrizzAnimTextureSamplingVDimension(128),
	mFrizzAnimSpeedTextureSamplingUDimension(128),
	mFrizzAnimSpeedTextureSamplingVDimension(128),
	/* MAYA KINK PROPERTIES */
	mRootKinkTextureSamplingUDimension(128),
	mRootKinkTextureSamplingVDimension(128),
	mTipKinkTextureSamplingUDimension(128),
	mTipKinkTextureSamplingVDimension(128),
	mKinkXFrequencyTextureSamplingUDimension(128),
	mKinkXFrequencyTextureSamplingVDimension(128),
	mKinkYFrequencyTextureSamplingUDimension(128),
	mKinkYFrequencyTextureSamplingVDimension(128),
	mKinkZFrequencyTextureSamplingUDimension(128),
	mKinkZFrequencyTextureSamplingVDimension(128),
	/* MAYA MULTI STRAND PROPERTIES */
	mRootSplayTextureSamplingUDimension(128),
	mRootSplayTextureSamplingVDimension(128),
	mTipSplayTextureSamplingUDimension(128),
	mTipSplayTextureSamplingVDimension(128),
	mCenterSplayTextureSamplingUDimension(128),
	mCenterSplayTextureSamplingVDimension(128),
	mTwistTextureSamplingUDimension(128),
	mTwistTextureSamplingVDimension(128),
	mOffsetTextureSamplingUDimension(128),
	mOffsetTextureSamplingVDimension(128),
	mAspectTextureSamplingUDimension(128),
	mAspectTextureSamplingVDimension(128),
	mRandomizeStrandTextureSamplingUDimension(128),
	mRandomizeStrandTextureSamplingVDimension(128)
{
	mScaleFactor = 1;
	mInterpolationGroupsTexture = new Texture( 1, 1, 1 );
	mDensityTexture = new Texture( 1 );
	mInterpolationGroups = new Interpolation::InterpolationGroups( *mInterpolationGroupsTexture, DEFAULT_SEGMENTS_COUNT );
	mCutTexture = new Texture( 1 );
	mScaleTexture = new Texture( 1 );
	mRandScaleTexture = new Texture( 1 );
	mRootThicknessTexture = new Texture( 1 );
	mTipThicknessTexture = new Texture( 1 );
	mDisplacementTexture = new Texture( 1 );
	mRootOpacityTexture = new Texture( 1 );
	mTipOpacityTexture = new Texture( 1 );
	mRootColorTexture = new Texture( 1, 1, 1 );
	mTipColorTexture = new Texture( 1, 1, 1 );
	mHueVariationTexture = new Texture( 1 );
	mValueVariationTexture = new Texture( 1 );
	mMutantHairColorTexture = new Texture( 1, 1, 1 );
	mPercentMutantHairTexture = new Texture( 1 );
	mRootFrizzTexture = new Texture( 1 );
	mTipFrizzTexture = new Texture( 1 );
	mFrizzXFrequencyTexture = new Texture( 1 );
	mFrizzYFrequencyTexture = new Texture( 1 );
	mFrizzZFrequencyTexture = new Texture( 1 );
	mFrizzAnimTexture = new Texture( 1 );
	mFrizzAnimSpeedTexture = new Texture( 1 );
	mRootKinkTexture = new Texture( 1 );
	mTipKinkTexture = new Texture( 1 );
	mKinkXFrequencyTexture = new Texture( 1 );
	mKinkYFrequencyTexture = new Texture( 1 );
	mKinkZFrequencyTexture = new Texture( 1 );
	mRootSplayTexture = new Texture( 1 );
	mTipSplayTexture = new Texture( 1 );
	mCenterSplayTexture = new Texture( 1 );
	mTwistTexture = new Texture( 1 );
	mOffsetTexture = new Texture( 1 );
	mAspectTexture = new Texture( 1 );
	mRandomizeStrandTexture = new Texture( 1 );
}

MStatus MayaHairProperties::initializeAttributes()
{
	try
	{
		int int_max = std::numeric_limits< int >::max();
		float float_max = std::numeric_limits< float >::max();
		float float_min = std::numeric_limits< float >::min();

		/* MAYA BASIC PROPERTIES */
		//addIntArrayAttribute( "segments_count", "sgc", segmentsCountAttr, 1, DEFAULT_SEGMENTS_COUNT, 1, 100, 1, 10 );
		addFloatAttribute( "density_texture", "dtxt", densityTextureAttr, 1, 0, 1, 0, 1 );
		addColorAttribute( "interpolation_groups_texture", "itxt", interpolationGroupsTextureAttr, 1, 1, 1 );
		addIntAttribute( "interpolation_samples", "ints", numberOfGuidesToInterpolateFromAttr, 3, 3, 20, 3, 20 );
		addFloatAttribute( "cut_texture", "ctxt", cutTextureAttr, 1, 0, 1, 0, 1 );
		addBoolAttribute( "calculate_normals", "clcn", areNormalsCalculatedAttr, false );
		addFloatAttribute( "scale_texture", "scltxt", scaleTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "scale", "scl", scaleAttr, 1, 0.01f, float_max, 0.01f, 1 );
		addFloatAttribute( "rand_scale_texture", "rscltxt", randScaleTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "random_scale", "rscl", randScaleAttr, 0, 0, 1, 0, 1 );
		addFloatAttribute( "root_thickness_texture", "rthcktxt", rootThicknessTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "root_thickness", "rthck", rootThicknessAttr,  0.1f, 0, float_max, 0, 2 );
		addFloatAttribute( "tip_thickness_texture", "tthcktxt", tipThicknessTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "tip_thickness", "tthck", tipThicknessAttr,  0.1f, 0, float_max, 0, 2 );
		addFloatAttribute( "displacement_texture", "distxt", displacementTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "displacement", "dis", displacementAttr, 0, float_min, float_max, -10, 10 );
		addFloatAttribute( "skip_threshold", "sktr", skipThresholdAttr, 0, 0, 1, 0, 1 );
		/* MAYA COLOR PROPERTIES */
		addFloatAttribute( "root_opacity_texture", "roptxt", rootOpacityTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "root_opacity", "rop", rootOpacityAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "tip_opacity_texture", "toptxt", tipOpacityTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "tip_opacity", "top", tipOpacityAttr, 1, 0, 1, 0, 1 );
		addColorAttribute( "root_color_texture", "rcoltxt", rootColorTextureAttr, 1, 1, 1);
		addColorAttribute( "root_color", "rcol", rootColorAttr, 1, 1, 1);
		addColorAttribute( "tip_color_texture", "tcoltxt", tipColorTextureAttr, 1, 1, 1);
		addColorAttribute( "tip_color", "tcol", tipColorAttr, 1, 1, 1);
		addFloatAttribute( "hue_variation_texture", "huetxt", hueVariationTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "hue_variation", "hue", hueVariationAttr, 0, 0, 180, 0, 180 );
		addFloatAttribute( "value_variation_texture", "valuetxt", valueVariationTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "value_variation", "value", valueVariationAttr, 0, 0, 1, 0, 1 );
		addColorAttribute( "mutant_hair_color_texture", "mcoltxt", mutantHairColorTextureAttr, 1, 1, 1);
		addColorAttribute( "mutant_hair_color", "mcol", mutantHairColorAttr, 1, 1, 1);
		addFloatAttribute( "percent_mutant_hair_texture", "pmhtxt", percentMutantHairTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "percent_mutant_hair", "pmh", percentMutantHairAttr, 0, 0, 100, 0, 100 );
		/* MAYA FRIZZ PROPERTIES */
		addFloatAttribute( "root_frizz_texture", "rfzztxt", rootFrizzTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "root_frizz", "rfzz", rootFrizzAttr, 1, 0, float_max, 0, 10 );
		addFloatAttribute( "tip_frizz_texture", "tfzztxt", tipFrizzTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "tip_frizz", "tfzz", tipFrizzAttr, 1, 0, float_max, 0, 10 );
		addFloatAttribute( "frizz_X_freq_texture", "fzzXftxt", frizzXFrequencyTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "frizz_X_frequency", "fzzXf", frizzXFrequencyAttr, 0.5f, 0.001f, float_max, 0.001f, 2 );
		addFloatAttribute( "frizz_Y_freq_texture", "fzzYftxt", frizzYFrequencyTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "frizz_Y_frequency", "fzzYf", frizzYFrequencyAttr, 0.5f, 0.001f, float_max, 0.001f, 2 );
		addFloatAttribute( "frizz_Z_freq_texture", "fzzZftxt", frizzZFrequencyTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "frizz_Z_frequency", "fzzZf", frizzZFrequencyAttr, 0.5f, 0.001f, float_max, 0.001f, 2 );
		addFloatAttribute( "frizz_anim_texture", "afzztxt", frizzAnimTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "frizz_anim", "afzz", frizzAnimAttr, 0, 0, 1, 0, 1 );
		addFloatAttribute( "frizz_anim_speed_texture", "asfzztxt", frizzAnimSpeedTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "frizz_anim_speed", "asfzz", frizzAnimSpeedAttr, 0, 0, float_max, 0, 0.2f );
		addFloatAttribute( "frizz_anim_direction_X", "adXfzz", frizzAnimDirectionXAttr, 1, -float_min, float_max, 
			-float_min, float_max );
		addFloatAttribute( "frizz_anim_direction_Y", "adYfzz", frizzAnimDirectionYAttr, 1, -float_min, float_max, 
			-float_min, float_max );
		addFloatAttribute( "frizz_anim_direction_Z", "adZfzz", frizzAnimDirectionZAttr, 1, -float_min, float_max, 
			-float_min, float_max );
		addParentAttribute( "frizz_anim_direction", "adfzz", frizzAnimDirectionAttr, frizzAnimDirectionXAttr, 
			frizzAnimDirectionYAttr, frizzAnimDirectionZAttr );
		/* MAYA KINK PROPERTIES */
		addFloatAttribute( "root_kink_texture", "rknktxt", rootKinkTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "root_kink", "rknk", rootKinkAttr, 1, 0, float_max, 0, 10 );
		addFloatAttribute( "tip_kink_texture", "tknktxt", tipKinkTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "tip_kink", "tknk", tipKinkAttr, 1, 0, float_max, 0, 10 );
		addFloatAttribute( "kink_X_freq_texture", "knkXftxt", kinkXFrequencyTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "kink_X_frequency", "knkXf", kinkXFrequencyAttr, 0.5f, 0.001f, float_max, 0.001f, 2 );
		addFloatAttribute( "kink_Y_freq_texture", "knkYftxt", kinkYFrequencyTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "kink_Y_frequency", "knkYf", kinkYFrequencyAttr, 0.5f, 0.001f, float_max, 0.001f, 2 );
		addFloatAttribute( "kink_Z_freq_texture", "knkZftxt", kinkZFrequencyTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "kink_Z_frequency", "knkZf", kinkZFrequencyAttr, 0.5f, 0.001f, float_max, 0.001f, 2 );
		/* MAYA MULTI STRAND PROPERTIES */
		addIntAttribute( "multi_strand_count", "msc", multiStrandCountAttr, 0, 0, int_max, 0, 10 );
		addFloatAttribute( "root_splay_texture", "rspltxt", rootSplayTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "root_splay", "rspl", rootSplayAttr, 0, 0, float_max, 0, 5 );
		addFloatAttribute( "tip_splay_texture", "tspltxt", tipSplayTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "tip_splay", "tspl", tipSplayAttr, 0, 0, float_max, 0, 5 );
		addFloatAttribute( "center_splay_texture", "cspltxt", centerSplayTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "center_splay", "cspl", centerSplayAttr, 0, 0, float_max, 0, 5 );
		addFloatAttribute( "twist_texture", "twtxt", twistTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "twist", "twst", twistAttr, 0, 0, float_max, 0, 5 );
		addFloatAttribute( "offset_texture", "offtxt", offsetTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "offset", "off", offsetAttr, 0, 0, float_max, 0, 5 );
		addFloatAttribute( "aspect_texture", "asptxt", aspectTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "aspect", "asp", aspectAttr, 1, 0, float_max, 0, 5 );
		addFloatAttribute( "randomize_strand_texture", "rstrtxt", randomizeStrandTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "randomize_strand", "rstr", randomizeStrandAttr, 0, 0, 1, 0, 1 );
		/* TEXTURE DIMENSIONS FOR SAMPLING ATTRIBUTES */
		addIntAttribute( "density_texture_sampling_u_dimension", "dtxtsmpludm",
			densityTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "density_texture_sampling_v_dimension", "dtxtsmplvdm",
			densityTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "interpolation_groups_texture_sampling_u_dimension", "itxtsmpludm",
			interpolationGroupsTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "interpolation_groups_texture_sampling_v_dimension", "itxtsmplvdm",
			interpolationGroupsTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "cut_texture_sampling_u_dimension", "ctxtsmpludm",
			cutTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "cut_texture_sampling_v_dimension", "ctxtsmplvdm",
			cutTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "scale_texture_sampling_u_dimension", "scltxtsmpludm",
			scaleTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "scale_texture_sampling_v_dimension", "scltxtsmplvdm",
			scaleTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "rand_scale_texture_sampling_u_dimension", "rscltxtsmpludm",
			randScaleTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "rand_scale_texture_sampling_v_dimension", "rscltxtsmplvdm",
			randScaleTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "root_thickness_texture_sampling_u_dimension", "rthcktxtsmpludm",
			rootThicknessTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "root_thickness_texture_sampling_v_dimension", "rthcktxtsmplvdm",
			rootThicknessTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "tip_thickness_texture_sampling_u_dimension", "tthcktxtsmpludm",
			tipThicknessTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "tip_thickness_texture_sampling_v_dimension", "tthcktxtsmplvdm",
			tipThicknessTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "displacement_texture_sampling_u_dimension", "distxtsmpludm",
			displacementTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "displacement_texture_sampling_v_dimension", "distxtsmplvdm",
			displacementTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		/* MAYA COLOR PROPERTIES */                                                                                                 	;
		addIntAttribute( "root_opacity_texture_sampling_u_dimension", "roptxtsmpludm",
			rootOpacityTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "root_opacity_texture_sampling_v_dimension", "roptxtsmplvdm",
			rootOpacityTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "tip_opacity_texture_sampling_u_dimension", "toptxtsmpludm",
			tipOpacityTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "tip_opacity_texture_sampling_v_dimension", "toptxtsmplvdm",
			tipOpacityTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "root_color_texture_sampling_u_dimension", "rcoltxtsmpludm",
			rootColorTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "root_color_texture_sampling_v_dimension", "rcoltxtsmplvdm",
			rootColorTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "tip_color_texture_sampling_u_dimension", "tcoltxtsmpludm",
			tipColorTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "tip_color_texture_sampling_v_dimension", "tcoltxtsmplvdm",
			tipColorTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "hue_variation_texture_sampling_u_dimension", "huetxtsmpludm",
			hueVariationTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "hue_variation_texture_sampling_v_dimension", "huetxtsmplvdm",
			hueVariationTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "value_variation_texture_sampling_u_dimension", "valuetxtsmpludm",
			valueVariationTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "value_variation_texture_sampling_v_dimension", "valuetxtsmplvdm",
			valueVariationTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "mutant_hair_color_texture_sampling_u_dimension", "mcoltxtsmpludm",
			mutantHairColorTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "mutant_hair_color_texture_sampling_v_dimension", "mcoltxtsmplvdm",
			mutantHairColorTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "percent_mutant_hair_texture_sampling_u_dimension", "pmhtxtsmpludm",
			percentMutantHairTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "percent_mutant_hair_texture_sampling_v_dimension", "pmhtxtsmplvdm",
			percentMutantHairTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		/* MAYA FRIZZ PROPERTIES */	;
		addIntAttribute( "root_frizz_texture_sampling_u_dimension", "rfzztxtsmpludm",
			rootFrizzTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "root_frizz_texture_sampling_v_dimension", "rfzztxtsmplvdm",
			rootFrizzTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "tip_frizz_texture_sampling_u_dimension", "tfzztxtsmpludm",
			tipFrizzTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "tip_frizz_texture_sampling_v_dimension", "tfzztxtsmplvdm",
			tipFrizzTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "frizz_X_freq_texture_sampling_u_dimension", "fzzXftxtsmpludm",
			frizzXFrequencyTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "frizz_X_freq_texture_sampling_v_dimension", "fzzXftxtsmplvdm",
			frizzXFrequencyTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "frizz_Y_freq_texture_sampling_u_dimension", "fzzYftxtsmpludm",
			frizzYFrequencyTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "frizz_Y_freq_texture_sampling_v_dimension", "fzzYftxtsmplvdm",
			frizzYFrequencyTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "frizz_Z_freq_texture_sampling_u_dimension", "fzzZftxtsmpludm",
			frizzZFrequencyTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "frizz_Z_freq_texture_sampling_v_dimension", "fzzZftxtsmplvdm",
			frizzZFrequencyTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "frizz_anim_texture_sampling_u_dimension", "afzztxtsmpludm",
			frizzAnimTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "frizz_anim_texture_sampling_v_dimension", "afzztxtsmplvdm",
			frizzAnimTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "frizz_anim_speed_texture_sampling_u_dimension", "asfzztxtsmpludm",
			frizzAnimSpeedTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "frizz_anim_speed_texture_sampling_v_dimension", "asfzztxtsmplvdm",
			frizzAnimSpeedTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		/* MAYA KINK PROPERTIES */	;
		addIntAttribute( "root_kink_texture_sampling_u_dimension", "rknktxtsmpludm",
			rootKinkTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "root_kink_texture_sampling_v_dimension", "rknktxtsmplvdm",
			rootKinkTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "tip_kink_texture_sampling_u_dimension", "tknktxtsmpludm",
			tipKinkTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "tip_kink_texture_sampling_v_dimension", "tknktxtsmplvdm",
			tipKinkTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "kink_X_freq_texture_sampling_u_dimension", "knkXftxtsmpludm",
			kinkXFrequencyTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "kink_X_freq_texture_sampling_v_dimension", "knkXftxtsmplvdm",
			kinkXFrequencyTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "kink_Y_freq_texture_sampling_u_dimension", "knkYftxtsmpludm",
			kinkYFrequencyTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "kink_Y_freq_texture_sampling_v_dimension", "knkYftxtsmplvdm",
			kinkYFrequencyTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "kink_Z_freq_texture_sampling_u_dimension", "knkZftxtsmpludm",
			kinkZFrequencyTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "kink_Z_freq_texture_sampling_v_dimension", "knkZftxtsmplvdm",
			kinkZFrequencyTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		/* MAYA MULTI STRAND PROPERTIES */	;
		addIntAttribute( "root_splay_texture_sampling_u_dimension", "rspltxtsmpludm",
			rootSplayTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "root_splay_texture_sampling_v_dimension", "rspltxtsmplvdm",
			rootSplayTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "tip_splay_texture_sampling_u_dimension", "tspltxtsmpludm",
			tipSplayTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "tip_splay_texture_sampling_v_dimension", "tspltxtsmplvdm",
			tipSplayTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "center_splay_texture_sampling_u_dimension", "cspltxtsmpludm",
			centerSplayTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "center_splay_texture_sampling_v_dimension", "cspltxtsmplvdm",
			centerSplayTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "twist_texture_sampling_u_dimension", "twtxtsmpludm",
			twistTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "twist_texture_sampling_v_dimension", "twtxtsmplvdm",
			twistTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "offset_texture_sampling_u_dimension", "offtxtsmpludm",
			offsetTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "offset_texture_sampling_v_dimension", "offtxtsmplvdm",
			offsetTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "aspect_texture_sampling_u_dimension", "asptxtsmpludm",
			aspectTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "aspect_texture_sampling_v_dimension", "asptxtsmplvdm",
			aspectTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "randomize_strand_texture_sampling_u_dimension", "rstrtxtsmpludm",
			randomizeStrandTextureSamplingUDimensionAttr, 128, 1, 4096, 32, 1024);
		addIntAttribute( "randomize_strand_texture_sampling_v_dimension", "rstrtxtsmplvdm",
			randomizeStrandTextureSamplingVDimensionAttr, 128, 1, 4096, 32, 1024);
	}
	catch( const StubbleException & ex )
	{
		MStatus s;
		s.perror( ex.what() );
		return s;
	}
	return MStatus::kSuccess;
}

bool MayaHairProperties::setAttributesValues( const MPlug& aPlug, const MDataHandle& aDataHandle,
		bool & aSegmentsCountChanged, bool & aInterpolationGroupsSelectableChanged, bool & aHairPropertiesChanged,
		bool & aTextureResolutionHasChanged )
{
	const MPlug &root = aPlug.isChild() ? aPlug.parent() : aPlug;  // root
	aSegmentsCountChanged = aInterpolationGroupsSelectableChanged = aHairPropertiesChanged =
		aTextureResolutionHasChanged = false;
	if ( root == mSegmentsCountAttr )
	{
		// For all interpolation groups
		for( unsigned __int32 i = 0; i < root.numChildren(); ++i )
		{
			if ( root.child( i ) == aPlug )
			{
				// Refresh segments count
				mInterpolationGroups->setGroupSegmentsCount( i, static_cast< unsigned __int32 >( aDataHandle.asInt() ) );
			}
		}
		aSegmentsCountChanged = true;
		return false;
	}
	if ( root == mInterpolationGroupsSelectableAttr )
	{
		// For all interpolation groups
		for( unsigned __int32 i = 0; i < root.numChildren(); ++i )
		{
			if ( root.child( i ) == aPlug )
			{
				// Refresh segments count
				mInterpolationGroupsSelectable[i] = static_cast< unsigned __int32 >( aDataHandle.asInt() );
			}
		}
		aInterpolationGroupsSelectableChanged = true;
		return false;
	}
	if ( aPlug == densityTextureAttr )
	{
		mDensityTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == interpolationGroupsTextureAttr || root == interpolationGroupsTextureAttr )
	{
		mInterpolationGroupsTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == numberOfGuidesToInterpolateFromAttr )
	{
		mNumberOfGuidesToInterpolateFrom = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == areNormalsCalculatedAttr )
	{
		mAreNormalsCalculated = aDataHandle.asBool();
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == aspectAttr )
	{
		mAspect = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == aspectTextureAttr )
	{
		mAspectTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == centerSplayAttr )
	{
		mCenterSplay = static_cast< Real >( aDataHandle.asFloat() ) * mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == centerSplayTextureAttr )
	{
		mCenterSplayTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == cutTextureAttr )
	{
		mCutTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == displacementAttr )
	{
		mDisplacement = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == skipThresholdAttr )
	{
		// map from [0,1] -> [-1,1] and swap
		mSkipThreshold = 2 * ( static_cast< Real >( 0.5f - aDataHandle.asFloat() ) );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == displacementTextureAttr )
	{
		mDisplacementTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == frizzAnimAttr )
	{
		mFrizzAnim = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == frizzAnimDirectionAttr )
	{
		mFrizzAnimDirection.x = static_cast< Real >( aDataHandle.asFloat3()[ 0 ] );
		mFrizzAnimDirection.y = static_cast< Real >( aDataHandle.asFloat3()[ 1 ] );
		mFrizzAnimDirection.z = static_cast< Real >( aDataHandle.asFloat3()[ 2 ] );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == frizzAnimDirectionXAttr )
	{
		mFrizzAnimDirection.x = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == frizzAnimDirectionYAttr )
	{
		mFrizzAnimDirection.y = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == frizzAnimDirectionZAttr )
	{
		mFrizzAnimDirection.z = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == frizzAnimSpeedAttr )
	{
		mFrizzAnimSpeed = static_cast< Real >( aDataHandle.asFloat() ) * mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == frizzAnimSpeedTextureAttr )
	{
		mFrizzAnimSpeedTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == frizzAnimTextureAttr )
	{
		mFrizzAnimTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == frizzXFrequencyAttr )
	{
		mFrizzXFrequency = static_cast< Real >( aDataHandle.asFloat() ) / mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == frizzXFrequencyTextureAttr )
	{
		mFrizzXFrequencyTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == frizzYFrequencyAttr )
	{
		mFrizzYFrequency = static_cast< Real >( aDataHandle.asFloat() ) / mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == frizzYFrequencyTextureAttr )
	{
		mFrizzYFrequencyTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == frizzZFrequencyAttr )
	{
		mFrizzZFrequency = static_cast< Real >( aDataHandle.asFloat() ) / mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == frizzZFrequencyTextureAttr )
	{
		mFrizzZFrequencyTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == hueVariationAttr )
	{
		mHueVariation = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == hueVariationTextureAttr )
	{
		mHueVariationTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == kinkXFrequencyAttr )
	{
		mKinkXFrequency = static_cast< Real >( aDataHandle.asFloat() ) / mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == kinkXFrequencyTextureAttr )
	{
		mKinkXFrequencyTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == kinkYFrequencyAttr )
	{
		mKinkYFrequency = static_cast< Real >( aDataHandle.asFloat() ) / mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == kinkYFrequencyTextureAttr )
	{
		mKinkYFrequencyTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == kinkZFrequencyAttr )
	{
		mKinkZFrequency = static_cast< Real >( aDataHandle.asFloat() ) / mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == kinkZFrequencyTextureAttr )
	{
		mKinkZFrequencyTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == multiStrandCountAttr )
	{
		mMultiStrandCount = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == mutantHairColorAttr )
	{
		mMutantHairColor[ 0 ] = static_cast< Real >( aDataHandle.asFloat3()[ 0 ] );
		mMutantHairColor[ 1 ] = static_cast< Real >( aDataHandle.asFloat3()[ 1 ] );
		mMutantHairColor[ 2 ] = static_cast< Real >( aDataHandle.asFloat3()[ 2 ] );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == mutantHairColorTextureAttr || root == mutantHairColorTextureAttr )
	{
		mMutantHairColorTexture->setConnection( root );
		return true;
	}
	if ( aPlug == offsetAttr )
	{
		mOffset = static_cast< Real >( aDataHandle.asFloat() ) * mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == offsetTextureAttr )
	{
		mOffsetTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == percentMutantHairAttr )
	{
		mPercentMutantHair = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == percentMutantHairTextureAttr )
	{
		mPercentMutantHairTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == randomizeStrandAttr )
	{
		mRandomizeStrand = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == randomizeStrandTextureAttr )
	{
		mRandomizeStrandTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == randScaleAttr )
	{
		mRandScale = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == randScaleTextureAttr )
	{
		mRandScaleTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == rootColorAttr )
	{
		mRootColor[ 0 ] = static_cast< Real >( aDataHandle.asFloat3()[ 0 ] );
		mRootColor[ 1 ] = static_cast< Real >( aDataHandle.asFloat3()[ 1 ] );
		mRootColor[ 2 ] = static_cast< Real >( aDataHandle.asFloat3()[ 2 ] );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == rootColorTextureAttr || root == rootColorTextureAttr )
	{
		mRootColorTexture->setConnection( root );
		return true;
	}
	if ( aPlug == rootFrizzAttr )
	{
		mRootFrizz = static_cast< Real >( aDataHandle.asFloat() ) * mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == rootFrizzTextureAttr )
	{
		mRootFrizzTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == rootKinkAttr )
	{
		mRootKink = static_cast< Real >( aDataHandle.asFloat() ) * mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == rootKinkTextureAttr )
	{
		mRootKinkTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == rootOpacityAttr )
	{
		mRootOpacity = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == rootOpacityTextureAttr )
	{
		mRootOpacityTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == rootSplayAttr )
	{
		mRootSplay = static_cast< Real >( aDataHandle.asFloat() ) * mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == rootSplayTextureAttr )
	{
		mRootSplayTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == rootThicknessAttr )
	{
		mRootThickness = static_cast< Real >( aDataHandle.asFloat() ) * mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == rootThicknessTextureAttr )
	{
		mRootThicknessTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == scaleAttr )
	{
		mScale = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == scaleTextureAttr )
	{
		mScaleTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == tipColorAttr )
	{
		mTipColor[ 0 ] = static_cast< Real >( aDataHandle.asFloat3()[ 0 ] );
		mTipColor[ 1 ] = static_cast< Real >( aDataHandle.asFloat3()[ 1 ] );
		mTipColor[ 2 ] = static_cast< Real >( aDataHandle.asFloat3()[ 2 ] );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == tipColorTextureAttr || root == tipColorTextureAttr  )
	{
		mTipColorTexture->setConnection( root );
		return true;
	}
	if ( aPlug == tipFrizzAttr )
	{
		mTipFrizz = static_cast< Real >( aDataHandle.asFloat() ) * mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == tipFrizzTextureAttr )
	{
		mTipFrizzTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == tipKinkAttr )
	{
		mTipKink = static_cast< Real >( aDataHandle.asFloat() ) * mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == tipKinkTextureAttr )
	{
		mTipKinkTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == tipOpacityAttr )
	{
		mTipOpacity = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == tipOpacityTextureAttr )
	{
		mTipOpacityTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == tipSplayAttr )
	{
		mTipSplay = static_cast< Real >( aDataHandle.asFloat() ) * mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == tipSplayTextureAttr )
	{
		mTipSplayTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == tipThicknessAttr )
	{
		mTipThickness = static_cast< Real >( aDataHandle.asFloat() ) * mScaleFactor;
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == tipThicknessTextureAttr )
	{
		mTipThicknessTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == twistAttr )
	{
		mTwist = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == twistTextureAttr )
	{
		mTwistTexture->setConnection( aPlug );
		return true;
	}
	if ( aPlug == valueVariationAttr )
	{
		mValueVariation = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return false;
	}
	if ( aPlug == valueVariationTextureAttr )
	{
		mValueVariationTexture->setConnection( aPlug );
		return true;
	}
	/* TEXTURE DIMENSIONS FOR SAMPLING ATTRIBUTES */
	if ( aPlug == densityTextureSamplingUDimensionAttr )
	{
		mDensityTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == densityTextureSamplingVDimensionAttr )
	{
		mDensityTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == interpolationGroupsTextureSamplingUDimensionAttr )
	{
		mInterpolationGroupsTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == interpolationGroupsTextureSamplingVDimensionAttr )
	{
		mInterpolationGroupsTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == cutTextureSamplingUDimensionAttr )
	{
		mCutTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == cutTextureSamplingVDimensionAttr )
	{
		mCutTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == scaleTextureSamplingUDimensionAttr )
	{
		mScaleTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == scaleTextureSamplingVDimensionAttr )
	{
		mScaleTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == randScaleTextureSamplingUDimensionAttr )
	{
		mRandScaleTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == randScaleTextureSamplingVDimensionAttr )
	{
		mRandScaleTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == rootThicknessTextureSamplingUDimensionAttr )
	{
		mRootThicknessTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == rootThicknessTextureSamplingVDimensionAttr )
	{
		mRootThicknessTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == tipThicknessTextureSamplingUDimensionAttr )
	{
		mTipThicknessTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == tipThicknessTextureSamplingVDimensionAttr )
	{
		mTipThicknessTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == displacementTextureSamplingUDimensionAttr )
	{
		mDisplacementTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == displacementTextureSamplingVDimensionAttr )
	{
		mDisplacementTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	/* MAYA COLOR PROPERTIES */
	if ( aPlug == rootOpacityTextureSamplingUDimensionAttr )
	{
		mRootOpacityTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == rootOpacityTextureSamplingVDimensionAttr )
	{
		mRootOpacityTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == tipOpacityTextureSamplingUDimensionAttr )
	{
		mTipOpacityTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == tipOpacityTextureSamplingVDimensionAttr )
	{
		mTipOpacityTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == rootColorTextureSamplingUDimensionAttr )
	{
		mRootColorTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == rootColorTextureSamplingVDimensionAttr )
	{
		mRootColorTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == tipColorTextureSamplingUDimensionAttr )
	{
		mTipColorTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == tipColorTextureSamplingVDimensionAttr )
	{
		mTipColorTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == hueVariationTextureSamplingUDimensionAttr )
	{
		mHueVariationTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == hueVariationTextureSamplingVDimensionAttr )
	{
		mHueVariationTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == valueVariationTextureSamplingUDimensionAttr )
	{
		mValueVariationTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == valueVariationTextureSamplingVDimensionAttr )
	{
		mValueVariationTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == mutantHairColorTextureSamplingUDimensionAttr )
	{
		mMutantHairColorTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == mutantHairColorTextureSamplingVDimensionAttr )
	{
		mMutantHairColorTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == percentMutantHairTextureSamplingUDimensionAttr )
	{
		mPercentMutantHairTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == percentMutantHairTextureSamplingVDimensionAttr )
	{
		mPercentMutantHairTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	/* MAYA FRIZZ PROPERTIES */
	if ( aPlug == rootFrizzTextureSamplingUDimensionAttr )
	{
		mRootFrizzTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == rootFrizzTextureSamplingVDimensionAttr )
	{
		mRootFrizzTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == tipFrizzTextureSamplingUDimensionAttr )
	{
		mTipFrizzTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == tipFrizzTextureSamplingVDimensionAttr )
	{
		mTipFrizzTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == frizzXFrequencyTextureSamplingUDimensionAttr )
	{
		mFrizzXFrequencyTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == frizzXFrequencyTextureSamplingVDimensionAttr )
	{
		mFrizzXFrequencyTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == frizzYFrequencyTextureSamplingUDimensionAttr )
	{
		mFrizzYFrequencyTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == frizzYFrequencyTextureSamplingVDimensionAttr )
	{
		mFrizzYFrequencyTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == frizzZFrequencyTextureSamplingUDimensionAttr )
	{
		mFrizzZFrequencyTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == frizzZFrequencyTextureSamplingVDimensionAttr )
	{
		mFrizzZFrequencyTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == frizzAnimTextureSamplingUDimensionAttr )
	{
		mFrizzAnimTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == frizzAnimTextureSamplingVDimensionAttr )
	{
		mFrizzAnimTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == frizzAnimSpeedTextureSamplingUDimensionAttr )
	{
		mFrizzAnimSpeedTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == frizzAnimSpeedTextureSamplingVDimensionAttr )
	{
		mFrizzAnimSpeedTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	/* MAYA KINK PROPERTIES */
	if ( aPlug == rootKinkTextureSamplingUDimensionAttr )
	{
		mRootKinkTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == rootKinkTextureSamplingVDimensionAttr )
	{
		mRootKinkTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == tipKinkTextureSamplingUDimensionAttr )
	{
		mTipKinkTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == tipKinkTextureSamplingVDimensionAttr )
	{
		mTipKinkTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == kinkXFrequencyTextureSamplingUDimensionAttr )
	{
		mKinkXFrequencyTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == kinkXFrequencyTextureSamplingVDimensionAttr )
	{
		mKinkXFrequencyTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == kinkYFrequencyTextureSamplingUDimensionAttr )
	{
		mKinkYFrequencyTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == kinkYFrequencyTextureSamplingVDimensionAttr )
	{
		mKinkYFrequencyTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == kinkZFrequencyTextureSamplingUDimensionAttr )
	{
		mKinkZFrequencyTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == kinkZFrequencyTextureSamplingVDimensionAttr )
	{
		mKinkZFrequencyTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	/* MAYA MULTI STRAND PROPERTIES */
	if ( aPlug == rootSplayTextureSamplingUDimensionAttr )
	{
		mRootSplayTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == rootSplayTextureSamplingVDimensionAttr )
	{
		mRootSplayTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == tipSplayTextureSamplingUDimensionAttr )
	{
		mTipSplayTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == tipSplayTextureSamplingVDimensionAttr )
	{
		mTipSplayTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == centerSplayTextureSamplingUDimensionAttr )
	{
		mCenterSplayTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == centerSplayTextureSamplingVDimensionAttr )
	{
		mCenterSplayTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == twistTextureSamplingUDimensionAttr )
	{
		mTwistTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == twistTextureSamplingVDimensionAttr )
	{
		mTwistTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == offsetTextureSamplingUDimensionAttr )
	{
		mOffsetTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == offsetTextureSamplingVDimensionAttr )
	{
		mOffsetTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == aspectTextureSamplingUDimensionAttr )
	{
		mAspectTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == aspectTextureSamplingVDimensionAttr )
	{
		mAspectTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == randomizeStrandTextureSamplingUDimensionAttr )
	{
		mRandomizeStrandTextureSamplingUDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}

	if ( aPlug == randomizeStrandTextureSamplingVDimensionAttr )
	{
		mRandomizeStrandTextureSamplingVDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aTextureResolutionHasChanged = true;
		return false;
	}


	return false;
}

MStatus MayaHairProperties::brakeConnection( const MPlug & aPlug )
{
	const MPlug &root = aPlug.isChild() ? aPlug.parent() : aPlug;  // root
	if ( aPlug == densityTextureAttr )
	{
		mDensityTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == interpolationGroupsTextureAttr || root == interpolationGroupsTextureAttr )
	{
		mInterpolationGroupsTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == aspectTextureAttr )
	{
		mAspectTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == centerSplayTextureAttr )
	{
		mCenterSplayTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == cutTextureAttr )
	{
		mCutTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == displacementTextureAttr )
	{
		mDisplacementTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == frizzAnimSpeedTextureAttr )
	{
		mFrizzAnimSpeedTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == frizzAnimTextureAttr )
	{
		mFrizzAnimTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == frizzXFrequencyTextureAttr )
	{
		mFrizzXFrequencyTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == frizzYFrequencyTextureAttr )
	{
		mFrizzYFrequencyTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == frizzZFrequencyTextureAttr )
	{
		mFrizzZFrequencyTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == hueVariationTextureAttr )
	{
		mHueVariationTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == kinkXFrequencyTextureAttr )
	{
		mKinkXFrequencyTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == kinkYFrequencyTextureAttr )
	{
		mKinkYFrequencyTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == kinkZFrequencyTextureAttr )
	{
		mKinkZFrequencyTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == mutantHairColorTextureAttr || root == mutantHairColorTextureAttr )
	{
		mMutantHairColorTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == offsetTextureAttr )
	{
		mOffsetTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == percentMutantHairTextureAttr )
	{
		mPercentMutantHairTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == randomizeStrandTextureAttr )
	{
		mRandomizeStrandTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == randScaleTextureAttr )
	{
		mRandScaleTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == rootColorTextureAttr || root == rootColorTextureAttr )
	{
		mRootColorTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == rootFrizzTextureAttr )
	{
		mRootFrizzTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == rootKinkTextureAttr )
	{
		mRootKinkTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == rootOpacityTextureAttr )
	{
		mRootOpacityTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == rootSplayTextureAttr )
	{
		mRootSplayTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == rootThicknessTextureAttr )
	{
		mRootThicknessTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == scaleTextureAttr )
	{
		mScaleTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == tipColorTextureAttr || root == tipColorTextureAttr  )
	{
		mTipColorTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == tipFrizzTextureAttr )
	{
		mTipFrizzTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == tipKinkTextureAttr )
	{
		mTipKinkTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == tipOpacityTextureAttr )
	{
		mTipOpacityTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == tipSplayTextureAttr )
	{
		mTipSplayTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == tipThicknessTextureAttr )
	{
		mTipThicknessTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == twistTextureAttr )
	{
		mTwistTexture->removeConnection();
		return MStatus::kSuccess;
	}
	if ( aPlug == valueVariationTextureAttr )
	{
		mValueVariationTexture->removeConnection();
		return MStatus::kSuccess;
	}
	return MStatus::kSuccess;
}

void MayaHairProperties::refreshTextures( unsigned __int32 aTextureSamples, bool aForceRefresh, bool & aDensityChanged,
	bool & aInterpolationGroupsChanged, bool & aHairPropertiesChanged )
{
	aDensityChanged = aInterpolationGroupsChanged = aHairPropertiesChanged = false;
	if ( mInterpolationGroupsTexture->isDirty() || aForceRefresh )
	{
		mInterpolationGroupsTexture->resample( mInterpolationGroupsTextureSamplingUDimension,
			mInterpolationGroupsTextureSamplingVDimension );
		mInterpolationGroups->updateGroups( *mInterpolationGroupsTexture, DEFAULT_SEGMENTS_COUNT );
		updateIntArrayComponentsCount( segmentsCountAttr, mInterpolationGroups->getGroupsCount(), 
			DEFAULT_SEGMENTS_COUNT, 1, 30, 1, 30 );
		updateIntArrayComponentsCount( interpolationGroupsSelectableAttr, mInterpolationGroups->getGroupsCount(), 
			1, 0, 1, 0, 1, "groups_selectable_" );
		aInterpolationGroupsChanged = true;
	}
	if ( mDensityTexture->isDirty() || aForceRefresh )
	{
		mDensityTexture->resample( mDensityTextureSamplingUDimension, mDensityTextureSamplingVDimension );
		aDensityChanged = true;
	}
	// Refreshes all other textures
	if ( mAspectTexture->isDirty() || aForceRefresh )
	{
		mAspectTexture->resample( mAspectTextureSamplingUDimension, mAspectTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mCenterSplayTexture->isDirty() || aForceRefresh )
	{
		mCenterSplayTexture->resample( mCenterSplayTextureSamplingUDimension, mCenterSplayTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mCutTexture->isDirty() || aForceRefresh )
	{
		mCutTexture->resample( mCutTextureSamplingUDimension, mCutTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mDisplacementTexture->isDirty() || aForceRefresh )
	{
		mDisplacementTexture->resample( mDisplacementTextureSamplingUDimension,
			mDisplacementTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mFrizzAnimSpeedTexture->isDirty() || aForceRefresh )
	{
		mFrizzAnimSpeedTexture->resample( mFrizzAnimSpeedTextureSamplingUDimension,
			mFrizzAnimSpeedTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mFrizzAnimTexture->isDirty() || aForceRefresh )
	{
		mFrizzAnimTexture->resample( mFrizzAnimTextureSamplingUDimension, mFrizzAnimTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mFrizzXFrequencyTexture->isDirty() || aForceRefresh )
	{
		mFrizzXFrequencyTexture->resample( mFrizzXFrequencyTextureSamplingUDimension,
			mFrizzXFrequencyTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mFrizzYFrequencyTexture->isDirty() || aForceRefresh )
	{
		mFrizzYFrequencyTexture->resample( mFrizzYFrequencyTextureSamplingUDimension,
			mFrizzYFrequencyTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mFrizzZFrequencyTexture->isDirty() || aForceRefresh )
	{
		mFrizzZFrequencyTexture->resample( mFrizzZFrequencyTextureSamplingUDimension,
			mFrizzZFrequencyTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mHueVariationTexture->isDirty() || aForceRefresh )
	{
		mHueVariationTexture->resample( mHueVariationTextureSamplingUDimension,
			mHueVariationTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mKinkXFrequencyTexture->isDirty() || aForceRefresh )
	{
		mKinkXFrequencyTexture->resample( mKinkXFrequencyTextureSamplingUDimension,
			mKinkXFrequencyTextureSamplingVDimension);
		aHairPropertiesChanged = true;
	}
	if ( mKinkYFrequencyTexture->isDirty() || aForceRefresh )
	{
		mKinkYFrequencyTexture->resample( mKinkYFrequencyTextureSamplingUDimension,
			mKinkXFrequencyTextureSamplingVDimension);
		aHairPropertiesChanged = true;
	}
	if ( mKinkZFrequencyTexture->isDirty() || aForceRefresh )
	{
		mKinkZFrequencyTexture->resample( mKinkZFrequencyTextureSamplingUDimension,
			mKinkZFrequencyTextureSamplingVDimension);
		aHairPropertiesChanged = true;
	}
	if ( mMutantHairColorTexture->isDirty() || aForceRefresh )
	{
		mMutantHairColorTexture->resample( mMutantHairColorTextureSamplingUDimension,
			mMutantHairColorTextureSamplingVDimension);
		aHairPropertiesChanged = true;
	}
	if ( mOffsetTexture->isDirty() || aForceRefresh )
	{
		mOffsetTexture->resample( mOffsetTextureSamplingUDimension,
			mOffsetTextureSamplingVDimension);
		aHairPropertiesChanged = true;
	}
	if ( mPercentMutantHairTexture->isDirty() || aForceRefresh )
	{
		mPercentMutantHairTexture->resample( mPercentMutantHairTextureSamplingUDimension,
			mPercentMutantHairTextureSamplingVDimension);
		aHairPropertiesChanged = true;
	}
	if ( mRandomizeStrandTexture->isDirty() || aForceRefresh )
	{
		mRandomizeStrandTexture->resample( mRandomizeStrandTextureSamplingUDimension,
			mRandomizeStrandTextureSamplingVDimension);
		aHairPropertiesChanged = true;
	}
	if ( mRandScaleTexture->isDirty() || aForceRefresh )
	{
		mRandScaleTexture->resample( mRandScaleTextureSamplingUDimension,
			mRandScaleTextureSamplingVDimension);
		aHairPropertiesChanged = true;
	}
	if ( mRootColorTexture->isDirty() || aForceRefresh )
	{
		mRootColorTexture->resample( mRootColorTextureSamplingUDimension,
			mRootColorTextureSamplingVDimension);
		aHairPropertiesChanged = true;
	}
	if ( mRootFrizzTexture->isDirty() || aForceRefresh )
	{
		mRootFrizzTexture->resample( mRootFrizzTextureSamplingUDimension,
			mRootFrizzTextureSamplingVDimension);
		aHairPropertiesChanged = true;
	}
	if ( mRootOpacityTexture->isDirty() || aForceRefresh )
	{
		mRootOpacityTexture->resample( mRootOpacityTextureSamplingUDimension,
			mRootOpacityTextureSamplingVDimension);
		aHairPropertiesChanged = true;
	}
	if ( mRootSplayTexture->isDirty() || aForceRefresh )
	{
		mRootSplayTexture->resample( mRootSplayTextureSamplingUDimension,
			mRootSplayTextureSamplingVDimension);
		aHairPropertiesChanged = true;
	}
	if ( mRootThicknessTexture->isDirty() || aForceRefresh )
	{
		mRootThicknessTexture->resample( mRootThicknessTextureSamplingUDimension,
			mRootThicknessTextureSamplingVDimension);
		aHairPropertiesChanged = true;
	}
	if ( mScaleTexture->isDirty() || aForceRefresh )
	{
		mScaleTexture->resample( mScaleTextureSamplingUDimension, mScaleTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mTipColorTexture->isDirty() || aForceRefresh )
	{
		mTipColorTexture->resample( mTipColorTextureSamplingUDimension, mTipColorTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mTipFrizzTexture->isDirty() || aForceRefresh )
	{
		mTipFrizzTexture->resample( mTipFrizzTextureSamplingUDimension, mTipFrizzTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mTipKinkTexture->isDirty() || aForceRefresh )
	{
		mTipKinkTexture->resample( mTipKinkTextureSamplingUDimension, mTipKinkTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mTipOpacityTexture->isDirty() || aForceRefresh )
	{
		mTipOpacityTexture->resample( mTipOpacityTextureSamplingUDimension, mTipOpacityTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mTipSplayTexture->isDirty() || aForceRefresh )
	{
		mTipSplayTexture->resample( mTipSplayTextureSamplingUDimension, mTipSplayTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mTipThicknessTexture->isDirty() || aForceRefresh )
	{
		mTipThicknessTexture->resample( mTipThicknessTextureSamplingUDimension,
			mTipThicknessTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mTwistTexture->isDirty() || aForceRefresh )
	{
		mTwistTexture->resample( mTwistTextureSamplingUDimension, mTwistTextureSamplingVDimension );
		aHairPropertiesChanged = true;
	}
	if ( mValueVariationTexture->isDirty() || aForceRefresh )
	{
		mValueVariationTexture->resample( mValueVariationTextureSamplingUDimension,
			mValueVariationTextureSamplingVDimension);
		aHairPropertiesChanged = true;
	}
}

void MayaHairProperties::setCurrentTime( Time aTime )
{
	mCurrentTime = aTime;
	/* Density and Interpolation groups texture are not animated !!! */
	mCutTexture->setCurrentTime( aTime );
	mScaleTexture->setCurrentTime( aTime );
	mRandScaleTexture->setCurrentTime( aTime );
	mRootThicknessTexture->setCurrentTime( aTime );
	mTipThicknessTexture->setCurrentTime( aTime );
	mDisplacementTexture->setCurrentTime( aTime );
	mRootOpacityTexture->setCurrentTime( aTime );
	mTipOpacityTexture->setCurrentTime( aTime );
	mRootColorTexture->setCurrentTime( aTime );
	mTipColorTexture->setCurrentTime( aTime );
	mHueVariationTexture->setCurrentTime( aTime );
	mValueVariationTexture->setCurrentTime( aTime );
	mMutantHairColorTexture->setCurrentTime( aTime );
	mPercentMutantHairTexture->setCurrentTime( aTime );
	mRootFrizzTexture->setCurrentTime( aTime );
	mTipFrizzTexture->setCurrentTime( aTime );
	mFrizzXFrequencyTexture->setCurrentTime( aTime );
	mFrizzYFrequencyTexture->setCurrentTime( aTime );
	mFrizzZFrequencyTexture->setCurrentTime( aTime );
	mFrizzAnimTexture->setCurrentTime( aTime );
	mFrizzAnimSpeedTexture->setCurrentTime( aTime );
	mRootKinkTexture->setCurrentTime( aTime );
	mTipKinkTexture->setCurrentTime( aTime );
	mKinkXFrequencyTexture->setCurrentTime( aTime );
	mKinkYFrequencyTexture->setCurrentTime( aTime );
	mKinkZFrequencyTexture->setCurrentTime( aTime );
	mRootSplayTexture->setCurrentTime( aTime );
	mTipSplayTexture->setCurrentTime( aTime );
	mCenterSplayTexture->setCurrentTime( aTime );
	mTwistTexture->setCurrentTime( aTime );
	mOffsetTexture->setCurrentTime( aTime );
	mAspectTexture->setCurrentTime( aTime );
	mRandomizeStrandTexture->setCurrentTime( aTime );
}

void MayaHairProperties::addColorAttribute( const MString & aFullName, const MString & aBriefName,
	MObject & aAttribute, float aDefaultR, float aDefaultB, float aDefaultG )
{
	MFnNumericAttribute nAttr;
	aAttribute = nAttr.createColor( aFullName, aBriefName );
	nAttr.setDefault( aDefaultR, aDefaultB, aDefaultG );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	if ( MPxSurfaceShape::addAttribute( aAttribute ) != MStatus::kSuccess )
	{
		throw StubbleException( ( " MayaHairProperties::addColorTextureAttribute : adding attribute " 
			+ aFullName + " failed " ).asChar() );
	}
}

void MayaHairProperties::addFloatAttribute( const MString & aFullName, const MString & aBriefName,
	MObject & aAttribute, float aDefault, float aMin, float aMax, float aSoftMin, float aSoftMax )
{
	MFnNumericAttribute nAttr;
	aAttribute = nAttr.create( aFullName, aBriefName, MFnNumericData::kFloat, aDefault );
	nAttr.setMin( aMin );
	nAttr.setMax( aMax );
	nAttr.setSoftMin( aSoftMin );
	nAttr.setSoftMax( aSoftMax );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	if ( MPxSurfaceShape::addAttribute( aAttribute ) != MStatus::kSuccess )
	{
		throw StubbleException( ( " MayaHairProperties::addFloatAttribute : adding attribute " 
			+ aFullName + " failed " ).asChar() );
	}
}

void MayaHairProperties::fillIntArrayAttributes( MObject & aAttribute, int aFillCount, int aDefault, int aMin, int aMax, int aSoftMin, int aSoftMax, MString aGroupNamePrefix )
{
	MStatus status;
	MFnCompoundAttribute nAttr(aAttribute, &status);

	for(int i = 0; i < aFillCount; ++i)
	{
		MFnNumericAttribute numericAttribute;
		MString s = aGroupNamePrefix;
		s += nAttr.numChildren();

		MObject c = numericAttribute.create( s, s, MFnNumericData::kInt, aDefault );
		numericAttribute.setMin( aMin );
		numericAttribute.setMax( aMax );
		numericAttribute.setSoftMin( aSoftMin );
		numericAttribute.setSoftMax( aSoftMax );
		numericAttribute.setKeyable( false );
		numericAttribute.setInternal( true );
		nAttr.addChild(c);
	}
}

void MayaHairProperties::fillColorArrayAttributes( MObject & aAttribute, const InterpolationGroups & aInterpolationGroups )
{
	MStatus status;
	MFnCompoundAttribute nAttr(aAttribute, &status);

	for( unsigned __int32 i = 0; i < aInterpolationGroups.getGroupsCount() ; ++i)
	{
		MFnNumericAttribute numericAttribute;
		MString s = "color_";
		s += nAttr.numChildren();
		MObject c = numericAttribute.createColor( s, s );
		const float * color = aInterpolationGroups.getColorOfGroup( i );
		numericAttribute.setDefault( color[ 0 ] , color[ 1 ], color[ 2 ] );
		numericAttribute.setKeyable( false );
		numericAttribute.setWritable( false );
		nAttr.addChild(c);
	}
}

void MayaHairProperties::addIntArrayAttribute( const MString & aFullName, const MString & aBriefName,
	MObject & aAttribute, int aComponentsCount, int aDefault, int aMin, int aMax, int aSoftMin, int aSoftMax )
{
	MFnCompoundAttribute nAttr;
	aAttribute = nAttr.create(aFullName, aBriefName);
	fillIntArrayAttributes( aAttribute, aComponentsCount, aDefault, aMin, aMax, aSoftMin, aSoftMax );

	if ( MPxSurfaceShape::addAttribute( aAttribute ) != MStatus::kSuccess )
	{
		throw StubbleException( ( " MayaHairProperties::addIntArrayAttribute : adding attribute " 
			+ aFullName + " failed " ).asChar() );
	}
}

void MayaHairProperties::updateIntArrayComponentsCount( MObject & aAttribute, unsigned int aComponentsCount, 
	int aDefault, int aMin, int aMax, int aSoftMin, int aSoftMax, MString aGroupNamePrefix )
{
	MStatus s;
	MFnCompoundAttribute nAttr(aAttribute, &s);

	if( aComponentsCount < nAttr.numChildren() ) // we will remove children
		while( aComponentsCount != nAttr.numChildren() )
		{
			MObject child = nAttr.child( nAttr.numChildren() - 1 );
			nAttr.removeChild ( child );

			MFnNumericAttribute numAttr( child );
			numAttr.setHidden( true );
		}
	else // adding new children
		fillIntArrayAttributes( aAttribute, aComponentsCount - nAttr.numChildren(), aDefault, aMin, aMax, aSoftMin, aSoftMax, aGroupNamePrefix );
	MStringArray arr;
	nAttr.getAddAttrCmds( arr, true );
	for ( unsigned __int32 i = 0; i < arr.length(); ++i )
	{
		MGlobal::executeCommand( arr[ i ] );
	}
}

void MayaHairProperties::addIntAttribute( const MString & aFullName, const MString & aBriefName,
	MObject & aAttribute, int aDefault, int aMin, int aMax, int aSoftMin, int aSoftMax )
{
	MFnNumericAttribute nAttr;
	aAttribute = nAttr.create( aFullName, aBriefName, MFnNumericData::kInt, aDefault );
	nAttr.setMin( aMin );
	nAttr.setMax( aMax );
	nAttr.setSoftMin( aSoftMin );
	nAttr.setSoftMax( aSoftMax );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	if ( MPxSurfaceShape::addAttribute( aAttribute ) != MStatus::kSuccess )
	{
		throw StubbleException( ( " MayaHairProperties::addIntAttribute : adding attribute " 
			+ aFullName + " failed " ).asChar() );
	}
}

void MayaHairProperties::addBoolAttribute( const MString & aFullName, const MString & aBriefName,
	MObject & aAttribute, bool aDefault )
{
	MFnNumericAttribute nAttr;
	aAttribute = nAttr.create( aFullName, aBriefName, MFnNumericData::kBoolean, aDefault );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	if ( MPxSurfaceShape::addAttribute( aAttribute ) != MStatus::kSuccess )
	{
		throw StubbleException( ( " MayaHairProperties::addBoolAttribute : adding attribute " 
			+ aFullName + " failed " ).asChar() );
	}
}

void MayaHairProperties::addParentAttribute( const MString & aFullName, const MString & aBriefName,
	MObject & aAttribute, const MObject & aChildAttribute1, const MObject & aChildAttribute2, 
	const MObject & aChildAttribute3 )
{
	MFnNumericAttribute nAttr;
	aAttribute = nAttr.create( aFullName, aBriefName, aChildAttribute1, aChildAttribute2, aChildAttribute3 );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	if ( MPxSurfaceShape::addAttribute( aAttribute ) != MStatus::kSuccess )
	{
		throw StubbleException( ( " MayaHairProperties::addBoolAttribute : adding attribute " 
			+ aFullName + " failed " ).asChar() );
	}
}

} // namespace Maya

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble