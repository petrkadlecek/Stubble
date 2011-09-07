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
/// Cached frame of HairShape object. 
///-------------------------------------------------------------------------------------------------
class CachedFrame
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Constructor. 
	///
	/// \param	aHairShape	The HairShape node. 
	/// \param	aNodeName	Name of the HairShape node.
	/// \param	aSampleTime	Time of the sample. 
	///-------------------------------------------------------------------------------------------------
	CachedFrame( HairShape::HairShape & aHairShape, std::string aNodeName, Time aSampleTime );

	///-------------------------------------------------------------------------------------------------
	/// Adds another time sample
	///
	/// \param	aHairShape	The HairShape node. 
	/// \param	aNodeName	Name of the HairShape node.
	/// \param	aSampleTime	Time of the sample. 
	///-------------------------------------------------------------------------------------------------
	void AddTimeSample( HairShape::HairShape & aHairShape, std::string aNodeName, Time aSampleTime );

	///-------------------------------------------------------------------------------------------------
	/// Emits this object (renders stored frame).
	///-------------------------------------------------------------------------------------------------
	void emit();

private:

	///----------------------------------------------------------------------------------------------------
	/// Gets the stubble dll file name. 
	///
	/// \return	The stubble dll file name. 
	///----------------------------------------------------------------------------------------------------
	static std::string getStubbleDLLFileName();

	///----------------------------------------------------------------------------------------------------
	/// Generates a frame file name. 
	///
	/// \param	aNodeName	Name of the HairShape node. 
	/// \param	aSampleTime	Time of the sample. 
	///
	/// \return	The frame file name. 
	///----------------------------------------------------------------------------------------------------
	static std::string generateFrameFileName( std::string aNodeName, Time aSampleTime );

	///-------------------------------------------------------------------------------------------------
	/// Loads the stubble work dir. 
	///-------------------------------------------------------------------------------------------------
	static void loadStubbleWorkDir();

	BoundingBoxes mBoundingBoxes;   ///< The bounding boxes of voxels

	Time mMaxTime;  ///< Max. time of sample

	Time mMinTime;  ///< Min. time of sample

	static std::string mStubbleWorkDir; ///< The stubble work dir

	///-------------------------------------------------------------------------------------------------
	/// Sample of HairShape in time. 
	///-------------------------------------------------------------------------------------------------
	struct Sample
	{
		Time mSampleTime;   ///< Time of the sample

		std::string mFileName; ///< Filename of the file with HairShape time sample
	};

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing the samples .
	///-------------------------------------------------------------------------------------------------
	typedef std::vector< Sample > Samples;

	Samples samples;	///< The samples of HairShape in different times
};

} // namespace RibExport

} // namespace Stubble

#endif // STUBBLE_CACHED_FRAME_HPP