#ifndef STUBBLE_MAYA_HAIR_PROPERTIES_HPP
#define STUBBLE_MAYA_HAIR_PROPERTIES_HPP

#include "HairProperties.hpp"

#include <ostream>

#include <maya/MDataHandle.h>
#include <maya/MPlug.h>
#include <maya/MPxNode.h>
#include <maya/MObject.h> 
#include <maya/MStatus.h>

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Maya Hair properties. Stores all properties of the interpolated hair used in Maya.
///-------------------------------------------------------------------------------------------------
class MayaHairProperties : public HairProperties
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Export hair properties to file. 
	///
	/// \param [in,out]	aOutputStream	The file output stream. 
	///----------------------------------------------------------------------------------------------------
	void exportToFile( std::ostream & aOutputStream ) const;

	/* MAYA PROPERTIES */
	static MObject densityTextureAttr; ///< The density texture attribute

	static MObject interpolationGroupsTextureAttr; ///< The interpolation groups texture attribute

	static MObject segmentsCountAttr; ///< The segments count attribute

	static MObject numberOfGuidesToInterpolateFromAttr; ///< Number of guides to interpolate from attribute

protected:
	
	///-------------------------------------------------------------------------------------------------
	/// Default constructor.
	///-------------------------------------------------------------------------------------------------
	MayaHairProperties();

	///----------------------------------------------------------------------------------------------------
	/// Initializes attributes for Maya.
	///
	/// \return	Maya status code. 
	///----------------------------------------------------------------------------------------------------
	static MStatus initializeAttributes();

	///----------------------------------------------------------------------------------------------------
	/// Sets the attributes values. 
	///
	/// \param	aPlug							Which attribute is being set.
	/// \param	aDataHandle						Handle to data storage. 
	/// \param [in,out]	aSegmentsCountChanged	The segments count has changed. 
	/// \param [in,out]	aHairPropertiesChanged	The hair properties has changed. 
	///----------------------------------------------------------------------------------------------------
	void setAttributesValues( const MPlug& aPlug, const MDataHandle& aDataHandle,
		bool & aSegmentsCountChanged, bool & aHairPropertiesChanged);

	///----------------------------------------------------------------------------------------------------
	/// Refresh textures. 
	///
	/// \param [in,out]	aDensityChanged				The density has changed. 
	/// \param [in,out]	aInterpolationGroupsChanged	The interpolation groups has changed. 
	/// \param [in,out]	aHairPropertiesChanged		The hair properties has changed. 
	///----------------------------------------------------------------------------------------------------
	void refreshTextures( bool & aDensityChanged, bool & aInterpolationGroupsChanged, 
		bool & aHairPropertiesChanged );

	///----------------------------------------------------------------------------------------------------
	/// Sets the textures time. 
	///
	/// \param	aTime	The curren time.
	///----------------------------------------------------------------------------------------------------
	void setTexturesTime( Time aTime );

	///----------------------------------------------------------------------------------------------------
	/// Refresh pointers to guides. 
	///
	/// \param	aGuidesSegments			The guides segments. 
	/// \param	aGuidesRestPositionsUG	The guides rest positions ug. 
	///----------------------------------------------------------------------------------------------------
	inline void refreshPointersToGuides( const HairComponents::GuidesSegments * aGuidesSegments,
		const HairComponents::RestPositionsUG * aGuidesRestPositionsUG );

};

// inline functions implementation

inline void MayaHairProperties::refreshPointersToGuides( const HairComponents::GuidesSegments * aGuidesSegments,
	const HairComponents::RestPositionsUG * aGuidesRestPositionsUG )
{
	mGuidesSegments = aGuidesSegments;
	mGuidesRestPositionsUG = aGuidesRestPositionsUG;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MAYA_HAIR_PROPERTIES_HPP