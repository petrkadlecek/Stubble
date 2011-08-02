#ifndef STUBBLE_MESH_POINT_HPP
#define STUBBLE_MESH_POINT_HPP

#include "Common\CommonTypes.hpp"
#include "Primitives\Vector3D.hpp"

#include <sstream>

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// Stores all information needed about single point on mesh. 
///----------------------------------------------------------------------------------------------------
class MeshPoint
{
	friend inline std::ostream & operator<<( std::ostream & aStreamOut, const MeshPoint & aPointOnMesh );
	friend inline std::istream & operator>>( std::istream & aStreamIn, MeshPoint & aPointOnMesh );

public:

	///----------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///----------------------------------------------------------------------------------------------------
	inline MeshPoint(); 

	///----------------------------------------------------------------------------------------------------
	/// Creates MeshPoint class. 
	///
	/// \param	aPosition		the point position. 
	/// \param	aNormal			the normal in selected position. 
	/// \param	aTangent		the tangent in selected position. 
	/// \param	aUCoordinate	the u coordinate. 
	/// \param	aVCoordinate	the v coordinate. 
	///----------------------------------------------------------------------------------------------------
	inline MeshPoint( const Vector3D< Real > &aPosition, const Vector3D< Real > &aNormal, 
		const Vector3D< Real > &aTangent, Real aUCoordinate, Real aVCoordinate );

	///----------------------------------------------------------------------------------------------------
	/// Creates MeshPoint class without normal and tangent specification. 
	///
	/// \param	aPosition		the point position. 
	/// \param	aUCoordinate	the u coordinate. 
	/// \param	aVCoordinate	the v coordinate. 
	///----------------------------------------------------------------------------------------------------
	inline MeshPoint( const Vector3D< Real > &aPosition, Real aUCoordinate, Real aVCoordinate );

	///----------------------------------------------------------------------------------------------------
	/// Gets the position of point. 
	///
	/// \return	The position of point. 
	///----------------------------------------------------------------------------------------------------
	inline const Vector3D< Real > & getPosition() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the normal. 
	///
	/// \return	The normal. 
	///----------------------------------------------------------------------------------------------------
	inline const Vector3D< Real > & getNormal() const;
	
	///----------------------------------------------------------------------------------------------------
	/// Gets the tangent. 
	///
	/// \return	The tangent. 
	///----------------------------------------------------------------------------------------------------
	inline const Vector3D< Real > & getTangent() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the binormal. 
	///
	/// \return	The binormal. 
	///----------------------------------------------------------------------------------------------------
	inline const Vector3D< Real > & getBinormal() const;
		
	///----------------------------------------------------------------------------------------------------
	/// Gets the u coordinate. 
	///
	/// \return	The u coordinate. 
	///----------------------------------------------------------------------------------------------------
	inline Real getUCoordinate() const;
	
	///----------------------------------------------------------------------------------------------------
	/// Gets the v coordinate. 
	///
	/// \return	The v coordinate. 
	///----------------------------------------------------------------------------------------------------
	inline Real getVCoordinate() const; 

	///----------------------------------------------------------------------------------------------------
	/// Converts local vector to a world vector. 
	///
	/// \param	aLocalVector	the local vector. 
	///
	/// \return	world vector
	///----------------------------------------------------------------------------------------------------
	Vector3D< Real > toWorld( const Vector3D< Real > aLocalVector ) const;
	
private:

	Vector3D< Real > mPosition; ///< The point position 

	Vector3D< Real > mNormal; ///< The normal in selected position

	Vector3D< Real > mTangent; ///< The tangent in selected position

	Vector3D< Real > mBinormal; ///< The binormal in selected position
	
	Real mUCoordinate; ///< The texture u coordinate

	Real mVCoordinate; ///< The texture v coordinate
};

///----------------------------------------------------------------------------------------------------
/// Put point on mesh into stream 
///
/// \param [in,out]	aStreamOut	a stream out. 
/// \param aPointOnMesh			a point on mesh. 
///
/// \return	The updated stream. 
///----------------------------------------------------------------------------------------------------
inline std::ostream & operator<<( std::ostream & aStreamOut, const MeshPoint & aPointOnMesh );

///----------------------------------------------------------------------------------------------------
/// Pull point on mesh from stream 
///
/// \param [in,out]	aStreamIn		a stream in. 
/// \param [in,out]	aPointOnMesh	a point on mesh. 
///
/// \return	The updated stream. 
///----------------------------------------------------------------------------------------------------
inline std::istream & operator>>( std::istream & aStreamIn, MeshPoint & aPointOnMesh );

// inline functions implementation
inline MeshPoint::MeshPoint()
{
}

inline MeshPoint::MeshPoint( const Vector3D< Real > &aPosition, const Vector3D< Real > &aNormal, 
	const Vector3D< Real > &aTangent, Real aUCoordinate, Real aVCoordinate ):
	mPosition( aPosition ),
	mNormal( aNormal ),
	mTangent( aTangent ),
	mUCoordinate( aUCoordinate ),
	mVCoordinate( aVCoordinate ),
	// Calculates binormal as normalized aTangent x aNormal
	mBinormal( Vector3D< Real >::crossProduct( aTangent, aNormal ).normalize() )
{
}

inline MeshPoint::MeshPoint( const Vector3D< Real > &aPosition, Real aUCoordinate, Real aVCoordinate ):
mPosition( aPosition ),
mUCoordinate( aUCoordinate ),
mVCoordinate( aVCoordinate )
{
}

inline Vector3D< Real > MeshPoint::toWorld( const Vector3D< Real > aLocalVector ) const
{
	return Vector3D< Real >( 
		aLocalVector.x * mBinormal.x +
		aLocalVector.y * mTangent.x +
		aLocalVector.z * mNormal.x +
		mPosition.x,
		aLocalVector.x * mBinormal.y +
		aLocalVector.y * mTangent.y +
		aLocalVector.z * mNormal.y +
		mPosition.y,
		aLocalVector.x * mBinormal.z +
		aLocalVector.y * mTangent.z +
		aLocalVector.z * mNormal.z +
		mPosition.z );
}

inline const Vector3D< Real > & MeshPoint::getPosition() const 
{
	return mPosition;
}

inline const Vector3D< Real > & MeshPoint::getNormal() const 
{
	return mNormal;
}

inline const Vector3D< Real > & MeshPoint::getTangent() const 
{
	return mTangent;
}

inline const Vector3D< Real > & MeshPoint::getBinormal() const 
{
	return mBinormal;
}
	
inline Real MeshPoint::getUCoordinate() const 
{
	return mUCoordinate;
}

inline Real MeshPoint::getVCoordinate() const 
{
	return mVCoordinate;
}

inline std::ostream & HairShape::operator<<( std::ostream &aStreamOut, const MeshPoint &aPointOnMesh )
{
	aStreamOut << aPointOnMesh.mPosition << aPointOnMesh.mNormal << aPointOnMesh.mTangent << aPointOnMesh.mBinormal;
	aStreamOut.write( reinterpret_cast< const char * >( &aPointOnMesh.mUCoordinate ), sizeof( Real ) );
	aStreamOut.write( reinterpret_cast< const char * >( &aPointOnMesh.mVCoordinate ), sizeof( Real ) );
	return aStreamOut;
}

inline std::istream & HairShape::operator>>( std::istream & aStreamIn, MeshPoint & aPointOnMesh )
{
	aStreamIn >> aPointOnMesh.mPosition >> aPointOnMesh.mNormal >> aPointOnMesh.mTangent >> aPointOnMesh.mBinormal;
	aStreamIn.read( reinterpret_cast< char * >( &aPointOnMesh.mUCoordinate ), sizeof( Real ) );
	aStreamIn.read( reinterpret_cast< char * >( &aPointOnMesh.mVCoordinate ), sizeof( Real ) );
	return aStreamIn;
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MESH_POINT_HPP