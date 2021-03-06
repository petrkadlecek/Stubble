#include "BrushTool.hpp"

// parameters that can be passed from the UI,
// declared in GenericTool.cpp
extern const char *toolScaleFlag;
extern const char *toolScaleLongFlag;
extern const char *toolShapeFlag;
extern const char *toolShapeLongFlag;
extern const char *brushModeChoiceFlag;
extern const char *brushModeChoiceLongFlag;
extern const char *brushSensitivityFlag;
extern const char *brushSensitivityLongFlag;
extern const char *brushFalloffFlag;
extern const char *brushFalloffLongFlag;
extern const char *brushCollisionFlag;
extern const char *brushCollisionLongFlag;
extern const char *brushNumberOfThreadsFlag;
extern const char *brushNumberOfThreadsLongFlag;

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

const MString BrushTool::sHelpTxt( "Click to select hair/hair vertices and drag them to brush." );

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
		pars.getFlagArgument( brushModeChoiceFlag, 0, mCurrentBrushToolObject->mBrushModeChoice );
		// this is the connection between the UI and the brushMode state change
		mCurrentBrushToolObject->changeBrushMode();
	}

	if( pars.isFlagSet( toolScaleFlag ) )
	{
		pars.getFlagArgument( toolScaleFlag, 0, mCurrentBrushToolObject->mScale );
		mCurrentBrushToolObject->notify();
	}

	if( pars.isFlagSet( brushSensitivityFlag ) )
	{
		pars.getFlagArgument( brushSensitivityFlag, 0, mCurrentBrushToolObject->mSensitivity );
	}

	if( pars.isFlagSet( brushNumberOfThreadsFlag ) )
	{
		pars.getFlagArgument( brushNumberOfThreadsFlag, 0, HairTaskProcessor::getNumberOfThreads() );
	}

	if ( pars.isFlagSet( brushFalloffFlag ) )
	{
		pars.getFlagArgument( brushFalloffFlag, 0, mCurrentBrushToolObject->mEnableFalloff );
		mCurrentBrushToolObject->notify();
	}

	if ( pars.isFlagSet( brushCollisionFlag ) )
	{
		pars.getFlagArgument( brushCollisionFlag, 0, mCurrentBrushToolObject->mEnableCollisionDetection );
		mCurrentBrushToolObject->notify();
	}

	if( pars.isFlagSet( toolShapeFlag ) )
	{
		MString shape;
		pars.getFlagArgument( toolShapeFlag, 0, shape ); //TODO: make proper reaction
		mCurrentBrushToolObject->changeToolShape(shape);

		mCurrentBrushToolObject->notify();
	}

	return MS::kSuccess;
}

MStatus	BrushToolCommand::doQueryFlags()
{
	MArgParser pars = parser();

	if ( pars.isFlagSet( brushModeChoiceFlag ) )
	{
		setResult( mCurrentBrushToolObject->mBrushModeChoice );
	}
	
	if ( pars.isFlagSet( toolScaleFlag ) )
	{
		setResult( mCurrentBrushToolObject->mScale );
	}

	if ( pars.isFlagSet( brushSensitivityFlag ) )
	{
		setResult( mCurrentBrushToolObject->mSensitivity );
	}

	if ( pars.isFlagSet( brushNumberOfThreadsFlag ) )
	{
		setResult( HairTaskProcessor::getNumberOfThreads() );
	}

	if ( pars.isFlagSet( brushFalloffFlag ) )
	{
		setResult( mCurrentBrushToolObject->mEnableFalloff );
	}

	if ( pars.isFlagSet( brushCollisionFlag ) )
	{
		setResult( mCurrentBrushToolObject->mEnableCollisionDetection );
	}
	if( pars.isFlagSet( toolShapeFlag ) )
	{
		setResult( mCurrentBrushToolObject->getToolShape()->getName() );
	}
	
	return MS::kSuccess;
}


//----------------------------------------------------------------------------------------------------
// BrushTool
//----------------------------------------------------------------------------------------------------

const Real BrushTool::SENSITIVITY_RATIO = 0.1;

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

BrushTool::BrushTool() :
	GenericTool(new CircleToolShape()),
	mSensitivity(1.0f),
	mEnableFalloff(true),
	mEnableCollisionDetection(false),
	mBrushModeChoice(1), // These two lines must go together. The TranslateBrushMode has an index of 1 (represented by mBrushModeChoice),
	mBrushMode(&BrushTool::sTranslateBrushMode) // so we make sure that the starting state is valid and consistent
{
	setTitleString( "Stubble Brush Tool" );
	mBrushMode->setFalloffSwitch(mEnableFalloff);
	mBrushMode->setCollisionDetectionSwitch(mEnableCollisionDetection);
}

BrushTool::~BrushTool()
{
}

void BrushTool::getClassName( MString &aName ) const
{ 
	aName = "StubbleBrushTool";
}

void BrushTool::toolOnSetup ( MEvent &aEvent )
{
	// Get the GUI's click box size.
	MGlobal::executeCommand( "selectPref -query -clickBoxSize", mClickBoxSize );

	// Get the active view in which we will do the brushing.
	getActiveView();
	
	setHelpString( sHelpTxt );	//	Sets the help text in the help UI item.
	
	setCursor( MCursor::editCursor );

	// initialize the mouse move listener with the current tool as its owner
	sMouseMoveListener = new MouseMoveListener( mView.widget(), this );

	// update the haptic listener with the current tool as its owner
	HapticListener::setTool( this );

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

	HapticListener::setTool( NULL );
	BrushTool::deleteMouseMoveListener();
}

MStatus BrushTool::doPress( MEvent &aEvent )
{
	if ( mHapticToolShape ) return MS::kSuccess; // do nothing when brushing with haptics

	// If we have a left mouse click, start the selection.
	if( aEvent.mouseButton() == MEvent::kLeftMouse )
	{
		// We need to keep a reference of the view in which this command
		// was called. This basically lets us know which viewport to draw in.
		getActiveView();

		// We'll only handle the event if the left mouse button is the only thing held down.
		if( !aEvent.isModifierShift() && !aEvent.isModifierControl() && !aEvent.isModifierMiddleMouseButton() )
		{
			// Get the current position of the cursor. We need to know this so we can
			// start drawing our BrushToolShape and to be able to calculate the deltas for the brushing.
			aEvent.getPosition( mStartPos[0], mStartPos[1] );

			// make sure both the start and end points are in the same place.
			mEndPos[ 0 ] = mPrevPos[ 0 ] = mStartPos[ 0 ];
			mEndPos[ 1 ] = mPrevPos[ 1 ] = mStartPos[ 1 ];

			filterAffectedGuides();

			return MS::kSuccess;
		}
	}

	// In every other case, just let the base class handle the event.
	return MPxContext::doPress( aEvent );
}


void BrushTool::doHapticPress()
{
	// We need to keep a reference of the view in which this command
	// was called. This basically lets us know which viewport to draw in.
	getActiveView();

	// filter affected guides in haptic shape
	filterAffectedGuidesHaptic();
}

MStatus BrushTool::doDrag( MEvent &aEvent )
{
	if ( mHapticToolShape ) return MS::kSuccess; // do nothing when brushing with haptics

	// If we are dragging and left mouse button is pressed, then handle the event.
	if( !aEvent.isModifierLeftMouseButton() )
	{
		// Save the old coordinates
		mPrevPos[ 0 ] = mEndPos[ 0 ];
		mPrevPos[ 1 ] = mEndPos[ 1 ];

		// Get the new location of the cursor
		aEvent.getPosition( mEndPos[ 0 ], mEndPos[ 1 ] );

		// Dispatch brushing event
		short dX = mEndPos[ 0 ] - mPrevPos[ 0 ];
		short dY = mEndPos[ 1 ] - mPrevPos[ 1 ];
		this->doBrush( Vector3D< double >(dX, dY, 0.0) );
	}

	// In every other case, just let the base class handle the event.
	return MPxContext::doDrag( aEvent );
}

void BrushTool::doHapticDrag( MVector &aDragVector )
{
	this->doBrush( Vector3D< double >( 10.0 * aDragVector.x, 10.0 * aDragVector.y, 10.0 * aDragVector.z ) );
}

MStatus BrushTool::doRelease( MEvent & aEvent )
{
	if ( mHapticToolShape ) return MS::kSuccess; // do nothing when brushing with haptics

	MStatus stat;

	// only bother handling the release of a left mouse button.
	if( aEvent.mouseButton() != MEvent::kLeftMouse ) {
		return MPxContext::doRelease( aEvent );
	}
	
	// nullify selection area
	mStartPos[ 0 ] = mStartPos[ 1 ] = mPrevPos[ 0 ] = mPrevPos[ 1 ] = mEndPos[ 0 ] = mEndPos[ 1 ] = 0;

	// Makes sure that no computations in the worker thread are done beyond this point to prevent
	// horrible crashes due to changes in shared resources (affected guides etc.)
	HairTaskProcessor::stopWorkerThread();

	// Put the change into the undo stack
	HairShape::HairShape *activeHairShape = HairShape::HairShape::getActiveObject();
	if ( 0 != activeHairShape )
	{
		activeHairShape->updateGuides(true);
	}

	return MS::kSuccess;
}

void BrushTool::doHapticRelease()
{
	// Makes sure that no computations in the worker thread are done beyond this point to prevent
	// horrible crashes due to changes in shared resources (affected guides etc.)
	HairTaskProcessor::stopWorkerThread();

	// Put the change into the undo stack
	HairShape::HairShape *activeHairShape = HairShape::HairShape::getActiveObject();
	if ( 0 != activeHairShape )
	{
		activeHairShape->updateGuides(true);
	}
}

void BrushTool::doBrush( Vector3D< double > aDX )
{
	HairShape::HairShape *activeHairShape = HairShape::HairShape::getActiveObject();
	if ( 0 == activeHairShape )
	{
		return;
	}

	Real ratio = BrushTool::SENSITIVITY_RATIO * mSensitivity;
	Vector3D< Real > moveVector( ratio * aDX.x, ratio * aDX.y, ratio * aDX.z );

	// Get the viewing vector and cursor position in world coordinates
	MPoint pos;
	MVector dir;
	mView.viewToWorld(mStartPos[ 0 ], mStartPos[ 1 ], pos, dir);

	Vector3D< Real > position(pos.x, pos.y, pos.z);
	Vector3D< Real > direction(dir.x, dir.y, dir.z);
	direction.normalize();

	// Create and dispatch the hair task
	HairTask *task = new HairTask( mView, position, direction, moveVector, activeHairShape, &mAffectedGuides, mBrushMode );
	HairTaskProcessor::getInstance()->enqueueTask( task );
}

void BrushTool::notify()
{
	mShape->update( this );
	mBrushMode->setFalloffSwitch( mEnableFalloff );
	mBrushMode->setCollisionDetectionSwitch( mEnableCollisionDetection );
}

void BrushTool::changeBrushMode()
{
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
	mBrushMode->setFalloffSwitch(mEnableFalloff);
	mBrushMode->setCollisionDetectionSwitch(mEnableCollisionDetection);
}

void BrushTool::filterAffectedGuides()
{
	if ( mHapticToolShape ) return; // do nothing when brushing with haptics

	HairShape::HairShape *activeHairShape = HairShape::HairShape::getActiveObject();
	if ( 0 == activeHairShape )
	{
		return;
	}

	CircleToolShape *shape = dynamic_cast<CircleToolShape *>(mShape);

	if ( shape )
	{
		activeHairShape->getSelectedGuidesDS().select(shape, mStartPos[0], mStartPos[1], mAffectedGuides);
	}
}

void BrushTool::filterAffectedGuidesHaptic()
{
	HairShape::HairShape *activeHairShape = HairShape::HairShape::getActiveObject();
	if ( 0 == activeHairShape )
	{
		return;
	}

	if (mShape->getName() == "Haptic Sphere Tool Shape") // called once when pressed - optimization possible
	{
		SphereToolShape *shape = dynamic_cast<SphereToolShape *>(mShape);

		if ( shape )
		{
			activeHairShape->getSelectedGuidesDS().select(shape, mAffectedGuides);
		}
	}
	else
	{
		CylinderToolShape *shape = dynamic_cast<CylinderToolShape *>(mShape);

		if ( shape )
		{
			activeHairShape->getSelectedGuidesDS().select(shape, mAffectedGuides);
		}
	}
	
}


} // namespace Toolbox

} // namespace Stubble
