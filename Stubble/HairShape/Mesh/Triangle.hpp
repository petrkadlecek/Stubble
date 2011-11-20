#ifndef STUBBLE_TRIANGLE_HPP
#define STUBBLE_TRIANGLE_HPP

#include "HairShape\Mesh\MeshPoint.hpp"
#include "Common\CommonConstants.hpp"
#include "Common\CommonFunctions.hpp"

#include <vector>
#include <sstream>
#include <string>

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// Triangle. 
///----------------------------------------------------------------------------------------------------
class Triangle
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///----------------------------------------------------------------------------------------------------
	inline Triangle();

	///----------------------------------------------------------------------------------------------------
	/// Constructor. 
	///
	/// \param	mVertex1	The first triangle Vertex. 
	/// \param	mVertex2	The second triangle Vertex. 
	/// \param	mVertex3	The third triangle Vertex. 
	///----------------------------------------------------------------------------------------------------
	inline Triangle( const MeshPoint & mVertex1, const MeshPoint & mVertex2, const MeshPoint & mVertex3 );

	///----------------------------------------------------------------------------------------------------
	/// Constructor realized from stream
	///
	/// \param	aInStream				input file stream
	/// \param	aCalculateDerivatives	if true, partial derivatives of position and normal will be
	/// 								calculated ( used for surface displacement )
	///----------------------------------------------------------------------------------------------------
	inline Triangle( std::istream & aInStream, bool aCalculateDerivatives = false );

	///-------------------------------------------------------------------------------------------------
	/// Exports triangle. 
	///
	/// \param [in,out]	aOutputStream	The output stream. 
	///-------------------------------------------------------------------------------------------------
	inline void exportTriangle( std::ostream & aOutputStream ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the Vertex 1. 
	///
	/// \return	The Vertex 1. 
	///----------------------------------------------------------------------------------------------------
	inline const MeshPoint & getVertex1() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the Vertex 2. 
	///
	/// \return	The Vertex 2. 
	///----------------------------------------------------------------------------------------------------
	inline const MeshPoint & getVertex2() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the Vertex 3. 
	///
	/// \return	The Vertex 3. 
	///----------------------------------------------------------------------------------------------------
	inline const MeshPoint & getVertex3() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the barycenter of triangle.
	///
	/// \return	The barycenter. 
	///----------------------------------------------------------------------------------------------------
	inline Vector3D< Real > getBarycenter() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the derivation of position according to u coordinate.
	///
	/// \return	The derivation of position according to u coordinate.
	///-------------------------------------------------------------------------------------------------
	inline const Vector3D< Real > & getDPDU() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the derivation of position according to v coordinate.
	///
	/// \return	The derivation of position according to v coordinate.
	///-------------------------------------------------------------------------------------------------
	inline const Vector3D< Real > & getDPDV() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the derivation of normal according to u coordinate.
	///
	/// \return	The derivation of normal according to u coordinate.
	///-------------------------------------------------------------------------------------------------
	inline const Vector3D< Real > & getDNDU() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the derivation of normal according to v coordinate.
	///
	/// \return	The derivation of normal according to v coordinate.
	///-------------------------------------------------------------------------------------------------
	inline const Vector3D< Real > & getDNDV() const;

	///-------------------------------------------------------------------------------------------------
	/// Recalculates derivatives. 
	///-------------------------------------------------------------------------------------------------
	inline void recalculateDerivatives();

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

private:

	MeshPoint mVertices[ 3 ]; ///< The Vertices

	Vector3D< Real > mDPDU; ///< The derivation of position according to u coordinate

	Vector3D< Real > mDPDV; ///< The derivation of position according to v coordinate

	Vector3D< Real > mDNDU; ///< The derivation of normal according to u coordinate

	Vector3D< Real > mDNDV; ///< The derivation of normal according to v coordinate
};

///----------------------------------------------------------------------------------------------------
/// Defines an alias representing the triangles .
///----------------------------------------------------------------------------------------------------
typedef std::vector< Triangle > Triangles;

///----------------------------------------------------------------------------------------------------
/// Defines an alias representing list of identifiers for the triangles .
///----------------------------------------------------------------------------------------------------
typedef std::vector< unsigned __int32 > TrianglesIds;

// inline functions implementation

inline Triangle::Triangle()
{
}

inline Triangle::Triangle( const MeshPoint & mVertex1, const MeshPoint & mVertex2, const MeshPoint & mVertex3 )
{
	mVertices[ 0 ] = mVertex1;
	mVertices[ 1 ] = mVertex2;
	mVertices[ 2 ] = mVertex3;
}

inline Triangle::Triangle( std::istream & aInStream, bool aCalculateDerivatives )
{
	aInStream >> mVertices[ 0 ] >> mVertices[ 1 ] >> mVertices[ 2 ];
	if ( aCalculateDerivatives )
	{
		recalculateDerivatives();
	}
}

inline void Triangle::exportTriangle( std::ostream & aOutputStream ) const
{
	aOutputStream << mVertices[ 0 ];
	aOutputStream << mVertices[ 1 ];
	aOutputStream << mVertices[ 2 ];
}

inline const MeshPoint & Triangle::getVertex1() const
{
	return mVertices[ 0 ];
}

inline const MeshPoint & Triangle::getVertex2() const
{
	return mVertices[ 1 ];
}

inline const MeshPoint & Triangle::getVertex3() const
{
	return mVertices[ 2 ];
}

inline Vector3D< Real > Triangle::getBarycenter() const
{
	return ( mVertices[ 0 ].getPosition() + 
		mVertices[ 1 ].getPosition() + 
		mVertices[ 2 ].getPosition() ) / 3; 
}

inline const Vector3D< Real > & Triangle::getDPDU() const
{
	return mDPDU;
}

inline const Vector3D< Real > & Triangle::getDPDV() const
{
	return mDPDV;
}

inline const Vector3D< Real > & Triangle::getDNDU() const
{
	return mDNDU;
}

inline const Vector3D< Real > & Triangle::getDNDV() const
{
	return mDNDV;
}

inline void Triangle::recalculateDerivatives()
{
	// Calculate du, dv for 2 triangle vectors
	Real du1 = mVertices[ 0 ].getUCoordinate() - mVertices[ 2 ].getUCoordinate();
	Real du2 = mVertices[ 1 ].getUCoordinate() - mVertices[ 2 ].getUCoordinate();
	Real dv1 = mVertices[ 0 ].getVCoordinate() - mVertices[ 2 ].getVCoordinate();
	Real dv2 = mVertices[ 1 ].getVCoordinate() - mVertices[ 2 ].getVCoordinate();
	// Calculate determinant of matrix 2x2
	Real determinant = du1 * dv2 - dv1 * du2;
	// Calculate diff. vectors based on position or normals
	Vector3D< Real > dp1 = mVertices[ 0 ].getPosition() - mVertices[ 2 ].getPosition();
	Vector3D< Real > dp2 = mVertices[ 1 ].getPosition() - mVertices[ 2 ].getPosition();
	Vector3D< Real > dn1 = mVertices[ 0 ].getNormal() - mVertices[ 2 ].getNormal();
	Vector3D< Real > dn2 = mVertices[ 1 ].getNormal() - mVertices[ 2 ].getNormal();
	if ( determinant == 0 ) // Handle singular matrix
	{
		// Keep DNDU and DNDV as 0,0,0 vectors
		// Set DPDU and DPDV as 2 perpendicular vectors together perpendicular to normal of the triangle
		Vector3D< Real > n = Vector3D< Real >::crossProduct( dp1, dp2 );
		// Select smaller component to become zero, switch the other two and negate one of them
		if ( abs( n.x ) > abs( n.y ) )
		{
			Real invLen = static_cast< Real >( 1.0 / sqrt( n.x * n.x + n.z * n.z ) );
			mDPDU = Vector3D< Real >( -n.z * invLen, 0, n.x * invLen );
		}
		else
		{
			Real invLen = static_cast< Real >( 1.0 / sqrt( n.y * n.y + n.z * n.z ) );
			mDPDU = Vector3D< Real >( 0, n.z * invLen, - n.y * invLen );
		}
		mDPDV = Vector3D< Real >::crossProduct( mDPDU, n );
	}
	else
	{
		// Finally calculate derivatives
		Real invDet = 1.f / determinant;
		mDPDU = ( dp1 * dv2 - dp2 * dv1 ) * invDet;
		mDPDV = ( -dp1 * du2 + dp2 * du1 ) * invDet;
		mDNDU = ( dn1 * dv2 - dn2 * dv1 ) * invDet;
		mDNDV = ( -dn1 * du2 + dn2 * du1 ) * invDet;
	}
}

inline std::string Triangle::serialize() const
{
	std::ostringstream oss;
	oss << mVertices[0].serialize()
		<< mVertices[1].serialize()
		<< mVertices[2].serialize()
		<< mDPDU.serialize()
		<< mDPDV.serialize()
		<< mDNDU.serialize()
		<< mDNDV.serialize();
	return oss.str();
}

inline size_t Triangle::deserialize( const std::string &aStr, size_t aPos )
{
	aPos = mVertices[0].deserialize( aStr, aPos );
	aPos = mVertices[1].deserialize( aStr, aPos );
	aPos = mVertices[2].deserialize( aStr, aPos );
	aPos = mDPDU.deserialize( aStr, aPos );
	aPos = mDPDV.deserialize( aStr, aPos );
	aPos = mDNDU.deserialize( aStr, aPos );
	aPos = mDNDV.deserialize( aStr, aPos );
	return aPos;
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_TRIANGLE_HPP