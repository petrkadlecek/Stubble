#include "TranslateBrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

	void TranslateBrushMode::doBrush ( const Vector3D< double > &aDX, HairShape::HairComponents::SelectedGuide &aGuideHair )
	{
		Vector3D< double > dX = Vector3D< double >::transform(aDX, aGuideHair.mPosition.mLocalTransformMatrix);
		const size_t SEGMENT_COUNT = aGuideHair.mGuideSegments.mSegments.size();

		// Loop through all guide segments except the first one (that's a follicle
		// and should not move)
		for (size_t i = 1; i < SEGMENT_COUNT; ++i)
		{
			if ( aGuideHair.mSegmentsAdditionalInfo[ i ].mInsideBrush )
			{
				aGuideHair.mGuideSegments.mSegments[ i ] += dX * aGuideHair.mSegmentsAdditionalInfo[ i ].mFallOff;
			}
		}
	}

} // namespace Toolbox

} // namespace Stubble