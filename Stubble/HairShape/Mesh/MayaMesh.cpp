#include "MayaMesh.hpp"

#include <maya\MIntArray.h>
#include <maya\MFnMesh.h>
#include <maya\MItMeshPolygon.h>
#include <maya\MPointArray.h>

namespace Stubble
{

namespace HairShape
{

MayaMesh::MayaMesh(const MObject & aMesh, const MString & aUVSet): mUpdatedMesh(0)
{
	MItMeshPolygon iter( aMesh ); // Polygon iterator
	MFnMesh fnMesh( aMesh ); // Mesh functions
	unsigned __int32 * localVerticesIndices = new unsigned __int32[ fnMesh.numVertices() ]; // Global to local indices
	unsigned __int32 polygonID = 0;

	while ( !iter.isDone() )
	{
		// Get local vertices indices
		for( unsigned __int32 i = 0; i < iter.polygonVertexCount(); ++i )
		{
			localVerticesIndices[ iter.vertexIndex( i ) ] = i;
		}
		int count; // Number of triangles
		iter.numTriangles( count ); // Get new triangles count
		for ( int i = 0; i < count; ++i ) // For each triangle in polygon
		{
			MIntArray trianglePointsIndices; 
			MPointArray trianglePoints;
			// Select triangle
			iter.getTriangle( i, trianglePoints, trianglePointsIndices, MSpace::kWorld );

			// mTriangles.push_back( Triangle() ); // New triangle in our storage
			// Triangle & triangle = *mTriangles.rbegin(); // Our triangle storage

			MeshPoint triangleMeshPoints[3];

			for ( unsigned __int32 j = 0; j < 3 ; ++j ) // For each vertex in triangle
			{
				// Select texture coordinates
				float2 uv;
				iter.getUV( localVerticesIndices[ trianglePointsIndices[ j ] ], uv, &aUVSet );
				// Select normal
				MVector normal;
				iter.getNormal( localVerticesIndices[ trianglePointsIndices[ j ] ], normal, MSpace::kWorld );
				// Select tangent
				MVector tangent;
				fnMesh.getFaceVertexTangent( polygonID, trianglePointsIndices[ j ], tangent, MSpace::kWorld, &aUVSet );
				iter.getNormal( localVerticesIndices[ trianglePointsIndices[ j ] ], normal, MSpace::kWorld );

				triangleMeshPoints[j] = MeshPoint( Vector3D<Real> ( trianglePoints [ j ] ), // Position
					Vector3D< Real > ( normal ), // Normal
					Vector3D< Real > ( tangent ), // Tangent
					static_cast< double >( uv[ 0 ] ), // U Coordinate
					static_cast< double >( uv[ 1 ] )); // V Coordinate

				// Update bounding box
				mRestPose.mBoundingBox.expand( Vector3D< Real > ( trianglePoints[ j ] ) );
			}

			mRestPose.mTriangles.push_back( Triangle( triangleMeshPoints[0], triangleMeshPoints[1], triangleMeshPoints[2] ) );

			// Set maya triangle IDs for fast access to updated triangle
			mMeshTriangles.push_back( MeshTriangle( polygonID, localVerticesIndices[ trianglePointsIndices[ 0 ] ],
				localVerticesIndices[ trianglePointsIndices[ 1 ] ],
				localVerticesIndices[ trianglePointsIndices[ 2 ] ]));
		}

		++polygonID; // Next polygon ID
		iter.next();
	}
}

const Mesh MayaMesh::getRestPose() const
{
	return mRestPose;
}

MeshPoint MayaMesh::getMeshPoint( const UVPoint &aPoint ) const
{
	if ( mUpdatedMesh == 0 )
	{
		return mRestPose.getMeshPoint( aPoint );
	}

	// First select full barycentric coordinates
	double u = aPoint.getU();
	double v = aPoint.getV();
	double w = 1 - u - v;

	// Get triangle
	const MeshTriangle & triangle = mMeshTriangles [ aPoint.getTriangleID() ];

	// Get vertices indices
	int3 indices;
	MIntArray vertices;
	mUpdatedMesh->getPolygonVertices( triangle.getFaceID(), vertices );
	indices[ 0 ] = vertices[ triangle.getLocalVertex1() ];
	indices[ 1 ] = vertices[ triangle.getLocalVertex2() ];
	indices[ 2 ] = vertices[ triangle.getLocalVertex3() ];

	// Calculate position of point
	MPoint point, tmpPoint;
	mUpdatedMesh->getPoint( indices[ 0 ], point, MSpace::kWorld );
	mUpdatedMesh->getPoint( indices[ 1 ], tmpPoint, MSpace::kWorld );
	point = point * u + tmpPoint * v;
	mUpdatedMesh->getPoint( indices[ 2 ], tmpPoint, MSpace::kWorld );
	point += tmpPoint * w;

	// Calculate normal
	MVector normal1, normal2, normal3;
	mUpdatedMesh->getFaceVertexNormal( triangle.getFaceID(), indices[ 0 ], normal1, MSpace::kWorld );
	mUpdatedMesh->getFaceVertexNormal( triangle.getFaceID(), indices[ 1 ], normal2, MSpace::kWorld );
	mUpdatedMesh->getFaceVertexNormal( triangle.getFaceID(), indices[ 2 ], normal3, MSpace::kWorld );
	MVector normal = normal1 * u + normal2 * v + normal3 * w;
	normal.normalize();

	// Calculate tangent
	MVector tangent1, tangent2, tangent3;
	mUpdatedMesh->getFaceVertexTangent( triangle.getFaceID(), indices[ 0 ], tangent1, MSpace::kWorld, mUVSet );
	mUpdatedMesh->getFaceVertexTangent( triangle.getFaceID(), indices[ 1 ], tangent2, MSpace::kWorld, mUVSet );
	mUpdatedMesh->getFaceVertexTangent( triangle.getFaceID(), indices[ 2 ], tangent3, MSpace::kWorld, mUVSet );
	MVector tangent = tangent1 * u + tangent2 * v + tangent3 * w;

	// Orthonormalize tangent to normal
	tangent -= normal * ( tangent * normal ); 
	tangent.normalize();

	// Calculate texture coordinates
	double textU, textV;
	float tmpU, tmpV;
	mUpdatedMesh->getPolygonUV( triangle.getFaceID(), triangle.getLocalVertex1(), tmpU, tmpV, mUVSet);
	textU = tmpU * u;
	textV = tmpV * u;
	mUpdatedMesh->getPolygonUV( triangle.getFaceID(), triangle.getLocalVertex2(), tmpU, tmpV, mUVSet);
	textU += tmpU * v;
	textV += tmpV * v;
	mUpdatedMesh->getPolygonUV( triangle.getFaceID(), triangle.getLocalVertex3(), tmpU, tmpV, mUVSet);
	textU += tmpU * ( 1 - u - v );
	textV += tmpV * ( 1 - u - v );

	// Return PointOnMesh
	return MeshPoint( point, normal, tangent, textU, textV );
}

inline const Triangle MayaMesh::getTriangle( unsigned __int32 aID) const
{
	if(mUpdatedMesh == 0)
	{
		return mRestPose.getTriangle( aID );
	}
	else
	{
		// Get triangle
		const MeshTriangle & triangle = mMeshTriangles [ aID ];

		// points store variables
		int3 indices;
		float3 textU, textV;
		MPoint points[ 3 ];
		MVector normals[ 3 ];
		MVector tangents[ 3 ];
		MeshPoint meshPoints[ 3 ];

		MIntArray vertices;
		mUpdatedMesh->getPolygonVertices( triangle.getFaceID(), vertices );
		indices[ 0 ] = vertices[ triangle.getLocalVertex1() ];
		indices[ 1 ] = vertices[ triangle.getLocalVertex2() ];
		indices[ 2 ] = vertices[ triangle.getLocalVertex3() ];

		for( unsigned __int32 i = 0; i < 3; ++i)
		{
			mUpdatedMesh->getPoint( indices[ 0 ], points [ i ], MSpace::kWorld );
			mUpdatedMesh->getFaceVertexNormal( triangle.getFaceID(), indices[ 0 ], normals[ i ], MSpace::kWorld );
			mUpdatedMesh->getFaceVertexTangent( triangle.getFaceID(), indices[ 0 ], tangents[ i ], MSpace::kWorld, mUVSet );
			mUpdatedMesh->getPolygonUV( triangle.getFaceID(), triangle.getLocalVertex3(), textU [ i ], textV [ i ], mUVSet);

			meshPoints [ i ] = MeshPoint(Vector3D < Real > ( points[ i ] ), Vector3D < Real > ( normals[ i ] ),
				Vector3D < Real > ( points[ i ] ), static_cast< Real >(textU[ i ]), static_cast < Real >(textV[ i ] ));
		}

		return Triangle( meshPoints[0], meshPoints[1], meshPoints[2] );
	}
}

inline unsigned __int32 MayaMesh::getTriangleCount() const
{
	return static_cast< unsigned __int32 >( mMeshTriangles.size() );
}

void MayaMesh::meshUpdate(MObject & aUpdatedMesh, const MString * aUVSet)
{
	delete mUpdatedMesh;
	mUpdatedMesh = new MFnMesh( aUpdatedMesh );
	mUVSet = aUVSet;
	mMeshUG.setDirty();
}

MayaMesh::~MayaMesh()
{
	delete mUpdatedMesh;
}

void MayaMesh::getTriangles( Triangles & aResult ) const
{
	if ( mUpdatedMesh == 0 )
	{
		aResult = mRestPose.mTriangles; // Copies from rest pose
	}
	else
	{
		aResult.clear();
		aResult.reserve( mMeshTriangles.size() );
		for ( unsigned __int32 i = 0; i < aResult.size(); ++i )
		{
			aResult.push_back( getTriangle( i ) );
		}
	}
}

} // namespace HairShape

} // namespace Stubble