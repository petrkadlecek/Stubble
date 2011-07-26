#ifndef STUBBLE_HAIR_SHAPE_HPP
#define STUBBLE_HAIR_SHAPE_HPP

#include <maya\MPxSurfaceShapeUI.h>

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
	///----------------------------------------------------------------------------------------------------
    virtual bool select( MSelectInfo &selectInfo, MSelectionList &selectionList,  MPointArray &worldSpaceSelectPts ) const;

	///----------------------------------------------------------------------------------------------------
	/// Creates HairShapeUI.
	///----------------------------------------------------------------------------------------------------
    static void * creator();

	///----------------------------------------------------------------------------------------------------
	/// Finalizer
	///----------------------------------------------------------------------------------------------------
    virtual ~HairShapeUI(); 
};

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_SHAPE_HPP
