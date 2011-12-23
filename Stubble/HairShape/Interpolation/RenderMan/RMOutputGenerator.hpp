#ifndef STUBBLE_RM_OUTPUT_GENERATOR_HPP
#define STUBBLE_RM_OUTPUT_GENERATOR_HPP

#include "RMPositionGenerator.hpp"
#include "../OutputGenerator.hpp"

#include "ri.h"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Defines types used by RenderMan output generator.
///-------------------------------------------------------------------------------------------------
struct RMTypes
{
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
	/// Defines an alias representing type used to store one component of the 3 components of 
	/// the opacity.
	///-------------------------------------------------------------------------------------------------
	typedef RtFloat OpacityType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store one of the 2 u v coordinates.
	///-------------------------------------------------------------------------------------------------
	typedef RtFloat UVCoordinateType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type used to store hair and strand index.
	///-------------------------------------------------------------------------------------------------
	typedef RtInt IndexType;

};

///-------------------------------------------------------------------------------------------------
/// Class for drawing generated hair inside RenderMan plugin.
/// This class implements OutputGenerator which is the standard interface for 
/// communication with hair generator class.
///-------------------------------------------------------------------------------------------------
class RMOutputGenerator : public OutputGenerator< RMTypes >, public RMTypes
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	inline RMOutputGenerator();

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	inline ~RMOutputGenerator();

	///-------------------------------------------------------------------------------------------------
	/// Sets whether to output normals to RenderMan. 
	///
	/// \param	aOutputNormals	true to an output normals. 
	///-------------------------------------------------------------------------------------------------
	inline void setOutputNormals( bool aOutputNormals );

	///-------------------------------------------------------------------------------------------------
	/// Begins an output of interpolated hair.
	/// Must be called before any hair is outputed. 
	///
	/// \param	aMaxHairCount	Number of a maximum hair. 
	/// \param	aMaxPointsCount	Number of a maximum points. 
	///-------------------------------------------------------------------------------------------------
	inline void beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount );

	///----------------------------------------------------------------------------------------------------
	/// Ends an output.
	/// After this function no output will be received until beginOutput is called.
	///----------------------------------------------------------------------------------------------------
	inline void endOutput();

	///-------------------------------------------------------------------------------------------------
	/// Begins an output of single interpolated hair.
	/// The upper estimate of points on hair must be known to make sure enough resources are be 
	/// prepared. Afterwards hair geometry, color etc. can be send to OutputGenerator via 
	/// positionPointer, colorPointer etc.
	///
	/// \param	aMaxPointsCount	Number of a maximum points on current hair. 
	///-------------------------------------------------------------------------------------------------
	inline void beginHair( unsigned __int32 aMaxPointsCount );

	///-------------------------------------------------------------------------------------------------
	/// Ends an output of single interpolated hair.
	/// Parameter corresponds to number of outputed positions via positionPointer.	
	/// 
	/// \param	aPointsCount	Number of points on finished hair. 
	///-------------------------------------------------------------------------------------------------
	inline void endHair( unsigned __int32 aPointsCount );

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points positions. 
	/// Caller must output as many hair points as specified in later endHair call.
	///
	/// \return	Pointer to position buffer.
	///-------------------------------------------------------------------------------------------------
	inline PositionType * positionPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points colors.
	/// Caller must output as many colors as hair positions count minus 2.
	///
	/// \return	Pointer to color buffer.
	///-------------------------------------------------------------------------------------------------
	inline ColorType * colorPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points normals. 
	/// Caller must output as many normals as hair positions count minus 2.
	/// 
	/// \return	Pointer to normal buffer.
	///-------------------------------------------------------------------------------------------------
	inline NormalType * normalPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points widths. 
	/// Caller must output as many widths as hair positions count minus 2.
	///
	/// \return	Pointer to width buffer.
	///-------------------------------------------------------------------------------------------------
	inline WidthType * widthPointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points opacities. 
	/// Caller must output as many opacities as hair positions count minus 2.
	///
	/// \return	null Pointer to opacity buffer.
	///-------------------------------------------------------------------------------------------------
	inline OpacityType * opacityPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair UV coordinates.
	/// Caller must output one pair of UV coordinates per hair.
	/// Texture UV Coordinates of hair root should be outputed.
	///
	/// \return	Pointer to UV coordinates buffer.
	///-------------------------------------------------------------------------------------------------
	inline UVCoordinateType * hairUVCoordinatePointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair strand UV coordinates.
	/// Caller must output one pair of UV coordinates per hair.
	/// Texture UV Coordinates of main strand hair root should be outputed. 
	///
	/// \return	Pointer to strand UV coordinates buffer.
	///-------------------------------------------------------------------------------------------------
	inline UVCoordinateType * strandUVCoordinatePointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair indices.
	/// Caller must output one unique index per hair.
	///
	/// \return	Pointer to hair indices buffer.
	///-------------------------------------------------------------------------------------------------
	inline IndexType * hairIndexPointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to strand indices.
	/// Caller must output one strand index per hair. Strand index is unique for every strand,
	/// but is same for hair in one strand.
	///
	/// \return	Pointer to strand indices buffer.
	///-------------------------------------------------------------------------------------------------
	inline IndexType * strandIndexPointer();

	///-------------------------------------------------------------------------------------------------
	/// Declares renderman variables. 
	///-------------------------------------------------------------------------------------------------
	inline static void declareVariables();

private:

	static const RtString HAIR_UV_COORDINATE_TOKEN;	///< The hair uv coordinate token

	static const RtString STRAND_UV_COORDINATE_TOKEN;	///< The strand uv coordinate token

	static const RtString HAIR_INDEX_TOKEN;   ///< The hair index token

	static const RtString STRAND_INDEX_TOKEN;   ///< The strand index token

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

	unsigned __int32 mBuffersSize;   ///< Size of the internal buffers ( hair points mutliplied by hair count )

	unsigned __int32 mMaxHairCount; ///< Number of maximum hairs

	RtInt * mSegmentsCount; ///< Number of segments for each hair

	RMTypes::PositionType * mPositionData;   ///< Information describing the position of each hair points

	RMTypes::ColorType * mColorData; ///< Information describing the color of each hair points

	RMTypes::NormalType * mNormalData;   ///< Information describing the normal of each hair points

	RMTypes::WidthType * mWidthData; ///< Information describing the width of each hair points
	
	RMTypes::OpacityType * mOpacityData;	///< Information describing the opacity of each hair points

	RMTypes::UVCoordinateType * mHairUVCoordinateData;	///< Information describing the uv coordinates of each hair

	RMTypes::UVCoordinateType * mStrandUVCoordinateData;///< Information describing the uv coordinates of each strand

	RMTypes::IndexType * mHairIndexData;	///< Information describing the indices of each hair

	RMTypes::IndexType * mStrandIndexData;	///< Information describing the indices of each hair in strand

	RtInt * mSegmentsCountPointer;	///< Number of segments for current hair

	RMTypes::PositionType * mPositionDataPointer;	///< The position of current hair

	RMTypes::ColorType * mColorDataPointer;  ///<  The color of current hair

	RMTypes::NormalType * mNormalDataPointer;   ///<  The normal of current hair

	RMTypes::WidthType * mWidthDataPointer; ///<  The width of current hair

	RMTypes::OpacityType * mOpacityDataPointer; ///<  The opacity of current hair

	RMTypes::UVCoordinateType * mHairUVCoordinateDataPointer; ///<  The uv coordinate of current hair

	RMTypes::UVCoordinateType * mStrandUVCoordinateDataPointer; ///<  The uv coordinate of current strand

	RMTypes::IndexType * mHairIndexDataPointer; ///<  The index of current hair

	RMTypes::IndexType * mStrandIndexDataPointer; ///<  The index of current strand

	bool mOutputNormals;   ///< true to output normals
};

// inline functions implementation

inline RMOutputGenerator::RMOutputGenerator():
	mSegmentsCount( 0 ),
	mPositionData( 0 ),
	mColorData( 0 ),
	mNormalData( 0 ),
	mWidthData( 0 ),
	mOpacityData( 0 ),
	mHairUVCoordinateData( 0 ),
	mStrandUVCoordinateData( 0 ),
	mHairIndexData( 0 ),
	mStrandIndexData( 0 ),
	mSegmentsCountPointer( 0 ),
	mPositionDataPointer( 0 ),
	mColorDataPointer( 0 ),
	mNormalDataPointer( 0 ),
	mWidthDataPointer( 0 ),
	mOpacityDataPointer( 0 ),
	mHairUVCoordinateDataPointer( 0 ),
	mStrandUVCoordinateDataPointer( 0 ),
	mHairIndexDataPointer( 0 ),
	mStrandIndexDataPointer( 0 ),
	mBuffersSize( 0 ),
	mMaxHairCount( 0 )
{
}

inline RMOutputGenerator::~RMOutputGenerator()
{
	freeMemory();
}

inline void RMOutputGenerator::setOutputNormals( bool aOutputNormals )
{
	mOutputNormals = aOutputNormals;
}

inline void RMOutputGenerator::beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount )
{
	// Calculate needed buffers size
	unsigned __int32 newBuffersSize = aMaxHairCount * aMaxPointsCount;
	// Need to allocate more memory ?
	if ( newBuffersSize > mBuffersSize || aMaxHairCount > mMaxHairCount )
	{
		try
		{
			// Kill old memory
			freeMemory();
			// Allocate new memory
			mMaxHairCount = aMaxHairCount;
			mBuffersSize = newBuffersSize;
			mPositionData = new RMTypes::PositionType[ mBuffersSize * 3 ];
			mColorData = new RMTypes::ColorType[ mBuffersSize * 3 ];
			mNormalData = new RMTypes::NormalType[ mBuffersSize * 3];
			mWidthData = new RMTypes::WidthType[ mBuffersSize ];
			mOpacityData = new RMTypes::OpacityType[ mBuffersSize * 3 ];
			mSegmentsCount = new RtInt[ mMaxHairCount ]; 
			mHairUVCoordinateData = new RMTypes::UVCoordinateType[ mMaxHairCount * 2 ];
			mStrandUVCoordinateData = new RMTypes::UVCoordinateType[ mMaxHairCount * 2 ];
			mHairIndexData = new RMTypes::IndexType[ mMaxHairCount ]; 
			mStrandIndexData = new RMTypes::IndexType[ mMaxHairCount ]; 
		}
		catch( ... )
		{
			freeMemory();
			throw;
		}
	}
	// Resets pointers to buffers
	reset();
}

inline void RMOutputGenerator::endOutput()
{
	commit();
}

inline void RMOutputGenerator::beginHair( unsigned __int32 aMaxPointsCount )
{
	/* EMPTY */
}

inline void RMOutputGenerator::endHair( unsigned __int32 aPointsCount )
{
	unsigned __int32 aCountMinus2 = aPointsCount - 2; // Other data than points have 2 less items
	// Move position pointer
	mPositionDataPointer += aPointsCount * 3; 
	mColorDataPointer += aCountMinus2 * 3;
	mNormalDataPointer += aCountMinus2 * 3;
	mWidthDataPointer += aCountMinus2;
	mOpacityDataPointer += aCountMinus2 * 3;
	mHairUVCoordinateDataPointer += 2;
	mStrandUVCoordinateDataPointer += 2;
	++mHairIndexDataPointer;
	++mStrandIndexDataPointer;
	// Store segments count and move pointer
	* mSegmentsCountPointer = static_cast< RtInt >( aPointsCount );
	++mSegmentsCountPointer; 
	// Increase current size

}

inline RMTypes::PositionType * RMOutputGenerator::positionPointer()
{
	return mPositionDataPointer;
}

inline RMTypes::ColorType * RMOutputGenerator::colorPointer()
{
	return mColorDataPointer;
}

inline RMTypes::NormalType * RMOutputGenerator::normalPointer()
{
	return mNormalDataPointer;
}

inline RMTypes::WidthType * RMOutputGenerator::widthPointer()
{
	return mWidthDataPointer;
}

inline RMTypes::OpacityType * RMOutputGenerator::opacityPointer()
{
	return mOpacityDataPointer;
}

inline RMTypes::UVCoordinateType * RMOutputGenerator::hairUVCoordinatePointer()
{
	return mHairUVCoordinateDataPointer;
}

inline RMTypes::UVCoordinateType * RMOutputGenerator::strandUVCoordinatePointer()
{
	return mStrandUVCoordinateDataPointer;
}

inline RMTypes::IndexType * RMOutputGenerator::hairIndexPointer()
{
	return mHairIndexDataPointer;
}

inline RMTypes::IndexType * RMOutputGenerator::strandIndexPointer()
{
	return mStrandIndexDataPointer;
}

inline void RMOutputGenerator::declareVariables()
{
	RiDeclare( HAIR_UV_COORDINATE_TOKEN, "uniform float[2]" );
	RiDeclare( STRAND_UV_COORDINATE_TOKEN, "uniform float[2]" );
	RiDeclare( HAIR_INDEX_TOKEN, "uniform int" );
	RiDeclare( STRAND_INDEX_TOKEN, "uniform int" );
}

inline void RMOutputGenerator::reset()
{
	mSegmentsCountPointer = mSegmentsCount;
	mPositionDataPointer = mPositionData;
	mColorDataPointer = mColorData;
	mNormalDataPointer = mNormalData;
	mWidthDataPointer = mWidthData;
	mOpacityDataPointer = mOpacityData;
	mHairUVCoordinateDataPointer = mHairUVCoordinateData;
	mStrandUVCoordinateDataPointer = mStrandUVCoordinateData;
	mHairIndexDataPointer = mHairIndexData;
	mStrandIndexDataPointer = mStrandIndexData;
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
	if ( mOutputNormals )
	{
		RiCurves( RI_CUBIC, hairCount, mSegmentsCount , RI_NONPERIODIC, RI_P, mPositionData, RI_CS, mColorData,
			RI_OS, mOpacityData, RI_N, mNormalData, RI_WIDTH, mWidthData, HAIR_UV_COORDINATE_TOKEN, mHairUVCoordinateData, 
			STRAND_UV_COORDINATE_TOKEN, mStrandUVCoordinateData, HAIR_INDEX_TOKEN, mHairIndexData, 
			STRAND_INDEX_TOKEN, mStrandIndexData, RI_NULL );
	}
	else
	{
		RiCurves( RI_CUBIC, hairCount, mSegmentsCount , RI_NONPERIODIC, RI_P, mPositionData, RI_CS, mColorData, 
			RI_OS, mOpacityData, RI_WIDTH, mWidthData, HAIR_UV_COORDINATE_TOKEN, mHairUVCoordinateData, 
			STRAND_UV_COORDINATE_TOKEN, mStrandUVCoordinateData, HAIR_INDEX_TOKEN, mHairIndexData, 
			STRAND_INDEX_TOKEN, mStrandIndexData, RI_NULL );
	}
}

inline void RMOutputGenerator::freeMemory()
{
	delete mSegmentsCount;
	mSegmentsCount = 0;
	delete mPositionData;
	mPositionData = 0;
	delete mColorData;
	mColorData = 0;
	delete mNormalData;
	mNormalData = 0;
	delete mWidthData;
	mWidthData = 0;
	delete mOpacityData;
	mOpacityData = 0;
	delete mHairUVCoordinateData;
	mHairUVCoordinateData = 0;
	delete mStrandUVCoordinateData;
	mStrandUVCoordinateData = 0;
	delete mHairIndexData;
	mHairIndexData = 0;
	delete mStrandIndexData;
	mStrandIndexData = 0;
}


} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_RM_OUTPUT_GENERATOR_HPP