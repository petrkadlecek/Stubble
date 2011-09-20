#ifndef STUBBLE_HAIR_GENERATOR_TEMPLATE_HPP
#define STUBBLE_HAIR_GENERATOR_TEMPLATE_HPP

#include "HairGenerator.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

template< typename tPositionGenerator, typename tOutputGenerator >
void inline HairGenerator< tPositionGenerator, tOutputGenerator >::generate( const HairProperties & aHairProperties )
{
	/* TODO */
	// Testing version only
	mOutputGenerator.beginOutput( mPositionGenerator.getHairCount(), aHairProperties.getInterpolationGroups().getGroupSegmentsCount( 0 ),false );
	for ( unsigned __int32 i = 0; i < mPositionGenerator.getHairCount(); ++i )
	{
		// Generate position
		MeshPoint currPos;
		MeshPoint restPos;
		mPositionGenerator.generate( currPos, restPos );
		// Begin hair
		unsigned __int32 segc = aHairProperties.getInterpolationGroups().getSegmentsCount( restPos.getUCoordinate(), restPos.getVCoordinate() );
		mOutputGenerator.beginHair( segc );
		// Prepare iterators
		typename tOutputGenerator::PositionType * tp = mOutputGenerator.positionPointer();
		typename tOutputGenerator::ColorType * cp = mOutputGenerator.colorPointer();
		typename tOutputGenerator::WidthType * wp = mOutputGenerator.widthPointer();
		Real z = 0;
		for ( unsigned __int32 j = 0; j < segc; ++j )
		{
			// Handle positions
			Vector3D< Real > seg = currPos.toWorld( Vector3D< Real >( 0, 0, z ) );
			*(tp++) = static_cast< typename tOutputGenerator::PositionType >( seg.x );
			*(tp++) = static_cast< typename tOutputGenerator::PositionType >( seg.y );
			*(tp++) = static_cast< typename tOutputGenerator::PositionType >( seg.z );
			z += 0.5f;
			// Handle color
			*(cp++) = static_cast< typename tOutputGenerator::PositionType >( 1.0f );
			*(cp++) = static_cast< typename tOutputGenerator::PositionType >( 0.0f );
			*(cp++) = static_cast< typename tOutputGenerator::PositionType >( 0.0f );
			*(wp++) = 0.075f;
		}
		mOutputGenerator.endHair( segc );
	}
	mOutputGenerator.endOutput();

}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_GENERATOR_TEMPLATE_HPP