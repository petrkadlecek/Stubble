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
	inline Vector3D< double > toWorld( const Vector3D< double > aLocalVector ) const;
	
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

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MESH_POINT_HPP