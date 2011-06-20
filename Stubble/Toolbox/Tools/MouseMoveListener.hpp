#ifndef MOUSE_MOVE_LISTENER_HPP
#define MOUSE_MOVE_LISTENER_HPP

#include "GenericTool.hpp"
#include <QtOpenGL/QGLWidget>

namespace Stubble
{

namespace Toolbox
{

class MouseMoveListener
	: public QWidget
{
public:
    
	MouseMoveListener( QWidget *aParent, GenericTool *aOwner );
 
protected:

	bool eventFilter( QObject *aObject, QEvent *aEvent ); 

private:

	GenericTool *mOwner;

	short mScreenCoords[ 2 ];
};

} // namespace Toolbox

} // namespace Stubble

#endif  // MOUSE_MOVE_LISTENER_HPP