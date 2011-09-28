#ifndef STUBBLE_INTERPOLATION_GROUPS_HPP
#define STUBBLE_INTERPOLATION_GROUPS_HPP

#include "Common\CommonTypes.hpp"
#include "Common\StubbleException.hpp"
#include "HairShape\Texture\Texture.hpp"

#include <vector>

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Class for managing interpolation groups
///-------------------------------------------------------------------------------------------------
class InterpolationGroups
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Constructor. 
	///
	/// \param	aInterpolationGroupsTexture	The interpolation groups texture. 
	/// \param	aSegmentsCount				Number of the segments for all groups. 
	///-------------------------------------------------------------------------------------------------
	InterpolationGroups( const Texture & aInterpolationGroupsTexture, unsigned __int32 aSegmentsCount );

	///----------------------------------------------------------------------------------------------------
	/// Finalizer
	///----------------------------------------------------------------------------------------------------
	inline ~InterpolationGroups();

	///-------------------------------------------------------------------------------------------------
	/// Updates interpolation groups
	///
	/// \param	aInterpolationGroupsTexture	The interpolation groups texture. 
	/// \param	aSegmentsCount				Number of the segments for all groups. 
	///-------------------------------------------------------------------------------------------------
	void updateGroups( const Texture & aInterpolationGroupsTexture, unsigned __int32 aSegmentsCount );

	///-------------------------------------------------------------------------------------------------
	/// Gets the interpolation groups count. 
	/// 
	/// \return	The interpolation groups count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 getGroupsCount() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the interpolation group segments count. 
	///
	/// \param	aGroupId	Identifier for a group. 
	///
	/// \return	The interpolation group segments count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 getGroupSegmentsCount( unsigned __int32 aGroupId ) const;

	///-------------------------------------------------------------------------------------------------
	/// Sets the interpolation group segments count. 
	///
	/// \param	aGroupId		Identifier for a group. 
	/// \param	aSegmentsCount	Number of the segments for selected group. 
	///-------------------------------------------------------------------------------------------------
	inline void setGroupSegmentsCount( unsigned __int32 aGroupId, unsigned __int32 aSegmentsCount );

	///-------------------------------------------------------------------------------------------------
	/// Gets a group identifier. 
	///
	/// \param	aU	the u texture coordinate. 
	/// \param	aV	the v texture coordinate. 
	///
	/// \return	The group identifier. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 getGroupId( Real aU, Real aV ) const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the segments count. 
	///
	/// \param	aU	the u texture coordinate. 
	/// \param	aV	the v texture coordinate. 
	///
	/// \return	The segments count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 getSegmentsCount( Real aU, Real aV ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the color compoment count. 
	///
	///
	/// \return	The color compoment count. 
	///----------------------------------------------------------------------------------------------------
	inline unsigned __int32 getColorCompomentCount() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets a color of interpolation group. 
	///
	/// \param	aGroupId	Identifier for a interpolation group. 
	///
	/// \return	The color of interpolation group. 
	///----------------------------------------------------------------------------------------------------
	inline Texture::Color getColorOfGroup( unsigned __int32 aGroupId ) const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the maximum segments count. 
	///
	/// \return	The maximum segments count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 getMaxSegmentsCount() const;

	///----------------------------------------------------------------------------------------------------
	/// Export segments count to file. 
	///
	/// \param [in,out]	aOutputStream	a output stream. 
	///----------------------------------------------------------------------------------------------------
	void exportSegmentsCountToFile( std::ostream & aOutputStream ) const;

	///----------------------------------------------------------------------------------------------------
	/// Import segments count from file. 
	///
	/// \param [in,out]	aInputStream	a input stream. 
	///----------------------------------------------------------------------------------------------------
	void importSegmentsCountFromFile( std::istream & aInputStream );

private:

	///----------------------------------------------------------------------------------------------------
	/// Defines an alias representing number of interpolation groups segments .
	///----------------------------------------------------------------------------------------------------
	typedef std::vector< unsigned __int32 > InterpolationGroupsSegmentsCount;

	unsigned __int32 mTextureHeight;   ///< The texture height

	unsigned __int32 mTextureWidth;	///< The texture width

	unsigned __int32 * mInterpolationGroupsTexture;	///< Interpolation groups texture

	Texture::Color mInterpolationGroupsColors; ///< List of colors of the interpolation groups

	unsigned __int32 mColorComponentCount;  ///< Number of color components

	InterpolationGroupsSegmentsCount mInterpolationGroupsSegmentsCount; ///< Number of interpolation groups segments
};

// inline functions implementation

inline InterpolationGroups::~InterpolationGroups()
{
	delete [] mInterpolationGroupsTexture;
	delete [] mInterpolationGroupsColors;
}

inline unsigned __int32 InterpolationGroups::getGroupsCount() const
{
	return static_cast< unsigned __int32 >( mInterpolationGroupsSegmentsCount.size() );
}

inline unsigned __int32 InterpolationGroups::getGroupSegmentsCount( unsigned __int32 aGroupId ) const
{
	return mInterpolationGroupsSegmentsCount[ aGroupId ];
}

inline void InterpolationGroups::setGroupSegmentsCount( unsigned __int32 aGroupId, unsigned __int32 aSegmentsCount )
{
	if ( aSegmentsCount == 0 )
	{
		throw StubbleException(" InterpolationGroups::setGroupSegmentsCount : segments count must be greater than 0 ");
	}
	mInterpolationGroupsSegmentsCount[ aGroupId ] = aSegmentsCount;
}

inline unsigned __int32 InterpolationGroups::getGroupId( Real aU, Real aV ) const
{
	unsigned __int32 x = static_cast< unsigned __int32 > ( floor(aU * mTextureWidth) );
	x = x == mTextureWidth ? mTextureWidth - 1 : x;

	unsigned __int32 y = static_cast< unsigned __int32 > ( floor(aV * mTextureHeight) );
	y = y == mTextureHeight ? mTextureHeight - 1 :  y;

	return *( mInterpolationGroupsTexture + y * mTextureWidth + x );
}

inline unsigned __int32 InterpolationGroups::getSegmentsCount( Real aU, Real aV ) const
{
	unsigned __int32 x = static_cast< unsigned __int32 > ( floor(aU * mTextureWidth) );
	x = x == mTextureWidth ? mTextureWidth - 1 : x;

	unsigned __int32 y = static_cast< unsigned __int32 > ( floor(aV * mTextureHeight) );
	y = y == mTextureHeight ? mTextureHeight - 1 :  y;

	return mInterpolationGroupsSegmentsCount[ *( mInterpolationGroupsTexture + y * mTextureWidth + x ) ];
}

inline unsigned __int32 InterpolationGroups::getColorCompomentCount() const
{
	return mColorComponentCount;
}

inline Texture::Color InterpolationGroups::getColorOfGroup( unsigned __int32 aGroupId ) const
{
	return mInterpolationGroupsColors + aGroupId * mColorComponentCount;
}

inline unsigned __int32 InterpolationGroups::getMaxSegmentsCount() const
{
	unsigned __int32 max = 0;
	// For every interpolation group
	for ( InterpolationGroupsSegmentsCount::const_iterator it = mInterpolationGroupsSegmentsCount.begin();
		it != mInterpolationGroupsSegmentsCount.end(); ++it )
	{
		if ( max < *it )
		{
			max = *it;
		}
	}
	return max;
}


} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_INTERPOLATION_GROUPS_HPP