#ifndef STUBBLE_VOXELIZATION_HPP
#define STUBBLE_VOXELIZATION_HPP

#include "Common\CommonTypes.hpp"
#include "HairShape\Mesh\Mesh.hpp"
#include "HairShape\Generators\UVPointGenerator.hpp"
#include "Primitives\BoundingBox.hpp"

#include <fstream>

namespace Stubble
{

namespace HairShape
{

namespace RibExport
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
	~Voxelization();

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
	inline unsigned int getVoxelHairCount( unsigned int aTotalHairCount, unsigned int aVoxelId ) const;

	///-------------------------------------------------------------------------------------------------
	/// Queries if a voxel is empty. 
	///
	/// \param	aVoxelId	Identifier for a voxel. 
	///
	/// \return	true if a voxel is empty, false if not. 
	///-------------------------------------------------------------------------------------------------
	inline bool isVoxelEmpty( unsigned int aVoxelId ) const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the voxels count. 
	///
	/// \return	The voxels count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned int getVoxelsCount() const;

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
		unsigned int aVoxelId ) const;

	///-------------------------------------------------------------------------------------------------
	/// Export rest pose voxel. 
	///
	/// \param [in,out]	aOutputStream	a output stream. 
	/// \param	aRestPoseMesh			a rest pose mesh. 
	/// \param	aVoxelId				Identifier for a voxel. 
	///-------------------------------------------------------------------------------------------------
	void exportRestPoseVoxel( std::ostream & aOutputStream, const Mesh & aRestPoseMesh, unsigned int aVoxelId ) const;

private:

};

// inline methods implementation

inline unsigned int Voxelization::getVoxelHairCount( unsigned int aTotalHairCount, unsigned int aVoxelId ) const
{
}

inline bool Voxelization::isVoxelEmpty( unsigned int aVoxelId ) const
{
}

inline unsigned int Voxelization::getVoxelsCount() const
{
}

} // namespace RibExport

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MAYA_MESH_HPP