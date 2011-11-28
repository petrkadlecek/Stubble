#ifndef STUBBLE_TABLET_SETTINGS_TOOL_HPP
#define STUBBLE_TABLET_SETTINGS_TOOL_HPP

#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MPxContext.h>
#include <maya/MPxContextCommand.h>

#include <maya/MTimer.h>
#include <maya/MThreadAsync.h>
#include <maya/MThreadPool.h>
#include <maya/MSpinLock.h>
#include <maya/MMutexLock.h>

#include <maya/MPoint.h>
#include <maya/MVector.h>

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// This class implements the haptic settings tool.
///----------------------------------------------------------------------------------------------------
class TabletSettingsTool
	: public MPxContext
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Class constructor.
	///----------------------------------------------------------------------------------------------------
	TabletSettingsTool();

	///----------------------------------------------------------------------------------------------------
	/// Class destructor.
	///----------------------------------------------------------------------------------------------------
	virtual ~TabletSettingsTool();

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
	/// The method is called on tool cleanup
	///----------------------------------------------------------------------------------------------------
	virtual void toolOffCleanup();


protected:

	M3dView mView; ///< The view in which we are currently operating.

private:
  friend class HapticSettingsToolCommand;

	bool mInitFlag; ///< initialization flag

	///----------------------------------------------------------------------------------------------------
	/// Initialize WINTAB support
	///----------------------------------------------------------------------------------------------------
  void initTablet();
};


///----------------------------------------------------------------------------------------------------
/// This class represents a simple mel command that will allow
/// the user to enter a context for our tool. It simply has to return
/// an instance of the Context class.
///----------------------------------------------------------------------------------------------------
class TabletSettingsToolCommand :
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
	TabletSettingsToolCommand();

	///----------------------------------------------------------------------------------------------------
	/// Class destructor.
	///----------------------------------------------------------------------------------------------------
	virtual ~TabletSettingsToolCommand();

	virtual	MStatus	doEditFlags();
	virtual MStatus	doQueryFlags();
	virtual MStatus	appendSyntax();

	static const MString sCommandName;

protected:
	TabletSettingsTool *mCurrentObject; ///< The current TabletSettingsTool object.

}; // class TabletSettingsToolCommand

} // namespace Toolbox

} // namespace Stubble

#endif  // STUBBLE_TABLET_SETTINGS_TOOL_HPP
