#include "Texture.hpp"

#include "math.h"

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
	unsigned int x = static_cast< unsigned int > ( floor(u * mWidth - 0.5) );
	unsigned int y = static_cast< unsigned int > ( floor(v * mHeight - 0.5) );

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

unsigned int Texture::getWidth() const
{
	return mWidth;
}

unsigned int Texture::getHeight() const
{
	return mHeight;
}

unsigned int Texture::getColorCompomentsCount() const
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

Texture::~Texture()
{
	delete[] mTexture;
}

} // namespace HairShape

} // namespace Stubble