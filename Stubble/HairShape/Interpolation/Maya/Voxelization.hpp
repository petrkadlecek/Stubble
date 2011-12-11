#ifndef STUBBLE_VOXELIZATION_HPP
#define STUBBLE_VOXELIZATION_HPP

#include "Common/CommonTypes.hpp"
#include "HairShape/Generators/UVPointGenerator.hpp"
#include "../HairGenerator.tmpl.hpp"
#include "../HairProperties.hpp"
#include "SimplePositionGenerator.hpp"
#include "SimpleOutputGenerator.hpp"
#include "HairShape/Mesh/MayaMesh.hpp"
#include "HairShape/Mesh/Mesh.hpp"
#include "Primitives/BoundingBox.hpp"

#include <fstream>
#include <vector>

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

namespace Maya
{

///-------------------------------------------------------------------------------------------------
/// Class for dividing hair object to several voxels which will be rendered independently.
/// Voxels are cells of 3D uniform grid defined by rest position mesh bounding box and user defined
/// resolution.
/// Each voxel contain its own rest pose and current geometry and each hair belong only to one
/// voxel.
/// Voxel can be exported to binary stream and then used in 3Delight or other renderer.
///-------------------------------------------------------------------------------------------------
class Voxelization
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Constructor. 
	/// Voxelizes and stores rest pose mesh to several voxels ( voxels count is defined by aResolution 
	/// parameter, triangle is put in voxel depending on his barycentr position ). Constructs samples 
	/// generator for each voxel of mesh and stores them.
	///
	/// \param	aRestPoseMesh		The rest pose mesh. 
	/// \param	aDensityTexture		The hair density texture. 
	/// \param	aResolution		The 3D resolution of voxelization
	///-------------------------------------------------------------------------------------------------
	Voxelization( const Mesh & aRestPoseMesh, const Texture & aDensityTexture, 
		const Dimensions3 & aResolution );

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	inline ~Voxelization();

	///-------------------------------------------------------------------------------------------------
	/// Updates voxel data and properties.
	/// Voxelizes current mesh and calculates bounding box of hair curves and hair count for each voxel
	/// ( samples generators total densities are used for hair count calculation ). Bounding box is 
	/// calculated by complete generation of hair geometry.
	///
	/// \param	aCurrentMesh	The current mesh. 
	/// \param	aHairProperties	The hair properties. 
	/// \param	aTotalHairCount	The total hair count
	///-------------------------------------------------------------------------------------------------
	void updateVoxels( const MayaMesh & aCurrentMesh, const Interpolation::HairProperties & aHairProperties,
		unsigned __int32 aTotalHairCount );

	///-------------------------------------------------------------------------------------------------
	/// Exports requested voxel data to binary stream.
	/// Hair count, hair start index, current and rest pose mesh of requested voxel are exported.
	/// 
	/// \param [in,out]	aOutputStream	The output stream. 
	/// \param	aVoxelId				Requested voxel identifier.
	/// 
	/// \return the bounding box of exported voxel
	///-------------------------------------------------------------------------------------------------
	BoundingBox exportVoxel( std::ostream & aOutputStream, unsigned __int32 aVoxelId );

	///-------------------------------------------------------------------------------------------------
	/// Gets a number of hair in requested voxel.
	///
	/// \param	aVoxelId	Requested voxel identifier.
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
	/// Defines an alias representing list of identifiers for the voxels array.
	///-------------------------------------------------------------------------------------------------
	typedef TrianglesIds VoxelsIds;

	///-------------------------------------------------------------------------------------------------
	/// Class for holding one voxel data and properties. 
	///-------------------------------------------------------------------------------------------------
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
	/// Defines an alias representing the voxels array .
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

} // namespace Maya

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_VOXELIZATION_HPP