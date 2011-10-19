#ifndef STUBBLE_MAYA_POSITION_GENERATOR_HPP
#define STUBBLE_MAYA_POSITION_GENERATOR_HPP

#include "HairShape/Generators/UVPointGenerator.hpp"
#include "HairShape/Mesh/MayaMesh.hpp"
#include "HairShape/Mesh/MeshPoint.hpp"
#include "HairShape/Mesh/Mesh.hpp"
#include "PositionGenerator.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// The position generator of interpolated hair used in Maya plugin.
///-------------------------------------------------------------------------------------------------
class MayaPositionGenerator : public PositionGenerator
{
public:
	
	///-------------------------------------------------------------------------------------------------
	/// Generated position. 
	///-------------------------------------------------------------------------------------------------
	struct GeneratedPosition
	{
		MeshPoint mCurrentPosition; ///< The current position of the hair

		MeshPoint mRestPosition; ///< The rest position of the hair

		UVPoint mUVPoint;   ///< The uv point, stored for recalculation of current position
	};

	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	inline MayaPositionGenerator();

	///-------------------------------------------------------------------------------------------------
	/// Sets the internal variables of maya positions generator. 
	///
	/// \param [in,out]	mGeneratedPositions	If non-null, the generated positions. 
	/// \param	aCount						Number of hair. 
	/// \param	aHairIndex					Zero-based index of a hair. 
	///-------------------------------------------------------------------------------------------------
	inline void set( GeneratedPosition * mGeneratedPositions, unsigned __int32 aCount, 
		unsigned __int32 aHairIndex );

	///-------------------------------------------------------------------------------------------------
	/// Generates position of interpolated hair. Pre-generate must be called first.
	///
	/// \param [in,out]	aCurrentPosition	The current position of hair on mesh. 
	/// \param [in,out]	aRestPosition		The rest position of hair in 3D space. 
	///-------------------------------------------------------------------------------------------------
	inline void generate( MeshPoint & aCurrentPosition, MeshPoint & aRestPosition );
	
	///-------------------------------------------------------------------------------------------------
	/// Generates position of interpolated hair on displaced mesh. Pre-generate must be called first.
	///
	/// \param [in,out]	aCurrentPosition	The current position of hair on displaced mesh. 
	/// \param [in,out]	aRestPosition		The rest position of hair in 3D space. 
	/// \param aDisplacementTexture			The texture defining displacement of mesh.
	///	\param aDisplacementFactor			The displacement texture will be mutliplied by this factor.
	///-------------------------------------------------------------------------------------------------
	inline void generate( MeshPoint & aCurrentPosition, MeshPoint & aRestPosition,
		const Texture & aDisplacementTexture, Real aDisplacementFactor );

	///-------------------------------------------------------------------------------------------------
	/// Gets the number of the hair to be interpolated. Pre-generate must be called first.
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

	///-------------------------------------------------------------------------------------------------
	/// Resets returning generated values.
	///-------------------------------------------------------------------------------------------------
	inline void reset();
private:

	GeneratedPosition * mGeneratedPositions;	///< The generated positions

	GeneratedPosition * mCurrentPosition;   ///< The current position, that will be returned
	
	unsigned __int32 mCount;	///< Number of the interpolated hair.

	unsigned __int32 mHairIndex;	///< Zero-based index of a hair
};

// inline functions implementation

inline MayaPositionGenerator::MayaPositionGenerator():
	mGeneratedPositions( 0 ),
	mCount( 0 )
{
}

inline void MayaPositionGenerator::set( GeneratedPosition * aGeneratedPositions, unsigned __int32 aCount, 
	unsigned __int32 aHairIndex )
{
	mGeneratedPositions = aGeneratedPositions;
	mCurrentPosition = mGeneratedPositions;
	mCount = aCount;
	mHairIndex = aHairIndex;
}

inline void MayaPositionGenerator::generate( MeshPoint & aCurrentPosition, MeshPoint & aRestPosition )
{
	aCurrentPosition = mCurrentPosition->mCurrentPosition;
	aRestPosition = mCurrentPosition->mRestPosition;
	++mCurrentPosition; // Move to next generated position
}

inline void MayaPositionGenerator::generate( MeshPoint & aCurrentPosition, MeshPoint & aRestPosition,
	const Texture & aDisplacementTexture, Real aDisplacementFactor )
{
	generate( aCurrentPosition, aRestPosition ); // Displacement will not be used
}

inline unsigned __int32 MayaPositionGenerator::getHairCount() const
{
	return mCount;
}

inline unsigned __int32 MayaPositionGenerator::getHairStartIndex() const
{
	return static_cast< unsigned __int32 >( mCurrentPosition - mGeneratedPositions ) + mHairIndex;
}

inline void MayaPositionGenerator::reset()
{
	mCurrentPosition = mGeneratedPositions;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MAYA_POSITION_GENERATOR_HPP