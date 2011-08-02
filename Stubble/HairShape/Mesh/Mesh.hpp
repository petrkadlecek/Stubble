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
	inline unsigned int getTriangleCount() const;

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

inline unsigned int Mesh::getTriangleCount() const
{
	return static_cast< unsigned int > (mTriangles.size());
}

inline BoundingBox Mesh::getBoundingBox() const
{
	return mBoundingBox;
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MESH_HPP