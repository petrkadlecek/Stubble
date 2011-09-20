#ifndef STUBBLE_RM_POSITION_GENERATOR_HPP
#define STUBBLE_RM_POSITION_GENERATOR_HPP

#include "HairShape/Generators/UVPointGenerator.hpp"
#include "HairShape/Mesh/Mesh.hpp"
#include "PositionGenerator.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// The position generator of interpolated hair used in RenderMan plugin.
///-------------------------------------------------------------------------------------------------
class RMPositionGenerator : public PositionGenerator
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Constructor. Loads mesh data from voxel file and creates UV point generator.
	///
	/// \param	aDensityTexture	The density texture. 
	/// \param	aVoxelFileName	Filename of the voxel file. 
	///-------------------------------------------------------------------------------------------------
	RMPositionGenerator( const Texture & aDensityTexture, const std::string & aVoxelFileName );

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	inline ~RMPositionGenerator();
	
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

private:

	Mesh * mCurrentMesh;	///< The current mesh

	Mesh * mRestPoseMesh;   ///< The rest pose mesh

	UVPointGenerator * mUVPointGenerator;   ///< The uv point generator

	RandomGenerator randomGenerator;	///< The random generator
	
	unsigned __int32 mCount;	///< Number of the interpolated hair.

	unsigned __int32 mStartIndex;   ///< The start index of hair
};

// inline functions implementation

inline RMPositionGenerator::~RMPositionGenerator()
{
	delete mCurrentMesh;
	delete mRestPoseMesh;
	delete mUVPointGenerator;
}

inline void RMPositionGenerator::generate( MeshPoint & aCurrentPosition, MeshPoint & aRestPosition )
{
	UVPoint uv = mUVPointGenerator->next(); // Generate uv pos
	aCurrentPosition = mCurrentMesh->getMeshPoint( uv );
	aRestPosition = mRestPoseMesh->getMeshPoint( uv );
}

inline void RMPositionGenerator::generate( MeshPoint & aCurrentPosition, MeshPoint & aRestPosition,
	const Texture & aDisplacementTexture )
{
	UVPoint uv = mUVPointGenerator->next(); // Generate uv pos
	aCurrentPosition = mCurrentMesh->getDisplacedMeshPoint( uv, aDisplacementTexture );
	aRestPosition = mRestPoseMesh->getMeshPoint( uv );
}

inline unsigned __int32 RMPositionGenerator::getHairCount() const
{
	return mCount;
}

inline unsigned __int32 RMPositionGenerator::getHairStartIndex() const
{
	return mStartIndex;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_RM_POSITION_GENERATOR_HPP