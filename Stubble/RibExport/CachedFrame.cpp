#include "CachedFrame.hpp"

#include <ri.h>
#include <sstream>
#include <Windows.h>
#include <maya/MFileObject.h>

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
	return getEnvironmentVariable("STUBBLE_BIN") + "\\HairVoxel.dll";
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
	s << getEnvironmentVariable("STUBBLE_WORKDIR") << "\\" << aNodeName << "-" << aSampleTime << ".bin";
	return s.str();
}

std::string CachedFrame::getEnvironmentVariable( const char * aVariableName )
{
	char *pValue = 0;
	size_t len;
	errno_t err = _dupenv_s( &pValue, &len, aVariableName );
	if ( err || pValue == 0 )
	{
		free( pValue );
		throw StubbleException( " CachedFrame::getEnvironmentVariable : variable was not found " );
	}
	std::string res( pValue );
	free( pValue );
	return res;
}


} // namespace RibExport

} // namespace Stubble