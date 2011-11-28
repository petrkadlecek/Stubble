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

void HapticListener::draw( M3dView& view, const MDagPath& DGpath, M3dView::DisplayStyle style, M3dView::DisplayStatus status )
{
	gluQuadricDrawStyle(q, GLU_LINE);
	gluQuadricNormals(q, GLU_SMOOTH);

	// think glPushMatrix()
	view.beginGL();
	
  MVector pos = HapticSettingsTool::getLastPosition();

	// this makes a copy of the current openGL settings so that anything
	// we change will not affect anything else maya draws afterwards.
	glPushAttrib( GL_CURRENT_BIT );
	
	// draw a red line
	glColor3f(1,0,0);
	glBegin(GL_LINES);
		glVertex3f(  pos.x, pos.y - 0.2f, pos.z );
		glVertex3f(  pos.x, pos.y + 0.2f, pos.z );
		glVertex3f(  pos.x - 0.2f, pos.y, pos.z );
		glVertex3f(  pos.x + 0.2f, pos.y, pos.z );
		glVertex3f(  pos.x, pos.y, pos.z + 0.2f );
		glVertex3f(  pos.x, pos.y, pos.z - 0.2f );

		glColor3f(1,1,1);
		// helper lines
		glVertex3f(0, 0, 0);
		glVertex3f(pos.x, 0, pos.z);

		glVertex3f(pos.x, 0, pos.z);
		glVertex3f(pos.x, pos.y, pos.z);
	glEnd();

	glBegin(GL_QUADS);
		glVertex3f(pos.x - 0.5f, 0.0f, pos.z - 0.5f);
		glVertex3f(pos.x + 0.5f, 0.0f, pos.z - 0.5f);
		glVertex3f(pos.x + 0.5f, 0.0f, pos.z + 0.5f);
		glVertex3f(pos.x - 0.5f, 0.0f, pos.z + 0.5f);
	glEnd();

	glColor3f(1,0,0);

	// draw yellow quad on xz plane (going to alpha blend it so we can see through it,
	// which i reackon makes it a bit less obtrusive)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


	MDagPath cameraPath;
	M3dView::active3dView().getCamera(cameraPath);
	MFnCamera camera(cameraPath);

	//glLoadIdentity();
	glTranslatef(pos.x, pos.y, pos.z);
	gluSphere(q, 1.0, 15, 15);		

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

  // todo
  //MGlobal::executeCommandOnIdle("createNode HapticListener;");
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