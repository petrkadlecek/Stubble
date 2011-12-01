#ifndef STUBBLE_GUIDE_POSITION_HPP
#define STUBBLE_GUIDE_POSITION_HPP

#include "Common\CommonTypes.hpp"
#include "HairShape\Mesh\MeshPoint.hpp"
#include "HairShape\Mesh\UVPoint.hpp"
#include "Primitives\Matrix.hpp"
#include "Common\CommonConstants.hpp"
#include "Common\CommonFunctions.hpp"

#include <sstream>
#include <string>
#include <vector>

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

///-------------------------------------------------------------------------------------------------
/// Guide current position. 
///-------------------------------------------------------------------------------------------------
struct GuideCurrentPosition
{
	MeshPoint mPosition;	///< The position on current mesh

	Matrix< Real > mWorldTransformMatrix; ///< The world transformation matrix

	Matrix< Real > mLocalTransformMatrix; ///< The local transformation matrix
};

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the guides current positions .
///-------------------------------------------------------------------------------------------------
typedef std::vector< GuideCurrentPosition > GuidesCurrentPositions;


///-------------------------------------------------------------------------------------------------
/// Guide rest position. 
///-------------------------------------------------------------------------------------------------
struct GuideRestPosition
{
	MeshPoint mPosition;	///< The position on rest pose mesh

	UVPoint mUVPoint;   ///< The uv point ( position immutable to transformations )

	///-------------------------------------------------------------------------------------------------
	/// Serialize object.
	/// 
	/// \param	aOutputStream	Output stream
	///-------------------------------------------------------------------------------------------------
	inline void serialize( std::ostream & aOutputStream ) const;

	///-------------------------------------------------------------------------------------------------
	/// Deserialize object.	
	///
	/// \param	aInputStream	Input stream
	///-------------------------------------------------------------------------------------------------
	inline void deserialize( std::istream & aInputStream );
};

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the guides rest positions .
///-------------------------------------------------------------------------------------------------
typedef std::vector< GuideRestPosition > GuidesRestPositions;

// inline function implementations

inline void GuideRestPosition::serialize( std::ostream & aOutputStream ) const
{
	aOutputStream << mPosition;
	aOutputStream << mUVPoint;
}

inline void GuideRestPosition::deserialize( std::istream & aInputStream )
{	
	aInputStream >> mPosition;
	aInputStream >> mUVPoint;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_GUIDE_POSITION_HPP