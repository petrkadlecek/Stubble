#ifndef STUBBLE_DISPLAYED_GUIDES_HPP
#define STUBBLE_DISPLAYED_GUIDES_HPP

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
/// Class for displaying guides .
///-------------------------------------------------------------------------------------------------
class DisplayedGuides
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Constructor
	///----------------------------------------------------------------------------------------------------
	DisplayedGuides();

	///----------------------------------------------------------------------------------------------------
	/// Finalizer
	///----------------------------------------------------------------------------------------------------
	~DisplayedGuides();

	///-------------------------------------------------------------------------------------------------
	/// Builds the display structure
	///
	/// \param	aGuidesCurrentPositions	the guides current positions. 
	/// \param	aFrameSegments			the guides segments in current frame. 
	/// \param	aSelectedGuides			the selected guides to be high lighted. 
	///-------------------------------------------------------------------------------------------------
	void build( const GuidesCurrentPositions & aGuidesCurrentPositions, 
		const FrameSegments & aFrameSegments,
		const SelectedGuides & aSelectedGuides );

	///-------------------------------------------------------------------------------------------------
	/// Redisplays the selected guides
	///
	/// \param	aSelectedGuides	the selected guides. 
	/// \param	aHighlight		highlight the selected guides ?
	/// 						
	///-------------------------------------------------------------------------------------------------
	void selectionRebuild( const SelectedGuides & aSelectedGuides, bool aHighlight );

	///-------------------------------------------------------------------------------------------------
	/// Draws the guides using additional info - selection filter, hide/unhide non-selected guides, etc.
	///
	/// \param	aDrawVerts	should the hair vertices be drawn?
	///-------------------------------------------------------------------------------------------------
	void draw( bool aDrawVerts = false ) const;

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

	///-------------------------------------------------------------------------------------------------
	/// Draws guides as coloured poly-lines
	///-------------------------------------------------------------------------------------------------
	void drawPolyline() const;

	///-------------------------------------------------------------------------------------------------
	/// Draws guides as coloured vertices
	///-------------------------------------------------------------------------------------------------
	void drawVertices() const;

	bool mDirtyBit; ///< true to dirty bit

	bool mHighLightSelected;	///< true to high light selected

	bool mHideNonSelected;		///< True if the user wants to hide non-selected guides

	const FrameSegments * mFrameSegments;   ///< The frame segments

	const SelectedGuides * mSelectedGuides; ///< The selected guides

	const GuidesCurrentPositions * mGuidesCurrentPositions;   ///< The guides current positions

	///----------------------------------------------------------------------------------------------------
	/// Defines an alias representing the guides selection .
	///----------------------------------------------------------------------------------------------------
	typedef std::vector< bool > GuidesSelection;

	GuidesSelection mGuidesSelection;   ///< The guides selection ( true = selected )
};

// inline functions implementation

inline void DisplayedGuides::setDirty()
{
	mDirtyBit = true;
}

inline bool DisplayedGuides::isDirty() const
{
	return mDirtyBit;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_DISPLAYED_GUIDES_HPP