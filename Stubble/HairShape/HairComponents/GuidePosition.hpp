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
/// Holds position of the guide hair root on current mesh and associated local coordinate system.
/// Transformation matrices from/to local coordinate system from/to world coordinate system are 
/// also stored.
///-------------------------------------------------------------------------------------------------
struct GuideCurrentPosition
{
	MeshPoint mPosition;	///< The position on current mesh

	Matrix< Real > mWorldTransformMatrix; ///< The world transformation matrix

	Matrix< Real > mLocalTransformMatrix; ///< The local transformation matrix
};

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the guides current positions array .
///-------------------------------------------------------------------------------------------------
typedef std::vector< GuideCurrentPosition > GuidesCurrentPositions;


///-------------------------------------------------------------------------------------------------
/// Guide rest position.
/// Holds position of the guide hair root on rest pose mesh and associated local coordinate system.
/// Also stores position of the guide hair root on rest pose mesh in uv point format, which is 
/// immutable to any mesh deformations and transformations.
/// This structure can be serialized/deserialized. 
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
/// Defines an alias representing the guides rest positions array.
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