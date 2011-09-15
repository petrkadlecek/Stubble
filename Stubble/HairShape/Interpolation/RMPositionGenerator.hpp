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
	inline void generate( MeshPoint & aCurrentPosition, Vector3D< Real > & aRestPosition );
	
	///-------------------------------------------------------------------------------------------------
	/// Generates position of interpolated hair on displaced mesh. 
	///
	/// \param [in,out]	aCurrentPosition	The current position of hair on displaced mesh. 
	/// \param [in,out]	aRestPosition		The rest position of hair in 3D space. 
	/// \param aDisplacementTexture			The texture defining displacement of mesh.
	///-------------------------------------------------------------------------------------------------
	inline void generate( MeshPoint & aCurrentPosition, Vector3D< Real > & aRestPosition,
		const Texture & aDisplacementTexture );

	///-------------------------------------------------------------------------------------------------
	/// Gets the number of the hair to be interpolated.
	///
	/// \return	The hair count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 getHairCount() const;
	
private:

	Mesh * mCurrentMesh;	///< The current mesh

	Mesh * mRestPoseMesh;   ///< The rest pose mesh

	UVPointGenerator * mUVPointGenerator;   ///< The uv point generator

	RandomGenerator randomGenerator;	///< The random generator
	
	unsigned __int32 mCount;	///< Number of the interpolated hair.
};

// inline functions implementation

inline RMPositionGenerator::~RMPositionGenerator()
{
	delete mCurrentMesh;
	delete mRestPoseMesh;
	delete mUVPointGenerator;
}

inline void RMPositionGenerator::generate( MeshPoint & aCurrentPosition, Vector3D< Real > & aRestPosition )
{
	UVPoint uv = mUVPointGenerator->next(); // Generate uv pos
	aCurrentPosition = mCurrentMesh->getMeshPoint( uv );
	aRestPosition = mRestPoseMesh->getPosition( uv );
}

inline void RMPositionGenerator::generate( MeshPoint & aCurrentPosition, Vector3D< Real > & aRestPosition,
	const Texture & aDisplacementTexture )
{
	UVPoint uv = mUVPointGenerator->next(); // Generate uv pos
	aCurrentPosition = mCurrentMesh->getDisplacedMeshPoint( uv, aDisplacementTexture );
	aRestPosition = mRestPoseMesh->getPosition( uv );
}

inline unsigned __int32 RMPositionGenerator::getHairCount() const
{
	return mCount;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_RM_POSITION_GENERATOR_HPP