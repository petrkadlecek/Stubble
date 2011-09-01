#ifndef STUBBLE_MESH_TRIANGLE_HPP
#define STUBBLE_MESH_TRIANGLE_HPP

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

	///----------------------------------------------------------------------------------------------------
	/// Constructor. 
	///
	/// \param	faceID			identifier for the face
	/// \param	localVertex1ID	identifier for the local vertex 1
	/// \param	localVertex2ID	identifier for the local vertex 2
	/// \param	localVertex3ID	identifier for the local vertex 3
	///----------------------------------------------------------------------------------------------------
	inline MeshTriangle( const unsigned __int32 faceID, const unsigned __int32 localVertex1ID, 
		const unsigned __int32 localVertex2ID, const unsigned __int32 localVertex3ID );

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

inline MeshTriangle::MeshTriangle( const unsigned __int32 faceID, const unsigned __int32 localVertex1ID,
	const unsigned __int32 localVertex2ID, const unsigned __int32 localVertex3ID):
	mFaceID( faceID ),
	mLocalVertex1ID ( mLocalVertex1ID ),
	mLocalVertex2ID ( mLocalVertex2ID ),
	mLocalVertex3ID ( mLocalVertex3ID )
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

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MESH_TRIANGLE_HPP