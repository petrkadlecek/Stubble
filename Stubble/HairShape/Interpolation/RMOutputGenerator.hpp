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
/// RenderMan output generator types.
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
/// Generator of finished interpolated hair used in RenderMan plugin.
///-------------------------------------------------------------------------------------------------
class RMOutputGenerator : public OutputGenerator< RMTypes >, public RMTypes
{
public:

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
	/// Sets whether to output normals. 
	///
	/// \param	aOutputNormals	true to an output normals. 
	///-------------------------------------------------------------------------------------------------
	inline void setOutputNormals( bool aOutputNormals );

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
	/// Gets the pointer to hair points positions. 
	///
	/// \return	null if it fails, else return position pointer. 
	///-------------------------------------------------------------------------------------------------
	inline RMTypes::PositionType * positionPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points colors. 
	///
	/// \return	null if it fails, else return color pointer. 
	///-------------------------------------------------------------------------------------------------
	inline RMTypes::ColorType * colorPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points normals. 
	///
	/// \return	null if it fails or normals are not outputed, else return normal pointer. 
	///-------------------------------------------------------------------------------------------------
	inline RMTypes::NormalType * normalPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points widths. 
	///
	/// \return	null if it fails, else return width pointer. 
	///-------------------------------------------------------------------------------------------------
	inline RMTypes::WidthType * widthPointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points opacities. 
	///
	/// \return	null if it fails, else return opacity pointer. 
	///-------------------------------------------------------------------------------------------------
	inline RMTypes::OpacityType * opacityPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair UV coordinates. 
	///
	/// \return	null if it fails, else return hair UV coordinate pointer. 
	///-------------------------------------------------------------------------------------------------
	inline RMTypes::UVCoordinateType * hairUVCoordinatePointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to strand UV coordinates. 
	///
	/// \return	null if it fails, else return strand UV coordinate pointer. 
	///-------------------------------------------------------------------------------------------------
	inline RMTypes::UVCoordinateType * strandUVCoordinatePointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair indices. 
	///
	/// \return	null if it fails, else return hair index pointer. 
	///-------------------------------------------------------------------------------------------------
	inline RMTypes::IndexType * hairIndexPointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to strand indices. 
	///
	/// \return	null if it fails, else return strand index pointer. 
	///-------------------------------------------------------------------------------------------------
	inline RMTypes::IndexType * strandIndexPointer();

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

	const unsigned __int32 mCommitSize;   ///< Size of the commit

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

inline RMOutputGenerator::RMOutputGenerator( unsigned __int32 aCommitSize ):
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
	mCommitSize( aCommitSize )
{
	try
	{
		// aCommitSize -> number of segments in one commit, hair has at least two of them
		mPositionData = new RMTypes::PositionType[ aCommitSize * 3 ];
		mColorData = new RMTypes::ColorType[ aCommitSize * 3 ];
		mNormalData = new RMTypes::NormalType[ aCommitSize * 3];
		mWidthData = new RMTypes::WidthType[ aCommitSize ];
		mOpacityData = new RMTypes::OpacityType[ aCommitSize * 3 ];
		// Low memory optimalization :
		mSegmentsCount = new RtInt[ aCommitSize / 2 ]; 
		mHairUVCoordinateData = new RMTypes::UVCoordinateType[ aCommitSize ];
		mStrandUVCoordinateData = new RMTypes::UVCoordinateType[ aCommitSize ];
		mHairIndexData = new RMTypes::IndexType[ aCommitSize / 2 ]; 
		mStrandIndexData = new RMTypes::IndexType[ aCommitSize / 2 ]; 

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

inline void RMOutputGenerator::setOutputNormals( bool aOutputNormals )
{
	mOutputNormals = aOutputNormals;
}

inline void RMOutputGenerator::beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount )
{
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
	mOpacityDataPointer += aPointsCount * 3;
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
	delete mPositionData;
	delete mColorData;
	delete mNormalData;
	delete mWidthData;
	delete mOpacityData;
	delete mHairUVCoordinateData;
	delete mStrandUVCoordinateData;
	delete mHairIndexData;
	delete mStrandIndexData;
}


} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_RM_OUTPUT_GENERATOR_HPP