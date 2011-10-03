#include "Mesh.hpp"

#include "Common/StubbleException.hpp"

namespace Stubble
{

namespace HairShape
{

Mesh::Mesh( std::istream & aInStream, bool aCalculateDerivatives )
{
	// Load triangles count
	unsigned __int32 trianglesCount;
	aInStream.read( reinterpret_cast< char * >( &trianglesCount ), sizeof( unsigned __int32 ) );
	mTriangles.resize( trianglesCount );
	// Load all triangles
	for ( Triangles::iterator it = mTriangles.begin(); it != mTriangles.end(); ++it )
	{
		*it = Triangle( aInStream, aCalculateDerivatives );
	}
	// Bounding box will not be calculated, it is not required in 3Delight
}

} // namespace HairShape

} // namespace Stubble