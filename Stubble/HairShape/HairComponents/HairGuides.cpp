#include "HairGuides.hpp"

#include <maya/MFnNurbsCurve.h>
#include <maya/MPointArray.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MAnimControl.h>
#include <maya/MTime.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <cassert>

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

Real GUIDE_SIZE = 10.0f;  ///< Size of the guide before scaling for huge/small mesh

HairGuides::HairGuides():
	mSegmentsStorage( 0 ),
	mBoundingBoxDirtyFlag( false ),
	mNumberOfGuidesToInterpolateFrom( 3 )
{
}

HairGuides::~HairGuides()
{
	clearSelectedGuides();
	delete mSegmentsStorage;
}

bool HairGuides::applySelection( const std::vector< unsigned __int32 > & aInterpolationGroupsSelectable, MSelectInfo &aSelectInfo, MSelectionList &aSelectionList,  MPointArray &aWorldSpaceSelectPts )
{
	if ( mAllSegmentsUG.isDirty() ) // Is UG for selection up-to-date ?
	{
		mAllSegmentsUG.build( mCurrentPositions, mSegmentsStorage->getCurrentSegments() );
	}
	// Rebuild selected segments UG
	clearSelectedGuides();
	bool isAnythingSelected = ( mSelectedSegmentsUG.build(mCurrentPositions, mSegmentsStorage->getCurrentSegments(), this->guidesVerticesStartIndex(), this->mGuidesInterpolationGroupIds, aInterpolationGroupsSelectable, aSelectInfo, aSelectionList, aWorldSpaceSelectPts, mSelectedGuides) );
	// Display selection
	if ( mDisplayedGuides.isDirty() )
	{
		mDisplayedGuides.build( mCurrentPositions, mSegmentsStorage->getCurrentSegments(), mSelectedGuides );
	}
	else
	{
		mDisplayedGuides.selectionRebuild( mSelectedGuides, true );
	}

	return isAnythingSelected;
}

void HairGuides::applySelection( MIntArray &aSelectedComponentIndices )
{
	if ( mAllSegmentsUG.isDirty() ) // Is UG for selection up-to-date ?
	{
		mAllSegmentsUG.build( mCurrentPositions, mSegmentsStorage->getCurrentSegments() );
	}
	// Rebuild selected segments UG
	clearSelectedGuides();
	mSelectedSegmentsUG.build(mCurrentPositions, mSegmentsStorage->getCurrentSegments(), this->guidesVerticesStartIndex(), aSelectedComponentIndices, mSelectedGuides);
	// Display selection
	if ( mDisplayedGuides.isDirty() )
	{
		mDisplayedGuides.build( mCurrentPositions, mSegmentsStorage->getCurrentSegments(), mSelectedGuides );
	}
	else
	{
		mDisplayedGuides.selectionRebuild( mSelectedGuides, true );
	}
}

BoundingBox HairGuides::getBoundingBox()
{
	if ( mBoundingBoxDirtyFlag && mSegmentsStorage )
	{
		mBoundingBox = mSegmentsStorage->getBoundingBox( mCurrentPositions );
		mBoundingBoxDirtyFlag = false;
	}
	return mBoundingBox;
}

const SegmentsUG & HairGuides::getSelectedGuidesDS()
{
	if ( mAllSegmentsUG.isDirty() ) // Is UG for selection up-to-date ?
	{
		mSelectedSegmentsUG.build( mSelectedGuides, true );
	}
	return mSelectedSegmentsUG;
}

void HairGuides::updateGuides( bool aStoreUpdate )
{
	// Update selection
	mDisplayedGuides.selectionRebuild( mSelectedGuides, true );
	// Update bounding box
	// For every selected guide
	for( SelectedGuides::const_iterator it = mSelectedGuides.begin(); it != mSelectedGuides.end(); ++it )
	{
		if ( (*it)->mDirtyFlag )
		{
			// For every segment
			for ( Segments::const_iterator segIt = (*it)->mGuideSegments.mSegments.begin();
				segIt != (*it)->mGuideSegments.mSegments.end(); ++segIt )
			{
				// Transform to world and expand bbox
				mBoundingBox.expand( mCurrentPositions[ (*it)->mGuideId].mPosition.toWorld( *segIt ) );
			}
		}
	}
	if ( aStoreUpdate ) // Final update ? ( user has stop using brush, etc. )
	{
		// Propagate changes to all frames and update undo stack
		mUndoStack.add( mSegmentsStorage->propagateChanges( mSelectedGuides ) );
		// Set guides segments dirty
		mAllSegmentsUG.setDirty();
		mSelectedSegmentsUG.setDirty();
		// Set selected guides as non dirty
		// For every selected guide
		for( SelectedGuides::iterator it = mSelectedGuides.begin(); it != mSelectedGuides.end(); ++it )
		{
			(*it)->mDirtyFlag = false;

			// Clear additional info
			SegmentsAdditionalInfo::iterator nfoIt;
			for (nfoIt = (*it)->mSegmentsAdditionalInfo.begin(); nfoIt != (*it)->mSegmentsAdditionalInfo.end(); ++nfoIt)
			{
				nfoIt->mClosestPointOnMesh.set(0.0, 0.0, 0.0);
				nfoIt->mFallOff = 0.0;
				nfoIt->mInsideBrush = false;
				nfoIt->mIsColliding = false;
			}
		}
	}
}

void HairGuides::reinitCuttedHair( Real aScaleFactor )
{
	// Propagate changes to all frames and update undo stack
	mUndoStack.add( mSegmentsStorage->reinitCuttedHair( aScaleFactor * GUIDE_SIZE ) );
	// Segments has changed...
	mDisplayedGuides.setDirty();
	mAllSegmentsUG.setDirty();
	updateSelectedGuides();
	mBoundingBoxDirtyFlag = true;
}

void HairGuides::resetGuides( Real aScaleFactor )
{
	// Propagate changes to all frames and update undo stack
	mUndoStack.add( mSegmentsStorage->resetGuides( aScaleFactor * GUIDE_SIZE ) );
	// Segments has changed...
	mDisplayedGuides.setDirty();
	mAllSegmentsUG.setDirty();
	updateSelectedGuides();
	mBoundingBoxDirtyFlag = true;
}

const RestPositionsDS & HairGuides::getGuidesPositionsDS( const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	if ( mRestPositionsDS.isDirty() ) // Is segments UG up-to-date ?
	{
		mRestPositionsDS.build( mRestPositions, aInterpolationGroups );
	}
	return mRestPositionsDS;
}

void HairGuides::draw( bool aDrawVerts )
{
	if ( mDisplayedGuides.isDirty() ) // Is display list up-to-date ?
	{
		mDisplayedGuides.build( mCurrentPositions, mSegmentsStorage->getCurrentSegments(), mSelectedGuides );
	}
	mDisplayedGuides.draw( aDrawVerts );
}

void HairGuides::importNURBS( const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	if ( mSegmentsStorage->imported() )
	{
		throw StubbleException( " HairGuides::importToNURBS : already used import command ! " );
	}
	// For every time frame
	for ( MTime time = MAnimControl::minTime(); time <= MAnimControl::maxTime(); time++ )
	{
		// Set current time ( Maya )
		MGlobal::viewFrame( time );	
		FrameSegments frameSegments;
		// Set current time ( internal )
		frameSegments.mFrame = time.as( MTime::uiUnit() );
		frameSegments.mSegments.resize( mCurrentPositions.size() );
		GuidesSegments::iterator it = frameSegments.mSegments.begin();
		// Select positions
		GuidesCurrentPositions::const_iterator currIt = mCurrentPositions.begin();
		// For every curve
		for ( unsigned __int32 nIt = 0; nIt < mNurbsCurvesNames.length(); ++nIt, ++currIt, ++it )
		{
			MStatus status = MGlobal::selectByName( mNurbsCurvesNames[ nIt ], MGlobal::kReplaceList );
			MSelectionList sel;
			MGlobal::getActiveSelectionList( sel );
			if ( sel.length() != 1 ) // Nothing selected
			{
				throw StubbleException(( " Could not load curve " + mNurbsCurvesNames[ nIt ] ).asChar() );
			}
			MItSelectionList iterator( sel, MFn::kNurbsCurve );
			MDagPath path;
			iterator.getDagPath( path ); // Selected object dag path
			MFnDependencyNode oldCurve( path.node(), &status ); // Select old curve
			MPlug plug = oldCurve.findPlug( "worldSpace", &status ); // Select oldCurve.worldSpace
			MPlug pl = plug.elementByPhysicalIndex( 0, &status ); // Select oldCurve.worldSpace[ 0 ]
			MPlugArray arr;
			pl.connectedTo( arr, false, true, &status ); // Get from oldCurve.worldSpace[ 0 ] to folicul.startPosition
			MFnDependencyNode folicul( arr[ 0 ].node(), &status ); // Get folicul
			MPlug outCurvePlug = folicul.findPlug( "outCurve", &status ); // Select folicul.outCurve
			outCurvePlug.connectedTo( arr, false, true, &status ); // Get from folicul.outCurve to curve.create
			MFnNurbsCurve curve( arr[ 0 ].node(), &status ); // Finally get curve
			// Prepare space for segments
			it->mSegments.resize( aInterpolationGroups.getSegmentsCount( currIt->mPosition.getUCoordinate(), 
				currIt->mPosition.getVCoordinate() ) + 1 );
			double param = 0, step = curve.length() / ( it->mSegments.size() - 1 );
			for ( Segments::iterator segIt = it->mSegments.begin(); segIt != it->mSegments.end(); 
				++segIt, param += step )
			{
				MPoint curvePoint;
				curve.getPointAtParam( curve.findParamFromLength( param ), curvePoint, MSpace::kWorld );
				*segIt = Vector3D< Real >::transformPoint( curvePoint, currIt->mLocalTransformMatrix );
			}
			it->mSegmentLength = static_cast< Real >( step );
			// Recalculate positions
			SegmentsStorage::uniformlyRepositionSegments( *it, 
				static_cast< unsigned __int32 >( it->mSegments.size() ) );
		}
		// Uniformly reposition segments
		#pragma omp parallel for schedule( guided )
		for ( int i = 0; i < static_cast< int >( frameSegments.mSegments.size() ); ++i )
		{
			OneGuideSegments & guide = frameSegments.mSegments[ i ];
			SegmentsStorage::uniformlyRepositionSegments( guide,
				static_cast< unsigned __int32 >( guide.mSegments.size() ) );
		}
		mSegmentsStorage->importSegments( frameSegments );		
	}
	// Refresh interpolation groups ids
	refreshInterpolationGroupIds( aInterpolationGroups );
	// All nurbs has been imported, set time back
	MAnimControl::setCurrentTime( MAnimControl::minTime() );
	// Clear curves names
	mNurbsCurvesNames.clear();
}

void HairGuides::exportToNURBS()
{
	if ( mSegmentsStorage->imported() )
	{
		throw StubbleException( " HairGuides::exportToNURBS : already used import command ! " );
	}
	// Select guides
	const GuidesSegments & guides = mSegmentsStorage->getCurrentSegments().mSegments;
	// Select positions
	GuidesCurrentPositions::const_iterator currIt = mCurrentPositions.begin();
	// For all current hair
	mNurbsCurvesNames.clear();
	for ( GuidesSegments::const_iterator hairIt = guides.begin(); 
		hairIt != guides.end(); ++hairIt, ++currIt )
	{		
		MPointArray pointArray;			
		for ( Segments::const_iterator segmentIt = hairIt->mSegments.begin()
			; segmentIt != hairIt->mSegments.end()
			; ++segmentIt )
		{			
			// Transform to world coordinates and append
			pointArray.append( currIt->mPosition.toWorld( *segmentIt ).toMayaPoint() );
		}
		MFnNurbsCurve nurbsCurve;
		MStatus status;
		nurbsCurve.createWithEditPoints( pointArray, 1, MFnNurbsCurve::kOpen, 
			false, false, true, MObject::kNullObj, &status );
		MString name = nurbsCurve.name();
		mNurbsCurvesNames.append( nurbsCurve.name() );
		if ( status != MStatus::kSuccess )
		{
			throw StubbleException( " HairGuides::exportNURBS : Failed to create NURBS curve. " );
		}
	}
	// After successfull export, select all curves
	MSelectionList sel;
	// Add all curves to selection list
	for ( unsigned __int32 nIt = 0; nIt < mNurbsCurvesNames.length(); ++nIt )
	{
		sel.add( mNurbsCurvesNames[ nIt ] );
	}
	// Execute selection
	MGlobal::setActiveSelectionList( sel );
}

void HairGuides::setCurrentTime( Time aTime )
{
	if ( !mSegmentsStorage->imported() ) // Same guides for all frames ?
	{
		return; // Time change has no meaning
	}
	mSegmentsStorage->setFrame( aTime );
	// Everything has changed...
	mDisplayedGuides.setDirty();
	mRestPositionsDS.setDirty();
	mAllSegmentsUG.setDirty();
	mUndoStack.clear();
	updateSelectedGuides();
	mBoundingBoxDirtyFlag = true;
}

void HairGuides::meshUpdate( const MayaMesh & aMayaMesh, const Interpolation::InterpolationGroups & aInterpolationGroups,
	bool aTopologyChanged )
{
	if ( aTopologyChanged )
	{
		const MeshUVCoordUG & positionConverter = aMayaMesh.getMeshUVCoordUG();
		// Temporary hair guides
		SegmentsStorage * tmpSegmentsStorage;
		GuidesRestPositions tmpRestPositions;
		tmpRestPositions.reserve( mRestPositions.size() ); // Optimalization
		mCurrentPositions.clear();
		mCurrentPositions.reserve( mRestPositions.size() ); // Optimalization
		GuidesIds remainingGuides; // Stores ids of remaining guides
		remainingGuides.reserve( mRestPositions.size() ); // Optimalization
		// Calculate new positions
		for( GuidesRestPositions::iterator restPosIt = mRestPositions.begin(); restPosIt != mRestPositions.end(); 
			++restPosIt )
		{
			GuideRestPosition restPos;
			restPos.mUVPoint = positionConverter.getUVPoint( restPosIt->mPosition );
			if ( restPos.mUVPoint.getTriangleID() != UVPoint::NOT_TRIANGLE ) // Topology change did not destroy the guide
			{
				// Handle rest position
				restPos.mPosition = aMayaMesh.getRestPose().getIncompleteMeshPoint( restPos.mUVPoint );
				tmpRestPositions.push_back( restPos );
				// Handle current position
				GuideCurrentPosition currPos;
				currPos.mPosition = aMayaMesh.getMeshPoint( restPos.mUVPoint );
				currPos.mPosition.getLocalTransformMatrix( currPos.mLocalTransformMatrix );
				currPos.mPosition.getWorldTransformMatrix( currPos.mWorldTransformMatrix );
				mCurrentPositions.push_back( currPos );
				// Remember id
				remainingGuides.push_back( static_cast< GuideId >( restPosIt - mRestPositions.begin() ) );
			}
		}
		// Copy old segments
		tmpSegmentsStorage = new SegmentsStorage( *mSegmentsStorage, remainingGuides );
		// Now we can throw away old data
		std::swap( tmpRestPositions, mRestPositions );
		std::swap( tmpSegmentsStorage, mSegmentsStorage );
		delete tmpSegmentsStorage;
		// Rest positions has changed...
		refreshInterpolationGroupIds( aInterpolationGroups );
		mRestPositionsDS.setDirty();
		clearSelectedGuides();
	}
	else
	{
		// Update current positions
		GuidesCurrentPositions::iterator currPosIt = mCurrentPositions.begin();
		for( GuidesRestPositions::iterator restPosIt = mRestPositions.begin(); restPosIt != mRestPositions.end(); 
			++restPosIt, ++currPosIt )
		{
			currPosIt->mPosition = aMayaMesh.getMeshPoint( restPosIt->mUVPoint );
			currPosIt->mPosition.getLocalTransformMatrix( currPosIt->mLocalTransformMatrix );
			currPosIt->mPosition.getWorldTransformMatrix( currPosIt->mWorldTransformMatrix );
		}
		updateSelectedGuides();
	}
	// Current positions has changed...
	mDisplayedGuides.setDirty();
	mAllSegmentsUG.setDirty();
	mUndoStack.clear();
	mBoundingBoxDirtyFlag = true;
}

void HairGuides::undo()
{
	bool noRedo = mUndoStack.canRedo();
	if ( !mUndoStack.canUndo() )
	{
		return;
	}
	mUndoStack.updateAfterUndo( mSegmentsStorage->replace( mUndoStack.undo() ) );
	// Segments has changed...
	mDisplayedGuides.setDirty();
	mAllSegmentsUG.setDirty();
	updateSelectedGuides();
	mBoundingBoxDirtyFlag = true;
}

void HairGuides::redo()
{
	if ( !mUndoStack.canRedo() )
	{
		return;
	}
	mUndoStack.updateAfterRedo( mSegmentsStorage->replace( mUndoStack.redo() ) );
	// Segments has changed...
	mDisplayedGuides.setDirty();
	mAllSegmentsUG.setDirty();
	updateSelectedGuides();
	mBoundingBoxDirtyFlag = true;
}

void HairGuides::emptyHistoryStack()
{
	mUndoStack.clear();
}

void HairGuides::generate( UVPointGenerator & aUVPointGenerator, const MayaMesh & aMayaMesh, 
	const Interpolation::InterpolationGroups & aInterpolationGroups, unsigned __int32 aCount, 
	Real aScaleFactor, bool aInterpolateFromPrevious )
{
	// Temporary hair guides
	SegmentsStorage * tmpSegmentsStorage;
	GuidesRestPositions tmpRestPositions;
	tmpRestPositions.resize( aCount );
	mCurrentPositions.clear();
	mCurrentPositions.resize( aCount );
	// Start generating positions
	GuidesRestPositions::iterator restPosIt = tmpRestPositions.begin();
	GuidesCurrentPositions::iterator currPosIt = mCurrentPositions.begin();
	aUVPointGenerator.reset();
	for ( unsigned __int32 i = 0; i < aCount; ++i, ++restPosIt, ++currPosIt )
	{
		// Generate rest pose position
		restPosIt->mUVPoint = aUVPointGenerator.next();
		restPosIt->mPosition = aMayaMesh.getRestPose().getIncompleteMeshPoint( restPosIt->mUVPoint );
		// Calculate current position
		currPosIt->mPosition = aMayaMesh.getMeshPoint( restPosIt->mUVPoint );
		currPosIt->mPosition.getLocalTransformMatrix( currPosIt->mLocalTransformMatrix );
		currPosIt->mPosition.getWorldTransformMatrix( currPosIt->mWorldTransformMatrix );
	}
	// Now we can create new segments
	if ( aInterpolateFromPrevious )
	{
		if ( mSegmentsStorage == 0 )
		{
			throw StubbleException(" HairGuides::generate : No old segments to interpolate from ! ");
		}
		tmpSegmentsStorage = new SegmentsStorage( *mSegmentsStorage, getGuidesPositionsDS( aInterpolationGroups ), 
			tmpRestPositions, aInterpolationGroups, mNumberOfGuidesToInterpolateFrom );
	}
	else
	{
		tmpSegmentsStorage = new SegmentsStorage( tmpRestPositions, aInterpolationGroups, 
			aScaleFactor * GUIDE_SIZE );
	}
	// Now we can throw away old data
	std::swap( tmpRestPositions, mRestPositions );
	std::swap( tmpSegmentsStorage, mSegmentsStorage );
	delete tmpSegmentsStorage;
	// Everything has changed...
	refreshInterpolationGroupIds( aInterpolationGroups );
	mUndoStack.clear();
	mDisplayedGuides.setDirty();
	mRestPositionsDS.setDirty();
	mAllSegmentsUG.setDirty();
	clearSelectedGuides();
	mBoundingBoxDirtyFlag = true;
}

void HairGuides::updateSegmentsCount( const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	refreshInterpolationGroupIds( aInterpolationGroups );
	mSegmentsStorage->setSegmentsCount( mRestPositions, aInterpolationGroups );
	// Segments has changed...
	mUndoStack.clear();
	mDisplayedGuides.setDirty();
	mRestPositionsDS.setDirty(); // Interpolation groups may have changed
	mAllSegmentsUG.setDirty();
	clearSelectedGuides();
	mBoundingBoxDirtyFlag = true;
}

void HairGuides::clearSelectedGuides()
{
	for ( SelectedGuides::iterator guideIt = mSelectedGuides.begin();
		guideIt != mSelectedGuides.end(); ++guideIt )
	{
		delete *guideIt;
	}
	mSelectedGuides.clear();
	mSelectedSegmentsUG.setDirty();
}

void HairGuides::serialize( std::ostream & aOutputStream ) const
{
	// Convert curvesNames to vector of strings
	std::vector< std::string > curvesNames;		
	for ( unsigned int i = 0; i < mNurbsCurvesNames.length(); i++ )
	{
		curvesNames.push_back( mNurbsCurvesNames[ i ].asChar() );
	}
	// Serialize object
	mSegmentsStorage->serialize( aOutputStream );
	serializeObjects( mRestPositions, aOutputStream );
	serializePrimitives( curvesNames, aOutputStream );
}

void HairGuides::deserialize( const MayaMesh & aMayaMesh, const Interpolation::InterpolationGroups & aInterpolationGroups,
		std::istream & aInputStream )
{	
	mSegmentsStorage->deserialize( aInputStream );
	deserializeObjects( mRestPositions, aInputStream );
	std::vector< std::string > curvesNames;
	deserializePrimitives( curvesNames, aInputStream );
	// Convert curvesNames back to maya representation
	mNurbsCurvesNames.clear();
	for ( std::vector< std::string >::const_iterator it = curvesNames.begin(); it != curvesNames.end(); it++ )
	{
		mNurbsCurvesNames.append( it->c_str() );
	}
	// Generate hair guides current positions
	mCurrentPositions.clear();
	mCurrentPositions.reserve( mRestPositions.size() );
	for( GuidesRestPositions::iterator restPosIt = mRestPositions.begin(); restPosIt != mRestPositions.end(); 
		++restPosIt )
	{		
		GuideCurrentPosition currPos;			
		currPos.mPosition = aMayaMesh.getMeshPoint( restPosIt->mUVPoint );
		currPos.mPosition.getLocalTransformMatrix( currPos.mLocalTransformMatrix );
		currPos.mPosition.getWorldTransformMatrix( currPos.mWorldTransformMatrix );
		mCurrentPositions.push_back( currPos );				
	}
	// Set dirty to all internal data
	refreshInterpolationGroupIds( aInterpolationGroups );
	mRestPositionsDS.setDirty();
	mDisplayedGuides.setDirty();
	mAllSegmentsUG.setDirty();
	mUndoStack.clear();
	mBoundingBoxDirtyFlag = true;
}

void HairGuides::updateSelectedGuides()
{
	const GuidesSegments & guideSegments = mSegmentsStorage->getCurrentSegments().mSegments;
	for ( SelectedGuides::iterator guideIt = mSelectedGuides.begin();
		guideIt != mSelectedGuides.end(); ++guideIt )
	{
		SelectedGuide & guide = **guideIt;
		// Updates segments info
		guide.mGuideSegments = guideSegments[ guide.mGuideId ];
		// Update position info
		const GuideCurrentPosition & pos = mCurrentPositions[ guide.mGuideId ];
		guide.mNormal = pos.mPosition.getNormal();
		guide.mPosition = pos;
		// Update flags
		guide.mDirtyFlag = true;
		guide.mDirtyRedrawFlag = true;
	} // for each guide
	// We will not update all segments ug, it will be updated when necessary
	// for each guide, we will only set selected segments as dirty
	mSelectedSegmentsUG.setDirty();
}

void HairGuides::refreshInterpolationGroupIds( const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	unsigned __int32 lastId = 0;
	// Realloc arrays
	mGuidesVerticesStartIndex.resize( mRestPositions.size() );
	mGuidesInterpolationGroupIds.resize( mRestPositions.size() );
	Indices::iterator index = mGuidesVerticesStartIndex.begin();
	Indices::iterator groupId = mGuidesInterpolationGroupIds.begin();
	// For every guide
	for ( GuidesRestPositions::const_iterator posIt = mRestPositions.begin(); posIt != mRestPositions.end();
		++posIt, ++index, ++groupId )
	{
		*groupId = aInterpolationGroups.getGroupId( posIt->mPosition.getUCoordinate(), posIt->mPosition.getVCoordinate() );
		*index = lastId;
		lastId += aInterpolationGroups.getGroupSegmentsCount( *groupId ) + 1; // i.e. 5 segments => 6 vertices
	}
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble