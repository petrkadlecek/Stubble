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
MObject HairShape::segmentsAttr;
MObject HairShape::densityTextureAttr;
MObject HairShape::interpolationGroupsTextureAttr;
MObject HairShape::voxelsResolutionAttr;
MObject HairShape::voxelsXResolutionAttr;
MObject HairShape::voxelsYResolutionAttr;
MObject HairShape::voxelsZResolutionAttr;
MObject HairShape::timeAttr;

// Callback ids
MCallbackIdArray HairShape::mCallbackIds;

// Active node
HairShape * HairShape::mActiveHairShapeNode = 0;

HairShape::HairShape(): 
	mUVPointGenerator( 0 ), 
	mMayaMesh( 0 ), 
	mHairGuides( 0 ), 
	mInterpolatedHairs( 0 ), 
	mVoxelization( 0 ),
	mDensityTexture( 0 ),
	mInterpolationGroupsTexture( 0 ),
	mInterpolationGroups( 0 ),
	mGuidesHairCount( 100 ),
	mGeneratedHairCount( 10000 ),
	mTime( 0 ),
	mIsTopologyModified( false ),
	mIsTopologyCallbackRegistered( false )
{
	// Sets voxels resolution
	mVoxelsResolution[ 0 ] = mVoxelsResolution[ 1 ] = mVoxelsResolution[ 2 ] = 1;
	// Creates textures with default values
	mInterpolationGroupsTexture = new Texture( 1, 1, 1 );
	mDensityTexture = new Texture( 1 );
	mInterpolationGroups = new Interpolation::InterpolationGroups( *mInterpolationGroupsTexture, 5 );
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
	delete mInterpolatedHairs;
	delete mVoxelization;
	delete mInterpolationGroups;
	delete mDensityTexture;
	delete mInterpolationGroupsTexture;
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
		mHairGuides->generate( *mUVPointGenerator, *mMayaMesh, *mInterpolationGroups, mGuidesHairCount, true );
		return false;
	}
	if ( aPlug == genCountAttr ) // Generated hair count was changed
	{
		mGeneratedHairCount = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		/* TODO */
		return false;
	}
	if ( aPlug == segmentsAttr ) // Segments count changed
	{
		mInterpolationGroups->setGroupSegmentsCount( 0, static_cast< unsigned __int32 >( aDataHandle.asInt() ) );
		mHairGuides->updateSegmentsCount( *mInterpolationGroups );
		return false;
	}
	if ( aPlug == densityTextureAttr )
	{
		// TODO uncomment when resample works mDensityTexture->setDirty();
		return false;
	}
	if ( root == interpolationGroupsTextureAttr )
	{
		// TODO uncomment when resample works mInterpolationGroupsTexture->setDirty();
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
	return false;
}

void* HairShape::creator()
{
	return new HairShape();
}

void HairShape::draw()
{
	mHairGuides->draw();
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
	addAttribute( countAttr );

	//define gen. count attribute
	genCountAttr = nAttr.create( "interpolated_hair_count", "gcnt", MFnNumericData::kInt, 10000 );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	nAttr.setMin( 1 );
	addAttribute( genCountAttr );

	//define segments attribute
	segmentsAttr = nAttr.create("segments_count", "sgc", MFnNumericData::kInt, 5);
	nAttr.setMin( 1 );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	addAttribute(segmentsAttr);

	// define density texture attribute
	densityTextureAttr = nAttr.create( "density_texture", "dtxt", MFnNumericData::kFloat, 1 );
	nAttr.setKeyable( false );
	nAttr.setChannelBox( true );
	nAttr.setInternal( true );
	addAttribute( densityTextureAttr );

	// define interpolation groups texture attribute
	interpolationGroupsTextureAttr = nAttr.createColor( "interpolation_groups_texture", "itxt");
	nAttr.setDefault( 1, 1, 1 );
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	addAttribute( interpolationGroupsTextureAttr );

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

	surfaceChangeAttr = nAttr.create("surface_change", "srfc", MFnNumericData::kInt, 0);
	nAttr.setHidden( true );
	addAttribute( surfaceChangeAttr );

	// I have to check surface attr all the time
	attributeAffects( surfaceAttr, surfaceChangeAttr );

	return MS::kSuccess;
}

void HairShape::sampleTime( Time aSampleTime, const std::string & aFileName, BoundingBoxes & aVoxelBoundingBoxes )
{
	// Sets current time
	setCurrentTime( aSampleTime );
	// Open file 
	std::string mainFileName = aFileName;
	mainFileName += ".FRM" ;
	std::ofstream mainFile( mainFileName.c_str(), ios::binary );
	// Write id
	mainFile.write( FRAME_FILE_ID, FRAME_FILE_ID_SIZE );
	// Refresh all textures
	refreshTextures();
	// Write all textures
	exportTextures( mainFile );
	// Write all guides
	mHairGuides->exportToFile( mainFile );
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
			// Write id
			voxelFile.write( VOXEL_FILE_ID, VOXEL_FILE_ID_SIZE );
			// Write number of generated hair in current voxel
			unsigned __int32 hairCount = mVoxelization->getVoxelHairCount( mGeneratedHairCount, i );
			voxelFile.write( reinterpret_cast< const char * >( &hairCount ), sizeof( unsigned __int32 ) );
			// Write rest pose mesh
			mVoxelization->exportRestPoseVoxel( voxelFile, mMayaMesh->getRestPose(), i );
			// Write current mesh and store its bounding box
			aVoxelBoundingBoxes.push_back( mVoxelization->exportCurrentVoxel( voxelFile, *mMayaMesh, i ) );
			// Closes voxel file
			voxelFile.close();
		}
	}
}

void HairShape::refreshTextures()
{
	if ( mInterpolationGroupsTexture->isDirty() )
	{
		mInterpolationGroupsTexture->resample();
		mInterpolationGroups->updateGroups( *mInterpolationGroupsTexture, 5 );
		// TODO handle UI segments count selection
	}
	if ( mDensityTexture->isDirty() )
	{
		mDensityTexture->resample();
		delete mUVPointGenerator;
		delete mVoxelization;
		mUVPointGenerator = new UVPointGenerator( Texture( 1 ),
			mMayaMesh->getRestPose().getTriangleConstIterator(), RandomGenerator());
		// HairGuides reconstruction
		mHairGuides->generate( *mUVPointGenerator,
			*mMayaMesh,
			Interpolation::InterpolationGroups( Texture( 1 ), 5 ),
			mGuidesHairCount, true ); // Interpolates from previous hair guides
		mBoundingBox = mHairGuides->getBoundingBox().toMBoundingBox();
	}
	/* TODO refresh all textures */
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

		mUVPointGenerator = new UVPointGenerator( Texture( 1 ),
			mMayaMesh->getRestPose().getTriangleConstIterator(), RandomGenerator());

		// HairGuides construction
		mHairGuides = new HairComponents::HairGuides();
		mHairGuides->generate( *mUVPointGenerator,
			*mMayaMesh,
			Interpolation::InterpolationGroups( Texture( 1 ), 5 ),
			mGuidesHairCount );
		mHairGuides->setCurrentTime( mTime );
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
			mUVPointGenerator = new UVPointGenerator( Texture( 1 ),
				mMayaMesh->getRestPose().getTriangleConstIterator(), RandomGenerator());
		}
		else
		{
			mMayaMesh->meshUpdate( aMeshObj, uvSetName );
		}
		mHairGuides->meshUpdate( *mMayaMesh, mIsTopologyModified );
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
	}
	mDensityTexture->setCurrentTime( aTime );
	mInterpolationGroupsTexture->setCurrentTime( aTime );
	/* TODO set internal time for all textures */	
}

void HairShape::exportTextures( std::ostream & aOutputStream ) const
{
	mDensityTexture->exportToFile( aOutputStream );
	mInterpolationGroupsTexture->exportToFile( aOutputStream );
	/* TODO export all textures to file */
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