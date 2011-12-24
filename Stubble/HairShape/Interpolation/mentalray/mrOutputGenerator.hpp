#ifndef STUBBLE_MR_OUTPUT_GENERATOR_HPP
#define STUBBLE_MR_OUTPUT_GENERATOR_HPP

#include "../RenderMan/RMPositionGenerator.hpp"
#include "../OutputGenerator.hpp"

#include "shader.h"
#include "geoshader.h"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Defines types used by mental ray output generator.
///-------------------------------------------------------------------------------------------------
struct MRTypes
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
/// Class for drawing generated hair inside mental ray plugin.
/// This class implements OutputGenerator which is the standard interface for 
/// communication with hair generator class.
/// Differences from RMOutputGenerator are minimal; 
///-------------------------------------------------------------------------------------------------
class MROutputGenerator : public OutputGenerator< MRTypes >, public MRTypes
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	/// Allocates memory for one hair commit. Generated hair are sent to mental ray when
	/// commit buffer is full or end of output is signaled.
	/// 
	/// \param	aCommitSize	Number of hair points in signle commit
	///-------------------------------------------------------------------------------------------------
	inline MROutputGenerator( unsigned __int32 aCommitSize );

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	inline ~MROutputGenerator();

	///-------------------------------------------------------------------------------------------------
	/// Sets whether to output normals.
	///
	/// \param	aOutputNormals	output normals?
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

private:

	///-------------------------------------------------------------------------------------------------
	/// Resets data storing.
	///-------------------------------------------------------------------------------------------------
	inline void reset();

	///----------------------------------------------------------------------------------------------------
	/// Commits all data to mental ray.
	///----------------------------------------------------------------------------------------------------
	inline void commit();

	///----------------------------------------------------------------------------------------------------
	/// Free all memory allocated by this object.
	///----------------------------------------------------------------------------------------------------
	inline void freeMemory();

	///----------------------------------------------------------------------------------------------------
	/// Compute hair vertex count from hair segment count.
	///
	/// \return Hair vertex count.
	///----------------------------------------------------------------------------------------------------
	inline static int verticesFromSegments( int aSegmentCount );

	const unsigned __int32 mCommitSize;   ///< Size of the commit

	int * mSegmentsCount; ///< Number of segments for each hair

	MRTypes::PositionType * mPositionData;   ///< Information describing the position of each hair points

	MRTypes::ColorType * mColorData; ///< Information describing the color of each hair points

	MRTypes::NormalType * mNormalData;   ///< Information describing the normal of each hair points

	MRTypes::WidthType * mWidthData; ///< Information describing the width of each hair points
	
	MRTypes::OpacityType * mOpacityData;	///< Information describing the opacity of each hair points

	MRTypes::UVCoordinateType * mHairUVCoordinateData;	///< Information describing the uv coordinates of each hair

	MRTypes::UVCoordinateType * mStrandUVCoordinateData;///< Information describing the uv coordinates of each strand

	MRTypes::IndexType * mHairIndexData;	///< Information describing the indices of each hair

	MRTypes::IndexType * mStrandIndexData;	///< Information describing the indices of each hair in strand

	int * mSegmentsCountPointer;	///< Number of segments for current hair

	MRTypes::PositionType * mPositionDataPointer;	///< The position of current hair

	MRTypes::ColorType * mColorDataPointer;  ///<  The color of current hair

	MRTypes::NormalType * mNormalDataPointer;   ///<  The normal of current hair

	MRTypes::WidthType * mWidthDataPointer; ///<  The width of current hair

	MRTypes::OpacityType * mOpacityDataPointer; ///<  The opacity of current hair

	MRTypes::UVCoordinateType * mHairUVCoordinateDataPointer; ///<  The uv coordinate of current hair

	MRTypes::UVCoordinateType * mStrandUVCoordinateDataPointer; ///<  The uv coordinate of current strand

	MRTypes::IndexType * mHairIndexDataPointer; ///<  The index of current hair

	MRTypes::IndexType * mStrandIndexDataPointer; ///<  The index of current strand

	bool mOutputNormals;   ///< true to output normals
};

// inline functions implementation

inline MROutputGenerator::MROutputGenerator( unsigned __int32 aCommitSize ):
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
		mPositionData = new MRTypes::PositionType[ aCommitSize * 3 ];
		mColorData = new MRTypes::ColorType[ aCommitSize * 3 ];
		mNormalData = new MRTypes::NormalType[ aCommitSize * 3];
		mWidthData = new MRTypes::WidthType[ aCommitSize ];
		mOpacityData = new MRTypes::OpacityType[ aCommitSize * 3 ];
		// Low memory optimalization :
		mSegmentsCount = new int[ aCommitSize / 2 ]; 
		mHairUVCoordinateData = new MRTypes::UVCoordinateType[ aCommitSize ];
		mStrandUVCoordinateData = new MRTypes::UVCoordinateType[ aCommitSize ];
		mHairIndexData = new MRTypes::IndexType[ aCommitSize / 2 ]; 
		mStrandIndexData = new MRTypes::IndexType[ aCommitSize / 2 ]; 

	}
	catch( ... )
	{
		freeMemory();
		throw;
	}
}

inline MROutputGenerator::~MROutputGenerator()
{
	freeMemory();
}

inline void MROutputGenerator::setOutputNormals( bool aOutputNormals )
{
	mOutputNormals = aOutputNormals;
}

inline void MROutputGenerator::beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount )
{
	reset();
}

inline void MROutputGenerator::endOutput()
{
	commit();
}

inline void MROutputGenerator::beginHair( unsigned __int32 aMaxPointsCount )
{
	// If the added hair won't fit into the buffer, commit and reset it.
	if ( ( mPositionDataPointer + aMaxPointsCount * 3 ) > ( mPositionData + mCommitSize * 3 ) )
	{
		commit();
		reset();
	}
}

inline void MROutputGenerator::endHair( unsigned __int32 aPointsCount )
{
	unsigned __int32 aCountMinus2 = aPointsCount - 2; // Other data than points have 2 fewer items
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
	* mSegmentsCountPointer = static_cast< int >( aPointsCount );
	++mSegmentsCountPointer; 
	// Increase current size

}

inline MRTypes::PositionType * MROutputGenerator::positionPointer()
{
	return mPositionDataPointer;
}

inline MRTypes::ColorType * MROutputGenerator::colorPointer()
{
	return mColorDataPointer;
}

inline MRTypes::NormalType * MROutputGenerator::normalPointer()
{
	return mNormalDataPointer;
}

inline MRTypes::WidthType * MROutputGenerator::widthPointer()
{
	return mWidthDataPointer;
}

inline MRTypes::OpacityType * MROutputGenerator::opacityPointer()
{
	return mOpacityDataPointer;
}

inline MRTypes::UVCoordinateType * MROutputGenerator::hairUVCoordinatePointer()
{
	return mHairUVCoordinateDataPointer;
}

inline MRTypes::UVCoordinateType * MROutputGenerator::strandUVCoordinatePointer()
{
	return mStrandUVCoordinateDataPointer;
}

inline MRTypes::IndexType * MROutputGenerator::hairIndexPointer()
{
	return mHairIndexDataPointer;
}

inline MRTypes::IndexType * MROutputGenerator::strandIndexPointer()
{
	return mStrandIndexDataPointer;
}

inline void MROutputGenerator::reset()
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

inline int MROutputGenerator::verticesFromSegments( int aSegmentCount )
{
	return 3 * (aSegmentCount - 2) + 1;
}

inline void MROutputGenerator::commit()
{
	// Anything to commit?
	if ( mSegmentsCountPointer == mSegmentsCount )
	{
		return; // Nothing to commit
	}
	// Get hair count
	int hairCount = int( mSegmentsCountPointer - mSegmentsCount );

	// Get vertex count
	int vertexCount = 0;
	for (int i=0; i < hairCount; i++)
	{
		vertexCount += verticesFromSegments( mSegmentsCount[ i ] );
	}

	// Begin hair
	miHair_list* hair = mi_api_hair_begin();

	mi_api_hair_info(1, 'r', 1);    // per-vertex radius
    // 0=per hair, 1=per vertex
    // types: n, m, t, u, r

	hair->degree = 3;               // cubic (Bezier)

    // Initialize scalar array
	const int SCALARS_PER_HAIR = 0;
	const int SCALARS_PER_VERTEX = 4;
	miScalar* sarray = mi_api_hair_scalars_begin( SCALARS_PER_HAIR * hairCount + SCALARS_PER_VERTEX * vertexCount );

	// Fill scalar array
	miScalar* p = sarray;
	for (int basePosIndex = 0, baseWidthIndex = 0, hairIndex = 0;
		 hairIndex < hairCount;
		 basePosIndex += 3 * mSegmentsCount[ hairIndex ], baseWidthIndex += mSegmentsCount[ hairIndex ] - 2, hairIndex++)
	{
	    // per-hair data: none

	    // per-vertex data: positions of control points, width
		int seg;
		MRTypes::PositionType* pos;
		for (seg = 0; seg < mSegmentsCount[ hairIndex ] - 2; seg++)
		{
			pos = &mPositionData[ basePosIndex + 3 * (seg+1) ];

            // three Bezier vertices per full Catmull-Rom segment
			*p++ = pos[0];
			*p++ = pos[1];
			*p++ = pos[2];
			*p++ = mWidthData[ baseWidthIndex + seg ];

			*p++ = (pos[3+0] - pos[-3+0]) / 6 + pos[0];
			*p++ = (pos[3+1] - pos[-3+1]) / 6 + pos[1];
			*p++ = (pos[3+2] - pos[-3+2]) / 6 + pos[2];
			*p++ = lerp(mWidthData[ baseWidthIndex + seg ], mWidthData[ baseWidthIndex + seg + 1 ], 1./3.);

			*p++ = (pos[0+0] - pos[6+0]) / 6 + pos[3+0];
			*p++ = (pos[0+1] - pos[6+1]) / 6 + pos[3+1];
			*p++ = (pos[0+2] - pos[6+2]) / 6 + pos[3+2];
			*p++ = lerp(mWidthData[ baseWidthIndex + seg ], mWidthData[ baseWidthIndex + seg + 1 ], 2./3.);
		}
		// final Bezier vertex
		*p++ = pos[0];
		*p++ = pos[1];
		*p++ = pos[2];
		*p++ = mWidthData[ baseWidthIndex + seg ];
	}
 
	mi_api_hair_scalars_end( SCALARS_PER_HAIR * hairCount + SCALARS_PER_VERTEX * vertexCount );  // consistency check

	// Write indices ponting into scalar array
    miGeoIndex* harray = mi_api_hair_hairs_begin(hairCount + 1);

    int sIndex = 0;
	for (int i = 0; i < hairCount; i++) {
        harray[ i ] = sIndex;
		sIndex += SCALARS_PER_HAIR + SCALARS_PER_VERTEX * verticesFromSegments( mSegmentsCount[ i ] );
    }
    harray[ hairCount ] = sIndex;  // last hair index = total count

    mi_api_hair_hairs_end();

	mi_api_hair_end();
}

inline void MROutputGenerator::freeMemory()
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

#endif // STUBBLE_MR_OUTPUT_GENERATOR_HPP