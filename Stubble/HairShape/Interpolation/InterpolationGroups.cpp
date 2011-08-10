#include "InterpolationGroups.hpp"

#include <map>

namespace Stubble
{

namespace HairShape
{
	
namespace Interpolation
{

InterpolationGroups::InterpolationGroups( const Texture & aInterpolationGroupsTexture, unsigned int aSegmentsCount ):
	mInterpolationGroupsTexture( 0 ),
	mInterpolationGroupsColors( 0 )
{
	updateGroups( aInterpolationGroupsTexture, aSegmentsCount );
}

void InterpolationGroups::updateGroups( const Texture & aInterpolationGroupsTexture, unsigned int aSegmentsCount )
{
	if ( /* TODO aInterpolationGroupsTexture.isAnimated()*/ false )
	{
		throw StubbleException(" InterpolationGroups::updateGroups : interpolation groups texture can not be animated ! ");
	}
	if ( aSegmentsCount == 0 )
	{
		throw StubbleException(" InterpolationGroups::setGroupSegmentsCount : segments count must be greater than 0 ");
	}
	unsigned int * tempInterpolationGroupsTexture = 0;
	Texture::Color tempInterpolationGroupsColors = 0;
	try 
	{
		const unsigned int tempColorComponentCount = 4; // TODO aInterpolationGroupsTexture.getComponentCount();
		const unsigned int tempTextureWidth = aInterpolationGroupsTexture.getWidth();
		const unsigned int tempTextureHeight = aInterpolationGroupsTexture.getHeight();
		const unsigned int size = tempTextureWidth * tempTextureHeight;
		// Prepare place for interpolation groups texture
		tempInterpolationGroupsTexture = new unsigned int[ size ];
		// Get texture raw data
		const Texture::Color rawData = 0; // TODO aInterpolationGroupsTexture.getRawData();
		const Texture::Color rawDataEnd = rawData + size * tempColorComponentCount;
		// Prepare structure for colors
		typedef std::map< Texture::Color, unsigned int, Texture::ColorComparator< tempColorComponentCount > > ColorMap;
		ColorMap colorMap;
		// Go through all pixels
		unsigned int * iterCpy = tempInterpolationGroupsTexture;
		for( Texture::Color iter = rawData; iter < rawDataEnd; iter += tempColorComponentCount, ++iterCpy )
		{
			// First find if there is existing interpolation group with selected color or create new group
			std::pair< ColorMap::iterator, bool > color = 
				colorMap.insert( std::make_pair( iter, static_cast< unsigned int > ( colorMap.size() ) ) );
			// Set group id
			*iterCpy = color.first->second;
		}
		// Now prepare array with colors
		tempInterpolationGroupsColors = new float[ colorMap.size() * tempColorComponentCount ];
		// For every color in map
		for( ColorMap::iterator it = colorMap.begin(); it != colorMap.end(); ++it )
		{
			// Copy color from map to array
			memcpy( tempInterpolationGroupsColors + it->second * tempColorComponentCount, it->first, 
				tempColorComponentCount * sizeof( float ) );
		}
		// Now create vector with segments count
		InterpolationGroupsSegmentsCount tempInterpolationGroupsSegmentsCount( colorMap.size(), aSegmentsCount );
		// Finally dealocate old arrays
		delete [] mInterpolationGroupsTexture;
		delete [] mInterpolationGroupsColors;
		mInterpolationGroupsSegmentsCount.clear();
		// Copy new values
		mInterpolationGroupsColors = tempInterpolationGroupsColors;
		mInterpolationGroupsTexture = tempInterpolationGroupsTexture;
		std::swap( tempInterpolationGroupsSegmentsCount, mInterpolationGroupsSegmentsCount );
		mTextureHeight = tempTextureHeight;
		mTextureWidth = tempTextureWidth;
		mColorComponentCount = tempColorComponentCount;
	}
	catch( ... )
	{
		delete tempInterpolationGroupsTexture;
		delete tempInterpolationGroupsColors;
		throw StubbleException(" InterpolationGroups::updateGroups : \
							   unknown error ( most likely insufficient memory ) ! ");
	}
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble