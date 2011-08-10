#ifndef STUBBLE_TRIANGLE_HPP
#define STUBBLE_TRIANGLE_HPP

#include "HairShape\Mesh\MeshPoint.hpp"

#include <vector>

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// Triangle. 
///----------------------------------------------------------------------------------------------------
class Triangle
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Constructor. 
	///
	/// \param	mVertex1	The first triangle Vertex. 
	/// \param	mVertex2	The second triangle Vertex. 
	/// \param	mVertex3	The third triangle Vertex. 
	///----------------------------------------------------------------------------------------------------
	inline Triangle( const MeshPoint & mVertex1, const MeshPoint & mVertex2, const MeshPoint & mVertex3 );

	///----------------------------------------------------------------------------------------------------
	/// Constructor realized from stream
	///
	/// \param	aInStream	input file stream
	///----------------------------------------------------------------------------------------------------
	inline Triangle( std::istream & aInStream );

	///----------------------------------------------------------------------------------------------------
	/// Gets the Vertex 1. 
	///
	/// \return	The Vertex 1. 
	///----------------------------------------------------------------------------------------------------
	inline const MeshPoint & getVertex1() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the Vertex 2. 
	///
	/// \return	The Vertex 2. 
	///----------------------------------------------------------------------------------------------------
	inline const MeshPoint & getVertex2() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the Vertex 3. 
	///
	/// \return	The Vertex 3. 
	///----------------------------------------------------------------------------------------------------
	inline const MeshPoint & getVertex3() const;

private:

	MeshPoint mVertices[ 3 ]; ///< The Vertices

};

///----------------------------------------------------------------------------------------------------
/// Defines an alias representing the triangles .
///----------------------------------------------------------------------------------------------------
typedef std::vector< Triangle > Triangles;

// inline functions implementation

inline Triangle::Triangle( const MeshPoint & mVertex1, const MeshPoint & mVertex2, const MeshPoint & mVertex3 )
{
	mVertices[ 0 ] = mVertex1;
	mVertices[ 1 ] = mVertex2;
	mVertices[ 2 ] = mVertex3;
}

inline Triangle::Triangle( std::istream & aInStream )
{
	aInStream >> mVertices[ 0 ] >> mVertices[ 1 ] >> mVertices[ 2 ];
}

inline const MeshPoint & Triangle::getVertex1() const
{
	return mVertices[ 0 ];
}

inline const MeshPoint & Triangle::getVertex2() const
{
	return mVertices[ 1 ];
}

inline const MeshPoint & Triangle::getVertex3() const
{
	return mVertices[ 2 ];
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_TRIANGLE_HPP