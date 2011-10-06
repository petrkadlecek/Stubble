#include "TranslateBrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

	void TranslateBrushMode::doBrush ( const Vector3D< double > &aDX, HairShape::HairComponents::SelectedGuide &aGuideHair )
	{
		std::cout << "TranslateBrushMode::doBrush()" << std::endl;

		Vector3D< double > dX = Vector3D< double >::transform(aDX, aGuideHair.mPosition.mLocalTransformMatrix);
		size_t segmentCount = aGuideHair.mGuideSegments.mSegments.size();

		// Loop through all guide segments except the first one (that's a follicle
		// and should not move)
		for (size_t i = 1; i < segmentCount; ++i)
		{
			if ( aGuideHair.mSegmentsAdditionalInfo[ i ].mInsideBrush )
			{
				aGuideHair.mGuideSegments.mSegments[ i ] += dX * aGuideHair.mSegmentsAdditionalInfo[ i ].mFallOff;
			}
		}
	}

} // namespace Toolbox

} // namespace Stubble