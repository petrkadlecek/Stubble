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
	/// \param	aUseNormals		Ignored, used only to have same interface as any OutputGenerator.
	///-------------------------------------------------------------------------------------------------
	inline void beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount,
		bool aUseNormals );

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
	/// Gets the pointer to hair points positions. 
	///
	/// \return	null if it fails, else return position pointer. 
	///-------------------------------------------------------------------------------------------------
	inline MayaTypes::PositionType * positionPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points colors. 
	///
	/// \return	null if it fails, else return color pointer. 
	///-------------------------------------------------------------------------------------------------
	inline MayaTypes::ColorType * colorPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the null pointer. Hair points normals are not supported. 
	///
	/// \return	null
	///-------------------------------------------------------------------------------------------------
	inline MayaTypes::NormalType * normalPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the null pointer. Hair points widths are not supported. 
	///
	/// \return	null
	///-------------------------------------------------------------------------------------------------
	inline MayaTypes::WidthType * widthPointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points opacity. 
	///
	/// \return	null if it fails, else return opacity pointer. 
	///-------------------------------------------------------------------------------------------------
	inline MayaTypes::OpacityType * opacityPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the null pointer. Hair uv coordinates are not supported. 
	///
	/// \return	null
	///-------------------------------------------------------------------------------------------------
	inline MayaTypes::UVCoordinateType * hairUVCoordinatePointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the null pointer. Strand uv coordinate are not supported. 
	///
	/// \return	null
	///-------------------------------------------------------------------------------------------------
	inline MayaTypes::UVCoordinateType * strandUVCoordinatePointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the null pointer. Hair indices are not supported. 
	///
	/// \return	null
	///-------------------------------------------------------------------------------------------------
	inline MayaTypes::IndexType * hairIndexPointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the null pointer. Strand indices are not supported. 
	///
	/// \return	null
	///-------------------------------------------------------------------------------------------------
	inline MayaTypes::IndexType * strandIndexPointer();

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

	MayaTypes::PositionType * mPositionData;   ///< Information describing a position of the hair

	MayaTypes::ColorType * mColorData; ///< Information describing a color of the hair

	MayaTypes::OpacityType * mOpacityData; ///< Information describing a opacity of the hair

	unsigned __int32 * mHairPointer;	///< The current hair pointer

	MayaTypes::PositionType * mPositionPointer;	///< The current position data pointer

	MayaTypes::ColorType * mColorPointer;  ///< The current color data pointer

	MayaTypes::OpacityType * mOpacityPointer;  ///< The current opacity data pointer
};

// inline functions implementation

inline MayaOutputGenerator::MayaOutputGenerator():
	mHairCount( 0 ),
	mHairData( 0 ),
	mPositionData( 0 ),
	mColorData( 0 ),
	mOpacityData( 0 ),
	mHairPointer( 0 ),
	mPositionPointer( 0 ),
	mColorPointer( 0 ),
	mOpacityPointer( 0 )
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
	mOpacityData = 0;
	mHairPointer = 0;
	mPositionPointer = 0;
	mColorPointer = 0;
	mOpacityPointer = 0;
}

inline void MayaOutputGenerator::beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount,
		bool aUseNormals )
{
	/* Ignores aUseNormals since it never uses them */
	// Free all memory
	clear();
	// We will ignore all aUse***
	try 
	{
		mHairCount = aMaxHairCount;
		unsigned __int32 totalSize = aMaxHairCount * aMaxPointsCount * 3;
		// Allocate memory for color, opacity and position data
		mPositionData = new MayaTypes::PositionType[ totalSize ];
		mColorData = new MayaTypes::ColorType[ totalSize ];
		mOpacityData = new MayaTypes::OpacityType[ totalSize ];
		// Allocate memory for indices
		mHairData = new unsigned __int32[ aMaxHairCount + 1 ];
		// Set pointers and index to start
		mPositionPointer = mPositionData;
		mColorPointer = mColorData;
		mOpacityPointer = mOpacityData;
		*mHairData = 0;
	}
	catch ( ... )
	{
		clear();
		throw;
	}
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
	// Move position pointer
	mPositionPointer += aPointsCount * 3; 
	mColorPointer += aPointsCount * 3;
	mOpacityPointer += aPointsCount * 3;
	++mHairPointer; // Next hair pointer
	// Remember index to data
	*mHairPointer = static_cast< unsigned __int32 >( mPositionData - mPositionPointer );
}

inline MayaTypes::PositionType * MayaOutputGenerator::positionPointer()
{
	return mPositionPointer;
}

inline MayaTypes::ColorType * MayaOutputGenerator::colorPointer()
{
	return mColorPointer;
}

inline MayaTypes::NormalType * MayaOutputGenerator::normalPointer()
{
	return 0; /* NOT SUPPORTED */
}

inline MayaTypes::WidthType * MayaOutputGenerator::widthPointer()
{
	return 0; /* NOT SUPPORTED */
}

inline MayaTypes::OpacityType * MayaOutputGenerator::opacityPointer()
{
	return mOpacityPointer;
}

inline MayaTypes::UVCoordinateType * MayaOutputGenerator::hairUVCoordinatePointer()
{
	return 0; /* NOT SUPPORTED */
}

inline MayaTypes::UVCoordinateType * MayaOutputGenerator::strandUVCoordinatePointer()
{
	return 0; /* NOT SUPPORTED */
}

inline MayaTypes::IndexType * MayaOutputGenerator::hairIndexPointer()
{
	return 0; /* NOT SUPPORTED */
}

inline MayaTypes::IndexType * MayaOutputGenerator::strandIndexPointer()
{
	return 0; /* NOT SUPPORTED */
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MAYA_OUTPUT_GENERATOR_HPP