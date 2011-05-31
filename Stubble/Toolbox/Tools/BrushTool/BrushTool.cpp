#include <maya/MCursor.h>

#include "BrushTool.hpp"
//#include "../../BrushModes/ScaleBrushMode/ScaleBrushMode.hpp"
//#include "../../BrushModes/TranslateBrushMode/TranslateBrushMode.hpp"

/*TODO*/
//const char *radiusFlag = "-r", *radiusLongFlag = "-radius";
const char *brushModeChoiceFlag = "-bmc", *brushModeChoiceLongFlag = "-brushModeChoice";

namespace Stubble
{

namespace Toolbox
{

//----------------------------------------------------------------------------------------------------
// BrushToolCommand
//----------------------------------------------------------------------------------------------------
MPxContext* BrushToolCommand::makeObj()
{
	mCurrentBrushToolObject = new BrushTool();
	return mCurrentBrushToolObject;
}

const MString BrushToolCommand::sCommandName( "StubbleBrushToolCommand" );

void* BrushToolCommand::creator()
{
	return new BrushToolCommand;
}

const MString BrushTool::sHelpTxt( "Click to select hair and drag to brush it." );

BrushToolCommand::BrushToolCommand()
{
	mCurrentBrushToolObject = NULL;
}

BrushToolCommand::~BrushToolCommand()
{
	delete mCurrentBrushToolObject;
}


MStatus	BrushToolCommand::doEditFlags()
{
	MArgParser pars = parser();

	if( pars.isFlagSet( brushModeChoiceFlag ) )
	{
		pars.getFlagArgument( brushModeChoiceFlag, 0, ( mCurrentBrushToolObject->mBrushModeChoice ) );
		// this is the connection between the UI and the brushMode state change
		mCurrentBrushToolObject->changeBrushMode();
	}
	return MS::kSuccess;
}

MStatus	BrushToolCommand::doQueryFlags()
{
	MArgParser pars = parser();

	if( pars.isFlagSet( brushModeChoiceFlag ) )
		setResult( mCurrentBrushToolObject->mBrushModeChoice );
		
	return MS::kSuccess;
}

MStatus	BrushToolCommand::appendSyntax()
{
	MSyntax syn = syntax();
	
	syn.addFlag( brushModeChoiceFlag, brushModeChoiceLongFlag, MSyntax::kLong );

	return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------
// BrushTool
//----------------------------------------------------------------------------------------------------

ClumpBrushMode BrushTool::clumpBrushMode;
PuffEndBrushMode BrushTool::puffEndBrushMode;
PuffRootBrushMode BrushTool::puffRootBrushMode;
RotateBrushMode BrushTool::rotateBrushMode;
ScaleBrushMode BrushTool::scaleBrushMode;
TranslateBrushMode BrushTool::translateBrushMode;

BrushTool::BrushTool()
{
	setTitleString( "Stubble Brush Tool" );
	
	// The next two lines must go together. The TranslateBrushMode has an index of 1 (represented by mBrushModeChoice),
	// so we make sure that the starting state is valid.
	mBrushModeChoice = 1;
	mBrushMode = &BrushTool::translateBrushMode;
}

BrushTool::~BrushTool()
{
}

void BrushTool::getClassName( MString &aName ) const
{ 
	aName = "StubbleBrushTool";
}

void BrushTool::toolOnSetup ( MEvent & )
{
	// Get the GUI's click box size.
	MGlobal::executeCommand( "selectPref -query -clickBoxSize", mClickBoxSize );

	// Get the active view in which we will do the brushing.
	mView = M3dView::active3dView();
	
	setHelpString( sHelpTxt );	//	Sets the help text in the help UI item.

	setCursor( MCursor::editCursor );

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

void BrushTool::toolOffCleanup()
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
}

MStatus BrushTool::doPress( MEvent &event )
{
	// If we have a left mouse click, start the selection.
	if( event.mouseButton() == MEvent::kLeftMouse )
	{
		// We need to keep a reference of the view in which this command
		// was called. This basically lets us know which viewport to draw in.
		mView = M3dView::active3dView();

		// We'll only handle the event if the left mouse button is the only thing held down.
		if( !event.isModifierShift() &&	!event.isModifierControl() && !event.isModifierMiddleMouseButton() )
		{
			// Get the current position of the cursor. We need to know this so we can
			// start drawing our BrushToolShape and to be able to calculate the deltas for the brushing.
			event.getPosition( mStartPos[0], mStartPos[1] );

			// make sure both the start and end points are in the same place.
			mEndPos[ 0 ] = mPrevPos[ 0 ] = mStartPos[ 0 ];
			mEndPos[ 1 ] = mPrevPos[ 1 ] = mStartPos[ 1 ];

			// Testing the state pattern.
			this->doBrush( MVector( 3, 5, 2), 4 );

			return MS::kSuccess;
		}
	}

	// In every other case, just let the base class handle the event.
	return MPxContext::doPress( event );
}

MStatus BrushTool::doDrag( MEvent &event )
{
	// If we are dragging and left mouse button is pressed, then handle the event.
	if( !event.isModifierLeftMouseButton() )
	{
		// Save the old coordinates
		mPrevPos[ 0 ] = mEndPos[ 0 ];
		mPrevPos[ 1 ] = mEndPos[ 1 ];

		// Get the new location of the cursor
		event.getPosition( mEndPos[ 0 ], mEndPos[ 1 ] );		
	}

	// In every other case, just let the base class handle the event.
	return MPxContext::doDrag( event );
}

MStatus BrushTool::doRelease( MEvent & event )
{
	MStatus stat;

	// only bother handling the release of a left mouse button.
	if( event.mouseButton() != MEvent::kLeftMouse ) {
		return MPxContext::doRelease( event );
	}
	
	// nullify selection area
	mStartPos[ 0 ] = mStartPos[ 1 ] = mPrevPos[ 0 ] = mPrevPos[ 1 ] = mEndPos[ 0 ] = mEndPos[ 1 ] = 0;

	return MS::kSuccess;
}

void BrushTool::doBrush( MVector aDX, float aDT )
{
	mBrushMode->doBrush( aDX, aDT );
}

void BrushTool::changeBrushMode()
{
	// Release the current BrushMode object.
	//delete mBrushMode;

	switch (mBrushModeChoice)
	{
	case 1:
		mBrushMode = &BrushTool::translateBrushMode;
		break;
	case 2:
		mBrushMode = &BrushTool::rotateBrushMode;
		break;
	case 3:
		mBrushMode = &BrushTool::scaleBrushMode;
		break;
	case 4:
		mBrushMode = &BrushTool::clumpBrushMode;
		break;
	case 5:
		mBrushMode = &BrushTool::puffRootBrushMode;
		break;
	case 6:
		mBrushMode = &BrushTool::puffEndBrushMode;
		break;
	default:
		mBrushMode = &BrushTool::translateBrushMode;
		mBrushModeChoice = 1;
	}
}

} // namespace Toolbox

} // namespace Stubble