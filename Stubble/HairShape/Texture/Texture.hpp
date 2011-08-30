#ifndef STUBBLE_TEXTURE_HPP
#define STUBBLE_TEXTURE_HPP

#ifdef MAYA
#include <maya\MDagPath.h>
#endif

#include "HairShape\Mesh\UVPoint.hpp"

#include <fstream>

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
	float realAtUV( Real u, Real v ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets texture value at the given UV coordinates.
	///
	/// \param	u	u coordinate
	/// \param	v	v coordinate
	///
	/// \return	float texture value
	///----------------------------------------------------------------------------------------------------
	Color colorAtUV( Real u, Real v ) const;

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
	/// Resample entire texture.
	///----------------------------------------------------------------------------------------------------
	void resample();
#endif

private:
	bool mDirty;	///< Dirty flag

	float *mTexture;	///< Texture matrix

	unsigned __int32 mWidth;	///< Texture width

	unsigned __int32 mHeight;	///< Texture height

	unsigned __int32 mColorComponents;	///< Color component count

	bool mIsAnimated;	///< Identification of texture type
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

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_SHAPE_HPP