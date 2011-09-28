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
	mHairProperties = & aHairProperties;
	/* TODO */
	// Testing version only
	mOutputGenerator.beginOutput( mPositionGenerator.getHairCount(), aHairProperties.getInterpolationGroups().getGroupSegmentsCount( 0 ) );
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
		typename tOutputGenerator::NormalType * np = mOutputGenerator.normalPointer();
		typename tOutputGenerator::ColorType * cp = mOutputGenerator.colorPointer();
		typename tOutputGenerator::OpacityType * op = mOutputGenerator.opacityPointer();
		typename tOutputGenerator::WidthType * wp = mOutputGenerator.widthPointer();
		Real z = 0;
		Stubble::Matrix< Real > transform;
		currPos.getWorldTransformMatrix( transform );
		for ( unsigned __int32 j = 0; j < segc; ++j )
		{
			// Handle positions
			Vector3D< Real > seg = Vector3D< Real >( 0, 0, z ).transformAsPoint( transform );
			*(tp++) = static_cast< typename tOutputGenerator::PositionType >( seg.x );
			*(tp++) = static_cast< typename tOutputGenerator::PositionType >( seg.y );
			*(tp++) = static_cast< typename tOutputGenerator::PositionType >( seg.z );
			z += 0.5f;
			// Handle color
			*(cp++) = static_cast< typename tOutputGenerator::ColorType >( 1.0f );
			*(cp++) = static_cast< typename tOutputGenerator::ColorType >( 0.0f );
			*(cp++) = static_cast< typename tOutputGenerator::ColorType >( 0.0f );
			// Handle opacity
			*(op++) = static_cast< typename tOutputGenerator::OpacityType >( 0.5f );
			*(op++) = static_cast< typename tOutputGenerator::OpacityType >( 0.5f );
			*(op++) = static_cast< typename tOutputGenerator::OpacityType >( 0.5f );
			// Handle widths
			*(wp++) = 0.075f;
			// Handle normals
			Vector3D< Real > n = Vector3D< Real >( 1, 0, 0 ).transformAsPoint( transform );
			*(np++) = static_cast< typename tOutputGenerator::NormalType >( n.x );
			*(np++) = static_cast< typename tOutputGenerator::NormalType >( n.y );
			*(np++) = static_cast< typename tOutputGenerator::NormalType >( n.z );
		}
		mOutputGenerator.endHair( segc );
	}
	mOutputGenerator.endOutput();

}

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	applyScale( Point * aPoints, unsigned __int32 aCount, const MeshPoint &aRestPosition )
{
	// Get the scale factor
	typename OutputGenerator::PositionType scale = static_cast< typename OutputGenerator::PositionType >( 
		mHairProperties->getScale() * mHairProperties->getScaleTexture().
		realAtUV( aRestPosition.getUCoordinate(), aRestPosition.getVCoordinate() ) );
	// Scale every point
	for ( Point * end = aPoints + aCount, *it = aPoints; it != end; ++it )
	{
		*it *= scale;
	}
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	transformPoints( Point * aPoints, unsigned __int32 aCount, const Matrix & aTransformMatrix )
{
	// For every point
	for ( Point * end = aPoints + aCount, *it = aPoints;
	it != end; ++it )
	{
		it->transformAsPoint( aTransformMatrix );
	}
}


} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_GENERATOR_TEMPLATE_HPP