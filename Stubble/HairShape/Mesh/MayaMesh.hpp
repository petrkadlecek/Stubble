#ifndef STUBBLE_MAYA_MESH_HPP
#define STUBBLE_MAYA_MESH_HPP

#include <maya\MFnMesh.h>
#include <maya\MObject.h>
#include <maya\MString.h>

#include "HairShape\Mesh\Mesh.hpp"
#include "HairShape\Mesh\MeshUG.hpp"
#include "HairShape\Mesh\MeshUVCoordUG.hpp"
#include "HairShape\Mesh\MeshTriangle.hpp"
#include "HairShape\Mesh\UVPoint.hpp"

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
	MayaMesh( const MObject & aMesh, const MString & aUVSet );

	///----------------------------------------------------------------------------------------------------
	/// Gets rest position of mesh.
	/// 
	/// \return rest position of mesh
	const Mesh getRestPose() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets point on mesh interpolated from 3 vertices of given triangle
	/// 
	/// \param aPoint	the triangle id and barycentric coordinates
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
	void meshUpdate( MObject & aUpdatedMesh, const MString * aUVSet = 0 );

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

	MFnMesh * mUpdatedMesh; ///< The updated mesh
	
	const MString * mUVSet; ///< UV Set name

	mutable MeshUG mMeshUG; ///< The mesh uniform grid

	mutable MeshUVCoordUG mMeshUVCoordUG;   ///< The mesh uv coordinate ug
};

// inline methods implementation

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

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MAYA_MESH_HPP