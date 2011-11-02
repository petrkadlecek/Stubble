#include "ScaleBrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

void ScaleBrushMode::doBrush ( const Vector3D< double > &aDX, HairShape::HairComponents::SelectedGuide &aGuideHair )
{
	HairShape::HairComponents::Segments &hairVertices = aGuideHair.mGuideSegments.mSegments; // Local alias
	const size_t SEGMENT_COUNT = hairVertices.size();
	Real length = aGuideHair.mGuideSegments.mSegmentLength + aDX.x;
	length = (length < 0.0) ? 0.0 : length;
	aGuideHair.mGuideSegments.mSegmentLength = length;

	// Loop through all guide segments except the first one
	Vector3D< double > previousVertex = hairVertices[ 0 ];
	Vector3D< double > v;
	for (size_t i = 1; i < SEGMENT_COUNT; ++i)
	{
		v = hairVertices[ i ] - previousVertex;
		v.normalize();
		previousVertex = hairVertices[ i ];
		hairVertices[ i ] = hairVertices[ i - 1 ] + v * length;
	}
}

} // namespace Toolbox

} // namespace Stubble