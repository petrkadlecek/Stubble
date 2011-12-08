#ifndef STUBBLE_MAYA_TRIANGLE_HPP
#define STUBBLE_MAYA_TRIANGLE_HPP

#include "Common\CommonConstants.hpp"
#include "Common\CommonFunctions.hpp"

#include <vector>

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// Stores indices to maya mesh triangle internal representation.
/// Triangle is stored as mesh face id and 3 face-local identifiers for vertices.
///----------------------------------------------------------------------------------------------------
class MayaTriangle
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Creates maya triangle.
	///
	/// \param	aFaceID				Identifier for the face
	/// \param	aLocalVertex1ID		Face-local identifier for the vertex 1
	/// \param	aLocalVertex2ID		Face-local identifier for the vertex 2
	/// \param	aLocalVertex3ID		Face-local identifier for the vertex 3
	///----------------------------------------------------------------------------------------------------
	inline MayaTriangle( const unsigned __int32 aFaceID, const unsigned __int32 aLocalVertex1ID, 
		const unsigned __int32 aLocalVertex2ID, const unsigned __int32 aLocalVertex3D );

	///----------------------------------------------------------------------------------------------------
	/// Gets the identifier for the face
	///
	/// \return	Face ID
	///----------------------------------------------------------------------------------------------------
	inline const unsigned __int32 & getFaceID() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the face-local identifier for the vertex 1
	///
	/// \return	Face-local identifier for the vertex 1
	///----------------------------------------------------------------------------------------------------
	inline const unsigned __int32 & getLocalVertex1() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the face-local identifier for the vertex 2
	///
	/// \return	Face-local identifier for the vertex 2
	///----------------------------------------------------------------------------------------------------
	inline const unsigned __int32 & getLocalVertex2() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the face-local identifier for the vertex 3
	///
	/// \return	Face-local identifier for the vertex 3
	///----------------------------------------------------------------------------------------------------
	inline const unsigned __int32 & getLocalVertex3() const;

private:

	unsigned __int32 mFaceID; ///< Identifier for the face

	unsigned __int32 mLocalVertex1ID;   ///< Face-local identifier for the vertex 1
	
	unsigned __int32 mLocalVertex2ID;	///< Face-local identifier for the vertex 2

	unsigned __int32 mLocalVertex3ID;	///< Face-local identifier for the vertex 3

};

///----------------------------------------------------------------------------------------------------
/// Defines an alias representing the array of maya triangles .
///----------------------------------------------------------------------------------------------------
typedef std::vector< MayaTriangle > MayaTriangles;

// inline functions implementation

inline MayaTriangle::MayaTriangle( const unsigned __int32 aFaceID, const unsigned __int32 aLocalVertex1ID,
	const unsigned __int32 aLocalVertex2ID, const unsigned __int32 aLocalVertex3ID ):
	mFaceID( aFaceID ),
	mLocalVertex1ID ( aLocalVertex1ID ),
	mLocalVertex2ID ( aLocalVertex2ID ),
	mLocalVertex3ID ( aLocalVertex3ID )
{
}

inline const unsigned __int32 & MayaTriangle::getFaceID() const
{
	return mFaceID;
}

inline const unsigned __int32 & MayaTriangle::getLocalVertex1() const
{
	return mLocalVertex1ID;
}

inline const unsigned __int32 & MayaTriangle::getLocalVertex2() const
{
	return mLocalVertex2ID;
}

inline const unsigned __int32 & MayaTriangle::getLocalVertex3() const
{
	return mLocalVertex3ID;
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MAYA_TRIANGLE_HPP