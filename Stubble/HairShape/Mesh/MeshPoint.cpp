#include "MeshPoint.hpp"

#include "Primitives\Vector3D.hpp"

using namespace Stubble;
using namespace HairShape;

///----------------------------------------------------------------------------------------------------
/// Default constructor. 
///----------------------------------------------------------------------------------------------------
inline MeshPoint::MeshPoint()
{
}

///----------------------------------------------------------------------------------------------------
/// Creates MeshPoint class. 
///
/// \param	aPosition		the point position. 
/// \param	aNormal			the normal in selected position. 
/// \param	aTangent		the tangent in selected position. 
/// \param	aUCoordinate	the u coordinate. 
/// \param	aVCoordinate	the v coordinate. 
///----------------------------------------------------------------------------------------------------
inline MeshPoint::MeshPoint( const Vector3D< double > &aPosition, const Vector3D< double > &aNormal, 
	const Vector3D< double > &aTangent, double aUCoordinate, double aVCoordinate ):
	mPosition( aPosition ),
	mNormal( aNormal ),
	mTangent( aTangent ),
	mUCoordinate( aUCoordinate ),
	mVCoordinate( aVCoordinate ),
	// Calculates binormal as normalized aTangent x aNormal
	mBinormal( Vector3D< double >::crossProduct( aTangent, aNormal ).normalize() )
{
}

///----------------------------------------------------------------------------------------------------
/// Creates MeshPoint class without normal and tangent specification. 
///
/// \param	aPosition		the point position. 
/// \param	aUCoordinate	the u coordinate. 
/// \param	aVCoordinate	the v coordinate. 
///----------------------------------------------------------------------------------------------------
inline MeshPoint::MeshPoint( const Vector3D< double > &aPosition, double aUCoordinate, double aVCoordinate ):
	mPosition( aPosition ),
	mUCoordinate( aUCoordinate ),
	mVCoordinate( aVCoordinate )
{
}

///----------------------------------------------------------------------------------------------------
/// Gets the position of point. 
///
/// \return	The position of point. 
///----------------------------------------------------------------------------------------------------
inline const Vector3D< double > & MeshPoint::getPosition() const 
{
	return mPosition;
}

///----------------------------------------------------------------------------------------------------
/// Gets the normal. 
///
/// \return	The normal. 
///----------------------------------------------------------------------------------------------------
inline const Vector3D< double > & MeshPoint::getNormal() const 
{
	return mNormal;
}

///----------------------------------------------------------------------------------------------------
/// Gets the tangent. 
///
/// \return	The tangent. 
///----------------------------------------------------------------------------------------------------
inline const Vector3D< double > & MeshPoint::getTangent() const 
{
	return mTangent;
}

///----------------------------------------------------------------------------------------------------
/// Gets the binormal. 
///
/// \return	The binormal. 
///----------------------------------------------------------------------------------------------------
inline const Vector3D< double > & MeshPoint::getBinormal() const 
{
	return mBinormal;
}

///----------------------------------------------------------------------------------------------------
/// Gets the u coordinate. 
///
/// \return	The u coordinate. 
///----------------------------------------------------------------------------------------------------
inline double MeshPoint::getUCoordinate() const 
{
	return mUCoordinate;
}

///----------------------------------------------------------------------------------------------------
/// Gets the v coordinate. 
///
/// \return	The v coordinate. 
///----------------------------------------------------------------------------------------------------
inline double MeshPoint::getVCoordinate() const 
{
	return mVCoordinate;
}

///----------------------------------------------------------------------------------------------------
/// Converts local vector to a world vector. 
///
/// \param	aLocalVector	the local vector. 
///
/// \return	world vector
///----------------------------------------------------------------------------------------------------
inline Vector3D< double > MeshPoint::toWorld( const Vector3D< double > aLocalVector ) const
{
	return Vector3D< double >( 
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

///----------------------------------------------------------------------------------------------------
/// Put point on mesh into stream 
///
/// \param [in,out]	aStreamOut	a stream out. 
/// \param aPointOnMesh			a point on mesh. 
///
/// \return	The updated stream. 
///----------------------------------------------------------------------------------------------------
inline std::ostream & HairShape::operator<<( std::ostream &aStreamOut, const MeshPoint &aPointOnMesh )
{
	return aStreamOut << aPointOnMesh.mPosition << " " << aPointOnMesh.mNormal << " " << aPointOnMesh.mTangent 
		<< " " << aPointOnMesh.mBinormal << " " << aPointOnMesh.mUCoordinate << " " << aPointOnMesh.mVCoordinate;
}

///----------------------------------------------------------------------------------------------------
/// Pull point on mesh from stream 
///
/// \param [in,out]	aStreamIn		a stream in. 
/// \param [in,out]	aPointOnMesh	a point on mesh. 
///
/// \return	The updated stream. 
///----------------------------------------------------------------------------------------------------
inline std::istream & HairShape::operator>>( std::istream & aStreamIn, MeshPoint & aPointOnMesh )
{
	return aStreamIn >> aPointOnMesh.mPosition >> aPointOnMesh.mNormal >> aPointOnMesh.mTangent 
		>> aPointOnMesh.mBinormal >> aPointOnMesh.mUCoordinate >> aPointOnMesh.mVCoordinate;
}