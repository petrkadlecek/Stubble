#ifndef STUBBLE_VOXELIZATION_HPP
#define STUBBLE_VOXELIZATION_HPP



#include "Common/CommonTypes.hpp"
#include "HairShape/Generators/UVPointGenerator.hpp"
#include "HairShape/Interpolation/HairGenerator.tmpl.hpp"
#include "HairShape/Interpolation/HairProperties.hpp"
#include "HairShape/Interpolation/SimplePositionGenerator.hpp"
#include "HairShape/Interpolation/SimpleOutputGenerator.hpp"
#include "MayaMesh.hpp"
#include "Mesh.hpp"
#include "Primitives/BoundingBox.hpp"

#include <fstream>
#include <vector>

namespace Stubble
{

namespace HairShape
{

///-------------------------------------------------------------------------------------------------
/// Stores information about how is mesh voxelized.
///-------------------------------------------------------------------------------------------------
class Voxelization
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Constructor. 
	///
	/// \param	aRestPoseMesh		the rest pose mesh. 
	/// \param	aDensityTexture		the density texture. 
	/// \param	aDimensions3		the 3D dimensions of voxelization
	///-------------------------------------------------------------------------------------------------
	Voxelization( const Mesh & aRestPoseMesh, const Texture & aDensityTexture, 
		const Dimensions3 & aDimensions3 );

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	inline ~Voxelization();

	///-------------------------------------------------------------------------------------------------
	/// Updates the voxels. 
	///
	/// \param	aCurrentMesh	The current mesh. 
	/// \param	aHairProperties	The hair properties. 
	/// \param	aTotalHairCount	The total hair count
	///-------------------------------------------------------------------------------------------------
	void updateVoxels( const MayaMesh & aCurrentMesh, const Interpolation::HairProperties & aHairProperties,
		unsigned __int32 aTotalHairCount );

	///-------------------------------------------------------------------------------------------------
	/// Exports voxel. 
	///
	/// \param [in,out]	aOutputStream	The output stream. 
	/// \param	aVoxelId				Identifier for a voxel. 
	/// 								
	/// \return the bounding box of exported voxel
	///-------------------------------------------------------------------------------------------------
	BoundingBox exportVoxel( std::ostream & aOutputStream, unsigned __int32 aVoxelId );

	///-------------------------------------------------------------------------------------------------
	/// Gets a number of guide hair in voxel.
	///
	/// \param	aVoxelId	Voxel id
	///
	/// \return	The voxel hair count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 getVoxelHairCount( unsigned __int32 aVoxelId ) const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the voxels count. 
	///
	/// \return	The voxels count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 getVoxelsCount() const;

private:

	///----------------------------------------------------------------------------------------------------
	/// Defines an alias representing list of identifiers for the triangles .
	///----------------------------------------------------------------------------------------------------
	typedef std::vector< unsigned __int32 > TrianglesIds;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing list of identifiers for the voxels .
	///-------------------------------------------------------------------------------------------------
	typedef TrianglesIds VoxelsIds;

	struct Voxel
	{
		unsigned __int32 mHairCount;	///< Number of hair inside this voxel

		unsigned __int32 mHairIndex;	///< Index of first hair inside this voxel

		TrianglesIds mTrianglesIds; ///< List of identifiers for the triangles

		BoundingBox mBoundingBox;   ///< The bounding box of voxel

		Mesh * mRestPoseMesh; ///< The rest pose mesh

		Mesh * mCurrentMesh;  ///< The current mesh

		UVPointGenerator * mUVPointGenerator;   ///< The uv point generator for current voxel

		RandomGenerator mRandom;	///< The random generator for this voxel
	};

	///----------------------------------------------------------------------------------------------------
	/// Defines an alias representing the voxels .
	///----------------------------------------------------------------------------------------------------
	typedef std::vector< Voxel > Voxels;

	Voxels mVoxels; ///< The voxels

};

// inline methods implementation

inline Voxelization::~Voxelization()
{
	for ( Voxels::iterator it = mVoxels.begin(); it != mVoxels.end(); ++it )
	{
		delete it->mUVPointGenerator;
		delete it->mRestPoseMesh;
		delete it->mCurrentMesh;
	}
}

inline unsigned __int32 Voxelization::getVoxelHairCount( unsigned __int32 aVoxelId ) const
{
	return mVoxels[ aVoxelId ].mHairCount;
}

inline unsigned __int32 Voxelization::getVoxelsCount() const
{
	return static_cast< unsigned __int32 >( mVoxels.size() );
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_VOXELIZATION_HPP