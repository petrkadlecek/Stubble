#include "HairShapeUI.hpp"

#include "maya\MDrawData.h"
#include "maya\MDagPath.h"
#include "maya\MFnSingleIndexedComponent.h"
#include "maya\MGlobal.h"
#include "maya\MMatrix.h"
#include "maya\MSelectionList.h"
#include "maya\MSelectionMask.h"

// Object and component color defines
//
const int LEAD_COLOR = 18;	// green
const int ACTIVE_COLOR = 15;	// white
const int ACTIVE_AFFECTED_COLOR = 8;	// purple
const int DORMANT_COLOR = 4;	// blue
const int HILITE_COLOR = 17;	// pale blue
const int DORMANT_VERTEX_COLOR = 8;	// purple
const int ACTIVE_VERTEX_COLOR = 16;	// yellow

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

bool HairShapeUI::select( MSelectInfo &aSelectInfo, MSelectionList &aSelectionList,  MPointArray &aWorldSpaceSelectPts ) const
{
	bool selected = false;
	bool componentSelected = false;
	//bool hilited = false;

	componentSelected = selectVertices( aSelectInfo, aSelectionList, aWorldSpaceSelectPts );
	selected = selected || componentSelected;
	
    // let the shape know that its selected components list might have changed
	HairShape* hairShape = ( HairShape* ) surfaceShape();
	hairShape->setSelectionModified( componentSelected );                                                      

	return selected;
}

bool HairShapeUI::selectVertices( MSelectInfo &aSelectInfo, MSelectionList &aSelectionList,  MPointArray &aWorldSpaceSelectPts ) const
{
	bool selected = false;
	//M3dView view = aSelectInfo.view();

	//MPoint 		xformedPoint;
	//MPoint 		selectionPoint;
	//double		z,previousZ = 0.0;
 	//int			closestPointVertexIndex = -1;

	//const MDagPath & path = aSelectInfo.multiPath();

	// Create a component that will store the selected vertices
	//
	//MFnSingleIndexedComponent fnComponent;
	//MObject surfaceComponent = fnComponent.create( MFn::kMeshVertComponent );
	//int vertexIndex;

	// if the user did a single mouse click and we find > 1 selection
	// we will use the alignmentMatrix to find out which is the closest
	//
	//MMatrix	alignmentMatrix;
	//MPoint singlePoint; 
	//bool singleSelection = aSelectInfo.singleSelection();
	//if( singleSelection ) {
	//	alignmentMatrix = aSelectInfo.getAlignmentMatrix();
	//}

	// Get the geometry information
	//
	HairShape* hairShape = ( HairShape* ) surfaceShape();

	// Select hair guides	
	return ( hairShape->mHairGuides->applySelection( hairShape->mInterpolationGroupsSelectable, aSelectInfo, aSelectionList, aWorldSpaceSelectPts ) );
}

HairShapeUI::SelectionMode HairShapeUI::getSelectionMode()
{
	int selectionMode;
	HairShapeUI::SelectionMode selMode;
	

	MStatus stat = MGlobal::executeCommand( MString("optionVar -q \"stubbleSelectionMode\";"), selectionMode );

	if ( stat != MStatus::kSuccess )
	{
		std::cout << "Error retrieving environment variable stubbleSelectionMode!" << endl;
	}

	//int selectionMode = ( int ) selectionModeEnv;

	switch ( selectionMode )
	{
	case 1:
		selMode = kSelectGuides;
		break;
	case 2:
		selMode = kSelectAllVertices;
		break;
	case 3:
		selMode = kSelectTips;
		break;
	case 4:
		selMode = kSelectRoots;
		break;
	default:
		std::cout << "Variable stubbleSelectionMode has an invalid value! Defaulting to 1." << endl;
		stat = MGlobal::executeCommand( "optionVar -iv \"stubbleSelectionMode\" 1;" );
		selMode = kSelectGuides;
	}

	return selMode;
}

HairShapeUI::DrawingState HairShapeUI::getDrawingState()
{
	HairShapeUI::DrawingState drawingState;
	HairShapeUI::SelectionMode selectionMode = HairShapeUI::getSelectionMode();
	
	switch ( selectionMode )
	{
	case kSelectGuides:
		drawingState = kDrawGuides;
		break;
	case kSelectAllVertices:
	case kSelectTips:
	case kSelectRoots:
		drawingState = kDrawVertices;
		break;
	default:
		std::cout << "Error in method getDrawingState()!" << endl;
		drawingState = kDrawGuides;
	}

	return drawingState;
}

} // namespace HairShape

} // namespace Stubble