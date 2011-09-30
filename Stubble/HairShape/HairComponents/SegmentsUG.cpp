#include <cassert>
#include "SegmentsUG.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

SegmentsUG::SegmentsUG():
	mDirtyBit( true )
{
	/* TODO */
}

SegmentsUG::~SegmentsUG()
{
	/* TODO */
}

void SegmentsUG::build( const GuidesCurrentPositions & aGuidesCurrentPositions, 
	const FrameSegments & aFrameSegments )
{
	// ------------------------------------------
	//TODO: rewrite testing code with actual code
	assert(aGuidesCurrentPositions.size() == aFrameSegments.mSegments.size());
	const size_t GUIDE_COUNT = aGuidesCurrentPositions.size();

	for (size_t i = 0; i < GUIDE_COUNT; ++i)
	{
		SelectedGuide guide;
		guide.mDirtyFlag = false;
		guide.mDirtyRedrawFlag = false;
		guide.mGuideId = (unsigned int)i;
		guide.mPosition = aGuidesCurrentPositions[ i ];
		guide.mGuideSegments = aFrameSegments.mSegments[ i ];
		
		OneSegmentAdditionalInfo sgmtInfo;
		sgmtInfo.mActive = true;
		sgmtInfo.mFallOff = 1.0;
		guide.mSegmentsAdditionalInfo = SegmentsAdditionalInfo(guide.mGuideSegments.mSegments.size(), sgmtInfo);

		mStoredGuides.push_back(guide);
	}
	// End of testing code
	// ------------------------------------------

	mDirtyBit = false;
}

void SegmentsUG::build( const SelectedGuides & aSelectedGuides, bool aFullBuild )
{
	// ------------------------------------------
	//TODO: rewrite testing code with actual code
	mStoredGuides = SelectedGuides(aSelectedGuides);
	// End of testing code
	// ------------------------------------------
	mDirtyBit = false;
}

void SegmentsUG::select( short aX, short aY, short aW, short aH, SelectedGuides &aResult ) const
{
	// ------------------------------------------
	//TODO: rewrite testing code with actual code
	aResult.clear();

	MStatus status;
	M3dView view = M3dView::active3dView(&status);
	bool selected; // Is current guide selected?
	SelectedGuide guide; // Current guide
	SelectedGuides::const_iterator gIt;

	// For each guide in the storage
	for (gIt = mStoredGuides.begin(); gIt != mStoredGuides.end(); ++gIt)
	{
		selected = false;
		assert(gIt->mGuideSegments.mSegments.size() == gIt->mSegmentsAdditionalInfo.size());
		guide = (*gIt);

		// For each endpoint on the guide
		for (size_t i = 0; i < guide.mGuideSegments.mSegments.size(); ++i)
		{
			MPoint p = Vector3D< Real >::transformPoint(guide.mGuideSegments.mSegments[ i ], guide.mPosition.mWorldTransformMatrix).toMayaPoint();
			short px, py;
			view.worldToView(p, px, py, &status);

			if (px >= aX && px <= (aX + aW) && py >= aY && py <= (aY + aH))
			{
				selected = true;

				guide.mSegmentsAdditionalInfo[ i ].mActive = true;
				guide.mSegmentsAdditionalInfo[ i ].mFallOff = 0.0;  //TODO: add actual code
			}
		}

		if (selected)
		{
			aResult.push_back(guide);
		}
	}
	// End of testing code
	// ------------------------------------------
}

void SegmentsUG::select( Stubble::Toolbox::CircleToolShape *aSelectionMask, short aX, short aY, SelectedGuides &aResult ) const
{
	// ------------------------------------------
	//TODO: rewrite testing code with actual code
	aResult.clear();

	MStatus status;
	M3dView view = M3dView::active3dView(&status);
	bool selected; // Is current guide selected?
	SelectedGuide guide; // Current guide
	SelectedGuides::const_iterator gIt;

	// For each guide in the storage
	for (gIt = mStoredGuides.begin(); gIt != mStoredGuides.end(); ++gIt)
	{
		selected = false;
		assert(gIt->mGuideSegments.mSegments.size() == gIt->mSegmentsAdditionalInfo.size());
		guide = (*gIt);

		// For each endpoint on the guide
		for (size_t i = 0; i < guide.mGuideSegments.mSegments.size(); ++i)
		{
			MPoint p = Vector3D< Real >::transformPoint(guide.mGuideSegments.mSegments[ i ], guide.mPosition.mWorldTransformMatrix).toMayaPoint();
			short px, py;
			view.worldToView(p, px, py, &status);
			
			short dx = aX - px;
			short dy = aY - py;
			float distSq = dx * dx + dy * dy;
			float radiusSq = aSelectionMask->getRadius() * aSelectionMask->getRadius();

			if (distSq <= radiusSq)
			{
				selected = true;

				guide.mSegmentsAdditionalInfo[ i ].mActive = true;
				guide.mSegmentsAdditionalInfo[ i ].mFallOff = 0.0; //TODO: add actual code
			}
		}

		if (selected)
		{
			aResult.push_back(guide);
		}
	}
	// End of testing code
	// ------------------------------------------
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble