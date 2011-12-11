//#include <maya/MIOStream.h>
//#include <math.h>
//#include <stdlib.h>

#include "HapticListener.hpp"
#include "HapticSettingsTool.hpp"
#include <GL/glut.h>

#include <maya/MPxSelectionContext.h>
#include <maya/MPxContextCommand.h>
#include <maya/M3dView.h>
#include <maya/MFnCamera.h>
#include <maya/MDagPath.h>

#include <iomanip>

namespace Stubble
{

namespace Toolbox
{

// ----------------------------------------------------------------------------
// Static data members
// ----------------------------------------------------------------------------
const MTypeId HapticListener::typeId( 0x70000 );
const MString HapticListener::typeName( "HapticListener" );
GenericTool* HapticListener::sTool;

// ----------------------------------------------------------------------------
// HapticListener
// ----------------------------------------------------------------------------
HapticListener::HapticListener() {}

HapticListener::~HapticListener() {}


MStatus HapticListener::compute( const MPlug& plug, MDataBlock& dataBlock )
{
	dataBlock.setClean( plug );
	return MS::kSuccess;
}

GLUquadric *q = gluNewQuadric(); // put this into the class

void HapticListener::VectorMatrixMul4f(float pVector[4], float pMat[16])
{
	float pVector2[4] = {pVector[0], pVector[1], pVector[2], pVector[3] };
	pVector[0] = pMat[0]*pVector2[0] + pMat[4]*pVector2[1] + pMat[ 8]*pVector2[2] + pMat[12]*pVector2[3] ;
	pVector[1] = pMat[1]*pVector2[0] + pMat[5]*pVector2[1] + pMat[ 9]*pVector2[2] + pMat[13]*pVector2[3] ;
	pVector[2] = pMat[2]*pVector2[0] + pMat[6]*pVector2[1] + pMat[10]*pVector2[2] + pMat[14]*pVector2[3] ;
	pVector[3] = pMat[3]*pVector2[0] + pMat[7]*pVector2[1] + pMat[11]*pVector2[2] + pMat[15]*pVector2[3] ;
}

void HapticListener::draw( M3dView& view, const MDagPath& DGpath, M3dView::DisplayStyle style, M3dView::DisplayStatus status )
{
	gluQuadricDrawStyle( q, GLU_LINE );
	gluQuadricNormals( q, GLU_SMOOTH );

	// think glPushMatrix()
	view.beginGL();
	
	// get current camera
	MDagPath cameraPath;
	view.getCamera(cameraPath);
	MFnCamera camera(cameraPath);

	// get haptic proxy eye space coordinates
	// TODO: set scaling
	MVector hapticProxyEyeSpacePos = 100.0f * HapticSettingsTool::getLastPosition();
	hapticProxyEyeSpacePos.z = -hapticProxyEyeSpacePos.z + 25.0f;;

	// compute haptic proxy object space coordinates - snap to camera
	MVector hapticProxyPos = camera.eyePoint( MSpace::kWorld );
	hapticProxyPos += camera.rightDirection( MSpace::kWorld ) * hapticProxyEyeSpacePos.x;
	hapticProxyPos += camera.upDirection( MSpace::kWorld ) * hapticProxyEyeSpacePos.y;
	hapticProxyPos += camera.viewDirection( MSpace::kWorld ) * hapticProxyEyeSpacePos.z;

	// compute haptic proxy position depending on camera view
	/*
	float hapticProxyPosVector[4] = { hapticProxyPos.x, hapticProxyPos.y, hapticProxyPos.z, 1.0f };
	float modelViewMatrix[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, modelViewMatrix );
	VectorMatrixMul4f( hapticProxyPosVector, modelViewMatrix );
	MVector hapticProxyHelperPos( hapticProxyPosVector[0], hapticProxyPosVector[1], hapticProxyPosVector[2] );
	*/

	// this makes a copy of the current openGL settings so that anything
	// we change will not affect anything else maya draws afterwards.
	glPushAttrib( GL_CURRENT_BIT );
	
	// set a color of haptic proxy
	glColor3f( 1.0f, 0.1f, 0.1f );

	// draw haptic sphere proxy in view space
	glPushMatrix();
	{
		glTranslatef( hapticProxyPos.x, hapticProxyPos.y, hapticProxyPos.z );
		gluSphere( q, 1.0, 8, 8 );
	}
	glPopMatrix();

	
	// draw proxy helper in world space
	glPushMatrix();
	{
		glBegin(GL_QUADS);
		glVertex3f(hapticProxyPos.x - 0.5f, 0.0f, hapticProxyPos.z - 0.5f);
		glVertex3f(hapticProxyPos.x + 0.5f, 0.0f, hapticProxyPos.z - 0.5f);
		glVertex3f(hapticProxyPos.x + 0.5f, 0.0f, hapticProxyPos.z + 0.5f);
		glVertex3f(hapticProxyPos.x - 0.5f, 0.0f, hapticProxyPos.z + 0.5f);
		glEnd();
	}
	glPopMatrix();

	// restore the old openGL settings
	glPopAttrib();

	// think glPopMatrix()
	view.endGL();
}

bool HapticListener::isBounded() const
{
	return true;
}

MBoundingBox HapticListener::boundingBox() const
{
	MBoundingBox bbox;
	// TODO

	bbox.expand( MPoint( -0.5f, 0.0f, -0.5f ) );
	bbox.expand( MPoint(  0.5f, 0.0f, -0.5f ) );
	bbox.expand( MPoint(  0.5f, 0.0f,  0.5f ) );
	bbox.expand( MPoint( -0.5f, 0.0f,  0.5f ) );
	bbox.expand( MPoint(  0.0f,-0.5f,  0.0f ) );
	bbox.expand( MPoint(  0.0f, 0.5f,  0.0f ) );

	return bbox;
}

void HapticListener::setTool( GenericTool *aOwner )
{
	HapticListener::sTool = aOwner;

	if ( HapticSettingsTool::sDeviceAvailable )
	{
		MGlobal::executeCommandOnIdle("createNode HapticListener;");
	}
	
}

void* HapticListener::creator()
{
	return new HapticListener();
}

MObject HapticListener::inputAttr;


MStatus HapticListener::initialize()
{
	return MS::kSuccess;
}


} // namespace Toolbox

} // namespace Stubble
