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

		Vector3D< Real > mRestPosition; ///< The rest position of the hair

		UVPoint mUVPoint;   ///< The uv point, stored for recalculation of current position
	};

	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	inline MayaPositionGenerator();

	///-------------------------------------------------------------------------------------------------
	/// Generates hair positions for later use.
	///
	/// \param [in,out]	aUVPointGenerator	The uv point generator. 
	/// \param	aCurrentMesh				The current mesh. 
	/// \param	aRestPoseMesh				The rest pose mesh. 
	/// \param	aCount						Number of the interpolated hair. 
	///-------------------------------------------------------------------------------------------------
	void preGenerate( UVPointGenerator & aUVPointGenerator, const MayaMesh & aCurrentMesh,
		const Mesh & aRestPoseMesh, unsigned __int32 aCount );

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	inline ~MayaPositionGenerator();
	
	///-------------------------------------------------------------------------------------------------
	/// Generates position of interpolated hair. Pre-generate must be called first.
	///
	/// \param [in,out]	aCurrentPosition	The current position of hair on mesh. 
	/// \param [in,out]	aRestPosition		The rest position of hair in 3D space. 
	///-------------------------------------------------------------------------------------------------
	inline void generate( MeshPoint & aCurrentPosition, Vector3D< Real > & aRestPosition );
	
	///-------------------------------------------------------------------------------------------------
	/// Generates position of interpolated hair on displaced mesh. Pre-generate must be called first.
	///
	/// \param [in,out]	aCurrentPosition	The current position of hair on displaced mesh. 
	/// \param [in,out]	aRestPosition		The rest position of hair in 3D space. 
	/// \param aDisplacementTexture			The texture defining displacement of mesh.
	///-------------------------------------------------------------------------------------------------
	inline void generate( MeshPoint & aCurrentPosition, Vector3D< Real > & aRestPosition,
		const Texture & aDisplacementTexture );

	///-------------------------------------------------------------------------------------------------
	/// Gets the number of the hair to be interpolated. Pre-generate must be called first.
	///
	/// \return	The hair count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 getHairCount() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the pre-generated positions in raw format. 
	///
	/// \return	null if it fails, else the pre-generated positions. 
	///-------------------------------------------------------------------------------------------------
	inline const GeneratedPosition * getPreGeneratedPositions() const;

	///-------------------------------------------------------------------------------------------------
	/// Recalculates current positions. 
	///
	/// \param	aCurrentMesh	The current mesh. 
	///-------------------------------------------------------------------------------------------------
	void recalculateCurrentPositions( const MayaMesh & aCurrentMesh );

private:

	GeneratedPosition * mGeneratedPositions;	///< The generated positions

	GeneratedPosition * mCurrentPosition;   ///< The current position, that will be returned
	
	unsigned __int32 mCount;	///< Number of the interpolated hair.
};

// inline functions implementation

inline MayaPositionGenerator::MayaPositionGenerator():
	mGeneratedPositions( 0 ),
	mCount( 0 )
{
}

inline MayaPositionGenerator::~MayaPositionGenerator()
{
	delete [] mGeneratedPositions;
}

inline void MayaPositionGenerator::generate( MeshPoint & aCurrentPosition, Vector3D< Real > & aRestPosition )
{
	aCurrentPosition = mCurrentPosition->mCurrentPosition;
	aRestPosition = mCurrentPosition->mRestPosition;
	++mCurrentPosition; // Move to next generated position
}

inline void MayaPositionGenerator::generate( MeshPoint & aCurrentPosition, Vector3D< Real > & aRestPosition,
	const Texture & aDisplacementTexture )
{
	generate( aCurrentPosition, aRestPosition ); // Displacement will not be used
}

inline unsigned __int32 MayaPositionGenerator::getHairCount() const
{
	return mCount;
}

inline const MayaPositionGenerator::GeneratedPosition * MayaPositionGenerator::getPreGeneratedPositions() const
{
	return mGeneratedPositions;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MAYA_POSITION_GENERATOR_HPP