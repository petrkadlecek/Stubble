#ifndef STUBBLE_VOXELIZATION_HPP
#define STUBBLE_VOXELIZATION_HPP



#include "Common\CommonTypes.hpp"
#include "HairShape\Generators\UVPointGenerator.hpp"
#include "MayaMesh.hpp"
#include "Mesh.hpp"
#include "Primitives\BoundingBox.hpp"

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
	/// \param	aUVPointGenerator	the uv point generator. 
	/// \param	aDimensions3		the 3D dimensions of voxelization
	///-------------------------------------------------------------------------------------------------
	Voxelization( const Mesh & aRestPoseMesh, const UVPointGenerator & aUVPointGenerator, 
		const Dimensions3 & aDimensions3 );

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	inline ~Voxelization();

	///-------------------------------------------------------------------------------------------------
	/// Gets a number of guide hair in voxel.
	///
	/// \author	martin_sik
	/// \date	30.8.2011
	///
	/// \param	aTotalHairCount	Number of a total guides in voxel. 
	/// \param	aVoxelId		Voxel id
	///
	/// \return	The voxel hair count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 getVoxelHairCount( unsigned __int32 aTotalHairCount, unsigned __int32 aVoxelId ) const;

	///-------------------------------------------------------------------------------------------------
	/// Queries if a voxel is empty. 
	///
	/// \param	aVoxelId	Identifier for a voxel. 
	///
	/// \return	true if a voxel is empty, false if not. 
	///-------------------------------------------------------------------------------------------------
	inline bool isVoxelEmpty( unsigned __int32 aVoxelId ) const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the voxels count. 
	///
	/// \return	The voxels count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 getVoxelsCount() const;

	///-------------------------------------------------------------------------------------------------
	/// Export current voxel to file. 
	///
	/// \param [in,out]	aOutputStream	a output stream. 
	/// \param	aCurrentMesh			a current mesh. 
	/// \param	aVoxelId				Identifier for a voxel. 
	///
	/// \return	bounding box of current voxel 
	///-------------------------------------------------------------------------------------------------
	BoundingBox exportCurrentVoxel( std::ostream & aOutputStream, const MayaMesh & aCurrentMesh, 
		unsigned __int32 aVoxelId ) const;

	///-------------------------------------------------------------------------------------------------
	/// Export rest pose voxel. 
	///
	/// \param [in,out]	aOutputStream	a output stream. 
	/// \param	aRestPoseMesh			a rest pose mesh. 
	/// \param	aVoxelId				Identifier for a voxel. 
	///-------------------------------------------------------------------------------------------------
	void exportRestPoseVoxel( std::ostream & aOutputStream, const Mesh & aRestPoseMesh, 
		unsigned __int32 aVoxelId ) const;

private:

	///----------------------------------------------------------------------------------------------------
	/// Defines an alias representing list of identifiers for the triangles .
	///----------------------------------------------------------------------------------------------------
	typedef std::vector< unsigned __int32 > TrianglesIds;

	struct Voxel
	{
		Real mDensity;  ///< The density of hair in this voxel

		TrianglesIds mTrianglesIds; ///< List of identifiers for the triangles
	};

	///----------------------------------------------------------------------------------------------------
	/// Defines an alias representing the voxels .
	///----------------------------------------------------------------------------------------------------
	typedef std::vector< Voxel > Voxels;

	Voxels mVoxels;
};

// inline methods implementation

inline Voxelization::~Voxelization()
{
}

inline unsigned __int32 Voxelization::getVoxelHairCount( unsigned __int32 aTotalHairCount, 
	unsigned __int32 aVoxelId ) const
{
	return static_cast< unsigned __int32 >( mVoxels[ aVoxelId ].mDensity * aTotalHairCount );
}

inline bool Voxelization::isVoxelEmpty( unsigned __int32 aVoxelId ) const
{
	return mVoxels[ aVoxelId ].mTrianglesIds.empty();
}

inline unsigned __int32 Voxelization::getVoxelsCount() const
{
	return static_cast< unsigned __int32 >( mVoxels.size() );
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_VOXELIZATION_HPP