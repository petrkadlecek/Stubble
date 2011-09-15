#include "InterpolationGroups.hpp"

#include <map>

namespace Stubble
{

namespace HairShape
{
	
namespace Interpolation
{

InterpolationGroups::InterpolationGroups( const Texture & aInterpolationGroupsTexture, unsigned __int32 aSegmentsCount ):
	mInterpolationGroupsTexture( 0 ),
	mInterpolationGroupsColors( 0 )
{
	updateGroups( aInterpolationGroupsTexture, aSegmentsCount );
}

void InterpolationGroups::updateGroups( const Texture & aInterpolationGroupsTexture, unsigned __int32 aSegmentsCount )
{
	if ( aInterpolationGroupsTexture.isAnimated() )
	{
		throw StubbleException(" InterpolationGroups::updateGroups : interpolation groups texture can not be animated ! ");
	}
	if ( aSegmentsCount == 0 )
	{
		throw StubbleException(" InterpolationGroups::setGroupSegmentsCount : segments count must be greater than 0 ");
	}
	unsigned __int32 * tempInterpolationGroupsTexture = 0;
	Texture::Color tempInterpolationGroupsColors = 0;
	try 
	{
		const unsigned __int32 tempColorComponentCount = aInterpolationGroupsTexture.getColorCompomentsCount();
		const unsigned __int32 tempTextureWidth = aInterpolationGroupsTexture.getWidth();
		const unsigned __int32 tempTextureHeight = aInterpolationGroupsTexture.getHeight();
		const unsigned __int32 size = tempTextureWidth * tempTextureHeight;
		// Prepare place for interpolation groups texture
		tempInterpolationGroupsTexture = new unsigned __int32[ size ];
		// Get texture raw data
		const Texture::Color rawData = aInterpolationGroupsTexture.getRawData();
		const Texture::Color rawDataEnd = rawData + size * tempColorComponentCount;
		// Prepare structure for colors
		typedef std::map< Texture::Color, unsigned __int32, Texture::ColorComparator > ColorMap;
		Texture::ColorComparator cmp( tempColorComponentCount );
		ColorMap colorMap( cmp );
		// Go through all pixels
		unsigned __int32 * iterCpy = tempInterpolationGroupsTexture;
		for( Texture::Color iter = rawData; iter < rawDataEnd; iter += tempColorComponentCount, ++iterCpy )
		{
			// First find if there is existing interpolation group with selected color or create new group
			std::pair< ColorMap::iterator, bool > color = 
				colorMap.insert( std::make_pair( iter, static_cast< unsigned __int32 > ( colorMap.size() ) ) );
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

void InterpolationGroups::exportSegmentsCountToFile( std::ostream & aOutputStream ) const
{
	// Export number of groups
	unsigned __int32 count = static_cast< unsigned __int32 >( mInterpolationGroupsSegmentsCount.size() );
	aOutputStream.write( reinterpret_cast< const char * >( & count ), sizeof( unsigned __int32 ) );
	// For every segments group
	for ( InterpolationGroupsSegmentsCount::const_iterator it = mInterpolationGroupsSegmentsCount.begin();
		it != mInterpolationGroupsSegmentsCount.end(); ++it )
	{
		// Write segments count
		aOutputStream.write( reinterpret_cast< const char * >( & *it ), sizeof( unsigned __int32 ) );
	}
}

void InterpolationGroups::importSegmentsCountFromFile( std::istream & aInputStream )
{
	// Load saved number of groups
	unsigned __int32 count, i = 0;
	aInputStream.read( reinterpret_cast< char * >( & count ), sizeof( unsigned __int32 ) );
	// For every segments group ( number of saved groups may differ from current count )
	for ( InterpolationGroupsSegmentsCount::iterator it = mInterpolationGroupsSegmentsCount.begin();
		it != mInterpolationGroupsSegmentsCount.end() && i < count ; ++it, ++i )
	{
		// Read segments count
		aInputStream.read( reinterpret_cast< char * >( & *it ), sizeof( unsigned __int32 ) );
	}
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble