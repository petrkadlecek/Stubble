#ifndef STUBBLE_HAIR_GENERATOR_TEMPLATE_HPP
#define STUBBLE_HAIR_GENERATOR_TEMPLATE_HPP

#include "Common/CommonFunctions.hpp"
#include "Common/CatmullRomUtilities.hpp"

#include "HairGenerator.hpp"

#include <cmath>

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

template< typename tPositionGenerator, typename tOutputGenerator >
void inline HairGenerator< tPositionGenerator, tOutputGenerator >::generate( const HairProperties & aHairProperties )
{
	// Store pointer to hair properties, so we don't need to send it to every function
	mHairProperties = & aHairProperties;
	// Get max points count = segments + 1 ( + 2 for duplicate of first and last point )
	const unsigned __int32 maxPointsCount = aHairProperties.getInterpolationGroups().getMaxSegmentsCount() + 3;
	// Prepare local buffers for hair
	Point * points = new Point[ maxPointsCount ];
	// The first point always equals the second one, so it will not be included in many calculations
	Point * pointsPlusOne = points + 1; 
	Point * pointsStrand = new Point[ maxPointsCount ];
	Vector * tangents = new Vector[ maxPointsCount ];
	Vector * tangentsPlusOne = tangents + 1; 
	// Prepare matrices
	Matrix localToRest, restToCurr, localToCurr, restToLocal;
	// Resets random generator
	mRandom.reset();
	// Start output
	mOutputGenerator.beginOutput( mPositionGenerator.getHairCount(), maxPointsCount );
	// For every main hair
	for ( unsigned __int32 i = 0; i < mPositionGenerator.getHairCount(); ++i )
	{
		// Generate position
		MeshPoint currPos;
		MeshPoint restPos;
		mPositionGenerator.generate( currPos, restPos );
		// Determine cut factor
		PositionType cutFactor = aHairProperties.getCutTexture().realAtUV( restPos.getUCoordinate(), restPos.getVCoordinate() );
		if ( cutFactor == 0 )
		{
			continue; // The hair has been cut at root
		}
		// Get interpolation group
		unsigned __int32 groupId = aHairProperties.getInterpolationGroups().
			getGroupId( restPos.getUCoordinate(), restPos.getVCoordinate() );
		// Get points count = segments count + 1
		unsigned __int32 ptsCountBeforeCut = aHairProperties.getInterpolationGroups().
			getGroupSegmentsCount( groupId ) + 1;
		// Calculate points count after cut, if cut < 1 than we need to include two more point for cut calculation
		unsigned __int32 ptsCountAfterCut = 
			static_cast< unsigned __int32 >( std::ceil( cutFactor * ptsCountBeforeCut ) ) + 2;
		// Limit pts count
		ptsCountAfterCut = ptsCountBeforeCut < ptsCountAfterCut ? ptsCountBeforeCut : ptsCountAfterCut;
		// Interpolate points of hair from closest guides
		interpolateFromGuides( pointsPlusOne, ptsCountAfterCut, restPos, groupId );
		// Apply scale to points 
		applyScale( pointsPlusOne, ptsCountAfterCut, restPos );
		// Get local to rest pose transform matrix
		restPos.getWorldTransformMatrix( localToRest );
		// Convert positions to rest pose world space
		transformPoints( pointsPlusOne, ptsCountAfterCut, localToRest );
		// Apply frizz and kink to points 
		applyFrizz( pointsPlusOne, ptsCountAfterCut, ptsCountBeforeCut, restPos );
		applyKink( pointsPlusOne, ptsCountAfterCut, restPos );
		// Calculate rest pose world space to current world space transform
		restPos.getLocalTransformMatrix( restToLocal );
		currPos.getWorldTransformMatrix( localToCurr );
		restToCurr = localToCurr * restToLocal;
		// Select hair color, opacity and width
		selectHairColorOpacityWidth( restPos );
		// Convert positions to current world space
		transformPoints( pointsPlusOne, ptsCountAfterCut, restToCurr );
		// Duplicate first and last point ( last points need to be duplicated, 
		// only if cut has not decreased points count, so we will use total points count )
		// These duplicated points are used for curve points calculation at any given param t
		copyToLastAndFirst( points, ptsCountBeforeCut + 2 );
		// Calculate tangents
		calculateTangents( tangentsPlusOne, pointsPlusOne, ptsCountAfterCut );
		// Duplicate tangents ( same reason as with points duplication )
		copyToLastAndFirst( tangents, ptsCountBeforeCut + 2 );
		if ( aHairProperties.getMultiStrandCount() ) // Uses multi strands ?
		{
			/* TODO handle multi strands */
		}
		else // Single hair only
		{
			// Finally begin hair output ( first and last points are duplicated )
			mOutputGenerator.beginHair( ptsCountAfterCut + 2 );
			// Generate final hair : calculates normals, colors, opacity, width and may reject some points,
			// so final points count is returned ( including two duplicated points : first and last )
			unsigned __int32 pointsCount = generateHair( pointsPlusOne, tangentsPlusOne, ptsCountAfterCut, 
				ptsCountBeforeCut, restPos, cutFactor );
			// End hair generation
			mOutputGenerator.endHair( pointsCount );
		}
		/*// Begin hair
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
		mOutputGenerator.endHair( segc );*/
	}
	mOutputGenerator.endOutput();
	// Release memory of local buffers
	delete [] points;
	delete [] pointsStrand;
	delete [] tangents;
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	interpolateFromGuides( Point * aPoints, unsigned __int32 aCount, const MeshPoint &aRestPosition, 
	unsigned __int32 aInterpolationGroupId )
{
	/* TODO : interpolate from closest guides */
	PositionType z = 0;
	for ( Point * end = aPoints + aCount, *it = aPoints; it != end; ++it )
	{
		*it = Point( 0, 0, z );
		z += 0.5f;
	}
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	applyScale( Point * aPoints, unsigned __int32 aCount, const MeshPoint &aRestPosition )
{
	// Get the scale factor
	PositionType scale = static_cast< PositionType >( 
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
	applyFrizz( Point * aPoints, unsigned __int32 aCount, unsigned __int32 aCurvePointsCount, 
	const MeshPoint &aRestPosition )
{
	/* TODO */
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	applyKink( Point * aPoints, unsigned __int32 aCount, const MeshPoint &aRestPosition )
{
	/* TODO */
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

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	calculateTangents( Vector * aTangents, const Point * aPoints, unsigned __int32 aCount )
{
	const Point * pre = aPoints - 1; // Previous point
	const Point * succ = aPoints + 1; // Succ. point
	// For every tangent
	for ( Vector * end = aTangents + aCount, *it = aTangents;
	it != end; ++it, ++pre, ++succ )
	{
		// Catmull-rom tangent calculation
		*it = ( *succ - *pre ) * 0.5;
	}
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline typename HairGenerator< tPositionGenerator, tOutputGenerator >::Vector 
	HairGenerator< tPositionGenerator, tOutputGenerator >::
	calculateNormal( const Point * aPoints, const Vector * aTangents,
		const Vector & aPreviousNormal )
{
	/*// The double reflection normal calculation
	Vector v1 = *aPoints - *( aPoints - 1 );
	NormalType c1 = v1.sizePwr2();
	Vector rL = aPreviousNormal - v1 * ( 2 / c1 ) * ( Vector::dotProduct( v1, aPreviousNormal ) ); 
	Vector tL = *( aTangents - 1 ) - v1 * ( 2 / c1 ) * ( Vector::dotProduct( v1, *( aTangents - 1 ) ) ); 
	Vector v2 = *aTangents - tL;
	NormalType c2 = v2.sizePwr2();
	return rL - v2 * ( 2 / c2 ) * ( Vector::dotProduct( v2, rL ) );*/
	return aPreviousNormal;
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	selectHairColorOpacityWidth( const MeshPoint &aRestPosition )
{
	// Store uv coordinates
	const Real u = aRestPosition.getUCoordinate();
	const Real v = aRestPosition.getVCoordinate();
	// Determine whether the hair is mutant
	if ( mRandom.uniformNumber() < 
		mHairProperties->getPercentMutantHair() * mHairProperties->getPercentMutantHairTexture().realAtUV( u, v ) )
	{
		// Select mutant hair color as root color
		mixColor( mRootColor, mHairProperties->getMutantHairColor(),
			mHairProperties->getMutantHairColorTexture().colorAtUV( u, v ) );
		// Copy it to tip color
		memcpy( reinterpret_cast< void * >( mTipColor ), reinterpret_cast< const void * >( mRootColor ), 
			sizeof( ColorType ) * 3 );
	}
	else
	{
		// Select root color
		mixColor( mRootColor, mHairProperties->getRootColor(),
			mHairProperties->getRootColorTexture().colorAtUV( u, v ) );
		// Select tip color
		mixColor( mTipColor, mHairProperties->getTipColor(),
			mHairProperties->getTipColorTexture().colorAtUV( u, v ) );
	}
	// Handle opacity
	mRootOpacity[ 2 ] = mRootOpacity[ 1 ] = mRootOpacity[ 0 ] = static_cast< OpacityType >( 
		mHairProperties->getRootOpacity() * mHairProperties->getRootOpacityTexture().realAtUV( u, v ) );
	mTipOpacity[ 2 ] = mTipOpacity[ 1 ] = mTipOpacity[ 0 ] = static_cast< OpacityType >( 
		mHairProperties->getTipOpacity() * mHairProperties->getTipOpacityTexture().realAtUV( u, v ) );
	// Handle width
	mRootWidth = static_cast< WidthType >( 
		mHairProperties->getRootThickness() * mHairProperties->getRootThicknessTexture().realAtUV( u, v ) );
	mTipWidth = static_cast< WidthType >( 
		mHairProperties->getTipThickness() * mHairProperties->getTipThicknessTexture().realAtUV( u, v ) );
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline bool HairGenerator< tPositionGenerator, tOutputGenerator >::
	skipPoint( const Point * aPoints, const Vector * aTangents )
{
	/* TODO */
	return false;
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline unsigned __int32 HairGenerator< tPositionGenerator, tOutputGenerator >::
	generateHair( Point * aPoints, Vector * aTangents, unsigned __int32 aCount, 
		unsigned __int32 aCurvePointsCount, const MeshPoint & aRestPosition, PositionType aCutFactor )
{
	// Select output pointers and skip first point ( first equals second, will be copied later )
	PositionType *posOutIt = mOutputGenerator.positionPointer() + 3;
	NormalType *normalOutIt = mOutputGenerator.normalPointer() + 3;
	ColorType *colorIt = mOutputGenerator.colorPointer() + 3;
	OpacityType *opacityIt = mOutputGenerator.opacityPointer() + 3;
	WidthType *widthIt = mOutputGenerator.widthPointer() + 1;
	// We need to store previous normal, for next normal calculation
	// First point has normal in tangent direction
	Vector normal = Vector( static_cast< NormalType >( aRestPosition.getTangent().x ),
							static_cast< NormalType >( aRestPosition.getTangent().y ),
							static_cast< NormalType >( aRestPosition.getTangent().z ) ); 
	unsigned __int32 count = 0;
	// Select parameter step and iteration end
	--aCount; // Points count -> segments count
	PositionType step = 1.0f / aCount;
	bool iterationEnd = false;
	// We have to ensure that cut procedure is only executed if needed
	aCutFactor = aCutFactor == 1 ? 2 : aCutFactor; 
	// Iterate with curve parameter t until iteration end is signaled
	for ( PositionType t = 0, oneMinusT = 1; !iterationEnd; 
		t += step, oneMinusT = 1 - t, ++aPoints, ++aTangents )
	{
		if ( t > aCutFactor ) // Reached curve cut end
		{
			// Calculate new point position
			Point newPos;
			catmullRomEval( newPos, aPoints - 2, ( t - aCutFactor ) * aCount );
			t = aCutFactor;
			iterationEnd = true;
			// Move current point to next and calculate tangent
			aTangents[ 1 ] = ( aPoints[ 2 ] - newPos ) * 0.5;
			aPoints[ 1 ] = *aPoints;
			// Set current point and calculate tangent
			*aPoints = newPos;
			*aTangents = ( aPoints[ 1 ] - *( aPoints - 1 ) ) * 0.5;
		}
		else // Did not reach 
		{
			iterationEnd = t >= 1; // Reached curve end ?
			if ( !iterationEnd && t != 0 && skipPoint( aPoints, aTangents ) )
			{
				continue; // Skip this point
			}
		}
		// Begin output
		// Output position
		memcpy( reinterpret_cast< void * >( posOutIt ), reinterpret_cast< const void * >( aPoints  ), 
			sizeof( PositionType ) * 3 );
		posOutIt += 3;
		// Output normal
		normal = calculateNormal( aPoints, aTangents, normal );
		memcpy( reinterpret_cast< void * >( normalOutIt ), reinterpret_cast< const void * >( &normal ),
				sizeof( NormalType ) * 3 );
		normalOutIt += 3;
		// Finally output color, opacity, width
		for ( unsigned __int32 j = 0; j < 3; ++j )
		{
			* ( colorIt++ ) = t * mTipColor[ j ] + oneMinusT * mRootColor[ j ];
			* ( opacityIt++ ) = t * mTipOpacity[ j ] + oneMinusT * mRootOpacity[ j ];      
		}
		* ( widthIt++ ) = t * mTipWidth + oneMinusT * mRootWidth; 
		// Increase segments count
		++count;
	}

	// Finally duplicates first & last hair points
	count += 2;
	copyToLastAndFirst< PositionType, 3 > ( mOutputGenerator.positionPointer(), count );
	copyToLastAndFirst< NormalType, 3 > ( mOutputGenerator.normalPointer(), count );
	copyToLastAndFirst< ColorType, 3 > ( mOutputGenerator.colorPointer(), count );
	copyToLastAndFirst< OpacityType, 3 > ( mOutputGenerator.opacityPointer(), count );
	copyToLastAndFirst< WidthType > ( mOutputGenerator.widthPointer(), count );      
	// Return number of outputed hair points
	return count;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_GENERATOR_TEMPLATE_HPP