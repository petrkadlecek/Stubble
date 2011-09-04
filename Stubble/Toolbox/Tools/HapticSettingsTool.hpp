#ifndef STUBBLE_HAPTIC_SETTINGS_TOOL_HPP
#define STUBBLE_HAPTIC_SETTINGS_TOOL_HPP

#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MPxContext.h>
#include <maya/MPxContextCommand.h>


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

	bool mInitFlag; ///< initialization flag
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
