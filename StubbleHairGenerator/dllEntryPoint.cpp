/* This dll must be stored in DL_PROCEDURALS_PATH folder */
#define NOMINMAX  // windows.h: don't define min() and max() macros!
#include "Common/CommonTypes.hpp"
#include "Common/CommonFunctions.hpp"

// If report is defined, subdivide running time will be measured and outputed
#define REPORT

// If defined bounding box of generated hair points will be calculated during hair
// generation ( for debug purpose )
#define CALCULATE_BBOX

#include "HairShape/Interpolation/HairGenerator.tmpl.hpp"
#include "HairShape/Interpolation/RenderMan/RMHairProperties.hpp"
#include "HairShape/Interpolation/RenderMan/RMOutputGenerator.hpp"
#include "HairShape/Interpolation/RenderMan/RMPositionGenerator.hpp"
#include "Common/StubbleTimer.hpp"

#include "ri.h"

#include "HairShape/Interpolation/mentalray/mrOutputGenerator.hpp"
#include "shader.h"
#include "geoshader.h"

#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector> 

using namespace Stubble;
using namespace HairShape::Interpolation;

#if !defined ( DLLEXPORT )
	#if defined ( _WIN32 )
		#define DLLEXPORT __declspec( dllexport )
	#else
		#define DLLEXPORT
	#endif
#endif
	
#ifdef __cplusplus
extern "C" {
#endif
	
/* Declarations */
RtPointer DLLEXPORT ConvertParameters( RtString aParamString );
RtVoid DLLEXPORT Subdivide( RtPointer aData, float aDetailSize );
RtVoid DLLEXPORT Free( RtPointer aData );

/* Declarations for mental ray */

int DLLEXPORT stubble_geometry_version(void);
miBoolean DLLEXPORT stubble_geometry(miTag* result, miState* state, void* paras);
miBoolean DLLEXPORT stubble_geometry_callback( miTag tag, void *args );

#ifdef __cplusplus
}
#endif



///----------------------------------------------------------------------------------------------------
/// Defines an alias representing list of names of the files .
///----------------------------------------------------------------------------------------------------
typedef std::string * FileNames;

///----------------------------------------------------------------------------------------------------
/// Defines an alias representing the time samples .
///----------------------------------------------------------------------------------------------------
typedef RtFloat * TimeSamples;

///----------------------------------------------------------------------------------------------------
/// Parameters of this hair generator plugin in binary format
///----------------------------------------------------------------------------------------------------
struct BinaryParams
{
	TimeSamples mTimeSamples;   ///< The time samples
	
	FileNames mFileNames;   ///< List of names of the files

	unsigned __int32 mSamplesCount; ///< Number of samples

	unsigned __int32 mVoxelId;  ///< Identifier for the current voxel
};

///-------------------------------------------------------------------------------------------------
/// Convert parameters to binary representation. 
///
/// \param	aParamString	The parameter in string format. 
///
/// \return	parameters in binary format. 
///-------------------------------------------------------------------------------------------------
RtPointer DLLEXPORT ConvertParameters( RtString aParamString )
{
	// Convert params to string stream
	std::istringstream str( aParamString );
	// Prepare binary params structure
	BinaryParams * bp = new BinaryParams;
	// Read voxel id
	str >> bp->mVoxelId;
	// Read samples count
	str >> bp->mSamplesCount;
	// Allocate memory for time samples and file names
	bp->mTimeSamples = new RtFloat[ bp->mSamplesCount ];
	bp->mFileNames = new std::string[ bp->mSamplesCount ];
	// Convert time samples and file names from string to binary format
	TimeSamples timeIt = bp->mTimeSamples;
	FileNames fileIt = bp->mFileNames;
	// For every sample
	for ( unsigned __int32 i = 0; i < bp->mSamplesCount; ++i, ++timeIt, ++fileIt )
	{
		str >> *timeIt;
		str >> *fileIt;
	}
	// Return binary params
	return reinterpret_cast< RtPointer >( bp );
}

///-------------------------------------------------------------------------------------------------
/// Subdivides procedural command to other renderman commands.
/// This function loads exported data from Maya and generate all hair using RenderMan commands. 
///
/// \param	aData		Parameters in binary format. 
/// \param	aDetailSize	Size of a detail. 
///-------------------------------------------------------------------------------------------------
RtVoid DLLEXPORT Subdivide( RtPointer aData, RtFloat aDetailSize )
{
#ifdef REPORT
	Timer timer;
	timer.start();
#endif
	// Get params
	const BinaryParams & bp = * reinterpret_cast< BinaryParams * >( aData );
	// Load stubble workdir
	std::string stubbleWorkDir = Stubble::getEnvironmentVariable("STUBBLE_WORKDIR") + "\\";
	// Prepare rendering params
	RtInt test = RI_CATMULLROMSTEP;
	RiBasis( RiCatmullRomBasis, RI_CATMULLROMSTEP, RiCatmullRomBasis, RI_CATMULLROMSTEP );
	// Declare output variables
	RMOutputGenerator::declareVariables();
	if ( bp.mSamplesCount > 1 )
	{
		// Start motion blur
		RiMotionBeginV( static_cast< RtInt >( bp.mSamplesCount ), bp.mTimeSamples );
	}
	// Create output generator
	RMOutputGenerator outputGenerator;
	// For every sample
	for ( FileNames it = bp.mFileNames, end = bp.mFileNames + bp.mSamplesCount; it != end; ++it )
	{
		// Get file prefix
		std::string filePrefix = stubbleWorkDir + *it;
		// Read frame file with hair properties
		RMHairProperties hairProperties( filePrefix + ".FRM" );
		// Get voxel file name
		std::ostringstream str;
		str << filePrefix << ".VX" << bp.mVoxelId;
		// Read voxel file with mesh geometry and create position generator
		RMPositionGenerator positionGenerator( hairProperties.getDensityTexture(), str.str() );
		// Create hair generator
		HairGenerator< RMPositionGenerator, RMOutputGenerator > hairGenerator( positionGenerator, outputGenerator );
		// Should normals be outputed ?
		outputGenerator.setOutputNormals( hairProperties.areNormalsCalculated() );
		// Finally begin generating hair
		hairGenerator.generate( hairProperties );
#ifdef CALCULATE_BBOX
		if ( !positionGenerator.getVoxelBoundingBox().contains( hairGenerator.getBoundingBox() ) )
		{
			std::cerr << "StubbleHairGenerator.dll::Subdivide containment failed !!!";
		}
#endif
	}
	if ( bp.mSamplesCount > 1 )
	{
		// End motion blur
		RiMotionEnd();
	}
#ifdef REPORT
	timer.stop();
	std::cerr << "StubbleHairGenerator.dll::Subdivide run time: " << timer.getElapsedTime()
		<< std::endl;
#endif
}

///-------------------------------------------------------------------------------------------------
/// Frees memory allocated by ConvertParameters . 
///
/// \param	aData	Parameters in binary format. 
///-------------------------------------------------------------------------------------------------
RtVoid DLLEXPORT Free( RtPointer aData )
{
	// Get params
	BinaryParams * bp = reinterpret_cast< BinaryParams * >( aData );
	// Free memory
	delete [] bp->mTimeSamples;
	delete [] bp->mFileNames;
	delete bp;
}


///-------------------------------------------------------------------------------------------------
/// Returns the geometry shader version. Called by mental ray.
///-------------------------------------------------------------------------------------------------
int DLLEXPORT stubble_geometry_version(void) { return 1; }


///-------------------------------------------------------------------------------------------------
/// Hair geometry shader for mental ray. Sets up the bounding box to generate hair on demand.
///
/// \param	result	Tag of created geometry. 
/// \param	state	Current mental ray state. 
/// \param	paras	Shader parameters. 
///-------------------------------------------------------------------------------------------------
DLLEXPORT miBoolean stubble_geometry(miTag* result, miState* state, void* paras) {
   miObject *obj = mi_api_object_begin(mi_mem_strdup("stubble_hair"));  // TODO: hardcoded name is hardcoded!

   // Setup a placeholder callback and enable hair geometry for it
   mi_api_object_callback(stubble_geometry_callback, (void*)paras);
   obj->visible = miTRUE;
   obj->shadow = obj->reflection = obj->refraction = 0x03;
   obj->shadowmap = miTRUE;
   obj->finalgather = 0x03;

   // HACK: should somehow obtain the true bounding box
   obj->bbox_min.x = -1e6;
   obj->bbox_min.y = -1e6;
   obj->bbox_min.z = -1e6;
   obj->bbox_max.x =  1e6;
   obj->bbox_max.y =  1e6;
   obj->bbox_max.z =  1e6;

   miTag tag = mi_api_object_end();
   mi_geoshader_add_result(result, tag);
   obj = (miObject *) mi_scene_edit(tag);
   obj->geo.placeholder_list.type = miOBJECT_HAIR;
   mi_scene_edit_end(tag);

   return miTRUE;
}


///-------------------------------------------------------------------------------------------------
/// Hair geometry shader for mental ray.
///
/// \param	tag	The tag of created geometry. 
/// \param	args	Arguments passed by the callback setup.
///-------------------------------------------------------------------------------------------------
DLLEXPORT miBoolean stubble_geometry_callback(miTag tag, void *args)
{
	// Load stubble workdir
	std::string stubbleWorkDir = Stubble::getEnvironmentVariable("STUBBLE_WORKDIR") + "\\";

    // Start mentalRay object.
	char const* name = mi_api_tag_lookup(tag);
    mi_api_incremental(miTRUE);
    miObject* obj = mi_api_object_begin(mi_mem_strdup(name));
    obj->visible = miTRUE;
    obj->shadow = obj->reflection = obj->refraction = 0x03;

	// Create output generator
	MROutputGenerator outputGenerator( 1000000 ); // One million segments max. for each commit

	// Use only a single voxel for output.
	
	// Get file prefix
	std::string filePrefix = stubbleWorkDir + "stubble_mr_hair";
	// Read frame file with hair properties
	RMHairProperties hairProperties( filePrefix + ".FRM" );
	// Get voxel file name
	std::ostringstream str;
	str << filePrefix << ".VX0";
	// Read voxel file with mesh geometry and create position generator (it's OK to use RenderMan's)
	RMPositionGenerator positionGenerator( hairProperties.getDensityTexture(), str.str() );
	// Create hair generator
	HairGenerator< RMPositionGenerator, MROutputGenerator > hairGenerator( positionGenerator, outputGenerator );
	// Should normals be output?
	outputGenerator.setOutputNormals( hairProperties.areNormalsCalculated() );
	// Finally begin generating hair
	hairGenerator.generate( hairProperties );

	// Set bounding box
	BoundingBox bb = hairGenerator.getBoundingBox();
	obj->bbox_min.x = miScalar( bb.min()[ 0 ] );
	obj->bbox_min.y = miScalar( bb.min()[ 1 ] );
	obj->bbox_min.z = miScalar( bb.min()[ 2 ] );
	obj->bbox_max.x = miScalar( bb.max()[ 0 ] );
	obj->bbox_max.y = miScalar( bb.max()[ 1 ] );
	obj->bbox_max.z = miScalar( bb.max()[ 2 ] );

	// Close mental ray object.
	mi_api_object_end();

	std::cerr << "Stubble for mental ray: Hair object generated." << std::endl;
	return miTRUE;
}

