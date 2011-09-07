#include "CachedFrame.hpp"

#include "Common/CommonFunctions.hpp"

#include <maya/MFileObject.h>

#include <ri.h>

#include <sstream>
#include <Windows.h>


namespace Stubble
{

namespace RibExport
{

std::string CachedFrame::mStubbleWorkDir = "";

// Free argument data
void freeData(RtPointer data)
{
	/* NOTHING TO FREE */
}

CachedFrame::CachedFrame( HairShape::HairShape & aHairShape, std::string aNodeName, Time aSampleTime )
{
	loadStubbleWorkDir();
	Sample s; 
	s.mFileName = generateFrameFileName( aNodeName, aSampleTime );
	s.mSampleTime = aSampleTime;
	mMinTime = aSampleTime;
	// Takes first sample
	aHairShape.sampleTime( aSampleTime, mStubbleWorkDir + s.mFileName, mBoundingBoxes );
	// Store sample
	samples.push_back( s );
}

void CachedFrame::AddTimeSample( HairShape::HairShape & aHairShape, std::string aNodeName, Time aSampleTime )
{
	Sample s; 
	s.mFileName = generateFrameFileName( aNodeName, aSampleTime );
	s.mSampleTime = aSampleTime;
	mMaxTime = aSampleTime;
	BoundingBoxes tmpBoxes;
	// Takes sample
	aHairShape.sampleTime( aSampleTime, mStubbleWorkDir + s.mFileName, tmpBoxes );
	// Store sample
	samples.push_back( s );
	// Update bounding boxes
	BoundingBoxes::const_iterator cIt = tmpBoxes.begin();
	for ( BoundingBoxes::iterator it = mBoundingBoxes.begin(); it != mBoundingBoxes.end(); ++it, ++cIt )
	{
		it->expand( cIt->min() );
		it->expand( cIt->max() );
	}
}

void CachedFrame::emit()
{
	// Generate samples part of the arguments
	Time middle = ( mMaxTime + mMinTime ) * 0.5;
	std::string artPart;
	if ( samples.size() == 1 ) // No motion blur
	{
		artPart = " 1 0 " + samples.begin()->mFileName; // 1 sample, 0 time, filename
	}
	else // Motion blur
	{
		std::ostringstream s;
		s << " " << samples.size(); // n samples
		for ( Samples::const_iterator it = samples.begin(); it != samples.end(); ++it )
		{
			s << " " << ( it->mSampleTime - middle ) << " " << it->mFileName; // Time diff, filename
		}
		artPart = s.str();
	}
	// For each voxel
	for( BoundingBoxes::const_iterator it = mBoundingBoxes.begin(); it != mBoundingBoxes.end(); ++it )
	{
		// Prepare arguments
		std::ostringstream s;
		s << it - mBoundingBoxes.begin() << artPart; // Voxel id, samples
		std::string arg1 = getStubbleDLLFileName(), arg2 = s.str();
		RtString args[] = { arg1.c_str(), arg2.c_str() };
		// Convert bounding box
		RtBound bound = { static_cast< RtFloat >( it->min().x ), 
			static_cast< RtFloat >( it->max().x ), 
			static_cast< RtFloat >( it->min().y ), 
			static_cast< RtFloat >( it->max().y ), 
			static_cast< RtFloat >( it->min().z ), 
			static_cast< RtFloat >( it->max().z ) };
		// Write rib command
		RiProcedural( reinterpret_cast< RtPointer >( args ), bound, RiProcDynamicLoad, freeData );
	}
}

std::string CachedFrame::getStubbleDLLFileName()
{
	return "StubbleHairGen.dll";
}

std::string CachedFrame::generateFrameFileName( std::string aNodeName, Time aSampleTime )
{
	std::ostringstream s;
	// Replaces | -> -
	for ( unsigned int i = 0; i < aNodeName.size(); ++i )
	{
		if ( aNodeName[ i ] == '|' )
		{
			aNodeName[ i ] = '-';
		}
	}
	s << aNodeName << "-" << aSampleTime;
	return s.str();
}

void CachedFrame::loadStubbleWorkDir()
{
	if ( mStubbleWorkDir.empty() )
	{
		mStubbleWorkDir = getEnvironmentVariable("STUBBLE_WORKDIR") + "\\";
	}
}

} // namespace RibExport

} // namespace Stubble