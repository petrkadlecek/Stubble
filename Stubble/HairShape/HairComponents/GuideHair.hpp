#ifndef STUBBLE_GUIDE_HAIR_HPP
#define STUBBLE_GUIDE_HAIR_HPP

#include "Primitives\Vector3D.hpp"
#include "HairShape\Mesh\MeshPoint.hpp"
#include "HairShape\Mesh\UVPoint.hpp"
#include "HairShape\Generators\RandomGenerator.hpp"

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// Class implementing the actual guide hair, consists of hair vertices, transform matrices, etc.
///----------------------------------------------------------------------------------------------------
class GuideHair
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Defines an alias representing the segments.
	///----------------------------------------------------------------------------------------------------
	typedef Vector3D< double > * GuideHairVertices;

	///----------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///----------------------------------------------------------------------------------------------------
	inline GuideHair();

	///----------------------------------------------------------------------------------------------------
	/// Copy constructor. 
	///----------------------------------------------------------------------------------------------------
	inline GuideHair( const GuideHair & aCopy );

	///----------------------------------------------------------------------------------------------------
	/// Copy operator. 
	///
	/// \param	aCopy	a copy. 
	///
	/// \return	The result of the operation. 
	///----------------------------------------------------------------------------------------------------
	inline const GuideHair & operator==( const GuideHair & aCopy );

	///----------------------------------------------------------------------------------------------------
	/// Gets a relative position. 
	///----------------------------------------------------------------------------------------------------
	inline const UVPoint & getRelativePosition() const;

	///----------------------------------------------------------------------------------------------------
	/// Sets a relative position. 
	///
	/// \param	aRelativePosition	a relative position. 
	///----------------------------------------------------------------------------------------------------
	inline void setRelativePosition( const UVPoint & aRelativePosition );

	///----------------------------------------------------------------------------------------------------
	/// Gets the world position, tangent, normal, binormal and u,v coordinates of guide hair. 
	///
	/// \return	The position, tangent, normal, binormal and u,v coordinates of guide hair as PointOnMesh. 
	///----------------------------------------------------------------------------------------------------
	inline const MeshPoint & getGuideWorldPosition() const;

	///----------------------------------------------------------------------------------------------------
	///  Resets the guide hair vertices in normal direction. 
	///
	/// \param	aGuideHairVerticesCount	Number of the vertices.
	/// \param	aSegmentSize	Size of a segment. 
	///----------------------------------------------------------------------------------------------------
	inline void resetGuideHairVertices( unsigned int aGuideHairVerticesCount, double aSegmentSize );

	///----------------------------------------------------------------------------------------------------
	///  Resets the segments in normal direction without changes to guide hair vertices count
	///
	/// \param	aSegmentSize	Size of a segment. 
	///----------------------------------------------------------------------------------------------------
	inline void resetSegments( double aSegmentSize );

	///----------------------------------------------------------------------------------------------------
	///  Randomize segments - function for testing only. 
	///
	/// \param	aRandomization					a randomization vector. 
	/// \param	aSegmentsCount					Number of the segments. 
	/// \param	aSegmentSize					Size of a segment. 
	/// \param [in,out]	aRandomGenerator		a random number generator. 
	///----------------------------------------------------------------------------------------------------
	inline void randomizeSegments( const Vector3D< double > aRandomization, int aSegmentsCount, double aSegmentSize, 
		RandomGenerator & aRandomGenerator );

	///----------------------------------------------------------------------------------------------------
	/// Gets the guide hair vertices.
	///
	/// \return	The guide hair vertices.
	///----------------------------------------------------------------------------------------------------
	inline const GuideHairVertices getGuideHairVertices() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the guide hair vertices count.
	///
	/// \return	The guide hair vertices.
	///----------------------------------------------------------------------------------------------------
	inline unsigned int getGuideHairVerticesCount() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the desired segment hair
	///
	/// \return The segment hair
	///----------------------------------------------------------------------------------------------------
	inline double getSegmentLength() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the world transform matrix. Can be used as model-view OpenGL matrix (glMultMatrix( * ) ).
	///
	/// \return	The world transform matrix. 
	///----------------------------------------------------------------------------------------------------
	inline const double * getWorldTransformMatrix() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the local transform matrix. Can be used as model-view OpenGL matrix (glMultMatrix( * ) ).
	///
	/// \return	The local transform matrix. 
	///----------------------------------------------------------------------------------------------------
	inline const double * getLocalTransformMatrix() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets a guide hair vertex in world coordinates. 
	///
	/// \param	aIndex	Zero-based index of vertex
	///
	/// \return	The vertex in world coordinates. 
	///----------------------------------------------------------------------------------------------------
	inline Vector3D< double > getGuideHairVertexInWorldCoordinates( unsigned int aIndex ) const;
	
	///----------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///----------------------------------------------------------------------------------------------------
	inline ~GuideHair();

private:
	UVPoint mRelativePosition; ///< The relative position of guide hair

	MeshPoint mWorldSpacePosition; ///< The world space position of guide hair with tangent, normal, binormal and u,v

	GuideHairVertices mGuideHairVertices; ///< The guide hair vertices ( including the root of the hair )

	unsigned int mGuideHairVerticesCount; ///< Number of guide hair vertices

	double mSegmentLength; ///< Lenght of the hair segment (same between all hair vertices

	double mWorldTransformMatrix[ 16 ]; ///< The local to world transformation matrix

	double mLocalTransformMatrix[ 16 ]; ///< The world to local transformation matrix
};

// inline functions implementation
inline GuideHair::GuideHair():
	mGuideHairVertices( 0 ),
	mGuideHairVerticesCount( 0 ),
	mSegmentLength( 0.0 )
{
}

inline GuideHair::GuideHair( const GuideHair & aCopy ):
	mRelativePosition( aCopy.mRelativePosition ),
	mWorldSpacePosition( aCopy.mWorldSpacePosition ),
	mGuideHairVertices( 0 ),
	mGuideHairVerticesCount( aCopy.mGuideHairVerticesCount ),
	mSegmentLength( aCopy.mSegmentLength )
{
	// Copy the world transformation matrix
	memcpy( reinterpret_cast< void * >( mWorldTransformMatrix ), reinterpret_cast< const void * >( aCopy.mWorldTransformMatrix ),
		sizeof( double ) * 16 );

	// Copy the local transformation matrix
	memcpy( reinterpret_cast< void * >( mLocalTransformMatrix ), reinterpret_cast< const void * >( aCopy.mLocalTransformMatrix ),
		sizeof( double ) * 16 );

	// Allocate memory for guide hair vertices
	mGuideHairVertices = new Vector3D< double >[ mGuideHairVerticesCount ];

	// Copy vertices
	memcpy( reinterpret_cast< void *>( mGuideHairVertices ), reinterpret_cast< const void *>( aCopy.mGuideHairVertices ),
		sizeof( Vector3D< double > ) * mGuideHairVerticesCount );
}

inline const GuideHair & GuideHair::operator==( const GuideHair & aCopy )
{
	// First allocate memory for vertices
	if ( aCopy.mGuideHairVerticesCount != mGuideHairVerticesCount )
	{
		GuideHairVertices temp = aCopy.mGuideHairVerticesCount > 0 ? new Vector3D< double >[ aCopy.mGuideHairVerticesCount ] : 0;
		mGuideHairVerticesCount = aCopy.mGuideHairVerticesCount;
		mSegmentLength = aCopy.mSegmentLength;
		delete [] mGuideHairVertices;
		mGuideHairVertices = temp;
	}

	mRelativePosition = aCopy.mRelativePosition;
	mWorldSpacePosition = aCopy.mWorldSpacePosition;

	// Copy the world transformation matrix
	memcpy( reinterpret_cast< void * >( mWorldTransformMatrix ), reinterpret_cast< const void * >( aCopy.mWorldTransformMatrix ),
		sizeof( double ) * 16 );

	// Copy the local transformation matrix
	memcpy( reinterpret_cast< void * >( mLocalTransformMatrix ), reinterpret_cast< const void * >( aCopy.mLocalTransformMatrix ),
		sizeof( double ) * 16 );

	// Allocate memory for vertices
	mGuideHairVertices = new Vector3D< double >[ mGuideHairVerticesCount ];

	// Copy vertices
	memcpy( reinterpret_cast< void *>( mGuideHairVertices ), reinterpret_cast< const void *>( aCopy.mGuideHairVertices ),
		sizeof( Vector3D< double > ) * mGuideHairVerticesCount );

	return *this;
}

inline const UVPoint &  GuideHair::getRelativePosition() const
{
	return mRelativePosition;
}

inline void GuideHair::setRelativePosition( const UVPoint & aRelativePosition )
{
	mRelativePosition = aRelativePosition;
}

inline const MeshPoint & GuideHair::getGuideWorldPosition() const
{
	return mWorldSpacePosition;
}

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

inline void GuideHair::resetSegments( double aSegmentSize )
{
	mSegmentLength = aSegmentSize;
	double z = aSegmentSize;
	for( GuideHairVertices it = mGuideHairVertices + 1, end = mGuideHairVertices + mGuideHairVerticesCount; it != end; ++it, 
		z += aSegmentSize )
	{
		it->set( 0, 0, z );
	}
}

inline const GuideHair::GuideHairVertices GuideHair::getGuideHairVertices() const
{
	return mGuideHairVertices;
}

inline unsigned int GuideHair::getGuideHairVerticesCount() const
{
	return mGuideHairVerticesCount;
}

inline double GuideHair::getSegmentLength() const
{
	return mSegmentLength;
}

inline const double * GuideHair::getWorldTransformMatrix() const
{
	return mWorldTransformMatrix;
}

inline const double * GuideHair::getLocalTransformMatrix() const
{
	return mLocalTransformMatrix;
}

inline Vector3D< double > GuideHair::getGuideHairVertexInWorldCoordinates( unsigned int aIndex ) const
{
	//TODO: rewrite using a common vector transform method
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

inline GuideHair::~GuideHair()
{
	delete mGuideHairVertices;
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_GUIDE_HAIR_HPP