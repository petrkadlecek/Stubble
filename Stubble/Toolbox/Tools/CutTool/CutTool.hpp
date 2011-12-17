#ifndef STUBBLE_Cut_TOOL_HPP
#define STUBBLE_Cut_TOOL_HPP

#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MPxContext.h>
#include <maya/MPxContextCommand.h>
#include <maya/MCursor.h>
#include <maya/MFnCamera.h>
#include <maya/MDagPath.h>

#include <maya/MQtUtil.h>
#include <QtCore/QPointer>
#include <QtGui/QApplication>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QMouseEvent>

#include "../../../Primitives/Vector3D.hpp"

#include "../../../HairShape/UserInterface/HairShape.hpp"

#include "../GenericTool.hpp"
#include "../MouseMoveListener.hpp"

#include "../../ToolShapes/CircleToolShape/CircleToolShape.hpp"


namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// This class implements the Cut tool.
///----------------------------------------------------------------------------------------------------
class CutTool :
	public GenericTool
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Class constructor.
	///----------------------------------------------------------------------------------------------------
	CutTool();

	///----------------------------------------------------------------------------------------------------
	/// Class destructor.
	///----------------------------------------------------------------------------------------------------
	virtual ~CutTool();

	///----------------------------------------------------------------------------------------------------
	/// Sets up any user interface things we need when the tool	is made current.
	///
	/// \param aEvent Information on the input event.
	///----------------------------------------------------------------------------------------------------
	virtual void toolOnSetup( MEvent &aEvent );

	///----------------------------------------------------------------------------------------------------
	/// Cleans up when the user switches to a different tool.
	///----------------------------------------------------------------------------------------------------
	virtual void toolOffCleanup();

	///----------------------------------------------------------------------------------------------------
	/// Handles the mouse press event.
	///
	/// \param aEvent Information on the input event.
	///
	/// \return	MS::kSuccess or MS::kFailure.
	///----------------------------------------------------------------------------------------------------
	virtual MStatus doPress( MEvent &aEvent );

	///----------------------------------------------------------------------------------------------------
	/// Handles the haptic press event.
	///----------------------------------------------------------------------------------------------------
	virtual void doHapticPress();

	///----------------------------------------------------------------------------------------------------
	/// Handles the haptic drag event.
	///----------------------------------------------------------------------------------------------------
	virtual void doHapticDrag( MVector &aDragVector );

	///----------------------------------------------------------------------------------------------------
	/// Handles the haptic release event.
	///----------------------------------------------------------------------------------------------------
	virtual void doHapticRelease();

	///----------------------------------------------------------------------------------------------------
	/// Handles the mouse release event. Calls doCut() if left mouse button is released.
	///
	/// \param aEvent Information on the input event.
	///
	/// \return	MS::kSuccess or MS::kFailure.
	///----------------------------------------------------------------------------------------------------
	virtual MStatus	doRelease( MEvent &aEvent );

	///----------------------------------------------------------------------------------------------------
	/// Provides the class name.
	///
	/// \param aName Placeholder for the return value.
	///----------------------------------------------------------------------------------------------------
	virtual void getClassName( MString &aName ) const;

	///----------------------------------------------------------------------------------------------------
	/// Changes the current tool shape. Called every time the user changes his preference in the UI.
	///----------------------------------------------------------------------------------------------------
	virtual void changeToolShape( void );

	///----------------------------------------------------------------------------------------------------
	///  Cut the affected hair.
	///----------------------------------------------------------------------------------------------------
	void doCut();

	///----------------------------------------------------------------------------------------------------
	/// Notifies the relevant depending objects of the changes in the user interface.
	///----------------------------------------------------------------------------------------------------
	void notify();

	///----------------------------------------------------------------------------------------------------
	/// Deletes the mouse move listener.
	///----------------------------------------------------------------------------------------------------
	static void deleteMouseMoveListener();
		
	static QPointer<MouseMoveListener> sMouseMoveListener; ///< the mouse move listener

protected:

	///----------------------------------------------------------------------------------------------------
	/// Takes currently selected guides and filters them against current hair shape. Stores it inside
	/// mAffectedGuides for further processing.
	///----------------------------------------------------------------------------------------------------
	void filterAffectedGuides();
		
private:

	///----------------------------------------------------------------------------------------------------
	/// An instance of CutToolCommand will be responsible for initiation of the CutMode 
	/// state transitions by calling the protected method changeCutMode().
	///----------------------------------------------------------------------------------------------------
	friend class CutToolCommand;
	
	short mPosition[ 2 ]; ///< The location of the cursor when the mouse gets released [x, y].

	MGlobal::MSelectionMode mPrevSelMode; ///< Previous selection mode, used when undoing the command.
	
	MSelectionMask mPrevCompMask; ///< Previous component selection mask, used when undoing the command.

	MSelectionMask mPrevObjMask; ///< Previous object selection mask, used when undoing the command.

	Stubble::HairShape::HairComponents::SelectedGuides mAffectedGuides; ///< Guides currently affected by present operation

	int mClickBoxSize; ///< GUI click box size.

	static const MString sHelpTxt; ///< Help text that will be displayed in the help line
};

///----------------------------------------------------------------------------------------------------
/// This class represents a simple mel command that will allow
/// the user to enter a context for our tool. It simply has to return
/// an instance of the Context class.
///----------------------------------------------------------------------------------------------------
class CutToolCommand : 
	public GenericToolCommand
{
public:
	///----------------------------------------------------------------------------------------------------
	/// This method needs to return a context for maya to use
	/// when our tool requires input from the viewport.
	///
	/// \return	a new instance of CutTool
	///----------------------------------------------------------------------------------------------------
	virtual MPxContext* makeObj();

	///----------------------------------------------------------------------------------------------------
	/// This method returns a new instance of the tool command for maya to use.
	///
	/// \return	an instance of CutToolCommand
	///----------------------------------------------------------------------------------------------------
	static void* creator();

	///----------------------------------------------------------------------------------------------------
	/// Class constructor.
	///----------------------------------------------------------------------------------------------------
	CutToolCommand();

	///----------------------------------------------------------------------------------------------------
	/// Class destructor.
	///----------------------------------------------------------------------------------------------------
	virtual ~CutToolCommand();

	// the methods that are necessary for the user interface to communicate with the internals
	virtual	MStatus	doEditFlags();

	virtual MStatus	doQueryFlags();

	static const MString sCommandName; ///< The command name

protected:

	CutTool *mCurrentCutToolObject; ///< The current CutTool object.
};

} // namespace Toolbox

} // namespace Stubble

#endif // STUBBLE_Cut_TOOL_HPP
