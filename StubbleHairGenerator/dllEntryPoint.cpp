/* This dll must be stored in DL_PROCEDURALS_PATH folder */
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

#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector> 

using namespace Stubble;
using namespace HairShape::Interpolation;

#if defined ( _WIN32 )
#define DLLEXPORT __declspec( dllexport )
#else
#define DLLEXPORT
#endif
	
#ifdef __cplusplus
extern "C" {
#endif
	
/* Declarations */
RtPointer DLLEXPORT ConvertParameters( RtString aParamString );
RtVoid DLLEXPORT Subdivide( RtPointer aData, float aDetailSize );
RtVoid DLLEXPORT Free( RtPointer aData );
	
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

