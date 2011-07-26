#ifndef STUBBLE_REST_POSITIONS_UG_HPP
#define STUBBLE_REST_POSITIONS_UG_HPP

#include "HairShape\HairComponents\GuidePosition.hpp"
#include "HairShape\HairComponents\Segments.hpp"
#include "HairShape\HairComponents\SelectedGuides.hpp"
#include "HairShape\Interpolation\InterpolationGroups.hpp"

#include <fstream>

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

///-------------------------------------------------------------------------------------------------
/// Guides segments uniform grid
///-------------------------------------------------------------------------------------------------
class RestPositionsUG
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	RestPositionsUG();

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	~RestPositionsUG();

	///-------------------------------------------------------------------------------------------------
	/// Builds the uniform grid. 
	///
	/// \author	Sr 4ck 0cuc
	/// \date	26.7.2011
	///
	/// \param	aGuidesRestPositions	The guides rest positions. 
	/// \param	aInterpolationGroups	The interpolation groups.
	///-------------------------------------------------------------------------------------------------
	void build( const GuidesRestPositions & aGuidesRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups );

	///-------------------------------------------------------------------------------------------------
	/// Gets the n closest guides from aPosition. 
	///
	/// \param	aPosition				the position. 
	/// \param	aInterpolationGroupId	Identifier for a interpolation group. 
	/// \param	aN						Number of closest guides to return. 
	/// \param [in,out]	aGuidesIds		List of identifiers for a closest guides. 
	///-------------------------------------------------------------------------------------------------
	void getNClosestGuides( const Vector3D< Real > & aPosition, unsigned int aInterpolationGroupId,
		unsigned int aN, GuidesIds & aGuidesIds );

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

	///-------------------------------------------------------------------------------------------------
	/// Export to file. 
	///
	/// \param [in,out]	aOutputStream	the output stream. 
	///-------------------------------------------------------------------------------------------------
	void exportToFile( std::ostream & aOutputStream );

	///-------------------------------------------------------------------------------------------------
	/// Import from file. 
	///
	/// \param [in,out]	aInputStream	the input stream. 
	///-------------------------------------------------------------------------------------------------
	void importFromFile( std::istream & aInputStream );

private:
	bool mDirtyBit; ///< true to dirty bit
};

// inline functions implementation

inline void RestPositionsUG::setDirty()
{
	mDirtyBit = true;
}

inline bool RestPositionsUG::isDirty() const
{
	return mDirtyBit;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_REST_POSITIONS_UG_HPP