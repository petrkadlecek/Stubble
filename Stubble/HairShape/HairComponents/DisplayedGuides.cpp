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
	mGuidesSelection.resize( mGuidesCurrentPositions->size(), false );
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
	mGuidesSelection.resize( mGuidesCurrentPositions->size(), false );
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
	// For every guide
	GuidesCurrentPositions::const_iterator posIt = mGuidesCurrentPositions->begin();
	GuidesSelection::const_iterator selectedIt = mGuidesSelection.begin();
	for ( GuidesSegments::const_iterator guideIt = mFrameSegments->mSegments.begin(); 
		guideIt != mFrameSegments->mSegments.end(); ++guideIt, ++posIt, ++selectedIt )
	{
		if ( !mHighLightSelected || !*selectedIt ) // Is not selected or hight light is off ?
		{
			glColor3f(0,1,0);
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
		}
	}
	if ( mHighLightSelected )
	{
		// For every selected guide
		for ( SelectedGuides::const_iterator guideIt = mSelectedGuides->begin(); 
			guideIt != mSelectedGuides->end(); ++guideIt )
		{
			glColor3f(0,1,0);
			glBegin(GL_LINE_STRIP);
			// For every segment
			for ( Segments::const_iterator segIt = guideIt->mGuideSegments.mSegments.begin(); 
				segIt != guideIt->mGuideSegments.mSegments.end(); ++segIt )
			{
				// Transform vertex to world
				Vector3D< Real > pos = ( *mGuidesCurrentPositions )[ guideIt->mGuideId ].mPosition.toWorld( *segIt );
				// Draw
				glVertex3d( pos.x, pos.y, pos.z );
			}
			glEnd();
		}
	}
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble