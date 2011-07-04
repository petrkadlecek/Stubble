#include "GuideHair.hpp"

using namespace Stubble;
using namespace HairShape;

///----------------------------------------------------------------------------------------------------
/// Default constructor. 
///----------------------------------------------------------------------------------------------------
inline GuideHair::GuideHair():
	mGuideHairVerticesCount( 0 ),
	mGuideHairVertices( 0 )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Copy constructor. 
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GuideHair::GuideHair( const GuideHair & aCopy ):
	mRelativePosition( aCopy.mRelativePosition ),
	mWorldSpacePosition( aCopy.mWorldSpacePosition ),
	mGuideHairVertices( 0 ),
	mGuideHairVerticesCount( aCopy.mGuideHairVerticesCount )
{
	// Copy the world transformation matrix
	memcpy( reinterpret_cast< void *>( mWorldTransformMatrix ), reinterpret_cast< const void *>( aCopy.mWorldTransformMatrix ),
		sizeof( double ) * 16 );

	// Allocate memory for guide hair vertices
	mGuideHairVertices = new Vector3D< double >[ mGuideHairVerticesCount ];

	// Copy vertices
	memcpy( reinterpret_cast< void *>( mGuideHairVertices ), reinterpret_cast< const void *>( aCopy.mGuideHairVertices ),
		sizeof( Vector3D< double > ) * mGuideHairVerticesCount );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Copy operator. 
///
/// \param	aCopy	a copy. 
///
/// \return	The result of the operation. 
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const GuideHair & GuideHair::operator==( const GuideHair & aCopy )
{
	// First allocate memory for vertices
	if ( aCopy.mGuideHairVerticesCount != mGuideHairVerticesCount )
	{
		GuideHairVertices temp = aCopy.mGuideHairVerticesCount > 0 ? new Vector3D< double >[ aCopy.mGuideHairVerticesCount ] : 0;
		mGuideHairVerticesCount = aCopy.mGuideHairVerticesCount;
		delete [] mGuideHairVertices;
		mGuideHairVertices = temp;
	}

	mRelativePosition = aCopy.mRelativePosition;
	mWorldSpacePosition = aCopy.mWorldSpacePosition;

	// Copy the world transformation matrix
	memcpy( reinterpret_cast< void *>( mWorldTransformMatrix ), reinterpret_cast< const void *>( aCopy.mWorldTransformMatrix ),
		sizeof( double ) * 16 );

	// Allocate memory for vertices
	mGuideHairVertices = new Vector3D< double >[ mGuideHairVerticesCount ];

	// Copy vertices
	memcpy( reinterpret_cast< void *>( mGuideHairVertices ), reinterpret_cast< const void *>( aCopy.mGuideHairVertices ),
		sizeof( Vector3D< double > ) * mGuideHairVerticesCount );

	return *this;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Gets a relative position. 
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const UVPoint &  GuideHair::getRelativePosition() const
{
	return mRelativePosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Sets a relative position. 
///
/// \param	aRelativePosition	a relative position. 
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GuideHair::setRelativePosition( const UVPoint & aRelativePosition )
{
	mRelativePosition = aRelativePosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Gets the world position, tangent, normal, binormal and u,v coordinates of guide hair. 
///
/// \return	The position, tangent, normal, binormal and u,v coordinates of guide hair as PointOnMesh. 
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const MeshPoint & GuideHair::getGuideWorldPosition() const
{
	return mWorldSpacePosition;
}

///----------------------------------------------------------------------------------------------------
///  Resets the guide hair vertices in normal direction. 
///
/// \param	aGuideHairVerticesCount	Number of the vertices.
/// \param	aSegmentSize	Size of a segment. 
///----------------------------------------------------------------------------------------------------
inline void GuideHair::resetGuideHairVertices( unsigned int aGuideHairVerticesCount, double aSegmentSize )
{
	if ( aGuideHairVerticesCount != mGuideHairVerticesCount )
	{
		GuideHairVertices temp = aGuideHairVerticesCount > 0 ? new Vector3D< double >[ aGuideHairVerticesCount ] : 0;
		mGuideHairVerticesCount = aGuideHairVerticesCount;
		delete [] mGuideHairVertices;
		mGuideHairVertices = temp;
	}

	resetSegments( aSegmentSize );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///  Resets the segments in normal direction without changes to guide hair vertices count
///
/// \param	aSegmentSize				Size of segment. 
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GuideHair::resetSegments( double aSegmentSize )
{
	double z = aSegmentSize;
	for( GuideHairVertices it = mGuideHairVertices + 1, end = mGuideHairVertices + mGuideHairVerticesCount; it != end; ++it, 
		z += aSegmentSize )
		it->set( 0, 0, z );
}

///----------------------------------------------------------------------------------------------------
/// Gets the guide hair vertices.
///
/// \return	The guide hair vertices.
///----------------------------------------------------------------------------------------------------
inline const GuideHair::GuideHairVertices GuideHair::getGuideHairVertices() const
{
	return mGuideHairVertices;
}

///----------------------------------------------------------------------------------------------------
/// Gets the guide hair vertices count.
///
/// \return	The guide hair vertices.
///----------------------------------------------------------------------------------------------------
inline unsigned int GuideHair::getGuideHairVerticesCount() const
{
	return mGuideHairVerticesCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Gets the world transform matrix. Can be used as model-view OpenGL matrix (glMultMatrix( * ) ). 
///
/// \return	The world transform matrix. 
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const double * GuideHair::getWorldTransformMatrix() const
{
	return mWorldTransformMatrix;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Gets a guide hair vertex in world coordinates. 
///
/// \param	aIndex	Zero-based index of vertex
///
/// \return	The vertex in world coordinates. 
////////////////////////////////////////////////////////////////////////////////////////////////////
inline Vector3D< double > GuideHair::getGuideHairVertexInWorldCoordinates( unsigned int aIndex ) const
{
	Vector3D< double > guideHairVertex = mGuideHairVertices[ aIndex ];
	return Vector3D< double >( 
		guideHairVertex.x * mWorldTransformMatrix[ 0 ] +
		guideHairVertex.y * mWorldTransformMatrix[ 4 ] +
		guideHairVertex.z * mWorldTransformMatrix[ 8 ] +
		mWorldTransformMatrix[ 12 ],
		guideHairVertex.x * mWorldTransformMatrix[ 1 ] +
		guideHairVertex.y * mWorldTransformMatrix[ 5 ] +
		guideHairVertex.z * mWorldTransformMatrix[ 9 ] +
		mWorldTransformMatrix[ 13 ],
		guideHairVertex.x * mWorldTransformMatrix[ 2 ] +
		guideHairVertex.y * mWorldTransformMatrix[ 6 ] +
		guideHairVertex.z * mWorldTransformMatrix[ 10 ] +
		mWorldTransformMatrix[ 14 ] );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Finaliser. 
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GuideHair::~GuideHair()
{
	delete mGuideHairVertices;
}