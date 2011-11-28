#include "HapticSettingsTool.hpp"

namespace Stubble
{

namespace Toolbox
{

// ----------------------------------------------------------------------------
// Static data members
// ----------------------------------------------------------------------------
cHapticDeviceHandler* HapticSettingsTool::mHandler;
cGenericHapticDevice* HapticSettingsTool::mHapticDevice;
bool HapticSettingsTool::mHapticThreadRunning;
MVector HapticSettingsTool::mLastPosition;
bool HapticSettingsTool::mHapticButton1;
bool HapticSettingsTool::mHapticButton2;

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
}

HapticSettingsToolCommand::~HapticSettingsToolCommand()
{
	delete mCurrentObject;
}

MStatus	HapticSettingsToolCommand::doEditFlags()
{
	MArgParser pars = parser();

  //std::cout << "doEditFlags" << std::endl;
	// TODO

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

  //std::cout << "doQueryFlags" << std::endl;
	// TODO

	return MS::kSuccess;
}

MStatus	HapticSettingsToolCommand::appendSyntax()
{
	MSyntax syn = syntax();

  //std::cout << "appendSyntax" << std::endl;
	
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
  HapticSettingsTool::mHapticThreadRunning = false;
  
  // set CHAI3D handler
  HapticSettingsTool::mHandler = NULL;
  HapticSettingsTool::mHapticDevice = NULL;
}

HapticSettingsTool::~HapticSettingsTool()
{
  HapticSettingsTool::mHapticThreadRunning = false;
  
  if (HapticSettingsTool::mHapticDevice != NULL)
  {
    HapticSettingsTool::mHapticDevice->close();
  }
}

MVector HapticSettingsTool::getLastPosition()
{
  return mLastPosition;
}

void HapticSettingsTool::getClassName( MString &aName ) const
{ 
	aName = "StubbleHapticSettingsTool";
}

void HapticSettingsTool::toolOnSetup( MEvent &event )
{
	if (mInitFlag) {
		MString s = "confirmDialog -title \"HapticSettingsTool\" -message \"Initialization already completed.\" -button \"OK\";";
		MGlobal::executeCommand(s);

		return;
	}

	mInitFlag = true;

  // create instance of mHandler on ToolSetup
  if (HapticSettingsTool::mHandler == NULL)
  { 
    HapticSettingsTool::mHandler = new cHapticDeviceHandler();
  }

  cHapticDeviceInfo info; // haptic device info structure

  // clear scrollList
	MGlobal::executeCommandOnIdle("textScrollList -e -ra hapticDevices;");

  // debug
  std::cout << "HapticSettingsTool: number of detected haptic devices = " << HapticSettingsTool::mHandler->getNumDevices() << std::endl;

  assert(mHandler != NULL);

  // add devices
	for ( unsigned int i = 0; i < HapticSettingsTool::mHandler->getNumDevices(); ++i)
	{
		HapticSettingsTool::mHandler->getDeviceSpecifications(info, i);

		MString sCmd = "textScrollList -e -a \"";
		sCmd += i;
		sCmd += ": ";
		sCmd += info.m_manufacturerName.c_str();
		sCmd += " ";
		sCmd += info.m_modelName.c_str();
		sCmd += "\" hapticDevices;";

		MGlobal::executeCommandOnIdle(sCmd);
	}

	mView = M3dView::active3dView();
}

void HapticSettingsTool::toolOffCleanup()
{
  // do not close haptic device here
}

MThreadRetVal HapticSettingsTool::AsyncHapticLoop(void *aData)
{
	MGlobal::displayInfo("HapticSettingsTool: start loop");

	MStatus stat;
	MVector vector;

	static cVector3d newPosition;
	static cVector3d force;
	force.zero();

	int sleepTime = 200; // todo

	M3dView mView = M3dView::active3dView();

  HapticSettingsTool::mHapticThreadRunning = true;

  while (HapticSettingsTool::mHapticThreadRunning)
	{
		HapticSettingsTool::mHapticDevice->getPosition(newPosition);
		HapticSettingsTool::mHapticDevice->setForce(force);
		HapticSettingsTool::mHapticDevice->getUserSwitch(0, HapticSettingsTool::mHapticButton1);
		HapticSettingsTool::mHapticDevice->getUserSwitch(1, HapticSettingsTool::mHapticButton2);
		
		HapticSettingsTool::mLastPosition.x = newPosition.y * 30.0;
		HapticSettingsTool::mLastPosition.y = newPosition.z * 30.0;
		HapticSettingsTool::mLastPosition.z = newPosition.x * 30.0;

    MGlobal::executeCommandOnIdle("refresh");

    std::cout << "HapticSettingsTool::AsyncHapticLoop " <<  newPosition << std::endl;

		Sleep(sleepTime + 1);
	}

	return 0;
}

void HapticSettingsTool::AsyncHapticCallback (void *aData)
{
  // TODO: handle callback
}

void HapticSettingsTool::initHapticDevice( int aHapticDeviceIndex )
{
  std::cout << "HapticSettingsTool: preparing device " << aHapticDeviceIndex << std::endl;
  
	if (HapticSettingsTool::mHandler->getDevice(HapticSettingsTool::mHapticDevice, aHapticDeviceIndex) == 0)
	{
		MGlobal::displayInfo("HapticSettingsTool: open");
		if (mHapticDevice->open() == 0)
		{
			MGlobal::displayInfo("HapticSettingsTool: init");

			if (HapticSettingsTool::mHapticDevice->initialize() == 0)
			{
				MStatus pr = MThreadAsync::init();
        pr = MThreadAsync::createTask(HapticSettingsTool::AsyncHapticLoop, 0, HapticSettingsTool::AsyncHapticCallback, NULL);
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
