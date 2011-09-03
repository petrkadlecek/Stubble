#include "HairGuides.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

HairGuides::HairGuides():
	mSegmentsStorage( 0 ),
	mBoundingBoxDirtyFlag( false )
{
}

HairGuides::~HairGuides()
{
	delete mSegmentsStorage;
}

void HairGuides::applySelection( const SelectionMask & aSelectionMask )
{
	if ( mAllSegmentsUG.isDirty() ) // Is UG for selection up-to-date ?
	{
		mAllSegmentsUG.build( mCurrentPositions, mSegmentsStorage->getCurrentSegments() );
	}
	// Hide old selection
	mDisplayedGuides.selectionRebuild( mSelectedGuides, false );
	// Apply selection
	mAllSegmentsUG.select( aSelectionMask, mSelectedGuides );
	// Rebuild selected segments UG
	mSelectedSegmentsUG.build( mSelectedGuides, true );
	// Display selection
	mDisplayedGuides.selectionRebuild( mSelectedGuides, true );
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

const SegmentsUG & HairGuides::getSelectedGuidesUG()
{
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
		if ( it->mDirtyFlag )
		{
			// For every segment
			for ( Segments::const_iterator segIt = it->mSegments.mSegments.begin();
				segIt != it->mSegments.mSegments.end(); ++segIt )
			{
				// Transform to world and expand bbox
				mBoundingBox.expand( mCurrentPositions[ it->mGuideId].mPosition.toWorld( *segIt ) );
			}
		}
	}
	if ( aStoreUpdate ) // Final update ? ( user has stop using brush, etc. )
	{
		// Propagate changes to all frames and update undo stack
		mUndoStack.add( mSegmentsStorage->propagateChanges( mSelectedGuides ) );
		// Set guides segments dirty
		mAllSegmentsUG.setDirty();
	}
	// Set selected guides as non dirty
	// For every selected guide
	for( SelectedGuides::iterator it = mSelectedGuides.begin(); it != mSelectedGuides.end(); ++it )
	{
		it->mDirtyFlag = false;
	}
}

const RestPositionsUG & HairGuides::getGuidesPositionsUG( const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	if ( mRestPositionsUG.isDirty() ) // Is segments UG up-to-date ?
	{
		mRestPositionsUG.build( mRestPositions, aInterpolationGroups );
	}
	return mRestPositionsUG;
}

void HairGuides::draw()
{
	if ( mDisplayedGuides.isDirty() ) // Is display list up-to-date ?
	{
		mDisplayedGuides.build( mCurrentPositions, mSegmentsStorage->getCurrentSegments(), mSelectedGuides );
	}
	mDisplayedGuides.draw();
}

void HairGuides::importNURBS()
{
	MSelectionList selection;
	MGlobal::getActiveSelectionList( selection );	
	MItSelectionList curveIt( selection, MFn::kNurbsCurve );	
	for ( ; !curveIt.isDone(); curveIt.next() )
	{
		MDagPath path;
		curveIt.getDagPath( path );
		MFnNurbsCurve curve( path );
		// got the curve, TODO: get it into segment storage
	}
	mUndoStack.clear();
	mDisplayedGuides.setDirty();
	mRestPositionsUG.setDirty();
	mAllSegmentsUG.setDirty();
	mBoundingBoxDirtyFlag = true;
}

void HairGuides::exportNURBS()
{
	for ( SelectedGuides::const_iterator hairIt = mSelectedGuides.begin(); hairIt != mSelectedGuides.end(); hairIt++ )
	{
		MPointArray pointArray;				
		for ( Segments::const_iterator segmentIt = hairIt->mSegments.mSegments.begin()
			; segmentIt != hairIt->mSegments.mSegments.end()
			; segmentIt++ )
		{			
			pointArray.append( segmentIt->toMayaPoint() );
		}
		MFnNurbsCurve nurbsCurve;
		MStatus status;

		nurbsCurve.createWithEditPoints( pointArray, 1, MFnNurbsCurve::kOpen, false, false, true, MObject::kNullObj, &status );
		if ( status != MStatus::kSuccess )
		{
			status.perror( "HairGuides: Failed to create NURBS curve." );			
		}
	}
}

void HairGuides::setCurrentTime( Time aTime )
{
	if ( !mSegmentsStorage->imported() ) // Same guides for all frames ?
	{
		return; // Time change has no meaning
	}
	// Everything has changed...
	mDisplayedGuides.setDirty();
	mRestPositionsUG.setDirty();
	mAllSegmentsUG.setDirty();
	mUndoStack.clear();
	mBoundingBoxDirtyFlag = true;
}

GuideId HairGuides::meshUpdate( const MayaMesh & aMayaMesh, bool aTopologyChanged )
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
			if ( restPos.mUVPoint.getTriangleID() >= 0 ) // Topology change did not destroy the guide
			{
				// Handle rest position
				restPos.mPosition = aMayaMesh.getRestPose().getMeshPoint( restPos.mUVPoint );
				tmpRestPositions.push_back( restPos );
				// Handle current position
				GuideCurrentPosition currPos;
				currPos.mPosition = aMayaMesh.getMeshPoint( restPosIt->mUVPoint );
				currPos.mPosition.getLocalTransformMatrix( currPos.mLocalTransformMatrix );
				currPos.mPosition.getWorldTransformMatrix( currPos.mWorldTransformMatrix );
				mCurrentPositions.push_back( GuideCurrentPosition() );
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
		mRestPositionsUG.setDirty();
	}
	// Update current positions
	GuidesCurrentPositions::iterator currPosIt = mCurrentPositions.begin();
	for( GuidesRestPositions::iterator restPosIt = mRestPositions.begin(); restPosIt != mRestPositions.end(); 
		++restPosIt, ++currPosIt )
	{
		currPosIt->mPosition = aMayaMesh.getMeshPoint( restPosIt->mUVPoint );
		currPosIt->mPosition.getLocalTransformMatrix( currPosIt->mLocalTransformMatrix );
		currPosIt->mPosition.getWorldTransformMatrix( currPosIt->mWorldTransformMatrix );
	}
	// Current positions has changed...
	mDisplayedGuides.setDirty();
	mAllSegmentsUG.setDirty();
	mUndoStack.clear();
	mBoundingBoxDirtyFlag = true;
	return static_cast< GuideId >( mRestPositions.size() );
}

void HairGuides::undo()
{
	if ( !mUndoStack.canUndo() )
	{
		return;
	}
	mSegmentsStorage->replace( mUndoStack.undo() );
	// Segments has changed...
	mDisplayedGuides.setDirty();
	mAllSegmentsUG.setDirty();
	mBoundingBoxDirtyFlag = true;
}

void HairGuides::redo()
{
	if ( !mUndoStack.canRedo() )
	{
		return;
	}
	mSegmentsStorage->replace( mUndoStack.redo() );
	// Segments has changed...
	mDisplayedGuides.setDirty();
	mAllSegmentsUG.setDirty();
	mBoundingBoxDirtyFlag = true;
}

void HairGuides::emptyHistoryStack()
{
	mUndoStack.clear();
}

void HairGuides::generate( UVPointGenerator & aUVPointGenerator, const MayaMesh & aMayaMesh, 
	const Interpolation::InterpolationGroups & aInterpolationGroups, unsigned __int32 aCount, 
	bool aInterpolateFromPrevious )
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
	for ( unsigned __int32 i = 0; i < aCount; ++i, ++restPosIt, ++currPosIt )
	{
		// Generate rest pose position
		restPosIt->mUVPoint = aUVPointGenerator.next();
		restPosIt->mPosition = aMayaMesh.getRestPose().getMeshPoint( restPosIt->mUVPoint );
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
		tmpSegmentsStorage = new SegmentsStorage( *mSegmentsStorage, getGuidesPositionsUG( aInterpolationGroups ), 
			tmpRestPositions, aInterpolationGroups );
	}
	else
	{
		tmpSegmentsStorage = new SegmentsStorage( tmpRestPositions, aInterpolationGroups );
	}
	// Now we can throw away old data
	std::swap( tmpRestPositions, mRestPositions );
	std::swap( tmpSegmentsStorage, mSegmentsStorage );
	delete tmpSegmentsStorage;
	// Everything has changed...
	mUndoStack.clear();
	mDisplayedGuides.setDirty();
	mRestPositionsUG.setDirty();
	mAllSegmentsUG.setDirty();
	mBoundingBoxDirtyFlag = true;
}

void HairGuides::updateSegmentsCount( const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	mSegmentsStorage->setSegmentsCount( mRestPositions, aInterpolationGroups );
	// Segments has changed...
	mUndoStack.clear();
	mDisplayedGuides.setDirty();
	mAllSegmentsUG.setDirty();
	mBoundingBoxDirtyFlag = true;
}

void HairGuides::exportToFile( std::ostream & aOutputStream ) const
{
	// First export rest positions
	unsigned __int32 size = static_cast< unsigned __int32 >( mRestPositions.size() );
	aOutputStream.write( reinterpret_cast< const char *>( &size ), sizeof( unsigned __int32 ) );
	for ( GuidesRestPositions::const_iterator it = mRestPositions.begin(); it != mRestPositions.end(); ++it )
	{
		aOutputStream << it->mPosition;
		aOutputStream << it->mUVPoint;
	}
	// Then export current segments of all guides
	const GuidesSegments & currentSegments = mSegmentsStorage->getCurrentSegments().mSegments;
	// Export guides count
	size = static_cast< unsigned __int32 >( currentSegments.size() );
	aOutputStream.write( reinterpret_cast< const char *>( &size ), sizeof( unsigned __int32 ) );
	// For each guide
	for ( GuidesSegments::const_iterator it = currentSegments.begin(); it != currentSegments.end(); ++it )
	{
		// Export vertices count
		size = static_cast< unsigned __int32 >( it->mSegments.size() );
		aOutputStream.write( reinterpret_cast< const char *>( &size ), sizeof( unsigned __int32 ) );
		// For each hair vertex
		for ( Segments::const_iterator segIt = it->mSegments.begin(); segIt != it->mSegments.end(); ++segIt )
		{
			aOutputStream << *segIt;
		}
	}
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble