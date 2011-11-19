#include "HairShape.hpp"

#include "Common/GLExtensions.hpp"
#include "Common/CommonConstants.hpp"

#include <maya/MAttributeSpecArray.h>
#include <maya/MAttributeSpec.h>
#include <maya/MAttributeIndex.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnSingleIndexedComponent.h>   
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MItMeshEdge.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItSelectionList.h>
#include <maya/MPlugArray.h>
#include <maya/MPolyMessage.h>
#include <maya/MTime.h>
#include <maya/MSceneMessage.h>

#include <exception>
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
MObject HairShape::timeChangeAttr;
MObject HairShape::genDisplayCountAttr;
MObject HairShape::displayGuidesAttr;
MObject HairShape::displayInterpolatedAttr;
MObject HairShape::sampleTextureDimensionAttr;
MObject HairShape::serializedDataAttr;

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
	mDisplayInterpolated( false ),
	mSampleTextureDimension( 128 ),
	mIsSelectionModified( false )
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
	if ( mActiveHairShapeNode == this )
	{
		mActiveHairShapeNode = 0;
	}
}

bool HairShape::isBounded() const
{
	return true;
}

MBoundingBox HairShape::boundingBox() const
{
	MObject thisNode = thisMObject();
	// get surfaceChangeAttr and timeChangeAttr plug, causes compute function to be called
	int val;
	MPlug RedrawPlug( thisNode, HairShape::surfaceChangeAttr );
	RedrawPlug.getValue( val );
	MPlug RedrawPlug2( thisNode, HairShape::timeChangeAttr );
	RedrawPlug2.getValue( val );
	// returns updated bounding box
	return mBoundingBox;
}

MStatus HairShape::compute(const MPlug &aPlug, MDataBlock &aDataBlock)
{	
	MStatus status; // Error code
	if ( aPlug == surfaceChangeAttr ) // Handle mesh change
	{
		aDataBlock.setClean( surfaceChangeAttr );
		meshChange( aDataBlock.inputValue( surfaceAttr ).asMesh() );
	}
	if ( aPlug == timeChangeAttr ) // Handle time change
	{
		aDataBlock.setClean( timeChangeAttr );
		setCurrentTime( static_cast< Time >( aDataBlock.inputValue( timeAttr ).asTime().value() ) );
	}	
	return MS::kSuccess;
}

bool HairShape::getInternalValueInContext( const MPlug & plug, MDataHandle & result, MDGContext & aContext )
{
	bool isOk = true;

	if( (plug == mControlPoints) ||
		(plug == mControlValueX) ||
		(plug == mControlValueY) ||
		(plug == mControlValueZ) )
	{		
		double val = 0.0;
		if ( (plug == mControlPoints) && !plug.isArray() ) {
			MPoint pnt;
			int index = plug.logicalIndex();
			value( index, pnt );
			result.set( pnt[0], pnt[1], pnt[2] );
		}
		else if ( plug == mControlValueX ) {
			MPlug parentPlug = plug.parent();
			int index = parentPlug.logicalIndex();
			value( index, 0, val );
			result.set( val );
		}
		else if ( plug == mControlValueY ) {
			MPlug parentPlug = plug.parent();
			int index = parentPlug.logicalIndex();
			value( index, 1, val );
			result.set( val );
		}
		else if ( plug == mControlValueZ ) {
			MPlug parentPlug = plug.parent();
			int index = parentPlug.logicalIndex();
			value( index, 2, val );
			result.set( val );
		}
	}	
	else {
		isOk = MPxSurfaceShape::getInternalValue( plug, result );
	}

	return isOk;
}

bool HairShape::setInternalValueInContext( const MPlug& aPlug, const MDataHandle& aDataHandle, MDGContext& aContext)
{
	const MPlug &root = aPlug.isChild() ? aPlug.parent() : aPlug;  // root
	if ( aPlug == countAttr ) // Guides hair count was changed
	{
		mGuidesHairCount = static_cast< unsigned __int32 >( aDataHandle.asInt() );
		if ( mUVPointGenerator == 0 || mMayaMesh == 0 ) // object is in construction
		{
			return false;
		}
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
	if ( aPlug == sampleTextureDimensionAttr ) // Number of samples in one dimension of sampled texture
	{
		mSampleTextureDimension = static_cast< unsigned __int32 >( aDataHandle.asInt() );
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

MStatus HairShape::connectionBroken( const MPlug & aPlug, const MPlug & aOtherPlug,bool aAsSrc )
{
	return MayaHairProperties::brakeConnection(aPlug);
}

void* HairShape::creator()
{
	return new HairShape();
}

void HairShape::draw()
{
	if ( this->isSelectionModified() )
	{
		MSelectionList selList;
		
		// Get the list of selected items 
		MGlobal::getActiveSelectionList( selList );

		//if the list is empty, just deselect everything
		if ( !(selList.length()) )
		{			
			MIntArray arr;
			this->mHairGuides->applySelection( arr );
		}

		 // we will use an iterator this time to walk over the selection list.
		MItSelectionList it( selList );
		while ( !it.isDone() ) 
		{

			MDagPath dagPath; 
			MObject	component;
			MStatus stat;

			// we retrieve a dag path to a transform or shape, and an MObject
			// to any components that are selected on that object (if any).
			//
			it.getDagPath( dagPath, component );

			// attach a function set to the object
			MFnDependencyNode fn(dagPath.node());
		
			// print the object name
			std::cout << "\nOBJECT: " << fn.name().asChar() << std::endl;

			/*MObjectArray activeComponents = this->activeComponents();
			std::cout << "Number of vertices: " << activeComponents.length() << std::endl;*/

			// If we have components to iterate over
			if (!component.isNull()) 
			{
				// determine the component type
				switch (component.apiType()) {

					// we have mesh vertices
					case MFn::kMeshVertComponent: {

						// Single-indexed components
						MFnSingleIndexedComponent *iComp = new MFnSingleIndexedComponent( component );
						MIntArray arr;
						iComp->getElements( arr );
						cout << "Index array length: " << arr.length() << endl;
						for ( int i = 0; i < arr.length(); i++ )
						{
							cout << arr[i] << " ";
						}

						cout << endl;

						// tell the node that it needs to rebuild its internal selection list
						this->mHairGuides->applySelection( arr );

						break;
					}

					case MFn::kMeshEdgeComponent: {
						MItMeshEdge itEdge( dagPath, component, &stat );
						while ( !itEdge.isDone() )
						{
							MPoint point = itEdge.center(MSpace::kWorld );

							// write the index and the position
							std::cout << "\t" << itEdge.index()
									  << ") " << point.x 
									  << " "  << point.y 
									  << " "  << point.z 
									  << (itEdge.isSmooth() ? " smooth\n" : " hard\n");

							itEdge.next();
						}
						break;
					}
											  				
					// do the default
					default:
						{
							std::cout << "HairShape::Draw() - Unknown Component Type!" << endl;
						}
						break;

				}

			}			
			it.next();
		}

		// important: let the shape know that its list of selected components is up to date
		this->setSelectionModified( false );
	}
	//--------------------------------------------------------------------------------
	
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

void HairShape::componentToPlugs( MObject &aComponent,  MSelectionList &aList ) const
{
	if ( aComponent.hasFn(MFn::kSingleIndexedComponent) ) {

		MFnSingleIndexedComponent fnEdgeComp( aComponent );
    	MObject thisNode = thisMObject();
		MPlug plug( thisNode, mControlPoints );
		// If this node is connected to a tweak node, reset the
		// plug to point at the tweak node.
		//
		//convertToTweakNodePlug(plug);

		int len = fnEdgeComp.elementCount();

		for ( int i = 0; i < len; i++ )
		{
			plug.selectAncestorLogicalIndex(fnEdgeComp.element(i),
											plug.attribute());
			aList.add(plug);
		}
	}
}

MPxSurfaceShape::MatchResult HairShape::matchComponent( const MSelectionList& aItem, 
										const MAttributeSpecArray& aSpec, 
										MSelectionList& aList )
{
	MPxSurfaceShape::MatchResult result = MPxSurfaceShape::kMatchOk;
	
	MAttributeSpec attrSpec = aSpec[0];
	int dim = attrSpec.dimensions();
	MAttributeSpec attrSpec2;
	int dim2 = 0;

	/*if ( aSpec.length() > 1 )
	{
		attrSpec2 = aSpec[1];
		dim2 = attrSpec2.dimensions();
		cout << "aSpec.length(): " << aSpec.length() << endl;
	}*/
	

	// Look for attributes specifications of the form :
	//     vtx[ index ]
	//     vtx[ lower:upper ]
	//
	
	if ( (1 == aSpec.length()) && (dim > 0) && (attrSpec.name() == "e") ) {
		//int numVertices = meshGeom()->vertices.length();
		int numVertices = 500;
		MAttributeIndex attrIndex = attrSpec[0];

		int upper = 0;
		int lower = 0;
		if ( attrIndex.hasLowerBound() ) {
			attrIndex.getLower( lower );
			cout << "Has lower bound: " << lower << endl;
		}
		if ( attrIndex.hasUpperBound() ) {
			attrIndex.getUpper( upper );
			cout << "Has upper bound: " << upper << endl;
		}

		// Check the attribute index range is valid
		//
		if ( (lower > upper) || (upper >= numVertices) ) {
			result = MPxSurfaceShape::kMatchInvalidAttributeRange;	
		}
		else {
			MDagPath path;
			aItem.getDagPath( 0, path );
			MFnSingleIndexedComponent fnEdgeComp;
			MObject edgeComp = fnEdgeComp.create( MFn::kMeshVertComponent );
			
			for ( int i=lower; i<=upper; i++ )
			{
				fnEdgeComp.addElement( i );
				cout << i << " ";
			}
			aList.add( path, edgeComp );
			cout << endl;
		}
	}
	//else if ( (2 == aSpec.length()) && (dim > 0) && (dim2 > 0) && (attrSpec.name() == "cv") ) {
	//	int numVertices = 500;
	//	MAttributeIndex attrIndex = attrSpec[0];

	//	int upper = 0;
	//	int lower = 0;
	//	if ( attrIndex.hasLowerBound() ) {
	//		attrIndex.getLower( lower );
	//		cout << "Has lower bound: " << lower << endl;
	//	}
	//	if ( attrIndex.hasUpperBound() ) {
	//		attrIndex.getUpper( upper );
	//		cout << "Has upper bound: " << upper << endl;
	//	}

	//	// Check the attribute index range is valid
	//	//
	//	if ( (lower > upper) || (upper >= numVertices) ) {
	//		result = MPxSurfaceShape::kMatchInvalidAttributeRange;	
	//	}
	//	else {
	//		/*MDagPath path;
	//		aItem.getDagPath( 0, path );
	//		MFnSingleIndexedComponent fnVtxComp;
	//		MObject vtxComp = fnVtxComp.create( MFn::kMeshVertComponent );*/
	//		
	//		for ( int i=lower; i<=upper; i++ )
	//		{
	//			//fnVtxComp.addElement( i );
	//			cout << i << " ";
	//		}
	//		//aList.add( path, vtxComp );
	//		cout << endl;
	//	}
	//}
	else {
		// Pass this to the parent class
		return MPxSurfaceShape::matchComponent( aItem, aSpec, aList );
	}

	return result;
}

bool HairShape::match(	const MSelectionMask & aMask, const MObjectArray& aComponentList ) const
{
	bool result = false;

	if( aComponentList.length() == 0 ) {
		result = aMask.intersects( MSelectionMask::kSelectMeshes );
	}
	else {
		for ( int i = 0; i < ( int ) aComponentList.length(); i++ ) {
			if ( ( aComponentList[i].apiType() == MFn::kMeshVertComponent ) &&
				 ( aMask.intersects( MSelectionMask::kSelectMeshVerts ) )
			) 
			{
				result = true;
				break;
			}
			else if ( ( aComponentList[i].apiType() == MFn::kMeshEdgeComponent ) &&
				( aMask.intersects( MSelectionMask::kSelectMeshEdges ) )
			) 
			{
				result = true;
				break;
			}
		}
	}
	return result;
}

// serialization callbacks

static void saveSceneCallback( void * )
{	
	HairShape *hairShape = 0;
	if ( ( hairShape = HairShape::getActiveObject() ) != 0 )
	{	
		MObject thisNode = hairShape->asMObject();	
		MPlug dataPlug( thisNode, HairShape::serializedDataAttr );
		dataPlug.setString( hairShape->serialize().c_str() );
	}		
}

static void loadSceneCallback( void * )
{
	HairShape *hairShape = 0;
	if ( ( hairShape = HairShape::getActiveObject() ) != 0 )
	{
		MObject thisNode = hairShape->asMObject();	
		MPlug dataPlug( thisNode, HairShape::serializedDataAttr );
		hairShape->deserialize( dataPlug.asString().asChar() );
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
		surfaceChangeAttr = nAttr.create("surface_change", "srfc", MFnNumericData::kInt, 0 );
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
		// I have to check time attr all the time
		timeChangeAttr = nAttr.create("time_change", "tmfc", MFnNumericData::kInt, 0 );
		nAttr.setHidden( true );
		if( !addAttribute( timeChangeAttr ) )
		{
			status.perror( "Adding time attr has failed" );
			return status;
		}
		attributeAffects( timeAttr, timeChangeAttr );
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
		addIntAttribute( "displayed_hair_count", "dhc", genDisplayCountAttr, 1000, 1, 10000, 1, 10000 );
		//define display guides attribute
		addBoolAttribute( "display_guides", "digu", displayGuidesAttr, true );
		//define display interpolated hair attribute
		addBoolAttribute( "display_hair", "diha", displayInterpolatedAttr, false );
		//define number of samples in one dimension of texture
		addIntAttribute( "texture_dimension", "txtdm", sampleTextureDimensionAttr, 128, 1, 4096, 32, 1024);

		// shape components influence the surface as a whole
		attributeAffects( mControlPoints, surfaceAttr );
		attributeAffects( mControlValueX, surfaceAttr );
		attributeAffects( mControlValueY, surfaceAttr );
		attributeAffects( mControlValueZ, surfaceAttr );

		//serialized data attribute
		MFnTypedAttribute sAttr;
		serializedDataAttr = sAttr.create( "serialized_data", "sdata", MFnData::kString, MObject::kNullObj, &status );
		sAttr.setHidden( true );
		sAttr.setInternal( true );
		sAttr.setWritable( true );
		sAttr.setStorable( true );
		if ( !addAttribute( serializedDataAttr ) )
		{
			status.perror( "Adding serialized data attr has failed" );
			return status;
		}

		// register serialization callbacks		
		MSceneMessage::addCallback( MSceneMessage::kAfterOpen, loadSceneCallback, 0, &status );		
		MSceneMessage::addCallback( MSceneMessage::kAfterImport, loadSceneCallback, 0, &status );		
		
		MSceneMessage::addCallback( MSceneMessage::kBeforeExport, saveSceneCallback, 0, &status );		
		MSceneMessage::addCallback( MSceneMessage::kBeforeSave, saveSceneCallback, 0, &status );
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
		mVoxelization = new Voxelization( mMayaMesh->getRestPose(), getDensityTexture(), mVoxelsResolution );
	}
	mVoxelization->updateVoxels( *mMayaMesh, *this, mGeneratedHairCount );
	// For every voxel
	for ( unsigned __int32 i = 0; i < mVoxelization->getVoxelsCount(); ++i )
	{
		if ( mVoxelization->getVoxelHairCount( i ) > 0 )
		{
			// Open file 
			std::ostringstream voxelFileName; aFileName;
			voxelFileName << aFileName << ".VX" << aVoxelBoundingBoxes.size();
			std::ofstream voxelFile( voxelFileName.str().c_str(), ios::binary );
			zlib_stream::zip_ostream zipper( voxelFile, ios::out, false, COMPRESSION, 
				zlib_stream::StrategyFiltered, 15, 9, BUFFER_SIZE );
			// Write id
			zipper.write( VOXEL_FILE_ID, VOXEL_FILE_ID_SIZE );
			// Write voxel to file and stores voxel bounding box
			aVoxelBoundingBoxes.push_back( mVoxelization->exportVoxel( zipper, i ) );
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
	MayaHairProperties::refreshTextures(mSampleTextureDimension, densityChanged,
		interpolationGroupsChanged, hairPropertiesChanged );
	// React on refreshed textures
	if ( interpolationGroupsChanged )
	{
		updateSegmentsCountAttributes( false );
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

void HairShape::setSelectionModified( bool aFlag )
{
	mIsSelectionModified = aFlag;
}

bool HairShape::isSelectionModified()
{
	return mIsSelectionModified;
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
		// Updates segments attributes
		updateSegmentsCountAttributes( true );

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
		// Set interpolated params scaling
		setScaleFactor( mMayaMesh->getRestPose().getBoundingBox().diagonal() );

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
			mHairGuides->meshUpdate( *mMayaMesh, *mInterpolationGroups, true );
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
			mHairGuides->meshUpdate( *mMayaMesh, *mInterpolationGroups, false );
			// Interpolated hair positions recalculation
			if ( mDisplayInterpolated )
			{
				mInterpolatedHair.meshUpdate( *mMayaMesh, *this );
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

// Inline, but only called inside HairGuides.cpp
inline void HairShape::updateSegmentsCountAttributes( bool aFirstUpdate )
{
	// Remove attr value from storage
	setSegmentsCountAttr( MObject() );
	MFnDependencyNode node( thisMObject() );
	if ( !aFirstUpdate )
	{
		// Removes old attribute
		node.removeAttribute( node.findPlug( "segments_count" ).attribute() );
		node.removeAttribute( node.findPlug( "interpolation_groups_colors" ).attribute() );
	}
	// Creates new compound attribute
	MStatus s;
	MObject attr;
	MFnCompoundAttribute nAttr;
	attr = nAttr.create( "segments_count", "sgc", &s );
	nAttr.setInternal( true );
	// Add children -> counts
	fillIntArrayAttributes( attr, mInterpolationGroups->getGroupsCount(), 5, 1, 100, 1, 100 );
	// Add attr
	s = node.addAttribute( attr );
	// Sets children values
	for( unsigned __int32 i = 0; i < mInterpolationGroups->getGroupsCount(); ++i )
	{
		MPlug plug( thisMObject(), nAttr.child( i, &s ) ); 
		s = plug.setInt( static_cast< int >( mInterpolationGroups->getGroupSegmentsCount( i ) ) );
	}
	// Finally remember attr value
	setSegmentsCountAttr( attr );
	// Creates colors
	attr = nAttr.create( "interpolation_groups_colors", "igc", &s );
	nAttr.setWritable( false );
	// Add children -> colors
	fillColorArrayAttributes( attr, *mInterpolationGroups );
	// Add attr
	s = node.addAttribute( attr );
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

/*TODO Test and replace with proper documentation*/
bool HairShape::value( int pntInd, int vlInd, double & val ) const
//
// Description
//
//	  Helper function to return the value of a given vertex
//    from the mHairGuides.
//
{
	bool result = false;

	HairShape* nonConstThis = (HairShape*)this;
	HairComponents::HairGuides* hairGuidesPtr = nonConstThis->mHairGuides;
	if ( NULL != hairGuidesPtr )
	{
		HairComponents::OneGuideSegments guideSegments = hairGuidesPtr->getCurrentFrameSegments().mSegments.at( pntInd );
		unsigned int segmentCount = guideSegments.mSegments.size();
		Vector3D< Real > point3d = guideSegments.mSegments.at( segmentCount - 1 );
		val = point3d[ vlInd ];
		result = true;
	}

	return result;
}

bool HairShape::value( int pntInd, MPoint & val ) const
//
// Description
//
//	  Helper function to return the value of a given vertex
//    from the cachedMesh.
//
{
	bool result = false;

	HairShape* nonConstThis = (HairShape*)this;
	HairComponents::HairGuides* hairGuidesPtr = nonConstThis->mHairGuides;
	if ( NULL != hairGuidesPtr )
	{
		HairComponents::OneGuideSegments guideSegments = hairGuidesPtr->getCurrentFrameSegments().mSegments.at( pntInd );
		unsigned int segmentCount = guideSegments.mSegments.size();
		Vector3D< Real > point3d = guideSegments.mSegments.at( segmentCount - 1 );
		/*TODO insert support for world matrix transformations - create an interface from hairGuidesPtr */
		//val = point3d.transformAsPoint( hairGuidesPtr->mCurrentPositions.at( pntInd ).mWorldTransformMatrix ).toMayaPoint();
		val = point3d.toMayaPoint();
		result = true;
	}

	return result;
}

bool HairShape::setValue( int pntInd, int vlInd, double val )
//
// Description
//
//	  Helper function to set the value of a given vertex
//    in the cachedMesh.
//
{
	bool result = false;

	HairShape* nonConstThis = (HairShape*)this;
	HairComponents::HairGuides* hairGuidesPtr = nonConstThis->mHairGuides;
	if ( NULL != hairGuidesPtr )
	{
		HairComponents::OneGuideSegments guideSegments = hairGuidesPtr->getCurrentFrameSegments().mSegments.at( pntInd );
		unsigned int segmentCount = guideSegments.mSegments.size();
		Vector3D< Real >& point3d = guideSegments.mSegments.at( segmentCount - 1 );
		point3d[ vlInd ] = val;
		result = true;
	}

	childChanged( MPxSurfaceShape::kBoundingBoxChanged );
	childChanged( MPxSurfaceShape::kObjectChanged );

	return result;
}

bool HairShape::setValue( int pntInd, const MPoint & val )
//
// Description
//
//	  Helper function to set the value of a given vertex
//    in the cachedMesh.
//
{
	bool result = false;

	HairShape* nonConstThis = (HairShape*)this;
	HairComponents::HairGuides* hairGuidesPtr = nonConstThis->mHairGuides;
	if ( NULL != hairGuidesPtr )
	{
		HairComponents::OneGuideSegments guideSegments = hairGuidesPtr->getCurrentFrameSegments().mSegments.at( pntInd );
		unsigned int segmentCount = guideSegments.mSegments.size();
		Vector3D< Real >& point3d = guideSegments.mSegments.at( segmentCount - 1 );
		point3d = val;
		result = true;
	}

	childChanged( MPxSurfaceShape::kBoundingBoxChanged );
	childChanged( MPxSurfaceShape::kObjectChanged );

	return result;
}


} // namespace HairShape

} // namespace Stubble