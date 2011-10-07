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
		mGuidesSelection[ guideIt->mGuideId ] = true;
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
		mGuidesSelection[ guideIt->mGuideId ] = true;
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
	// For every guide
	GuidesCurrentPositions::const_iterator posIt = mGuidesCurrentPositions->begin();
	GuidesSelection::const_iterator selectedIt = mGuidesSelection.begin();
	for ( GuidesSegments::const_iterator guideIt = mFrameSegments->mSegments.begin(); 
		guideIt != mFrameSegments->mSegments.end(); ++guideIt, ++posIt, ++selectedIt )
	{
		if (mHideNonSelected && !*selectedIt)
		{
			continue;
		}
		if (*selectedIt)
		{
			glColor3f(1,0,0);
		}
		else
		{
			glColor3f(0,1,0);
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
	} // for every guide
}

void DisplayedGuides::drawVertices() const
{
	glPointSize(2.0f);
	glColor3f(0,1,0);

	// For every guide
	GuidesCurrentPositions::const_iterator posIt = mGuidesCurrentPositions->begin();
	GuidesSelection::const_iterator selectedIt = mGuidesSelection.begin();
	for ( GuidesSegments::const_iterator guideIt = mFrameSegments->mSegments.begin(); 
		guideIt != mFrameSegments->mSegments.end(); ++guideIt, ++posIt, ++selectedIt )
	{
		if (mHideNonSelected && !*selectedIt || *selectedIt)
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
	} // For every guide

	// For every selected guide
	for ( SelectedGuides::const_iterator guideIt = mSelectedGuides->begin(); 
		guideIt != mSelectedGuides->end(); ++guideIt )
	{
		glBegin(GL_POINTS);
		// For every segment
		assert(guideIt->mGuideSegments.mSegments.size() == guideIt->mSegmentsAdditionalInfo.size());
		SegmentsAdditionalInfo::const_iterator infoIt = guideIt->mSegmentsAdditionalInfo.begin();
		for ( Segments::const_iterator segIt = guideIt->mGuideSegments.mSegments.begin(); 
			segIt != guideIt->mGuideSegments.mSegments.end(); ++segIt, ++infoIt )
		{
			// Transform vertex to world
			Vector3D< Real > pos = ( *mGuidesCurrentPositions )[ guideIt->mGuideId ].mPosition.toWorld( *segIt );
			// Draw
			if (infoIt->mSelected)
			{
				glColor3f(1,0,0);
			}
			else
			{
				glColor3f(0,1,0);
			}
			glVertex3d( pos.x, pos.y, pos.z );
		}
		glEnd();
	} // For every selected guide
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble