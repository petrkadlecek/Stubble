#ifndef STUBBLE_CACHED_FRAME_HPP
#define STUBBLE_CACHED_FRAME_HPP

#include "Common/CommonTypes.hpp"
#include "HairShape/UserInterface/HairShape.hpp"
#include "Primitives/BoundingBox.hpp"

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
	/// \param	aSampleTime	Time of the sample. 
	///-------------------------------------------------------------------------------------------------
	CachedFrame( HairShape::HairShape & aHairShape, Time aSampleTime );

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	~CachedFrame();

	///-------------------------------------------------------------------------------------------------
	/// Emits this object (renders stored frame).
	///-------------------------------------------------------------------------------------------------
	void emit();

private:

	bool mIsEmitted;	///< true if this object was emitted

	BoundingBoxes mBoundingBoxes;   ///< The bounding boxes of voxels

	std::string mFileName;  ///< Filename of the file with HairShape time sample
};

} // namespace RibExport

} // namespace Stubble

#endif // STUBBLE_CACHED_FRAME_HPP