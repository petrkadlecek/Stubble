#ifndef STUBBLE_MAYA_MESH_HPP
#define STUBBLE_MAYA_MESH_HPP

#include <maya\MFnMesh.h>
#include <maya\MObject.h>
#include <maya\MString.h>

#include "HairShape\Mesh\Mesh.hpp"
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
	MayaMesh(const MObject & aMesh, const MString & aUVSet);

	///----------------------------------------------------------------------------------------------------
	/// Gets rest position of mesh.
	/// 
	/// \return rest position of mesh
	const Mesh getRestPose() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets point on mesh interpolated from 3 vertices of given triangle
	///----------------------------------------------------------------------------------------------------
	MeshPoint getMeshPoint(const UVPoint &aPoint) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets triangle as 3 vertices.
	///----------------------------------------------------------------------------------------------------
	const Triangle getTriangle(int aID) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets number of mesh's triangles.
	///----------------------------------------------------------------------------------------------------
	unsigned int getTriangleCount() const;

	///----------------------------------------------------------------------------------------------------
	/// Method called in case of updated mesh.
	///
	/// \param aUpdatedMesh	updated mesh
	/// \param aUVSet		UVSet string
	///----------------------------------------------------------------------------------------------------
	void meshUpdate(MObject & aUpdatedMesh, const MString * aUVSet = 0);

	// TODO - add
	// const UniformGrid & getMeshUG();

	///----------------------------------------------------------------------------------------------------
	/// Finalizer
	///----------------------------------------------------------------------------------------------------
	~MayaMesh();

private:
	MeshTriangles mMeshTriangles; ///< Triangles of mesh stored only as indices to rest pose

	Mesh mRestPose; ///< Rest pose mesh

	MFnMesh * mUpdatedMesh; ///< The updated mesh
	
	const MString * mUVSet; ///< UV Set name
};

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MAYA_MESH_HPP