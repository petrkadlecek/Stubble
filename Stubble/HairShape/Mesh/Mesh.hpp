#ifndef STUBBLE_MESH_HPP
#define STUBBLE_MESH_HPP

#include "HairShape\Mesh\TriangleConstIterator.hpp"
#include "HairShape\Mesh\UVPoint.hpp"
#include "Primitives\BoundingBox.hpp"

#include <fstream>

namespace Stubble
{

namespace HairShape
{
///----------------------------------------------------------------------------------------------------
/// Stores mesh in inner format.
///----------------------------------------------------------------------------------------------------
class Mesh
{
#ifdef MAYA
	friend class MayaMesh;
#endif

public:
	///----------------------------------------------------------------------------------------------------
	/// Constructor realized from stream
	///
	/// \param	aInStream		input file stream
	/// \param	aPositionsOnly	if true, only positions are stored in stream
	///----------------------------------------------------------------------------------------------------
	Mesh( std::istream & aInStream, bool aPositionsOnly = false );

	///----------------------------------------------------------------------------------------------------
	/// Copies the texture coordinates from aMeshWithTextureCoordinates. 
	///
	/// \param	aMeshWithTextureCoordinates	The mesh with texture coordinates. 
	///----------------------------------------------------------------------------------------------------
	void CopyTextureCoordinates( const Mesh & aMeshWithTextureCoordinates );

	///----------------------------------------------------------------------------------------------------
	/// Gets const triangle iterator.
	///----------------------------------------------------------------------------------------------------
	inline TriangleConstIterator getTriangleConstIterator() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets point on mesh interpolated from 3 vertices of given triangle
	/// 
	/// \param	aPoint	The triangle id and barycentric coordinates
	///----------------------------------------------------------------------------------------------------
	inline MeshPoint getMeshPoint( const UVPoint &aPoint ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets point on mesh interpolated from 3 vertices of given triangle.
	/// Only position and texture coordinates are interpolated.
	/// 
	/// \param	aPoint	The triangle id and barycentric coordinates
	///----------------------------------------------------------------------------------------------------
	inline MeshPoint getIncompleteMeshPoint( const UVPoint &aPoint ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets point on mesh interpolated from 3 vertices of given triangle.
	/// Only position is interpolated.
	/// 
	/// \param	aPoint	The triangle id and barycentric coordinates
	///----------------------------------------------------------------------------------------------------
	inline Vector3D< Real > getPosition( const UVPoint &aPoint ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets triangle as 3 vertices.
	///----------------------------------------------------------------------------------------------------
	inline const Triangle & getTriangle( unsigned __int32 aID ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets number of mesh's triangles.
	///----------------------------------------------------------------------------------------------------
	inline unsigned __int32 getTriangleCount() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets mesh's bounding box
	///----------------------------------------------------------------------------------------------------
	inline BoundingBox getBoundingBox() const;

	///----------------------------------------------------------------------------------------------------
	/// Finalizer.
	///----------------------------------------------------------------------------------------------------
	inline ~Mesh();

protected:
	///----------------------------------------------------------------------------------------------------
	/// Protected default constructor
	///----------------------------------------------------------------------------------------------------
	inline Mesh();

private:
	Triangles mTriangles; ///< Triangles of mesh

	BoundingBox mBoundingBox; ///< Bounding box of mesh
};

inline Mesh::Mesh()
{
}

// inline functions implementation
inline TriangleConstIterator Mesh::getTriangleConstIterator() const
{
	return TriangleConstIterator(mTriangles.begin(), mTriangles.end());
}

inline MeshPoint Mesh::getMeshPoint( const UVPoint &aPoint ) const
{
	const double u = aPoint.getU();
	const double v = aPoint.getV();
	const double w = 1 - u - v;

	// Get triangle
	const Triangle &triangle = mTriangles[ aPoint.getTriangleID() ];

	const MeshPoint & p0 = triangle.getVertex1();
	const MeshPoint & p1 = triangle.getVertex2();
	const MeshPoint & p2 = triangle.getVertex3();

	// Calculate interpolation
	const Vector3D< Real > position = p0.getPosition() * u + p1.getPosition() * v + p2.getPosition() * w;
	const Vector3D< Real > normal = p0.getNormal() * u + p1.getNormal() * v + p2.getNormal() * w;
	Vector3D< Real > tangent = p0.getTangent() * u + p1.getTangent() * v + p2.getTangent() * w;

	// Orthonormalize tangent to normal
	tangent -= normal * ( Vector3D< Real >::dotProduct( tangent, normal ) ); 
	tangent.normalize();

	Real textU = static_cast< Real >( u * p0.getUCoordinate() + v * p1.getUCoordinate() + w * p2.getUCoordinate() );
	Real textV = static_cast< Real >( u * p0.getVCoordinate() + v * p1.getVCoordinate() + w * p2.getVCoordinate() );

	return MeshPoint( position, normal, tangent, textU, textV );
}

inline MeshPoint Mesh::getIncompleteMeshPoint( const UVPoint &aPoint ) const
{
	const double u = aPoint.getU();
	const double v = aPoint.getV();
	const double w = 1 - u - v;

	// Get triangle
	const Triangle &triangle = mTriangles[ aPoint.getTriangleID() ];

	const MeshPoint & p0 = triangle.getVertex1();
	const MeshPoint & p1 = triangle.getVertex2();
	const MeshPoint & p2 = triangle.getVertex3();

	// Calculate interpolation
	const Vector3D< Real > position = p0.getPosition() * u + p1.getPosition() * v + p2.getPosition() * w;

	const Real textU = static_cast< Real >( u * p0.getUCoordinate() + v * p1.getUCoordinate() + w * p2.getUCoordinate() );
	const Real textV = static_cast< Real >( u * p0.getVCoordinate() + v * p1.getVCoordinate() + w * p2.getVCoordinate() );

	return MeshPoint( position, textU, textV );
}

inline Vector3D< Real > Mesh::getPosition( const UVPoint &aPoint ) const
{
	const double u = aPoint.getU();
	const double v = aPoint.getV();
	const double w = 1 - u - v;

	// Get triangle
	const Triangle &triangle = mTriangles[ aPoint.getTriangleID() ];

	const MeshPoint & p0 = triangle.getVertex1();
	const MeshPoint & p1 = triangle.getVertex2();
	const MeshPoint & p2 = triangle.getVertex3();

	// Calculate interpolation
	return p0.getPosition() * u + p1.getPosition() * v + p2.getPosition() * w;
}

inline const Triangle & Mesh::getTriangle( unsigned __int32 aID ) const
{
	return mTriangles[ aID ];
}

inline unsigned __int32 Mesh::getTriangleCount() const
{
	return static_cast< unsigned __int32 > (mTriangles.size());
}

inline BoundingBox Mesh::getBoundingBox() const
{
	return mBoundingBox;
}

inline Mesh::~Mesh()
{
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MESH_HPP