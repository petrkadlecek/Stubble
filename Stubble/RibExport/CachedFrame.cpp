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

CachedFrame::CachedFrame( HairShape::HairShape & aHairShape, Time aSampleTime ):
	mIsEmitted( false ),
	mFileName( /*TODO*/ "UNIQUE FILENAME ")
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
		RtString args[] = { "HairVoxel.dll", " " };
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


} // namespace RibExport

} // namespace Stubble