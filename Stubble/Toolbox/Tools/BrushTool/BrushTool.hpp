#ifndef STUBBLE_BRUSH_TOOL_HPP
#define STUBBLE_BRUSH_TOOL_HPP

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
#include "../HapticListener.hpp"
#include "../HapticSettingsTool.hpp"

#include "../../HairTaskProcessor.hpp"

#include "../../BrushModes/BrushMode.hpp"
#include "../../BrushModes/ClumpBrushMode/ClumpBrushMode.hpp"
#include "../../BrushModes/PuffEndBrushMode/PuffEndBrushMode.hpp"
#include "../../BrushModes/PuffRootBrushMode/PuffRootBrushMode.hpp"
#include "../../BrushModes/RotateBrushMode/RotateBrushMode.hpp"
#include "../../BrushModes/ScaleBrushMode/ScaleBrushMode.hpp"
#include "../../BrushModes/TranslateBrushMode/TranslateBrushMode.hpp"
#include "../../ToolShapes/CircleToolShape/CircleToolShape.hpp"
#include "../../ToolShapes/SphereToolShape/SphereToolShape.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// This class implements the brush tool.
///----------------------------------------------------------------------------------------------------
class BrushTool :
	public GenericTool
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Class constructor.
	///----------------------------------------------------------------------------------------------------
	BrushTool();

	///----------------------------------------------------------------------------------------------------
	/// Class destructor.
	///----------------------------------------------------------------------------------------------------
	virtual ~BrushTool();

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
	virtual MStatus	doPress( MEvent &aEvent );

	///----------------------------------------------------------------------------------------------------
	/// Handles the haptic press event.
	///----------------------------------------------------------------------------------------------------
	virtual void doHapticPress();

	///----------------------------------------------------------------------------------------------------
	/// Handles the haptic drag event.
	///
	/// \param aDragVector Haptic eye space coordinates
	///----------------------------------------------------------------------------------------------------
	virtual void doHapticDrag( MVector &aDragVector );

	///----------------------------------------------------------------------------------------------------
	/// Handles the haptic release event.
	///----------------------------------------------------------------------------------------------------
	virtual void doHapticRelease();

	///----------------------------------------------------------------------------------------------------
	/// Handles the mouse drag event.
	///
	/// \param aEvent Information on the input event.
	///
	/// \return	MS::kSuccess or MS::kFailure.
	///----------------------------------------------------------------------------------------------------
	virtual MStatus	doDrag( MEvent &aEvent );

	///----------------------------------------------------------------------------------------------------
	/// Handles the mouse release event.
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
	/// The method which performs the brushing transformation by dispatching a HairTask to the
	/// HairTaskProcessor which takes care of the rest in the asynchronous loop.
	///
	/// \param aDX Position change in the screen coordinates
	///----------------------------------------------------------------------------------------------------
	void doBrush( Vector3D< double > aDX );

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
	/// Changes the current brush mode. Called by BrushToolCommand every time the user changes
	/// his preference in the user interface. 
	///----------------------------------------------------------------------------------------------------
	void changeBrushMode();

	///----------------------------------------------------------------------------------------------------
	/// Takes currently selected guides and filters them against current hair shape. Stores it inside
	/// mAffectedGuides for further processing.
	///----------------------------------------------------------------------------------------------------
	void filterAffectedGuides();

	///----------------------------------------------------------------------------------------------------
	/// Takes currently selected guides and filters them against current hair shape using haptic device. 
	/// Stores it inside mAffectedGuides for further processing.
	///----------------------------------------------------------------------------------------------------
	void filterAffectedGuidesHaptic();

	///----------------------------------------------------------------------------------------------------
	/// Takes care when the user changes tool shape from the UI
	///----------------------------------------------------------------------------------------------------
	virtual void changeToolShape();

	Stubble::HairShape::HairComponents::SelectedGuides mAffectedGuides; ///< Guides currently affected by present operation

	double mSensitivity; ///< Brush sensitivity.

	bool mEnableFalloff; ///< Enable brush falloff.

	bool mEnableCollisionDetection; ///< Enable brush collision detection.

	int mBrushModeChoice; ///< The index of the brush mode to be used (translate, rotate, scale, clump...)

	BrushMode *mBrushMode; ///< Current brush mode (pointing to one of the static brush modes below).

	static const Real SENSITIVITY_RATIO; ///< Constant for calculating move vector from the mouse movement.

	// State pattern singletons
	static ClumpBrushMode sClumpBrushMode;
	static PuffEndBrushMode sPuffEndBrushMode;
	static PuffRootBrushMode sPuffRootBrushMode;
	static RotateBrushMode sRotateBrushMode;
	static ScaleBrushMode sScaleBrushMode;
	static TranslateBrushMode sTranslateBrushMode;

private:

	///----------------------------------------------------------------------------------------------------
	/// An instance of BrushToolCommand will be responsible for initiation of the brushMode 
	/// state transitions by calling the protected method changeBrushMode().
	///----------------------------------------------------------------------------------------------------
	friend class BrushToolCommand;

	short mStartPos[ 2 ]; ///< The location of the cursor when the mouse was first clicked.

	short mPrevPos[ 2 ]; ///< The previous location of the cursor during dragging.

	short mEndPos[ 2 ]; ///< The location of the cursor when the mouse gets released.

	MGlobal::MSelectionMode mPrevSelMode; ///< Previous selection mode, used when undoing the command.
	
	MSelectionMask mPrevCompMask; ///< Previous component selection mask, used when undoing the command.

	MSelectionMask mPrevObjMask; ///< Previous object selection mask, used when undoing the command.

	int mClickBoxSize; ///< GUI click box size.

	static const MString sHelpTxt; ///< Help text that will be displayed in the help line
};


///----------------------------------------------------------------------------------------------------
/// This class represents a simple mel command that will allow
/// the user to enter a context for our tool. It simply has to return
/// an instance of the Context class.
///----------------------------------------------------------------------------------------------------
class BrushToolCommand : 
	public GenericToolCommand
{
public:

	///----------------------------------------------------------------------------------------------------
	/// This method needs to return a context for maya to use
	/// when our tool requires input from the viewport.
	///
	/// \return	a new instance of BrushTool
	///----------------------------------------------------------------------------------------------------
	virtual MPxContext* makeObj();

	///----------------------------------------------------------------------------------------------------
	/// This method returns a new instance of the tool command for maya to use.
	///
	/// \return	an instance of BrushToolCommand
	///----------------------------------------------------------------------------------------------------
	static void* creator();

	///----------------------------------------------------------------------------------------------------
	/// Class constructor.
	///----------------------------------------------------------------------------------------------------
	BrushToolCommand();

	///----------------------------------------------------------------------------------------------------
	/// Class destructor.
	///----------------------------------------------------------------------------------------------------
	virtual ~BrushToolCommand();

	// the methods that are necessary for the user interface to communicate with the internals
	virtual	MStatus	doEditFlags();
	virtual MStatus	doQueryFlags();

	static const MString sCommandName;

protected:

	BrushTool *mCurrentBrushToolObject; ///< The current BrushTool object.
};

} // namespace Toolbox

} // namespace Stubble

#endif // STUBBLE_BRUSH_TOOL_HPP
