#include "MeshPoint.hpp"

#include "Primitives\Vector3D.hpp"

using namespace Stubble;
using namespace HairShape;

inline MeshPoint::MeshPoint()
{
}

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

inline MeshPoint::MeshPoint( const Vector3D< double > &aPosition, double aUCoordinate, double aVCoordinate ):
	mPosition( aPosition ),
	mUCoordinate( aUCoordinate ),
	mVCoordinate( aVCoordinate )
{
}

inline const Vector3D< double > & MeshPoint::getPosition() const 
{
	return mPosition;
}

inline const Vector3D< double > & MeshPoint::getNormal() const 
{
	return mNormal;
}

inline const Vector3D< double > & MeshPoint::getTangent() const 
{
	return mTangent;
}

inline const Vector3D< double > & MeshPoint::getBinormal() const 
{
	return mBinormal;
}

inline double MeshPoint::getUCoordinate() const 
{
	return mUCoordinate;
}

inline double MeshPoint::getVCoordinate() const 
{
	return mVCoordinate;
}

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

inline std::ostream & HairShape::operator<<( std::ostream &aStreamOut, const MeshPoint &aPointOnMesh )
{
	return aStreamOut << aPointOnMesh.mPosition << " " << aPointOnMesh.mNormal << " " << aPointOnMesh.mTangent 
		<< " " << aPointOnMesh.mBinormal << " " << aPointOnMesh.mUCoordinate << " " << aPointOnMesh.mVCoordinate;
}

inline std::istream & HairShape::operator>>( std::istream & aStreamIn, MeshPoint & aPointOnMesh )
{
	return aStreamIn >> aPointOnMesh.mPosition >> aPointOnMesh.mNormal >> aPointOnMesh.mTangent 
		>> aPointOnMesh.mBinormal >> aPointOnMesh.mUCoordinate >> aPointOnMesh.mVCoordinate;
}