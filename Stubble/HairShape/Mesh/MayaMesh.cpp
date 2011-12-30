#include "MayaMesh.hpp"
#include "HairShape\UserInterface\HairShape.hpp"

#include <maya\MIntArray.h>
#include <maya\MFnMesh.h>
#include <maya\MItMeshPolygon.h>
#include <maya\MPointArray.h>
#include <maya\MFnDagNode.h>
#include <maya\MDagPathArray.h>

namespace Stubble
{

namespace HairShape
{

MayaMesh::MayaMesh(MObject & aMesh, const MString & aUVSet): 
	mUVSet( aUVSet )
{
	// creating acceleration structure
	mMayaMesh = new MFnMesh( aMesh );
	mMeshIntersector = new MMeshIntersector();

	MStatus status = mMeshIntersector->create( aMesh, HairShape::getActiveObject()->getCurrentInclusiveMatrix() );

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
				if ( !iter.getUV( localVerticesIndices[ trianglePointsIndices[ j ] ], uv, &aUVSet ) )
				{
					uv[ 0 ] = 0;
					uv[ 1 ] = 0;
				}

				// Select normal
				MVector normal;
				if ( !iter.getNormal( localVerticesIndices[ trianglePointsIndices[ j ] ], normal, MSpace::kWorld ) )
				{
					normal = ( trianglePoints[ 1 ] - trianglePoints[ 0 ] )^( trianglePoints[ 2 ] - trianglePoints[ 0 ] );
				}
				// Select tangent
				MVector tangent;
				MStatus tangentOk = 
					fnMesh.getFaceVertexTangent( polygonID, trianglePointsIndices[ j ], tangent, MSpace::kWorld, &aUVSet );
				if ( !tangentOk )
				{
					switch( j )
					{
					case 0:tangent = trianglePoints[ 1 ] - trianglePoints[ 0 ];break;
					case 1:tangent = trianglePoints[ 2 ] - trianglePoints[ 1 ];break;
					case 2:tangent = trianglePoints[ 2 ] - trianglePoints[ 0 ];break;
					}
				}
				triangleMeshPoints[j] = MeshPoint( Vector3D<Real> ( trianglePoints [ j ] ), // Position
					Vector3D< Real > ( normal ), // Normal
					Vector3D< Real > ( tangent ), // Tangent
					static_cast< Real >( uv[ 0 ] ), // U Coordinate
					static_cast< Real >( uv[ 1 ] )); // V Coordinate

				// Update bounding box
				mRestPose.mBoundingBox.expand( Vector3D< Real > ( trianglePoints[ j ] ) );
			}

			mRestPose.mTriangles.push_back( Triangle( triangleMeshPoints[0], triangleMeshPoints[1], triangleMeshPoints[2] ) );

			// Set maya triangle IDs for fast access to updated triangle
			mMayaTriangles.push_back( MayaTriangle( polygonID, localVerticesIndices[ trianglePointsIndices[ 0 ] ],
				localVerticesIndices[ trianglePointsIndices[ 1 ] ],
				localVerticesIndices[ trianglePointsIndices[ 2 ] ] ) );
		}

		++polygonID; // Next polygon ID
		iter.next();
	}
}

MeshPoint MayaMesh::getMeshPoint( const UVPoint &aPoint ) const
{

	// First select full barycentric coordinates
	double u = aPoint.getU();
	double v = aPoint.getV();
	double w = 1 - u - v;

	// Get triangle
	const MayaTriangle & triangle = mMayaTriangles [ aPoint.getTriangleID() ];

	// Get vertices indices
	int3 indices;
	MIntArray vertices;
	mMayaMesh->getPolygonVertices( triangle.getFaceID(), vertices );
	indices[ 0 ] = vertices[ triangle.getLocalVertex1() ];
	indices[ 1 ] = vertices[ triangle.getLocalVertex2() ];
	indices[ 2 ] = vertices[ triangle.getLocalVertex3() ];

	// Calculate position of point
	MPoint tmpPoint[ 3 ];
	MPoint point;
	mMayaMesh->getPoint( indices[ 0 ], tmpPoint[ 0 ], MSpace::kWorld );
	mMayaMesh->getPoint( indices[ 1 ], tmpPoint[ 1 ], MSpace::kWorld );
	mMayaMesh->getPoint( indices[ 2 ], tmpPoint[ 2 ], MSpace::kWorld );
	point = tmpPoint[ 0 ] * u + tmpPoint[ 1 ] * v;
	point += tmpPoint[ 2 ] * w;

	// Calculate normal
	MVector normal1, normal2, normal3;
	if ( !mMayaMesh->getFaceVertexNormal( triangle.getFaceID(), indices[ 0 ], normal1, MSpace::kWorld ) )
	{
		normal1 = ( tmpPoint[ 1 ] - tmpPoint[ 0 ] )^( tmpPoint[ 2 ] - tmpPoint[ 0 ] );
	}
	if ( !mMayaMesh->getFaceVertexNormal( triangle.getFaceID(), indices[ 1 ], normal2, MSpace::kWorld ) )
	{
		normal2 = ( tmpPoint[ 1 ] - tmpPoint[ 0 ] )^( tmpPoint[ 2 ] - tmpPoint[ 0 ] );
	}
	if ( !mMayaMesh->getFaceVertexNormal( triangle.getFaceID(), indices[ 2 ], normal3, MSpace::kWorld ) )
	{
		normal3 = ( tmpPoint[ 1 ] - tmpPoint[ 0 ] )^( tmpPoint[ 2 ] - tmpPoint[ 0 ] );
	}
	MVector normal = normal1 * u + normal2 * v + normal3 * w;
	normal.normalize();

	// Calculate tangent
	MVector tangent1, tangent2, tangent3;
	if ( !mMayaMesh->getFaceVertexTangent( triangle.getFaceID(), indices[ 0 ], tangent1, MSpace::kWorld, &mUVSet ) )
	{
		tangent1 = tmpPoint[ 1 ] - tmpPoint[ 0 ];
	}
	if ( !mMayaMesh->getFaceVertexTangent( triangle.getFaceID(), indices[ 1 ], tangent2, MSpace::kWorld, &mUVSet ) )
	{
		tangent2 = tmpPoint[ 2 ] - tmpPoint[ 1 ];
	}
	if ( !mMayaMesh->getFaceVertexTangent( triangle.getFaceID(), indices[ 2 ], tangent3, MSpace::kWorld, &mUVSet ) )
	{
		tangent3 = tmpPoint[ 2 ] - tmpPoint[ 0 ];
	}
	MVector tangent = tangent1 * u + tangent2 * v + tangent3 * w;

	// Orthonormalize tangent to normal
	tangent -= normal * ( tangent * normal ); 
	tangent.normalize();

	// Calculate texture coordinates
	double textU, textV;
	float tmpU, tmpV;
	if ( !mMayaMesh->getPolygonUV( triangle.getFaceID(), triangle.getLocalVertex1(), tmpU, tmpV, &mUVSet) )
	{
		tmpU = 0;
		tmpV = 0;
	}
	textU = tmpU * u;
	textV = tmpV * u;
	if ( !mMayaMesh->getPolygonUV( triangle.getFaceID(), triangle.getLocalVertex2(), tmpU, tmpV, &mUVSet))
	{
		tmpU = 0;
		tmpV = 0;
	}
	textU += tmpU * v;
	textV += tmpV * v;
	if ( !mMayaMesh->getPolygonUV( triangle.getFaceID(), triangle.getLocalVertex3(), tmpU, tmpV, &mUVSet))
	{
		tmpU = 0;
		tmpV = 0;
	}
	textU += tmpU * ( 1 - u - v );
	textV += tmpV * ( 1 - u - v );

	// Return PointOnMesh
	return MeshPoint( point, normal, tangent, textU, textV );
}

inline const Triangle MayaMesh::getTriangle( unsigned __int32 aID) const
{
	// Get triangle
	const MayaTriangle & triangle = mMayaTriangles [ aID ];

	// points store variables
	int3 indices;
	int3 local_indices;
	float3 textU, textV;
	MPoint points[ 3 ];
	MVector normals[ 3 ];
	MVector tangents[ 3 ];
	MeshPoint meshPoints[ 3 ];

	MIntArray vertices;
	mMayaMesh->getPolygonVertices( triangle.getFaceID(), vertices );
	indices[ 0 ] = vertices[ triangle.getLocalVertex1() ];
	indices[ 1 ] = vertices[ triangle.getLocalVertex2() ];
	indices[ 2 ] = vertices[ triangle.getLocalVertex3() ];
	local_indices[ 0 ] = triangle.getLocalVertex1();
	local_indices[ 1 ] = triangle.getLocalVertex2();
	local_indices[ 2 ] = triangle.getLocalVertex3();

	for( unsigned __int32 i = 0; i < 3; ++i)
	{
		mMayaMesh->getPoint( indices[ i ], points [ i ], MSpace::kWorld );
	}
	for( unsigned __int32 i = 0; i < 3; ++i)
	{
		if ( ! mMayaMesh->getFaceVertexNormal( triangle.getFaceID(), indices[ i ], normals[ i ], MSpace::kWorld ) )
		{
			normals[ i ] = ( points[ 1 ] - points[ 0 ] )^( points[ 2 ] - points[ 0 ] );
		}
		if ( !mMayaMesh->getFaceVertexTangent( triangle.getFaceID(), indices[ i ], tangents[ i ], MSpace::kWorld, &mUVSet ) )
		{
			switch( i )
			{
			case 0:tangents[ 0 ] = points[ 1 ] - points[ 0 ];break;
			case 1:tangents[ 1 ] = points[ 2 ] - points[ 1 ];break;
			case 2:tangents[ 2 ] = points[ 2 ] - points[ 0 ];break;
			}
		}
		if ( !mMayaMesh->getPolygonUV( triangle.getFaceID(), local_indices[ i ], textU [ i ], textV [ i ], &mUVSet) )
		{
			textU[ i ] = 0;
			textV[ i ] = 0;
		}
		meshPoints [ i ] = MeshPoint(Vector3D < Real > ( points[ i ] ), Vector3D < Real > ( normals[ i ] ),
			Vector3D < Real > ( tangents[ i ] ), static_cast< Real >(textU[ i ]), static_cast < Real >(textV[ i ] ));
	}

	return Triangle( meshPoints[0], meshPoints[1], meshPoints[2] );
}

inline unsigned __int32 MayaMesh::getTriangleCount() const
{
	return static_cast< unsigned __int32 >( mMayaTriangles.size() );
}

void MayaMesh::meshUpdate( MObject & aUpdatedMesh, const MString & aUVSet )
{
	// updating acceleration structure
	MStatus status = mMeshIntersector->create( aUpdatedMesh, HairShape::getActiveObject()->getCurrentInclusiveMatrix() );

	delete mMayaMesh;
	mMayaMesh = new MFnMesh( aUpdatedMesh );
	mUVSet = aUVSet;
}

void MayaMesh::serialize( std::ostream & aOutputStream ) const
{
	mRestPose.exportMesh( aOutputStream );		
}

void MayaMesh::deserialize( std::istream & aInputStream  )
{
	mRestPose.importMesh( aInputStream );	
}

MayaMesh::~MayaMesh()
{
	delete mMayaMesh;
	delete mMeshIntersector;
}

void MayaMesh::getTriangles( Triangles & aResult ) const
{
	aResult.clear();
	aResult.reserve( mMayaTriangles.size() );
	for ( unsigned __int32 i = 0; i < mMayaTriangles.size(); ++i )
	{
		aResult.push_back( getTriangle( i ) );
	}
}

} // namespace HairShape

} // namespace Stubble