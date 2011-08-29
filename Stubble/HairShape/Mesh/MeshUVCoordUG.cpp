#include "MeshUVCoordUG.hpp"

namespace Stubble
{

namespace HairShape
{
	
MeshUVCoordUG::MeshUVCoordUG():
	mDirtyBit( true )
{
	/* TODO */
}

MeshUVCoordUG::~MeshUVCoordUG()
{
	/* TODO */
}

void MeshUVCoordUG::build( Triangles & aTriangles )
{
	/* TODO */
	mDirtyBit = false;
}

UVPoint MeshUVCoordUG::getUVPoint( const MeshPoint &aPoint ) const
{
	/* TODO */
	return UVPoint();
}

} // namespace HairShape

} // namespace Stubble