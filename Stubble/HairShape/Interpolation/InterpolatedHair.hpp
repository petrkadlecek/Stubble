#ifndef STUBBLE_INTERPOLATED_HAIR_HPP
#define STUBBLE_INTERPOLATED_HAIR_HPP

#include "HairShape/Generators/UVPointGenerator.hpp"

#include "HairGenerator.tmpl.hpp"
#include "HairProperties.hpp"
#include "MayaOutputGenerator.hpp"
#include "MayaPositionGenerator.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Class for generating and displaying interpolated hair inside Maya
///-------------------------------------------------------------------------------------------------
class InterpolatedHair
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	inline InterpolatedHair();

	///-------------------------------------------------------------------------------------------------
	/// Generates interpolated hair. 
	///
	/// \param [in,out]	aUVPointGenerator	The uv point generator. 
	/// \param	aCurrentMesh				The current mesh. 
	/// \param	aRestPoseMesh				The rest pose mesh. 
	/// \param	aHairProperties				The hair properties. 
	/// \param	aCount						Number of the hair. 
	///-------------------------------------------------------------------------------------------------
	inline void generate( UVPointGenerator & aUVPointGenerator, const MayaMesh & aCurrentMesh,
		const Mesh & aRestPoseMesh, const HairProperties & aHairProperties, unsigned __int32 aCount );

	///-------------------------------------------------------------------------------------------------
	/// Updates hair positions and local space due to mesh update. This method can not handle 
	/// mesh topology update (generate should be used instead).
	///
	/// \param	aCurrentMesh	The current mesh. 
	/// \param	aHairProperties	The hair properties. 
	///-------------------------------------------------------------------------------------------------
	inline void meshUpdate( const MayaMesh & aCurrentMesh, const HairProperties & aHairProperties );

	///-------------------------------------------------------------------------------------------------
	/// Updates hair properties.
	///
	/// \param	aHairProperties	The hair properties. 
	///-------------------------------------------------------------------------------------------------
	inline void propertiesUpdate( const HairProperties & aHairProperties );

	///-------------------------------------------------------------------------------------------------
	/// Draws interpolated hair. 
	///-------------------------------------------------------------------------------------------------
	inline void draw();

	///-------------------------------------------------------------------------------------------------
	/// Gets the bounding box of displayed hair. 
	///
	/// \return	The bounding box. 
	///-------------------------------------------------------------------------------------------------
	inline const BoundingBox & getBoundingBox();

private:

	MayaPositionGenerator mPositionGenerator;   ///< The hair position generator

	MayaOutputGenerator mOutputGenerator;   ///< The output hair generator

	HairGenerator< MayaPositionGenerator, MayaOutputGenerator > mHairGenerator; ///< The hair generator

};

// inline functions implementation

inline InterpolatedHair::InterpolatedHair():
	mHairGenerator( mPositionGenerator, mOutputGenerator )
{
}

inline void InterpolatedHair::generate( UVPointGenerator & aUVPointGenerator, const MayaMesh & aCurrentMesh,
	const Mesh & aRestPoseMesh, const HairProperties & aHairProperties, unsigned __int32 aCount )
{
	mPositionGenerator.preGenerate( aUVPointGenerator, aCurrentMesh, aRestPoseMesh, aCount );
	mHairGenerator.generate( aHairProperties );
}

inline void InterpolatedHair::meshUpdate( const MayaMesh & aCurrentMesh, const HairProperties & aHairProperties )
{
	// Get hair strand count
	unsigned __int32 count = aHairProperties.getMultiStrandCount();
	count = count == 0 ? 1 : count;
	// Move all hair points to local space
	mOutputGenerator.recalculateToLocalSpace( mPositionGenerator.getPreGeneratedPositions(), count );
	// Alter hair points local space
	mPositionGenerator.recalculateCurrentPositions( aCurrentMesh );
	// Move all hair points to world space
	mOutputGenerator.recalculateToWorldSpace( mPositionGenerator.getPreGeneratedPositions(), count );
}

inline void InterpolatedHair::propertiesUpdate( const HairProperties & aHairProperties )
{
	mPositionGenerator.reset();
	mHairGenerator.generate( aHairProperties );
}

inline void InterpolatedHair::draw()
{
	mOutputGenerator.draw();
}

inline const BoundingBox & InterpolatedHair::getBoundingBox()
{
	return mOutputGenerator.getBoundingBox();
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_INTERPOLATED_HAIR_HPP