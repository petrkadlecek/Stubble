#ifndef STUBBLE_TEXTURE_HPP
#define STUBBLE_TEXTURE_HPP

#ifdef MAYA
#include <maya\MDagPath.h>
#endif

#include "HairShape\Mesh\UVPoint.hpp"

#include <fstream>

#define COLOR_COMPONENTS	4

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
	/// Initilize values of texture
	///----------------------------------------------------------------------------------------------------
	void init( float value, float value1, float value2, float value3 );

	///----------------------------------------------------------------------------------------------------
	/// Defines an alias representing Color
	///----------------------------------------------------------------------------------------------------
	typedef float* Color;

	///----------------------------------------------------------------------------------------------------
	/// Color comparator. 
	///----------------------------------------------------------------------------------------------------
	template < unsigned int tComponentCount >
	struct ColorComparator
	{
		///----------------------------------------------------------------------------------------------------
		/// Comparator operator. 
		///
		/// \param	aColor	the first color to compare
		/// \param	aColor	the second color to compare
		/// 				
		/// \return	true, if aColor1 < aColor2				
		///----------------------------------------------------------------------------------------------------
		inline bool operator() ( const Color & aColor1, const Color & aColor2 ) const;
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
	/// Puts texture in stream.
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
	unsigned int getWidth() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets informatin about texture height
	///----------------------------------------------------------------------------------------------------
	unsigned int getHeight() const;

#ifdef MAYA
	///----------------------------------------------------------------------------------------------------
	/// Resample entire texture.
	///----------------------------------------------------------------------------------------------------
	void resample();
#endif

private:
	bool mDirty;	///< Dirty flag

	float* mTexture;	///< Texture matrix

	unsigned int mWidth;	///< Texture width

	unsigned int mHeight;	///< Texture height
};

// inline functions implementation

template< unsigned int tComponentCount >
inline bool Texture::ColorComparator< tComponentCount >::operator() ( const Texture::Color & aColor1, 
	const Texture::Color & aColor2 ) const
{
	for( unsigned int i = 0; i < tComponentCount; ++i ) // For every component
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