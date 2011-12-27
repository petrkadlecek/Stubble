#include "SphereToolShape.hpp"

#include <maya/MQuaternion.h>

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
	// mouse support not implemented
}

GLUquadric *q = gluNewQuadric(); // put this into the class

void SphereToolShape::draw( M3dView *aView, MVector &aProxyPosition, MVector &aHapticProxyRotation, double &aHapticProxyRotationAngle )
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

	// restore the old openGL settings
	glPopAttrib();

	// think glPopMatrix()
	aView->endGL();

	mPrevProxyPosition = aProxyPosition;
}

MString SphereToolShape::getName()
{
	return "Sphere Tool Shape";
}

} // namespace Toolbox

} // namespace Stubble
