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
	/// \param	u	u coordinate
	/// \param	v	v coordinate
	///
	/// \return	float texture value
	///----------------------------------------------------------------------------------------------------
	inline Color colorAtUV( Real u, Real v ) const;

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
	/// Resample entire texture.
	///
	/// \param aTextureSamples number of samples in one dimension of sampled texture
	///----------------------------------------------------------------------------------------------------
	void resample(unsigned __int32 aTextureSamples);

	///----------------------------------------------------------------------------------------------------
	/// Loads image texture into internal datastructure.
	///
	/// \param aTextureImage	image from which are loaded necessary pixel channels.
	///----------------------------------------------------------------------------------------------------
	void reloadFileTextureImage(MImage aTextureImage);

	///----------------------------------------------------------------------------------------------------
	/// Samples connected 2DTexture and stores the samplevalues.
	///
	/// \param aTextureSamples number of samples in one dimension of sampled texture
	///----------------------------------------------------------------------------------------------------
	void resample2DTexture(unsigned __int32 aTextureSamples);

#endif

private:
	bool mDirty;	///< Dirty flag

	bool mImageTexture; ///< Image texture flag

#ifdef MAYA
	MPlug textureDataSourcePlug; ///< TextureData source
#endif

	float *mTexture;	///< Texture matrix

	unsigned __int32 mWidth;	///< Texture width

	unsigned __int32 mHeight;	///< Texture height

	unsigned __int32 mColorComponents;	///< Color component count

	bool mIsAnimated;	///< Identification of texture type

	float mInverseWidth;  ///< The inverse value of texture width

	float mInverseHeight;  ///< The inverse value of texture height


	void GetSampleUVPoints(float aUSamples[], float aVSamples[], unsigned __int32 dimension);

	void ComputeInverseSize();

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
	return colorAtUV(u, v)[ mColorComponents - 1 ]; // Alpha
}

inline float Texture::derivativeByUAtUV( Real u, Real v ) const
{
	return ( realAtUV( std::max( u + mInverseWidth, static_cast< Real >( 1.0f ) ), v ) -
		realAtUV( u, v ) ) * mWidth;
}

inline float Texture::derivativeByVAtUV( Real u, Real v ) const
{
	return ( realAtUV( u, std::max( v + mInverseHeight, static_cast< Real >( 1.0f ) ) ) -
		realAtUV( u, v ) ) * mHeight;
}

inline Texture::Color Texture::colorAtUV( Real u, Real v ) const
{
	unsigned __int32 x = static_cast< unsigned __int32 > ( floor(u * mWidth) );
	x = x == mWidth ? mWidth - 1 : x;

	unsigned __int32 y = static_cast< unsigned __int32 > ( floor(v * mHeight) );
	y = y == mHeight ? mHeight - 1 :  y;

	//return mTexture + y * mWidth * mColorComponents * sizeof(float) + x * mColorComponents * sizeof(float);
	return mTexture + y * mWidth * mColorComponents + x * mColorComponents;
}

inline void Texture::ComputeInverseSize()
{
	mInverseHeight = 1.0f / mHeight;
	mInverseWidth = 1.0f / mWidth;
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_SHAPE_HPP