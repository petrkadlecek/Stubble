#include "CachedFrame.hpp"

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
	mFileName( generateFrameFileName( aNodeName, aSampleTime ) )
{
	aHairShape.sampleTime( aSampleTime, mFileName, mBoundingBoxes );
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
		RtString args[] = { getStubbleDLLFileName().c_str(), s.str().c_str() };
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
	/* TODO return path to stubble dll file*/
	return "HairVoxel.dll";
}

std::string CachedFrame::generateFrameFileName( std::string aNodeName, Time aSampleTime )
{
	/* TODO generate frame file name */
	std::ostringstream s;
	s << aNodeName << aSampleTime << ".bin";
	return s.str();
}

} // namespace RibExport

} // namespace Stubble