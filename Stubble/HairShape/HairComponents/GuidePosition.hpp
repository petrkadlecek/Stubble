#ifndef STUBBLE_GUIDE_POSITION_HPP
#define STUBBLE_GUIDE_POSITION_HPP

#include "Common\CommonTypes.hpp"
#include "HairShape\Mesh\MeshPoint.hpp"
#include "HairShape\Mesh\UVPoint.hpp"
#include "Primitives\Matrix.hpp"

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
};

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the guides rest positions .
///-------------------------------------------------------------------------------------------------
typedef std::vector< GuideRestPosition > GuidesRestPositions;

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_GUIDE_POSITION_HPP