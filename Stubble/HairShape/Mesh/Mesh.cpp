#include "Mesh.hpp"

#include "Common/StubbleException.hpp"

namespace Stubble
{

namespace HairShape
{

Mesh::Mesh( std::istream & aInStream, bool aPositionsOnly )
{
	// Load triangles count
	unsigned __int32 trianglesCount;
	aInStream.read( reinterpret_cast< char * >( &trianglesCount ), sizeof( unsigned __int32 ) );
	mTriangles.resize( trianglesCount );
	// Load all triangles
	for ( Triangles::iterator it = mTriangles.begin(); it != mTriangles.end(); ++it )
	{
		*it = Triangle( aInStream, aPositionsOnly );
	}
	// Bounding box will not be calculated, it is not required in 3Delight
}

void Mesh::copyTextureCoordinates( const Mesh & aMeshWithTextureCoordinates )
{
	if ( aMeshWithTextureCoordinates.mTriangles.size() != mTriangles.size() )
	{
		throw StubbleException( " Mesh::CopyTextureCoordinates : input mesh must have same size ! " );
	}
	// For every triangle
	Triangles::const_iterator sourceIt = aMeshWithTextureCoordinates.mTriangles.begin();
	for ( Triangles::iterator destIt = mTriangles.begin(); destIt != mTriangles.end(); ++destIt, ++sourceIt )
	{
		// Copies texture coordinates from aMeshWithTextureCoordinates, but keeps original position
		*destIt = Triangle( MeshPoint( destIt->getVertex1().getPosition(), 
										sourceIt->getVertex1().getUCoordinate(), 
										sourceIt->getVertex1().getVCoordinate() ),
							MeshPoint( destIt->getVertex2().getPosition(), 
										sourceIt->getVertex2().getUCoordinate(), 
										sourceIt->getVertex2().getVCoordinate() ),
							MeshPoint( destIt->getVertex3().getPosition(), 
										sourceIt->getVertex3().getUCoordinate(), 
										sourceIt->getVertex3().getVCoordinate() ) );
	}
}

} // namespace HairShape

} // namespace Stubble