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
	/// Draws the guides.
	///-------------------------------------------------------------------------------------------------
	void draw() const;

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

	unsigned int mVBO;  ///< The display structure
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