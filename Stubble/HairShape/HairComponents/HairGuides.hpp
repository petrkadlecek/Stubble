#ifndef STUBBLE_HAIR_GUIDES_HPP
#define STUBBLE_HAIR_GUIDES_HPP

#include "HairShape\Generators\RandomGenerator.hpp"
#include "HairShape\Generators\UVPointGenerator.hpp"
#include "HairShape\HairComponents\DisplayedGuides.hpp"
#include "HairShape\HairComponents\GuidePosition.hpp"
#include "HairShape\HairComponents\RestPositionsDS.hpp"
#include "HairShape\HairComponents\SegmentsStorage.hpp"
#include "HairShape\HairComponents\SegmentsUG.hpp"
#include "HairShape\HairComponents\SelectedGuides.hpp"
#include "HairShape\HairComponents\UndoStack.hpp"
#include "HairShape\Mesh\MayaMesh.hpp"
#include "HairShape\Interpolation\InterpolationGroups.hpp"
#include "Common\CommonConstants.hpp"
#include "Common\CommonFunctions.hpp"

#include <sstream>
#include <string>
#include <fstream>

#include <maya/MStringArray.h>

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

///----------------------------------------------------------------------------------------------------
/// Class for storing hair guides.
/// This class allow to select, draw and update guides ( also supports undo/redo operations ),
/// calculation of bounding box of hair guides, importing/exporting to/from NURBS curves and other
/// trivial operations.
/// Class reacts to external events such as : interpolation groups and segments count change,
/// mesh change or time change.
/// This class can be serialized/deserialized.  
///----------------------------------------------------------------------------------------------------
class HairGuides
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Constructor.
	/// Creates empty hair guides object.
	///----------------------------------------------------------------------------------------------------
	HairGuides();

	///----------------------------------------------------------------------------------------------------
	/// Finalizer.
	///----------------------------------------------------------------------------------------------------
	~HairGuides();

	///----------------------------------------------------------------------------------------------------
	/// Applies the selection described by aSelectionMask.
	/// Selection is applied to current hair guides segments by internal SegmentsUG object and
	/// then selected guides/segments are redisplayed.
	///
	/// \param	aInterpolationGroupsSelectable	The array containing the "is selectable" flag for each interpolation group
	/// \param	aSelectInfo						Structure containing selection region and various other information
	///	\param	aSelectionList					Out parameter for returning selected components list. 
	///											(For MPxSurfaceShape::select compatibility reasons)
	///	\param	aWorldSpaceSelectedPts			Out parameter for returning selected points list. 
	///											(For MPxSurfaceShape::select compatibility reasons)
	/// \return Has anything been selected ?
	///----------------------------------------------------------------------------------------------------
	bool applySelection( const std::vector< unsigned __int32 > & aInterpolationGroupsSelectable, 
		MSelectInfo & aSelectInfo, MSelectionList & aSelectionList,  MPointArray & aWorldSpaceSelectPts );

	///----------------------------------------------------------------------------------------------------
	/// Applies the selection received from Maya's global selection list.
	/// This method is used to synchronize the node's internal selection list, which has no other way
	/// of accessing information about the keyboard modifiers being pressed during selection.
	///
	/// \param	aSelectedComponentIndices		The component list received from Maya's global selection list
	///----------------------------------------------------------------------------------------------------
	void applySelection( MIntArray &aSelectedComponentIndices );

	///----------------------------------------------------------------------------------------------------
	/// Gets the bounding box of hair guides segments in current frame. 
	///
	/// \return	The bounding box. 
	///----------------------------------------------------------------------------------------------------
	BoundingBox getBoundingBox();

	///----------------------------------------------------------------------------------------------------
	/// Gets the selected guides segments data structure. This data structure is only updated after 
	/// selection or on demand and it's dirty flag is only set, when the selected guides are changed due to 
	/// not brushing event. The data structure is stored inside this class and updated only when this 
	/// method is called ( and only if necessary ).
	///
	/// \return	The selected guides segments data structure.
	///----------------------------------------------------------------------------------------------------
	const SegmentsUG & getSelectedGuidesDS();

	///----------------------------------------------------------------------------------------------------
	/// Updates the guides after the brush or any other tool was used. 
	/// This method has two modes : store update - stores changes to internal structures and updates the
	/// stack, non-store update - only redisplay changed hair guides segments
	/// 
	/// \param	aStoreUpdate	if true, stores updates of selected guides to stack, 
	/// 						otherwise draw update only
	///----------------------------------------------------------------------------------------------------
	void updateGuides( bool aStoreUpdate );

	///-------------------------------------------------------------------------------------------------
	/// Reinits all cutted hair with zero length to initial length multiplied by scale factor.
	/// The scale factor is used for scaling the hair guides so they have propert length comparet to 
	/// geometry they grow on.
	///
	/// \param	aScaleFactor	Scale factor.
	///-------------------------------------------------------------------------------------------------
	void reinitCuttedHair( Real aScaleFactor );

	///-------------------------------------------------------------------------------------------------
	/// Resets guide hair to their initial configuration
	/// 
	/// \param aScaleFactor Scale factor
	///-------------------------------------------------------------------------------------------------
	void resetGuides ( Real aScaleFactor );

	///----------------------------------------------------------------------------------------------------
	/// Gets the guides positions data structure. 
	/// Since guides rest positions data structure needs interpolation groups. The data structure is stored 
	/// inside this class and updated only when this method is called ( and only if necessary ). 
	///
	/// \param	aInterpolationGroups	the interpolation groups object 
	///
	/// \return	The guides positions data structure. 
	///----------------------------------------------------------------------------------------------------
	const RestPositionsDS & getGuidesPositionsDS( const Interpolation::InterpolationGroups & aInterpolationGroups );

	///----------------------------------------------------------------------------------------------------
	/// Draws hair guides.
	///
	/// \param	aDrawVerts	Should the hair vertices be drawn?
	///----------------------------------------------------------------------------------------------------
	void draw( bool aDrawVerts = false );

	///----------------------------------------------------------------------------------------------------
	/// Import NURBS.
	/// Imports only NURBS to which hair guides has been exported before. Import expects that Maya
	/// Hair System is build on top of these curves.
	/// For every frame in Maya animation control we create new time frame inside our object and for
	/// each frame we import segments of all hair guides.
	/// 
	/// \param	aInterpolationGroups	The interpolation groups object for segments count retrieval.
	///----------------------------------------------------------------------------------------------------
	void importNURBS( const Interpolation::InterpolationGroups & aInterpolationGroups );

	///----------------------------------------------------------------------------------------------------
	/// Export hair guides to NURBS curves.
	/// NURBS curves names are stored for later import.
	///----------------------------------------------------------------------------------------------------
	void exportToNURBS();

	///----------------------------------------------------------------------------------------------------
	/// Sets a current time.
	/// Uses segments storage to switch hair guides segments to selected frame. 
	///
	/// \param	aTime	the time. 
	///----------------------------------------------------------------------------------------------------
	void setCurrentTime( Time aTime );

	///----------------------------------------------------------------------------------------------------
	/// Informs the object, that the underlying mesh was changed.
	/// Hair guides roots positions must be recalculated, if topology was also changed, new positions for 
	/// hair will be generated and segments will be interpolated from old ones.
	///
	/// \param	aMayaMesh				Maya mesh object.
	/// \param	aInterpolationGroups	The interpolation groups object 
	/// \param	aTopologyChanged		True if topology of the mesh has changed.
	///----------------------------------------------------------------------------------------------------
	void meshUpdate( const MayaMesh & aMayaMesh, const Interpolation::InterpolationGroups & aInterpolationGroups,
		bool aTopologyChanged = false );

	///----------------------------------------------------------------------------------------------------
	/// Undoes changes to hair guides segments.
	///----------------------------------------------------------------------------------------------------
	void undo();

	///----------------------------------------------------------------------------------------------------
	/// Redoes changes to hair guides segments.
	///----------------------------------------------------------------------------------------------------
	void redo();

	///-------------------------------------------------------------------------------------------------
	/// Queries if we can undo changes to hair guides segments.
	///
	/// \return	true if we can undo changes to hair guides segments.
	///-------------------------------------------------------------------------------------------------
	inline bool canUndo() const;

	///-------------------------------------------------------------------------------------------------
	/// Queries if we can redo changes to hair guides segments.
	///
	/// \return	true if we can redo changes to hair guides segments.
	///-------------------------------------------------------------------------------------------------
	inline bool canRedo() const;

	///----------------------------------------------------------------------------------------------------
	/// Empties history stack of hair guides segments changes.
	///----------------------------------------------------------------------------------------------------
	void emptyHistoryStack();

	///----------------------------------------------------------------------------------------------------
	/// Generates hair guides.
	/// This method works in two modes : if interpolate from previous is set as false, hair guides are
	/// generated with scaled default length pointing along surface normal ( scaled to match mesh size ).
	/// When interpolate from previous is true hair guides segments are interpolated from old guides 
	/// segments.
	///
	/// \param [in,out]	aUVPointGenerator	The uv point generator. 
	/// \param	aMayaMesh					The maya mesh. 
	/// \param	aInterpolationGroups		The interpolation groups object 
	/// \param	aCount						Number of hair guides. 
	/// \param	aInterpolateFromPrevious	True to an interpolate from old guides. 
	/// \param	aScaleFactor				Scale factor.
	///----------------------------------------------------------------------------------------------------
	void generate( UVPointGenerator & aUVPointGenerator, const MayaMesh & aMayaMesh, 
		const Interpolation::InterpolationGroups & aInterpolationGroups, unsigned __int32 aCount,
		Real aScaleFactor, bool aInterpolateFromPrevious = false );

	///-------------------------------------------------------------------------------------------------
	/// Updates the segments count of all hair guides described by aInterpolationGroups.
	/// Must be called whenever interpolation groups or segment count is changed. 
	///
	/// \param	aInterpolationGroups	The interpolation groups object 
	///-------------------------------------------------------------------------------------------------
	void updateSegmentsCount( const Interpolation::InterpolationGroups & aInterpolationGroups );

	///-------------------------------------------------------------------------------------------------
	/// Sets a number of guides to interpolate from.
	/// This number is used during interpolation of guides ( see generate method ), which calculates new
	/// guides segments from requested number of closest guides segments.
	///
	/// \param	aNumberOfGuidesToInterpolateFrom	The number of guides to interpolate from. 
	///-------------------------------------------------------------------------------------------------
	inline void setNumberOfGuidesToInterpolateFrom( unsigned __int32 aNumberOfGuidesToInterpolateFrom );

	///----------------------------------------------------------------------------------------------------
	/// Gets the hair guides segments for current frame. 
	///
	/// \return	The hair guides segments for current frame. 
	///----------------------------------------------------------------------------------------------------
	inline const FrameSegments & getCurrentFrameSegments() const;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing the indices .
	///-------------------------------------------------------------------------------------------------
	typedef std::vector< unsigned __int32 > Indices;
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the index to first vertex of guide in all guides vertices array
	///
	/// \return	Index to first vertex of guide in all guides vertices array. 
	///-------------------------------------------------------------------------------------------------
	inline const Indices & guidesVerticesStartIndex() const;

	///-------------------------------------------------------------------------------------------------
	/// Serialize object data.
	/// Stores only data that can not be generated during deserialization.
	/// 
	/// \param	aOutputStream	Output stream.
	///-------------------------------------------------------------------------------------------------
	void serialize( std::ostream & aOutputStream ) const;

	///-------------------------------------------------------------------------------------------------
	/// Deserialize object data.	
	/// After deserialization several non-serialized data are generated.
	///
	/// \param	aMayaMesh				Maya mesh 
	/// \param	aInterpolationGroups	The interpolation groups object 
	/// \param	aInputStream			Input stream 
	///-------------------------------------------------------------------------------------------------
	void deserialize( const MayaMesh & aMayaMesh, const Interpolation::InterpolationGroups & aInterpolationGroups,
		std::istream & aInputStream );

private:	

	///-------------------------------------------------------------------------------------------------
	/// Updates the selected guides if mesh, time or undo/redo event happens. 
	///-------------------------------------------------------------------------------------------------
	void updateSelectedGuides();

	///-------------------------------------------------------------------------------------------------
	/// Clears the selected guides. 
	///-------------------------------------------------------------------------------------------------
	void clearSelectedGuides();

	///-------------------------------------------------------------------------------------------------
	/// Refresh interpolation group identifiers.
	/// For each guide we store interpolation group id for selection purpose ( when interpolation
	/// groups selection filter is applied ). 
	/// 
	/// \param	aInterpolationGroups	the interpolation groups object 
	///-------------------------------------------------------------------------------------------------
	void refreshInterpolationGroupIds( const Interpolation::InterpolationGroups & aInterpolationGroups );

	RestPositionsDS mRestPositionsDS;   ///< The rest positions data structure

	SegmentsUG mAllSegmentsUG; ///< The all segments data structure

	SegmentsUG mSelectedSegmentsUG; ///< The selected segments data structure

	SegmentsStorage * mSegmentsStorage;   ///< The segments storage

	GuidesRestPositions mRestPositions;   ///< The rest positions of hair guides roots

	GuidesCurrentPositions mCurrentPositions; ///< The current positions of hair guides

	DisplayedGuides mDisplayedGuides;   ///< The displayed guides

	UndoStack mUndoStack;   ///< Stack of guides changes.

	SelectedGuides mSelectedGuides; ///< The selected guides ( via Maya selection tool )

	BoundingBox mBoundingBox;   ///< The bounding box of current guides

	MStringArray mNurbsCurvesNames;   ///< The NURBS curves names ( will be used during import )

	bool mBoundingBoxDirtyFlag; ///< Should be bounding box recalculated ?

	unsigned __int32 mNumberOfGuidesToInterpolateFrom;  ///< The number of guides to interpolate from

	Indices mGuidesVerticesStartIndex;  ///< Index to first vertex of guide in all guides vertices array

	Indices mGuidesInterpolationGroupIds;	///< Identifiers for the guides interpolation group
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

inline const FrameSegments & HairGuides::getCurrentFrameSegments() const
{
	if ( mSegmentsStorage == 0 )
	{
		throw StubbleException( " HairGuides::getCurrentFrameSegments : \
								method can not be called before segments generation " );
	}
	return mSegmentsStorage->getCurrentSegments();
}

inline const HairGuides::Indices & HairGuides::guidesVerticesStartIndex() const
{
	return mGuidesVerticesStartIndex;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_GUIDES_HPP