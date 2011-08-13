#include <maya/MCursor.h>

#include "BrushTool.hpp"
//#include "../../BrushModes/ScaleBrushMode/ScaleBrushMode.hpp"
//#include "../../BrushModes/TranslateBrushMode/TranslateBrushMode.hpp"


const char *circleRadiusFlag = "-cr", *circleRadiusLongFlag = "-circleRadius";
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

	if( pars.isFlagSet( circleRadiusFlag ) )
	{
		pars.getFlagArgument( circleRadiusFlag, 0, ( mCurrentBrushToolObject->mCircleRadius ) );
		mCurrentBrushToolObject->notify();
	}

	return MS::kSuccess;
}

MStatus	BrushToolCommand::doQueryFlags()
{
	MArgParser pars = parser();

	if( pars.isFlagSet( brushModeChoiceFlag ) )
		setResult( mCurrentBrushToolObject->mBrushModeChoice );
	
	if( pars.isFlagSet( circleRadiusFlag ) )
		setResult( mCurrentBrushToolObject->mCircleRadius );
	
	return MS::kSuccess;
}

MStatus	BrushToolCommand::appendSyntax()
{
	MSyntax syn = syntax();
	
	syn.addFlag( brushModeChoiceFlag, brushModeChoiceLongFlag, MSyntax::kLong );

	syn.addFlag( circleRadiusFlag, circleRadiusLongFlag, MSyntax::kLong );

	return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------
// BrushTool
//----------------------------------------------------------------------------------------------------

ClumpBrushMode BrushTool::sClumpBrushMode;
PuffEndBrushMode BrushTool::sPuffEndBrushMode;
PuffRootBrushMode BrushTool::sPuffRootBrushMode;
RotateBrushMode BrushTool::sRotateBrushMode;
ScaleBrushMode BrushTool::sScaleBrushMode;
TranslateBrushMode BrushTool::sTranslateBrushMode;

QPointer<MouseMoveListener> BrushTool::sMouseMoveListener;

void BrushTool::deleteMouseMoveListener()
{
	if (!(sMouseMoveListener.isNull())) 
	{
		delete sMouseMoveListener;
	}
}

BrushTool::BrushTool()
{
	setTitleString( "Stubble Brush Tool" );
	
	// The next two lines must go together. The TranslateBrushMode has an index of 1 (represented by mBrushModeChoice),
	// so we make sure that the starting state is valid.
	mBrushModeChoice = 1;
	mBrushMode = &BrushTool::sTranslateBrushMode;
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

	BrushTool::deleteMouseMoveListener();
}

MStatus BrushTool::doPress( MEvent &event )
{
	//std::cout << "doPress()\n" << std::flush;

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

			return MS::kSuccess;
		}
	}

	// In every other case, just let the base class handle the event.
	return MPxContext::doPress( event );
}

MStatus BrushTool::doDrag( MEvent &event )
{
	//std::cout << "doDrag()\n" << std::flush;

	// If we are dragging and left mouse button is pressed, then handle the event.
	if( !event.isModifierLeftMouseButton() )
	{
		// Save the old coordinates
		mPrevPos[ 0 ] = mEndPos[ 0 ];
		mPrevPos[ 1 ] = mEndPos[ 1 ];

		// Get the new location of the cursor
		event.getPosition( mEndPos[ 0 ], mEndPos[ 1 ] );

		//TODO: calculate dX
		this->doBrush( Vector3D< double >(1.0, 1.0, 1.0) );
	}

	// In every other case, just let the base class handle the event.
	return MPxContext::doDrag( event );
}

MStatus BrushTool::doRelease( MEvent & event )
{
	//std::cout << "doRelease()\n" << std::flush;

	MStatus stat;

	// only bother handling the release of a left mouse button.
	if( event.mouseButton() != MEvent::kLeftMouse ) {
		return MPxContext::doRelease( event );
	}
	
	// nullify selection area
	mStartPos[ 0 ] = mStartPos[ 1 ] = mPrevPos[ 0 ] = mPrevPos[ 1 ] = mEndPos[ 0 ] = mEndPos[ 1 ] = 0;

	return MS::kSuccess;
}

void BrushTool::doBrush( Vector3D< double > aDX )
{
	//std::cout << "doBrush()\n" << std::flush;

	HairTask *task = new HairTask(); //TODO: create a factory method
	task->mBrushMode = this->mBrushMode;
	task->mDx = aDX;
	//TODO: add affected hair

	HairTaskProcessor::getInstance()->enqueueTask(task);
}

void BrushTool::notify()
{
	mShape->update( this );
}

void BrushTool::changeBrushMode()
{
	// Release the current BrushMode object.
	//delete mBrushMode;

	switch (mBrushModeChoice)
	{
	case 1:
		mBrushMode = &BrushTool::sTranslateBrushMode;
		break;
	case 2:
		mBrushMode = &BrushTool::sRotateBrushMode;
		break;
	case 3:
		mBrushMode = &BrushTool::sScaleBrushMode;
		break;
	case 4:
		mBrushMode = &BrushTool::sClumpBrushMode;
		break;
	case 5:
		mBrushMode = &BrushTool::sPuffRootBrushMode;
		break;
	case 6:
		mBrushMode = &BrushTool::sPuffEndBrushMode;
		break;
	default:
		mBrushMode = &BrushTool::sTranslateBrushMode;
		mBrushModeChoice = 1;
	}
}

void BrushTool::changeToolShape()
{
	/*TODO*/
}

} // namespace Toolbox

} // namespace Stubble