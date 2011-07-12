#include "UVPointGenerator.hpp"

#include "..\..\Primitives\Vector3D.hpp"

namespace Stubble
{

namespace HairShape
{

UVPointGenerator::UVPointGenerator( TriangleConstIterator & aTriangleConstIterator, RandomGenerator & aRandomNumberGenerator ):
	mVertices( 0 ),
	mIsInited( false ),
	mRandomNumberGenerator( aRandomNumberGenerator )
{
	SubTriangle * stack = new SubTriangle[ STACK_SIZE ];
	try
	{
		BuildVertices();
	}
	catch( ... ) // Not enough memory for vertices
	{
		delete [] stack; // Clear stack
	}

	// Cumulative distribution fuction
	double cdf = 0;
	// Stack for subtriangles
	SubTriangle * stackHead = stack;
	SubTriangle * father[ MAX_DIVISION_DEPTH ] = { 0 };
	// One third
	const double oneThird = 1.0 / 3.0;
	// For each triangle
	for( std::size_t index = 0; !aTriangleConstIterator.end(); ++aTriangleConstIterator, ++index )
	{
		// Remember triangle ID
		unsigned int triangleSimpleID = aTriangleConstIterator.getTriangleID();

		// Calculate triangle area
		const Triangle & triangle = aTriangleConstIterator.getTriangle();
		const MeshPoint & p1 = triangle.getPoint1();
		const MeshPoint & p2 = triangle.getPoint2();
		const MeshPoint & p3 = triangle.getPoint3();
		double area = (Vector3D< double >::crossProduct( p2.getPosition() - p1.getPosition(), 
			p3.getPosition() - p1.getPosition() ).size()) / 2;

		// Calculate size of texture on triangle
		double sizeU12 = abs( p1.getUCoordinate() - p2.getUCoordinate() );
		double sizeV12 = abs( p1.getVCoordinate() - p2.getVCoordinate() );
		double sizeU13 = abs( p1.getUCoordinate() - p3.getUCoordinate() );
		double sizeV13 = abs( p1.getVCoordinate() - p3.getVCoordinate() );
		double sizeU23 = abs( p2.getUCoordinate() - p3.getUCoordinate() );
		double sizeV23 = abs( p2.getVCoordinate() - p3.getVCoordinate() );
		double sizeU = sizeU12 > sizeU13 ? ( sizeU12 > sizeU23 ? sizeU12 : sizeU23) : ( sizeU13 > sizeU23 ? sizeU13 : sizeU23);
		double sizeV = sizeV12 > sizeV13 ? ( sizeV12 > sizeV23 ? sizeV12 : sizeV23) : ( sizeV13 > sizeV23 ? sizeV13 : sizeV23);

		// TODO - place here aTextureProxy.getWidth() & aTextureProxy.getHeight()
		double size = sizeU > sizeV ? sizeU * 1.00000000000000000000000000000000000000000000001 : sizeV * 1.00000000000000000000000000000000000000000000001;

		// Power 2 size
		unsigned int isize = static_cast< unsigned int >( ceil( size ) ) - 1;
		isize = ( isize >> 1 ) | isize;
		isize = ( isize >> 2 ) | isize;
		isize = ( isize >> 4 ) | isize;
		isize = ( isize >> 8 ) | isize;
		isize = ( isize >> 16 ) | isize;
		++isize;

		// Calculate 2^(max division depth)
		int twoPwrMaxDepth = isize > MAX_TRIANGLE_UV_SIZE ? MAX_TRIANGLE_UV_SIZE : isize ;
		// Put triangle on stack
		stack->set( FIRST_VERTEX_INDEX , SECOND_VERTEX_INDEX, THIRD_VERTEX_INDEX, area, 0 );
		++stackHead;
		// While stack is not empty
		while ( stackHead != stack )
		{
			--stackHead;
			// Select sub triangle on stack
			SubTriangle & subTriangle = *stackHead;
			if ( father[ subTriangle.mTriangleSimpleID ] != stackHead ) // Not returning from recursion
			{
				// Select area
				double area = subTriangle.mCDFValue;
				// Until division criterium is reached
				if ( ( 1 << subTriangle.mTriangleSimpleID ) < twoPwrMaxDepth )
				{
					// Subdivide triangle
					area /= 4;
					subTriangle.mCDFValue = -1;
					int depth = subTriangle.mTriangleSimpleID + 1;
					father[ subTriangle.mTriangleSimpleID ] = stackHead;
					// Connect middle vertices of lines
					unsigned int differentRowFix = 1 << ( ( MAX_DIVISION_DEPTH - depth ) << 1 );
					unsigned int v1ID = subTriangle.mVertex1ID;
					unsigned int v2ID = subTriangle.mVertex2ID;
					unsigned int v3ID = subTriangle.mVertex3ID;
					unsigned __int16 middle12ID = 
						static_cast< unsigned __int16 >( ( v1ID + v2ID ) >> 1 );
					unsigned __int16 middle23ID = 
						static_cast< unsigned __int16 >( ( v2ID + v3ID + differentRowFix ) >> 1 );
					unsigned __int16 middle13ID = 
						static_cast< unsigned __int16 >( ( v3ID + v1ID + differentRowFix ) >> 1 );
					// Create sub triangles
					( ++stackHead )->set( subTriangle.mVertex1ID, middle12ID, middle13ID, area, depth );
					( ++stackHead )->set( middle23ID, middle13ID, middle12ID, area, depth );
					( ++stackHead )->set( middle12ID, subTriangle.mVertex2ID, middle23ID, area, depth );
					( ++stackHead )->set( middle13ID, middle23ID, subTriangle.mVertex3ID, area, depth );
					++stackHead;
				}
				else
				{
					// Select vertices of subtriangle
					const Vertex & v1 = mVertices[ subTriangle.mVertex1ID ];
					const Vertex & v2 = mVertices[ subTriangle.mVertex2ID ];
					const Vertex & v3 = mVertices[ subTriangle.mVertex3ID ];
					// Select barycentric coordinates of the sub triangle middle
					double barU = oneThird * ( v1.mU + v2.mU + v3.mU );
					double barV = oneThird * ( v1.mV + v2.mV + v3.mV );
					double barW = 1 - barU - barV;
					// Select u,v coordinates in the sub triangle middle
					double u = p1.getUCoordinate() * barU + p2.getUCoordinate() * barV + 
						p3.getUCoordinate() * barW;
					double v = p1.getVCoordinate() * barU + p2.getVCoordinate() * barV + 
						p3.getVCoordinate() * barW;
					// New cdf value (texture average * area of sub triangle)
					// TODO - place averageAt( u, v )
					double p = 1.00000000000000000000000000000000000000000000001 * area;

					// Do I have father ?
					if ( subTriangle.mTriangleSimpleID != 0 ) 
					{
						// Select father
						SubTriangle * myFather = father[ subTriangle.mTriangleSimpleID - 1 ];
						// Send my probability to father, if I am first son or all previous sons have
						// same probability as I do
						myFather->mCDFValue = myFather->mCDFValue == p || myFather->mCDFValue == - 1 ? p : -2;
					}
					// Any probability to hit this triangle
					if ( p != 0 )  
					{
						cdf += p;
						subTriangle.mCDFValue = cdf;
						subTriangle.mTriangleSimpleID = triangleSimpleID;
						mSubTriangles.push_back( subTriangle );
					}
				}
			}
			else
			{
				father[ subTriangle.mTriangleSimpleID ] = 0;
				// I have same sons
				if ( subTriangle.mCDFValue != -2)
				{
					// Do I have father ?
					if ( subTriangle.mTriangleSimpleID != 0 ) 
					{
						// Select father
						SubTriangle * myFather = father[ subTriangle.mTriangleSimpleID - 1 ];
						// Send my probability to father, if I am first son or all previous sons have
						// same probability as I do
						myFather->mCDFValue = myFather->mCDFValue == subTriangle.mCDFValue 
							|| myFather->mCDFValue == - 1 ? subTriangle.mCDFValue : -2;
					}
					if ( subTriangle.mCDFValue != 0 )
					{
						// Remove sons
						mSubTriangles.pop_back();
						mSubTriangles.pop_back();
						mSubTriangles.pop_back();
						mSubTriangles.pop_back();
						// Insert father
						subTriangle.mCDFValue = cdf;
						subTriangle.mTriangleSimpleID = triangleSimpleID;
						mSubTriangles.push_back( subTriangle );
					}
					
				}
			}
		}
	}

	// Free memory of triangles iterator and stack for sub triangles
	mIsInited = mSubTriangles.size() != 0;
}

void UVPointGenerator::BuildVertices()
{
	mVertices = new Vertex[ VERTICES_COUNT ];
	// Now fill mVertices with barycentic coordinates
	unsigned int index = 0, verticesInRowMinus1 = SECOND_VERTEX_INDEX;
	double u , v = 0, stepU = 1.0 / verticesInRowMinus1, stepV = stepU;
	while ( index < VERTICES_COUNT )
	{
		u = 0; // New row of vertices
		for( unsigned int rowIndex = 0; rowIndex <= verticesInRowMinus1; ++rowIndex, ++index )
		{
			mVertices[ index ].mU = u;
			mVertices[ index ].mV = v;
			u += stepU;
		}

		// Next row will have less vertices
		--verticesInRowMinus1;
		v += stepV;
		stepU = ( 1.0 - v ) / verticesInRowMinus1;
	}
}

} // namespace HairShape

} // namespace Stubble