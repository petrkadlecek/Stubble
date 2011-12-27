#include "CylinderToolShape.hpp"

#include <GL/gl.h>
#include <GL/glu.h>

namespace Stubble
{

namespace Toolbox
{

const double CylinderToolShape::DEFAULT_RADIUS = 2.0;

CylinderToolShape::CylinderToolShape()
	: mIsDrawn( false )
{
	mScale = 1.0; // initialize the value before it is first passed from the UI

	mRadius = mScale * DEFAULT_RADIUS;
}

void CylinderToolShape::update( GenericTool *aTool )
{
	mScale = aTool->getToolScale();
	mRadius = mScale * DEFAULT_RADIUS;
}

void CylinderToolShape::draw( M3dView *aView, short aScreenCoords[ 2 ], QEvent::Type aEventType )
{
	// mouse support not implemented
}

GLUquadric *qc = gluNewQuadric(); // put this into the class

void CylinderToolShape::draw( M3dView *aView, MVector &aProxyPosition, MVector &aHapticProxyRotation, double &aHapticProxyRotationAngle )
{
	gluQuadricDrawStyle( qc, GLU_SILHOUETTE );
	gluQuadricNormals( qc, GLU_SMOOTH );

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

	// draw haptic Cylinder proxy in view space
	glPushMatrix();
	{
		glTranslatef( aProxyPosition.x, aProxyPosition.y, aProxyPosition.z );
		glRotated( aHapticProxyRotationAngle, aHapticProxyRotation.x, aHapticProxyRotation.y, aHapticProxyRotation.z );
		gluCylinder( qc, mRadius * 0.5, mRadius * 0.6, mRadius * 5.0, 8, 8 );
	}
	glPopMatrix();

	// restore the old openGL settings
	glPopAttrib();

	// think glPopMatrix()
	aView->endGL();

	// set proxy properties to this tool shape
	mPrevProxyPosition = aProxyPosition;
	mPrevProxyRotationAxis = aHapticProxyRotation;
	mPrevProxyRotationAngle = aHapticProxyRotationAngle;
}

MString CylinderToolShape::getName()
{
	return "Cylinder Tool Shape";
}

} // namespace Toolbox

} // namespace Stubble
