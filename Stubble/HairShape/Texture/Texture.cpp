#include "Texture.hpp"

#include "math.h"
#include "Common\StubbleException.hpp"

namespace Stubble
{

namespace HairShape
{

Texture::Texture(float value): mColorComponents(1)
{
	init();

	mTexture[0] = value;
}

Texture::Texture(float value, float value1, float value2): mColorComponents(3)
{
	init();

	mTexture[0] = value;
	mTexture[1] = value1;
	mTexture[2] = value2;
}

Texture::Texture(float value, float value1, float value2, float value3): mColorComponents(4)
{
	init();

	mTexture[0] = value;
	mTexture[1] = value1;
	mTexture[2] = value2;
	mTexture[3] = value3;
}

Texture::Texture( std::istream & aIsStream )
{
	aIsStream.read( reinterpret_cast< char * >( &mWidth ), sizeof( unsigned __int32 ) );
	aIsStream.read( reinterpret_cast< char * >( &mHeight ), sizeof( unsigned __int32 ) );
	aIsStream.read( reinterpret_cast< char * >( &mColorComponents ), sizeof( unsigned __int32 ) );
	mTexture = new float[ mWidth * mHeight * mColorComponents ];
	aIsStream.read( reinterpret_cast< char * >( mTexture ), 
		mWidth * mHeight * mColorComponents * sizeof( float ) );
	mDirty = false;
	mIsAnimated = false;
	mInverseWidth = 1.0f / mWidth;
	mInverseHeight = 1.0f / mHeight;
}

Texture::~Texture()
{
	delete[] mTexture;
}

void Texture::init()
{
	mIsAnimated = false;
	mWidth = 1;
	mHeight = 1;
	mDirty = false;
	mTexture = new float[mWidth * mHeight * mColorComponents];
	mInverseWidth = 1.0f / mWidth;
	mInverseHeight = 1.0f / mHeight;
}

void Texture::exportToFile( std::ostream &aOutStream ) const
{
	//std::string dir = Stubble::getEnvironmentVariable("STUBBLE_WORKDIR") + "\\";
	aOutStream.write( reinterpret_cast< const char * >( &mWidth ), sizeof( unsigned __int32 ) );
	aOutStream.write( reinterpret_cast< const char * >( &mHeight ), sizeof( unsigned __int32 ) );
	aOutStream.write( reinterpret_cast< const char * >( &mColorComponents ), sizeof( unsigned __int32 ) );
	aOutStream.write( reinterpret_cast< const char * >( mTexture ), 
		mWidth * mHeight * mColorComponents * sizeof( float ) );
	/* TODO : export must also save current time value or only data for current time */
}

void Texture::setDirty()
{
	mDirty = true;
}

bool Texture::isDirty() const
{
	return mDirty;
}

unsigned __int32 Texture::getWidth() const
{
	return mWidth;
}

unsigned __int32 Texture::getHeight() const
{
	return mHeight;
}

unsigned __int32 Texture::getColorCompomentsCount() const
{
	return mColorComponents;
}

float *Texture::getRawData() const
{
	return mTexture;
}

bool Texture::isAnimated() const
{
	return mIsAnimated;
}

void setCurrentTime( Time aTime )
{
	throw StubbleException(" Texture::setCurrentTime : NOT IMPLEMENTED ");
}

void Texture::setCurrentTime( Time aTime )
{
	if ( !mIsAnimated )
	{
		return;
	}
	/* TODO sets current time of texture for animated textures (should not fail for non-animated) */
}

#ifdef MAYA
void Texture::setConnection( const MPlug& aPlug )
{
	MStatus status;
	MPlugArray sourceTexturePlugs;
	aPlug.connectedTo(sourceTexturePlugs, true, false, &status);
	if ( status==MStatus::kSuccess ){
		if ( sourceTexturePlugs.length() > 0 ) // We have some connections
		{
			// We are destination, so we have only one connection
			textureDataSourcePlug = sourceTexturePlugs[0];
		}
	}
	mDirty = true;
}

void Texture::resample(unsigned __int32 aTextureSamples)
{
	MStatus status;
	MObject sourceNode = textureDataSourcePlug.node( &status );//TODO: test if this is not null
	if ( status == MStatus::kSuccess )
		if ( sourceNode.hasFn( MFn::kFileTexture ) )
		{
			MImage textureData;
			status = textureData.readFromTextureNode( sourceNode, MImage::kUnknown );
			if( status == MStatus::kSuccess )
			{
				reloadFileTextureImage(textureData);				
			}
		}
		else if ( sourceNode.hasFn( MFn::kTexture2d ) )
		{
			resample2DTexture(aTextureSamples);
		}
		else if ( sourceNode.hasFn( MFn::kTexture3d ) )
		{
			MDagPath cameraPath;
			M3dView::active3dView().getCamera( cameraPath );
			MFloatMatrix cameraMat( cameraPath.inclusiveMatrix().matrix );
			//MRenderUtil::sampleShadingNetwork(textureDataSourcePlug.name(),1,false,false,cameraMat,NULL,
		}
		mDirty = false;
}

void Texture::reloadFileTextureImage(MImage & aTextureImage)
{	
	aTextureImage.getSize( mWidth, mHeight );
	ComputeInverseSize();
	delete[] mTexture;
	mTexture = new float[ mWidth * mHeight * mColorComponents ];
	unsigned int depth = aTextureImage.depth();
	// Loading texture with float color channels
	if ( aTextureImage.pixelType() == MImage::kFloat )
	{
		#pragma omp parallel for
		for( int i = 0; i < mWidth * mHeight; ++i )
		{
			for( unsigned __int32 j = 0; j < mColorComponents; ++j )
			{	
				// Modulo solves problems with sourceTexture with less channels than we need
				mTexture[ i * mColorComponents + j ] =
					aTextureImage.floatPixels()[ i * depth + j % depth ];
			}
		}
	}
	// Loading texture with standard byte color channels
	if ( aTextureImage.pixelType() == MImage::kByte )
	{
		#pragma omp parallel for
		for ( int i = 0; i < mWidth * mHeight; ++i )
		{
			for ( unsigned __int32 j = 0; j < mColorComponents; ++j )
			{	
				// Modulo solves problems with sourceTexture with less channels than we need
				mTexture[ i * mColorComponents + j ] = aTextureImage.pixels()[ i * depth + j % depth ];
			}
		}
	}
}

void Texture::resample2DTexture(unsigned __int32 aTextureSamples)
{
	if ( mWidth != aTextureSamples || mHeight != aTextureSamples )
	{
		// Change texture and texture attributes only if the dimension of texture changes
		mWidth = aTextureSamples;
		mHeight = aTextureSamples;
		ComputeInverseSize();
		// Prepare texture array for saving new texture values
		delete[] mTexture;
		mTexture = new float[ mWidth * mHeight * mColorComponents ];
	}
	// Generate sample points
	float * uCoords = new float[ mWidth * mHeight ];
	float * vCoords = new float[ mWidth * mHeight ];
	GetSampleUVPoints( uCoords, vCoords, aTextureSamples );
	// Prepare arrays for saving results of sampling 
	MFloatMatrix cameraMat;
	MFloatVectorArray sampleColors;
	MFloatVectorArray sampleTransparencies;
	// Rows are sampled separately because of stack overflow problems
	for ( unsigned __int32 row = 0; row < mHeight; ++ row)
	{
		MFloatArray uCoordinates( uCoords + row * mWidth, mWidth );
		MFloatArray vCoordinates( vCoords + row * mWidth, mWidth );
		MRenderUtil::sampleShadingNetwork( textureDataSourcePlug.name(), mWidth , false, false,
			cameraMat, NULL, &uCoordinates, &vCoordinates, NULL, NULL, NULL, NULL, NULL,
			sampleColors, sampleTransparencies);
		for ( unsigned __int32 i = 0; i < mWidth; ++i )
		{
			for ( unsigned __int32 j = 0; j<mColorComponents; ++j )
			{
				mTexture[ mColorComponents * ( row * mWidth + i ) + j ] = sampleColors[ i ][ j ];			
			}
			if( mColorComponents == 4 )
			{
				mTexture[ mColorComponents * ( row * mWidth + i ) + 3 ] = sampleTransparencies[ i ][ 0 ];
			}
		}
	}
}

#endif

void Texture::GetSampleUVPoints(float* aUSamples, float* aVSamples, unsigned __int32 dimension)
{
	for( unsigned __int32 i = 0; i < dimension; ++i )
	{
		for( unsigned __int32 j = 0; j < dimension; ++j )
		{
			aUSamples[ i * dimension + j ] = ( float )i / ( dimension - 1 );
			aVSamples[ i * dimension + j ] = ( float )j / ( dimension - 1 );
		}
	}
}

} // namespace HairShape

} // namespace Stubble