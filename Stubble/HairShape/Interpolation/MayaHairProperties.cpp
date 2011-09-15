#include "MayaHairProperties.hpp"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MPxSurfaceShape.h>

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

/* MAYA PROPERTIES */

MObject MayaHairProperties::densityTextureAttr;
MObject MayaHairProperties::interpolationGroupsTextureAttr;
MObject MayaHairProperties::segmentsCountAttr;
MObject MayaHairProperties::numberOfGuidesToInterpolateFromAttr;

void MayaHairProperties::exportToFile( std::ostream & aOutputStream ) const
{	
	mDensityTexture->exportToFile( aOutputStream );
	mInterpolationGroupsTexture->exportToFile( aOutputStream );
	mInterpolationGroups->exportSegmentsCountToFile( aOutputStream );
	// Write number of guides to interpolate from
	aOutputStream.write( reinterpret_cast< const char * >( &mNumberOfGuidesToInterpolateFrom ), 
		sizeof( unsigned __int32 ) );
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
	// TODO : export all other properties
}

MayaHairProperties::MayaHairProperties()
{
	mInterpolationGroupsTexture = new Texture( 1, 1, 1 );
	mDensityTexture = new Texture( 1 );
	mInterpolationGroups = new Interpolation::InterpolationGroups( *mInterpolationGroupsTexture, DEFAULT_SEGMENTS_COUNT );
}

MStatus MayaHairProperties::initializeAttributes()
{
	MStatus status;
	MFnNumericAttribute nAttr;

	// TODO handle segments count for each group 
	segmentsCountAttr = nAttr.create("segments_count", "sgc", MFnNumericData::kInt, DEFAULT_SEGMENTS_COUNT );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	nAttr.setMin( 1 );
	nAttr.setSoftMin( 1 );
	nAttr.setSoftMax( 10 );
	status = MPxSurfaceShape::addAttribute(segmentsCountAttr);
	if ( status != MStatus::kSuccess )
	{
		return status;
	}

	// define density texture attribute
	densityTextureAttr = nAttr.create( "density_texture", "dtxt", MFnNumericData::kFloat, 1 );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	nAttr.setMin( 0.0 );
	nAttr.setSoftMin( 0.0 );
	nAttr.setSoftMax( 1.0 );
	status = MPxSurfaceShape::addAttribute( densityTextureAttr );
	if ( status != MStatus::kSuccess )
	{
		return status;
	}

	// define interpolation groups texture attribute
	interpolationGroupsTextureAttr = nAttr.createColor( "interpolation_groups_texture", "itxt");
	nAttr.setDefault( 1, 1, 1 );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	status = MPxSurfaceShape::addAttribute( interpolationGroupsTextureAttr );
	if ( status != MStatus::kSuccess )
	{
		return status;
	}

	// define number of guides to interpolate from
	numberOfGuidesToInterpolateFromAttr = nAttr.create("interpolation_samples", "ints", MFnNumericData::kInt, 3 );
	nAttr.setMin( 3 );
	nAttr.setMax( 20 );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	status = MPxSurfaceShape::addAttribute( numberOfGuidesToInterpolateFromAttr );
	if ( status != MStatus::kSuccess )
	{
		return status;
	}

	return MS::kSuccess;
}

void MayaHairProperties::setAttributesValues( const MPlug& aPlug, const MDataHandle& aDataHandle,
		bool & aSegmentsCountChanged, bool & aHairPropertiesChanged )
{
	aSegmentsCountChanged = aHairPropertiesChanged = false;
	if ( aPlug == segmentsCountAttr ) // Segments count changed
	{
		mInterpolationGroups->setGroupSegmentsCount( 0, static_cast< unsigned __int32 >( aDataHandle.asInt() ) );
		aSegmentsCountChanged = true;
		return;
	}
	if ( aPlug == densityTextureAttr )
	{
		// TODO uncomment when resample works mDensityTexture->setDirty();
		return;
	}
	if ( aPlug == interpolationGroupsTextureAttr )
	{
		// TODO uncomment when resample works mInterpolationGroupsTexture->setDirty();
		return;
	}
	if ( aPlug == numberOfGuidesToInterpolateFromAttr ) // Number of guides to interpolate from was changed
	{
		mNumberOfGuidesToInterpolateFrom = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		return;
	}
	// TODO Handle all other textures and attributes
}

void MayaHairProperties::refreshTextures( bool & aDensityChanged, bool & aInterpolationGroupsChanged, 
	bool & aHairPropertiesChanged )
{
	aDensityChanged = aInterpolationGroupsChanged = aHairPropertiesChanged = false;
	if ( mInterpolationGroupsTexture->isDirty() )
	{
		mInterpolationGroupsTexture->resample();
		// TODO handle UI segments count selection
		mInterpolationGroups->updateGroups( *mInterpolationGroupsTexture, DEFAULT_SEGMENTS_COUNT );
		aInterpolationGroupsChanged = true;
	}
	if ( mDensityTexture->isDirty() )
	{
		mDensityTexture->resample();
		aDensityChanged = true;
	}
	/* TODO refresh all textures and do mInterpolatedHair.propertiesUpdate */
}

void MayaHairProperties::setTexturesTime( Time aTime )
{
	/* TODO set internal time for all textures */
	/* Density and Interpolation groups texture are not animated !!! */
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble