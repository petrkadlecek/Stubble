#ifndef STUBBLE_REST_POSITIONS_UG_HPP
#define STUBBLE_REST_POSITIONS_UG_HPP

#include "HairShape\HairComponents\GuidePosition.hpp"
#include "HairShape\HairComponents\Segments.hpp"
#include "HairShape\Interpolation\InterpolationGroups.hpp"

#include "kdtmpl.h"

#include <fstream>

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

///----------------------------------------------------------------------------------------------------
/// Identifier and distance. 
///----------------------------------------------------------------------------------------------------
struct IdAndDistance
{
	GuideId mGuideId;	// Guide id

	Real mDistance;	// Distance to guide's rest position
};

///----------------------------------------------------------------------------------------------------
/// Defines an alias representing list of identifiers for the closest guides .
///----------------------------------------------------------------------------------------------------
typedef std::vector< IdAndDistance> ClosestGuidesIds;

///----------------------------------------------------------------------------------------------------
/// Defines a KDTree for finding N closest points for the query point.
///----------------------------------------------------------------------------------------------------
typedef KdTreeTmplPtr<Stubble::Vector3D<Real>, Stubble::Vector3D<Real>> KdTree;

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
	/// \param	aGuidesRestPositions	The guides rest positions. 
	/// \param	aInterpolationGroups	The interpolation groups.
	///-------------------------------------------------------------------------------------------------
	void build( const GuidesRestPositions & aGuidesRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups );

	///-------------------------------------------------------------------------------------------------
	/// Gets the n closest guides from aPosition. 
	///
	/// \param	aPosition					the position. 
	/// \param	aInterpolationGroupId		Identifier for a interpolation group. 
	/// \param	aN							Number of closest guides to return. 
	/// \param [in,out]	aClosestGuidesIds	List of identifiers for a closest guides. 
	///-------------------------------------------------------------------------------------------------
	void getNClosestGuides( const Vector3D< Real > & aPosition, unsigned __int32 aInterpolationGroupId,
		unsigned __int32 aN, ClosestGuidesIds & aClosestGuidesIds ) const;

	std::vector< const Vector3D< Real > * > getNClosestGuides( const Vector3D< Real > & aPosition, unsigned __int32 aInterpolationGroupId,
		unsigned __int32 aN ) const;

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
	/// \param [in,out]	aOutputStream	The output stream. 
	///-------------------------------------------------------------------------------------------------
	void exportToFile( std::ostream & aOutputStream ) const;

	///-------------------------------------------------------------------------------------------------
	/// Import rest positions from file and builds the uniform grid.
	///
	/// \param [in,out]	aInputStream	The input stream. 
	/// \param	aInterpolationGroups	The interpolation groups.
	///-------------------------------------------------------------------------------------------------
	void importFromFile( std::istream & aInputStream,
		const Interpolation::InterpolationGroups & aInterpolationGroups );

private:

	///-------------------------------------------------------------------------------------------------
	/// Builds the uniform grid from already stored guides rest position. 
	///
	/// \param	aInterpolationGroups	The interpolation groups.
	///-------------------------------------------------------------------------------------------------
	void innerBuild( const Interpolation::InterpolationGroups & aInterpolationGroups,
		const std::vector< unsigned int > &groupIdentifiers );

	bool mDirtyBit; ///< true to dirty bit

	///----------------------------------------------------------------------------------------------------
	/// Defines an alias representing the positions .
	///----------------------------------------------------------------------------------------------------
	typedef std::vector< Vector3D< Real > > Positions;

	Positions mGuidesRestPositions;   ///< The guides rest positions

	KdTree *mKdForest;	///< KDTree forest for closest points query

	unsigned int mForestSize;	///< Size of forrest
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