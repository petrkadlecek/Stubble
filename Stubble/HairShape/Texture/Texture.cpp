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

void Texture::init()
{
	mIsAnimated = false;
	mWidth = 1;
	mHeight = 1;

	mTexture = new float[mWidth * mHeight * mColorComponents * sizeof(float)];
}

float Texture::realAtUV( Real u, Real v ) const
{
	return colorAtUV(u, v)[0];
}

Texture::Color Texture::colorAtUV( Real u, Real v ) const
{
	unsigned __int32 x = static_cast< unsigned __int32 > ( floor(u * mWidth) );
	x = x == mWidth ? mWidth - 1 : x;

	unsigned __int32 y = static_cast< unsigned __int32 > ( floor(v * mHeight) );
	y = y == mHeight ? mHeight - 1 :  y;

	return mTexture + y * mWidth * mColorComponents * sizeof(float) + y * mColorComponents * sizeof(float);
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

Texture::~Texture()
{
	delete[] mTexture;
}

} // namespace HairShape

} // namespace Stubble