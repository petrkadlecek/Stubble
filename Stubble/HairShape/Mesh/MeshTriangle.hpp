#ifndef STUBBLE_MESH_TRIANGLE_HPP
#define STUBBLE_MESH_TRIANGLE_HPP

#include "Common\CommonConstants.hpp"
#include "Common\CommonFunctions.hpp"

#include <sstream>
#include <string>
#include <vector>

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// MeshTriangle. 
///----------------------------------------------------------------------------------------------------
class MeshTriangle
{
public:
	inline MeshTriangle(); ///< empty constructor used in deserialization

	///----------------------------------------------------------------------------------------------------
	/// Constructor. 
	///
	/// \param	faceID			identifier for the face
	/// \param	localVertex1ID	identifier for the local vertex 1
	/// \param	localVertex2ID	identifier for the local vertex 2
	/// \param	localVertex3ID	identifier for the local vertex 3
	///----------------------------------------------------------------------------------------------------
	inline MeshTriangle( const unsigned __int32 aFaceID, const unsigned __int32 aLocalVertex1ID, 
		const unsigned __int32 aLocalVertex2ID, const unsigned __int32 aLocalVertex3D );

	///----------------------------------------------------------------------------------------------------
	/// Gets the identifier for the face
	///
	/// \return	Face ID
	///----------------------------------------------------------------------------------------------------
	inline const unsigned __int32 & getFaceID() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the identifier for the local vertex 1
	///
	/// \return	ID for local vertex 1
	///----------------------------------------------------------------------------------------------------
	inline const unsigned __int32 & getLocalVertex1() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the identifier for the local vertex 2
	///
	/// \return	ID for local vertex 2
	///----------------------------------------------------------------------------------------------------
	inline const unsigned __int32 & getLocalVertex2() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the identifier for the local vertex 3
	///
	/// \return	ID for local vertex 3
	///----------------------------------------------------------------------------------------------------
	inline const unsigned __int32 & getLocalVertex3() const;

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

	///< Identifier for the face
	unsigned __int32 mFaceID;

	///< Identifier for the local vertex 1
	unsigned __int32 mLocalVertex1ID;
	
	///< Identifier for the local vertex 2
	unsigned __int32 mLocalVertex2ID;

	///< Identifier for the local vertex 3
	unsigned __int32 mLocalVertex3ID;

};

///----------------------------------------------------------------------------------------------------
/// Defines an alias representing the triangles .
///----------------------------------------------------------------------------------------------------
typedef std::vector< MeshTriangle > MeshTriangles;

// inline functions implementation

inline MeshTriangle::MeshTriangle()	
{
}

inline MeshTriangle::MeshTriangle( const unsigned __int32 aFaceID, const unsigned __int32 aLocalVertex1ID,
	const unsigned __int32 aLocalVertex2ID, const unsigned __int32 aLocalVertex3ID ):
	mFaceID( aFaceID ),
	mLocalVertex1ID ( aLocalVertex1ID ),
	mLocalVertex2ID ( aLocalVertex2ID ),
	mLocalVertex3ID ( aLocalVertex3ID )
{
}

inline const unsigned __int32 & MeshTriangle::getFaceID() const
{
	return mFaceID;
}

inline const unsigned __int32 & MeshTriangle::getLocalVertex1() const
{
	return mLocalVertex1ID;
}

inline const unsigned __int32 & MeshTriangle::getLocalVertex2() const
{
	return mLocalVertex2ID;
}

inline const unsigned __int32 & MeshTriangle::getLocalVertex3() const
{
	return mLocalVertex3ID;
}


inline std::string MeshTriangle::serialize() const
{
	std::ostringstream oss;
	oss << Stubble::serialize< unsigned __int32 >( mFaceID )
		<< Stubble::serialize< unsigned __int32 >( mLocalVertex1ID )
		<< Stubble::serialize< unsigned __int32 >( mLocalVertex2ID )
		<< Stubble::serialize< unsigned __int32 >( mLocalVertex3ID );		
	return oss.str();
}

inline size_t MeshTriangle::deserialize( const std::string &aStr, size_t aPos )
{
	mFaceID = Stubble::deserialize< unsigned __int32 >( aStr, aPos );
	mLocalVertex1ID = Stubble::deserialize< unsigned __int32 >( aStr, aPos );
	mLocalVertex2ID = Stubble::deserialize< unsigned __int32 >( aStr, aPos );
	mLocalVertex3ID = Stubble::deserialize< unsigned __int32 >( aStr, aPos );
	return aPos;
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MESH_TRIANGLE_HPP