#ifndef STUBBLE_MESH_POINT_HPP
#define STUBBLE_MESH_POINT_HPP

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
	inline MeshPoint( const Vector3D< double > &aPosition, const Vector3D< double > &aNormal, 
		const Vector3D< double > &aTangent, double aUCoordinate, double aVCoordinate );

	///----------------------------------------------------------------------------------------------------
	/// Creates MeshPoint class without normal and tangent specification. 
	///
	/// \param	aPosition		the point position. 
	/// \param	aUCoordinate	the u coordinate. 
	/// \param	aVCoordinate	the v coordinate. 
	///----------------------------------------------------------------------------------------------------
	inline MeshPoint( const Vector3D< double > &aPosition, double aUCoordinate, double aVCoordinate );

	///----------------------------------------------------------------------------------------------------
	/// Gets the position of point. 
	///
	/// \return	The position of point. 
	///----------------------------------------------------------------------------------------------------
	inline const Vector3D< double > & getPosition() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the normal. 
	///
	/// \return	The normal. 
	///----------------------------------------------------------------------------------------------------
	inline const Vector3D< double > & getNormal() const;
	
	///----------------------------------------------------------------------------------------------------
	/// Gets the tangent. 
	///
	/// \return	The tangent. 
	///----------------------------------------------------------------------------------------------------
	inline const Vector3D< double > & getTangent() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the binormal. 
	///
	/// \return	The binormal. 
	///----------------------------------------------------------------------------------------------------
	inline const Vector3D< double > & getBinormal() const;
		
	///----------------------------------------------------------------------------------------------------
	/// Gets the u coordinate. 
	///
	/// \return	The u coordinate. 
	///----------------------------------------------------------------------------------------------------
	inline double getUCoordinate() const;
	
	///----------------------------------------------------------------------------------------------------
	/// Gets the v coordinate. 
	///
	/// \return	The v coordinate. 
	///----------------------------------------------------------------------------------------------------
	inline double getVCoordinate() const; 

	///----------------------------------------------------------------------------------------------------
	/// Converts local vector to a world vector. 
	///
	/// \param	aLocalVector	the local vector. 
	///
	/// \return	world vector
	///----------------------------------------------------------------------------------------------------
	Vector3D< double > toWorld( const Vector3D< double > aLocalVector ) const;
	
private:

	Vector3D< double > mPosition; ///< The point position 

	Vector3D< double > mNormal; ///< The normal in selected position

	Vector3D< double > mTangent; ///< The tangent in selected position

	Vector3D< double > mBinormal; ///< The binormal in selected position
	
	double mUCoordinate; ///< The texture u coordinate

	double mVCoordinate; ///< The texture v coordinate
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

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MESH_POINT_HPP