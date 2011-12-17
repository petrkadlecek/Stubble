#include "HapticListener.hpp"
#include "HapticSettingsTool.hpp"
#include <GL/glut.h>
#include <iomanip>

#include <maya/MPxSelectionContext.h>
#include <maya/MPxContextCommand.h>
#include <maya/M3dView.h>
#include <maya/MFnCamera.h>
#include <maya/MDagPath.h>

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
HapticListener::HapticListener() 
{
	mHapticButton1Last = false;
	mHapticButton2Last = false;
}

HapticListener::~HapticListener() {}


MStatus HapticListener::compute( const MPlug& plug, MDataBlock& dataBlock )
{
	dataBlock.setClean( plug );
	return MS::kSuccess;
}

void HapticListener::VectorMatrixMul4f(float pVector[4], float pMat[16])
{
	float pVector2[4] = { pVector[0], pVector[1], pVector[2], pVector[3] };
	pVector[0] = pMat[0]*pVector2[0] + pMat[4]*pVector2[1] + pMat[ 8]*pVector2[2] + pMat[12]*pVector2[3] ;
	pVector[1] = pMat[1]*pVector2[0] + pMat[5]*pVector2[1] + pMat[ 9]*pVector2[2] + pMat[13]*pVector2[3] ;
	pVector[2] = pMat[2]*pVector2[0] + pMat[6]*pVector2[1] + pMat[10]*pVector2[2] + pMat[14]*pVector2[3] ;
	pVector[3] = pMat[3]*pVector2[0] + pMat[7]*pVector2[1] + pMat[11]*pVector2[2] + pMat[15]*pVector2[3] ;
}

GLUquadric *ql = gluNewQuadric(); // put this into the class

void HapticListener::draw( M3dView& view, const MDagPath& DGpath, M3dView::DisplayStyle style, M3dView::DisplayStatus status )
{
	// get current haptic switch state
	bool hapticButton1State = HapticSettingsTool::getHapticButton1State();
	bool hapticButton2State = HapticSettingsTool::getHapticButton2State();

	// get current camera
	MDagPath cameraPath;
	view.getCamera( cameraPath );
	MFnCamera camera( cameraPath );

	// get haptic proxy eye space coordinates
	// TODO: set scaling
	MVector hapticProxyEyeSpacePos = 150.0f * HapticSettingsTool::getLastPosition();
	hapticProxyEyeSpacePos.z = -hapticProxyEyeSpacePos.z + 25.0f;;

	// compute haptic proxy object space coordinates - snap to camera
	MVector hapticProxyPos = camera.eyePoint( MSpace::kWorld );
	hapticProxyPos += camera.rightDirection( MSpace::kWorld ) * hapticProxyEyeSpacePos.x;
	hapticProxyPos += camera.upDirection( MSpace::kWorld ) * hapticProxyEyeSpacePos.y;
	hapticProxyPos += camera.viewDirection( MSpace::kWorld ) * hapticProxyEyeSpacePos.z;

	gluQuadricDrawStyle( ql, GLU_FILL );
	gluQuadricNormals( ql, GLU_SMOOTH );

	// think glPushMatrix()
	view.beginGL();
	
	// this makes a copy of the current openGL settings so that anything
	// we change will not affect anything else maya draws afterwards.
	glPushAttrib( GL_CURRENT_BIT );
	
	glColor3f( 0.5f, 0.5f, 0.5f );

	// draw proxy helper in world space
	glBegin(GL_QUADS);
	glVertex3f( hapticProxyPos.x - 0.5f, 0.0f, hapticProxyPos.z - 0.5f );
	glVertex3f( hapticProxyPos.x + 0.5f, 0.0f, hapticProxyPos.z - 0.5f );
	glVertex3f( hapticProxyPos.x + 0.5f, 0.0f, hapticProxyPos.z + 0.5f );
	glVertex3f( hapticProxyPos.x - 0.5f, 0.0f, hapticProxyPos.z + 0.5f );
	glEnd();

	glLineStipple(3, 0xAAAA);
	glPushAttrib(GL_ENABLE_BIT); 
	glEnable(GL_LINE_STIPPLE);
	glBegin( GL_LINES );
	glVertex3f( hapticProxyPos.x, 0.0f, hapticProxyPos.z );
	glVertex3f( hapticProxyPos.x, hapticProxyPos.y, hapticProxyPos.z );
	glEnd();
	glPopAttrib();

	// set a color of haptic proxy
	if ( hapticButton1State == true )
	{
		glColor3f( 1.0f, 1.0f, 0.4f );
	}
	else
	{
		glColor3f( 1.0f, 0.2f, 0.2f );
	}


	// draw haptic sphere proxy in view space
	glPushMatrix();
	{
		glTranslatef( hapticProxyPos.x, hapticProxyPos.y, hapticProxyPos.z );
		gluSphere( ql, 0.2, 8, 8 );

	}
	glPopMatrix();
	
	// restore the old openGL settings
	glPopAttrib();

	// think glPopMatrix()
	view.endGL();

	// handle associated tool (if there's any)
	if (HapticListener::sTool != NULL)
	{
		// draw haptic tool shape and set shape position
		HapticListener::sTool->drawHapticToolShape( hapticProxyPos );

		// call tool haptic events
		if ( hapticButton1State == true && mHapticButton1Last == false )
		{
			HapticListener::sTool->doHapticPress();
		}
		else if ( hapticButton1State == true && mHapticButton1Last == true )
		{
			HapticListener::sTool->doHapticDrag( hapticProxyEyeSpacePos - hapticProxyEyeSpacePosLast );
		}
		else if ( hapticButton1State == false && mHapticButton1Last == true )
		{
			HapticListener::sTool->doHapticRelease();
		}
	}

	// set last states
	mHapticButton1Last = hapticButton1State;
	mHapticButton2Last = hapticButton2State;
	mhapticProxyPosLast = hapticProxyPos;
	hapticProxyEyeSpacePosLast = hapticProxyEyeSpacePos;
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
}

// TODO unsetTool

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
