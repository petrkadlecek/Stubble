#ifndef STUBBLE_HAIR_SHAPE_UI_HPP
#define STUBBLE_HAIR_SHAPE_UI_HPP

#include <maya\MPxSurfaceShapeUI.h>

#include "HairShape\UserInterface\HairShape.hpp"

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// UI class defines the UI part of a hair shape node.
///----------------------------------------------------------------------------------------------------
class HairShapeUI : public MPxSurfaceShapeUI
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Default constructor.
	///----------------------------------------------------------------------------------------------------
	HairShapeUI();

	///----------------------------------------------------------------------------------------------------
	/// This function places drawing requests on maya's drawing queue and is called whenever the view is refreshed.
	///
	/// \param info	the drawing state information
	/// \param objectAndActiveOnly	This parameter is used to determine if draw requests for components need to be supplied.
	/// \param queue	the drawing queue to place the draw request on
	///----------------------------------------------------------------------------------------------------
    virtual void getDrawRequests( const MDrawInfo & info, bool objectAndActiveOnly, MDrawRequestQueue & requests );

	///----------------------------------------------------------------------------------------------------
	/// Maya will call this routine with requests that have been previously added to the drawing queue.
	///
	/// \param request	the drawing request
	/// \param view	the interactive 3d view in which to draw
	///----------------------------------------------------------------------------------------------------
    virtual void draw( const MDrawRequest & request, M3dView & view ) const;

	///----------------------------------------------------------------------------------------------------
	/// Selection method called when selection has been detected.
	///
	/// \param	aSelectInfo				structure containing selection region and various other information
	///	\param	aSelectionList			out parameter for returning selected objects list
	///	\param	aWorldSpaceSelectedPts	out parameter for returning selected points list
	/// \return	True if the object was selected
	///----------------------------------------------------------------------------------------------------
    virtual bool select( MSelectInfo &aSelectInfo, MSelectionList &aSelectionList,  MPointArray &aWorldSpaceSelectPts ) const;

	///----------------------------------------------------------------------------------------------------
	/// Helper selection method, called when component selection has been detected.
	///
	/// \param	aSelectInfo				structure containing selection region and various other information
	///	\param	aSelectionList			out parameter for returning selected components list
	///	\param	aWorldSpaceSelectedPts	out parameter for returning selected points list
	/// \return	True if the object was selected
	///----------------------------------------------------------------------------------------------------
    bool selectVertices( MSelectInfo &aSelectInfo, MSelectionList &aSelectionList,  MPointArray &aWorldSpaceSelectPts ) const;

	///----------------------------------------------------------------------------------------------------
	/// Creates HairShapeUI.
	///
	/// \param aHairShape	hair shape 
	///----------------------------------------------------------------------------------------------------
    static void * creator();

	///----------------------------------------------------------------------------------------------------
	/// Finalizer
	///----------------------------------------------------------------------------------------------------
    virtual ~HairShapeUI();

	///----------------------------------------------------------------------------------------------------
	/// Draw tokens, which determine what request should be put on the draw queue
	///----------------------------------------------------------------------------------------------------
	enum DrawingState {
		kDrawGuides,
		kDrawVertices 
	};

	///----------------------------------------------------------------------------------------------------
	/// Gets the selection mode from the environment and derives the correct DrawingState.
	/// (select guides -> kDrawGuides)
	/// (select vertices, roots, tips -> kDrawVertices)
	///----------------------------------------------------------------------------------------------------
	static DrawingState getDrawingState();

	///----------------------------------------------------------------------------------------------------
	/// Selection indicators, representing the environment variable stubbleSelectionMode
	///----------------------------------------------------------------------------------------------------
	enum SelectionMode {
		kSelectGuides,
		kSelectAllVertices,
		kSelectTips,
		kSelectRoots
	};

	///----------------------------------------------------------------------------------------------------
	/// Gets the selection mode from the environment by using optionVar -q "stubbleSelectionMode".
	///----------------------------------------------------------------------------------------------------
	static SelectionMode getSelectionMode();
	
};

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_SHAPE_UI_HPP
