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
	///-------------------------------------------------------------------------------------------------
	inline std::string serialize() const;

	///-------------------------------------------------------------------------------------------------
	/// Deserialize object.	
	///
	/// \param	aStr	String from which to read.
	/// \param	aPos	Position at which to start.
	///-------------------------------------------------------------------------------------------------
	inline size_t deserialize( const std::string &aStr, size_t aPos );
};

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the guides rest positions .
///-------------------------------------------------------------------------------------------------
typedef std::vector< GuideRestPosition > GuidesRestPositions;

// inline function implementations

inline std::string GuideRestPosition::serialize() const
{
	std::ostringstream oss;		
	oss << mPosition.serialize()
		<< mUVPoint.serialize();
	return oss.str();
}

inline size_t GuideRestPosition::deserialize( const std::string &aStr, size_t aPos )
{		
	aPos = mPosition.deserialize( aStr, aPos );
	aPos = mUVPoint.deserialize( aStr, aPos );
	return aPos;	
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_GUIDE_POSITION_HPP