#ifndef STUBBLE_RM_HAIR_PROPERTIES_HPP
#define STUBBLE_RM_HAIR_PROPERTIES_HPP

#include "../HairProperties.hpp"

#include <fstream>
#include <string>

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Renderman Hair properties. Stores all properties of the interpolated hair used in RM.
/// These properties are imported from file to which they had been exported by Maya plugin
/// ( see HairShape::sampleTime and MayaHairProperties::exportToFile )
///-------------------------------------------------------------------------------------------------
class RMHairProperties : public HairProperties
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Constructor. Loads properties from frame file name
	///
	/// \param	aFrameFileName	Filename of a frame file. 
	///----------------------------------------------------------------------------------------------------
	RMHairProperties( const std::string & aFrameFileName );

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	~RMHairProperties();

private:
	/* RMHairProperties owns guides data */
	HairComponents::GuidesSegments * mGuidesSegmentsMutable;   ///< The guides segments

	HairComponents::RestPositionsDS * mGuidesRestPositionsDSMutable;   ///< The guides rest positions data structure

};

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_RM_HAIR_PROPERTIES_HPP