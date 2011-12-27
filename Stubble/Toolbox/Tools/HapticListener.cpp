#include "HapticListener.hpp"
#include "HapticSettingsTool.hpp"
#include "../../HairShape/UserInterface/HairShape.hpp"

#include <GL/glut.h>
#include <iomanip>

#include <maya/MPxSelectionContext.h>
#include <maya/MPxContextCommand.h>
#include <maya/M3dView.h>
#include <maya/MFnCamera.h>
#include <maya/MDagPath.h>

#include <maya/MEulerRotation.h>
#include <maya/MQuaternion.h>
#include <maya/MMatrix.h>

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
	mHapticProxyRotAngleLast = 0.0;
	mHapticProxyRotLast.x = 1.0;
	mHapticProxyRotLast.y = 0.0;
	mHapticProxyRotLast.z = 0.0;
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
	bool simulate5DOF = true;
	bool enabled5DOFsim = hapticButton1State && simulate5DOF;

	// get current camera
	MDagPath cameraPath;
	view.getCamera( cameraPath );
	MFnCamera camera( cameraPath );

	MVector activeObjectCenter( 0, 0, 0 );

	HairShape::HairShape *activeHairShape = HairShape::HairShape::getActiveObject();
	if ( activeHairShape != NULL )
	{
		activeObjectCenter = ( activeHairShape->getActiveObject()->boundingBox().min() + activeHairShape->getActiveObject()->boundingBox().max() )/2;
	}

	// compute haptic proxy object space coordinates - snap to camera
	MVector cameraEyePoint = camera.eyePoint( MSpace::kWorld );

	double workspaceLength = ( activeObjectCenter - cameraEyePoint ).length();

	// get haptic proxy eye space coordinates
	MVector hapticDeviceLastPos = HapticSettingsTool::getLastPosition();
	MVector hapticProxyEyeSpacePos = hapticDeviceLastPos;

	hapticProxyEyeSpacePos.x =  workspaceLength * hapticProxyEyeSpacePos.x;
	hapticProxyEyeSpacePos.y =  workspaceLength * hapticProxyEyeSpacePos.y;
	hapticProxyEyeSpacePos.z = -workspaceLength * ( -1.0 + hapticProxyEyeSpacePos.z );

	MVector hapticProxyPos( 0, 0, 0 );
	if ( !enabled5DOFsim )
	{
		hapticProxyPos = cameraEyePoint;
		hapticProxyPos += camera.rightDirection( MSpace::kWorld ) * hapticProxyEyeSpacePos.x;
		hapticProxyPos += camera.upDirection( MSpace::kWorld ) * hapticProxyEyeSpacePos.y;
		hapticProxyPos += camera.viewDirection( MSpace::kWorld ) * hapticProxyEyeSpacePos.z;
	}
	else
	{
		hapticProxyPos = mHapticProxyPosLast;
	}

	// compute haptic proxy object space rotation - snap to camera
	MVector hapticProxyEyeSpaceRotRobotic = HapticSettingsTool::getLastRotation();

	// map robotic frame to opengl right-handed frame
	MVector hapticProxyEyeSpaceRot;
	hapticProxyEyeSpaceRot.x = hapticProxyEyeSpaceRotRobotic.y;
	hapticProxyEyeSpaceRot.y = hapticProxyEyeSpaceRotRobotic.z;
	hapticProxyEyeSpaceRot.z = hapticProxyEyeSpaceRotRobotic.x;

	double hapticProxyRotAngle = HapticSettingsTool::getLastRotationAngle();
	
	MVector hapticProxyRot = hapticProxyEyeSpaceRot;

	// Simulate 5DOF - rotate using translation
	if ( enabled5DOFsim )
	{
		hapticProxyRot.x = -hapticDeviceLastPos.y;
		hapticProxyRot.y = hapticDeviceLastPos.x;
		hapticProxyRot.z = 0.0;
		hapticProxyRotAngle = hapticProxyRot.length();
	}

	if ( simulate5DOF && !enabled5DOFsim )
	{
		hapticProxyRot = mHapticProxyRotLast;
		hapticProxyRotAngle = mHapticProxyRotAngleLast;
	}

	// set last proxy rotation vector and angle
	mHapticProxyRotLast = hapticProxyRot;
	mHapticProxyRotAngleLast = hapticProxyRotAngle;
	
	// get rotation quaternions to apply camera rotation
	MMatrix mmatrix = cameraPath.inclusiveMatrix();
	MTransformationMatrix tmatrix( mmatrix );
	MQuaternion cameraRotation = tmatrix.rotation();
	MQuaternion qHapticProxy(hapticProxyRotAngle, hapticProxyRot);
	MQuaternion concatQRot = qHapticProxy * cameraRotation;

	// apply camera rotation
	MVector camAxis;
	double camTheta;
	concatQRot.getAxisAngle( camAxis, camTheta );

	// save result of both rotations into hapticProxyRot
	hapticProxyRot = camAxis;
	hapticProxyRotAngle = camTheta * 180.0 / CHAI_PI;


	// prepare Quadrics
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
	if ( HapticListener::sTool != NULL )
	{
		// draw haptic tool shape and set shape position
		HapticListener::sTool->drawHapticToolShape( hapticProxyPos, hapticProxyRot, hapticProxyRotAngle );

		// call tool haptic events
		if ( hapticButton1State == true && mHapticButton1Last == false )
		{
			HapticListener::sTool->doHapticPress();
		}
		else if ( hapticButton1State == true && mHapticButton1Last == true )
		{
			HapticListener::sTool->doHapticDrag( hapticProxyEyeSpacePos - mHapticProxyEyeSpacePosLast );
		}
		else if ( hapticButton1State == false && mHapticButton1Last == true )
		{
			HapticListener::sTool->doHapticRelease();
		}
	}
	else // camera movement
	{
		if ( hapticButton1State == true ) 
		{
			
			MVector dragVector = hapticProxyEyeSpacePos - mHapticProxyEyeSpacePosLast;
			MPoint newEyePoint = cameraEyePoint;

			/*
			MPoint r1(activeObjectCenter);
			MPoint r2(cameraEyePoint);
			r1.y = 0; 
			r2.y = 0;
			float radius = r1.distanceTo(r2);
			
			newEyePoint.x = activeObjectCenter.x + cos(hapticProxyEyeSpacePos.x) * radius;
			newEyePoint.z = activeObjectCenter.z + sin(hapticProxyEyeSpacePos.x) * radius;
			*/

			MVector viewDir( newEyePoint - activeObjectCenter );
			MVector up( 0, 1, 0 );
			

			camera.set( newEyePoint, viewDir, camera.upDirection(MSpace::kWorld), camera.horizontalFieldOfView(), camera.aspectRatio() );
		}
	}

	// set last states
	mHapticButton1Last = hapticButton1State;
	mHapticButton2Last = hapticButton2State;
	mHapticProxyPosLast = hapticProxyPos;
	mHapticProxyEyeSpacePosLast = hapticProxyEyeSpacePos;
}

bool HapticListener::isBounded() const
{
	return true;
}

MBoundingBox HapticListener::boundingBox() const
{
	MBoundingBox bbox;
	// TODO SET RADIUS

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
