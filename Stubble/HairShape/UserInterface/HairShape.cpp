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
MObject HairShape::timeAttr;

// Callback ids
MCallbackIdArray HairShape::mCallbackIds;

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
	memset( reinterpret_cast< void * >( mVoxelsResolution ), 0, sizeof( unsigned __int32 ) * 3 );
	// Creates textures with default values
	mInterpolationGroupsTexture = new Texture( 1, 1, 1 );
	mDensityTexture = new Texture( 1 );
	mInterpolationGroups = new Interpolation::InterpolationGroups( *mInterpolationGroupsTexture, 5 );
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
		if ( !mIsTopologyCallbackRegistered )
		{
			registerTopologyCallback();
		}
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
	}
	if ( aPlug == genCountAttr ) // Generated hair count was changed
	{
		mGeneratedHairCount = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		/* TODO */
	}
	if ( aPlug == segmentsAttr ) // Segments count changed
	{
		mInterpolationGroups->setGroupSegmentsCount( 0, static_cast< unsigned __int32 >( aDataHandle.asInt() ) );
		mHairGuides->updateSegmentsCount( *mInterpolationGroups );
	}
	if ( aPlug == densityTextureAttr )
	{
		mDensityTexture->setDirty();
	}
	if ( root == interpolationGroupsTextureAttr )
	{
		mInterpolationGroupsTexture->setDirty();
	}
	if ( aPlug == timeAttr )
	{
		setCurrentTime( static_cast< Time >( aDataHandle.asTime().value() ) );
	}
	if ( root == voxelsResolutionAttr ) // Voxels resolution was changed
	{
		// Copy values
		for ( unsigned __int32 i = 0; i < root.numElements (); i++)
		{
           mVoxelsResolution[ i ] = static_cast< unsigned __int32 >( root[ i ].asInt() );
		}
		delete mVoxelization; // Throw away old voxelization
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
	tAttr.setReadable( false );
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
	voxelsResolutionAttr = nAttr.create( "voxels_dimensions", "vxsdim", MFnNumericData::k3Int, 1);
	nAttr.setKeyable( false );
	nAttr.setInternal( true );
	nAttr.setMin( 1, 1, 1 );
	nAttr.setMax( 10, 10, 10 );
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
	std::ofstream file( aFileName.c_str() );
	// Write id
	file.write( FRAME_FILE_ID, FRAME_FILE_ID_SIZE );
	// Refresh all textures
	refreshTextures();
	// Write all textures
	exportTextures( file );
	// Write all guides
	mHairGuides->exportToFile( file );
	// Write voxel table pos 
	std::ofstream::pos_type voxelsTablePos, voxelsTablePointerPos = file.tellp();
	// Prepare voxelization
	if ( mVoxelization == 0 ) // Voxelization does not exist
	{
		// Creates voxelization
		mVoxelization = new Voxelization( mMayaMesh->getRestPose(), *mUVPointGenerator, mVoxelsResolution );
	}
	// Prepare voxels table
	typedef std::vector< std::ofstream::pos_type > VoxelsFilePos;
	VoxelsFilePos voxelsFilePos;
	voxelsFilePos.reserve( mVoxelization->getVoxelsCount() ); 
	// For every voxel
	for ( unsigned __int32 i = 0; i < mVoxelization->getVoxelsCount(); ++i )
	{
		if ( !mVoxelization->isVoxelEmpty( i ) )
		{
			voxelsFilePos.push_back( file.tellp() ); // Remember position
			// Write number of generated hair in current voxel
			unsigned __int32 hairCount = mVoxelization->getVoxelHairCount( mGeneratedHairCount, i );
			file.write( reinterpret_cast< const char * >( &hairCount ), sizeof( unsigned __int32 ) );
			// Write rest pose mesh
			mVoxelization->exportRestPoseVoxel( file, mMayaMesh->getRestPose(), i );
			// Write current mesh
			mVoxelization->exportCurrentVoxel( file, *mMayaMesh, i );
		}
	}
	// Get voxels table pos
	voxelsTablePos = file.tellp();
	// Write voxels table (just list of voxels file positions, voxels count is not stored)
	for ( VoxelsFilePos::const_iterator it = voxelsFilePos.begin(); it != voxelsFilePos.end(); ++it )
	{
		file.write( reinterpret_cast< const char * >( &( *it ) ), sizeof( std::ofstream::pos_type ) );
	}
	// Finally we will store voxels table pos
	file.seekp( voxelsTablePointerPos );
	file.write( reinterpret_cast< const char * >( &voxelsTablePos ), sizeof( std::ofstream::pos_type ) );
	// Close the file
	file.close();
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
		mBoundingBox = mHairGuides->getBoundingBox().toMBoundingBox();
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
}

void HairShape::refreshTextures()
{
	/* TODO refresh all textures */
}

void HairShape::setCurrentTime( Time aTime )
{
	mTime = aTime;
	mHairGuides->setCurrentTime( aTime );
	/* TODO set internal time for textures */	
}

void HairShape::exportTextures( std::ostream & aOutputStream ) const
{
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