#ifndef STUBBLE_HAIR_GENERATOR_TEMPLATE_HPP
#define STUBBLE_HAIR_GENERATOR_TEMPLATE_HPP

#include "Common/CommonFunctions.hpp"
#include "Common/CatmullRomUtilities.hpp"

#include "HairGenerator.hpp"

#include <algorithm>
#include <cmath>
#include <rx.h>

#undef min
#undef max

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
	Point * pointsStrandPlusOne = pointsStrand + 1; 
	Vector * tangents = new Vector[ maxPointsCount ];
	Vector * tangentsPlusOne = tangents + 1; 
	Vector * normals = new Vector[ maxPointsCount ];
	Vector * binormals = new Vector[ maxPointsCount ];
	// Prepare matrix
	Matrix localToCurr;
	// Resets random generator
	mRandom.reset();
	// Indices
	IndexType hairIndex = static_cast< IndexType >( mPositionGenerator.getHairStartIndex() * 
		mHairProperties->getMultiStrandCount() );
	IndexType strandIndex = static_cast< IndexType >( mPositionGenerator.getHairStartIndex() );
	// Start output
	mOutputGenerator.beginOutput( mPositionGenerator.getHairCount() * 
		std::max( aHairProperties.getMultiStrandCount(), static_cast< unsigned __int32 >( 1 ) ), maxPointsCount );
	// For every main hair
	for ( unsigned __int32 i = 0; i < mPositionGenerator.getHairCount(); ++i, ++strandIndex )
	{
		// Generate position
		MeshPoint currPos;
		MeshPoint restPos;
		Real displaceFactor = mHairProperties->getDisplacement();
		if ( displaceFactor == 0 )
		{
			mPositionGenerator.generate( currPos, restPos );
		}
		else
		{
			mPositionGenerator.generate( currPos, restPos, mHairProperties->getDisplacementTexture(), 
				mHairProperties->getDisplacement() );
		}
		// Determine cut factor
		PositionType cutFactor = static_cast< PositionType >( 
			aHairProperties.getCutTexture().realAtUV( restPos.getUCoordinate(), restPos.getVCoordinate() ) );
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
		// Apply frizz and kink to points 
		applyFrizz( pointsPlusOne, ptsCountAfterCut, ptsCountBeforeCut, restPos );
		applyKink( pointsPlusOne, ptsCountAfterCut, ptsCountBeforeCut, restPos );
		// Calculate local space to current world space transform
		currPos.getWorldTransformMatrix( localToCurr );
		// Select hair color, opacity and width
		selectHairColorOpacityWidth( restPos );
		if ( aHairProperties.getMultiStrandCount() ) // Uses multi strands ?
		{
			// Duplicate first and last point ( last points need to be duplicated, 
			// only if cut has not decreased points count, so we will use total points count )
			// These duplicated points are used for curve points calculation at any given param t
			copyToLastAndFirst( points, ptsCountBeforeCut + 2 );
			// Calculate tangents
			calculateTangents( tangentsPlusOne, pointsPlusOne, ptsCountAfterCut );
			// Duplicate tangents ( same reason as with points duplication )
			copyToLastAndFirst( tangents, ptsCountBeforeCut + 2 );
			// Create full rotation minimizing frame for main hair
			calculateNormalsAndBinormals( normals, binormals, pointsPlusOne, tangentsPlusOne, ptsCountAfterCut );
			// Get multi-strands properties
			selectMultiStrandProperties( restPos, ptsCountBeforeCut );
			// Generate all hair in strand
			for ( unsigned __int32 j = 0; j < aHairProperties.getMultiStrandCount(); ++j )
			{
				// First generate points
				generateHairInStrand( pointsStrandPlusOne, ptsCountAfterCut, ptsCountBeforeCut, pointsPlusOne,
					normals, binormals );
				// Convert positions to current world space
				transformPoints( pointsStrandPlusOne, ptsCountAfterCut, localToCurr );
				// Duplicate first and last point ( last points need to be duplicated, 
				// only if cut has not decreased points count, so we will use total points count )
				// These duplicated points are used for curve points calculation at any given param t
				copyToLastAndFirst( pointsStrand, ptsCountBeforeCut + 2 );
				// Calculate tangents
				calculateTangents( tangentsPlusOne, pointsStrandPlusOne, ptsCountAfterCut );
				// Duplicate tangents ( same reason as with points duplication )
				copyToLastAndFirst( tangents, ptsCountBeforeCut + 2 );
				// Finally begin hair output ( first and last points are duplicated )
				mOutputGenerator.beginHair( ptsCountAfterCut + 2 );
				// Output indices and uv coordinates
				outputHairIndexAndUVs( ++hairIndex, strandIndex, restPos );
				// Generate final hair : calculates normals, colors, opacity, width and may reject some points,
				// so final points count is returned ( including two duplicated points : first and last )
				unsigned __int32 pointsCount = generateHair( pointsStrandPlusOne, tangentsPlusOne, ptsCountAfterCut, 
					ptsCountBeforeCut, restPos, cutFactor );
				// End hair generation
				mOutputGenerator.endHair( pointsCount );
			}
		}
		else // Single hair only
		{
			// Convert positions to current world space
			transformPoints( pointsPlusOne, ptsCountAfterCut, localToCurr );
			// Duplicate first and last point ( last points need to be duplicated, 
			// only if cut has not decreased points count, so we will use total points count )
			// These duplicated points are used for curve points calculation at any given param t
			copyToLastAndFirst( points, ptsCountBeforeCut + 2 );
			// Calculate tangents
			calculateTangents( tangentsPlusOne, pointsPlusOne, ptsCountAfterCut );
			// Duplicate tangents ( same reason as with points duplication )
			copyToLastAndFirst( tangents, ptsCountBeforeCut + 2 );
			// Finally begin hair output ( first and last points are duplicated )
			mOutputGenerator.beginHair( ptsCountAfterCut + 2 );
			// Output indices and uv coordinates
			outputHairIndexAndUVs( ++hairIndex, strandIndex, restPos );
			// Generate final hair : calculates normals, colors, opacity, width and may reject some points,
			// so final points count is returned ( including two duplicated points : first and last )
			unsigned __int32 pointsCount = generateHair( pointsPlusOne, tangentsPlusOne, ptsCountAfterCut, 
				ptsCountBeforeCut, restPos, cutFactor );
			// End hair generation
			mOutputGenerator.endHair( pointsCount );
		}
	}
	mOutputGenerator.endOutput();
	// Release memory of local buffers
	delete [] points;
	delete [] pointsStrand;
	delete [] tangents;
	delete [] normals;
	delete [] binormals;
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
	// Get the scale factor = scale * scaleTexture * ( 1 - randScale * randScaleTexture * random )
	PositionType scale = static_cast< PositionType >( 
		mHairProperties->getScale() * mHairProperties->getScaleTexture().
		realAtUV( aRestPosition.getUCoordinate(), aRestPosition.getVCoordinate() ) *
		( 1 - mHairProperties->getRandScale() * mHairProperties->getRandScaleTexture().
		realAtUV( aRestPosition.getUCoordinate(), aRestPosition.getVCoordinate() ) * mRandom.uniformNumber() ) );
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
	// Gather Frizz properties for this hair
	Real u = aRestPosition.getUCoordinate();
	Real v = aRestPosition.getVCoordinate();
	Real freqX = mHairProperties->getFrizzXFrequency() * mHairProperties->getFrizzXFrequencyTexture().
		realAtUV( u, v );
	Real freqY = mHairProperties->getFrizzYFrequency() * mHairProperties->getFrizzYFrequencyTexture().
		realAtUV( u, v );
	Real freqZ = mHairProperties->getFrizzZFrequency() * mHairProperties->getFrizzZFrequencyTexture().
		realAtUV( u, v );
	Real rootDisplaceFactor = mHairProperties->getRootFrizz() * mHairProperties->getRootFrizzTexture().
		realAtUV( u, v );
	Real tipDisplaceFactor = mHairProperties->getTipFrizz() * mHairProperties->getTipFrizzTexture().
		realAtUV( u, v );
	Real frizzAnimFactor = mHairProperties->getFrizzAnim() * mHairProperties->getFrizzAnimTexture().
		realAtUV( u, v );
	Real frizzStaticFactor = 1 - frizzAnimFactor;
	Real frizzTimeFactor = mHairProperties->getFrizzAnimSpeed() * mHairProperties->getFrizzAnimSpeedTexture().
		realAtUV( u, v ) * mHairProperties->getCurrentTime(); 
	RtFloat in[ 3 ], staticNoise[ 3 ], animNoise[ 3 ], displace[ 3 ];
	// Calculate static noise at root
	Vector3D< Real > root = aRestPosition.getPosition();
	in[ 0 ] = static_cast< RtFloat >( freqX * root.x );
	in[ 1 ] = static_cast< RtFloat >( freqY * root.y );
	in[ 2 ] = static_cast< RtFloat >( freqZ * root.z );
	RxNoise( 3, in, 3, staticNoise );
	// Calculate anim noise at root
	in[ 0 ] = static_cast< RtFloat >( freqX * root.x + mHairProperties->getFrizzAnimDirection().x * frizzTimeFactor );
	in[ 1 ] = static_cast< RtFloat >( freqY * root.y + mHairProperties->getFrizzAnimDirection().y * frizzTimeFactor );
	in[ 2 ] = static_cast< RtFloat >( freqZ * root.z + mHairProperties->getFrizzAnimDirection().z * frizzTimeFactor );
	RxNoise( 3, in, 3, animNoise );
	// Combine anim and static noise to final displace vector, also transform noise from <0,1> -> <-1,1>
	displace[ 0 ] = 2 * static_cast< RtFloat >( frizzStaticFactor * ( staticNoise[ 0 ] - 0.5f ) + 
		frizzAnimFactor * ( animNoise[ 0 ] - 0.5f ) );
	displace[ 1 ] = 2 * static_cast< RtFloat >( frizzStaticFactor * ( staticNoise[ 1 ] - 0.5f ) + 
		frizzAnimFactor * ( animNoise[ 1 ] - 0.5f ) );
	displace[ 2 ] = 2 * static_cast< RtFloat >( frizzStaticFactor * ( staticNoise[ 2 ] - 0.5f ) + 
		frizzAnimFactor * ( animNoise[ 2 ] - 0.5f ) );
	// Calculate max displace factor
	Real maxFactor = std::max( rootDisplaceFactor, tipDisplaceFactor );  
	// Curve t param
	RtFloat step = 1.0f / ( aCurvePointsCount - 1 ), t = step;
	// For every point on cut curve except the first one
	for( Point * it = aPoints + 1, * end = aPoints + aCount; it != end; t += step, ++it )
	{
		// Calculate displace factor
		RtFloat t2 = t * t; 
		RtFloat tipT = t2, rootT = 4 * ( t - t2 );
		Real displaceFactor = std::min( rootDisplaceFactor * rootT + tipDisplaceFactor * tipT, maxFactor );
		// Apply displace
		it->x += static_cast< PositionType >( displaceFactor * displace[ 0 ] );
		it->y += static_cast< PositionType >( displaceFactor * displace[ 1 ] );
		it->z += static_cast< PositionType >( displaceFactor * displace[ 2 ] );
	} 
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	applyKink( Point * aPoints, unsigned __int32 aCount, unsigned __int32 aCurvePointsCount, 
	const MeshPoint &aRestPosition )
{
	// Gather kink properties for this hair
	Real u = aRestPosition.getUCoordinate();
	Real v = aRestPosition.getVCoordinate();
	Real freqX = mHairProperties->getKinkXFrequency() * mHairProperties->getKinkXFrequencyTexture().
		realAtUV( u, v );
	Real freqY = mHairProperties->getKinkYFrequency() * mHairProperties->getKinkYFrequencyTexture().
		realAtUV( u, v );
	Real freqZ = mHairProperties->getKinkZFrequency() * mHairProperties->getKinkZFrequencyTexture().
		realAtUV( u, v );
	Real rootDisplaceFactor = mHairProperties->getRootKink() * mHairProperties->getRootKinkTexture().
		realAtUV( u, v );
	Real tipDisplaceFactor = mHairProperties->getTipKink() * mHairProperties->getTipKinkTexture().
		realAtUV( u, v );
	// Curve t param
	Real step = 1.0f / ( aCurvePointsCount - 1 ), t = step, oneMinusT = 1 - step;
	// For every point on cut curve except the first one
	for( Point * it = aPoints + 1, * end = aPoints + aCount; it != end; t += step, ++it, oneMinusT = 1 - t )
	{
		Point rest = aRestPosition.toWorld( *it );
		RtFloat in[ 3 ] = { static_cast< RtFloat >( freqX * rest.x ),
			static_cast< RtFloat >( freqY * rest.y ), 
			static_cast< RtFloat >( freqZ * rest.z ) }, 
			out[ 3 ];
		// Remember to shift noise from <0,1> to <-1,1> (factor *= 2; and out[ i ] -=0.5 solves that )
		RxNoise( 3, in, 3, out );
		Real factor = 2 * ( t * tipDisplaceFactor + oneMinusT * rootDisplaceFactor );
		it->x += static_cast< PositionType >( ( out[ 0 ] - 0.5 ) * factor );
		it->y += static_cast< PositionType >( ( out[ 1 ] - 0.5 ) * factor );
		it->z += static_cast< PositionType >( ( out[ 2 ] - 0.5 ) * factor );
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
		it->normalize();
	}
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline typename HairGenerator< tPositionGenerator, tOutputGenerator >::Vector 
	HairGenerator< tPositionGenerator, tOutputGenerator >::
	calculateNormal( const Point * aPoints, const Vector * aTangents,
		const Vector & aPreviousNormal )
{
	// The double reflection normal calculation
	Vector v1 = *aPoints - *( aPoints - 1 );
	NormalType c1 = v1.sizePwr2();
	Vector rL = aPreviousNormal - v1 * ( 2 / c1 ) * ( Vector::dotProduct( v1, aPreviousNormal ) ); 
	Vector tL = *( aTangents - 1 ) - v1 * ( 2 / c1 ) * ( Vector::dotProduct( v1, *( aTangents - 1 ) ) ); 
	Vector v2 = *aTangents - tL;
	NormalType c2 = v2.sizePwr2();
	return rL - v2 * ( 2 / c2 ) * ( Vector::dotProduct( v2, rL ) );
	//return aPreviousNormal;
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	calculateNormalsAndBinormals( Vector * aNormals, Vector * aBinormals, const Point * aPoints, 
		const Vector * aTangents, unsigned __int32 aCount )
{
	// Select first hair point normal
	* aNormals = Vector( 0, 1, 0 );
	// Calculates first binormal
	*aBinormals = Vector::crossProduct( *aTangents, *aNormals );
	// Skip first point and tangent
	++aTangents;
	++aPoints;
	// For every point
	for ( Vector * end = aNormals + aCount, *n = aNormals + 1, *b = aBinormals + 1;
		n != end; ++n, ++b, ++aTangents, ++aPoints )
	{
		// Calculate rotation minimazing frame
		*n = calculateNormal( aPoints, aTangents, * ( n - 1 ) );
		// Calculate binormal
		*b = Vector::crossProduct( *aTangents, *n );
	}
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	applyHueValueShift( ColorType * mColor, ColorType aValueShift, ColorType aHueShift )
{
	// Create tmp
	ColorType tmp[ 3 ];
	memcpy( reinterpret_cast< void * >( tmp ), reinterpret_cast< const void * >( mColor ), 
			sizeof( ColorType ) * 3 );
	// RGB -> HSV
	RGBtoHSV( tmp, mColor );
	// Apply hue shift
	tmp[ 0 ] = circleValue( tmp[ 0 ] - aHueShift, static_cast< ColorType >( 0 ), static_cast< ColorType >( 360 ) );
	// Apply value shift
	tmp[ 2 ] = clamp( tmp[ 2 ] - aValueShift, static_cast< ColorType >( 0 ), static_cast< ColorType >( 1 ) );
	// HSV -> RGB
	HSVtoRGB( mColor, tmp );
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	selectHairColorOpacityWidth( const MeshPoint & aRestPosition )
{
	// Store uv coordinates
	const Real u = aRestPosition.getUCoordinate();
	const Real v = aRestPosition.getVCoordinate();
	// Calculate hue shift
	Real hueVar = mHairProperties->getHueVariation() * 
		mHairProperties->getHueVariationTexture().realAtUV( u, v ) * 0.5f;
	ColorType hueShift = static_cast< ColorType >( mRandom.randomReal( -hueVar, hueVar ) );
	// Calculate value shift
	Real valueVar = mHairProperties->getValueVariation() * 
		mHairProperties->getValueVariationTexture().realAtUV( u, v ) * 0.5f;
	ColorType valueShift = static_cast< ColorType >( mRandom.randomReal( -valueVar, valueVar ) );
	// Determine whether the hair is mutant
	if ( mRandom.uniformNumber() < 
		mHairProperties->getPercentMutantHair() * mHairProperties->getPercentMutantHairTexture().realAtUV( u, v ) / 100 )
	{
		// Select mutant hair color as root color
		mixColor( mRootColor, mHairProperties->getMutantHairColor(),
			mHairProperties->getMutantHairColorTexture().colorAtUV( u, v ) );
		// Applies hue-value shift
		applyHueValueShift( mRootColor, valueShift, hueShift );
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
		// Applies hue-value shift
		applyHueValueShift( mRootColor, valueShift, hueShift );
		applyHueValueShift( mTipColor, valueShift, hueShift );
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
	static const NormalType skipTreshold = 0.1f;
	// may not pass certain treshold inorder to skip current point
	return Vector::dotProduct( aTangents[ 0 ], aTangents[ - 1 ] ) < skipTreshold 
		&& Vector::dotProduct( aTangents[ 0 ], aTangents[ 1 ] ) < skipTreshold;
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
			catmullRomEval( newPos, aPoints - 2, 1 - ( t - aCutFactor ) * aCount );
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
		if ( t != 0 )
		{
			normal = calculateNormal( aPoints, aTangents, normal );
		}
		memcpy( reinterpret_cast< void * >( normalOutIt ), reinterpret_cast< const void * >( &normal ),
				sizeof( NormalType ) * 3 );
		normalOutIt += 3;
		// Finally output color, opacity, width
		for ( unsigned __int32 j = 0; j < 3; ++j )
		{
			* ( colorIt++ ) = clamp( t * mTipColor[ j ] + oneMinusT * mRootColor[ j ], 0.0f, 1.0f );
			* ( opacityIt++ ) = clamp( t * mTipOpacity[ j ] + oneMinusT * mRootOpacity[ j ], 0.0f, 1.0f );      
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

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	outputHairIndexAndUVs( IndexType aHairIndex, IndexType aStrandIndex, const MeshPoint &aRestPosition )
{
	// Store uv coordinates
	const UVCoordinateType u = static_cast< UVCoordinateType >( aRestPosition.getUCoordinate() );
	const UVCoordinateType v = static_cast< UVCoordinateType >( aRestPosition.getVCoordinate() );
	// Output indices
	* mOutputGenerator.hairIndexPointer() = aHairIndex;
	* mOutputGenerator.strandIndexPointer() = aStrandIndex;
	// Output uv coordinates
	* mOutputGenerator.hairUVCoordinatePointer() = u;
	* ( mOutputGenerator.hairUVCoordinatePointer() + 1 ) = v;
	* mOutputGenerator.strandUVCoordinatePointer() = u;
	* ( mOutputGenerator.strandUVCoordinatePointer() + 1 ) = v;
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	sampleDisk( PositionType aSampleX, PositionType aSampleY, PositionType & aCosPhi, 
	PositionType & aSinPhi, PositionType & aRadius )
{
	static const PositionType PI = 3.14159265f;
	static const PositionType PI_4 = PI / 4;
	static const PositionType PI_2 = PI / 2;
	// Transform [0-1]^2 to [-1,1]^2
	PositionType a = 2 * aSampleX - 1;
	PositionType b = 2 * aSampleY - 1;
	PositionType phi; // Angle
	// use squares instead of absolute values
	if ( a * a > b * b ) 
	{ 
		aRadius = a;
		phi = PI_4 * ( b / a );
	} 
	else 
	{
 		aRadius = b;
		phi = PI_2 - PI_4 * ( a / b );
	}
	// Calculate cos, sin
	aCosPhi = static_cast< PositionType >( cos( phi ) );
	aSinPhi = static_cast< PositionType >( sin( phi ) );
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	selectMultiStrandProperties( const MeshPoint &aRestPosition, unsigned __int32 aCurvePointsCount )
{
	// Store uv coordinates
	const Real u = aRestPosition.getUCoordinate();
	const Real v = aRestPosition.getVCoordinate();
	// Select twist and divide it by aCurvePointsCount - 1 to get twist angle for each hair point
	PositionType twist = static_cast< PositionType >( mHairProperties->getTwist() * 
		mHairProperties->getTwistTexture().realAtUV( u, v ) ) / ( aCurvePointsCount - 1 );
	// Calculate cos, sin
	mCosTwist = static_cast< PositionType >( cos( twist ) );
	mSinTwist = static_cast< PositionType >( sin( twist ) );
	// Select tip splay
	mTipSplay = static_cast< PositionType >( mHairProperties->getTipSplay() * 
		mHairProperties->getTipSplayTexture().realAtUV( u, v ) );
	// Select center splay
	mCenterSplay = static_cast< PositionType >( mHairProperties->getCenterSplay() * 
		mHairProperties->getCenterSplayTexture().realAtUV( u, v ) );
	// Select root splay
	mRootSplay = static_cast< PositionType >( mHairProperties->getRootSplay() * 
		mHairProperties->getRootSplayTexture().realAtUV( u, v ) );
	// Select randomize scale
	mRandomizeScale = static_cast< PositionType >( mHairProperties->getRandomizeStrand() * 
		mHairProperties->getRandomizeStrandTexture().realAtUV( u, v ) );
	// Select offset of tips
	mOffset = static_cast< PositionType >( mHairProperties->getOffset() * 
		mHairProperties->getOffsetTexture().realAtUV( u, v ) );
	// Select aspect ratio of disk samples
	mAspect = static_cast< PositionType >( mHairProperties->getAspect() * 
		mHairProperties->getAspectTexture().realAtUV( u, v ) );
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline void HairGenerator< tPositionGenerator, tOutputGenerator >::
	generateHairInStrand( Point * aPoints, unsigned __int32 aCount, unsigned __int32 aCurvePointsCount,
	const Point * aMainHairPoints, const Vector * aMainHairNormals, const Vector * aMainHairBinormals )
{
	// First generate relative position on disk
	PositionType cosPhi, sinPhi, radius;
	sampleDisk( static_cast< PositionType >( mRandom.uniformNumber() ), 
		static_cast< PositionType >( mRandom.uniformNumber() * mAspect ), cosPhi, sinPhi, radius );
	// Generate random scale
	PositionType scale = static_cast< PositionType >( 1 - mRandomizeScale * mRandom.uniformNumber() );
	// Curve t param
	PositionType step = 1.0f / ( aCurvePointsCount - 1 ), t = 0;
	// For every point on cut curve
	for( Point * it = aPoints, * end = aPoints + aCount; it != end; 
		t += step, ++it, ++aMainHairPoints, ++aMainHairNormals, ++aMainHairBinormals )
	{
		// Calculate radius and offset
		PositionType currRadius = radius, offset = mOffset * t * t * t;
		// Calculate final position :
		// mainHairPos + position on plane defined by normal and binormal * radius + offset in direction of normal
		// and finally scale whole thing by scale factor
		*it = ( *aMainHairPoints + ( *aMainHairNormals * cosPhi + *aMainHairBinormals * sinPhi ) * currRadius +
			*aMainHairNormals * offset ) * scale;
		// Add twist to cosPhi and sinPhi using trigonometric formulas
		PositionType newSinPhi = sinPhi * mCosTwist + mSinTwist * cosPhi;
		cosPhi *= mCosTwist;
		cosPhi -= mSinTwist * sinPhi;
		sinPhi = newSinPhi;
	} 
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_GENERATOR_TEMPLATE_HPP