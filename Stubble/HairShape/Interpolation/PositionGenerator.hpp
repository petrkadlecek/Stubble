#ifndef STUBBLE_POSITION_GENERATOR_HPP
#define STUBBLE_POSITION_GENERATOR_HPP

#include "Common/CommonTypes.hpp"
#include "Common/StubbleException.hpp"
#include "HairShape/Mesh/MeshPoint.hpp"
#include "HairShape/Texture/Texture.hpp"
#include "Primitives/Vector3D.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Interface of position generator of interpolated hair. No virtual functions are used, this class
/// here only represents the interface for classes used as HairGenerator template argument.
///-------------------------------------------------------------------------------------------------
class PositionGenerator
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Generates position of interpolated hair. 
	///
	/// \param [in,out]	aCurrentPosition	The current position of hair on mesh. 
	/// \param [in,out]	aRestPosition		The rest position of hair in 3D space. 
	///-------------------------------------------------------------------------------------------------
	inline void generate( MeshPoint & aCurrentPosition, MeshPoint & aRestPosition );
	
	///-------------------------------------------------------------------------------------------------
	/// Generates position of interpolated hair on displaced mesh. 
	///
	/// \param [in,out]	aCurrentPosition	The current position of hair on displaced mesh. 
	/// \param [in,out]	aRestPosition		The rest position of hair in 3D space. 
	/// \param aDisplacementTexture			The texture defining displacement of mesh.
	///-------------------------------------------------------------------------------------------------
	inline void generate( MeshPoint & aCurrentPosition, MeshPoint & aRestPosition,
		const Texture & aDisplacementTexture );

	///-------------------------------------------------------------------------------------------------
	/// Gets the number of the hair to be interpolated.
	///
	/// \return	The hair count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 getHairCount() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the hair start index. 
	///
	/// \return	The hair start index. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 getHairStartIndex() const;

protected:
	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	inline PositionGenerator();
};

// inline functions implementation

inline void PositionGenerator::generate( MeshPoint & aCurrentPosition, MeshPoint & aRestPosition )
{
	throw StubbleException( "PositionGenerator::generate : this method is not implemented !" ); 
}

inline void PositionGenerator::generate( MeshPoint & aCurrentPosition, MeshPoint & aRestPosition,
	const Texture & aDisplacementTexture )
{
	throw StubbleException( "PositionGenerator::generate : this method is not implemented !" );
}

inline unsigned __int32 PositionGenerator::getHairCount() const
{
	throw StubbleException( "PositionGenerator::getHairCount : this method is not implemented !" );
}

inline unsigned __int32 PositionGenerator::getHairStartIndex() const
{
	throw StubbleException( "PositionGenerator::getHairStartIndex : this method is not implemented !" );
}

inline PositionGenerator::PositionGenerator()
{
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_POSITION_GENERATOR_HPP