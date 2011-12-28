#ifndef STUBBLE_TEXTURE_HPP
#define STUBBLE_TEXTURE_HPP

#ifdef MAYA
#include <maya\MDagPath.h>
#include <maya\MPlugArray.h>
#include <maya\MPlug.h>
#include <maya\MImage.h>
#include <maya\MRenderUtil.h>
#include <maya\M3dView.h>
#include <maya\MMatrix.h>
#include <maya\MFloatMatrix.h>
#include <maya\MFloatVectorArray.h>
#include <maya\MFloatArray.h>
#include <maya\MTypes.h>
#endif

#include "HairShape\Mesh\UVPoint.hpp"
#include "Common\CommonFunctions.hpp"

#include <algorithm>
#include <fstream>

#undef min
#undef max

namespace Stubble
{

namespace HairShape
{
///----------------------------------------------------------------------------------------------------
/// Class that holds texture that is used as an attribute
///----------------------------------------------------------------------------------------------------
class Texture
{
public:

#ifdef MAYA
	///----------------------------------------------------------------------------------------------------
	/// Maya mesh constructor. 
	///----------------------------------------------------------------------------------------------------
	Texture(MDagPath & node);
#endif

	///----------------------------------------------------------------------------------------------------
	/// Single float constructor
	///----------------------------------------------------------------------------------------------------
	Texture( float value );

	///----------------------------------------------------------------------------------------------------
	/// 3-dimensional float constructor
	///----------------------------------------------------------------------------------------------------
	Texture( float value, float value1, float value2 );

	///----------------------------------------------------------------------------------------------------
	/// 4-dimensional float constructor
	///----------------------------------------------------------------------------------------------------
	Texture( float value, float value1, float value2, float value3 );

	///----------------------------------------------------------------------------------------------------
	/// Initilize array for texture
	///----------------------------------------------------------------------------------------------------
	void init();

	///----------------------------------------------------------------------------------------------------
	/// Defines an alias representing Color
	///----------------------------------------------------------------------------------------------------
	typedef float* Color;

	///----------------------------------------------------------------------------------------------------
	/// Defines an alias representing interpolated Color
	///----------------------------------------------------------------------------------------------------
	typedef float Color3[3];

	///----------------------------------------------------------------------------------------------------
	/// Color comparator. 
	///----------------------------------------------------------------------------------------------------
	class ColorComparator
	{
	public:

		///----------------------------------------------------------------------------------------------------
		/// Constructor. 
		///
		/// \param	aComponentCount	Number of the components. 
		explicit inline ColorComparator( unsigned __int32 aComponentCount );

		///----------------------------------------------------------------------------------------------------
		/// Comparator operator. 
		///
		/// \param	aColor	the first color to compare
		/// \param	aColor	the second color to compare
		/// 				
		/// \return	true, if aColor1 < aColor2				
		///----------------------------------------------------------------------------------------------------
		inline bool operator() ( const Color & aColor1, const Color & aColor2 ) const;
	private:
		unsigned __int32 mComponentCount;   ///< Number of components
	};

	///----------------------------------------------------------------------------------------------------
	/// Stream constructor
	///----------------------------------------------------------------------------------------------------
	Texture( std::istream & aIsStream );

	///----------------------------------------------------------------------------------------------------
	/// Finalizer
	///----------------------------------------------------------------------------------------------------
	~Texture();

	///----------------------------------------------------------------------------------------------------
	/// Gets texture value at the given UV coordinates.
	///
	/// \param	u	u coordinate
	/// \param	v	v coordinate
	///
	/// \return	float texture value
	///----------------------------------------------------------------------------------------------------
	inline float realAtUV( Real u, Real v ) const;

	///-------------------------------------------------------------------------------------------------
	/// Derivative by u at given UV coordinates. 
	///
	/// \param	u	u coordinate
	/// \param	v	v coordinate
	///
	/// \return	derivative value. 
	///-------------------------------------------------------------------------------------------------
	inline float derivativeByUAtUV( Real u, Real v ) const;

	///-------------------------------------------------------------------------------------------------
	/// Derivative by v at given UV coordinates. 
	///
	/// \param	u	u coordinate
	/// \param	v	v coordinate
	///
	/// \return	derivative value. 
	///-------------------------------------------------------------------------------------------------
	inline float derivativeByVAtUV( Real u, Real v ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets texture value at the given UV coordinates.
	///
	/// \param	aU	u coordinate
	/// \param	aV	v coordinate
	/// \param	[out]aOutColor	Color on coordinates [aU,aV]
	/// 
	///----------------------------------------------------------------------------------------------------
	inline void colorAtUV( Real aU, Real aV, Color3 aOutColor ) const;

	///----------------------------------------------------------------------------------------------------
	/// Puts texture in stream, exports only current time frame.
	///
	/// \param aOutStream	output stream for saving
	///----------------------------------------------------------------------------------------------------
	void exportToFile( std::ostream &aOutStream ) const;

	///----------------------------------------------------------------------------------------------------
	/// Marks the texture as dirty.
	///----------------------------------------------------------------------------------------------------
	void setDirty();

	///----------------------------------------------------------------------------------------------------
	/// Gets informatin about dirty flag of texture.
	///
	/// \return true if dirty
	///----------------------------------------------------------------------------------------------------
	bool isDirty() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets informatin about texture width
	///----------------------------------------------------------------------------------------------------
	unsigned __int32 getWidth() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets informatin about texture height
	///----------------------------------------------------------------------------------------------------
	unsigned __int32 getHeight() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets count of color components
	///----------------------------------------------------------------------------------------------------
	unsigned __int32 getColorCompomentsCount() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets raw data of texture
	///----------------------------------------------------------------------------------------------------
	float *getRawData() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets info about texture animation
	///----------------------------------------------------------------------------------------------------
	bool isAnimated() const;

	///-------------------------------------------------------------------------------------------------
	/// Sets a current time for animated texture. 
	///
	/// \param	aTime	the current time
	///-------------------------------------------------------------------------------------------------
	void setCurrentTime( Time aTime );

#ifdef MAYA
	///----------------------------------------------------------------------------------------------------
	/// Sets a connection to a source of texture data.
	/// Saves the plug of outColor or outAlpha attribute from the texture node.
	///
	/// \param aPlug	plug to which the data are connected
	///----------------------------------------------------------------------------------------------------
	void setConnection(const MPlug& aPlug); 

	///----------------------------------------------------------------------------------------------------
	/// Removes a connection to a source of texture data.
	///
	///----------------------------------------------------------------------------------------------------
	void removeConnection(); 

	///----------------------------------------------------------------------------------------------------
	/// Resample entire texture.
	///
	/// \param aTextureUSamples number of samples in U dimension of sampled texture
	/// \param aTextureVSamples number of samples in V dimension of sampled texture
	///----------------------------------------------------------------------------------------------------
	void resample( unsigned __int32 aTextureUSamples, unsigned __int32 aTextureVSamples );

	///----------------------------------------------------------------------------------------------------
	/// Loads image texture into internal datastructure.
	///
	/// \param aTextureImage	image from which are loaded necessary pixel channels.
	///----------------------------------------------------------------------------------------------------
	void reloadFileTextureImage( MImage & aTextureImage );

	///----------------------------------------------------------------------------------------------------
	/// Samples connected 2DTexture and stores the samplevalues.
	///
	/// \param aTextureUSamples number of samples in U dimension of sampled texture
	/// \param aTextureVSamples number of samples in V dimension of sampled texture
	///----------------------------------------------------------------------------------------------------
	void resample2DTexture( unsigned __int32 aTextureUSamples, unsigned __int32 aTextureVSamples );

#endif

private:
	bool mDirty;	///< Dirty flag

	bool mImageTexture; ///< Image texture flag

#ifdef MAYA
	MPlug mTextureDataSourcePlug; ///< TextureData source
#endif

	float *mTexture;	///< Texture matrix

	unsigned __int32 mWidth;	///< Texture width

	unsigned __int32 mHeight;	///< Texture height

	unsigned __int32 mColorComponents;	///< Color component count

	bool mIsAnimated;	///< Identification of texture type

	float mInverseWidth;  ///< The inverse value of texture width

	float mInverseHeight;  ///< The inverse value of texture height


	void getSampleUVPoints( float aUSamples[], float aVSamples[], unsigned __int32 aUDimension, unsigned __int32 aVDimension );

	void computeInverseSize();

	///----------------------------------------------------------------------------------------------------
	/// Compute bilinear interpolation from 4 samples with more dimensions along directions U and V
	///
	/// \param	aSampleU0V0	coordinate of sample in left up corner of pixel
	/// \param	aSampleU0V0	coordinate of sample in left down corner of pixel
	/// \param	aSampleU0V0	coordinate of sample in right up corner of pixel
	/// \param	aSampleU0V0	coordinate of sample in right down corner of pixel
	/// \param  aUratio	ratio between samples in direction U
	/// \param	aVratio ratio between samples in direction V
	/// \param  [out]aOutColor output parametr for returning interpolated color 
	///----------------------------------------------------------------------------------------------------
	inline void colorAtUV( const Color aSampleU0V0, const Color aSampleU0V1,
		const Color aSampleU1V0, const Color aSampleU1V1,
		const Real aURatio, const Real aVRatio, Color3 aOutColor ) const;

	inline void interpolateColors( const Color3 aColor1, const Color3 aColor2,
		const Real aRatio, Color3 aOutColor ) const;

	///----------------------------------------------------------------------------------------------------
	/// Compute bilinear interpolation from 4 real samples along directions U and V
	///
	/// \param	aSampleU0V0	coordinate of sample in left up corner of pixel
	/// \param	aSampleU0V0	coordinate of sample in left down corner of pixel
	/// \param	aSampleU0V0	coordinate of sample in right up corner of pixel
	/// \param	aSampleU0V0	coordinate of sample in right down corner of pixel
	/// \param  aUratio	ratio between samples in direction U
	/// \param	aVratio ratio between samples in direction V
	/// return value
	///----------------------------------------------------------------------------------------------------
	inline Real realAtUV( const Color aSampleU0V0, const Color aSampleU0V1,
		const Color aSampleU1V0, const Color aSampleU1V1,
		const Real aURatio, const Real aVRatio ) const;

	inline Real interpolateReals( const Real aColor1, const Real aColor2, const Real aRatio ) const;
};

// inline functions implementation

inline Texture::ColorComparator::ColorComparator( unsigned __int32 aComponentCount ):
	mComponentCount( aComponentCount )
{
}

inline bool Texture::ColorComparator::operator() ( const Texture::Color & aColor1, 
	const Texture::Color & aColor2 ) const
{
	for( unsigned __int32 i = 0; i < mComponentCount; ++i ) // For every component
	{
		if ( aColor1[ i ] < aColor2[ i ] )
		{
			return true;
		}
		else
		{
			if ( aColor1[ i ] > aColor2[ i ] )
			{
				return false;
			}
		}
		// Components equal so far -> continue
	}
	return false; // color1 equals color2
}

inline float Texture::realAtUV( Real u, Real v ) const
{
	//Color3 color;
	//colorAtUV( u, v, color ); // Alpha
	//return color[ 0 ]; //TODO: napsat funkce primo pro float aby se zrychlilo
	u = clamp( u, 0.0, 1.0 );
	v = clamp( v, 0.0, 1.0 );
	unsigned __int32 x0 = static_cast< unsigned __int32 > ( floor( u * ( mWidth - 1 ) ) );
	unsigned __int32 y0 = static_cast< unsigned __int32 > ( floor( v * ( mHeight - 1 ) ) );
	unsigned __int32 x1 = static_cast< unsigned __int32 > ( ceil( u * ( mWidth - 1 ) ) );
	unsigned __int32 y1 = static_cast< unsigned __int32 > ( ceil( v * ( mHeight - 1 ) ) );

	return (float) realAtUV(mTexture + y0 * mWidth * mColorComponents + x0 * mColorComponents,
		mTexture + y1 * mWidth * mColorComponents + x0 * mColorComponents,
		mTexture + y0 * mWidth * mColorComponents + x1 * mColorComponents,
		mTexture + y1 * mWidth * mColorComponents + x1 * mColorComponents,
		(Real) x1 - u * (mWidth -1), (Real) y1 - v * (mHeight -1));
}

inline float Texture::derivativeByUAtUV( Real u, Real v ) const
{
	return ( realAtUV( std::min( u + mInverseWidth, static_cast< Real >( 1.0f ) ), v ) -
		realAtUV( u, v ) ) * mWidth;
}

inline float Texture::derivativeByVAtUV( Real u, Real v ) const
{
	return ( realAtUV( u, std::min( v + mInverseHeight, static_cast< Real >( 1.0f ) ) ) -
		realAtUV( u, v ) ) * mHeight;
}

inline void Texture::computeInverseSize()
{
	mInverseHeight = 1.0f / mHeight;
	mInverseWidth = 1.0f / mWidth;
}

inline void Texture::colorAtUV( Real u, Real v, Color3 aOutColor ) const
{
	u = clamp( u, 0.0, 1.0 );
	v = clamp( v, 0.0, 1.0 );
	unsigned __int32 x0 = static_cast< unsigned __int32 > ( floor( u * ( mWidth - 1 ) ) );
	unsigned __int32 y0 = static_cast< unsigned __int32 > ( floor( v * ( mHeight - 1 ) ) );
	unsigned __int32 x1 = static_cast< unsigned __int32 > ( ceil( u * ( mWidth - 1 ) ) );
	unsigned __int32 y1 = static_cast< unsigned __int32 > ( ceil( v * ( mHeight - 1 ) ) );

	colorAtUV(mTexture + y0 * mWidth * mColorComponents + x0 * mColorComponents,
		mTexture + y1 * mWidth * mColorComponents + x0 * mColorComponents,
		mTexture + y0 * mWidth * mColorComponents + x1 * mColorComponents,
		mTexture + y1 * mWidth * mColorComponents + x1 * mColorComponents,
		(Real) x1 - u * (mWidth -1), (Real) y1 - v * (mHeight -1), aOutColor );
}

inline void Texture::colorAtUV( const Color aSampleU0V0, const Color aSampleU0V1,
		const Color aSampleU1V0, const Color aSampleU1V1, const Real aURatio,
		const Real aVRatio, Color3 aOutColor ) const
{
	Color3 colV0;
	Color3 colV1;
	interpolateColors( aSampleU0V0, aSampleU1V0, aURatio, colV0 );	
	interpolateColors( aSampleU0V1, aSampleU1V1, aURatio, colV1 );  
	interpolateColors( colV0, colV1, aVRatio, aOutColor ); 
}

inline void Texture::interpolateColors( const Color3 aColor1, const Color3 aColor2,
	const Real aRatio, Color3 aOutColor ) const
{
	for( int i = 0; i < (int) mColorComponents; ++i )
	{
		aOutColor[ i ] = (float) (aRatio * aColor1[ i ] + ( 1 - aRatio ) * aColor2[ i ]);
	}
}

inline Real Texture::realAtUV( const Color aSampleU0V0, const Color aSampleU0V1,
		const Color aSampleU1V0, const Color aSampleU1V1, const Real aURatio,
		const Real aVRatio ) const
{ 
	return interpolateReals( interpolateReals( aSampleU0V0[0], aSampleU1V0[0], aURatio ),
		interpolateReals( aSampleU0V1[0], aSampleU1V1[0], aURatio ), aVRatio ); 
}

inline Real Texture::interpolateReals( const Real aColor1, const Real aColor2,
	const Real aRatio ) const
{
	return ( aRatio * aColor1 + ( 1 - aRatio ) * aColor2 );
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_TEXTURE_HPP