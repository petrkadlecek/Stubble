#ifndef MOUSE_MOVE_LISTENER_HPP
#define MOUSE_MOVE_LISTENER_HPP

#include "GenericTool.hpp"
#include <QtOpenGL/QGLWidget>

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Mouse listener that helps to handle drawing of our tools.
///----------------------------------------------------------------------------------------------------
class MouseMoveListener
	: public QWidget
{
public:
    
	///----------------------------------------------------------------------------------------------------
	/// Class constructor.
	///
	/// \param aParent Widget that owns this listener
	/// \param Tool which is using this listener
	///----------------------------------------------------------------------------------------------------
	MouseMoveListener( QWidget *aParent, GenericTool *aOwner );
 
protected:

	///----------------------------------------------------------------------------------------------------
	/// Method for filtering mouse events that are interesting for us.
	///
	/// \param aObject Object that triggered the event
	/// \param aEvent Event that occurred
	///
	/// \return See Qt documentation for more info
	///----------------------------------------------------------------------------------------------------
	bool eventFilter( QObject *aObject, QEvent *aEvent ); 

private:

	GenericTool *mOwner; ///< Tool that is using this listener

	short mScreenCoords[ 2 ]; ///< Stores previous mouse coordinates [x, y]
};

} // namespace Toolbox

} // namespace Stubble

#endif  // MOUSE_MOVE_LISTENER_HPP
