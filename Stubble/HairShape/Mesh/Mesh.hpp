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
	/// \param	aInStream	input file stream
	///----------------------------------------------------------------------------------------------------
	inline Mesh( std::istream & aInStream );

	///----------------------------------------------------------------------------------------------------
	/// Gets const triangle iterator.
	///----------------------------------------------------------------------------------------------------
	inline TriangleConstIterator getTriangleConstIterator() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets point on mesh interpolated from 3 vertices of given triangle
	///----------------------------------------------------------------------------------------------------
	inline MeshPoint getMeshPoint( const UVPoint &aPoint ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets triangle as 3 vertices.
	///----------------------------------------------------------------------------------------------------
	inline const Triangle & getTriangle( int aID ) const;

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
	double u = aPoint.getU();
	double v = aPoint.getV();
	double w = 1 - u - v;

	// Get triangle
	const Triangle &triangle = mTriangles[ aPoint.getTriangleID() ];

	MeshPoint p0 = triangle.getVertex1();
	MeshPoint p1 = triangle.getVertex2();
	MeshPoint p2 = triangle.getVertex3();

	// Calculate interpolation
	Vector3D< Real > position = p0.getPosition() * u + p1.getPosition() * v + p2.getPosition() * w;
	Vector3D< Real > normal = p0.getNormal() * u + p1.getNormal() * v + p2.getNormal() * w;
	Vector3D< Real > tangent = p0.getTangent() * u + p1.getTangent() * v + p2.getTangent() * w;

	// Orthonormalize tangent to normal
	tangent -= normal * ( Vector3D< Real >::dotProduct( tangent, normal ) ); 
	tangent.normalize();

	Real textU = static_cast< Real >( u * p0.getUCoordinate() + v * p1.getUCoordinate() + w * p2.getUCoordinate() );
	Real textV = static_cast< Real >( u * p0.getVCoordinate() + v * p1.getVCoordinate() + w * p2.getVCoordinate() );

	return MeshPoint( position, normal, tangent, textU, textV );
}

inline const Triangle & Mesh::getTriangle( int aID ) const
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