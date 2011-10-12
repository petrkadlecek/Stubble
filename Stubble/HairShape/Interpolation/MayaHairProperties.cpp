#include "MayaHairProperties.hpp"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MPxSurfaceShape.h>

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

/* MAYA BASIC PROPERTIES */
MObject MayaHairProperties::densityTextureAttr; ///< The density texture attribute
MObject MayaHairProperties::interpolationGroupsTextureAttr; ///< The interpolation groups texture attribute
MObject MayaHairProperties::cutTextureAttr;  ///< The cut texture attribute
MObject MayaHairProperties::segmentsCountAttr; ///< The segments count attribute
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
	mGuidesRestPositionsUG->exportToFile( aOutputStream );
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

MayaHairProperties::MayaHairProperties()
{
	mInterpolationGroupsTexture = new Texture( 1, 1, 1 );
	mDensityTexture = new Texture( 1, 1, 1, 1 );
	mInterpolationGroups = new Interpolation::InterpolationGroups( *mInterpolationGroupsTexture, DEFAULT_SEGMENTS_COUNT );
	mCutTexture = new Texture( 1, 1, 1, 1 );
	mScaleTexture = new Texture( 1, 1, 1, 1 );
	mRandScaleTexture = new Texture( 1, 1, 1, 1 );
	mRootThicknessTexture = new Texture( 1, 1, 1, 1 );
	mTipThicknessTexture = new Texture( 1, 1, 1, 1 );
	mDisplacementTexture = new Texture( 1, 1, 1, 1 );
	mRootOpacityTexture = new Texture( 1, 1, 1, 1 );
	mTipOpacityTexture = new Texture( 1, 1, 1, 1 );
	mRootColorTexture = new Texture( 1, 1, 1 );
	mTipColorTexture = new Texture( 1, 1, 1 );
	mHueVariationTexture = new Texture( 1, 1, 1, 1 );
	mValueVariationTexture = new Texture( 1, 1, 1, 1 );
	mMutantHairColorTexture = new Texture( 1, 1, 1 );
	mPercentMutantHairTexture = new Texture( 1, 1, 1, 1 );
	mRootFrizzTexture = new Texture( 1, 1, 1, 1 );
	mTipFrizzTexture = new Texture( 1, 1, 1, 1 );
	mFrizzXFrequencyTexture = new Texture( 1, 1, 1, 1 );
	mFrizzYFrequencyTexture = new Texture( 1, 1, 1, 1 );
	mFrizzZFrequencyTexture = new Texture( 1, 1, 1, 1 );
	mFrizzAnimTexture = new Texture( 1, 1, 1, 1 );
	mFrizzAnimSpeedTexture = new Texture( 1, 1, 1, 1 );
	mRootKinkTexture = new Texture( 1, 1, 1, 1 );
	mTipKinkTexture = new Texture( 1, 1, 1, 1 );
	mKinkXFrequencyTexture = new Texture( 1, 1, 1, 1 );
	mKinkYFrequencyTexture = new Texture( 1, 1, 1, 1 );
	mKinkZFrequencyTexture = new Texture( 1, 1, 1, 1 );
	mRootSplayTexture = new Texture( 1, 1, 1, 1 );
	mTipSplayTexture = new Texture( 1, 1, 1, 1 );
	mCenterSplayTexture = new Texture( 1, 1, 1, 1 );
	mTwistTexture = new Texture( 1, 1, 1, 1 );
	mOffsetTexture = new Texture( 1, 1, 1, 1 );
	mAspectTexture = new Texture( 1, 1, 1, 1 );
	mRandomizeStrandTexture = new Texture( 1, 1, 1, 1 );
}

MStatus MayaHairProperties::initializeAttributes()
{
	try
	{
		int int_max = std::numeric_limits< int >::max();
		float float_max = std::numeric_limits< float >::max();
		float float_min = std::numeric_limits< float >::min();

		/* MAYA BASIC PROPERTIES */
		addIntArrayAttribute( "segments_count", "sgc", segmentsCountAttr, 10, DEFAULT_SEGMENTS_COUNT, 1, 100, 1, 10 );
		updateIntArrayComponentsCount( segmentsCountAttr, 6, DEFAULT_SEGMENTS_COUNT, 1, 100, 1, 10 );

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
		addFloatAttribute( "hue_variation", "hue", hueVariationAttr, 0, 0, 360, 0, 360 );
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
		addFloatAttribute( "aspect", "asp", aspectAttr, 0, 0, float_max, 0, 5 );
		addFloatAttribute( "randomize_strand_texture", "rstrtxt", randomizeStrandTextureAttr, 1, 0, 1, 0, 1 );
		addFloatAttribute( "randomize_strand", "rstr", randomizeStrandAttr, 0, 0, 1, 0, 1 );
	}
	catch( const StubbleException & ex )
	{
		MStatus s;
		s.perror( ex.what() );
		return s;
	}
	return MStatus::kSuccess;
}

void MayaHairProperties::setAttributesValues( const MPlug& aPlug, const MDataHandle& aDataHandle,
		bool & aSegmentsCountChanged, bool & aHairPropertiesChanged )
{
	const MPlug &root = aPlug.isChild() ? aPlug.parent() : aPlug;  // root
	aSegmentsCountChanged = aHairPropertiesChanged = false;
	if ( aPlug == segmentsCountAttr )
	{
		mInterpolationGroups->setGroupSegmentsCount( 0, static_cast< unsigned __int32 >( aDataHandle.asInt() ) );
		aSegmentsCountChanged = true;
		return;
	}
	if ( aPlug == densityTextureAttr )
	{
		mDensityTexture->setConnection( aPlug );
		// TODO uncomment when resample works mDensityTexture->setDirty();
		return;
	}
	if ( aPlug == interpolationGroupsTextureAttr || root == interpolationGroupsTextureAttr )
	{
		mInterpolationGroupsTexture->setConnection( aPlug );
		// TODO: decide if it should be root or aPlug
		// TODO uncomment when resample works mInterpolationGroupsTexture->setDirty();
		return;
	}
	if ( aPlug == numberOfGuidesToInterpolateFromAttr )
	{
		mNumberOfGuidesToInterpolateFrom = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		return;
	}
	if ( aPlug == areNormalsCalculatedAttr )
	{
		mAreNormalsCalculated = aDataHandle.asBool();
		return;
	}
	// TODO Handle all other textures and attributes
	if ( aPlug == aspectAttr )
	{
		mAspect = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == aspectTextureAttr )
	{
		mAspectTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == centerSplayAttr )
	{
		mCenterSplay = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == centerSplayTextureAttr )
	{
		mCenterSplayTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == cutTextureAttr )
	{
		mCutTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == displacementAttr )
	{
		mDisplacement = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == displacementTextureAttr )
	{
		mDisplacementTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == frizzAnimAttr )
	{
		mFrizzAnim = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == frizzAnimDirectionAttr )
	{
		mFrizzAnimDirection.x = static_cast< Real >( aDataHandle.asFloat3()[ 0 ] );
		mFrizzAnimDirection.y = static_cast< Real >( aDataHandle.asFloat3()[ 1 ] );
		mFrizzAnimDirection.z = static_cast< Real >( aDataHandle.asFloat3()[ 2 ] );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == frizzAnimDirectionXAttr )
	{
		mFrizzAnimDirection.x = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == frizzAnimDirectionYAttr )
	{
		mFrizzAnimDirection.y = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == frizzAnimDirectionZAttr )
	{
		mFrizzAnimDirection.z = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == frizzAnimSpeedAttr )
	{
		mFrizzAnimSpeed = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == frizzAnimSpeedTextureAttr )
	{
		mFrizzAnimSpeedTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == frizzAnimTextureAttr )
	{
		mFrizzAnimTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == frizzXFrequencyAttr )
	{
		mFrizzXFrequency = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == frizzXFrequencyTextureAttr )
	{
		mFrizzXFrequencyTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == frizzYFrequencyAttr )
	{
		mFrizzYFrequency = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == frizzYFrequencyTextureAttr )
	{
		mFrizzYFrequencyTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == frizzZFrequencyAttr )
	{
		mFrizzZFrequency = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == frizzZFrequencyTextureAttr )
	{
		mFrizzZFrequencyTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == hueVariationAttr )
	{
		mHueVariation = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == hueVariationTextureAttr )
	{
		mHueVariationTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == kinkXFrequencyAttr )
	{
		mKinkXFrequency = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == kinkXFrequencyTextureAttr )
	{
		mKinkXFrequencyTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == kinkYFrequencyAttr )
	{
		mKinkYFrequency = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == kinkYFrequencyTextureAttr )
	{
		mKinkYFrequencyTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == kinkZFrequencyAttr )
	{
		mKinkZFrequency = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == kinkZFrequencyTextureAttr )
	{
		mKinkZFrequencyTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == multiStrandCountAttr )
	{
		mMultiStrandCount = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == mutantHairColorAttr )
	{
		mMutantHairColor[ 0 ] = static_cast< Real >( aDataHandle.asFloat3()[ 0 ] );
		mMutantHairColor[ 1 ] = static_cast< Real >( aDataHandle.asFloat3()[ 1 ] );
		mMutantHairColor[ 2 ] = static_cast< Real >( aDataHandle.asFloat3()[ 2 ] );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == mutantHairColorTextureAttr || root == mutantHairColorTextureAttr )
	{
		mMutantHairColorTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == offsetAttr )
	{
		mOffset = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == offsetTextureAttr )
	{
		mOffsetTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == percentMutantHairAttr )
	{
		mPercentMutantHair = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == percentMutantHairTextureAttr )
	{
		mPercentMutantHairTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == randomizeStrandAttr )
	{
		mRandomizeStrand = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == randomizeStrandTextureAttr )
	{
		mRandomizeStrandTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == randScaleAttr )
	{
		mRandScale = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == randScaleTextureAttr )
	{
		mRandScaleTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == rootColorAttr )
	{
		mRootColor[ 0 ] = static_cast< Real >( aDataHandle.asFloat3()[ 0 ] );
		mRootColor[ 1 ] = static_cast< Real >( aDataHandle.asFloat3()[ 1 ] );
		mRootColor[ 2 ] = static_cast< Real >( aDataHandle.asFloat3()[ 2 ] );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == rootColorTextureAttr || root == rootColorTextureAttr )
	{
		mRootColorTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == rootFrizzAttr )
	{
		mRootFrizz = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == rootFrizzTextureAttr )
	{
		mRootFrizzTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == rootKinkAttr )
	{
		mRootKink = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == rootKinkTextureAttr )
	{
		mRootKinkTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == rootOpacityAttr )
	{
		mRootOpacity = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == rootOpacityTextureAttr )
	{
		mRootOpacityTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == rootSplayAttr )
	{
		mRootSplay = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == rootSplayTextureAttr )
	{
		mRootSplayTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == rootThicknessAttr )
	{
		mRootThickness = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == rootThicknessTextureAttr )
	{
		mRootThicknessTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == scaleAttr )
	{
		mScale = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == scaleTextureAttr )
	{
		mScaleTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == tipColorAttr )
	{
		mTipColor[ 0 ] = static_cast< Real >( aDataHandle.asFloat3()[ 0 ] );
		mTipColor[ 1 ] = static_cast< Real >( aDataHandle.asFloat3()[ 1 ] );
		mTipColor[ 2 ] = static_cast< Real >( aDataHandle.asFloat3()[ 2 ] );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == tipColorTextureAttr || root == tipColorTextureAttr  )
	{
		mTipColorTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == tipFrizzAttr )
	{
		mTipFrizz = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == tipFrizzTextureAttr )
	{
		mTipFrizzTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == tipKinkAttr )
	{
		mTipKink = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == tipKinkTextureAttr )
	{
		mTipKinkTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == tipOpacityAttr )
	{
		mTipOpacity = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == tipOpacityTextureAttr )
	{
		mTipOpacityTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == tipSplayAttr )
	{
		mTipSplay = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == tipSplayTextureAttr )
	{
		mTipSplayTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == tipThicknessAttr )
	{
		mTipThickness = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == tipThicknessTextureAttr )
	{
		mTipThicknessTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == twistAttr )
	{
		mTwist = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == twistTextureAttr )
	{
		mTwistTexture->setConnection( aPlug );
		return;
	}
	if ( aPlug == valueVariationAttr )
	{
		mValueVariation = static_cast< Real >( aDataHandle.asFloat() );
		aHairPropertiesChanged = true;
		return;
	}
	if ( aPlug == valueVariationTextureAttr )
	{
		mValueVariationTexture->setConnection( aPlug );
		return;
	}
}

void MayaHairProperties::refreshTextures(unsigned __int32 aTextureSamples, bool & aDensityChanged,
	bool & aInterpolationGroupsChanged, bool & aHairPropertiesChanged )
{
	aDensityChanged = aInterpolationGroupsChanged = aHairPropertiesChanged = false;
	if ( mInterpolationGroupsTexture->isDirty() )
	{
		mInterpolationGroupsTexture->resample( aTextureSamples );
		// TODO handle UI segments count selection
		mInterpolationGroups->updateGroups( *mInterpolationGroupsTexture, DEFAULT_SEGMENTS_COUNT );
		aInterpolationGroupsChanged = true;
	}
	if ( mDensityTexture->isDirty() )
	{
		mDensityTexture->resample( aTextureSamples );
		aDensityChanged = true;
	}
	// Refreshes all other textures
	if ( mAspectTexture->isDirty() )
	{
		mAspectTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mCenterSplayTexture->isDirty() )
	{
		mCenterSplayTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mCutTexture->isDirty() )
	{
		mCutTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mDisplacementTexture->isDirty() )
	{
		mDisplacementTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mFrizzAnimSpeedTexture->isDirty() )
	{
		mFrizzAnimSpeedTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mFrizzAnimTexture->isDirty() )
	{
		mFrizzAnimTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mFrizzXFrequencyTexture->isDirty() )
	{
		mFrizzXFrequencyTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mFrizzYFrequencyTexture->isDirty() )
	{
		mFrizzYFrequencyTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mFrizzZFrequencyTexture->isDirty() )
	{
		mFrizzZFrequencyTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mHueVariationTexture->isDirty() )
	{
		mHueVariationTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mKinkXFrequencyTexture->isDirty() )
	{
		mKinkXFrequencyTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mKinkYFrequencyTexture->isDirty() )
	{
		mKinkYFrequencyTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mKinkZFrequencyTexture->isDirty() )
	{
		mKinkZFrequencyTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mMutantHairColorTexture->isDirty() )
	{
		mMutantHairColorTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mOffsetTexture->isDirty() )
	{
		mOffsetTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mPercentMutantHairTexture->isDirty() )
	{
		mPercentMutantHairTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mRandomizeStrandTexture->isDirty() )
	{
		mRandomizeStrandTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mRandScaleTexture->isDirty() )
	{
		mRandScaleTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mRootColorTexture->isDirty() )
	{
		mRootColorTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mRootFrizzTexture->isDirty() )
	{
		mRootFrizzTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mRootOpacityTexture->isDirty() )
	{
		mRootOpacityTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mRootSplayTexture->isDirty() )
	{
		mRootSplayTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mRootThicknessTexture->isDirty() )
	{
		mRootThicknessTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mScaleTexture->isDirty() )
	{
		mScaleTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mTipColorTexture->isDirty() )
	{
		mTipColorTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mTipFrizzTexture->isDirty() )
	{
		mTipFrizzTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mTipKinkTexture->isDirty() )
	{
		mTipKinkTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mTipOpacityTexture->isDirty() )
	{
		mTipOpacityTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mTipSplayTexture->isDirty() )
	{
		mTipSplayTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mTipThicknessTexture->isDirty() )
	{
		mTipThicknessTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mTwistTexture->isDirty() )
	{
		mTwistTexture->resample( aTextureSamples );
		aHairPropertiesChanged = true;
	}
	if ( mValueVariationTexture->isDirty() )
	{
		mValueVariationTexture->resample( aTextureSamples );
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

void MayaHairProperties::fillIntArrayAttributes( MObject & aAttribute, int aFillCount, int aDefault, int aMin, int aMax, int aSoftMin, int aSoftMax )
{
	MStatus status;
	MFnCompoundAttribute nAttr(aAttribute, &status);

	for(int i = 0; i < aFillCount; ++i)
	{
		MFnNumericAttribute numericAttribute;
		MString s = "group_";
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
	int aDefault, int aMin, int aMax, int aSoftMin, int aSoftMax )
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
		fillIntArrayAttributes( aAttribute, aComponentsCount - nAttr.numChildren(), aDefault, aMin, aMax, aSoftMin, aSoftMax );
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

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble