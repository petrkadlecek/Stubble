#include "HapticSettingsTool.hpp"

namespace Stubble
{

namespace Toolbox
{

// ----------------------------------------------------------------------------
// Static data members
// ----------------------------------------------------------------------------
cHapticDeviceHandler* HapticSettingsTool::sHandler;

cGenericHapticDevice* HapticSettingsTool::sHapticDevice;
bool HapticSettingsTool::sHapticThreadRunning;
MVector HapticSettingsTool::sLastPosition;
MVector HapticSettingsTool::sLastRotation;
MVector HapticSettingsTool::sSpringDamper;
MVector HapticSettingsTool::sCollisionVector;
double HapticSettingsTool::sLastRotationAngle;
double HapticSettingsTool::sWorkspaceRadius;
bool HapticSettingsTool::sHapticButton1;
bool HapticSettingsTool::sHapticButton2;
bool HapticSettingsTool::sHapticButton1Last;
bool HapticSettingsTool::sHapticButton2Last;
bool HapticSettingsTool::sDeviceAvailable;
bool HapticSettingsTool::sSpringDamperSet;
int HapticSettingsTool::sHapticThreadFrequency;
int HapticSettingsTool::sProxyDetail;
bool HapticSettingsTool::sInteractiveBrush;
bool HapticSettingsTool::sSimulate5DOF;
bool HapticSettingsTool::sShowXZHelper;
bool HapticSettingsTool::sEnableSpringForce;
bool HapticSettingsTool::sEnableCollisionDetection;
bool HapticSettingsTool::sHapticRendering;
int HapticSettingsTool::sForceMagnitude;

//----------------------------------------------------------------------------------------------------
// HapticSettingsToolCommand
//----------------------------------------------------------------------------------------------------

const MString HapticSettingsToolCommand::sCommandName( "StubbleHapticSettingsToolCommand" );

const char *initFlag = "-i"; const char *initLongFlag = "-init";

const char *hapticThreadFrequencyFlag = "-fr"; const char *hapticThreadFrequencyFlagLong = "-freq";

const char *proxyDetailFlag = "-d"; const char *proxyDetailFlagLong = "-proxyDetail";

const char *interactiveBrushFlag = "-ib"; const char *interactiveBrushFlagLong = "-interactiveBrushing";

const char *simulate5DOFFlag = "-s"; const char *simulate5DOFFlagLong = "-simulate5dof";

const char *showXZHelperFlag = "-h"; const char *showXZHelperFlagLong = "-showXZHelper";

const char *enableSpringForceFlag = "-fs"; const char *enableSpringForceFlagLong = "-sprinForce";

const char *enableCollisionDetectionFlag = "-cd"; const char *enableCollisionDetectionFlagLong = "-collisionDetecion";

const char *hapticRenderingFlag = "-r"; const char *hapticRenderingFlagLong = "-hapticRendering";

const char *forceMagnitudeFlag = "-mr"; const char *forceMagnitudeFlagLong = "-magnitudeRatio";

MPxContext* HapticSettingsToolCommand::makeObj()
{
	mCurrentObject = new HapticSettingsTool();
	return mCurrentObject;
}

void* HapticSettingsToolCommand::creator()
{
	return new HapticSettingsToolCommand;
}

HapticSettingsToolCommand::HapticSettingsToolCommand()
{
	mCurrentObject = NULL;
}

HapticSettingsToolCommand::~HapticSettingsToolCommand()
{
	delete mCurrentObject;
}

MStatus	HapticSettingsToolCommand::doEditFlags()
{
	MArgParser pars = parser();

	if( pars.isFlagSet( initFlag ) )
	{
		pars.getFlagArgument( initFlag, 0, (mCurrentObject->mHapticDeviceStr) );
		mCurrentObject->initHapticDevice(mCurrentObject->mHapticDeviceStr.substring(0,0).asInt());
	}

	if( pars.isFlagSet( interactiveBrushFlag ) ) {
		pars.getFlagArgument( interactiveBrushFlag, 0, HapticSettingsTool::sInteractiveBrush );
	}

	if( pars.isFlagSet( proxyDetailFlag ) )
	{
		pars.getFlagArgument( proxyDetailFlag, 0, HapticSettingsTool::sProxyDetail );
	}

	if( pars.isFlagSet( showXZHelperFlag ) )
	{
		pars.getFlagArgument( showXZHelperFlag, 0, HapticSettingsTool::sShowXZHelper );
	}

	if( pars.isFlagSet( hapticThreadFrequencyFlag ) )
	{
		int freq;
		pars.getFlagArgument( hapticThreadFrequencyFlag, 0, freq );
		HapticSettingsTool::sHapticThreadFrequency = 1000 / freq;
	}

	if( pars.isFlagSet( simulate5DOFFlag ) )
	{
		pars.getFlagArgument( simulate5DOFFlag, 0, HapticSettingsTool::sSimulate5DOF );
	}

	if( pars.isFlagSet( enableSpringForceFlag ) )
	{
		pars.getFlagArgument( enableSpringForceFlag, 0, HapticSettingsTool::sEnableSpringForce );
	}

	if( pars.isFlagSet( enableCollisionDetectionFlag ) )
	{
		pars.getFlagArgument( enableCollisionDetectionFlag, 0, HapticSettingsTool::sEnableCollisionDetection );

		if ( !HapticSettingsTool::sEnableCollisionDetection && HapticSettingsTool::sHapticRendering )
		{
			HapticSettingsTool::sHapticRendering = false;
			MGlobal::executeCommand("checkBoxGrp -e -v1 false stubbleHapticRendering");
		}
	}

	if( pars.isFlagSet( hapticRenderingFlag ) )
	{
		pars.getFlagArgument( hapticRenderingFlag, 0, HapticSettingsTool::sHapticRendering );

		if ( HapticSettingsTool::sHapticRendering )
		{
			HapticSettingsTool::sEnableCollisionDetection = true;
			MGlobal::executeCommand("checkBoxGrp -e -v1 true stubbleHapticCollisionDetection");
		}
	}

	if( pars.isFlagSet( forceMagnitudeFlag ) )
	{
		pars.getFlagArgument( forceMagnitudeFlag, 0, HapticSettingsTool::sForceMagnitude );
	}

	return MS::kSuccess;
}

MStatus	HapticSettingsToolCommand::doQueryFlags()
{
	MArgParser pars = parser();

	return MS::kSuccess;
}

MStatus	HapticSettingsToolCommand::appendSyntax()
{
	MSyntax syn = syntax();
	
	syn.addFlag( initFlag, initLongFlag, MSyntax::kString );

	syn.addFlag( interactiveBrushFlag, interactiveBrushFlagLong, MSyntax::kBoolean );

	syn.addFlag( simulate5DOFFlag, simulate5DOFFlagLong, MSyntax::kBoolean );

	syn.addFlag( showXZHelperFlag, showXZHelperFlagLong, MSyntax::kBoolean );

	syn.addFlag( enableSpringForceFlag, enableSpringForceFlagLong, MSyntax::kBoolean );

	syn.addFlag( enableCollisionDetectionFlag, enableCollisionDetectionFlagLong, MSyntax::kBoolean );

	syn.addFlag( hapticRenderingFlag, hapticRenderingFlagLong, MSyntax::kBoolean );
	
	syn.addFlag( proxyDetailFlag, proxyDetailFlagLong, MSyntax::kLong );

	syn.addFlag( hapticThreadFrequencyFlag, hapticThreadFrequencyFlagLong, MSyntax::kLong );

	syn.addFlag( forceMagnitudeFlag, forceMagnitudeFlagLong, MSyntax::kLong );

	return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------
// HapticSettingsTool
//----------------------------------------------------------------------------------------------------

HapticSettingsTool::HapticSettingsTool()
{
	setTitleString( "Stubble haptic settings Tool" );
	
	mInitFlag = false;

	HapticSettingsTool::sHapticThreadRunning = false;

	// set CHAI3D handler
	HapticSettingsTool::sHandler = NULL;
	
	HapticSettingsTool::sHapticDevice = NULL;
	
	HapticSettingsTool::sDeviceAvailable = false;

	// UI default settings

	HapticSettingsTool::sSpringDamperSet = false;

	HapticSettingsTool::sHapticButton1 = false;

	HapticSettingsTool::sHapticButton2 = false;

	HapticSettingsTool::sHapticThreadFrequency = 10;

	HapticSettingsTool::sProxyDetail = 8;

	HapticSettingsTool::sInteractiveBrush  = true;

	HapticSettingsTool::sSimulate5DOF = false;

	HapticSettingsTool::sShowXZHelper = true;

	HapticSettingsTool::sEnableSpringForce = false;

	HapticSettingsTool::sEnableCollisionDetection = false;

	HapticSettingsTool::sHapticRendering = false;

	HapticSettingsTool::sForceMagnitude = 30;

	// zero collision vector

	sCollisionVector.x = 0;
	sCollisionVector.y = 0;
	sCollisionVector.z = 0;
}

HapticSettingsTool::~HapticSettingsTool()
{
	HapticSettingsTool::sHapticThreadRunning = false;

	if ( HapticSettingsTool::sHapticDevice != NULL )
	{
		HapticSettingsTool::sHapticDevice->close();
	}
}

void HapticSettingsTool::setCollisionVector( MVector &aCollisionVector )
{
	sCollisionVector = aCollisionVector;
}

void HapticSettingsTool::setSpringDamper()
{
	HapticSettingsTool::sSpringDamper = HapticSettingsTool::sLastPosition;
	HapticSettingsTool::sSpringDamperSet = true;
}

void HapticSettingsTool::unsetSpringDamper()
{
	HapticSettingsTool::sSpringDamperSet = false;
}

MVector HapticSettingsTool::getLastPosition()
{
	return sLastPosition / sWorkspaceRadius;
}

MVector HapticSettingsTool::getLastRotation()
{
	return sLastRotation;
}

double HapticSettingsTool::getLastRotationAngle()
{
	return sLastRotationAngle;
}

bool HapticSettingsTool::getHapticButton1State()
{
	return sHapticButton1;
}

bool HapticSettingsTool::getHapticButton2State()
{
	return sHapticButton2;
}

void HapticSettingsTool::getClassName( MString &aName ) const
{ 
	aName = "StubbleHapticSettingsTool";
}

void HapticSettingsTool::toolOnSetup( MEvent &event )
{
	if (mInitFlag) {
		return;
	}
	
	mInitFlag = true;

	// create instance of mHandler on ToolSetup
	if ( HapticSettingsTool::sHandler == NULL )
	{
		HapticSettingsTool::sHandler = new cHapticDeviceHandler();
	}

	cHapticDeviceInfo info; // haptic device info structure

	// clear scrollList
	MGlobal::executeCommandOnIdle( "textScrollList -e -ra hapticDevices;" );

	// debug
	std::cout << "HapticSettingsTool: number of detected haptic devices = " << HapticSettingsTool::sHandler->getNumDevices() << std::endl;

	assert( sHandler != NULL );

	// add devices
	for ( unsigned int i = 0; i < HapticSettingsTool::sHandler->getNumDevices(); ++i )
	{
		HapticSettingsTool::sHandler->getDeviceSpecifications( info, i );

		MString sCmd = "textScrollList -e -a \"";
		sCmd += i;
		sCmd += ": ";
		sCmd += info.m_manufacturerName.c_str();
		sCmd += " ";
		sCmd += info.m_modelName.c_str();
		sCmd += "\" hapticDevices;";

		MGlobal::executeCommandOnIdle( sCmd );
	}

	mView = M3dView::active3dView();
}

void HapticSettingsTool::toolOffCleanup()
{
  // do not close haptic device here
}

MThreadRetVal HapticSettingsTool::AsyncHapticLoop( void *aData )
{
	MGlobal::displayInfo("HapticSettingsTool: start loop");

	MStatus stat;
	MVector vector;

	static cVector3d lastPosition;
	static cVector3d newPosition;
	static cMatrix3d newRotation;
	static cVector3d newRotationVector;
	static double newRotationAngle;
	static cVector3d force;
	force.zero();
	static double minMovementEps = std::numeric_limits<double>::epsilon();

	int sleepTime = HapticSettingsTool::sHapticThreadFrequency;
	int noRefreshPass = 100;
	bool refreshNeeded = true;

	M3dView mView = M3dView::active3dView();

	HapticSettingsTool::sDeviceAvailable = true;
	HapticSettingsTool::sHapticThreadRunning = true;

	// create haptic listener node
	MGlobal::executeCommandOnIdle("createNode HapticListener;");

	while ( HapticSettingsTool::sHapticThreadRunning )
	{
		HapticSettingsTool::sHapticDevice->getPosition( newPosition );
		HapticSettingsTool::sHapticDevice->getRotation( newRotation );
		HapticSettingsTool::sHapticDevice->getUserSwitch( 0, HapticSettingsTool::sHapticButton1 );
		HapticSettingsTool::sHapticDevice->getUserSwitch( 1, HapticSettingsTool::sHapticButton2 );
		
		// optimize refreshing - refresh only when position of proxy changed 
		if ( lastPosition.distancesq( newPosition ) > minMovementEps )
		{
			refreshNeeded = true;
		}

		if ( HapticSettingsTool::sHapticButton1 == true || HapticSettingsTool::sHapticButton2 == true )
		{
			refreshNeeded = true;
		}

		if (
			HapticSettingsTool::sHapticButton1 != HapticSettingsTool::sHapticButton1Last || 
			HapticSettingsTool::sHapticButton2 != HapticSettingsTool::sHapticButton2Last 
		)
		{
			refreshNeeded = true;
		}

		// handle position vectors
		lastPosition = newPosition;
		HapticSettingsTool::sLastPosition.x = newPosition.y;
		HapticSettingsTool::sLastPosition.y = newPosition.z;
		HapticSettingsTool::sLastPosition.z = newPosition.x;

		// handle force reaction
		if ( sHapticRendering )
		{
			force.x = 1e-2 * sForceMagnitude * sCollisionVector.z;
			force.y = 1e-2 * sForceMagnitude * sCollisionVector.x;
			force.z = 1e-2 * sForceMagnitude * sCollisionVector.y;
		}
		else if ( sEnableSpringForce && sSpringDamperSet )
		{
			MVector forceVector = 1e-1 * sForceMagnitude * 
				( HapticSettingsTool::sSpringDamper / HapticSettingsTool::sWorkspaceRadius 
				- HapticSettingsTool::sLastPosition / HapticSettingsTool::sWorkspaceRadius );

			force.x = forceVector.z;
			force.y = forceVector.x;
			force.z = forceVector.y;
		}
		else
		{
			force.zero();
		}

		HapticSettingsTool::sHapticDevice->setForce( force );

		// handle rotation matrix
		newRotation.toAngleAxis(newRotationAngle, newRotationVector);
		HapticSettingsTool::sLastRotation.x = newRotationVector.x;
		HapticSettingsTool::sLastRotation.y = newRotationVector.y;
		HapticSettingsTool::sLastRotation.z = newRotationVector.z;
		HapticSettingsTool::sLastRotationAngle = newRotationAngle;
		
		// handle switches
		HapticSettingsTool::sHapticButton1Last = HapticSettingsTool::sHapticButton1;
		HapticSettingsTool::sHapticButton2Last = HapticSettingsTool::sHapticButton2;

		--noRefreshPass;

		if ( refreshNeeded || noRefreshPass == 0 )
		{
			MGlobal::executeCommandOnIdle("refresh");
			refreshNeeded = false;
			noRefreshPass = 100;
		}

		Sleep( sleepTime + 1 );

		sleepTime = HapticSettingsTool::sHapticThreadFrequency;
	}
	
	HapticSettingsTool::sHapticThreadRunning = false;
	HapticSettingsTool::sDeviceAvailable = false;

	return 0;
}

void HapticSettingsTool::AsyncHapticCallback (void *aData)
{
  // nothing to do here
}

void HapticSettingsTool::initHapticDevice( int aHapticDeviceIndex )
{
	// inform user that haptic thread is already running and return
	if (HapticSettingsTool::sHapticThreadRunning)
	{
		MString s = "confirmDialog -title \"HapticSettingsTool\" -message \"Haptic device has been already initialized.\" -button \"OK\";";
		MGlobal::executeCommand(s);
		return;
	}

	std::cout << "HapticSettingsTool: preparing device " << aHapticDeviceIndex << std::endl;

	if (HapticSettingsTool::sHandler->getDevice(HapticSettingsTool::sHapticDevice, aHapticDeviceIndex) == 0)
	{
		MGlobal::displayInfo("HapticSettingsTool: open");
		if (sHapticDevice->open() == 0)
		{
			MGlobal::displayInfo("HapticSettingsTool: init");

			if (HapticSettingsTool::sHapticDevice->initialize() == 0)
			{
				// get the workspace radius from the device specification
				HapticSettingsTool::sWorkspaceRadius = HapticSettingsTool::sHapticDevice->getSpecifications().m_workspaceRadius;

				MStatus pr = MThreadAsync::init();
				pr = MThreadAsync::createTask(HapticSettingsTool::AsyncHapticLoop, 0, HapticSettingsTool::AsyncHapticCallback, NULL);
				
				MString s = "confirmDialog -title \"HapticSettingsTool\" -message \"Haptic device successfully initialized.\" -button \"OK\";";
				MGlobal::executeCommand(s);
			}
			else
			{
				MGlobal::displayError("HapticTool ERROR - could not init device");
			}
		}
		else
		{
			MGlobal::displayError("HapticTool ERROR - could not open device");
		}
	}
	else
	{
		MGlobal::displayError("HapticTool ERROR - could not get device");
	}
}

} // namespace Toolbox

} // namespace Stubble
