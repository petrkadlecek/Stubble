#include "MayaOutputGenerator.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

void MayaOutputGenerator::draw() const
{
	// Bind buffers
    GLExt::glBindBuffer( GL_ARRAY_BUFFER_ARB, mVertexBO ); // For vertices data
	GLExt::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB, mIndexBO ); // For indices data
	// Enable arrays
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	// Set position and color data
    glVertexPointer( 3, GL_FLOAT, 7 * sizeof( GLfloat ), 0 ); // specify vertex data array
    glColorPointer( 4, GL_FLOAT, 7 * sizeof( GLfloat ), ( void * )( 3 * sizeof( GLfloat ) ) ); // specify color array
	// Draw
	glDrawElements( GL_TRIANGLES, static_cast< GLsizei >( mIndicesPointer - mIndices ), GL_UNSIGNED_BYTE, 0);
	// Disable arrays
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	// Unbind buffers
	GLExt::glBindBuffer( GL_ARRAY_BUFFER_ARB, 0 ); // For vertices data
	GLExt::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 ); // For indices data
}

void MayaOutputGenerator::recalculateToLocalSpace( const MayaPositionGenerator::GeneratedPosition * aHairSpace )
{
	// For all hair
	const MayaPositionGenerator::GeneratedPosition * rootPosIt = aHairSpace; // Root positions
	GLfloat * posIt = mVertices, * endPos; // Points positions
	Matrix< Real > transform;
	for ( const unsigned __int32 * ptsCountIt = mPointsCount, *ptsCountEnd = mPointsCount + mHairCount; 
		ptsCountIt != ptsCountEnd; ++ptsCountIt, ++rootPosIt )
	{
		rootPosIt->mCurrentPosition.getLocalTransformMatrix( transform );
		// For every point of single hair
		for ( endPos = posIt + *ptsCountIt; posIt != endPos; posIt += 4 ) // Jumps RGBA
		{
			// Make position vector from posIt
			Vector3D< Real > pos( static_cast< Real >( *posIt ),
								  static_cast< Real >( *( posIt + 1 ) ),
								  static_cast< Real >( *( posIt + 2 ) ) );
			// Transform point
			pos.transformAsPoint( transform );
			// Copy data from vector to posIt
			*posIt = static_cast< MayaTypes::PositionType >( pos.x );
			*( ++posIt ) = static_cast< MayaTypes::PositionType >( pos.y );
			*( ++posIt ) = static_cast< MayaTypes::PositionType >( pos.z );
		}
	}
	killVBO();
	rebuildVBO();
}

void MayaOutputGenerator::recalculateToWorldSpace( const MayaPositionGenerator::GeneratedPosition * aHairSpace )
{
	// For all hair
	const MayaPositionGenerator::GeneratedPosition * rootPosIt = aHairSpace; // Root positions
	GLfloat * posIt = mVertices, * endPos; // Points positions
	Matrix< Real > transform;
	for ( const unsigned __int32 * ptsCountIt = mPointsCount, *ptsCountEnd = mPointsCount + mHairCount; 
		ptsCountIt != ptsCountEnd; ++ptsCountIt, ++rootPosIt )
	{
		rootPosIt->mCurrentPosition.getWorldTransformMatrix( transform );
		// For every point of single hair
		for ( endPos = posIt + *ptsCountIt; posIt != endPos; posIt += 4 ) // Jumps RGBA
		{
			// Make position vector from posIt
			Vector3D< Real > pos( static_cast< Real >( *posIt ),
								  static_cast< Real >( *( posIt + 1 ) ),
								  static_cast< Real >( *( posIt + 2 ) ) );
			// Transform point
			pos.transformAsPoint( transform );
			// Copy data from vector to posIt
			*posIt = static_cast< MayaTypes::PositionType >( pos.x );
			*( ++posIt ) = static_cast< MayaTypes::PositionType >( pos.y );
			*( ++posIt ) = static_cast< MayaTypes::PositionType >( pos.z );
		}
	}
	killVBO();
	rebuildVBO();
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble