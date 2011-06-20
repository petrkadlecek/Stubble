#include "CircleToolShape.hpp"

#include <GL/gl.h>
#include <GL/glu.h>

namespace Stubble
{

namespace Toolbox
{

CircleToolShape::CircleToolShape()
	: mRadius( 30 ), mIsDrawn( false )
{
	mPrevScreenCoords[ 0 ] = 0;
	mPrevScreenCoords[ 1 ] = 0;
}

void CircleToolShape::update( GenericTool *aTool )
{
	mRadius = aTool->getCircleRadius();
}

void CircleToolShape::draw( M3dView *aView, short aScreenCoords[ 2 ], QEvent::Type aEventType )
{
	switch ( aEventType )
	{
	case QEvent::Enter : 
						break;
	case QEvent::MouseMove : aView->refresh( true );
							if ( mIsDrawn ) 
							{
								drawToolShape( aView, mPrevScreenCoords );
							}
							mIsDrawn = true;
							drawToolShape( aView, aScreenCoords );
							mPrevScreenCoords[ 0 ] = aScreenCoords[ 0 ];
							mPrevScreenCoords[ 1 ] = aScreenCoords[ 1 ];
						break;
	case QEvent::Leave : aView->refresh( true );
						 drawToolShape( aView, aScreenCoords );
						 mIsDrawn = false;
						 aView->refresh( true );
						break;
	default: break;
	}
}

void CircleToolShape::drawToolShape( M3dView *aView, short aScreenCoords[ 2 ] )
{
	double w = aView->portWidth();
	double h = aView->portHeight();

	aView->beginGL();

	glPushAttrib( GL_ALL_ATTRIB_BITS );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( 0.0, ( GLdouble ) w, 0.0, ( GLdouble ) h );
	glDisable( GL_DEPTH_TEST );
	glMatrixMode( GL_MODELVIEW );
	glShadeModel( GL_FLAT );

	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ZERO );

	aView->beginXorDrawing( true, true, 1.0f, M3dView::kStippleNone, MColor( 1.0f, 1.0f, 1.0f ) );
	
		glTranslatef( 0.375f + aScreenCoords[ 0 ], 0.375f + aScreenCoords[ 1 ], 0.0f );

		glBegin( GL_LINE_LOOP );
			for( int i = 0; i < 36 ; ++i )
			{
				float x = mRadius * cos( i * 10 * 3.14159f / 180.0f );
				float y = mRadius * sin( i * 10 * 3.14159f / 180.0f );
				glVertex2f( x, y );
			}
		glEnd();

	aView->endXorDrawing();

	glPopAttrib();

	aView->endGL();
}


} // namespace Toolbox

} // namespace Stubble
