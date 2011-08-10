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
	InterpolationGroups( const Texture & aInterpolationGroupsTexture, unsigned int aSegmentsCount );

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
	void updateGroups( const Texture & aInterpolationGroupsTexture, unsigned int aSegmentsCount );

	///-------------------------------------------------------------------------------------------------
	/// Gets the interpolation groups count. 
	/// 
	/// \return	The interpolation groups count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned int getGroupsCount() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the interpolation group segments count. 
	///
	/// \param	aGroupId	Identifier for a group. 
	///
	/// \return	The interpolation group segments count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned int getGroupSegmentsCount( unsigned int aGroupId ) const;

	///-------------------------------------------------------------------------------------------------
	/// Sets the interpolation group segments count. 
	///
	/// \param	aGroupId		Identifier for a group. 
	/// \param	aSegmentsCount	Number of the segments for selected group. 
	///-------------------------------------------------------------------------------------------------
	inline void setGroupSegmentsCount( unsigned int aGroupId, unsigned int aSegmentsCount );

	///-------------------------------------------------------------------------------------------------
	/// Gets a group identifier. 
	///
	/// \param	aU	the u texture coordinate. 
	/// \param	aV	the v texture coordinate. 
	///
	/// \return	The group identifier. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned int getGroupId( Real aU, Real aV ) const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the segments count. 
	///
	/// \param	aU	the u texture coordinate. 
	/// \param	aV	the v texture coordinate. 
	///
	/// \return	The segments count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned int getSegmentsCount( Real aU, Real aV ) const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the color compoment count. 
	///
	///
	/// \return	The color compoment count. 
	///----------------------------------------------------------------------------------------------------
	inline unsigned int getColorCompomentCount() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets a color of interpolation group. 
	///
	/// \param	aGroupId	Identifier for a interpolation group. 
	///
	/// \return	The color of interpolation group. 
	///----------------------------------------------------------------------------------------------------
	inline Texture::Color getColorOfGroup( unsigned int aGroupId ) const;

private:

	///----------------------------------------------------------------------------------------------------
	/// Defines an alias representing number of interpolation groups segments .
	///----------------------------------------------------------------------------------------------------
	typedef std::vector< unsigned int > InterpolationGroupsSegmentsCount;

	unsigned int mTextureHeight;   ///< The texture height

	unsigned int mTextureWidth;	///< The texture width

	unsigned int * mInterpolationGroupsTexture;	///< Interpolation groups texture

	Texture::Color mInterpolationGroupsColors; ///< List of colors of the interpolation groups

	unsigned int mColorComponentCount;  ///< Number of color components

	InterpolationGroupsSegmentsCount mInterpolationGroupsSegmentsCount; ///< Number of interpolation groups segments
};

// inline functions implementation

inline InterpolationGroups::~InterpolationGroups()
{
	delete [] mInterpolationGroupsTexture;
	delete [] mInterpolationGroupsColors;
}

inline unsigned int InterpolationGroups::getGroupsCount() const
{
	return static_cast< unsigned int >( mInterpolationGroupsSegmentsCount.size() );
}

inline unsigned int InterpolationGroups::getGroupSegmentsCount( unsigned int aGroupId ) const
{
	return mInterpolationGroupsSegmentsCount[ aGroupId ];
}

inline void InterpolationGroups::setGroupSegmentsCount( unsigned int aGroupId, unsigned int aSegmentsCount )
{
	if ( aSegmentsCount == 0 )
	{
		throw StubbleException(" InterpolationGroups::setGroupSegmentsCount : segments count must be greater than 0 ");
	}
	mInterpolationGroupsSegmentsCount[ aGroupId ] = aSegmentsCount;
}

inline unsigned int InterpolationGroups::getGroupId( Real aU, Real aV ) const
{
	unsigned int x = static_cast< unsigned int > ( floor(aU * mTextureWidth - 0.5) );
	unsigned int y = static_cast< unsigned int > ( floor(aV * mTextureHeight - 0.5) );

	return *( mInterpolationGroupsTexture + y * mTextureWidth + x );
}

inline unsigned int InterpolationGroups::getSegmentsCount( Real aU, Real aV ) const
{
	unsigned int x = static_cast< unsigned int > ( floor(aU * mTextureWidth - 0.5) );
	unsigned int y = static_cast< unsigned int > ( floor(aV * mTextureHeight - 0.5) );

	return mInterpolationGroupsSegmentsCount[ *( mInterpolationGroupsTexture + y * mTextureWidth + x ) ];
}

inline unsigned int InterpolationGroups::getColorCompomentCount() const
{
	return mColorComponentCount;
}

inline Texture::Color InterpolationGroups::getColorOfGroup( unsigned int aGroupId ) const
{
	return mInterpolationGroupsColors + aGroupId * mColorComponentCount;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_INTERPOLATION_GROUPS_HPP