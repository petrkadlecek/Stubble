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

Mesh::Mesh( const Triangles &aTriangles, bool aCalculateDerivatives )
{
	// Load triangles count
	mTriangles = aTriangles;
	// Calculate derivatives if needed
	if ( aCalculateDerivatives )
	{
		for ( Triangles::iterator it = mTriangles.begin(); it != mTriangles.end(); ++it )
		{
			it->recalculateDerivatives();
		}
	}
}

void Mesh::exportMesh( std::ostream & aOutputStream ) const
{
	// Export triangles count
	unsigned __int32 size = static_cast< unsigned __int32 >( mTriangles.size() );
	aOutputStream.write( reinterpret_cast< const char *>( &size ), sizeof( unsigned __int32 ) );
	// Export triangles
	for ( Triangles::const_iterator it = mTriangles.begin(); it != mTriangles.end(); ++it )
	{
		it->exportTriangle( aOutputStream );
	}
}

void Mesh::importMesh( std::istream & aInputStream )
{
	// Load triangles count
	unsigned __int32 trianglesCount;
	aInputStream.read( reinterpret_cast< char * >( &trianglesCount ), sizeof( unsigned __int32 ) );
	assert( trianglesCount == mTriangles.size() );
	mBoundingBox.clear();
	// Load all triangles
	for ( Triangles::iterator it = mTriangles.begin(); it != mTriangles.end(); ++it )
	{
		*it = Triangle( aInputStream );
		// Expand bbox
		mBoundingBox.expand( it->getVertex1().getPosition() );
		mBoundingBox.expand( it->getVertex2().getPosition() );
		mBoundingBox.expand( it->getVertex3().getPosition() );
	}
}

} // namespace HairShape

} // namespace Stubble