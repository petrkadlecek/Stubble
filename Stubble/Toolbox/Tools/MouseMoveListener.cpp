#include "MouseMoveListener.hpp"

namespace Stubble
{

namespace Toolbox
{

MouseMoveListener::MouseMoveListener( QWidget *aParent, GenericTool *aOwner )
        : QWidget( aParent ), mOwner( aOwner)
{
	this->setMouseTracking( true );
	aParent->installEventFilter( this );
}

bool MouseMoveListener::eventFilter( QObject *aObject, QEvent *aEvent )
{
	// did we enter or exit the viewport, or are we moving inside it?
	QEvent::Type eventType = aEvent->type();

	// if the event is not of interest to us, pass it to the parent to be handled
	if (!( eventType == QEvent::Enter || eventType == QEvent::MouseMove || eventType == QEvent::Leave ))
		return QWidget::eventFilter( aObject, aEvent);

	if ( eventType == QEvent::Enter || eventType == QEvent::MouseMove )
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>( aEvent );
		// get the current position of the cursor.
		mScreenCoords[ 0 ] = mouseEvent->x();
		mScreenCoords[ 1 ] = mOwner->getActiveView()->portHeight() - mouseEvent->y();
	}
	
	// pass the necessary drawing information to the tool object
	mOwner->drawToolShape( mScreenCoords, eventType );

	// call the super-class implementation, doesn't work w/o this line
	return QWidget::eventFilter(aObject, aEvent);
}

} // namespace Toolbox

} // namespace Stubble
