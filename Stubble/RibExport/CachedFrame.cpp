#include "CachedFrame.hpp"

#include "Common/CommonFunctions.hpp"

#include <maya/MFileObject.h>
#include <maya/MFileIO.h>

#include <ri.h>

#include <limits>
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
	// Takes sample
	generateSample( aHairShape, aNodeName, aSampleTime, mBoundingBoxes );
	mMaxTime = std::numeric_limits< Time >::min();
}

void CachedFrame::addTimeSample( HairShape::HairShape & aHairShape, std::string aNodeName, Time aSampleTime )
{
	BoundingBoxes tmpBoxes;
	// Takes sample
	generateSample( aHairShape, aNodeName, aSampleTime, tmpBoxes );
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
	Time middle = floor( mMaxTime );
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

void CachedFrame::generateSample( HairShape::HairShape & aHairShape, std::string aNodeName, Time aSampleTime,
	BoundingBoxes & aBoundingBoxes )
{
	// Get scene directory
	std::string dir = getDirectory();
	// Get full dir. path
	std::string fullDirName = mStubbleWorkDir + dir;
	// Create dir
	if ( GetFileAttributes( fullDirName.c_str() ) == INVALID_FILE_ATTRIBUTES )
	{
		CreateDirectory( fullDirName.c_str(), NULL );
	}
	// Prepare sample properties
	Sample s; 
	s.mFileName = dir + "\\" + generateFrameFileName( aNodeName, aSampleTime );
	s.mSampleTime = aSampleTime;
	// Takes first sample
	aHairShape.sampleTime( aSampleTime, mStubbleWorkDir + s.mFileName, aBoundingBoxes );
	// Store sample
	samples.push_back( s );
	// Update max time of samples
	if ( mMaxTime < s.mSampleTime )
	{
		mMaxTime = s.mSampleTime;
	}
}


std::string CachedFrame::getStubbleDLLFileName()
{
	return "StubbleHairGenerator.dll";
}

std::string CachedFrame::getDirectory()
{
	MFileObject fo;
	// Gets file scene full name
	fo.setRawFullName( MFileIO::currentFile() );
	// Gets file scene name
	MString currentFile = fo.name();
	MStringArray arr;
	// Remove extension
	currentFile.split( '.', arr );
	// Convert to std::string
	std::string result = std::string( arr[ 0 ].asChar() );
	for ( unsigned __int32 i = 1; i < ( arr.length() - 1 ); ++i )
	{
		result += std::string( arr[ i ].asChar() );
	}
	return result;
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