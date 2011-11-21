#ifndef STUBBLE_MAYA_MESH_HPP
#define STUBBLE_MAYA_MESH_HPP

#include <maya\MFnMesh.h>
#include <maya\MObject.h>
#include <maya\MString.h>
#include <maya\MMeshIntersector.h>

#include "HairShape\Mesh\Mesh.hpp"
#include "HairShape\Mesh\MeshUG.hpp"
#include "HairShape\Mesh\MeshUVCoordUG.hpp"
#include "HairShape\Mesh\MeshTriangle.hpp"
#include "HairShape\Mesh\UVPoint.hpp"
#include "Common\CommonConstants.hpp"
#include "Common\CommonFunctions.hpp"

#include <sstream>
#include <string>
#include <fstream>

namespace Stubble
{

namespace HairShape
{
///----------------------------------------------------------------------------------------------------
/// Stores proxy to current Maya mesh and rest pose mesh.
///----------------------------------------------------------------------------------------------------
class MayaMesh
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Maya mesh constructor that accepts Maya mesh object and UV set
	///
	/// \param aMesh	Maya mesh object.
	/// \param aUVSet	UVSet string.
	///----------------------------------------------------------------------------------------------------
	MayaMesh( MObject & aMesh, const MString & aUVSet );

	///----------------------------------------------------------------------------------------------------
	/// Gets acceleration structure for the current mesh.
	/// 
	/// \return acceleration structure
	inline MMeshIntersector * getMeshIntersector() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets current Maya mesh
	/// 
	/// \return Maya mesh
	inline MFnMesh * getMayaMesh() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets rest position of mesh.
	/// 
	/// \return rest position of mesh
	inline const Mesh & getRestPose() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets point on mesh interpolated from 3 vertices of given triangle
	/// 
	/// \param aPoint	The triangle id and barycentric coordinates
	///----------------------------------------------------------------------------------------------------
	MeshPoint getMeshPoint( const UVPoint &aPoint ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets triangle as 3 vertices.
	///----------------------------------------------------------------------------------------------------
	const Triangle getTriangle( unsigned __int32 aID ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets number of mesh's triangles.
	///----------------------------------------------------------------------------------------------------
	unsigned __int32 getTriangleCount() const;

	///----------------------------------------------------------------------------------------------------
	/// Method called in case of updated mesh.
	///
	/// \param aUpdatedMesh	updated mesh
	/// \param aUVSet		UVSet string
	///----------------------------------------------------------------------------------------------------
	void meshUpdate( MObject & aUpdatedMesh, const MString & aUVSet );

	///-------------------------------------------------------------------------------------------------
	/// Gets the mesh uniform grid. 
	///
	/// \return	The mesh uniform grid. 
	///-------------------------------------------------------------------------------------------------
	inline const MeshUG & getMeshUG() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the mesh uv coordinate uniform grid. 
	///
	/// \return	The mesh uv coordinate uniform grid. 
	///-------------------------------------------------------------------------------------------------
	inline const MeshUVCoordUG & getMeshUVCoordUG() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the selected triangles. 
	///
	/// \param	aTrianglesIds	List of identifiers for a triangles. 
	/// \param [in,out]	aResult	The selected triangles. 
	///-------------------------------------------------------------------------------------------------
	inline void getSelectedTriangles( const TrianglesIds aTrianglesIds, Triangles & aResult ) const;

	///-------------------------------------------------------------------------------------------------
	/// Serialize object.
	///-------------------------------------------------------------------------------------------------
	inline std::string serialize() const;

	///-------------------------------------------------------------------------------------------------
	/// Deserialize object.	
	///
	/// \param	aStr	String from which to read.
	/// \param	aPos	Position at which to start.
	///-------------------------------------------------------------------------------------------------
	inline size_t deserialize( const std::string &aStr, size_t aPos );
	
	///----------------------------------------------------------------------------------------------------
	/// Finalizer
	///----------------------------------------------------------------------------------------------------
	~MayaMesh();

private:

	///-------------------------------------------------------------------------------------------------
	/// Gets the triangles. 
	///
	/// \param [in,out]	aResult	a returned triangles. 
	///-------------------------------------------------------------------------------------------------
	void getTriangles( Triangles & aResult ) const;

	MeshTriangles mMeshTriangles; ///< Triangles of mesh stored only as indices to rest pose

	Mesh mRestPose; ///< Rest pose mesh
	
	MFnMesh * mRestPoseMesh; ///< Rest pose maya mesh

	MMeshIntersector * mMeshIntersector; ///< Acceleration structure for finding closest point on the mesh

	MFnMesh * mUpdatedMesh; ///< The updated mesh
	
	MString mUVSet; ///< UV Set name

	mutable MeshUG mMeshUG; ///< The mesh uniform grid

	mutable MeshUVCoordUG mMeshUVCoordUG;   ///< The mesh uv coordinate ug
};

// inline methods implementation

inline const Mesh & MayaMesh::getRestPose() const
{
	return mRestPose;
}

inline MMeshIntersector * MayaMesh::getMeshIntersector() const
{
	return mMeshIntersector;
}

inline MFnMesh * MayaMesh::getMayaMesh() const
{
	return mUpdatedMesh ? mUpdatedMesh : mRestPoseMesh;
}

inline const MeshUG & MayaMesh::getMeshUG() const
{
	if ( mMeshUG.isDirty() )
	{
		Triangles triangles;
		getTriangles( triangles ); // Get triangles from maya
		mMeshUG.build( triangles );
	}
	return mMeshUG;
}

inline const MeshUVCoordUG & MayaMesh::getMeshUVCoordUG() const
{
	if ( mMeshUVCoordUG.isDirty() )
	{
		Triangles triangles;
		getTriangles( triangles ); // Get triangles from maya
		mMeshUVCoordUG.build( triangles );
	}
	return mMeshUVCoordUG;
}

inline void MayaMesh::getSelectedTriangles( const TrianglesIds aTrianglesIds, Triangles & aResult ) const
{
	aResult.resize( aTrianglesIds.size() );
	Triangles::iterator outIt = aResult.begin();
	for ( TrianglesIds::const_iterator idIt = aTrianglesIds.begin(); idIt != aTrianglesIds.end();
		++idIt, ++outIt )
	{
		*outIt = getTriangle( *idIt );
	}
}

inline std::string MayaMesh::serialize() const
{
	std::ostringstream oss;
	oss << Stubble::serializeObjects< MeshTriangle >( mMeshTriangles )
		<< mRestPose.serialize();		
	return oss.str();
}

inline size_t MayaMesh::deserialize( const std::string &aStr, size_t aPos )
{
	mMeshTriangles = Stubble::deserializeObjects< MeshTriangle >( aStr, aPos );
	aPos = mRestPose.deserialize( aStr, aPos );
	return aPos;
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MAYA_MESH_HPP