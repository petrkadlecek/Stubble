#ifndef STUBBLE_SEGMENTS_DS_HPP
#define STUBBLE_SEGMENTS_DS_HPP

#include "HairShape\HairComponents\GuidePosition.hpp"
#include "HairShape\HairComponents\Segments.hpp"
#include "HairShape\HairComponents\SelectedGuides.hpp"

#include <maya/MPointArray.h>
#include <maya/MSelectionList.h>
#include <maya/MSelectInfo.h>

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

///-------------------------------------------------------------------------------------------------
/// Guides segments uniform grid
///-------------------------------------------------------------------------------------------------
class SegmentsDS
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	SegmentsDS();

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	~SegmentsDS();

	///-------------------------------------------------------------------------------------------------
	/// Builds the uniform grid using selection information, the end product is a grid built using only
	/// selected guides. TODO: add selection filter support
	///
	/// \param	aGuidesCurrentPositions			the guides current positions. 
	/// \param	aFrameSegments					the guides segments in current frame.
	/// \param	aGuidesVerticesEndIndices		the array of last indices of vertices for each guide
	/// \param	aGuidesInterpolationGroupIds	the array of interpolation group indices for each guide
	/// \param	aInterpolationGroupsSelectable	the array containing the "is selectable" flag for each interpolation group
	/// \param	aSelectInfo						structure containing selection region and various other information
	///	\param	aSelectionList					out parameter for returning selected components list. (For MPxSurfaceShape::select compatibility reasons)
	///	\param	aWorldSpaceSelectedPts			out parameter for returning selected points list. (For MPxSurfaceShape::select compatibility reasons)
	///	\param	aSelectedGuides					out parameter for returning currently selected guides.
	/// \return Has anything been selected?
	///-------------------------------------------------------------------------------------------------
	bool build( const GuidesCurrentPositions & aGuidesCurrentPositions,
		const FrameSegments & aFrameSegments,
		const std::vector< unsigned __int32 > & aGuidesVerticesEndIndices,
		const std::vector< unsigned __int32 > & aGuidesInterpolationGroupIds,
		const std::vector< unsigned __int32 > & aInterpolationGroupsSelectable,
		MSelectInfo & aSelectInfo,
		MSelectionList & aSelectionList,
		MPointArray & aWorldSpaceSelectedPts,
		SelectedGuides & aSelectedGuides);

	///-------------------------------------------------------------------------------------------------
	/// Builds the uniform grid using selection information, the end product is a grid built using only
	/// selected guides. TODO: add selection filter support
	///
	/// \param	aGuidesCurrentPositions		the guides current positions. 
	/// \param	aFrameSegments				the guides segments in current frame. 
	/// \param	aGuidesVerticesEndIndices	the array of last indices of vertices for each guide
	/// \param	aSelectedComponentIndices	the component list received from Maya's global selection list
	///	\param	aSelectedGuides				out parameter for returning currently selected guides.
	///-------------------------------------------------------------------------------------------------
	void build( const GuidesCurrentPositions & aGuidesCurrentPositions,
		const FrameSegments & aFrameSegments,
		const std::vector< unsigned __int32 > & aGuidesVerticesEndIndices,
		MIntArray &aSelectedComponentIndices,
		SelectedGuides & aSelectedGuides);

	///-------------------------------------------------------------------------------------------------
	/// Builds the uniform grid. 
	///
	/// \param	aGuidesCurrentPositions	the guides current positions. 
	/// \param	aFrameSegments			the guides segments in current frame. 
	///-------------------------------------------------------------------------------------------------
	void build( const GuidesCurrentPositions & aGuidesCurrentPositions, 
		const FrameSegments & aFrameSegments );

	///-------------------------------------------------------------------------------------------------
	/// Builds the part of uniform grid holding the selected guides.
	///
	/// \param	aSelectedGuides	the selected guides.
	/// \param	aFullBuild	if false, only updates "dirty" selected guides 
	///-------------------------------------------------------------------------------------------------
	void build( const SelectedGuides & aSelectedGuides, bool aFullBuild );

	///-------------------------------------------------------------------------------------------------
	/// Selects the guides using selection mask. 
	///
	/// \param	aSelectionMask	The selection mask. 
	/// \param	aX	X screen coordinate of the selection mask
	/// \param	aY	Y screen coordinate of the selection mask
	/// \param	[in,out]	aResult	The selected guides.
	///-------------------------------------------------------------------------------------------------
	void select( Stubble::Toolbox::CircleToolShape *aSelectionMask, short aX, short aY, SelectedGuides &aResult ) const;

	/// TODO
	void select( Stubble::Toolbox::SphereToolShape *aSelectionMask, SelectedGuides &aResult ) const;

	///-------------------------------------------------------------------------------------------------
	/// Sets the dirty bit. 
	///-------------------------------------------------------------------------------------------------
	inline void setDirty();

	///-------------------------------------------------------------------------------------------------
	/// Query if this object is dirty. 
	///
	/// \return	true if dirty, false if not. 
	///-------------------------------------------------------------------------------------------------
	inline bool isDirty() const;

private:
	bool mDirtyBit; ///< true to dirty bit

	SelectedGuides mStoredGuides; ///< Testing data storage, TODO: remove me
};

// inline functions implementation

inline void SegmentsDS::setDirty()
{
	mDirtyBit = true;
}

inline bool SegmentsDS::isDirty() const
{
	return mDirtyBit;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_SEGMENTS_UG_HPP