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

	// TODO
    // HairShape* shapeNode = (HairShape*)surfaceShape();
	// getDrawData( shapeNode, data );

    request.setDrawData( data );
	requests.add( request );
}

void HairShapeUI::draw( const MDrawRequest & request, M3dView & view ) const
{       
	// From the given draw request, get the draw data and determine
	// which quadric to draw and with what values.
    MDrawData data = request.drawData();

	// TODO
    // HairShape * geom = (HairShape*)data.geometry();

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

	// TODO
	// geom->DrawHaircut(request.displayStatus() != M3dView::DisplayStatus::kDormant);

	glPopMatrix();
	glPopAttrib();

	view.endGL(); //maya open GL stuff
}

bool HairShapeUI::select( MSelectInfo &selectInfo, MSelectionList &selectionList, MPointArray &worldSpaceSelectPts ) const
{
	// Select function. Gets called when the bbox for the object is selected.
	// This function just selects the object without doing any intersection tests.
    MSelectionMask priorityMask( MSelectionMask::kSelectHairSystems );
    MSelectionList item;
    item.add( selectInfo.selectPath() );
    MPoint xformedPt;
    selectInfo.addSelection( item, xformedPt, selectionList, worldSpaceSelectPts, priorityMask, false );

    return true;
}

} // namespace HairShape

} // namespace Stubble