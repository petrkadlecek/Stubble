#include "SphereToolShape.hpp"

#include <GL/gl.h>
#include <GL/glu.h>

namespace Stubble
{

namespace Toolbox
{

const double SphereToolShape::DEFAULT_RADIUS = 2.0;

SphereToolShape::SphereToolShape()
	: mIsDrawn( false )
{
	mScale = 1.0; // initialize the value before it is first passed from the UI

	mRadius = mScale * DEFAULT_RADIUS;
}

void SphereToolShape::update( GenericTool *aTool )
{
	mScale = aTool->getToolScale();
	mRadius = mScale * DEFAULT_RADIUS;
}

void SphereToolShape::getPosition(MVector &aProxyPositon) const
{
	aProxyPositon = mPrevProxyPosition;
}

void SphereToolShape::draw( M3dView *aView, short aScreenCoords[ 2 ], QEvent::Type aEventType )
{
	/* TODO */
}

GLUquadric *q = gluNewQuadric(); // put this into the class

void SphereToolShape::draw( M3dView *aView, MVector &aProxyPosition )
{
	gluQuadricDrawStyle( q, GLU_SILHOUETTE );
	gluQuadricNormals( q, GLU_SMOOTH );

	// think glPushMatrix()
	aView->beginGL();

	// this makes a copy of the current openGL settings so that anything
	// we change will not affect anything else maya draws afterwards.
	glPushAttrib( GL_CURRENT_BIT );
	
	// set a color of haptic proxy
	if (HapticSettingsTool::getHapticButton1State() == true)
	{
		glColor3f( 1.0f, 1.0f, 0.4f );
	}
	else
	{
		glColor3f( 1.0f, 0.4f, 0.4f );
	}
	

	// draw haptic sphere proxy in view space
	glPushMatrix();
	{
		glTranslatef( aProxyPosition.x, aProxyPosition.y, aProxyPosition.z );
		gluSphere( q, mRadius, 8, 8 );
	}
	glPopMatrix();

	
	// draw proxy helper in world space
	glPushMatrix();
	{
		glBegin(GL_QUADS);
		glVertex3f(aProxyPosition.x - 0.5f, 0.0f, aProxyPosition.z - 0.5f);
		glVertex3f(aProxyPosition.x + 0.5f, 0.0f, aProxyPosition.z - 0.5f);
		glVertex3f(aProxyPosition.x + 0.5f, 0.0f, aProxyPosition.z + 0.5f);
		glVertex3f(aProxyPosition.x - 0.5f, 0.0f, aProxyPosition.z + 0.5f);
		glEnd();
	}
	glPopMatrix();

	// restore the old openGL settings
	glPopAttrib();

	// think glPopMatrix()
	aView->endGL();

	mPrevProxyPosition = aProxyPosition;
}

void SphereToolShape::drawToolShape( M3dView *aView, MVector &aProxyPosition )
{
	double w = aView->portWidth();
	double h = aView->portHeight();

	/*
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
	*/
}


} // namespace Toolbox

} // namespace Stubble
