#ifndef STUBBLE_GENERIC_TOOL_HPP
#define STUBBLE_GENERIC_TOOL_HPP

#include <maya/M3dView.h>
#include <maya/MPxContext.h>
#include "maya/MPxContextCommand.h"
#include "../ToolShapes/ToolShape.hpp"

namespace Stubble
{

namespace Toolbox
{

class GenericTool
	: public MPxContext
{
public:

    ///----------------------------------------------------------------------------------------------------
	/// Default constructor
	///----------------------------------------------------------------------------------------------------
	GenericTool();

	///----------------------------------------------------------------------------------------------------
	/// Constructor specifying the tool shape
	/// \param aToolShape The shape of the tool
	///----------------------------------------------------------------------------------------------------
	GenericTool(ToolShape *aToolShape);
	
	///----------------------------------------------------------------------------------------------------
	/// The method which returns the current 3D view.
	/// \return	The current 3D view. 
	///----------------------------------------------------------------------------------------------------
	M3dView* getActiveView();

	///----------------------------------------------------------------------------------------------------
	/// Draws the current tool shape on the screen by delegating the request to its ToolShape object.
	///----------------------------------------------------------------------------------------------------
	void drawToolShape( short aScreenCoords[ 2 ], QEvent::Type aEventType );

	///----------------------------------------------------------------------------------------------------
	/// Returns the tool shape.
	/// \return The tool shape
	///----------------------------------------------------------------------------------------------------
	ToolShape *getToolShape();
		
	///----------------------------------------------------------------------------------------------------
	/// Changes the current tool shape. Called every time the user changes
	/// his preference in the user interface. 
	///----------------------------------------------------------------------------------------------------
	virtual void changeToolShape() = 0; 

	///----------------------------------------------------------------------------------------------------
	/// Returns the current tool scale, as selected in the UI. 
	///----------------------------------------------------------------------------------------------------
	double getToolScale();

protected:

	ToolShape *mShape; ///< Current brush shape (circle, texture shape...).

	M3dView mView; ///< The view in which we are currently operating.

	double mScale; ///< The scale of the tool (passed from the UI).
};



///----------------------------------------------------------------------------------------------------
/// This class represents a simple mel command that will allow
/// the user to enter a context for our tool. It simply has to return
/// an instance of the Context class.
///----------------------------------------------------------------------------------------------------
class GenericToolCommand : 
	public MPxContextCommand
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Class constructor.
	///----------------------------------------------------------------------------------------------------
	GenericToolCommand();

	///----------------------------------------------------------------------------------------------------
	/// Class destructor.
	///----------------------------------------------------------------------------------------------------
	virtual ~GenericToolCommand();

	virtual MStatus	appendSyntax();
};



} // namespace Toolbox

} // namespace Stubble

#endif  // STUBBLE_GENERIC_TOOL_HPP
