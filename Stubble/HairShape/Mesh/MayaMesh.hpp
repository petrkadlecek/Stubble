#ifndef STUBBLE_MAYA_MESH_HPP
#define STUBBLE_MAYA_MESH_HPP

#include <maya\MFnMesh.h>
#include <maya\MObject.h>
#include <maya\MString.h>
#include <maya\MMeshIntersector.h>

#include "HairShape\Mesh\Mesh.hpp"
#include "HairShape\Mesh\MeshUVCoordUG.hpp"
#include "HairShape\Mesh\MayaTriangle.hpp"
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
/// Works as proxy to current Maya mesh and stores rest pose mesh as Mesh class object.
/// This class also provides access to maya internal objects used for intersections and other 
/// operations.
/// Uniform grid of uv coordinates of the current mesh is stored inside this class.
/// Enables calculation of point on mesh ( MeshPoint = position, normal, tangent etc. )
/// Object critical data can be serialized.
///----------------------------------------------------------------------------------------------------
class MayaMesh
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Maya mesh constructor that accepts Maya mesh object and UV set.
	/// Rest pose mesh will be constructed from current Maya mesh.
	///
	/// \param aMesh	Maya mesh object.
	/// \param aUVSet	UVSet name.
	///----------------------------------------------------------------------------------------------------
	MayaMesh( MObject & aMesh, const MString & aUVSet );

	///----------------------------------------------------------------------------------------------------
	/// Gets acceleration structure for the current mesh.
	/// 
	/// \return acceleration structure
	inline MMeshIntersector * getMeshIntersector() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets current Maya mesh function object.
	/// 
	/// \return Maya mesh function object.
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
	MeshPoint getMeshPoint( const UVPoint & aPoint ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets triangle represented as Triangle struct.
	/// 
	/// \param aID		The triangle index.
	/// 
	/// \return Requested triangle.
	///----------------------------------------------------------------------------------------------------
	const Triangle getTriangle( unsigned __int32 aID ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets number of mesh's triangles.
	/// 
	/// \return Number of mesh's triangles.
	///----------------------------------------------------------------------------------------------------
	unsigned __int32 getTriangleCount() const;

	///----------------------------------------------------------------------------------------------------
	/// If maya mesh was updated, this method will refresh proxies to new maya mesh.
	///
	/// \param aUpdatedMesh	Updated maya mesh object.
	/// \param aUVSet		UVSet name.
	///----------------------------------------------------------------------------------------------------
	void meshUpdate( MObject & aUpdatedMesh, const MString & aUVSet );

	///-------------------------------------------------------------------------------------------------
	/// Gets the mesh uv coordinate uniform grid. 
	/// Grid is stored inside this class and updated only when this method is called.
	///
	/// \return	The mesh uv coordinate uniform grid. 
	///-------------------------------------------------------------------------------------------------
	inline const MeshUVCoordUG & getMeshUVCoordUG() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the requested triangles. 
	///
	/// \param	aTrianglesIds	List of identifiers for the requested triangles. 
	/// \param [in,out]	aResult	The selected triangles array. 
	///-------------------------------------------------------------------------------------------------
	inline void getRequestedTriangles( const TrianglesIds aTrianglesIds, Triangles & aResult ) const;

	///-------------------------------------------------------------------------------------------------
	/// Serialize object (only critical data is stored).
	/// 
	/// \param	aOutputStream	Output stream
	///-------------------------------------------------------------------------------------------------
	void serialize( std::ostream & aOutputStream ) const;

	///-------------------------------------------------------------------------------------------------
	/// Deserialize object (only critical data is loaded, rest is rebuilded if needed).	
	///
	/// \param	aInputStream	Input stream
	///-------------------------------------------------------------------------------------------------
	void deserialize( std::istream & aInputStream );
	
	///----------------------------------------------------------------------------------------------------
	/// Finalizer
	///----------------------------------------------------------------------------------------------------
	~MayaMesh();

private:

	///-------------------------------------------------------------------------------------------------
	/// Gets all the triangles of the current mesh.
	///
	/// \param [in,out]	aResult	a returned triangles. 
	///-------------------------------------------------------------------------------------------------
	void getTriangles( Triangles & aResult ) const;

	MayaTriangles mMayaTriangles; ///< Triangles of mesh stored as indices to maya internal representation of triangles						

	Mesh mRestPose; ///< Rest pose mesh
	
	MMeshIntersector * mMeshIntersector; ///< Acceleration structure for finding closest point on the mesh

	MFnMesh * mMayaMesh; ///< The maya mesh function object
	
	MString mUVSet; ///< UV Set name

	mutable MeshUVCoordUG mMeshUVCoordUG;   ///< The mesh uv coordinates 2D uniform grid
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
	return mMayaMesh;
}

inline const MeshUVCoordUG & MayaMesh::getMeshUVCoordUG() const
{
	if ( mMeshUVCoordUG.isDirty() ) // Rebuild of ug is needed ?
	{
		Triangles triangles;
		getTriangles( triangles ); // Get triangles from maya
		mMeshUVCoordUG.build( triangles );
	}
	return mMeshUVCoordUG;
}

inline void MayaMesh::getRequestedTriangles( const TrianglesIds aTrianglesIds, Triangles & aResult ) const
{
	aResult.resize( aTrianglesIds.size() );
	Triangles::iterator outIt = aResult.begin();
	for ( TrianglesIds::const_iterator idIt = aTrianglesIds.begin(); idIt != aTrianglesIds.end();
		++idIt, ++outIt )
	{
		*outIt = getTriangle( *idIt ); // Output requested triangle
	}
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MAYA_MESH_HPP