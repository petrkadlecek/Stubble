#ifndef STUBBLE_REST_POSITIONS_DS_HPP
#define STUBBLE_REST_POSITIONS_DS_HPP

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
/// Structure for hodling guide identifier and distance from that guide.
/// Used as output of RestPositionsDS closest guides query.
///----------------------------------------------------------------------------------------------------
struct IdAndDistance
{
	///-------------------------------------------------------------------------------------------------
	/// Constructor. 
	///
	/// \param	aGuideId	Identifier for a guide. 
	/// \param	aDistance	The distance to guide's rest position of root. 
	///-------------------------------------------------------------------------------------------------
	IdAndDistance( GuideId aGuideId = 0 , float aDistance = 0 )
	{
		mGuideId = aGuideId;
		mDistance = aDistance;
	}

	GuideId mGuideId;	// Guide identifier

	float mDistance;	// Distance to guide's rest position of root
};

///----------------------------------------------------------------------------------------------------
/// Defines an alias representing list of identifiers and distances for the closest guides .
///----------------------------------------------------------------------------------------------------
typedef std::vector< IdAndDistance > ClosestGuides;

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the float 3D vector .
///-------------------------------------------------------------------------------------------------
typedef Vector3D< float > FloatVector;

///----------------------------------------------------------------------------------------------------
/// Defines a KDTree for finding N closest points for the query point.
///----------------------------------------------------------------------------------------------------
typedef KdTreeTmplPtr< FloatVector, FloatVector > KdTree;

///-------------------------------------------------------------------------------------------------
/// Guides' roots rest positions data structure for closest points queries.
/// Roots of guides of different interpolation groups are stored in separate KD trees,
/// any query is executed only for one requested interpolation group.
/// Enables queries for n closest guides' roots in requested interpolation group from given point.
/// Can export/import rest position roots of guides to/from binary stream, during import KD trees 
/// are rebuild.
/// This class uses float numbers instead of Real, because KD tree structure also uses only float.
///-------------------------------------------------------------------------------------------------
class RestPositionsDS
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	/// Initializes empty uniform grid.
	///-------------------------------------------------------------------------------------------------
	RestPositionsDS();

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	~RestPositionsDS();

	///-------------------------------------------------------------------------------------------------
	/// Builds the internal KD Trees for closest points queries.
	/// Roots of guides of different interpolation groups are stored in separate KD trees. 
	///
	/// \param	aGuidesRestPositions	The guides rest positions. 
	/// \param	aInterpolationGroups	The interpolation groups.
	///-------------------------------------------------------------------------------------------------
	void build( const GuidesRestPositions & aGuidesRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups );

	///-------------------------------------------------------------------------------------------------
	/// Gets the n closest guides from requested position in world coordinates.
	/// Only guides from requested interpolation group are returned. 
	///
	/// \param	aPosition					The requested position in world coordinates. 
	/// \param	aInterpolationGroupId		Identifier for a interpolation group. 
	/// \param	aN							Number of closest guides to return. 
	/// \param [in,out]	aClosestGuides		List of identifiers and distances for a closest guides. 
	///-------------------------------------------------------------------------------------------------
	void getNClosestGuides( const Vector3D< Real > & aPosition, unsigned __int32 aInterpolationGroupId,
		unsigned __int32 aN, ClosestGuides & aClosestGuides ) const;

	///-------------------------------------------------------------------------------------------------
	/// Informs the structure about changes of roots rest pose positions or change of interpolation 
	/// groups.
	/// Dirty bit is not checked by any method of this class in release mode. User of this class is
	/// responsible to check the dirty bit and rebuild the data structure by himself.
	///-------------------------------------------------------------------------------------------------
	inline void setDirty();

	///-------------------------------------------------------------------------------------------------
	/// Query if this object has been set as dirty. 
	///
	/// \return	true if dirty, false if not. 
	///-------------------------------------------------------------------------------------------------
	inline bool isDirty() const;

	///-------------------------------------------------------------------------------------------------
	/// Exports data structure to file.
	/// Only roots rest positions and texture coordinates are exported to binary stream, KD trees will
	/// be rebuild from these during import.
	/// Texture coordinates will be used to distinguish guide's interpolation group. 
	///
	/// \param [in,out]	aOutputStream	The output stream. 
	///-------------------------------------------------------------------------------------------------
	void exportToFile( std::ostream & aOutputStream ) const;

	///-------------------------------------------------------------------------------------------------
	/// Imports data from file and builds the internal data structures.
	/// Only roots rest positions and texture coordinates are imported, KD trees are rebuild afterwards.
	/// Texture coordinates are used to distinguish guide's interpolation group.
	///
	/// \param [in,out]	aInputStream	The input stream. 
	/// \param	aInterpolationGroups	The interpolation groups.
	///-------------------------------------------------------------------------------------------------
	void importFromFile( std::istream & aInputStream,
		const Interpolation::InterpolationGroups & aInterpolationGroups );

private:

	///-------------------------------------------------------------------------------------------------
	/// Builds the KD trees from already stored guides rest positions.
	/// Builds separete tree for every interpolation group. 
	/// 
	/// \param	aInterpolationGroups	The interpolation groups.
	///-------------------------------------------------------------------------------------------------
	void innerBuild( const Interpolation::InterpolationGroups & aInterpolationGroups );

	///-------------------------------------------------------------------------------------------------
	/// Rest position of guide root.
	/// The 3D position in world coordinates is stored and texture coordinates are also kept for
	/// distinguishing guide's interpolation group.
	///-------------------------------------------------------------------------------------------------
	struct Position
	{
		FloatVector mPosition;	///< The position ( in float, kd-tree is also float )

		Real mUCoordinate;  ///< The u texture coordinate

		Real mVCoordinate;  ///< The v texture coordinate
	};
	///----------------------------------------------------------------------------------------------------
	/// Defines an alias representing the array of rest positions of guides' roots.
	///----------------------------------------------------------------------------------------------------
	typedef std::vector< Position > Positions;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing list of identifiers for the interpolation groups .
	///-------------------------------------------------------------------------------------------------
	typedef std::vector< unsigned __int32 > GroupIds;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing the number of guides for each group.
	///-------------------------------------------------------------------------------------------------
	typedef GroupIds GroupCounts;

	bool mDirtyBit; ///< True if user of this structure must call the build method

	Positions mGuidesRestPositions;   ///< The guides' roots rest positions

	KdTree *mKdForest;	///< KDTree forest for closest points query ( separate tree for each interpolation group )

	unsigned int mForestSize;	///< Size of the forrest
};

// inline functions implementation

inline void RestPositionsDS::setDirty()
{
	mDirtyBit = true;
}

inline bool RestPositionsDS::isDirty() const
{
	return mDirtyBit;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_REST_POSITIONS_DS_HPP