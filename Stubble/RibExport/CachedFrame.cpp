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

// Free argument data
void freeData(RtPointer data)
{
	/* NOTHING TO FREE */
}

CachedFrame::CachedFrame( HairShape::HairShape & aHairShape, std::string aNodeName, Time aSampleTime ):
	mIsEmitted( false ),
	mFileName( generateFrameFileName( aNodeName, aSampleTime ) ),
	mFullPathFileName( getEnvironmentVariable("STUBBLE_WORKDIR") + "\\" + mFileName )
{
	aHairShape.sampleTime( aSampleTime, mFullPathFileName, mBoundingBoxes );
}

CachedFrame::~CachedFrame()
{
	if ( !mIsEmitted )
	{
		// Delete voxel file
		DeleteFile( mFileName.c_str() );
	}
}

void CachedFrame::emit()
{
	// For each voxel
	for( BoundingBoxes::const_iterator it = mBoundingBoxes.begin(); it != mBoundingBoxes.end(); ++it )
	{
		// Prepare arguments
		std::ostringstream s;
		s << it - mBoundingBoxes.begin() << " " << mFileName; // Voxel id, FileName
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
	mIsEmitted = true;
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

} // namespace RibExport

} // namespace Stubble