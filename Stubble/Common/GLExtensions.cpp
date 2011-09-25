#include "GLExtensions.hpp"

#include "StubbleException.hpp"

namespace Stubble
{

PFNGLGENBUFFERSPROC GLExt::glGenBuffers = 0;

PFNGLBINDBUFFERARBPROC GLExt::glBindBuffer = 0;

PFNGLBUFFERDATAARBPROC GLExt::glBufferData = 0;

PFNGLDELETEBUFFERSARBPROC GLExt::glDeleteBuffers = 0;
		
bool GLExt::mIsInited = false;

void GLExt::init()
{
	if ( !checkExtension("GL_ARB_vertex_buffer_object") )
	{
		throw StubbleException(" GLExt::init : extension \"GL_ARB_vertex_buffer_object\" is not available ");
	}
	glGenBuffers = reinterpret_cast< PFNGLGENBUFFERSPROC >( wglGetProcAddress("glGenBuffersARB") );
	glBindBuffer = reinterpret_cast< PFNGLBINDBUFFERARBPROC >( wglGetProcAddress("glBindBufferARB") );
	glBufferData = reinterpret_cast< PFNGLBUFFERDATAARBPROC >( wglGetProcAddress("glBufferDataARB") );
	glDeleteBuffers = reinterpret_cast< PFNGLDELETEBUFFERSARBPROC >( wglGetProcAddress("glDeleteBuffersARB") );
	mIsInited = true;
}

bool GLExt::checkExtension( const char *aExtensionName )
{
	// Get ext. name length
	size_t extNameLen = strlen( aExtensionName ), n = 0;
	// Get extensions list
	const char * p = (const char *) glGetString( GL_EXTENSIONS );
	if ( p == 0 )
	{
		throw StubbleException(" GLExt::checkExtension : no extensions are available ");
	}
	// End of extensions list, iterator
	const char * end = p + strlen( p ), * i; 
	// For all extensions
	while ( p < end )
	{
		// Get next extension position
		for ( i = p; *i != ' ' && i < end; ++i );
		n = i - p;
		// Compare current ext. name with selected extension name
		if ( ( extNameLen == n ) && ( strncmp( aExtensionName, p, n ) == 0 ) )
		{
			return true;
		}
		// Moves to next extension
		p += n + 1;
	}
	return false;
}

} // namespace Stubble