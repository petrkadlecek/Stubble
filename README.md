Stubble
=======

Stubble is a plugin for Autodesk Maya providing user-friendly virtual hairstyling, i.e.,
creation, editing, and animation of hair or fur on 3D models. It is a student software
project developed on Faculty of Mathematics and Physics, Charles University in Prague,
managed by Computer Graphics Group - Charles University (http://cgg.mff.cuni.cz).
It is written mainly in C++ with bits of MEL scripting for proper Maya binding.

Main features
-------------

 *  Low disk space footprint of the generated data at render time
 *  Procedural hair generation and interpolation via editable guide hairs
 *  Hair dynamics via Maya NURBS curves
 *  Wide user control over the output using textures
 *  Intuitive interface with real-life like tools, e.g., scissors or brush
 *  Large variety of input devices, e.g., mouse, tablet, or a haptic device

Requirements
------------

The provided source code was built and tested using Microsoft Visual Studio 2010. It was
run and tested in Maya 2011. However, it should run just fine under newer versions.

### Third party software

To successfully build Stubble several third party software packages are needed which are
not distributed with the source code. The solution is configured to look for them in the
`external` directory one level above the solution directory or in the standard paths.

 *  __Autodesk Maya SDK__ - software development kit distributed along with Autodesk Maya
    located in the same location as maya.exe.
 *  __3Delight Studio Pro (9.5 or higher) 64-bit (includes plug-in for Maya)__ - 
    RenderMan-compliant rendering solution for Autodesk Maya available at
    http://www.3delight.com/downloads/free/3delight-9.0.84-setup-x64.exe
    with performance limited license.
 *  __Qt library 4.7__ - user interface in Autodesk Maya is based on Qt UI framework,
    the framework is used mainly for mouse interaction features. Required version is 4.7
    or newer. Available at http://qt-project.org/downloads
 *  __CHAI 3D 2.0.0__ - CHAI 3D is an open source set of C++ libraries for computer haptics,
    visualization and interactive real-time simulation.
    Available at http://www.chai3d.org/download.html
 *  __libnoise 1.0.0__ - A portable, open-source, coherent noise-generating library for C++.
    Available at http://libnoise.sourceforge.net/downloads/index.html
 *  __zlib 1.2.5__ - Zlib is designed to be a free, general-purpose, legally unencumbered
    lossless data-compression library. Required version is 1.2.5 or newer.
    Available at http://zlib.net/
 *  __zipstream 1.0__ - zipstream is c++ iostream wrapper for the zlib library.
    Available at http://www.codeproject.com/KB/stl/zipstream.aspx
 *  __Wintab__ - Wintab is a graphics tablet software development kit
    available at http://www.wacomeng.com/
 *  __newmat 10__ - Portable, open-source library for large matrix manipulations.
    Available at http://www.robertnz.net/download.html. Probably the best choice to use
    without the need for platform optimized LAPACK and BLAS.
 *  __OpenGL 2.0__ - basicaly the same version as used by Maya. Optimized version is provided
    by graphics card vendors. For more information see http://www.opengl.org. The code present
    in Stubble should be portable to newer OpenGL versions.
