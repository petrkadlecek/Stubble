#include "Texture.hpp"

#include "math.h"

namespace Stubble
{

namespace HairShape
{

Texture::Texture(float value)
{
	init(value, 0.f, 0.f, 0.f);
}

Texture::Texture(float value, float value1, float value2)
{
	init(value, value1, value2, 0.f);
}

Texture::Texture(float value, float value1, float value2, float value3)
{
	init(value, value1, value2, value3);
}

void Texture::init(float value, float value1, float value2, float value3)
{
	mWidth = 1;
	mHeight = 1;

	mTexture = new float[mWidth * mHeight * COLOR_COMPONENTS * sizeof(float)];

	mTexture[0] = value;
	mTexture[1] = value1;
	mTexture[2] = value2;
	mTexture[3] = value3;
}

float Texture::realAtUV( float u, float v ) const
{
	return colorAtUV(u, v)[0];
}

Texture::Color Texture::colorAtUV( float u, float v ) const
{
	unsigned int x = static_cast< unsigned int > ( floor(u * mWidth + 0.5) );
	unsigned int y = static_cast< unsigned int > ( floor(v * mHeight + 0.5) );

	return mTexture + y * mWidth * COLOR_COMPONENTS * sizeof(float) + y * COLOR_COMPONENTS * sizeof(float);
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

Texture::~Texture()
{
	delete[] mTexture;
}

} // namespace HairShape

} // namespace Stubble