#include "HairShape.hpp"

#include "Common/GLExtensions.hpp"
#include "Common/CommonConstants.hpp"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MPlugArray.h>
#include <maya/MPolyMessage.h>
#include <maya/MTime.h>

#include <fstream>
#include <limits>
#include <sstream>
#include <zipstream.hpp>

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
		meshChange( aDataBlock.inputValue( surfaceAttr ).asMesh() );
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
		mDisplayInterpolated = aDataHandle.asBool();
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
	// First init gl extensions
	if ( !GLExt::isInited() )
	{
		GLExt::init();
	}
	// Display guides & interpolated hair
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
	try 
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
		if( !addAttribute( surfaceAttr ) )
		{
			status.perror( "Creation of a mesh has failed" );
			return status;
		}
		// I have to check surface attr all the time
		MFnNumericAttribute nAttr;
		surfaceChangeAttr = nAttr.create("surface_change", "srfc", MFnNumericData::kInt, 0);
		nAttr.setHidden( true );
		if( !addAttribute( surfaceChangeAttr ) )
		{
			status.perror( "Creation of a mesh has failed" );
			return status;
		}
		attributeAffects( surfaceAttr, surfaceChangeAttr );
		// define time attribute
		MFnUnitAttribute unitAttr;
		timeAttr = unitAttr.create( "time", "tm", MFnUnitAttribute::kTime, 0.0, &status );
		unitAttr.setInternal( true );
		if( !addAttribute( timeAttr ) )
		{
			status.perror( "Adding time attr has failed" );
			return status;
		}
		//define count attribute
		addIntAttribute( "guides_count", "cnt", countAttr, 100, 3, 1000000, 3, 1000 );
		//define gen. count attribute
		addIntAttribute( "interpolated_hair_count", "gcnt", genCountAttr, 10000, 1, 
			std::numeric_limits< int >::max(), 1, 20000 );
		// define voxels dimensions attribute
		addIntAttribute( "voxels_X_dimensions", "vxsxdim", voxelsXResolutionAttr, 1, 1, 10, 1, 10 );
		addIntAttribute( "voxels_Y_dimensions", "vxsydim", voxelsYResolutionAttr, 1, 1, 10, 1, 10 );
		addIntAttribute( "voxels_Z_dimensions", "vxszdim", voxelsZResolutionAttr, 1, 1, 10, 1, 10 );
		addParentAttribute( "voxels_dimensions", "vxsdim", voxelsResolutionAttr, voxelsXResolutionAttr, 
			voxelsYResolutionAttr, voxelsZResolutionAttr );
		//define gen. display count attribute
		addIntAttribute( "displayed_hair_count", "dhc", genDisplayCountAttr, 1000, 1, 50000, 1, 10000 );
		//define display guides attribute
		addBoolAttribute( "display_guides", "digu", displayGuidesAttr, true );
		//define display interpolated hair attribute
		addBoolAttribute( "display_hair", "diha", displayInterpolatedAttr, false );
	}
	catch( const StubbleException & ex )
	{
		MStatus s;
		s.perror( ex.what() );
		return s;
	}
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
	zlib_stream::zip_ostream zipper( mainFile, ios::out, false, COMPRESSION, 
		zlib_stream::StrategyFiltered, 15, 9, BUFFER_SIZE );
	// Write id
	zipper.write( FRAME_FILE_ID, FRAME_FILE_ID_SIZE );
	// Write all hair properties ( textures, guides ... )
	MayaHairProperties::exportToFile( zipper );
	// Flush zipper
	zipper.zflush();
	// Closes main file
	mainFile.close();
	// Prepare voxelization
	if ( mVoxelization == 0 ) // Voxelization does not exist
	{
		// Creates voxelization
		mVoxelization = new Voxelization( mMayaMesh->getRestPose(), *mUVPointGenerator, mVoxelsResolution );
	}
	// For every voxel
	for ( unsigned __int32 i = 0, hairCountTotal = 0; i < mVoxelization->getVoxelsCount(); ++i )
	{
		if ( !mVoxelization->isVoxelEmpty( i ) )
		{
			// Open file 
			std::ostringstream voxelFileName; aFileName;
			voxelFileName << aFileName << ".VX" << aVoxelBoundingBoxes.size();
			std::ofstream voxelFile( voxelFileName.str().c_str(), ios::binary );
			zlib_stream::zip_ostream zipper( voxelFile, ios::out, false, COMPRESSION, 
				zlib_stream::StrategyFiltered, 15, 9, BUFFER_SIZE );
			// Write id
			zipper.write( VOXEL_FILE_ID, VOXEL_FILE_ID_SIZE );
			// Write start hair index
			zipper.write( reinterpret_cast< const char * >( &hairCountTotal ), sizeof( unsigned __int32 ) );
			// Write number of generated hair in current voxel
			unsigned __int32 hairCount = mVoxelization->getVoxelHairCount( mGeneratedHairCount, i );
			zipper.write( reinterpret_cast< const char * >( &hairCount ), sizeof( unsigned __int32 ) );
			// Increase hair count total
			hairCountTotal += hairCount;
			// Write rest pose mesh
			mVoxelization->exportRestPoseVoxel( zipper, mMayaMesh->getRestPose(), i );
			// Write current mesh and store its bounding box
			aVoxelBoundingBoxes.push_back( mVoxelization->exportCurrentVoxel( zipper, *mMayaMesh, i ) );
			// Flush zipper
			zipper.zflush();
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
			mMayaMesh->getRestPose().getTriangleConstIterator(), mRandom);
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
			mMayaMesh->getRestPose().getTriangleConstIterator(), mRandom);

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
				mMayaMesh->getRestPose().getTriangleConstIterator(), mRandom);
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
	MayaHairProperties::setCurrentTime( aTime );
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