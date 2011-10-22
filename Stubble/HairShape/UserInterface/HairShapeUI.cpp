#include "HairShapeUI.hpp"

#include "maya\MDrawData.h"
#include "maya\MDagPath.h"
#include "maya\MMatrix.h"
#include "maya\MSelectionList.h"
#include "maya\MSelectionMask.h"

namespace Stubble
{

namespace HairShape
{

HairShapeUI::HairShapeUI() {}

HairShapeUI::~HairShapeUI() {}

void* HairShapeUI::creator()
{
	return new HairShapeUI();
}

void HairShapeUI::getDrawRequests( const MDrawInfo & info, bool objectAndActiveOnly, MDrawRequestQueue & requests )
{
	// The draw data is used to pass geometry through the 
    // draw queue. The data should hold all the information
    // needed to draw the shape.
    MDrawData data;
    MDrawRequest request = info.getPrototype( *this );

    HairShape* shapeNode = (HairShape*)surfaceShape();
	getDrawData( shapeNode, data );

    request.setDrawData( data );
	requests.add( request );
}

void HairShapeUI::draw( const MDrawRequest & request, M3dView & view ) const
{       
	// From the given draw request, get the draw data and determine
	// which quadric to draw and with what values.
    MDrawData data = request.drawData();

    HairShape * geom = (HairShape*)data.geometry();

	MDagPath thisCameraView; // the dagPath for this modelPanel's Camera
	MMatrix cameraInverseMatrix; // inverse matrix for the modelPanel's Camera
	double dMatrix[4][4]; // the 4×4 array for the OpenGL matrix

	// Get the Maya Camera associated with this view
	view.getCamera( thisCameraView );

	// Load inverse of the Camera's Matrix
	cameraInverseMatrix = (thisCameraView.inclusiveMatrix()).inverse();

	// Get 4×4 array to represent matrix (required for glLoadMatrix)
	cameraInverseMatrix.get( dMatrix );

	//a maya specific GL call
	view.beginGL();
	
	// store the current color and openGL settings
	glPushAttrib( GL_CURRENT_BIT );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	// Load the inverse of the Camera's matrix into the Model View.
	// This places the render origin at Maya's world center.
	glLoadMatrixd( (GLdouble *)dMatrix );

	geom->draw();

	glPopMatrix();
	glPopAttrib();

	view.endGL(); //maya open GL stuff
}

bool HairShapeUI::select( MSelectInfo &selectInfo, MSelectionList &selectionList, MPointArray &worldSpaceSelectPts ) const
{
	// Tell Maya that the object has been selected
	MSelectionMask priorityMask( MSelectionMask::kSelectEdges );
    MSelectionList item;
    item.add( selectInfo.selectPath() );

	//MDagPath	dagPath;	// will hold a path to the selected object
	//MObject		component;	// will hold a list of selected components
    
	HairShape *hairShape = (HairShape *)surfaceShape();
	bool isComponent = true;

	MPoint xformedPt;
	if ( selectInfo.singleSelection() ) {
			MPoint center = hairShape->boundingBox().center();
			xformedPt = center;
			xformedPt *= selectInfo.selectPath().inclusiveMatrix();
			isComponent = false;
		}
    selectInfo.addSelection( item, xformedPt, selectionList, worldSpaceSelectPts, priorityMask, isComponent );

	// Select hair guides	
	hairShape->mHairGuides->applySelection(selectInfo, selectionList, worldSpaceSelectPts);

    return true;
}

} // namespace HairShape

} // namespace Stubble