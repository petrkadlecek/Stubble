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
	inline MeshTriangle( const int faceID, const int localVertex1ID, const int localVertex2ID, const int localVertex3ID );

	///----------------------------------------------------------------------------------------------------
	/// Gets the identifier for the face
	///
	/// \return	Face ID
	///----------------------------------------------------------------------------------------------------
	inline const int & getFaceID() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the identifier for the local vertex 1
	///
	/// \return	ID for local vertex 1
	///----------------------------------------------------------------------------------------------------
	inline const int & getLocalVertex1() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the identifier for the local vertex 2
	///
	/// \return	ID for local vertex 2
	///----------------------------------------------------------------------------------------------------
	inline const int & getLocalVertex2() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the identifier for the local vertex 3
	///
	/// \return	ID for local vertex 3
	///----------------------------------------------------------------------------------------------------
	inline const int & getLocalVertex3() const;

private:

	///< Identifier for the face
	int mFaceID;

	///< Identifier for the local vertex 1
	int mLocalVertex1ID;
	
	///< Identifier for the local vertex 2
	int mLocalVertex2ID;

	///< Identifier for the local vertex 3
	int mLocalVertex3ID;

};

///----------------------------------------------------------------------------------------------------
/// Defines an alias representing the triangles .
///----------------------------------------------------------------------------------------------------
typedef std::vector< MeshTriangle > MeshTriangles;

// inline functions implementation

inline MeshTriangle::MeshTriangle( const int faceID, const int localVertex1ID,
	const int localVertex2ID, const int localVertex3ID):
	mFaceID( faceID ),
	mLocalVertex1ID ( mLocalVertex1ID ),
	mLocalVertex2ID ( mLocalVertex2ID ),
	mLocalVertex3ID ( mLocalVertex3ID )
{
}

inline const int & MeshTriangle::getFaceID() const
{
	return mFaceID;
}

inline const int & MeshTriangle::getLocalVertex1() const
{
	return mLocalVertex1ID;
}

inline const int & MeshTriangle::getLocalVertex2() const
{
	return mLocalVertex2ID;
}

inline const int & MeshTriangle::getLocalVertex3() const
{
	return mLocalVertex3ID;
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MESH_TRIANGLE_HPP