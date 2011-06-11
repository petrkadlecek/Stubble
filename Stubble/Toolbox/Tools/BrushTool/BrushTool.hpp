#ifndef STUBBLE_BRUSH_TOOL_HPP
#define STUBBLE_BRUSH_TOOL_HPP

#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MPxContext.h>
#include <maya/MPxContextCommand.h>

#include "../../BrushModes/BrushMode.hpp"
#include "../../BrushModes/ClumpBrushMode/ClumpBrushMode.hpp"
#include "../../BrushModes/PuffEndBrushMode/PuffEndBrushMode.hpp"
#include "../../BrushModes/PuffRootBrushMode/PuffRootBrushMode.hpp"
#include "../../BrushModes/RotateBrushMode/RotateBrushMode.hpp"
#include "../../BrushModes/ScaleBrushMode/ScaleBrushMode.hpp"
#include "../../BrushModes/TranslateBrushMode/TranslateBrushMode.hpp"
#include "../../ToolShapes/CircleToolShape/CircleToolShape.hpp"


namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// This class implements the brush tool.
///----------------------------------------------------------------------------------------------------
class BrushTool :
	public MPxContext
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
	/// \param aEvent Information on the input event.
	///----------------------------------------------------------------------------------------------------
	virtual void toolOnSetup( MEvent &aEvent );

	///----------------------------------------------------------------------------------------------------
	/// Cleans up when the user switches to a different tool.
	///----------------------------------------------------------------------------------------------------
	virtual void toolOffCleanup();

	///----------------------------------------------------------------------------------------------------
	/// Handles the mouse press event.
	/// \param aEvent Information on the input event.
	/// \return	MS::kSuccess or MS::kFailure.
	///----------------------------------------------------------------------------------------------------
	virtual MStatus	doPress( MEvent &aEvent );

	///----------------------------------------------------------------------------------------------------
	/// Handles the mouse drag event.
	/// \param aEvent Information on the input event.
	/// \return	MS::kSuccess or MS::kFailure.
	///----------------------------------------------------------------------------------------------------
	virtual MStatus	doDrag( MEvent &aEvent );

	///----------------------------------------------------------------------------------------------------
	/// Handles the mouse release event.
	/// \param aEvent Information on the input event.
	/// \return	MS::kSuccess or MS::kFailure.
	///----------------------------------------------------------------------------------------------------
	virtual MStatus	doRelease( MEvent &aEvent );

	///----------------------------------------------------------------------------------------------------
	/// Provides the class name.
	/// \param aName Placeholder for the return value.
	///----------------------------------------------------------------------------------------------------
	virtual void getClassName( MString &aName ) const;

	///----------------------------------------------------------------------------------------------------
	/// The method which performs the brushing transformations by calling the BrushMode's doBrush() method.
	///----------------------------------------------------------------------------------------------------
	void doBrush( MVector aDX, float aDT );

	int mBrushModeChoice; ///< The index of the brush mode to be used (translate, rotate, scale, clump...)

	/*TODO*/
	/*/// removes the widget that draws the visual aid
	static void cleanup();

	/// the widget that draws the visual aid
	static QPointer<CoolWidget>	coolwdg;*/

protected:

	///----------------------------------------------------------------------------------------------------
	/// Changes the current brush mode. Called by BrushToolCommand every time the user changes
	/// his preference in the user interface. 
	///----------------------------------------------------------------------------------------------------
	void changeBrushMode();

	float mSensitivity; ///< Brush sensitivity.

	float mFalloff; ///< Brush falloff.

	ToolShape *mShape; ///< Current brush shape (circle, texture shape...).

	BrushMode *mBrushMode; ///< Current brush mode (pointing to one of the static brush modes below).

	static ClumpBrushMode clumpBrushMode;
	static PuffEndBrushMode puffEndBrushMode;
	static PuffRootBrushMode puffRootBrushMode;
	static RotateBrushMode rotateBrushMode;
	static ScaleBrushMode scaleBrushMode;
	static TranslateBrushMode translateBrushMode;

private:

	///----------------------------------------------------------------------------------------------------
	/// An instance of BrushToolCommand will be responsible for initiation of the brushMode 
	/// state transitions by calling the protected method changeBrushMode().
	///----------------------------------------------------------------------------------------------------
	friend class BrushToolCommand;

	short mStartPos[ 2 ]; ///< The location of the cursor when the mouse was first clicked.

	short mPrevPos[ 2 ]; ///< The previous location of the cursor during dragging.

	short mEndPos[ 2 ]; ///< The location of the cursor when the mouse gets released.

	M3dView mView; ///< The view in which we are doing the brushing.

	MGlobal::MSelectionMode mPrevSelMode; ///< Previous selection mode, used when undoing the command.
	
	MSelectionMask mPrevCompMask; ///< Previous component selection mask, used when undoing the command.
	MSelectionMask mPrevObjMask; ///< Previous object selection mask, used when undoing the command.

	int mClickBoxSize; ///< GUI click box size.

	static const MString sHelpTxt; ///< Help text that will be displayed in the help line.
};


///----------------------------------------------------------------------------------------------------
/// This class represents a simple mel command that will allow
/// the user to enter a context for our tool. It simply has to return
/// an instance of the Context class.
///----------------------------------------------------------------------------------------------------
class BrushToolCommand : 
	public MPxContextCommand
{
public:
	///----------------------------------------------------------------------------------------------------
	/// This method needs to return a context for maya to use
	/// when our tool requires input from the viewport.
	/// \return	a new instance of BrushTool
	///----------------------------------------------------------------------------------------------------
	virtual MPxContext* makeObj();

	///----------------------------------------------------------------------------------------------------
	/// This method returns a new instance of the tool command for maya to use.
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
	virtual MStatus	appendSyntax();

	static const MString sCommandName;

protected:

	BrushTool *mCurrentBrushToolObject; ///< The current BrushTool object.
};

} // namespace Toolbox

} // namespace Stubble

#endif // STUBBLE_BRUSH_TOOL_HPP