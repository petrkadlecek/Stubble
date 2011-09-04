#include "HapticSettingsTool.hpp"
#include <chai3d.h> // CHAI 3D - haptics support

namespace Stubble
{

namespace Toolbox
{

//----------------------------------------------------------------------------------------------------
// HapticSettingsToolCommand
//----------------------------------------------------------------------------------------------------

const MString HapticSettingsToolCommand::sCommandName( "StubbleHapticSettingsToolCommand" );

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

	// TODO

	return MS::kSuccess;
}

MStatus	HapticSettingsToolCommand::doQueryFlags()
{
	MArgParser pars = parser();

	// TODO

	return MS::kSuccess;
}

MStatus	HapticSettingsToolCommand::appendSyntax()
{
	MSyntax syn = syntax();

	// TODO

	return MS::kSuccess;
}


//----------------------------------------------------------------------------------------------------
// HapticSettingsTool
//----------------------------------------------------------------------------------------------------

HapticSettingsTool::HapticSettingsTool()
{
	setTitleString( "Stubble haptic settings Tool" );
	mInitFlag = false;
}

HapticSettingsTool::~HapticSettingsTool()
{
}

void HapticSettingsTool::toolOnSetup( MEvent &event )
{
	if (mInitFlag) {
		MString s = "confirmDialog -title \"HapticSettingsTool\" -message \"Initialization already completed.\" -button \"OK\";";
		MGlobal::executeCommand(s);

		return;
	}
	mInitFlag = true;

	// Testing initialization

	cHapticDeviceHandler* handler = new cHapticDeviceHandler();
	
	MString s = "confirmDialog -title \"HapticSettingsTool\" -message \"Number of haptic devices: ";
	s += handler->getNumDevices();
	s += "\" -button \"OK\";";
	MGlobal::executeCommand(s);

	if (handler->getNumDevices() > 0)
	{
		cGenericHapticDevice* hapticDevice;
		handler->getDevice(hapticDevice, 0);
		hapticDevice->open();
		hapticDevice->initialize();
		cVector3d newPosition(0, 0, 0);
		hapticDevice->setForce(newPosition);
		hapticDevice->getPosition(newPosition);
		hapticDevice->close();
	}

	delete handler;

	mView = M3dView::active3dView();
}


void HapticSettingsTool::toolOffCleanup()
{
}

} // namespace Toolbox

} // namespace Stubble
