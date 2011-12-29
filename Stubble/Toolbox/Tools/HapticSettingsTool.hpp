#ifndef STUBBLE_HAPTIC_SETTINGS_TOOL_HPP
#define STUBBLE_HAPTIC_SETTINGS_TOOL_HPP

#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MPxContext.h>
#include <maya/MPxContextCommand.h>
#include <chai3d.h> // CHAI 3D - haptics support

#include <maya/MTimer.h>
#include <maya/MThreadAsync.h>
#include <maya/MThreadPool.h>
#include <maya/MSpinLock.h>
#include <maya/MMutexLock.h>

#include <maya/MPoint.h>
#include <maya/MVector.h>

#include "HapticListener.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// This class implements the haptic settings tool.
///----------------------------------------------------------------------------------------------------
class HapticSettingsTool
	: public MPxContext
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Class constructor.
	///----------------------------------------------------------------------------------------------------
	HapticSettingsTool();

	///----------------------------------------------------------------------------------------------------
	/// Class destructor.
	///----------------------------------------------------------------------------------------------------
	virtual ~HapticSettingsTool();

	///----------------------------------------------------------------------------------------------------
	/// The method which returns the current 3D view.
	/// \return	The current 3D view. 
	///----------------------------------------------------------------------------------------------------
	M3dView* getActiveView();

	///----------------------------------------------------------------------------------------------------
	/// Provides the class name.
	///
	/// \param aName Placeholder for the return value.
	///----------------------------------------------------------------------------------------------------
	virtual void getClassName( MString &aName ) const;

	///----------------------------------------------------------------------------------------------------
	/// The method is called on tool setup
	///----------------------------------------------------------------------------------------------------
	virtual void toolOnSetup( MEvent &event );

	///----------------------------------------------------------------------------------------------------
	/// Haptic Tool spring damper position setter for force calculation
	///----------------------------------------------------------------------------------------------------
	static void setCollisionVector( MVector &aCollisionVector );

	///----------------------------------------------------------------------------------------------------
	/// Haptic Tool spring damper position setter for force calculation
	///----------------------------------------------------------------------------------------------------
	static void setSpringDamper();

	///----------------------------------------------------------------------------------------------------
	/// Haptic Tool spring damper position unsetter for force calculation
	///----------------------------------------------------------------------------------------------------
	static void unsetSpringDamper();

	///----------------------------------------------------------------------------------------------------
	/// Haptic Tool position getter
	///----------------------------------------------------------------------------------------------------
	static MVector getLastPosition();

	///----------------------------------------------------------------------------------------------------
	/// Haptic Tool rotation getter
	///----------------------------------------------------------------------------------------------------
	static MVector getLastRotation();

	///----------------------------------------------------------------------------------------------------
	/// Haptic Tool rotation angle getter
	///----------------------------------------------------------------------------------------------------
	static double getLastRotationAngle();

	///----------------------------------------------------------------------------------------------------
	/// Haptic Tool button1 state getter
	///----------------------------------------------------------------------------------------------------
	static bool getHapticButton1State();

	///----------------------------------------------------------------------------------------------------
	/// Haptic Tool button2 state getter
	///---------------------------------------------------
	static bool getHapticButton2State();

	///----------------------------------------------------------------------------------------------------
	/// The method is called on tool cleanup
	///----------------------------------------------------------------------------------------------------
	virtual void toolOffCleanup();

	///----------------------------------------------------------------------------------------------------
	/// Haptic device availability flag
	///----------------------------------------------------------------------------------------------------
	static bool sDeviceAvailable; ///< is there any haptic device available

protected:

	M3dView mView; ///< The view in which we are currently operating.

private:
	friend class HapticSettingsToolCommand;

	bool mInitFlag; ///< initialization flag

	///----------------------------------------------------------------------------------------------------
	/// CHAI 3D haptic device handlers
	///----------------------------------------------------------------------------------------------------
	static cHapticDeviceHandler* sHandler;  ///< handler of haptic devices CHAI 3D

	static cGenericHapticDevice* sHapticDevice;  ///< haptic device CHAI 3D

	///----------------------------------------------------------------------------------------------------
	/// CHAI 3D device control
	///----------------------------------------------------------------------------------------------------
	static cVector3d sDevicePosition; ///< haptic device position

	static cMatrix3d sDeviceRotation; ///< haptic device rotation

	static cVector3d sDeviceForce; ///< haptic device sent force

	static bool sHapticButton1; ///< haptic device button 1

	static bool sHapticButton2; ///< haptic device button 2

	static double sWorkspaceRadius; ///< haptic device workspace radius

	///----------------------------------------------------------------------------------------------------
	/// Stubble Haptic Tool properties
	///----------------------------------------------------------------------------------------------------
	MString mHapticDeviceStr; ///< haptic device string identifier

	static bool sHapticThreadRunning; ///< haptic thread running flag

	static MVector sSpringDamper;  ///< spring damper force vector

	static bool sSpringDamperSet;  ///< spring damper force vector set flag

	static MVector sLastPosition;  ///< last position of haptic device

	static MVector sLastRotation;  ///< last rotation vector of haptic device

	static double sLastRotationAngle;  ///< last rotation angle of haptic device

	static bool sHapticButton1Last; ///< last haptic device button 1 state

	static bool sHapticButton2Last; ///< last haptic device button 2 state

	///----------------------------------------------------------------------------------------------------
	/// Initialize haptic device by index aHapticDeviceIndex
	///----------------------------------------------------------------------------------------------------
	void initHapticDevice( int aHapticDeviceIndex );

	///----------------------------------------------------------------------------------------------------
	/// Asynchronous haptic thread loop and callback handler
	///----------------------------------------------------------------------------------------------------
	static MThreadRetVal AsyncHapticLoop(void *aData);

	static void AsyncHapticCallback (void *aData);
};


///----------------------------------------------------------------------------------------------------
/// This class represents a simple mel command that will allow
/// the user to enter a context for our tool. It simply has to return
/// an instance of the Context class.
///----------------------------------------------------------------------------------------------------
class HapticSettingsToolCommand :
	public MPxContextCommand
{
public:

	virtual MPxContext* makeObj();

	///----------------------------------------------------------------------------------------------------
	/// This method returns a new instance of the tool command for maya to use.
	/// \return	an instance of BrushToolCommand
	///----------------------------------------------------------------------------------------------------
	static void* creator();

	///----------------------------------------------------------------------------------------------------
	/// Class constructor.
	///----------------------------------------------------------------------------------------------------
	HapticSettingsToolCommand();

	///----------------------------------------------------------------------------------------------------
	/// Class destructor.
	///----------------------------------------------------------------------------------------------------
	virtual ~HapticSettingsToolCommand();

	virtual	MStatus	doEditFlags();
	virtual MStatus	doQueryFlags();
	virtual MStatus	appendSyntax();

	static const MString sCommandName;

protected:
	HapticSettingsTool *mCurrentObject; ///< The current HapticSettingsTool object.

}; // class HapticSettingsToolCommand

} // namespace Toolbox

} // namespace Stubble

#endif  // STUBBLE_HAPTIC_SETTINGS_TOOL_HPP
