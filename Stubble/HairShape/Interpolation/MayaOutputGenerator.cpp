#include "MayaOutputGenerator.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

void MayaOutputGenerator::draw() 
{
	/* TODO */
}

void MayaOutputGenerator::recalculateToLocalSpace( const MayaPositionGenerator::GeneratedPosition * aHairSpace )
{
	// For all hair
	const MayaPositionGenerator::GeneratedPosition * rootPosIt = aHairSpace; // Root positions
	MayaTypes::PositionType * posIt = mPositionData, * endPos; // Points positions
	Matrix< Real > transform;
	for ( const unsigned __int32 * hairIt = mHairData + 1, * hairEnd = mHairData + mHairCount + 1; 
		hairIt != hairEnd; ++hairIt, ++rootPosIt )
	{
		rootPosIt->mCurrentPosition.getLocalTransformMatrix( transform );
		// For every point of single hair
		for ( endPos = mPositionData + *hairIt; posIt != endPos; ++posIt )
		{
			// Make position vector from posIt
			Vector3D< Real > pos( static_cast< Real >( *posIt ),
								  static_cast< Real >( *( posIt + 1 ) ),
								  static_cast< Real >( *( posIt + 2 ) ) );
			// Transform point
			pos.transformAsPoint( transform );
			// Copy data from vector to posIt
			*posIt = static_cast< MayaTypes::PositionType >( pos.x );
			*( ++posIt ) = static_cast< MayaTypes::PositionType >( pos.y );
			*( ++posIt ) = static_cast< MayaTypes::PositionType >( pos.z );
		}
	}
}

void MayaOutputGenerator::recalculateToWorldSpace( const MayaPositionGenerator::GeneratedPosition * aHairSpace )
{
	// For all hair
	const MayaPositionGenerator::GeneratedPosition * rootPosIt = aHairSpace; // Root positions
	MayaTypes::PositionType * posIt = mPositionData, * endPos; // Points positions
	Matrix< Real > transform;
	for ( const unsigned __int32 * hairIt = mHairData + 1, * hairEnd = mHairData + mHairCount + 1; 
		hairIt != hairEnd; ++hairIt, ++rootPosIt )
	{
		rootPosIt->mCurrentPosition.getWorldTransformMatrix( transform );
		// For every point of single hair
		for ( endPos = mPositionData + *hairIt; posIt != endPos; ++posIt )
		{
			// Make position vector from posIt
			Vector3D< Real > pos( static_cast< Real >( *posIt ),
								  static_cast< Real >( *( posIt + 1 ) ),
								  static_cast< Real >( *( posIt + 2 ) ) );
			// Transform point
			pos.transformAsPoint( transform );
			// Copy data from vector to posIt
			*posIt = static_cast< MayaTypes::PositionType >( pos.x );
			*( ++posIt ) = static_cast< MayaTypes::PositionType >( pos.y );
			*( ++posIt ) = static_cast< MayaTypes::PositionType >( pos.z );
		}
	}
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble