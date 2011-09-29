#ifndef STUBBLE_SEGMENTS_UG_HPP
#define STUBBLE_SEGMENTS_UG_HPP

#include "HairShape\HairComponents\GuidePosition.hpp"
#include "HairShape\HairComponents\Segments.hpp"
#include "HairShape\HairComponents\SelectedGuides.hpp"

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

///-------------------------------------------------------------------------------------------------
/// Guides segments uniform grid
///-------------------------------------------------------------------------------------------------
class SegmentsUG
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	SegmentsUG();

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	~SegmentsUG();

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
	/// Selects the guides using rectangle
	///
	/// \param	aX	X screen coordinate of the top left corner
	/// \param	aY	Y screen coordinate of the top right corner
	/// \param	aW	Width of the rectangle
	/// \param	aH	Height of the rectangle
	/// \param	[in,out]	aResult	The selected guides.
	///-------------------------------------------------------------------------------------------------
	void select( short aX, short aY, short aW, short aH, SelectedGuides &aResult ) const;

	///-------------------------------------------------------------------------------------------------
	/// Selects the guides using selection mask. 
	///
	/// \param	aSelectionMask	The selection mask. 
	/// \param	aX	X screen coordinate of the selection mask
	/// \param	aY	Y screen coordinate of the selection mask
	/// \param	[in,out]	aResult	The selected guides.
	///-------------------------------------------------------------------------------------------------
	void select( const Stubble::Toolbox::CircleToolShape &aSelectionMask, short aX, short aY, SelectedGuides &aResult ) const;

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

inline void SegmentsUG::setDirty()
{
	mDirtyBit = true;
}

inline bool SegmentsUG::isDirty() const
{
	return mDirtyBit;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_SEGMENTS_UG_HPP