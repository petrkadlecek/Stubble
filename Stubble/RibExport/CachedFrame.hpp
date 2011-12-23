#ifndef STUBBLE_CACHED_FRAME_HPP
#define STUBBLE_CACHED_FRAME_HPP

#include "Common/CommonTypes.hpp"
#include "HairShape/UserInterface/HairShape.hpp"
#include "Primitives/BoundingBox.hpp"

#include <string>

namespace Stubble
{

namespace RibExport
{

///-------------------------------------------------------------------------------------------------
/// Cached frame of one HairShape object. 
/// Caches one time sample per HairShape object or more if motion blur is used.
/// Executes all renderman commands needed for calling our hair generator plugin during renderman
/// renderin.
///-------------------------------------------------------------------------------------------------
class CachedFrame
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Constructor. 
	/// Cache one sample of selected hair shape node.
	///
	/// \param	aHairShape	The HairShape node. 
	/// \param	aNodeName	Name of the HairShape node.
	/// \param	aSampleTime	Time of the sample. 
	///-------------------------------------------------------------------------------------------------
	CachedFrame( HairShape::HairShape & aHairShape, std::string aNodeName, Time aSampleTime );

	///-------------------------------------------------------------------------------------------------
	/// Adds another time sample.
	/// Caches another sample of selected hair shape node.
	/// This method is used if motion blur is enabled.
	///
	/// \param	aHairShape	The HairShape node. 
	/// \param	aNodeName	Name of the HairShape node.
	/// \param	aSampleTime	Time of the sample. 
	///-------------------------------------------------------------------------------------------------
	void AddTimeSample( HairShape::HairShape & aHairShape, std::string aNodeName, Time aSampleTime );

	///-------------------------------------------------------------------------------------------------
	/// Emits all frames to renderman.
	/// Calls renderman function which will execute our hair generator plugin with proper attributes
	/// so cached frame is rendered.
	///-------------------------------------------------------------------------------------------------
	void emit();

private:

	///-------------------------------------------------------------------------------------------------
	/// Generates a time sample of selected HairShape.
	/// Sample is stored in several files on hard drive.
	///
	/// \param	aHairShape				The HairShape node. 
	/// \param	aNodeName				Name of the HairShape node.
	/// \param	aSampleTime				Time of the sample. 
	/// \param [in,out]	aBoundingBoxes	The bounding boxes of sample's voxels. 
	///-------------------------------------------------------------------------------------------------
	void generateSample( HairShape::HairShape & aHairShape, std::string aNodeName, Time aSampleTime,
		BoundingBoxes & amBoundingBoxes );

	///----------------------------------------------------------------------------------------------------
	/// Gets the stubble hair generator dll file name. 
	///
	/// \return	The stubble dll file name. 
	///----------------------------------------------------------------------------------------------------
	static std::string getStubbleDLLFileName();

	///-------------------------------------------------------------------------------------------------
	/// Gets the output directory. 
	/// Special directory will be created for every scene and every hair shape node and stored inside
	/// Stubble work directory defined by environment variable.
	///
	/// \return	The output directory. 
	///-------------------------------------------------------------------------------------------------
	static std::string getDirectory();

	///----------------------------------------------------------------------------------------------------
	/// Generates a prefix for all frame file names. 
	/// For each sample there is one main file and separate file for each voxel.
	///
	/// \param	aNodeName	Name of the HairShape node. 
	/// \param	aSampleTime	Time of the sample. 
	///
	/// \return	The frame file name. 
	///----------------------------------------------------------------------------------------------------
	static std::string generateFrameFileName( std::string aNodeName, Time aSampleTime );

	///-------------------------------------------------------------------------------------------------
	/// Loads the stubble work dir from environment variable.
	///-------------------------------------------------------------------------------------------------
	static void loadStubbleWorkDir();

	///-------------------------------------------------------------------------------------------------
	/// One sample of HairShape in time. 
	///-------------------------------------------------------------------------------------------------
	struct Sample
	{
		Time mSampleTime;   ///< Time of the sample

		std::string mFileName; ///< Prefix of filenames of the files with HairShape time sample
	};

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing the samples .
	///-------------------------------------------------------------------------------------------------
	typedef std::vector< Sample > Samples;

	Samples samples;	///< The samples of HairShape in different times
	
	BoundingBoxes mBoundingBoxes;   ///< The bounding boxes of voxels

	Time mMaxTime;  ///< Max. time of sample

	static std::string mStubbleWorkDir; ///< The stubble work dir
};

} // namespace RibExport

} // namespace Stubble

#endif // STUBBLE_CACHED_FRAME_HPP