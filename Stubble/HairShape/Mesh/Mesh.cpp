#include "Mesh.hpp"

namespace Stubble
{

namespace HairShape
{

inline Mesh::Mesh( std::istream & aInStream )
{
	// Load triangles count
	unsigned __int32 trianglesCount;
	aInStream.read( reinterpret_cast< char * >( trianglesCount ), sizeof( unsigned __int32 ) );
	mTriangles.resize( trianglesCount );
	// Load all triangles
	for ( Triangles::iterator it = mTriangles.begin(); it != mTriangles.end(); ++it )
	{
		*it = Triangle( aInStream );
	}
	// Bounding box will not be calculated, it is not required in 3Delight
}

} // namespace HairShape

} // namespace Stubble