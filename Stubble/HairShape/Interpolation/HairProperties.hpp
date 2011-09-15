#ifndef STUBBLE_HAIR_PROPERTIES_HPP
#define STUBBLE_HAIR_PROPERTIES_HPP

#include "HairShape/HairComponents/Segments.hpp"
#include "HairShape/HairComponents/RestPositionsUG.hpp"
#include "HairShape/Texture/Texture.hpp"
#include "InterpolationGroups.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Hair properties. Stores all properties of the interpolated hair.
///-------------------------------------------------------------------------------------------------
class HairProperties
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	~HairProperties();

	/* HERE WILL BE ALL HAIR PROPERTIES ACCESSIBLE BY GETTER */

	///----------------------------------------------------------------------------------------------------
	/// Gets the density texture. 
	///
	/// \return	The density texture. 
	///----------------------------------------------------------------------------------------------------
	inline const Texture & getDensityTexture() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the interpolation groups. 
	///
	/// \return	The interpolation groups. 
	///----------------------------------------------------------------------------------------------------
	inline const InterpolationGroups & getInterpolationGroups() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the number of guides to interpolate from. 
	///
	/// \return	The number of guides to interpolate from. 
	///----------------------------------------------------------------------------------------------------
	inline unsigned __int32 getNumberOfGuidesToInterpolateFrom() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the guides segments. 
	///
	/// \return	The guides segments. 
	///----------------------------------------------------------------------------------------------------
	inline const HairComponents::GuidesSegments & getGuidesSegments() const;

	///----------------------------------------------------------------------------------------------------
	/// Gets the guides rest positions uniform grid. 
	///
	/// \return	The guides rest positions uniform grid. 
	///----------------------------------------------------------------------------------------------------
	inline const HairComponents::RestPositionsUG & getGuidesRestPositionsUG() const;

protected:
	
	///-------------------------------------------------------------------------------------------------
	/// Default constructor. Properties will be set by class deriving from HairProperties.
	///-------------------------------------------------------------------------------------------------
	HairProperties();

	static const unsigned __int32 DEFAULT_SEGMENTS_COUNT = 5;

	/* HERE WILL BE STORED ALL HAIR PROPERTIES */

	Texture * mDensityTexture;  ///< The density texture

	Texture * mInterpolationGroupsTexture;  ///< The interpolation groups texture

	InterpolationGroups * mInterpolationGroups;  ///< Interpolation groups object

	unsigned __int32 mNumberOfGuidesToInterpolateFrom;  ///< Number of guides to interpolate from

	const HairComponents::GuidesSegments * mGuidesSegments;   ///< The guides segments

	const HairComponents::RestPositionsUG * mGuidesRestPositionsUG;   ///< The guides rest positions uniform grid

};

// inline functions implementation

inline const Texture & HairProperties::getDensityTexture() const
{
	return *mDensityTexture;
}

inline const InterpolationGroups & HairProperties::getInterpolationGroups() const
{
	return *mInterpolationGroups;
}

inline unsigned __int32 HairProperties::getNumberOfGuidesToInterpolateFrom() const
{
	return mNumberOfGuidesToInterpolateFrom;
}

inline const HairComponents::GuidesSegments & HairProperties::getGuidesSegments() const
{
	return *mGuidesSegments;
}

inline const HairComponents::RestPositionsUG & HairProperties::getGuidesRestPositionsUG() const
{
	return *mGuidesRestPositionsUG;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_PROPERTIES_HPP