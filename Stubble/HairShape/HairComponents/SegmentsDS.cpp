#include <cassert>

#include "maya\MDagPath.h"
#include "maya\MFnSingleIndexedComponent.h"
#include "maya\MSelectionMask.h"
#include "maya\MQuaternion.h"

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
			if ((view.endSelect() > 0) && ( selectionCondition ))
			{
				if ( ( selMode == HairShapeUI::kSelectGuides ) || ( selMode == HairShapeUI::kSelectRoots ) )
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
				case HairShapeUI::kSelectRoots : if ( true )
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
	aResult.clear();

	MStatus status;
	M3dView view = M3dView::active3dView(&status);
	bool selected; // Is current guide selected?
	SelectedGuide *guide; // Current guide
	SelectedGuides::const_iterator gIt;

	// position of the tool shape
	MVector shapePos;
	aSelectionMask->getPosition( shapePos );
	MPoint shapePosPoint( shapePos );

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
}

// Fast Point-In-Cylinder Test | Greg James - gjames@NVIDIA.com
float SegmentsDS::cylinderPointTest( const MVector &pt1, const MVector &pt2, float lengthsq, float radius_sq, const MPoint &testpt )
{
	float dx, dy, dz;	 // vector d  from line segment point 1 to point 2
	float pdx, pdy, pdz; // vector pd from point 1 to test point
	float dot, dsq;

	dx = pt2.x - pt1.x;	 // translate so pt1 is origin.  Make vector from
	dy = pt2.y - pt1.y;  // pt1 to pt2.  Need for this is easily eliminated
	dz = pt2.z - pt1.z;

	pdx = testpt.x - pt1.x; 	// vector from pt1 to test point.
	pdy = testpt.y - pt1.y;
	pdz = testpt.z - pt1.z;

	// Dot the d and pd vectors to see if point lies behind the 
	// cylinder cap at pt1.x, pt1.y, pt1.z

	dot = pdx * dx + pdy * dy + pdz * dz;

	// If dot is less than zero the point is behind the pt1 cap.
	// If greater than the cylinder axis line segment length squared
	// then the point is outside the other end cap at pt2.

	if( dot < 0.0f || dot > lengthsq )
	{
		return( -1.0f );
	}
	else 
	{
		// Point lies within the parallel caps, so find
		// distance squared from point to line, using the fact that sin^2 + cos^2 = 1
		// the dot = cos() * |d||pd|, and cross*cross = sin^2 * |d|^2 * |pd|^2
		// careful: '*' means mult for scalars and dot product for vectors
		// In short, where dist is pt distance to cylinder axis: 
		// dist = sin( pd to d ) * |pd|
		// distsq = dsq = (1 - cos^2( pd to d)) * |pd|^2
		// dsq = ( 1 - (pd * d)^2 / (|pd|^2 * |d|^2) ) * |pd|^2
		// dsq = pd * pd - dot * dot / lengthsq
		//  where lengthsq is d*d or |d|^2 that is passed into this function 

		// distance squared to the cylinder axis:

		dsq = ( pdx*pdx + pdy*pdy + pdz*pdz ) - dot*dot/lengthsq;

		if( dsq > radius_sq )
		{
			return( -1.0f );
		}
		else
		{
			return( dsq );	// return distance squared to axis
		}
	}
}

void SegmentsDS::select( Stubble::Toolbox::CylinderToolShape *aSelectionMask, SelectedGuides &aResult ) const
{
	aResult.clear();

	MStatus status;
	M3dView view = M3dView::active3dView( &status );
	bool selected; // Is current guide selected?
	SelectedGuide *guide; // Current guide
	SelectedGuides::const_iterator gIt;

	// get rotation properties of the tool shape
	MVector cylinderRotAxis;
	double cylinderRotAngle;
	aSelectionMask->getRotationAxis( cylinderRotAxis );
	cylinderRotAngle = aSelectionMask->getRotationAngle();
	MQuaternion quatDir( aSelectionMask->getRotationAngle() * CHAI_PI / 180.0, cylinderRotAxis );

	// compute direction vector of cylinder
	MVector direction( 0, 0, 1 );
	direction = direction.rotateBy( quatDir );
	direction *= aSelectionMask->getRadius() * 5;

	// prepare structures for cylinder - point test
	MVector pt1;
	aSelectionMask->getPosition( pt1 );
	MVector pt2( pt1 + direction );
	
	float radiusSq = aSelectionMask->getRadius() * aSelectionMask->getRadius() * 0.5 * 0.5;
	float lengthSq = aSelectionMask->getRadius() * aSelectionMask->getRadius() * 5 * 5;

	// For each guide in the storage
	for (gIt = mStoredGuides.begin(); gIt != mStoredGuides.end(); ++gIt)
	{
		selected = false;
		assert((*gIt)->mGuideSegments.mSegments.size() == (*gIt)->mSegmentsAdditionalInfo.size());
		guide = *gIt;

		// For each endpoint on the guide
		for (size_t i = 0; i < guide->mGuideSegments.mSegments.size(); ++i)
		{
			MPoint p = Vector3D< Real >::transformPoint( guide->mGuideSegments.mSegments[ i ], guide->mPosition.mWorldTransformMatrix ).toMayaPoint();

			// guide to cylinder distance
			float dist = SegmentsDS::cylinderPointTest( pt1, pt2, lengthSq, radiusSq, p );

			if ( dist >= 0 )
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
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble
