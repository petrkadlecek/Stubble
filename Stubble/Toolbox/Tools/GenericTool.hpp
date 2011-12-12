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
	GenericTool ();

	///----------------------------------------------------------------------------------------------------
	/// Constructor specifying the tool shape
	///
	/// \param aToolShape The shape of the tool
	///----------------------------------------------------------------------------------------------------
	GenericTool (ToolShape *aToolShape);
	
	///----------------------------------------------------------------------------------------------------
	/// Method for returning active viewport information
	///
	/// \return	The current 3D view
	///----------------------------------------------------------------------------------------------------
	M3dView* getActiveView();

	///----------------------------------------------------------------------------------------------------
	/// Draws the current tool shape on the screen by delegating the request to its ToolShape object.
	///
	/// \param aScreenCoords Mouse screen coordinates [x, y]
	/// \param aEventType Qt event type that triggered drawing
	///----------------------------------------------------------------------------------------------------
	void drawToolShape( short aScreenCoords[ 2 ], QEvent::Type aEventType );

	/// TODO
	void drawHapticToolShape( MVector &aHapticProxyPosition );

	///----------------------------------------------------------------------------------------------------
	/// Returns the tool shape.
	///
	/// \return The tool shape
	///----------------------------------------------------------------------------------------------------
	ToolShape *getToolShape();
		
	///----------------------------------------------------------------------------------------------------
	/// Changes the current tool shape. Called every time the user changes his preference in the UI.
	///----------------------------------------------------------------------------------------------------
	virtual void changeToolShape() = 0; 

	/// TODO
	virtual void doHapticPress() = 0;

	///----------------------------------------------------------------------------------------------------
	/// Returns the current tool scale, as selected in the UI.
	///
	/// \return The scale (size) of the tool.
	///----------------------------------------------------------------------------------------------------
	Real getToolScale();

protected:

	ToolShape *mShape; ///< Current brush shape (circle, texture shape...).

	M3dView mView; ///< The view in which we are currently operating.

	Real mScale; ///< The scale of the tool (passed from the UI).
};

///----------------------------------------------------------------------------------------------------
/// This class representing a simple mel command that will allow the user to enter a context for our
/// tool. It simply has to return an instance of the Context class. See Maya API reference for more
/// information
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

	///----------------------------------------------------------------------------------------------------
	/// Used for MEL command syntax manipulation. See Maya API reference for more information.
	///
	/// \return Maya status
	///----------------------------------------------------------------------------------------------------
	virtual MStatus	appendSyntax();
};

} // namespace Toolbox

} // namespace Stubble

#endif  // STUBBLE_GENERIC_TOOL_HPP
