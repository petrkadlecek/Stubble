#include <cassert>

#include "maya\MDagPath.h"
#include "maya\MFnSingleIndexedComponent.h"
#include "maya\MSelectionMask.h"

#include "..\UserInterface\HairShapeUI.hpp"
#include "SegmentsDS.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

SegmentsDS::SegmentsDS():
	mDirtyBit( true )
{
	/* TODO */
}

SegmentsDS::~SegmentsDS()
{
	/* TODO */
}

bool SegmentsDS::build( const GuidesCurrentPositions & aGuidesCurrentPositions,
	const FrameSegments & aFrameSegments,
	const std::vector< unsigned __int32 > & aGuidesVerticesEndIndices,
	const std::vector< unsigned __int32 > & aGuidesInterpolationGroupIds,
	const std::vector< unsigned __int32 > & aInterpolationGroupsSelectable,
	MSelectInfo & aSelectInfo,
	MSelectionList & aSelectionList,
	MPointArray & aWorldSpaceSelectedPts,
	SelectedGuides & aSelectedGuides)
{
	assert(aGuidesCurrentPositions.size() == aFrameSegments.mSegments.size());
	assert(aGuidesVerticesEndIndices.size() > 0 );
	aSelectedGuides.clear();
	M3dView view = aSelectInfo.view(); // Selection view
	
	MPoint selectionPoint;
	MFnSingleIndexedComponent fnComponent;
	// selecting only vertices (segments)	
	MObject surfaceComponent = fnComponent.create( MFn::kMeshVertComponent );

	// is there at least one guide tip that was selected
	bool isAnythingSelected = false;

	// retrieve the selection mask from the Maya UI
	HairShapeUI::SelectionMode selMode = HairShapeUI::getSelectionMode();

	// for each guide
	GuideId gId = 0;
	GuidesCurrentPositions::const_iterator posIt = aGuidesCurrentPositions.begin();
	for (GuidesSegments::const_iterator guideIt = aFrameSegments.mSegments.begin();
		guideIt != aFrameSegments.mSegments.end(); ++guideIt, ++posIt, ++gId)
	{
		bool selected = false; // Denotes whether the current guide is selected or not

		// if the hair guide belongs to the currently non-selectable interpolation group, then skip it
		if ( !( aInterpolationGroupsSelectable[ aGuidesInterpolationGroupIds[ gId ] ] ))
		{
			continue;
		}

		SegmentsAdditionalInfo additionalInfo;
		
		// for each guide segment (vertex)
		SegmentId sId = 0;
		// the index of the last segment on the guide
		unsigned __int32 lastSegment = static_cast< unsigned __int32 >( guideIt->mSegments.size() ) - 1;
		for (Segments::const_iterator segIt = guideIt->mSegments.begin();
			segIt != guideIt->mSegments.end(); ++segIt, ++sId)
		{
			OneSegmentAdditionalInfo sgmtInfo;
			
			// propagate the selection condition (tips, roots, whole guides) from the Maya UI
			bool selectionCondition = false;
			switch ( selMode )
			{
			case HairShapeUI::kSelectRoots : if ( sId == 0)
											 {
												 selectionCondition = true;
											 }
											break;
			case HairShapeUI::kSelectTips : if ( sId == lastSegment)
											 {
												 selectionCondition = true;
											 }
											break;
			case HairShapeUI::kSelectAllVertices :
			case HairShapeUI::kSelectGuides :
												 selectionCondition = true;										 
											break;
			default :
				selectionCondition = false;
			}

			Vector3D< Real > pos = posIt->mPosition.toWorld( *segIt );
			view.beginSelect();
			glBegin(GL_POINTS);
			glVertex3d(pos.x, pos.y, pos.z);
			glEnd();
			// If a hit has been recorded
			if ((view.endSelect() > 0) && ( selectionCondition )) // for now we are just selecting the roots or the tips
			{
				if ( selMode == HairShapeUI::kSelectGuides )
				{
					selected = true;
					// put all segments into the selected set that will later be passed on to maya's selection list
					isAnythingSelected = true;
					int end = aGuidesVerticesEndIndices[ gId ]; 
					int start = end;
					if ( gId == 0 )
					{
						start = 0;
					}
					else
					{
						start = aGuidesVerticesEndIndices[ gId - 1 ] + 1;
					}

					for ( int k = start; k <= end; k++ )
					{
						fnComponent.addElement( k );
					}
					break;
				}
				else
				{				
					sgmtInfo.mSelected = true; //TODO: use selection filter
					selected = true;
					// put this segment into the selected set that will later be passed on to maya's selection list
					isAnythingSelected = true;
					int guideVerticesStartIndex = ( gId == 0 ) ? 0 : aGuidesVerticesEndIndices[ gId - 1 ] + 1;
					fnComponent.addElement( guideVerticesStartIndex + sId );
				}
			}
			additionalInfo.push_back(sgmtInfo);
		}

		if (selected)
		{
			SelectedGuide *guide = new SelectedGuide();
			guide->mDirtyFlag = false;
			guide->mDirtyRedrawFlag = false;
			guide->mNormal = posIt->mPosition.getNormal();
			guide->mGuideId = gId;
			guide->mGuideSegments.mSegmentLength = guideIt->mSegmentLength;
			guide->mGuideSegments.mSegments = Segments(guideIt->mSegments);
			guide->mPosition = *posIt;
			guide->mSegmentsAdditionalInfo = additionalInfo;
			guide->mCollisionsCount = 0;

			aSelectedGuides.push_back(guide);
		}
	} // for each guide

	// return the selected set to maya!
	if ( isAnythingSelected )
	{
		const MDagPath & path = aSelectInfo.multiPath();
		MSelectionList selectionItem;
		selectionItem.add( path, surfaceComponent );

		MSelectionMask mask( MSelectionMask::kSelectComponentsMask );
		aSelectInfo.addSelection(
			selectionItem, selectionPoint,
			aSelectionList, aWorldSpaceSelectedPts,
			mask, true );
	}

	//std::cout << "Number of selected guides: " << aSelectedGuides.size() << "\n" << std::flush;
	build( aSelectedGuides, true );

	return isAnythingSelected;
}

void SegmentsDS::build( const GuidesCurrentPositions & aGuidesCurrentPositions,
		const FrameSegments & aFrameSegments,
		const std::vector< unsigned __int32 > & aGuidesVerticesEndIndices,
		MIntArray &aSelectedComponentIndices,
		SelectedGuides & aSelectedGuides)
{
	assert(aGuidesCurrentPositions.size() == aFrameSegments.mSegments.size());
	aSelectedGuides.clear();

	// if the list is not empty, then update the selection,
	// else just skip and deselect everything
	if ( aSelectedComponentIndices.length() )
	{

		// retrieve the selection mask from the Maya UI
		HairShapeUI::SelectionMode selMode = HairShapeUI::getSelectionMode();

		int selectedComponentArrayIndex = 0;

		// for each guide
		GuideId gId = 0;
		GuidesCurrentPositions::const_iterator posIt = aGuidesCurrentPositions.begin();
		for (GuidesSegments::const_iterator guideIt = aFrameSegments.mSegments.begin();
			guideIt != aFrameSegments.mSegments.end(); ++guideIt, ++posIt, ++gId)
		{
			bool selected = false; // Denotes whether the current guide is selected or not
			SegmentsAdditionalInfo additionalInfo;
		
			// for each guide segment (vertex)
			SegmentId sId = 0;
			// the index of the last segment on the guide
			unsigned __int32 lastSegment = static_cast< unsigned __int32 >( guideIt->mSegments.size() ) - 1;
			for (Segments::const_iterator segIt = guideIt->mSegments.begin();
				segIt != guideIt->mSegments.end(); ++segIt, ++sId)
			{
				OneSegmentAdditionalInfo sgmtInfo;
			
				// propagate the selection condition (tips, roots, whole guides) from the Maya UI
				bool selectionCondition = false;
				switch ( selMode )
				{
				case HairShapeUI::kSelectRoots : if ( sId == 0)
												 {
													 selectionCondition = true;
												 }
												break;
				case HairShapeUI::kSelectTips : if ( sId == lastSegment)
												 {
													 selectionCondition = true;
												 }
												break;
				case HairShapeUI::kSelectAllVertices :
				case HairShapeUI::kSelectGuides :
												 selectionCondition = true;										 
											break;
				default :
					selectionCondition = false;
				}

				// If a hit has been recorded (calculate the current vertex position in the array of all guides' vertices)
				int guideVerticesStartIndex = ( gId == 0 ) ? 0 : aGuidesVerticesEndIndices[ gId - 1 ] + 1;
				if ( ( aSelectedComponentIndices[ selectedComponentArrayIndex ] - guideVerticesStartIndex - sId == 0 ) && ( selectionCondition ) )
				{
					sgmtInfo.mSelected = true; //TODO: use selection filter
					selected = true;
					// move on to the next selected component
					selectedComponentArrayIndex++;
				}
				additionalInfo.push_back(sgmtInfo);
			}

			if (selected)
			{
				SelectedGuide *guide = new SelectedGuide();
				guide->mDirtyFlag = false;
				guide->mDirtyRedrawFlag = false;
				guide->mNormal = posIt->mPosition.getNormal();
				guide->mGuideId = gId;
				guide->mGuideSegments.mSegmentLength = guideIt->mSegmentLength;
				guide->mGuideSegments.mSegments = Segments(guideIt->mSegments);
				guide->mPosition = *posIt;
				guide->mSegmentsAdditionalInfo = additionalInfo;
				guide->mCollisionsCount = 0;

				aSelectedGuides.push_back( guide );
			}

			// if we have checked all of the selected component indices,
			// then there is no need to go through all the guides
			if ( selectedComponentArrayIndex >= static_cast< int >( aSelectedComponentIndices.length() ) )
			{
				break;
			}
		} // for each guide
	}
	//std::cout << "Number of selected guides: " << aSelectedGuides.size() << "\n" << std::flush;
	build( aSelectedGuides, true );
}

void SegmentsDS::build( const GuidesCurrentPositions & aGuidesCurrentPositions, 
	const FrameSegments & aFrameSegments )
{
	// ------------------------------------------
	//TODO: rewrite testing code with actual code
	assert(aGuidesCurrentPositions.size() == aFrameSegments.mSegments.size());
	const size_t GUIDE_COUNT = aGuidesCurrentPositions.size();

	for (size_t i = 0; i < GUIDE_COUNT; ++i)
	{
		SelectedGuide *guide = new SelectedGuide();
		guide->mDirtyFlag = false;
		guide->mDirtyRedrawFlag = false;
		guide->mGuideId = (unsigned int)i;
		guide->mPosition = aGuidesCurrentPositions[ i ];
		guide->mGuideSegments = aFrameSegments.mSegments[ i ];
		
		OneSegmentAdditionalInfo sgmtInfo;
		guide->mSegmentsAdditionalInfo = SegmentsAdditionalInfo(guide->mGuideSegments.mSegments.size(), sgmtInfo);
		guide->mCollisionsCount = 0;

		mStoredGuides.push_back(guide);
	}
	// End of testing code
	// ------------------------------------------

	mDirtyBit = false;
}

void SegmentsDS::build( const SelectedGuides & aSelectedGuides, bool aFullBuild )
{
	// Should not copy contents once again - just use information obtained via aSelectedGuides parameter
	// see usage in the first method

	// ------------------------------------------
	//TODO: rewrite testing code with actual code
	mStoredGuides = SelectedGuides(aSelectedGuides);
	// End of testing code
	// ------------------------------------------
	mDirtyBit = false;
}

void SegmentsDS::select( Stubble::Toolbox::CircleToolShape *aSelectionMask, short aX, short aY, SelectedGuides &aResult ) const
{
	// ------------------------------------------
	//TODO: rewrite testing code with actual code

	aResult.clear();

	MStatus status;
	M3dView view = M3dView::active3dView(&status);
	bool selected; // Is current guide selected?
	SelectedGuide *guide; // Current guide
	SelectedGuides::const_iterator gIt;

	// For each guide in the storage
	for (gIt = mStoredGuides.begin(); gIt != mStoredGuides.end(); ++gIt)
	{
		selected = false;
		assert((*gIt)->mGuideSegments.mSegments.size() == (*gIt)->mSegmentsAdditionalInfo.size());
		guide = *gIt;

		// For each endpoint on the guide
		for (size_t i = 0; i < guide->mGuideSegments.mSegments.size(); ++i)
		{
			MPoint p = Vector3D< Real >::transformPoint(guide->mGuideSegments.mSegments[ i ], guide->mPosition.mWorldTransformMatrix).toMayaPoint();
			short px, py;
			view.worldToView(p, px, py, &status);
			
			short dx = aX - px;
			short dy = aY - py;
			Real distSq = dx * dx + dy * dy;
			Real radiusSq = aSelectionMask->getRadius() * aSelectionMask->getRadius();

			if (distSq <= radiusSq && guide->mSegmentsAdditionalInfo[ i ].mSelected)
			{
				selected = true;

				guide->mSegmentsAdditionalInfo[ i ].mInsideBrush = true;
				guide->mSegmentsAdditionalInfo[ i ].mFallOff = 1.0 - distSq / radiusSq;
			}
			else
			{
				guide->mSegmentsAdditionalInfo[ i ].mInsideBrush = false;
				guide->mSegmentsAdditionalInfo[ i ].mFallOff = 0.0;
			}
		}

		if (selected)
		{
			aResult.push_back(guide);
		}
	} // for each guide in the storage

	// End of testing code
	// ------------------------------------------
}

void SegmentsDS::select( Stubble::Toolbox::SphereToolShape *aSelectionMask, SelectedGuides &aResult ) const
{
	// ------------------------------------------
	//TODO: rewrite testing code with actual code
	aResult.clear();

	MStatus status;
	M3dView view = M3dView::active3dView(&status);
	bool selected; // Is current guide selected?
	SelectedGuide *guide; // Current guide
	SelectedGuides::const_iterator gIt;

	// For each guide in the storage
	for (gIt = mStoredGuides.begin(); gIt != mStoredGuides.end(); ++gIt)
	{
		selected = false;
		assert((*gIt)->mGuideSegments.mSegments.size() == (*gIt)->mSegmentsAdditionalInfo.size());
		guide = *gIt;

		// For each endpoint on the guide
		for (size_t i = 0; i < guide->mGuideSegments.mSegments.size(); ++i)
		{
			MPoint p = Vector3D< Real >::transformPoint(guide->mGuideSegments.mSegments[ i ], guide->mPosition.mWorldTransformMatrix).toMayaPoint();

			// TODO: optimize
			MVector shapePos;
			aSelectionMask->getPosition(shapePos);

			MPoint shapePosPoint(shapePos);

			// guide to sphere distance
			double dist = shapePosPoint.distanceTo( p );
			
			if ( dist < aSelectionMask->getRadius() )
			{
				selected = true;

				guide->mSegmentsAdditionalInfo[ i ].mInsideBrush = true;
				guide->mSegmentsAdditionalInfo[ i ].mFallOff = 1.0 - dist/aSelectionMask->getRadius();
			} 
			else
			{
				guide->mSegmentsAdditionalInfo[ i ].mInsideBrush = false;
				guide->mSegmentsAdditionalInfo[ i ].mFallOff = 0.0;
			}
		}

		if (selected)
		{
			aResult.push_back(guide);
		}
	} // for each guide in the storage
	// End of testing code
	// ------------------------------------------
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble