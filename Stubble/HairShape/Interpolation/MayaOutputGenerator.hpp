#ifndef STUBBLE_MAYA_OUTPUT_GENERATOR_HPP
#define STUBBLE_MAYA_OUTPUT_GENERATOR_HPP

#include "MayaPositionGenerator.hpp"
#include "OutputGenerator.hpp"

#include "Common/GLExtensions.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Maya output generator types. 
///-------------------------------------------------------------------------------------------------
struct MayaTypes
{
	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of 
	/// the 3D position.
	///-------------------------------------------------------------------------------------------------
	typedef float PositionType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of the color.
	///-------------------------------------------------------------------------------------------------
	typedef float ColorType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of 
	/// the normal.
	///-------------------------------------------------------------------------------------------------
	typedef float NormalType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store width.
	///-------------------------------------------------------------------------------------------------
	typedef float WidthType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of 
	/// the opacity.
	///-------------------------------------------------------------------------------------------------
	typedef float OpacityType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one of the 2 u v coordinates.
	///-------------------------------------------------------------------------------------------------
	typedef float UVCoordinateType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store hair and strand index.
	///-------------------------------------------------------------------------------------------------
	typedef float IndexType;
};

///-------------------------------------------------------------------------------------------------
/// Generator of finished interpolated hair used in Maya plugin.
///-------------------------------------------------------------------------------------------------
class MayaOutputGenerator : public OutputGenerator< MayaTypes >, public MayaTypes
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	inline MayaOutputGenerator();

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	inline ~MayaOutputGenerator();

	///-------------------------------------------------------------------------------------------------
	/// Clears data.
	///-------------------------------------------------------------------------------------------------
	inline void clear();

	///-------------------------------------------------------------------------------------------------
	/// Begins an output of interpolated hair. 
	///
	/// \param	aMaxHairCount	Number of a maximum hair. 
	/// \param	aMaxPointsCount	Number of a maximum points. 
	///-------------------------------------------------------------------------------------------------
	inline void beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount );

	///----------------------------------------------------------------------------------------------------
	/// Ends an output.
	///----------------------------------------------------------------------------------------------------
	inline void endOutput();

	///-------------------------------------------------------------------------------------------------
	/// Begins an output of single interpolated hair. 
	///
	/// \param	aMaxPointsCount	Number of a maximum points on current hair. 
	///-------------------------------------------------------------------------------------------------
	inline void beginHair( unsigned __int32 aMaxPointsCount );

	///-------------------------------------------------------------------------------------------------
	/// Ends an output of single interpolated hair. 
	/// 
	/// \param	aPointsCount	Number of points on finished hair. 
	///-------------------------------------------------------------------------------------------------
	inline void endHair( unsigned __int32 aPointsCount );

	///-------------------------------------------------------------------------------------------------
	/// Return true, if hair generator should duplicate border points (root, tip)
	///
	/// \return	false, MayaOutputGenerator does not require border points duplication.
	///-------------------------------------------------------------------------------------------------
	inline bool getDuplicateBorderPoints() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points positions. 
	///
	/// \return	null if it fails, else return position pointer. 
	///-------------------------------------------------------------------------------------------------
	inline PositionType * positionPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points colors. 
	///
	/// \return	null if it fails, else return color pointer. 
	///-------------------------------------------------------------------------------------------------
	inline ColorType * colorPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points normals. 
	///
	/// \return	null if it fails, else return normal pointer. 
	///-------------------------------------------------------------------------------------------------
	inline NormalType * normalPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points widths. 
	///
	/// \return	null if it fails, else return width pointer. 
	///-------------------------------------------------------------------------------------------------
	inline WidthType * widthPointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points opacity. 
	///
	/// \return	null if it fails, else return opacity pointer. 
	///-------------------------------------------------------------------------------------------------
	inline OpacityType * opacityPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the null pointer. Hair uv coordinates are not supported. 
	///
	/// \return	null
	///-------------------------------------------------------------------------------------------------
	inline UVCoordinateType * hairUVCoordinatePointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the null pointer. Strand uv coordinate are not supported. 
	///
	/// \return	null
	///-------------------------------------------------------------------------------------------------
	inline UVCoordinateType * strandUVCoordinatePointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the null pointer. Hair indices are not supported. 
	///
	/// \return	null
	///-------------------------------------------------------------------------------------------------
	inline IndexType * hairIndexPointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the null pointer. Strand indices are not supported. 
	///
	/// \return	null
	///-------------------------------------------------------------------------------------------------
	inline IndexType * strandIndexPointer();

	///-------------------------------------------------------------------------------------------------
	/// Draws outputed hair.
	///-------------------------------------------------------------------------------------------------
	void draw();

	///-------------------------------------------------------------------------------------------------
	/// Recalculates all hair positions data to local space. 
	///
	/// \author	martin_sik
	/// \date	8.9.2011
	///
	/// \param	aHairSpace	The local space of each hair.
	///-------------------------------------------------------------------------------------------------
	void recalculateToLocalSpace( const MayaPositionGenerator::GeneratedPosition * aHairSpace );

	///-------------------------------------------------------------------------------------------------
	/// Recalculates all hair positions from local to world space 
	/// ( recalculateToLocalSpace must be called first ).
	///
	/// \author	martin_sik
	/// \date	8.9.2011
	///
	/// \param	aHairSpace	The local space of each hair.
	///-------------------------------------------------------------------------------------------------
	void recalculateToWorldSpace( const MayaPositionGenerator::GeneratedPosition * aHairSpace );

private:

	///-------------------------------------------------------------------------------------------------
	/// Rebuilds vertex buffer objects. 
	///-------------------------------------------------------------------------------------------------
	inline void rebuildVBO();

	///-------------------------------------------------------------------------------------------------
	/// Kills vertex buffer objects. 
	///-------------------------------------------------------------------------------------------------
	inline void killVBO();

	unsigned __int32 mMaxHairCount;	///< Maximum number of the hair

	unsigned __int32 mHairCount;	///< Number of the hair

	unsigned __int32 mMaxPointsCount; ///< Number of maximum points per hair

	unsigned __int32 * mPointsCount;  ///< Number of points for each hair

	unsigned __int32 * mPointsCountPointer;	///< The pointer to current hair points count

	MayaTypes::PositionType * mPositionData;   ///< Information describing a position of the hair

	MayaTypes::ColorType * mColorData; ///< Information describing a color of the hair

	MayaTypes::OpacityType * mOpacityData; ///< Information describing a opacity of the hair

	MayaTypes::NormalType * mNormalData; ///< Information describing a normals of the hair points

	MayaTypes::WidthType * mWidthData;  ///< Information describing the width of the hair

	/* GL structures */

	GLfloat * mVertices;	///< The vertices for GL drawing

	GLfloat * mVerticesPointer; ///< The vertices current pointer

	GLuint * mIndices;  ///< The indices for GL drawing

	GLuint * mIndicesPointer;   ///< The indices current pointer

	GLuint mVertexBO;  ///< The vertex bo handle

	GLuint mIndexBO;   ///< The index bo handle

	bool mDirty;	///< true if VBO should be rebuild
};

// inline functions implementation

inline MayaOutputGenerator::MayaOutputGenerator():
	mMaxHairCount( 0 ),
	mHairCount( 0 ),	
	mMaxPointsCount( 0 ),
	mPointsCount( 0 ),  
	mPointsCountPointer( 0 ),	
	mPositionData( 0 ),   
	mColorData( 0 ), 
	mOpacityData( 0 ), 
	mNormalData( 0 ), 
	mWidthData( 0 ),
	mVertices( 0 ),	
	mVerticesPointer( 0 ),
	mIndices( 0 ),  
	mIndicesPointer( 0 ),
	mVertexBO( 0 ),  
	mIndexBO( 0 )  
{
}

inline MayaOutputGenerator::~MayaOutputGenerator()
{
	killVBO();
	clear();
}

inline void MayaOutputGenerator::clear()
{
	delete [] mPointsCount;
	delete [] mPositionData;
	delete [] mColorData;
	delete [] mOpacityData;
	delete [] mNormalData;
	delete [] mWidthData;
	delete [] mVertices;
	delete [] mIndices;
	mMaxHairCount = 0;
	mHairCount = 0;	
	mMaxPointsCount = 0;
	mPointsCount = 0;  
	mPointsCountPointer = 0;	
	mPositionData = 0;   
	mColorData = 0; 
	mOpacityData = 0; 
	mNormalData = 0; 
	mWidthData = 0;  
	mVertices = 0;	
	mVerticesPointer = 0;
	mIndices = 0;  
	mIndicesPointer = 0;
	mVertexBO = 0;  
	mIndexBO = 0;  
}

inline void MayaOutputGenerator::beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount )
{
	mDirty = true;
	// Memory is not sufficient
	if ( mMaxPointsCount < aMaxPointsCount || aMaxHairCount > mMaxHairCount )
	{
		try
		{
			// Free all memory
			clear();
			// Sets new limits
			mMaxPointsCount = aMaxPointsCount;
			mMaxHairCount = aMaxHairCount;
			//		( XYZ + RGBA ) * totalPoints * 2 vertices for each point
			unsigned __int32 verticesCount =  7 * mMaxHairCount * aMaxPointsCount * 2; 
			//		2 triangles for each hair segment
			unsigned __int32 indicesCount = 6 * mMaxHairCount * ( aMaxPointsCount - 1 );	
			// Allocate sufficient memory for GL drawing
			mVertices = new GLfloat[ verticesCount ];
			mIndices = new GLuint[ indicesCount ];
			// Allocate memory for points count
			mPointsCount = new unsigned __int32[ aMaxHairCount ];
			// Allocate memory for color, opacity and position data
			mPositionData = new PositionType[ mMaxPointsCount * 3 ];
			mColorData = new ColorType[ mMaxPointsCount * 3 ];
			mOpacityData = new OpacityType[ mMaxPointsCount * 3 ];
			mNormalData = new NormalType[ mMaxPointsCount * 3 ];
			mWidthData = new WidthType[ mMaxPointsCount ];
		}
		catch( ... )
		{
			clear();
			throw;
		}
	}
	// Reset
	mVerticesPointer = mVertices;
	mIndicesPointer = mIndices;
	mHairCount = 0;
	mPointsCountPointer = mPointsCount;
}

inline void MayaOutputGenerator::endOutput()
{
	/* EMPTY */
}

inline void MayaOutputGenerator::beginHair( unsigned __int32 aMaxPointsCount )
{
	/* EMPTY */
}

inline void MayaOutputGenerator::endHair( unsigned __int32 aPointsCount )
{
	// Width multiplier constant
	const GLfloat wm = 0.05f;
	// We will calculate triangles from hair points
	for ( unsigned __int32 i = 0; i < aPointsCount; ++i )
	{
		// 2 points will be generated for each 1 point of hair
		// Color of first point
		*( mVerticesPointer++ ) = mColorData[ i * 3 ]; // R
		*( mVerticesPointer++ ) = mColorData[ i * 3 + 1 ]; // G
		*( mVerticesPointer++ ) = mColorData[ i * 3 + 2 ]; // B
		*( mVerticesPointer++ ) = mOpacityData[ i * 3 ]; // A
		// Position of first point
		*( mVerticesPointer++ ) = mPositionData[ i * 3 ] + mWidthData[ i ] * mNormalData[ i * 3 ] * wm;
		*( mVerticesPointer++ ) = mPositionData[ i * 3 + 1 ] + mWidthData[ i ] * mNormalData[ i * 3 + 1 ] * wm;
		*( mVerticesPointer++ ) = mPositionData[ i * 3 + 2 ] + mWidthData[ i ] * mNormalData[ i * 3 + 2 ] * wm;
		// Color of second point will be the same as first point
		*( mVerticesPointer++ ) = mColorData[ i * 3 ]; // R
		*( mVerticesPointer++ ) = mColorData[ i * 3 + 1 ]; // G
		*( mVerticesPointer++ ) = mColorData[ i * 3 + 2 ]; // B
		*( mVerticesPointer++ ) = mOpacityData[ i * 3 ]; // A
		// Position of second point ( displaced in opposite direction )
		*( mVerticesPointer++ ) = mPositionData[ i * 3 ] - mWidthData[ i ] * mNormalData[ i * 3 ] * wm;
		*( mVerticesPointer++ ) = mPositionData[ i * 3 + 1 ] - mWidthData[ i ] * mNormalData[ i * 3 + 1 ] * wm;
		*( mVerticesPointer++ ) = mPositionData[ i * 3 + 2 ] - mWidthData[ i ] * mNormalData[ i * 3 + 2 ] * wm;
	}
	// Calculate indices ( for each segment 2 )
	GLuint startIndex = mIndicesPointer == mIndices ? 0 : *( mIndicesPointer - 1 ) + 1;
	for ( unsigned __int32 i = 0; i < ( aPointsCount - 1 ); ++i )
	{
		// First triangle
		*( mIndicesPointer++ ) = startIndex + 3;
		*( mIndicesPointer++ ) = startIndex + 2;
		*( mIndicesPointer++ ) = startIndex;
		// Second triangle
		*( mIndicesPointer++ ) = startIndex;
		*( mIndicesPointer++ ) = startIndex + 1;
		*( mIndicesPointer++ ) = startIndex + 3;
		// Moves start index to next 2 vertices
		startIndex += 2;
	}
	// Increase hair count
	++mHairCount;
	// Store points count
	*( mPointsCountPointer++ ) = aPointsCount * 2; // 2 vertices for each original point
}

inline bool MayaOutputGenerator::getDuplicateBorderPoints() const
{
	return false;
}

inline MayaOutputGenerator::PositionType * MayaOutputGenerator::positionPointer()
{
	return mPositionData;
}

inline MayaOutputGenerator::ColorType * MayaOutputGenerator::colorPointer()
{
	return mColorData;
}

inline MayaOutputGenerator::NormalType * MayaOutputGenerator::normalPointer()
{
	return mNormalData;
}

inline MayaOutputGenerator::WidthType * MayaOutputGenerator::widthPointer()
{
	return mWidthData;
}

inline MayaOutputGenerator::OpacityType * MayaOutputGenerator::opacityPointer()
{
	return mOpacityData;
}

inline MayaOutputGenerator::UVCoordinateType * MayaOutputGenerator::hairUVCoordinatePointer()
{
	return 0; /* NOT SUPPORTED */
}

inline MayaOutputGenerator::UVCoordinateType * MayaOutputGenerator::strandUVCoordinatePointer()
{
	return 0; /* NOT SUPPORTED */
}

inline MayaOutputGenerator::IndexType * MayaOutputGenerator::hairIndexPointer()
{
	return 0; /* NOT SUPPORTED */
}

inline MayaOutputGenerator::IndexType * MayaOutputGenerator::strandIndexPointer()
{
	return 0; /* NOT SUPPORTED */
}

inline void MayaOutputGenerator::rebuildVBO()
{
	killVBO(); // Kills old VBO
	// Upload vertices BO
    GLExt::glGenBuffers( 1, &mVertexBO );
    GLExt::glBindBuffer( GL_ARRAY_BUFFER_ARB, mVertexBO );
    GLExt::glBufferData( GL_ARRAY_BUFFER_ARB, ( mVerticesPointer - mVertices ) * sizeof( GLfloat ), 
		mVertices, GL_STATIC_DRAW_ARB );

	// Upload indices BO
	GLExt::glGenBuffers( 1, &mIndexBO );
    GLExt::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB, mIndexBO );
    GLExt::glBufferData( GL_ELEMENT_ARRAY_BUFFER_ARB, ( mIndicesPointer - mIndices ) * sizeof( GLuint ),
		mIndices, GL_STATIC_DRAW_ARB );
	mDirty = false;
}

inline void MayaOutputGenerator::killVBO()
{
	// Delete buffers
	GLExt::glDeleteBuffers( 1, &mVertexBO );
	GLExt::glDeleteBuffers( 1, &mIndexBO );
	mVertexBO = 0;
	mIndexBO = 0;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MAYA_OUTPUT_GENERATOR_HPP