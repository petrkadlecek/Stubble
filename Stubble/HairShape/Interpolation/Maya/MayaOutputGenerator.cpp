#include "MayaOutputGenerator.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

namespace Maya
{

void MayaOutputGenerator::draw()
{
	if ( mDirty )
	{
		rebuildVBO();
	}
	// Enable depth-buffer
	glEnable( GL_DEPTH_TEST );
	// Enable blending
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
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
	// Disable blending
	glEnable( GL_BLEND );
	// Disable depth-buffer
	glDisable( GL_DEPTH_TEST );
	// Unbind buffers
	GLExt::glBindBuffer( GL_ARRAY_BUFFER_ARB, 0 ); // For vertices data
	GLExt::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 ); // For indices data
}

} // namespace Maya

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble