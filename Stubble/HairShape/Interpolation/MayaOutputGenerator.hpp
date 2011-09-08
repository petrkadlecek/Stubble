#ifndef STUBBLE_MAYA_OUTPUT_GENERATOR_HPP
#define STUBBLE_MAYA_OUTPUT_GENERATOR_HPP

#include "MayaPositionGenerator.hpp"
#include "OutputGenerator.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Generator of finished interpolated hair used in Maya plugin.
///-------------------------------------------------------------------------------------------------
class MayaOutputGenerator : public OutputGenerator< float, float, float, float >
{
public:

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
	/// \param	aUseColors		true to a use colors. 
	/// \param	aUseNormals		true to a use normals. 
	/// \param	aUseWidths		true to a use widths. 
	///-------------------------------------------------------------------------------------------------
	inline void beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount, 
		bool aUseColors, bool aUseNormals, bool aUseWidths );

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

	unsigned __int32 mHairCount;	///< Number of the hairs

	unsigned __int32 * mHairData; ///< Indices for each hair to aPositionData and aColorData

	PositionType * mPositionData;   ///< Information describing a position of the hair

	ColorType * mColorData; ///< Information describing a color of the hair

	unsigned __int32 * mHairPointer;	///< The current hair pointer

	PositionType * mPositionPointer;	///< The current position data pointer

	ColorType * mColorPointer;  ///< The current color data pointer
};

// inline functions implementation

inline MayaOutputGenerator::MayaOutputGenerator():
	mHairCount( 0 ),
	mHairData( 0 ),
	mPositionData( 0 ),
	mColorData( 0 ),
	mHairPointer( 0 ),
	mPositionPointer( 0 ),
	mColorPointer( 0 )
{
	
}

inline MayaOutputGenerator::~MayaOutputGenerator()
{
	clear();
}

inline void MayaOutputGenerator::clear()
{
	delete mHairData;
	delete mPositionData;
	delete mColorData;
	mHairCount = 0;
	mHairData = 0;
	mPositionData = 0;
	mColorData = 0;
	mHairPointer = 0;
	mPositionPointer = 0;
	mColorPointer = 0;
}

inline void MayaOutputGenerator::beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount, 
	bool aUseColors, bool aUseNormals, bool aUseWidths )
{
	// Free all memory
	clear();
	// We will ignore all aUse***
	try 
	{
		mHairCount = aMaxHairCount;
		unsigned __int32 totalSize = aMaxHairCount * aMaxPointsCount;
		// Allocate memory for color and position data
		mPositionData = new PositionType[ totalSize ];
		mColorData = new ColorType[ totalSize ];
		// Allocate memory for indices
		mHairData = new unsigned __int32[ aMaxHairCount + 1 ];
		// Set pointers and index to start
		mPositionPointer = mPositionData;
		mColorPointer = mColorData;
		*mHairData = 0;
	}
	catch ( ... )
	{
		clear();
		throw;
	}
}

inline void MayaOutputGenerator::beginHair( unsigned __int32 aMaxPointsCount )
{
	/* EMPTY */
}

inline void MayaOutputGenerator::endHair( unsigned __int32 aPointsCount )
{
	// Move position pointer
	mPositionPointer += aPointsCount; 
	mColorPointer += aPointsCount;
	++mHairPointer; // Next hair pointer
	// Remember index to data
	*mHairPointer = static_cast< unsigned __int32 >( mPositionData - mPositionPointer );
}

inline MayaOutputGenerator::PositionType * MayaOutputGenerator::positionPointer()
{
	return mPositionPointer;
}

inline MayaOutputGenerator::ColorType * MayaOutputGenerator::colorPointer()
{
	return mColorPointer;
}

inline MayaOutputGenerator::NormalType * MayaOutputGenerator::normalPointer()
{
	throw StubbleException( "MayaOutputGenerator::normalPointer : this method is not implemented !" ); 
}

inline MayaOutputGenerator::WidthType * MayaOutputGenerator::widthPointer()
{
	throw StubbleException( "MayaOutputGenerator::widthPointer : this method is not implemented !" ); 
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MAYA_OUTPUT_GENERATOR_HPP