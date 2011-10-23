#include <cassert>
#include "Common\StubbleException.hpp"
#include "DisplayedGuides.hpp"

#include <gl/GL.h>

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

DisplayedGuides::DisplayedGuides():
	mDirtyBit( true ),
	mHighLightSelected(true),
	mHideNonSelected(false),
	mFrameSegments( 0 ),
	mSelectedGuides( 0 )
{
}

DisplayedGuides::~DisplayedGuides()
{
}

void DisplayedGuides::build( const GuidesCurrentPositions & aGuidesCurrentPositions, 
	const FrameSegments & aFrameSegments, const SelectedGuides & aSelectedGuides )
{
	mGuidesCurrentPositions = &aGuidesCurrentPositions;
	mFrameSegments = &aFrameSegments;
	mSelectedGuides = &aSelectedGuides;
	mGuidesSelection = GuidesSelection(aGuidesCurrentPositions.size(), false);
	// For every selected guide
	for ( SelectedGuides::const_iterator guideIt = mSelectedGuides->begin(); 
		guideIt != mSelectedGuides->end(); ++guideIt )
	{
		mGuidesSelection[ (*guideIt)->mGuideId ] = true;
	}
	mDirtyBit = false;
}

void DisplayedGuides::selectionRebuild( const SelectedGuides & aSelectedGuides, bool aHighlight )
{
	mSelectedGuides = &aSelectedGuides;
	for ( GuidesSelection::iterator it = mGuidesSelection.begin(); it != mGuidesSelection.end(); ++it)
	{
		*it = false;
	}
	// For every selected guide
	for ( SelectedGuides::const_iterator guideIt = mSelectedGuides->begin(); 
		guideIt != mSelectedGuides->end(); ++guideIt )
	{
		mGuidesSelection[ (*guideIt)->mGuideId ] = true;
	}
	mDirtyBit = false;
}

void DisplayedGuides::draw() const
{
	if ( mDirtyBit )
	{
		throw StubbleException(" DisplayedGuides::draw : can not draw dirty object, used build method !" );
	}
	// Camera must be set to world coordinates at this point
	
	//TODO: add decision based on selection filter
	drawPolyline();
	drawVertices();
}

void DisplayedGuides::drawPolyline() const
{
	glColor3f(0.18f, 0.5f, 0.3f);

	// For all non-selected guides
	if (!mHideNonSelected)
	{
		GuidesCurrentPositions::const_iterator posIt = mGuidesCurrentPositions->begin();
		GuidesSelection::const_iterator selectedIt = mGuidesSelection.begin();
		// For each guide
		for ( GuidesSegments::const_iterator guideIt = mFrameSegments->mSegments.begin(); 
			guideIt != mFrameSegments->mSegments.end(); ++guideIt, ++posIt, ++selectedIt )
		{
			if (*selectedIt)
			{
				continue;
			}

			glBegin(GL_LINE_STRIP);
			// For every segment
			for ( Segments::const_iterator segIt = guideIt->mSegments.begin(); 
				segIt != guideIt->mSegments.end(); ++segIt )
			{
				// Transform vertex to world
				Vector3D< Real > pos = posIt->mPosition.toWorld( *segIt );
				// Draw
				glVertex3d( pos.x, pos.y, pos.z );
			}
			glEnd();
		} // For each guide
	} // if (!mHideNonSelected)

	glColor3f(0.5f, 1.0f, 0.8f);

	// For each selected guide
	for ( SelectedGuides::const_iterator guideIt = mSelectedGuides->begin(); 
		guideIt != mSelectedGuides->end(); ++guideIt )
	{
		glBegin(GL_LINE_STRIP);
		// For each segment
		for ( Segments::const_iterator segIt = (*guideIt)->mGuideSegments.mSegments.begin(); 
			segIt != (*guideIt)->mGuideSegments.mSegments.end(); ++segIt )
		{
			// Transform vertex to world
			Vector3D< Real > pos = ( *mGuidesCurrentPositions )[ (*guideIt)->mGuideId ].mPosition.toWorld( *segIt );
			// Draw
			glVertex3d( pos.x, pos.y, pos.z );
		}
		glEnd();

		//-----------------------
		//TODO: debug - remove me
		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		for (size_t i = 0; i < (*guideIt)->mGuideSegments.mSegments.size(); ++i)
		{
			if (!(*guideIt)->mSegmentsAdditionalInfo[ i ].mIsColliding)
			{
				continue;
			}
			Vector3D< Real > v = Vector3D< Real >::transformPoint((*guideIt)->mGuideSegments.mSegments[ i ], (*guideIt)->mPosition.mWorldTransformMatrix);
			Vector3D< Real > p = Vector3D< Real >::transformPoint((*guideIt)->mSegmentsAdditionalInfo[ i ].mClosestPointOnMesh, (*guideIt)->mPosition.mWorldTransformMatrix);
			glVertex3d(v.x, v.y, v.z);
			glVertex3d(p.x, p.y, p.z);
		}
		glColor3f(0.5f, 1.0f, 0.8f);
		glEnd();
		// End of debug code
		//-----------------------

		//-----------------------
		//TODO: debug - remove me
		/*Vector3D< Real > origin = Vector3D< Real >::transformPoint((*guideIt)->mGuideSegments.mSegments[ 0 ], (*guideIt)->mPosition.mWorldTransformMatrix);
		Vector3D< Real > normal = origin + (*guideIt)->mPosition.mPosition.getNormal();
		Vector3D< Real > binormal = origin + (*guideIt)->mPosition.mPosition.getBinormal();
		Vector3D< Real > tangent = origin + (*guideIt)->mPosition.mPosition.getTangent();

		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3d( origin.x, origin.y, origin.z );
		glVertex3d( normal.x, normal.y, normal.z );
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3d( origin.x, origin.y, origin.z );
		glVertex3d( binormal.x, binormal.y, binormal.z );
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3d( origin.x, origin.y, origin.z );
		glVertex3d( tangent.x, tangent.y, tangent.z );
		glEnd();*/
		// End of debug code
		//-----------------------
	} // For each selected guide
}

void DisplayedGuides::drawVertices() const
{
	glPointSize(3.0f);
	glColor3f(1.0f, 0.1f, 1.0f);

	// For all non-selected guides
	if (!mHideNonSelected)
	{
		GuidesCurrentPositions::const_iterator posIt = mGuidesCurrentPositions->begin();
		GuidesSelection::const_iterator selectedIt = mGuidesSelection.begin();
		// For each guide
		for ( GuidesSegments::const_iterator guideIt = mFrameSegments->mSegments.begin(); 
			guideIt != mFrameSegments->mSegments.end(); ++guideIt, ++posIt, ++selectedIt )
		{
			if (*selectedIt)
			{
				continue;
			}
		
			glBegin(GL_POINTS);
			// For every segment
			for ( Segments::const_iterator segIt = guideIt->mSegments.begin(); 
				segIt != guideIt->mSegments.end(); ++segIt )
			{
				// Transform vertex to world
				Vector3D< Real > pos = posIt->mPosition.toWorld( *segIt );
				// Draw
				glVertex3d( pos.x, pos.y, pos.z );
			}
			glEnd();
		} // For each guide
	} // if (!mHideNonSelected)

	// For each selected guide
	for ( SelectedGuides::const_iterator guideIt = mSelectedGuides->begin(); 
		guideIt != mSelectedGuides->end(); ++guideIt )
	{
		glBegin(GL_POINTS);
		// For each segment
		assert((*guideIt)->mGuideSegments.mSegments.size() == (*guideIt)->mSegmentsAdditionalInfo.size());
		SegmentsAdditionalInfo::const_iterator infoIt = (*guideIt)->mSegmentsAdditionalInfo.begin();
		for ( Segments::const_iterator segIt = (*guideIt)->mGuideSegments.mSegments.begin(); 
			segIt != (*guideIt)->mGuideSegments.mSegments.end(); ++segIt, ++infoIt )
		{
			// Transform vertex to world
			Vector3D< Real > pos = ( *mGuidesCurrentPositions )[ (*guideIt)->mGuideId ].mPosition.toWorld( *segIt );
			// Draw
			if (infoIt->mSelected)
			{
				glColor3f(1.0f, 1.0f, 0.1f);
			}
			else
			{
				glColor3f(1.0f, 0.1f, 1.0f);
			}
			glVertex3d( pos.x, pos.y, pos.z );
		} // For each segment
		glEnd();
	} // For each selected guide
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble