#ifndef STUBBLE_GENERIC_TOOL_HPP
#define STUBBLE_GENERIC_TOOL_HPP

#include <maya/M3dView.h>
#include <maya/MPxContext.h>
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

protected:

	ToolShape *mShape; ///< Current brush shape (circle, texture shape...).

	M3dView mView; ///< The view in which we are currently operating.
};

} // namespace Toolbox

} // namespace Stubble

#endif  // STUBBLE_GENERIC_TOOL_HPP
