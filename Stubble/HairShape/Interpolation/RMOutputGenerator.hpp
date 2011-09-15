#ifndef STUBBLE_RM_OUTPUT_GENERATOR_HPP
#define STUBBLE_RM_OUTPUT_GENERATOR_HPP

#include "RMPositionGenerator.hpp"
#include "OutputGenerator.hpp"

#include "ri.h"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Generator of finished interpolated hair used in RenderMan plugin.
///-------------------------------------------------------------------------------------------------
class RMOutputGenerator : public OutputGenerator< RtFloat, RtFloat, RtFloat, RtFloat >
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of 
	/// the 3D position.
	///-------------------------------------------------------------------------------------------------
	typedef RtFloat PositionType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of the color.
	///-------------------------------------------------------------------------------------------------
	typedef RtFloat ColorType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one component of the 3 components of 
	/// the normal.
	///-------------------------------------------------------------------------------------------------
	typedef RtFloat NormalType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store width.
	///-------------------------------------------------------------------------------------------------
	typedef RtFloat WidthType;

	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	/// 
	/// \param	aCommitSize	Number of hair points in signle commit
	///-------------------------------------------------------------------------------------------------
	inline RMOutputGenerator( unsigned __int32 aCommitSize );

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	inline ~RMOutputGenerator();

	///-------------------------------------------------------------------------------------------------
	/// Begins an output of interpolated hair. 
	///
	/// \param	aMaxHairCount	Number of a maximum hair. 
	/// \param	aMaxPointsCount	Number of a maximum points. 
	/// \param	aUseColors		Colors will be outputed
	/// \param	aUseNormals		Normals will be outputed
	/// \param	aUseWidths		Widths will be outputed
	///-------------------------------------------------------------------------------------------------
	inline void beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount,
		bool aUseColors, bool aUseNormals, bool aUseWidths );

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

private:

	///-------------------------------------------------------------------------------------------------
	/// Resets data storing.
	///-------------------------------------------------------------------------------------------------
	inline void reset();

	///----------------------------------------------------------------------------------------------------
	/// commits all data to RenderMan.
	///----------------------------------------------------------------------------------------------------
	inline void commit();

	///----------------------------------------------------------------------------------------------------
	/// Free memory allocated by this object.
	///----------------------------------------------------------------------------------------------------
	inline void freeMemory();

	const unsigned __int32 mCommitSize;   ///< Size of the commit

	RtInt * mSegmentsCount; ///< Number of segments for each hair

	PositionType * mPositionData;   ///< Information describing the position of each hair

	ColorType * mColorData; ///< Information describing the color of each hair

	NormalType * mNormalData;   ///< Information describing the normal of each hair

	WidthType * mWidthData; ///< Information describing the width of each hair

	RtInt * mSegmentsCountPointer;	///< Number of segments for current hair

	PositionType * mPositionDataPointer;	///< The position of current hair

	ColorType * mColorDataPointer;  ///<  The color of current hair

	NormalType * mNormalDataPointer;   ///<  The normal of current hair

	WidthType * mWidthDataPointer; ///<  The width of current hair

	bool mUseColors;	///< true to use colors

	bool mUseNormals;   ///< true to use normals

	bool mUseWidths;	///< true to use widths
};

// inline functions implementation

inline RMOutputGenerator::RMOutputGenerator( unsigned __int32 aCommitSize ):
	mSegmentsCount( 0 ),
	mPositionData( 0 ),
	mColorData( 0 ),
	mNormalData( 0 ),
	mWidthData( 0 ),
	mSegmentsCountPointer( 0 ),
	mPositionDataPointer( 0 ),
	mColorDataPointer( 0 ),
	mNormalDataPointer( 0 ),
	mWidthDataPointer( 0 ),
	mCommitSize( aCommitSize )
{
	try
	{
		mSegmentsCount = new RtInt[ aCommitSize / 2 ]; // Low memory optimalization !!!
		mPositionData = new RtFloat[ aCommitSize * 3 ];
		mColorData = new RtFloat[ aCommitSize * 3 ];
		mNormalData = new RtFloat[ aCommitSize * 3];
		mWidthData = new RtFloat[ aCommitSize ];
	}
	catch( ... )
	{
		freeMemory();
		throw;
	}
}

inline RMOutputGenerator::~RMOutputGenerator()
{
	freeMemory();
}

inline void RMOutputGenerator::beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount,
		bool aUseColors, bool aUseNormals, bool aUseWidths )
{
	mUseColors = aUseColors;
	mUseNormals = aUseNormals;
	mUseWidths = aUseWidths;
	reset();
}

inline void RMOutputGenerator::endOutput()
{
	commit();
}

inline void RMOutputGenerator::beginHair( unsigned __int32 aMaxPointsCount )
{
	// Will not fit into commit buffer ?
	if ( ( mWidthDataPointer + aMaxPointsCount ) > ( mWidthData + mCommitSize ) )
	{
		// Commit and reset buffer
		commit();
		reset();
	}
}

inline void RMOutputGenerator::endHair( unsigned __int32 aPointsCount )
{
	// Move position pointer
	mPositionDataPointer += aPointsCount * 3; 
	mColorDataPointer += aPointsCount * 3;
	mNormalDataPointer += aPointsCount * 3;
	mWidthDataPointer += aPointsCount;
	// Store segments count and move pointer
	* mSegmentsCount = static_cast< RtInt >( aPointsCount );
	++mSegmentsCount; 
	// Increase current size

}

inline RMOutputGenerator::PositionType * RMOutputGenerator::positionPointer()
{
	return mPositionDataPointer;
}

inline RMOutputGenerator::ColorType * RMOutputGenerator::colorPointer()
{
	return mColorDataPointer;
}

inline RMOutputGenerator::NormalType * RMOutputGenerator::normalPointer()
{
	return mNormalDataPointer;
}

inline RMOutputGenerator::WidthType * RMOutputGenerator::widthPointer()
{
	return mWidthDataPointer;
}

inline void RMOutputGenerator::reset()
{
	mSegmentsCountPointer = mSegmentsCount;
	mPositionDataPointer = mPositionData;
	mColorDataPointer = mColorData;
	mNormalDataPointer = mNormalData;
	mWidthDataPointer = mWidthData;
}

inline void RMOutputGenerator::commit()
{
	// Anything to commit ?
	if ( mSegmentsCountPointer == mSegmentsCount )
	{
		return; // Nothing to commit
	}
	// Get hair count
	RtInt hairCount = static_cast< RtInt >( mSegmentsCountPointer - mSegmentsCount );
	// Distinct different options
	unsigned __int32 options = ( mUseColors ? 1 : 0 ) + ( mUseNormals ? 2 : 0 ) + ( mUseWidths ? 4 : 0 );
	switch ( options )
	{
		case 0: // Not using anything
			RiCurves( RI_CUBIC, hairCount, mSegmentsCount , RI_NONPERIODIC, "P", RI_NULL );
			break;
		case 1: // Using color only
			RiCurves( RI_CUBIC, hairCount, mSegmentsCount , RI_NONPERIODIC, "P", mPositionData, 
				"C", mColorData, RI_NULL );
			break;
		case 2: // Using normals only
			RiCurves( RI_CUBIC, hairCount, mSegmentsCount , RI_NONPERIODIC, "P", mPositionData, 
				"N", mNormalData, RI_NULL );
			break;
		case 3: // Using colors + normals
			RiCurves( RI_CUBIC, hairCount, mSegmentsCount , RI_NONPERIODIC, "P", mPositionData, "C", mColorData, 
				"N", mNormalData, RI_NULL );
			break;
		case 4: // Using widths only
			RiCurves( RI_CUBIC, hairCount, mSegmentsCount , RI_NONPERIODIC, "P",
				RI_WIDTH, mWidthData, RI_NULL );
			break;
		case 5: // Using colors + widths
			RiCurves( RI_CUBIC, hairCount, mSegmentsCount , RI_NONPERIODIC, "P", mPositionData, "C", mColorData, 
				RI_WIDTH, mWidthData, RI_NULL );
			break;
		case 6: // Using normals + widths
			RiCurves( RI_CUBIC, hairCount, mSegmentsCount , RI_NONPERIODIC, "P", mPositionData, "N", mNormalData, 
				RI_WIDTH, mWidthData, RI_NULL );
			break;
		case 7: // Using all
			RiCurves( RI_CUBIC, hairCount, mSegmentsCount , RI_NONPERIODIC, "P", mPositionData, "C", mColorData, 
				"N", mNormalData, RI_WIDTH, mWidthData, RI_NULL );
			break;
	}
}

inline void RMOutputGenerator::freeMemory()
{
	delete mSegmentsCount;
	delete mPositionData;
	delete mColorData;
	delete mNormalData;
	delete mWidthData;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_RM_OUTPUT_GENERATOR_HPP