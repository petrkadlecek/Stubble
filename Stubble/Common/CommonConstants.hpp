#ifndef STUBBLE_COMMON_CONSTANTS_HPP
#define STUBBLE_COMMON_CONSTANTS_HPP

namespace Stubble 
{

static const char * FRAME_FILE_ID = "STUBBLE0001FRAMEFILE"; ///< Identifier for the frame file

static const char * VOXEL_FILE_ID = "STUBBLE0001VOXELFILE"; ///< Identifier for the voxel file

static const unsigned __int32 FRAME_FILE_ID_SIZE = sizeof( char ) * 20; ///< Size of the frame file identifier

static const unsigned __int32 VOXEL_FILE_ID_SIZE = sizeof( char ) * 20; ///< Size of the voxel file identifier

static const unsigned __int32 BUFFER_SIZE = 1 << 24;	///< Size of the buffer for gzip

static const unsigned __int32 COMPRESSION = 3;  ///< The compression quality of gzip ( 1 = FASTEST - 9 = BEST )

static const char * SEPARATOR = " "; ///< Object separator used in (de)serialization.

} // namespace Stubble

#endif // STUBBLE_COMMON_CONSTANTS_HPP