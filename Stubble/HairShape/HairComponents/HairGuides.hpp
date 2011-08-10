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
#include "HairShape\HairLength\LengthInfo.hpp"
#include "HairShape\Mesh\MayaMesh.hpp"
#include "HairShape\Interpolation\InterpolationGroups.hpp"

#include <fstream>

#include <maya/MFnMesh.h>

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
	/// Gets the selected guides. 
	///
	/// \return	The selected guides. 
	///----------------------------------------------------------------------------------------------------
	SelectedGuides & getSelectedGuides(); 

	///----------------------------------------------------------------------------------------------------
	/// Gets the guides segments uniform grid. 
	///
	/// \return	The guides segments uniform grid. 
	///----------------------------------------------------------------------------------------------------
	const SegmentsUG & getGuidesSegmentsUG();

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
	/// Import nurbs. TODO !!!
	///----------------------------------------------------------------------------------------------------
	void importNURBS( void );

	///----------------------------------------------------------------------------------------------------
	/// Export nurbs. TODO !!!
	///----------------------------------------------------------------------------------------------------
	void exportNURBS( void );

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
	///----------------------------------------------------------------------------------------------------
	void meshUpdate( const MayaMesh & aMayaMesh, bool aTopologyChanged = false );

	///----------------------------------------------------------------------------------------------------
	/// Undoes changed to hair guides.
	///----------------------------------------------------------------------------------------------------
	void undo();

	///----------------------------------------------------------------------------------------------------
	/// Redoes changes to hair guides.
	///----------------------------------------------------------------------------------------------------
	void redo();

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
	/// \param	aLengthInfo					Object describing a length of guides. 
	/// \param	aCount						Number of hair guides. 
	/// \param	aInterpolateFromPrevious	true to an interpolate from old guides. 
	///----------------------------------------------------------------------------------------------------
	void generate( UVPointGenerator & aUVPointGenerator, const MayaMesh & aMayaMesh, 
		const Interpolation::InterpolationGroups & aInterpolationGroups, const HairLength::LengthInfo & aLengthInfo, 
		int aCount, bool aInterpolateFromPrevious = false );

	///-------------------------------------------------------------------------------------------------
	/// Updates the segments count described by aInterpolationGroups. 
	///
	/// \param	aInterpolationGroups	the interpolation groups object 
	///-------------------------------------------------------------------------------------------------
	void updateSegmentsCount( const Interpolation::InterpolationGroups & aInterpolationGroups );

	///-------------------------------------------------------------------------------------------------
	/// Updates the length of hair guides described by aLengthInfo. 
	///
	/// \param	aLengthInfo	Object describing a length. 
	///-------------------------------------------------------------------------------------------------
	void updateLength( const HairLength::LengthInfo & aLengthInfo );

	///-------------------------------------------------------------------------------------------------
	/// Export to file.
	///
	/// \param [in,out]	aOutputStream	the output stream. 
	///-------------------------------------------------------------------------------------------------
	void exportToFile( std::ostream & aOutputStream );

private:
	RestPositionsUG mRestPositionsUG;   ///< The rest positions uniform grid

	SegmentsUG mSegmentsUG; ///< The segments uniform grid

	SegmentsStorage * mSegmentsStorage;   ///< The segments storage

	GuidesRestPositions mRestPositions;   ///< The rest positions of hair guides

	GuidesCurrentPositions mCurrentPositions; ///< The current positions of hair guides

	DisplayedGuides mDisplayedGuides;   ///< The displayed guides

	UndoStack mUndoStack;   ///< Stack of guides changes.

	SelectedGuides mSelectedGuides; ///< The selected guides
};

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_GUIDES_HPP