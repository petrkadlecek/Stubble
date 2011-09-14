#ifndef STUBBLE_HAIR_GUIDES_HPP
#define STUBBLE_HAIR_GUIDES_HPP

#include "HairShape\Generators\RandomGenerator.hpp"
#include "HairShape\Generators\UVPointGenerator.hpp"
#include "HairShape\HairComponents\DisplayedGuides.hpp"
#include "HairShape\HairComponents\GuidePosition.hpp"
#include "HairShape\HairComponents\RestPositionsUG.hpp"
#include "HairShape\HairComponents\SegmentsStorage.hpp"
#include "HairShape\HairComponents\SegmentsUG.hpp"
#include "HairShape\HairComponents\SelectedGuides.hpp"
#include "HairShape\HairComponents\UndoStack.hpp"
#include "HairShape\Mesh\MayaMesh.hpp"
#include "HairShape\Interpolation\InterpolationGroups.hpp"

#include <fstream>

#include <maya/MFnMesh.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MPointArray.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MAnimControl.h>
#include <maya/MTime.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

///----------------------------------------------------------------------------------------------------
/// Class stores hair guides, implements undo stack and selection.
///----------------------------------------------------------------------------------------------------
class HairGuides
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Constructor
	///----------------------------------------------------------------------------------------------------
	HairGuides();

	///----------------------------------------------------------------------------------------------------
	/// Finalizer
	///----------------------------------------------------------------------------------------------------
	~HairGuides();

	///----------------------------------------------------------------------------------------------------
	/// Applies the selection described by aSelectionMask. 
	///
	/// \param		the selection mask. 
	///----------------------------------------------------------------------------------------------------
	void applySelection( const SelectionMask & aSelectionMask );

	///----------------------------------------------------------------------------------------------------
	/// Gets the bounding box. 
	///
	/// \return	The bounding box. 
	///----------------------------------------------------------------------------------------------------
	BoundingBox getBoundingBox();

	///----------------------------------------------------------------------------------------------------
	/// Gets the selected guides segments uniform grid. This grid is only updated after selection or on 
	/// demand and it's dirty flag is never set by HairGuides class.
	///
	/// \return	The selected guides segments uniform grid. 
	///----------------------------------------------------------------------------------------------------
	const SegmentsUG & getSelectedGuidesUG();

	///----------------------------------------------------------------------------------------------------
	/// Updates the guides after the brush or any other tool was used. 
	/// 
	/// \param	aStoreUpdate	if true, stores updates of selected guides to stack, 
	/// 						otherwise draw update only
	///----------------------------------------------------------------------------------------------------
	void updateGuides( bool aStoreUpdate );

	///----------------------------------------------------------------------------------------------------
	/// Gets the guides positions uniform grid. 
	///
	/// \param	aInterpolationGroups	the interpolation groups object 
	///
	/// \return	The guides positions uniform grid. 
	///----------------------------------------------------------------------------------------------------
	const RestPositionsUG & getGuidesPositionsUG( const Interpolation::InterpolationGroups & aInterpolationGroups );

	///----------------------------------------------------------------------------------------------------
	/// Draws guides and interpolated hair.
	///----------------------------------------------------------------------------------------------------
	void draw();

	///----------------------------------------------------------------------------------------------------
	/// Import nurbs.
	///----------------------------------------------------------------------------------------------------
	void importNURBS();

	///----------------------------------------------------------------------------------------------------
	/// Export nurbs.
	///----------------------------------------------------------------------------------------------------
	void exportNURBS();

	///----------------------------------------------------------------------------------------------------
	/// Sets a current time. 
	///
	/// \param	aTime	the time. 
	///----------------------------------------------------------------------------------------------------
	void setCurrentTime( Time aTime );

	///----------------------------------------------------------------------------------------------------
	/// Informs the object, that the underlying mesh was changed.
	///
	/// \param	aMayaMesh			Maya mesh object.
	/// \param	aTopologyChanged	true if topology of the mesh has changed.
	/// 
	/// \return number of guides	
	///----------------------------------------------------------------------------------------------------
	GuideId meshUpdate( const MayaMesh & aMayaMesh, bool aTopologyChanged = false );

	///----------------------------------------------------------------------------------------------------
	/// Undoes changed to hair guides.
	///----------------------------------------------------------------------------------------------------
	void undo();

	///----------------------------------------------------------------------------------------------------
	/// Redoes changes to hair guides.
	///----------------------------------------------------------------------------------------------------
	void redo();

	///-------------------------------------------------------------------------------------------------
	/// Queries if we can undo. 
	///
	/// \return	true if it succeeds, false if it fails. 
	///-------------------------------------------------------------------------------------------------
	inline bool canUndo() const;

	///-------------------------------------------------------------------------------------------------
	/// Queries if we can redo. 
	///
	/// \return	true if it succeeds, false if it fails. 
	///-------------------------------------------------------------------------------------------------
	inline bool canRedo() const;

	///----------------------------------------------------------------------------------------------------
	/// Empties history stack of hair guides changes.
	///----------------------------------------------------------------------------------------------------
	void emptyHistoryStack();

	///----------------------------------------------------------------------------------------------------
	/// Generates hair guides pointing along surface normal or interpolate them from old guides if 
	/// aInterpolateFromPrevious equals true.
	///
	/// \param [in,out]	aUVPointGenerator	a uv point generator. 
	/// \param	aMayaMesh					a maya mesh. 
	/// \param	aInterpolationGroups		the interpolation groups object 
	/// \param	aCount						Number of hair guides. 
	/// \param	aInterpolateFromPrevious	true to an interpolate from old guides. 
	///----------------------------------------------------------------------------------------------------
	void generate( UVPointGenerator & aUVPointGenerator, const MayaMesh & aMayaMesh, 
		const Interpolation::InterpolationGroups & aInterpolationGroups, unsigned __int32 aCount, 
		bool aInterpolateFromPrevious = false );

	///-------------------------------------------------------------------------------------------------
	/// Updates the segments count described by aInterpolationGroups. 
	///
	/// \param	aInterpolationGroups	the interpolation groups object 
	///-------------------------------------------------------------------------------------------------
	void updateSegmentsCount( const Interpolation::InterpolationGroups & aInterpolationGroups );

	///-------------------------------------------------------------------------------------------------
	/// Export to file.
	///
	/// \param [in,out]	aOutputStream	the output stream. 
	///-------------------------------------------------------------------------------------------------
	void exportToFile( std::ostream & aOutputStream ) const;

	///-------------------------------------------------------------------------------------------------
	/// Sets a number of guides to interpolate from. 
	///
	/// \param	aNumberOfGuidesToInterpolateFrom	a number of guides to interpolate from. 
	///-------------------------------------------------------------------------------------------------
	inline void setNumberOfGuidesToInterpolateFrom( unsigned __int32 aNumberOfGuidesToInterpolateFrom );

private:
	RestPositionsUG mRestPositionsUG;   ///< The rest positions uniform grid

	SegmentsUG mAllSegmentsUG; ///< The all segments uniform grid

	SegmentsUG mSelectedSegmentsUG; ///< The selected segments uniform grid

	SegmentsStorage * mSegmentsStorage;   ///< The segments storage

	GuidesRestPositions mRestPositions;   ///< The rest positions of hair guides

	GuidesCurrentPositions mCurrentPositions; ///< The current positions of hair guides

	DisplayedGuides mDisplayedGuides;   ///< The displayed guides

	UndoStack mUndoStack;   ///< Stack of guides changes.

	SelectedGuides mSelectedGuides; ///< The selected guides

	BoundingBox mBoundingBox;   ///< The bounding box of current guides

	bool mBoundingBoxDirtyFlag; ///< true to bounding box dirty flag

	unsigned __int32 mNumberOfGuidesToInterpolateFrom;  ///< The number of guides to interpolate from
};

// inline functions implementation

inline bool HairGuides::canUndo() const
{
	return mUndoStack.canUndo();
}

inline bool HairGuides::canRedo() const
{
	return mUndoStack.canRedo();
}

inline void HairGuides::setNumberOfGuidesToInterpolateFrom( unsigned __int32 aNumberOfGuidesToInterpolateFrom )
{
	mNumberOfGuidesToInterpolateFrom = aNumberOfGuidesToInterpolateFrom;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_GUIDES_HPP