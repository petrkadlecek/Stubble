#ifndef STUBBLE_MESH_POINT_HPP
#define STUBBLE_MESH_POINT_HPP

#include "Common\CommonTypes.hpp"
#include "Primitives\Matrix.hpp"
#include "Primitives\Vector3D.hpp"

#include <ostream>
#include <istream>

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

	///----------------------------------------------------------------------------------------------------
	/// Gets a local transform matrix. 
	///
	/// \param [out]	aLocalTransformMatrix	a local transform matrix. 
	///----------------------------------------------------------------------------------------------------
	inline void getLocalTransformMatrix( Matrix< Real > & aLocalTransformMatrix ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets a world transform matrix. 
	///
	/// \param [out]	aWorldTransformMatrix	a world transform matrix. 
	///----------------------------------------------------------------------------------------------------
	inline void getWorldTransformMatrix( Matrix< Real > & aWorldTransformMatrix ) const;

	///----------------------------------------------------------------------------------------------------
	/// Import position from stream. 
	///
	/// \param	aInStream		input file stream
	///----------------------------------------------------------------------------------------------------
	inline void importPosition( std::istream & aStreamIn );

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
	// Calculates binormal as aTangent x aNormal ( already normalized )
	mBinormal( Vector3D< Real >::crossProduct( aTangent, aNormal ) )
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

inline void MeshPoint::getLocalTransformMatrix( Matrix< Real > & aLocalTransformMatrix ) const
{
	aLocalTransformMatrix[ 0  ] = mBinormal.x;
	aLocalTransformMatrix[ 4  ] = mBinormal.y;
	aLocalTransformMatrix[ 8  ] = mBinormal.z;
	aLocalTransformMatrix[ 3  ] = 0;
	aLocalTransformMatrix[ 1  ] = mTangent.x;
	aLocalTransformMatrix[ 5  ] = mTangent.y;
	aLocalTransformMatrix[ 9  ] = mTangent.z;
	aLocalTransformMatrix[ 7  ] = 0;
	aLocalTransformMatrix[ 2  ] = mNormal.x;
	aLocalTransformMatrix[ 6  ] = mNormal.y;
	aLocalTransformMatrix[ 10 ] = mNormal.z;
	aLocalTransformMatrix[ 11 ] = 0;
	aLocalTransformMatrix[ 12 ] = Vector3D< Real >::dotProduct( mPosition, mBinormal );
	aLocalTransformMatrix[ 13 ] = Vector3D< Real >::dotProduct( mPosition, mTangent );
	aLocalTransformMatrix[ 14 ] = Vector3D< Real >::dotProduct( mPosition, mNormal );
	aLocalTransformMatrix[ 15 ] = 1;
}

inline void MeshPoint::getWorldTransformMatrix( Matrix< Real > & aWorldTransformMatrix ) const
{
	aWorldTransformMatrix[ 0  ] = mBinormal.x;
	aWorldTransformMatrix[ 1  ] = mBinormal.y;
	aWorldTransformMatrix[ 2  ] = mBinormal.z;
	aWorldTransformMatrix[ 3  ] = 0;
	aWorldTransformMatrix[ 4  ] = mTangent.x;
	aWorldTransformMatrix[ 5  ] = mTangent.y;
	aWorldTransformMatrix[ 6  ] = mTangent.z;
	aWorldTransformMatrix[ 7  ] = 0;
	aWorldTransformMatrix[ 8  ] = mNormal.x;
	aWorldTransformMatrix[ 9  ] = mNormal.y;
	aWorldTransformMatrix[ 10 ] = mNormal.z;
	aWorldTransformMatrix[ 11 ] = 0;
	aWorldTransformMatrix[ 12 ] = mPosition.x;
	aWorldTransformMatrix[ 13 ] = mPosition.y;
	aWorldTransformMatrix[ 14 ] = mPosition.z;
	aWorldTransformMatrix[ 15 ] = 1;
}

///----------------------------------------------------------------------------------------------------
/// Import position from stream. 
///
/// \param	aInStream		input file stream
///----------------------------------------------------------------------------------------------------
inline void MeshPoint::importPosition( std::istream & aStreamIn )
{
	aStreamIn >> mPosition;
}

///----------------------------------------------------------------------------------------------------
/// Adds PointOnMesh to stream.
///
/// \param [in,out]	aStreamOut	The stream out. 
/// \param	aPointOnMesh		The point on mesh. 
///
/// \return	The modified stream.
///----------------------------------------------------------------------------------------------------
inline std::ostream & operator<<( std::ostream &aStreamOut, const MeshPoint &aPointOnMesh )
{
	aStreamOut << aPointOnMesh.mPosition << aPointOnMesh.mNormal << aPointOnMesh.mTangent;
	// Binormal is stored, it will be recalculated
	aStreamOut.write( reinterpret_cast< const char * >( &aPointOnMesh.mUCoordinate ), sizeof( Real ) );
	aStreamOut.write( reinterpret_cast< const char * >( &aPointOnMesh.mVCoordinate ), sizeof( Real ) );
	return aStreamOut;
}

///----------------------------------------------------------------------------------------------------
/// Pops PointOnMesh from stream.
///
/// \param [in,out]	aStreamIn		The stream in. 
/// \param [in,out]	aPointOnMesh	The point on mesh. 
///
/// \return	The modified stream.
///----------------------------------------------------------------------------------------------------
inline std::istream & operator>>( std::istream & aStreamIn, MeshPoint & aPointOnMesh )
{
	aStreamIn >> aPointOnMesh.mPosition >> aPointOnMesh.mNormal >> aPointOnMesh.mTangent; 
	// Recalculates binormal
	aPointOnMesh.mBinormal = Vector3D< Real >::crossProduct( aPointOnMesh.mTangent, aPointOnMesh.mNormal );
	aStreamIn.read( reinterpret_cast< char * >( &aPointOnMesh.mUCoordinate ), sizeof( Real ) );
	aStreamIn.read( reinterpret_cast< char * >( &aPointOnMesh.mVCoordinate ), sizeof( Real ) );
	return aStreamIn;
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MESH_POINT_HPP