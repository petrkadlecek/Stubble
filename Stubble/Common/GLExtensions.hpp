#ifndef STUBBLE_GL_EXTENSIONS_HPP
#define STUBBLE_GL_EXTENSIONS_HPP

#ifdef _MSC_VER
#  define WIN32_LEAN_AND_MEAN 1
#  include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/GLU.h>
#include <gl/glext.h>

namespace Stubble
{
	///-------------------------------------------------------------------------------------------------
	/// OpenGL extentensions. Initialize OpenGL extensions functions.
	///-------------------------------------------------------------------------------------------------
	class GLExt
	{
	public:

		///-------------------------------------------------------------------------------------------------
		/// Static constructor. 
		///-------------------------------------------------------------------------------------------------
		static void init();

		///-------------------------------------------------------------------------------------------------
		/// Query if this object is inited. 
		///
		/// \return	true if inited, false if not. 
		///-------------------------------------------------------------------------------------------------
		static inline bool isInited();

		///-------------------------------------------------------------------------------------------------
		/// Checks whether the extension is available. 
		///
		/// \param	aExtensionName	Name of the extension. 
		///
		/// \return	true if the extension is available, false if it is not. 
		///-------------------------------------------------------------------------------------------------
		static bool checkExtension( const char *aExtensionName );

		static PFNGLGENBUFFERSPROC glGenBuffers;   ///< Pointer to glGenBuffers method

		static PFNGLBINDBUFFERARBPROC glBindBuffer;	///< Pointer to glBindBuffer method

		static PFNGLBUFFERDATAARBPROC glBufferData;	///< Pointer to glBufferData method

		static PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;  ///< Pointer to glDeleteBuffers method
		
	private:

		static bool mIsInited;  ///< true if gl. extensions were initialized

		///-------------------------------------------------------------------------------------------------
		/// Private constructor, will never be called
		///-------------------------------------------------------------------------------------------------
		GLExt();
	};

// inline methods implementation

inline bool GLExt::isInited()
{
	return mIsInited;
}

} // namespace Stubble

#endif // STUBBLE_GL_EXTENSIONS_HPP