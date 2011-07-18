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

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///  Randomize segments - function for testing only. 
	///
	/// \param	aRandomization					a randomization vector. 
	/// \param	aSegmentsCount					Number of the segments. 
	/// \param	aSegmentSize					Size of a segment. 
	/// \param [in,out]	aRandomGenerator		a random number generator. 
	////////////////////////////////////////////////////////////////////////////////////////////////////
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
	/// Gets the world transform matrix. Can be used as model-view OpenGL matrix (glMultMatrix( * ) ).
	///
	/// \return	The world transform matrix. 
	///----------------------------------------------------------------------------------------------------
	inline const double * getWorldTransformMatrix() const;

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

	double mWorldTransformMatrix[ 16 ]; ///< The world transformation matrix
};

// inline functions implementation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_GUIDE_HAIR_HPP