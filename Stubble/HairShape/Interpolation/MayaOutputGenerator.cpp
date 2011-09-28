#include "MayaOutputGenerator.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

void MayaOutputGenerator::draw()
{
	if ( mDirty )
	{
		rebuildVBO();
	}
	// Bind vertex buffer 
	GLExt::glBindBuffer( GL_ARRAY_BUFFER_ARB, mVertexBO ); // For vertices data
	// Enable vertex arrays
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	// Set vertex arrays
    glVertexPointer( 3, GL_FLOAT, 7 * sizeof( GLfloat ), ( void * )( 4 * sizeof( GLfloat ) ) ); // specify vertex data array
    glColorPointer( 4, GL_FLOAT, 7 * sizeof( GLfloat ), 0 ); // specify color array
	// Bind indices buffer and draw
	GLExt::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB, mIndexBO ); // For indices data
	glDrawElements( GL_TRIANGLES, static_cast< GLsizei >( mIndicesPointer - mIndices ), GL_UNSIGNED_INT,  0 );
	// Disable vertex arrays
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	// Unbind buffers
	GLExt::glBindBuffer( GL_ARRAY_BUFFER_ARB, 0 ); // For vertices data
	GLExt::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 ); // For indices data
}

void MayaOutputGenerator::recalculateToLocalSpace( const MayaPositionGenerator::GeneratedPosition * aHairSpace )
{
	mDirty = true;
	// For all hair
	const MayaPositionGenerator::GeneratedPosition * rootPosIt = aHairSpace; // Root positions
	GLfloat * posIt = mVertices, * endPos; // Points positions
	Matrix< Real > transform;
	for ( const unsigned __int32 * ptsCountIt = mPointsCount, *ptsCountEnd = mPointsCount + mHairCount; 
		ptsCountIt != ptsCountEnd; ++ptsCountIt, ++rootPosIt )
	{
		rootPosIt->mCurrentPosition.getLocalTransformMatrix( transform );
		// For every point of single hair
		for ( endPos = posIt + *ptsCountIt * 7; posIt != endPos; ++posIt ) 
		{
			posIt += 4; // Jumps RGBA
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
}

void MayaOutputGenerator::recalculateToWorldSpace( const MayaPositionGenerator::GeneratedPosition * aHairSpace )
{
	mDirty = true;
	// For all hair
	const MayaPositionGenerator::GeneratedPosition * rootPosIt = aHairSpace; // Root positions
	GLfloat * posIt = mVertices, * endPos; // Points positions
	Matrix< Real > transform;
	for ( const unsigned __int32 * ptsCountIt = mPointsCount, *ptsCountEnd = mPointsCount + mHairCount; 
		ptsCountIt != ptsCountEnd; ++ptsCountIt, ++rootPosIt )
	{
		rootPosIt->mCurrentPosition.getWorldTransformMatrix( transform );
		// For every point of single hair
		for ( endPos = posIt + *ptsCountIt * 7; posIt != endPos; ++posIt ) 
		{
			posIt += 4; // Jumps RGBA
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
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble