#include "UVPointGenerator.hpp"

#include "Primitives\Vector3D.hpp"
#include "Common\StubbleException.hpp"
#include "HairShape\Mesh\UVPoint.hpp"

namespace Stubble
{

namespace HairShape
{

UVPointGenerator::UVPointGenerator(const Texture &aTexture, TriangleConstIterator & aTriangleConstIterator, RandomGenerator & aRandomNumberGenerator ):
	mVertices( 0 ),
	mRandomNumberGenerator( aRandomNumberGenerator )
{
	SubTriangle * stack = 0; // Stack for sub triangles
	try
	{
		stack = new SubTriangle[ STACK_SIZE ];
		BuildVertices();
		mTrianglesPDF.resize( aTriangleConstIterator.getTrianglesCount(), 0 );
		// Cumulative distribution fuction
		Real cdf = 0;
		// Stack for subtriangles
		SubTriangle * stackHead = stack;
		SubTriangle * father[ MAX_DIVISION_DEPTH ] = { 0 };
		// One third
		const Real oneThird = 1.0 / 3.0;
		// For each triangle
		for( std::size_t index = 0; !aTriangleConstIterator.end(); ++aTriangleConstIterator, ++index )
		{
			// Remember triangle ID
			unsigned __int32 triangleSimpleID = aTriangleConstIterator.getTriangleID();

			// Calculate triangle area
			const Triangle & triangle = aTriangleConstIterator.getTriangle();
			const MeshPoint & p1 = triangle.getVertex1();
			const MeshPoint & p2 = triangle.getVertex2();
			const MeshPoint & p3 = triangle.getVertex3();
			Real area = ( Vector3D< Real >::crossProduct( p2.getPosition() - p1.getPosition(), 
				p3.getPosition() - p1.getPosition() ).size() ) / 2;

			// Calculate size of texture on triangle
			Real sizeU12 = abs( p1.getUCoordinate() - p2.getUCoordinate() );
			Real sizeV12 = abs( p1.getVCoordinate() - p2.getVCoordinate() );
			Real sizeU13 = abs( p1.getUCoordinate() - p3.getUCoordinate() );
			Real sizeV13 = abs( p1.getVCoordinate() - p3.getVCoordinate() );
			Real sizeU23 = abs( p2.getUCoordinate() - p3.getUCoordinate() );
			Real sizeV23 = abs( p2.getVCoordinate() - p3.getVCoordinate() );
			Real sizeU = sizeU12 > sizeU13 ? ( sizeU12 > sizeU23 ? sizeU12 : sizeU23) : ( sizeU13 > sizeU23 ? sizeU13 : sizeU23);
			Real sizeV = sizeV12 > sizeV13 ? ( sizeV12 > sizeV23 ? sizeV12 : sizeV23) : ( sizeV13 > sizeV23 ? sizeV13 : sizeV23);

			Real size = sizeU > sizeV ? sizeU * aTexture.getWidth() : sizeV * aTexture.getHeight();

			// Power 2 size
			unsigned __int32 isize = static_cast< unsigned __int32 >( ceil( size ) ) - 1;
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
					Real area = subTriangle.mCDFValue;
					// Until division criterium is reached
					if ( ( 1 << subTriangle.mTriangleSimpleID ) < twoPwrMaxDepth )
					{
						// Subdivide triangle
						area /= 4;
						subTriangle.mCDFValue = -1;
						int depth = subTriangle.mTriangleSimpleID + 1;
						father[ subTriangle.mTriangleSimpleID ] = stackHead;
						// Connect middle vertices of lines
						unsigned __int32 differentRowFix = 1 << ( ( MAX_DIVISION_DEPTH - depth ) << 1 );
						unsigned __int32 v1ID = subTriangle.mVertex1ID;
						unsigned __int32 v2ID = subTriangle.mVertex2ID;
						unsigned __int32 v3ID = subTriangle.mVertex3ID;
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
						Real barU = oneThird * ( v1.mU + v2.mU + v3.mU );
						Real barV = oneThird * ( v1.mV + v2.mV + v3.mV );
						Real barW = 1 - barU - barV;
						// Select u,v coordinates in the sub triangle middle
						Real u = p1.getUCoordinate() * barU + p2.getUCoordinate() * barV + 
							p3.getUCoordinate() * barW;
						Real v = p1.getVCoordinate() * barU + p2.getVCoordinate() * barV + 
							p3.getVCoordinate() * barW;
						// New pdf value (texture average * area of sub triangle)
						Real p = aTexture.realAtUV(u, v) * area;

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
							// Update triangle pdf
							mTrianglesPDF[ triangleSimpleID ] += p;
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
		delete stack;
		if ( mSubTriangles.size() == 0 )
		{
			throw StubbleException( "UVPointGenerator::UVPointGenerator : zero probability all over the mesh !" );
		}
		Real oneDividedByTotalDensity = 1 / mSubTriangles.rbegin()->mCDFValue;
		// Finally normalize pdf of triangles
		for ( RealArray::iterator it = mTrianglesPDF.begin(); it != mTrianglesPDF.end(); ++it )
		{
			*it *= oneDividedByTotalDensity;
		}
	}
	catch( ... ) // Not enough memory for vertices
	{
		delete [] stack; // Clear stack
		throw;
	}
	
}

UVPoint UVPointGenerator::next()
{
	// Generate random value for triangle selection
	Real xi = mRandomNumberGenerator.randomReal( 0, mSubTriangles.rbegin()->mCDFValue );
	// Search for first triangle that has greater cdf value than the generated one
	SubTriangles::const_iterator min =  mSubTriangles.begin(), max =  mSubTriangles.end(), mid; 
	do 
	{
		mid = min + ( ( max - min ) >> 1 ); // Select middle
		if ( xi >= mid->mCDFValue ) // Too small
		{
			min = mid + 1; 
		}	
		else {
			if ( mid == mSubTriangles.begin() || ( mid - 1 )->mCDFValue <= xi ) // OK !
			{
				// Select vertices of subtriangle
				const Vertex & v1 = mVertices[ mid->mVertex1ID ];
				const Vertex & v2 = mVertices[ mid->mVertex2ID ];
				const Vertex & v3 = mVertices[ mid->mVertex3ID ];
				// Sample barycentric coordinates in sub triangle
				Real xi1 = mRandomNumberGenerator.uniformNumber();
				Real xi2 = mRandomNumberGenerator.uniformNumber();
				Real sqrtXi1 = static_cast< Real >( sqrt( xi1 ) );
				Real u = 1 - sqrtXi1;
				Real v = xi2 * sqrtXi1;
				Real w = 1 - u - v;
				// Return sample point
				return UVPoint( // Recalculate barycentric coordinates of sub triangle to bar.coord. of triangle
					u * v1.mU + v * v2.mU + w * v3.mU, // U coordinate
					u * v1.mV + v * v2.mV + w * v3.mV, // V coordinate
					mid->mTriangleSimpleID ); // Copy triangleID
			}
			else // Too large
			{
				max = mid - 1;
			}
		}
	} 
	while ( max >= min );
	throw StubbleException( "RecursivePointGenerator::next : sampling has failed !" );
}

void UVPointGenerator::BuildVertices()
{
	mVertices.resize( VERTICES_COUNT );
	// Now fill mVertices with barycentic coordinates
	unsigned __int32 index = 0, verticesInRowMinus1 = SECOND_VERTEX_INDEX;
	Real u , v = 0, stepU = 1.0 / verticesInRowMinus1, stepV = stepU;
	while ( index < VERTICES_COUNT )
	{
		u = 0; // New row of vertices
		for( unsigned __int32 rowIndex = 0; rowIndex <= verticesInRowMinus1; ++rowIndex, ++index )
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