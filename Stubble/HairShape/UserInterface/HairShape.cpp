#include "HairShape.hpp"

#include "Common/CommonConstants.hpp"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MPlugArray.h>
#include <maya/MPolyMessage.h>
#include <maya/MTime.h>

#include <fstream>
#include <sstream>
#include <bzip2stream.hpp>

namespace Stubble
{

namespace HairShape
{

const MTypeId HairShape::typeId( 0x80002 );
const MString HairShape::typeName( "HairShape" );

/// attributes for the HairShape
MObject HairShape::countAttr;
MObject HairShape::genCountAttr;
MObject HairShape::surfaceAttr;
MObject HairShape::surfaceChangeAttr;
MObject HairShape::voxelsResolutionAttr;
MObject HairShape::voxelsXResolutionAttr;
MObject HairShape::voxelsYResolutionAttr;
MObject HairShape::voxelsZResolutionAttr;
MObject HairShape::timeAttr;
MObject HairShape::genDisplayCountAttr;
MObject HairShape::displayGuidesAttr;
MObject HairShape::displayInterpolatedAttr;

// Callback ids
MCallbackIdArray HairShape::mCallbackIds;

// Active node
HairShape * HairShape::mActiveHairShapeNode = 0;

HairShape::HairShape(): 
	mUVPointGenerator( 0 ), 
	mMayaMesh( 0 ), 
	mHairGuides( 0 ), 
	mVoxelization( 0 ),
	mGuidesHairCount( 100 ),
	mGeneratedHairCount( 10000 ),
	mTime( 0 ),
	mIsTopologyModified( false ),
	mIsTopologyCallbackRegistered( false ),
	mGenDisplayCount( 1000 ),
	mDisplayGuides( true ),
	mDisplayInterpolated( false )
{
	// Sets voxels resolution
	mVoxelsResolution[ 0 ] = mVoxelsResolution[ 1 ] = mVoxelsResolution[ 2 ] = 1;
	// Sets active object
	mActiveHairShapeNode = this;
}

void HairShape::postConstructor()
{
	setRenderable( true );
}

HairShape::~HairShape()
{
	delete mUVPointGenerator;
	delete mMayaMesh;
	delete mHairGuides;
	delete mVoxelization;
}

bool HairShape::isBounded() const
{
	return true;
}

MBoundingBox HairShape::boundingBox() const
{
	MObject thisNode = thisMObject();
	// get surfaceChangeAttr plug, causes compute function to be called
	int val;
	MPlug RedrawPlug( thisNode, HairShape::surfaceChangeAttr );
	RedrawPlug.getValue( val );
	// returns updated bounding box
	return mBoundingBox;
}

MStatus HairShape::compute(const MPlug &aPlug, MDataBlock &aDataBlock)
{
	MStatus status; // Error code
	//we only care about the redrawAttr
	if ( aPlug == surfaceChangeAttr ) // Handle mesh change
	{
		aDataBlock.setClean( surfaceChangeAttr );
		meshChange( aDataBlock.inputValue(surfaceAttr).asMesh() );
	}
	return MS::kSuccess;
}

bool HairShape::getInternalValueInContext( const MPlug & aPlug, MDataHandle & aDataHandle, MDGContext & aContext )
{
	return false; // Everything is handled defaultly
}

bool HairShape::setInternalValueInContext( const MPlug& aPlug, const MDataHandle& aDataHandle, MDGContext& aContext)
{
	const MPlug &root = aPlug.isChild() ? aPlug.parent() : aPlug;  // root
	if ( aPlug == countAttr ) // Guides hair count was changed
	{
		mGuidesHairCount = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		mHairGuides->generate( *mUVPointGenerator, *mMayaMesh, MayaHairProperties::getInterpolationGroups(), 
			mGuidesHairCount, true );
		refreshPointersToGuidesForInterpolation();
		if ( mDisplayInterpolated )
		{
			mInterpolatedHair.propertiesUpdate( *this );
		}
		return false;
	}
	if ( aPlug == genCountAttr ) // Generated hair count was changed
	{
		mGeneratedHairCount = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		return false;
	}
	if ( aPlug == timeAttr )
	{
		setCurrentTime( static_cast< Time >( aDataHandle.asTime().value() ) );
		return false;
	}
	if ( aPlug == voxelsResolutionAttr ) // Voxels resolution was changed
	{
		const int3 & res = aDataHandle.asInt3();
        mVoxelsResolution[ 0 ] = static_cast< unsigned __int32 >( res[ 0 ] );
		mVoxelsResolution[ 1 ] = static_cast< unsigned __int32 >( res[ 1 ] );
		mVoxelsResolution[ 2 ] = static_cast< unsigned __int32 >( res[ 2 ] );
		delete mVoxelization; // Throw away old voxelization
		mVoxelization = 0;
		return false;
	}
	if ( aPlug == voxelsXResolutionAttr ) // Voxels X resolution was changed
	{
        mVoxelsResolution[ 0 ] = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		delete mVoxelization; // Throw away old voxelization
		mVoxelization = 0;
		return false;
	}
	if ( aPlug == voxelsYResolutionAttr ) // Voxels Y resolution was changed
	{
        mVoxelsResolution[ 1 ] = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		delete mVoxelization; // Throw away old voxelization
		mVoxelization = 0;
		return false;
	}
	if ( aPlug == voxelsZResolutionAttr ) // Voxels Z resolution was changed
	{
        mVoxelsResolution[ 2 ] = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		delete mVoxelization; // Throw away old voxelization
		mVoxelization = 0;
		return false;
	}
	if ( aPlug == genDisplayCountAttr ) // Number of interpolated hair to be displayed
	{
		mGenDisplayCount = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		if ( mDisplayInterpolated )
		{
			mInterpolatedHair.generate( *mUVPointGenerator, *mMayaMesh, 
				mMayaMesh->getRestPose(), *this, mGenDisplayCount );
		}
		return false;
	}
	if ( aPlug == displayGuidesAttr ) // Guides have been hidden/shown
	{
		mDisplayGuides = aDataHandle.asBool();
		return false;
	}
	if ( aPlug == displayInterpolatedAttr ) // Interpolated have been hidden/shown
	{
		/*mDisplayInterpolated = aDataHandle.asBool(); TODO UNCOMMENT WHEN READY*/
		if ( mDisplayInterpolated )
		{
			mInterpolatedHair.generate( *mUVPointGenerator, *mMayaMesh, 
				mMayaMesh->getRestPose(), *this, mGenDisplayCount );
		}
		return false;
	}
	// Set hair properties values
	bool segmentsCountChanged;
	bool hairPropertiesChanged;
	MayaHairProperties::setAttributesValues( aPlug, aDataHandle, segmentsCountChanged, hairPropertiesChanged );
	// Calls this always, it cost nothing..
	if ( mHairGuides != 0 )
	{
		mHairGuides->setNumberOfGuidesToInterpolateFrom( MayaHairProperties::getNumberOfGuidesToInterpolateFrom() );
	}
	if ( segmentsCountChanged )
	{
		mHairGuides->updateSegmentsCount( MayaHairProperties::getInterpolationGroups() );
		refreshPointersToGuidesForInterpolation();
		if ( mDisplayInterpolated )
		{
			mInterpolatedHair.propertiesUpdate( *this );
			hairPropertiesChanged = false; // Already handled
		}
		return false;
	}
	if ( hairPropertiesChanged )
	{
		if ( mDisplayInterpolated )
		{
			mInterpolatedHair.propertiesUpdate( *this );
		}
		return false;
	}
	return false;
}

void* HairShape::creator()
{
	return new HairShape();
}

void HairShape::draw()
{
	if ( mDisplayGuides )
	{
		mHairGuides->draw();
	}
	if ( mDisplayInterpolated )
	{
		mInterpolatedHair.draw();
	}
}

MStatus HairShape::initialize()
{	
	MStatus status;

	//define surface attribute
	MFnTypedAttribute tAttr;
	surfaceAttr = tAttr.create( "surface", "srf", MFnData::kMesh, MObject::kNullObj, &status );
	tAttr.setDisconnectBehavior( MFnAttribute::kDelete );
	tAttr.setWritable( true );
	tAttr.setHidden( true );
	tAttr.setInternal( true );
	tAttr.setStorable( false );
	if( !status )
	{
		status.perror( "Creation of a mesh has failed" );
		return status;
	}
	addAttribute( surfaceAttr );

	MFnUnitAttribute unitAttr;
    timeAttr = unitAttr.create( "time", "tm", MFnUnitAttribute::kTime, 0.0, &status );
	unitAttr.setInternal( true );
	//unitAttr.setHidden( true );
	addAttribute( timeAttr );

	//define count attribute
	MFnNumericAttribute nAttr;
	countAttr = nAttr.create( "guides_count", "cnt", MFnNumericData::kInt, 100 );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	nAttr.setMin( 3 );
	nAttr.setSoftMin( 3 );
	nAttr.setSoftMax( 1000 );
	addAttribute( countAttr );

	//define gen. count attribute
	genCountAttr = nAttr.create( "interpolated_hair_count", "gcnt", MFnNumericData::kInt, 10000 );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	nAttr.setMin( 1 );
	nAttr.setSoftMin( 1 );
	nAttr.setSoftMax( 20000 );
	addAttribute( genCountAttr );

	// define voxels dimensions attribute
	voxelsXResolutionAttr = nAttr.create("voxels_X_dimensions", "vxsxdim", MFnNumericData::kInt, 1);
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	voxelsYResolutionAttr = nAttr.create("voxels_Y_dimensions", "vxsydim", MFnNumericData::kInt, 1);
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	voxelsZResolutionAttr = nAttr.create("voxels_Z_dimensions", "vxszdim", MFnNumericData::kInt, 1);
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	voxelsResolutionAttr = nAttr.create( "voxels_dimensions", "vxsdim", voxelsXResolutionAttr, voxelsYResolutionAttr, voxelsZResolutionAttr );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	nAttr.setMin( 1, 1, 1 );
	nAttr.setMax( 10, 10, 10 );
	addAttribute( voxelsXResolutionAttr );
	addAttribute( voxelsYResolutionAttr );
	addAttribute( voxelsZResolutionAttr );
	addAttribute( voxelsResolutionAttr );

	//define gen. display count attribute
	genDisplayCountAttr = nAttr.create( "displayed_hair_count", "dhc", MFnNumericData::kInt, 1000 );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	nAttr.setMin( 1 );
	nAttr.setSoftMin( 1 );
	nAttr.setSoftMax( 10000 );
	nAttr.setMax( 50000 );
	addAttribute( genDisplayCountAttr );

	//define display guides attribute
	displayGuidesAttr = nAttr.create( "display_guides", "digu", MFnNumericData::kBoolean, true );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	addAttribute( displayGuidesAttr );

	//define display interpolated hair attribute
	displayInterpolatedAttr = nAttr.create( "display_hair", "diha", MFnNumericData::kBoolean, false );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	addAttribute( displayInterpolatedAttr );

	surfaceChangeAttr = nAttr.create("surface_change", "srfc", MFnNumericData::kInt, 0);
	nAttr.setHidden( true );
	addAttribute( surfaceChangeAttr );

	// I have to check surface attr all the time
	attributeAffects( surfaceAttr, surfaceChangeAttr );

	return MayaHairProperties::initializeAttributes();
}

void HairShape::sampleTime( Time aSampleTime, const std::string & aFileName, BoundingBoxes & aVoxelBoundingBoxes )
{
	// Refresh all textures
	refreshTextures();
	// Sets current time
	setCurrentTime( aSampleTime );
	// Open file 
	std::string mainFileName = aFileName;
	mainFileName += ".FRM" ;
	std::ofstream mainFile( mainFileName.c_str(), ios::binary );
	bzip2_stream::bzip2_ostream bzipper( mainFile );
	// Write id
	bzipper.write( FRAME_FILE_ID, FRAME_FILE_ID_SIZE );
	// Write all hair properties ( textures, guides ... )
	MayaHairProperties::exportToFile( bzipper );
	// Flush bzipper
	bzipper.zflush();
	// Closes main file
	mainFile.close();
	// Prepare voxelization
	if ( mVoxelization == 0 ) // Voxelization does not exist
	{
		// Creates voxelization
		mVoxelization = new Voxelization( mMayaMesh->getRestPose(), *mUVPointGenerator, mVoxelsResolution );
	}
	// For every voxel
	for ( unsigned __int32 i = 0; i < mVoxelization->getVoxelsCount(); ++i )
	{
		if ( !mVoxelization->isVoxelEmpty( i ) )
		{
			// Open file 
			std::ostringstream voxelFileName; aFileName;
			voxelFileName << aFileName << ".VX" << aVoxelBoundingBoxes.size();
			std::ofstream voxelFile( voxelFileName.str().c_str(), ios::binary );
			bzip2_stream::bzip2_ostream bzipper( voxelFile );
			// Write id
			bzipper.write( VOXEL_FILE_ID, VOXEL_FILE_ID_SIZE );
			// Write number of generated hair in current voxel
			unsigned __int32 hairCount = mVoxelization->getVoxelHairCount( mGeneratedHairCount, i );
			bzipper.write( reinterpret_cast< const char * >( &hairCount ), sizeof( unsigned __int32 ) );
			// Write rest pose mesh
			mVoxelization->exportRestPoseVoxel( bzipper, mMayaMesh->getRestPose(), i );
			// Write current mesh and store its bounding box
			aVoxelBoundingBoxes.push_back( mVoxelization->exportCurrentVoxel( bzipper, *mMayaMesh, i ) );
			// Flush bzipper
			bzipper.zflush();
			// Closes voxel file
			voxelFile.close();
		}
	}
}

void HairShape::refreshTextures()
{
	bool densityChanged, interpolationGroupsChanged, hairPropertiesChanged;
	// Actual refresh textures
	MayaHairProperties::refreshTextures( densityChanged, interpolationGroupsChanged, hairPropertiesChanged );
	// React on refreshed textures
	if ( interpolationGroupsChanged )
	{
		// Update segments count of hair guides
		mHairGuides->updateSegmentsCount( MayaHairProperties::getInterpolationGroups() );
		refreshPointersToGuidesForInterpolation();
		// Update hair properties
		if ( mDisplayInterpolated )
		{
			mInterpolatedHair.propertiesUpdate( *this );
			hairPropertiesChanged = false; // Already handled
		}
	}
	if ( densityChanged )
	{
		delete mUVPointGenerator;
		delete mVoxelization;
		mUVPointGenerator = new UVPointGenerator( MayaHairProperties::getDensityTexture(),
			mMayaMesh->getRestPose().getTriangleConstIterator(), RandomGenerator());
		// HairGuides reconstruction
		mHairGuides->generate( *mUVPointGenerator,
			*mMayaMesh,
			MayaHairProperties::getInterpolationGroups(),
			mGuidesHairCount, true ); // Interpolates from previous hair guides
		mBoundingBox = mHairGuides->getBoundingBox().toMBoundingBox();
		refreshPointersToGuidesForInterpolation();
		// Interpolated hair reconstruction
		if ( mDisplayInterpolated )
		{
			mInterpolatedHair.generate( *mUVPointGenerator, *mMayaMesh, 
				mMayaMesh->getRestPose(), *this, mGenDisplayCount );
			hairPropertiesChanged = false; // Already handled
		}
	}
	if ( hairPropertiesChanged )
	{
		// Update hair properties
		if ( mDisplayInterpolated )
		{
			mInterpolatedHair.propertiesUpdate( *this );
		}
	}
}

/************************************************************************************************************/
/*											PRIVATE METHODS													*/
/************************************************************************************************************/

void HairShape::meshChange( MObject aMeshObj )
{
	if( aMeshObj == MObject::kNullObj ) 
	{
		return;
	}
	if ( !mIsTopologyCallbackRegistered )
	{
		registerTopologyCallback();
	}
	MFnMesh mesh = aMeshObj;
	MString uvSetName;
	mesh.getCurrentUVSetName( uvSetName );
	if( mMayaMesh == 0 ) // No mesh exists ?
	{
		
		// maya mesh construction
		mMayaMesh = new MayaMesh( aMeshObj, uvSetName );

		mUVPointGenerator = new UVPointGenerator( MayaHairProperties::getDensityTexture(),
			mMayaMesh->getRestPose().getTriangleConstIterator(), RandomGenerator());

		// HairGuides construction
		mHairGuides = new HairComponents::HairGuides();
		mHairGuides->generate( *mUVPointGenerator,
			*mMayaMesh,
			MayaHairProperties::getInterpolationGroups(),
			mGuidesHairCount );
		mHairGuides->setCurrentTime( mTime );
		refreshPointersToGuidesForInterpolation();
		// Interpolated hair construction
		if ( mDisplayInterpolated )
		{
			mInterpolatedHair.generate( *mUVPointGenerator, *mMayaMesh, 
				mMayaMesh->getRestPose(), *this, mGenDisplayCount );
		}
	}
	else
	{
		if ( mIsTopologyModified )
		{
			// Creates new mesh
			delete mMayaMesh;
			mMayaMesh = new MayaMesh( aMeshObj, uvSetName );
			// Creates new generator
			delete mUVPointGenerator;
			delete mVoxelization;
			mUVPointGenerator = new UVPointGenerator( MayaHairProperties::getDensityTexture(),
				mMayaMesh->getRestPose().getTriangleConstIterator(), RandomGenerator());
			mHairGuides->meshUpdate( *mMayaMesh, true );
			refreshPointersToGuidesForInterpolation();
			// Interpolated hair construction
			if ( mDisplayInterpolated )
			{
				mInterpolatedHair.generate( *mUVPointGenerator, *mMayaMesh, 
					mMayaMesh->getRestPose(), *this, mGenDisplayCount );
			}
		}
		else
		{
			mMayaMesh->meshUpdate( aMeshObj, uvSetName );
			mHairGuides->meshUpdate( *mMayaMesh, false );
			// Interpolated hair positions recalculation
			if ( mDisplayInterpolated )
			{
				mInterpolatedHair.meshUpdate( *mMayaMesh );
			}
		}
		mIsTopologyModified = false;
	}
	// Calculates new bounding box
	mBoundingBox = mHairGuides->getBoundingBox().toMBoundingBox();
}

void HairShape::setCurrentTime( Time aTime )
{
	mTime = aTime;
	if ( mHairGuides != 0 ) // Might be called before any hairguides were created
	{
		mHairGuides->setCurrentTime( aTime );
		refreshPointersToGuidesForInterpolation();
	}
	MayaHairProperties::setTexturesTime( aTime );
	if ( mDisplayInterpolated )
	{
		mInterpolatedHair.propertiesUpdate( *this );
	}
}

// Inline, but only called inside HairGuides.cpp
inline void HairShape::refreshPointersToGuidesForInterpolation()
{
	MayaHairProperties::refreshPointersToGuides( & mHairGuides->getCurrentFrameSegments().mSegments,
		& mHairGuides->getGuidesPositionsUG( getInterpolationGroups() ) );
}

/************************************************************************************************************/
/*										TOPOLOGY CALLBACKS HANDLING											*/
/************************************************************************************************************/

///----------------------------------------------------------------------------------------------------
/// Mesh identifier changed function. 
///
/// \param	aComponentIds				IDs of changed components. 
/// \param	aCount						Number of component types. 
/// \param [in,out]	aIsTopologyModified	Pointer to mesh modified boolean.
///----------------------------------------------------------------------------------------------------
static void meshIDChangedFunction( MUintArray aComponentIds[], unsigned int aCount, void *aIsTopologyModified )
{
	*( reinterpret_cast< bool * >( aIsTopologyModified ) ) = true; // Mesh has been modified !!!
}

///----------------------------------------------------------------------------------------------------
/// Mesh topology changed function. 
///
/// \param [in,out]	aNode				The mesh node. 
/// \param [in,out]	aIsTopologyModified	Pointer to mesh modified boolean.
///----------------------------------------------------------------------------------------------------
static void meshTopologyChangedFunction( MObject & aNode, void *aIsTopologyModified )
{
	*( reinterpret_cast< bool * >( aIsTopologyModified ) ) = true; // Mesh has been modified !!!
}

MStatus HairShape::registerTopologyCallback()
{
	MStatus status; // Error checking
	// Try to select connected surface
	MPlug meshPlugMy( thisMObject(), HairShape::surfaceAttr );
	MPlugArray surfacePlugs;
	meshPlugMy.connectedTo( surfacePlugs, true, false, &status );
	if ( status != MStatus::kSuccess || surfacePlugs.length() != 1)
	{
		status.perror( " Failed to select connected surface " );
		return status;
	}
	MDagPath meshDag;
	MObject shapeNode = surfacePlugs[ 0 ].node();
	if ( MS::kSuccess == MDagPath::getAPathTo( shapeNode, meshDag )
			&& MS::kSuccess == meshDag.extendToShape() )
	{
	    shapeNode = meshDag.node();
	}
	else
	{
		status.perror( " Failed to select connected surface " );
		return status;
	}
	// We want report about all types of changes
	bool wantIdChanges[3];
    wantIdChanges[ MPolyMessage::kVertexIndex ] = true;
    wantIdChanges[ MPolyMessage::kEdgeIndex ] = true;        
    wantIdChanges[ MPolyMessage::kFaceIndex ] = true;
	// Register callback "id change"
	mCallbackIds.append( MPolyMessage::addPolyComponentIdChangedCallback( shapeNode,  wantIdChanges, 3, meshIDChangedFunction, 
		reinterpret_cast< void * >( &mIsTopologyModified ), &status ) );
	if ( status != MStatus::kSuccess )
	{
		status.perror( " Failed to register topology changed callback #1 " );
		return status;
	}
	// Register callback "topology change"
	mCallbackIds.append( MPolyMessage::addPolyTopologyChangedCallback( shapeNode, meshTopologyChangedFunction, 
		reinterpret_cast< void * >( &mIsTopologyModified ), &status ) );
	if ( status != MStatus::kSuccess )
	{
		status.perror( " Failed to register topology changed callback #2 " );
		return status;
	}
	// Register callback to mesh
	mIsTopologyCallbackRegistered = true;
	return MStatus::kSuccess;
}

void HairShape::removeCallbacks()
{
	MPolyMessage::removeCallbacks( mCallbackIds );
	mCallbackIds.clear();
}

} // namespace HairShape

} // namespace Stubble