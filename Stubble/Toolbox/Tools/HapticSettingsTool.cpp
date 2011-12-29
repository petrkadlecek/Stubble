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
double HapticSettingsTool::sLastRotationAngle;
double HapticSettingsTool::sWorkspaceRadius;
bool HapticSettingsTool::sHapticButton1;
bool HapticSettingsTool::sHapticButton2;
bool HapticSettingsTool::sHapticButton1Last;
bool HapticSettingsTool::sHapticButton2Last;
bool HapticSettingsTool::sDeviceAvailable;
MVector HapticSettingsTool::sSpringDamper;
bool HapticSettingsTool::sSpringDamperSet;

//----------------------------------------------------------------------------------------------------
// HapticSettingsToolCommand
//----------------------------------------------------------------------------------------------------

const MString HapticSettingsToolCommand::sCommandName( "StubbleHapticSettingsToolCommand" );

const char *bigBrushFlag = "-bb", *bigBrushLongFlag = "-bigBrush";
const char *interactiveSelectonFlag = "-is", *interactiveSelectonLongFlag = "-interactiveSelection";
const char *integratorRatioFlag = "-ir", *integratorRatioLongFlag = "-integratorRatio";
const char *proxyRadiusFlag = "-pr", *proxyRadiusLongFlag = "-proxyRadius";
const char *radiusFlag = "-r", *radiusLongFlag = "-radius";
const char *proxyDetailFlag = "-d", *proxyDetailLongFlag = "-proxyDetail";
const char *initFlag = "-i", *initLongFlag = "-init";

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
	HapticSettingsTool::sHapticButton1 = false;
	HapticSettingsTool::sHapticButton2 = false;
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
	
	syn.addFlag( bigBrushFlag, bigBrushLongFlag, MSyntax::kBoolean );
	syn.addFlag( interactiveSelectonFlag, interactiveSelectonLongFlag, MSyntax::kBoolean );
	syn.addFlag( integratorRatioFlag, integratorRatioLongFlag, MSyntax::kDouble );
	syn.addFlag( proxyRadiusFlag, proxyRadiusLongFlag, MSyntax::kDouble );
	syn.addFlag( proxyDetailFlag, proxyDetailLongFlag, MSyntax::kLong );
	syn.addFlag( radiusFlag, radiusLongFlag, MSyntax::kLong );
	syn.addFlag( initFlag, initLongFlag, MSyntax::kString );

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

	HapticSettingsTool::sSpringDamperSet = false;
}

HapticSettingsTool::~HapticSettingsTool()
{
	HapticSettingsTool::sHapticThreadRunning = false;

	if ( HapticSettingsTool::sHapticDevice != NULL )
	{
		HapticSettingsTool::sHapticDevice->close();
	}
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

	int sleepTime = 10;
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

		if ( HapticSettingsTool::sSpringDamperSet )
		{
			// compute force in robotic coord system
			MVector forceVector = 2.0 * ( HapticSettingsTool::sSpringDamper / HapticSettingsTool::sWorkspaceRadius - HapticSettingsTool::sLastPosition / HapticSettingsTool::sWorkspaceRadius );

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
