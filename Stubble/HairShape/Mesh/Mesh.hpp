#ifndef STUBBLE_MESH_HPP
#define STUBBLE_MESH_HPP

#include "HairShape/Mesh/TriangleConstIterator.hpp"
#include "HairShape/Mesh/UVPoint.hpp"
#include "HairShape/Texture/Texture.hpp"
#include "Primitives/BoundingBox.hpp"
#include "Common\CommonConstants.hpp"
#include "Common\CommonFunctions.hpp"

#include <fstream>
#include <string>
#include <sstream>

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
	/// \param	aInStream				input file stream
	/// \param	aCalculateDerivatives	if true, partial derivatives of position and normal will be
	/// 								calculated ( used for surface displacement )
	///----------------------------------------------------------------------------------------------------
	Mesh( std::istream & aInStream, bool aCalculateDerivatives = false );

	///----------------------------------------------------------------------------------------------------
	/// Constructor realized from triangles array
	///
	/// \param	aTriangles				Input triangles
	/// \param	aCalculateDerivatives	if true, partial derivatives of position and normal will be
	/// 								calculated ( used for surface displacement )
	///----------------------------------------------------------------------------------------------------
	Mesh( const Triangles &aTriangles , bool aCalculateDerivatives = false );

	///-------------------------------------------------------------------------------------------------
	/// Exports mesh. 
	///
	/// \param [in,out]	aOutputStream	The output stream. 
	///-------------------------------------------------------------------------------------------------
	void exportMesh( std::ostream & aOutputStream ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets const triangle iterator.
	///----------------------------------------------------------------------------------------------------
	inline TriangleConstIterator getTriangleConstIterator() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets point on mesh interpolated from 3 vertices of given triangle
	/// 
	/// \param	aPoint	The triangle id and barycentric coordinates
	/// 				
	/// \return	The mesh point.
	///----------------------------------------------------------------------------------------------------
	inline MeshPoint getMeshPoint( const UVPoint &aPoint ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets point on mesh interpolated from 3 vertices of given triangle.
	/// Only position and texture coordinates are interpolated.
	/// 
	/// \param	aPoint	The triangle id and barycentric coordinates
	/// 				
	/// \return	The position and texture coordinates.
	///----------------------------------------------------------------------------------------------------
	inline MeshPoint getIncompleteMeshPoint( const UVPoint &aPoint ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets point on mesh interpolated from 3 vertices of given triangle.
	/// Only position is interpolated.
	/// 
	/// \param	aPoint	The triangle id and barycentric coordinates
	/// 				
	/// \return	The position.
	///----------------------------------------------------------------------------------------------------
	inline Vector3D< Real > getPosition( const UVPoint &aPoint ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets point on displaced mesh interpolated from 3 vertices of given triangle
	///
	/// \param	aPoint					The triangle id and barycentric coordinates
	/// \param	aDisplacementTexture	The mesh displacement texture. 
	/// \param	aDisplacementFactor		The displacement texture will be mutliplied by this factor.
	///
	/// \return	The displaced mesh point. 
	///----------------------------------------------------------------------------------------------------
	inline MeshPoint getDisplacedMeshPoint( const UVPoint &aPoint, const Texture & aDisplacementTexture, 
		Real aDisplacementFactor ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets triangle as 3 vertices.
	///----------------------------------------------------------------------------------------------------
	inline const Triangle & getTriangle( unsigned __int32 aID ) const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the selected triangles. 
	///
	/// \param	aTrianglesIds	List of identifiers for a triangles. 
	/// \param [in,out]	aResult	The selected triangles. 
	///-------------------------------------------------------------------------------------------------
	inline void getSelectedTriangles( const TrianglesIds aTrianglesIds, Triangles & aResult ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets number of mesh's triangles.
	///----------------------------------------------------------------------------------------------------
	inline unsigned __int32 getTriangleCount() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets mesh's bounding box
	///----------------------------------------------------------------------------------------------------
	inline BoundingBox getBoundingBox() const;

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
	Vector3D< Real > normal = p0.getNormal() * u + p1.getNormal() * v + p2.getNormal() * w;
	Vector3D< Real > tangent = p0.getTangent() * u + p1.getTangent() * v + p2.getTangent() * w;

	// Normalize normal
	normal.normalize();

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

inline MeshPoint Mesh::getDisplacedMeshPoint( const UVPoint &aPoint, const Texture & aDisplacementTexture, 
	Real aDisplacementFactor ) const
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
	Vector3D< Real > position = p0.getPosition() * u + p1.getPosition() * v + p2.getPosition() * w;
	Vector3D< Real > normal = p0.getNormal() * u + p1.getNormal() * v + p2.getNormal() * w;
	Vector3D< Real > tangent = p0.getTangent() * u + p1.getTangent() * v + p2.getTangent() * w;

	Real textU = static_cast< Real >( u * p0.getUCoordinate() + v * p1.getUCoordinate() + w * p2.getUCoordinate() );
	Real textV = static_cast< Real >( u * p0.getVCoordinate() + v * p1.getVCoordinate() + w * p2.getVCoordinate() );

	// Select displace and its derivatives
	Real displace = aDisplacementTexture.realAtUV( textU, textV ) * aDisplacementFactor;
	Real displaceDU = aDisplacementTexture.derivativeByUAtUV( textU, textV ) * aDisplacementFactor;
	Real displaceDV = aDisplacementTexture.derivativeByVAtUV( textU, textV ) * aDisplacementFactor;

	// Normalize normal
	normal.normalize();

	// Now displace position
	position += normal * displace;

	// Recalculate normal 
	Vector3D< Real > dpdu = triangle.getDPDU() + normal * displaceDU + triangle.getDNDU() * displace;
	Vector3D< Real > dpdv = triangle.getDPDV() + normal * displaceDV + triangle.getDNDV() * displace;
	normal = Vector3D< Real >::crossProduct( dpdu, dpdv );
	normal.normalize();
	// Orthonormalize tangent to normal
	tangent -= normal * ( Vector3D< Real >::dotProduct( tangent, normal ) ); 
	tangent.normalize();
	return MeshPoint( position, normal, tangent, textU, textV );
}

inline const Triangle & Mesh::getTriangle( unsigned __int32 aID ) const
{
	return mTriangles[ aID ];
}

inline void Mesh::getSelectedTriangles( const TrianglesIds aTrianglesIds, Triangles & aResult ) const
{
	aResult.resize( aTrianglesIds.size() );
	Triangles::iterator outIt = aResult.begin();
	for ( TrianglesIds::const_iterator idIt = aTrianglesIds.begin(); idIt != aTrianglesIds.end();
		++idIt, ++outIt )
	{
		*outIt = mTriangles[ *idIt ];
	}
}

inline unsigned __int32 Mesh::getTriangleCount() const
{
	return static_cast< unsigned __int32 > (mTriangles.size());
}

inline BoundingBox Mesh::getBoundingBox() const
{
	return mBoundingBox;
}

inline std::string Mesh::serialize() const
{
	std::ostringstream oss;	
	oss << Stubble::serializeObjects< Triangle >( mTriangles )
		<< mBoundingBox.serialize();
	return oss.str();
}

inline size_t Mesh::deserialize( const std::string &aStr, size_t aPos )
{
	mTriangles = Stubble::deserializeObjects< Triangle >( aStr, aPos );
	aPos = mBoundingBox.deserialize( aStr, aPos );
	return aPos;
}

inline Mesh::~Mesh()
{
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MESH_HPP