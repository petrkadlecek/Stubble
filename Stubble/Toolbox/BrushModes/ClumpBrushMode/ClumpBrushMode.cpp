#include "ClumpBrushMode.hpp"
#include "Toolbox/HairTask.hpp"

namespace Stubble
{

namespace Toolbox
{

void ClumpBrushMode::doBrush ( HairTask *aTask )
{
	Vector3D< Real > clumpPosition, clumpNormal;
	findClumpCenter(const_cast<HairShape::HairComponents::SelectedGuides &>(*(aTask->mAffectedGuides)), clumpPosition, clumpNormal);

	// Loop through all guides
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aTask->mAffectedGuides->begin(); it != aTask->mAffectedGuides->end(); ++it)
	{
		HairShape::HairComponents::SelectedGuide *guide = *it; // Guide alias
		HairShape::HairComponents::Segments &hairVertices = guide->mGuideSegments.mSegments; // Local alias
		const size_t SEGMENT_COUNT = hairVertices.size();
		Vector3D< Real > clumpPositionLocal = Vector3D< Real >::transformPoint(clumpPosition, guide->mPosition.mLocalTransformMatrix);
		Vector3D< Real > clumpNormalLocal = Vector3D< Real >::transform(clumpNormal, guide->mPosition.mLocalTransformMatrix);

		// Loop through all guide segments except the first one
		Real segmentLength = guide->mGuideSegments.mSegmentLength;
		Vector3D< Real > vertexAtClumpNormal;
		Vector3D< Real > distance;
		Vector3D< Real > d;
		for (size_t i = 1; i < SEGMENT_COUNT; ++i)
		{
			if ( !guide->mSegmentsAdditionalInfo[ i ].mInsideBrush )
			{
				continue;
			}
			vertexAtClumpNormal = clumpPositionLocal + clumpNormalLocal * (i * segmentLength);
			distance = vertexAtClumpNormal - hairVertices[ i ];
			d = (mEnableFalloff == true) ? distance * aTask->mDx.x * guide->mSegmentsAdditionalInfo[ i ].mFallOff : distance * aTask->mDx.x;
			hairVertices[ i ] += d;
		}

		guide->mDirtyFlag = true;
		guide->mDirtyRedrawFlag = true;
	}
}

void ClumpBrushMode::findClumpCenter( const HairShape::HairComponents::SelectedGuides &aGuides, Vector3D< Real > &aPosition, Vector3D< Real > &aNormal )
{
	Real nFactor = 1.0 / aGuides.size();
	Vector3D< Real > meanPosition, meanNormal;

	HairShape::HairComponents::SelectedGuides::const_iterator it;
	for (it = aGuides.begin(); it != aGuides.end(); ++it)
	{
		meanPosition += (*it)->mPosition.mPosition.getPosition() * nFactor;
		meanNormal += (*it)->mPosition.mPosition.getNormal();
	}

	aPosition = meanPosition;
	aNormal = meanNormal.normalize();
}

} // namespace Toolbox

} // namespace Stubble