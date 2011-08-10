#include "HairGuides.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

HairGuides::HairGuides():
	mSegmentsStorage( 0 )
{
}

HairGuides::~HairGuides()
{
	delete mSegmentsStorage;
}

void HairGuides::applySelection( const SelectionMask & aSelectionMask )
{
	if ( mSegmentsUG.isDirty() ) // Is UG for selection up-to-date ?
	{
		mSegmentsUG.build( mCurrentPositions, mSegmentsStorage->getCurrentSegments() );
	}
	// Hide old selection
	mDisplayedGuides.selectionRebuild( mSelectedGuides, false );
	// Apply selection
	mSegmentsUG.select( aSelectionMask, mSelectedGuides );
	// Display selection
	mDisplayedGuides.selectionRebuild( mSelectedGuides, true );
}

SelectedGuides & HairGuides::getSelectedGuides() 
{
	return mSelectedGuides;
}

const SegmentsUG & HairGuides::getGuidesSegmentsUG()
{
	if ( mSegmentsUG.isDirty() ) // Is segments UG up-to-date ?
	{
		mSegmentsUG.build( mCurrentPositions, mSegmentsStorage->getCurrentSegments() );
	}
	return mSegmentsUG;
}

void HairGuides::updateGuides( bool aStoreUpdate )
{
	// Update selection
	mDisplayedGuides.selectionRebuild( mSelectedGuides, true );
	// Set guides segments dirty
	mSegmentsUG.setDirty();
	if ( aStoreUpdate ) // Final update ? ( user has stop using brush, etc. )
	{
		// Propagate changes to all frames and update undo stack
		mUndoStack.add( mSegmentsStorage->propagateChanges( mSelectedGuides ) );
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

void HairGuides::importNURBS( void )
{
}

void HairGuides::exportNURBS( void )
{
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
	mSegmentsUG.setDirty();
}

void HairGuides::meshUpdate( const MayaMesh & aMayaMesh, bool aTopologyChanged )
{
	if ( aTopologyChanged )
	{
		// Rest positions has changed...
		mRestPositionsUG.setDirty();
	}
	else
	{
		// First update current positions
		GuidesCurrentPositions::iterator currPosIt = mCurrentPositions.begin();
		for( GuidesRestPositions::iterator restPosIt = mRestPositions.begin(); restPosIt != mRestPositions.end(); 
			++restPosIt, ++currPosIt )
		{
			
		}
	}
	// Segments has changed...
	mDisplayedGuides.setDirty();
	mSegmentsUG.setDirty();
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
	mSegmentsUG.setDirty();
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
	mSegmentsUG.setDirty();
}

void HairGuides::emptyHistoryStack()
{
	mUndoStack.clear();
}

void HairGuides::generate( UVPointGenerator & aUVPointGenerator, const MayaMesh & aMayaMesh, 
	const Interpolation::InterpolationGroups & aInterpolationGroups, const HairLength::LengthInfo & aLengthInfo, 
	int aCount, bool aInterpolateFromPrevious )
{

}

void HairGuides::updateSegmentsCount( const Interpolation::InterpolationGroups & aInterpolationGroups )
{
}

void HairGuides::updateLength( const HairLength::LengthInfo & aLengthInfo )
{
}

void HairGuides::exportToFile( std::ostream & aOutputStream )
{
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble