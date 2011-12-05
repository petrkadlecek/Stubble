#include "CutTool.hpp"
#include "../../HairTaskProcessor.hpp"

// parameters that can be passed from the UI,
// inherited from GenericTool
extern const char *toolScaleFlag;
extern const char *toolScaleLongFlag;

namespace Stubble
{

namespace Toolbox
{

//----------------------------------------------------------------------------------------------------
// CutToolCommand
//----------------------------------------------------------------------------------------------------
MPxContext* CutToolCommand::makeObj()
{
	mCurrentCutToolObject = new CutTool();
	return mCurrentCutToolObject;
}

const MString CutToolCommand::sCommandName( "StubbleCutToolCommand" );

void* CutToolCommand::creator()
{
	return new CutToolCommand;
}

const MString CutTool::sHelpTxt( "TODO" );

CutToolCommand::CutToolCommand()
{
	mCurrentCutToolObject = NULL;
}

CutToolCommand::~CutToolCommand()
{
	delete mCurrentCutToolObject;
}

MStatus	CutToolCommand::doEditFlags()
{
	MArgParser pars = parser();

	if( pars.isFlagSet( toolScaleFlag ) )
	{
		pars.getFlagArgument( toolScaleFlag, 0, ( mCurrentCutToolObject->mScale ) );
		mCurrentCutToolObject->notify();
	}

	return MS::kSuccess;
}

MStatus	CutToolCommand::doQueryFlags()
{
	MArgParser pars = parser();

	if( pars.isFlagSet( toolScaleFlag ) )
		setResult( mCurrentCutToolObject->mScale );
	
	return MS::kSuccess;
}


//----------------------------------------------------------------------------------------------------
// CutTool
//----------------------------------------------------------------------------------------------------

QPointer<MouseMoveListener> CutTool::sMouseMoveListener;

void CutTool::deleteMouseMoveListener()
{
	if (!(sMouseMoveListener.isNull())) 
	{
		delete sMouseMoveListener;
	}
}

CutTool::CutTool() :
	GenericTool(new CircleToolShape())
{
	setTitleString( "Stubble Cut Tool" );	
}

CutTool::~CutTool()
{
}

void CutTool::getClassName( MString &aName ) const
{ 
	aName = "StubbleCutTool";
}

void CutTool::toolOnSetup ( MEvent & )
{
	// Get the GUI's click box size.
	MGlobal::executeCommand( "selectPref -query -clickBoxSize", mClickBoxSize );

	// Get the active view in which we will do the cutting.
	getActiveView();
	
	setHelpString( sHelpTxt );	//	Sets the help text in the help UI item.

	setCursor( MCursor::editCursor );

	// initialize the mouse move listener with the current tool as its owner
	sMouseMoveListener = new MouseMoveListener( mView.widget(), this );

	// Record current selection mode and masks
	mPrevSelMode = MGlobal::selectionMode();
	if( mPrevSelMode == MGlobal::kSelectComponentMode )
	{
		mPrevCompMask = MGlobal::componentSelectionMask();
	}
	else
	{
		mPrevObjMask = MGlobal::objectSelectionMask();	
	}
}

void CutTool::toolOffCleanup()
{
	// Restore previous selection mode and masks
	MGlobal::setSelectionMode( mPrevSelMode );
	if( mPrevSelMode == MGlobal::kSelectComponentMode )
	{
		MGlobal::setComponentSelectionMask( mPrevCompMask );
	}
	else
	{
		MGlobal::setObjectSelectionMask( mPrevObjMask );
	}

	CutTool::deleteMouseMoveListener();
}

MStatus CutTool::doPress( MEvent & aEvent )
{
	getActiveView();

	return MPxContext::doPress( aEvent );
}

MStatus CutTool::doRelease( MEvent & aEvent )
{	
	MStatus stat;

	// Let the base class handle release of other buttons
	if( aEvent.mouseButton() != MEvent::kLeftMouse ) {
		return MPxContext::doRelease( aEvent );
	}

	// Get the new location of the cursor
    aEvent.getPosition( mPosition[ 0 ], mPosition[ 1 ] );
		
	filterAffectedGuides();
	
	this->doCut();

	// Put the change into the undo stack
	HairShape::HairShape *activeHairShape = HairShape::HairShape::getActiveObject();
	if ( 0 != activeHairShape )
	{
		activeHairShape->updateGuides( true );
	}

	return MS::kSuccess;
}

void CutTool::doCut()
{
	// over all affected guides
	for ( HairShape::HairComponents::SelectedGuides::iterator it = mAffectedGuides.begin();
		it != mAffectedGuides.end(); it++ )
	{
		HairShape::HairComponents::SelectedGuide *guide = *it;
		HairShape::HairComponents::SegmentsAdditionalInfo::iterator infoIt = guide->mSegmentsAdditionalInfo.begin();		
		HairShape::HairComponents::Segments::iterator segmentIt;

		assert( guide->mGuideSegments.mSegments.size() == guide->mSegmentsAdditionalInfo.size() );

		// where do we cut
		int segmentsLeft = 0;
		size_t segmentCount = guide->mSegmentsAdditionalInfo.size();
		while ( segmentsLeft < segmentCount && ! guide->mSegmentsAdditionalInfo[ segmentsLeft ].mInsideBrush )
		{
			segmentsLeft++;
		}

		Real oldSegmentLength = guide->mGuideSegments.mSegmentLength;
		if ( oldSegmentLength >= -EPSILON && oldSegmentLength <= EPSILON )
		{
			continue; // nothing more to cut
		}

		Real newSegmentLength = oldSegmentLength * max( 0, segmentsLeft - 1 ) / ( segmentCount - 1 );
		
		HairShape::HairComponents::Segments newSegments;
		newSegments.reserve( segmentCount );
				
		// create new segments
		for ( int i = 0; i < segmentCount; i++ )
		{
			Real placeAfterSegment;
			Real dist = modf( i * newSegmentLength / oldSegmentLength, &placeAfterSegment );
			
			// XXX new points are placed according to distance on the *original* polyline
			newSegments.push_back( guide->mGuideSegments.mSegments[ placeAfterSegment ]
				+ ( guide->mGuideSegments.mSegments[ placeAfterSegment + 1 ]
				  - guide->mGuideSegments.mSegments[ placeAfterSegment ] ) * dist );
		}

		// replace old values
		guide->mGuideSegments.mSegmentLength = newSegmentLength;
		guide->mGuideSegments.mSegments = newSegments;
		guide->mDirtyFlag = true;
		guide->mDirtyRedrawFlag = true;
	}

	// Make sure that all segments have the same length
	HairTaskProcessor::enforceConstraints(mAffectedGuides);
}

void CutTool::changeToolShape( void )
{
	// TODO?
}

void CutTool::notify()
{
	mShape->update( this );
}

void CutTool::filterAffectedGuides()
{
	HairShape::HairShape *activeHairShape = HairShape::HairShape::getActiveObject();
	if ( 0 == activeHairShape )
	{
		return;
	}

	//FIXME: remove dynamic_cast - do it somehow better
	activeHairShape->getSelectedGuidesUG().select(
		dynamic_cast< CircleToolShape * >( mShape ), mPosition[ 0 ], mPosition[ 1 ], mAffectedGuides );
}

} // namespace Toolbox

} // namespace Stubble