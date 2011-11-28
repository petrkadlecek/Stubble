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

	/* MAYA BASIC PROPERTIES */
	static MObject densityTextureAttr; ///< The density texture attribute

	static MObject interpolationGroupsTextureAttr; ///< The interpolation groups texture attribute

	static MObject cutTextureAttr;  ///< The cut texture attribute

	static MObject segmentsCountAttr; ///< The segments count attribute

	static MObject interpolationGroupsSelectableAttr; ///< The interpolation groups selectable attribute

	static MObject interpolationGroupsColorsAttr;   ///< The interpolation groups colors attribute

	static MObject numberOfGuidesToInterpolateFromAttr; ///< Number of guides to interpolate from attribute

	static MObject areNormalsCalculatedAttr;	///< The are normals calculated attribute

	static MObject scaleTextureAttr;	///< The scale texture attribute

	static MObject scaleAttr;   ///< The scale attribute

	static MObject randScaleTextureAttr;	///< The rand scale texture attribute

	static MObject randScaleAttr;   ///< The rand scale attribute

	static MObject rootThicknessTextureAttr;	///< The root thickness texture attribute

	static MObject rootThicknessAttr;   ///< The root thickness attribute

	static MObject tipThicknessTextureAttr;	///< The tip thickness texture attribute

	static MObject tipThicknessAttr;   ///< The tip thickness attribute

	static MObject displacementTextureAttr; ///< The displacement texture attribute

	static MObject displacementAttr; ///< The displacement attribute

	static MObject skipThresholdAttr;	///< The skip threshold attribute

	/* MAYA COLOR PROPERTIES */

	static MObject rootOpacityTextureAttr;  ///< The root opacity texture attribute

	static MObject rootOpacityAttr;  ///< The root opacity attribute

	static MObject tipOpacityTextureAttr;  ///< The tip opacity texture attribute

	static MObject tipOpacityAttr;  ///< The tip opacity attribute

	static MObject rootColorTextureAttr;  ///< The root color texture attribute

	static MObject rootColorAttr;  ///< The root color attribute

	static MObject tipColorTextureAttr;  ///< The tip color texture attribute

	static MObject tipColorAttr;  ///< The tip color attribute

	static MObject hueVariationTextureAttr; ///< The hue variation texture attribute

	static MObject hueVariationAttr; ///< The hue variation attribute

	static MObject valueVariationTextureAttr; ///< The value variation texture attribute

	static MObject valueVariationAttr; ///< The value variation attribute

	static MObject mutantHairColorTextureAttr;  ///< The mutant hair color texture attribute

	static MObject mutantHairColorAttr;  ///< The mutant hair color attribute

	static MObject percentMutantHairTextureAttr;	///< The percent mutant hair texture attribute

	static MObject percentMutantHairAttr;	///< The percent mutant hair texture attribute

	/* MAYA FRIZZ PROPERTIES */

	static MObject rootFrizzTextureAttr;	///< The root frizz texture attribute

	static MObject rootFrizzAttr;	///< The root frizz attribute

	static MObject tipFrizzTextureAttr;	///< The tip frizz texture attribute

	static MObject tipFrizzAttr;	///< The tip frizz attribute

	static MObject frizzXFrequencyTextureAttr;  ///< The frizz x coordinate frequency texture attribute

	static MObject frizzXFrequencyAttr;  ///< The frizz x coordinate frequency attribute

	static MObject frizzYFrequencyTextureAttr;  ///< The frizz y coordinate frequency texture attribute

	static MObject frizzYFrequencyAttr;  ///< The frizz y coordinate frequency attribute

	static MObject frizzZFrequencyTextureAttr;  ///< The frizz z coordinate frequency texture attribute

	static MObject frizzZFrequencyAttr;  ///< The frizz z coordinate frequency attribute

	static MObject frizzAnimTextureAttr;	///< The frizz animation texture attribute
	
	static MObject frizzAnimAttr;	///< The frizz animation attribute

	static MObject frizzAnimSpeedTextureAttr;	///< The frizz animation speed texture attribute
	
	static MObject frizzAnimSpeedAttr;	///< The frizz animation speed attribute

	static MObject frizzAnimDirectionAttr;  ///< The frizz animation direction attribute

	static MObject frizzAnimDirectionXAttr;  ///< The frizz animation direction X attribute
	
	static MObject frizzAnimDirectionYAttr;  ///< The frizz animation direction Y attribute

	static MObject frizzAnimDirectionZAttr;  ///< The frizz animation direction Z attribute

	/* MAYA KINK PROPERTIES */

	static MObject rootKinkTextureAttr;	///< The root kink texture attribute

	static MObject rootKinkAttr;	///< The root kink attribute

	static MObject tipKinkTextureAttr;	///< The tip kink texture attribute

	static MObject tipKinkAttr;	///< The tip kink attribute

	static MObject kinkXFrequencyTextureAttr;  ///< The kink x coordinate frequency texture attribute

	static MObject kinkXFrequencyAttr;  ///< The kink x coordinate frequency attribute

	static MObject kinkYFrequencyTextureAttr;  ///< The kink y coordinate frequency texture attribute

	static MObject kinkYFrequencyAttr;  ///< The kink y coordinate frequency attribute

	static MObject kinkZFrequencyTextureAttr;  ///< The kink z coordinate frequency texture attribute

	static MObject kinkZFrequencyAttr;  ///< The kink z coordinate frequency attribute

	/* MAYA MULTI STRAND PROPERTIES */

	static MObject multiStrandCountAttr;	///< The multi strand count attribute

	static MObject rootSplayTextureAttr;	///< The root splay texture attribute

	static MObject rootSplayAttr;	///< The root splayattribute

	static MObject tipSplayTextureAttr;	///< The tip splay texture attribute

	static MObject tipSplayAttr;	///< The tip splayattribute

	static MObject centerSplayTextureAttr;	///< The center splay texture attribute

	static MObject centerSplayAttr;	///< The center splayattribute

	static MObject twistTextureAttr;	///< The twist texture attribute

	static MObject twistAttr;	///< The twist attribute

	static MObject offsetTextureAttr;	///< The offset texture attribute

	static MObject offsetAttr;	///< The offset attribute

	static MObject aspectTextureAttr;	///< The aspect texture attribute

	static MObject aspectAttr;	///< The aspect attribute

	static MObject randomizeStrandTextureAttr;	///< The randomizeStrand texture attribute

	static MObject randomizeStrandAttr;	///< The randomizeStrand attribute

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
	/// Brakes a connection to plug on our node.
	///
	/// \param aPlug				Which plug should be disconnected.
	///
	/// \return Maya status code.
	///----------------------------------------------------------------------------------------------------
	MStatus brakeConnection( const MPlug & aPlug );

	///----------------------------------------------------------------------------------------------------
	/// Refresh textures. 
	///
	/// \param [in] aTextureSamples					The number of samples in one dimension of sampled texture.
	/// \param [in,out]	aDensityChanged				The density has changed. 
	/// \param [in,out]	aInterpolationGroupsChanged	The interpolation groups has changed. 
	/// \param [in,out]	aHairPropertiesChanged		The hair properties has changed. 
	///----------------------------------------------------------------------------------------------------
	void refreshTextures(unsigned __int32 aTextureSamples, bool & aDensityChanged,
		bool & aInterpolationGroupsChanged, bool & aHairPropertiesChanged );

	///----------------------------------------------------------------------------------------------------
	/// Sets the current time. 
	///
	/// \param	aTime	The current time.
	///----------------------------------------------------------------------------------------------------
	void setCurrentTime( Time aTime );

	///----------------------------------------------------------------------------------------------------
	/// Refresh pointers to guides. 
	///
	/// \param	aGuidesSegments			The guides segments. 
	/// \param	aGuidesRestPositionsUG	The guides rest positions ug. 
	///----------------------------------------------------------------------------------------------------
	inline void refreshPointersToGuides( const HairComponents::GuidesSegments * aGuidesSegments,
		const HairComponents::RestPositionsUG * aGuidesRestPositionsUG );

	///-------------------------------------------------------------------------------------------------
	/// Sets the segments count attribute. 
	///
	/// \param	aSegmentsCountPlug	The segments count attribute. 
	///-------------------------------------------------------------------------------------------------
	inline void setSegmentsCountAttr( const MObject & aSegmentsCountAttr );

	///-------------------------------------------------------------------------------------------------
	/// Sets the selectable interpolation groups attribute. 
	///
	/// \param	aInterpolationGroupsSelectableAttr	The selectable interpolation groups attribute. 
	///-------------------------------------------------------------------------------------------------
	inline void setInterpolationGroupsSelectableAttr( const MObject & aInterpolationGroupsSelectableAttr );

	///-------------------------------------------------------------------------------------------------
	/// Sets a scale factor. 
	///
	/// \param	aScaleFactor	a scale factor. 
	///-------------------------------------------------------------------------------------------------
	inline void setScaleFactor( Real aScaleFactor );

	///-------------------------------------------------------------------------------------------------
	/// Gets the scale factor. 
	///
	/// \return	The scale factor. 
	///-------------------------------------------------------------------------------------------------
	inline Real getScaleFactor() const;

	///-------------------------------------------------------------------------------------------------
	/// Updates components count for an int array attribute. 
	///
	/// \param [in,out]	aAttribute	The attribute object.
	/// \param	aComponentsCount	The component count.
	/// \param	aDefault			The default value. 
	/// \param	aMin				The minimum value. 
	/// \param	aMax				The maximum value. 
	/// \param	aSoftMin			The soft minimum value. 
	/// \param	aSoftMax			The soft maximum value.
	/// \param	aGroupNamePrefix	The group name prefix.
	///-------------------------------------------------------------------------------------------------
	void MayaHairProperties::updateIntArrayComponentsCount( MObject & aAttribute, unsigned int aComponentsCount,
		int aDefault, int aMin, int aMax, int aSoftMin, int aSoftMax, MString aGroupNamePrefix = "group_" );

	///-------------------------------------------------------------------------------------------------
	/// Adds a color attribute. 
	///
	/// \param	aFullName			Full name.
	/// \param	aBriefName			Brief name.
	/// \param [in,out]	aAttribute	The attribute object. 
	/// \param	aDefaultR			The default r value. 
	/// \param	aDefaultB			The default b value. 
	/// \param	aDefaultG			The default g value. 
	///-------------------------------------------------------------------------------------------------
	static void addColorAttribute( const MString & aFullName, const MString & aBriefName,
		MObject & aAttribute, float aDefaultR, float aDefaultB, float aDefaultG );

	///-------------------------------------------------------------------------------------------------
	/// Adds a float attribute. 
	///
	/// \param	aFullName			Full name.
	/// \param	aBriefName			Brief name.
	/// \param [in,out]	aAttribute	The attribute object. 
	/// \param	aDefault			The default value. 
	/// \param	aMin				The minimum value. 
	/// \param	aMax				The maximum value. 
	/// \param	aSoftMin			The soft minimum value. 
	/// \param	aSoftMax			The soft maximum value. 
	///-------------------------------------------------------------------------------------------------
	static void addFloatAttribute( const MString & aFullName, const MString & aBriefName,
		MObject & aAttribute, float aDefault, float aMin, float aMax, float aSoftMin, float aSoftMax );

	///-------------------------------------------------------------------------------------------------
	/// Adds an int attribute. 
	///
	/// \param	aFullName			Full name.
	/// \param	aBriefName			Brief name.
	/// \param [in,out]	aAttribute	The attribute object. 
	/// \param	aDefault			The default value. 
	/// \param	aMin				The minimum value. 
	/// \param	aMax				The maximum value. 
	/// \param	aSoftMin			The soft minimum value. 
	/// \param	aSoftMax			The soft maximum value. 
	///-------------------------------------------------------------------------------------------------
	static void addIntAttribute( const MString & aFullName, const MString & aBriefName,
		MObject & aAttribute, int aDefault, int aMin, int aMax, int aSoftMin, int aSoftMax );

	///-------------------------------------------------------------------------------------------------
	/// Adds an int array attribute. 
	///
	/// \param	aFullName			Full name.
	/// \param	aBriefName			Brief name.
	/// \param [in,out]	aAttribute	The attribute object. 
	/// \param	aComponentsCount	The component count.
	/// \param	aDefault			The default value. 
	/// \param	aMin				The minimum value. 
	/// \param	aMax				The maximum value. 
	/// \param	aSoftMin			The soft minimum value. 
	/// \param	aSoftMax			The soft maximum value. 
	///-------------------------------------------------------------------------------------------------
	static void MayaHairProperties::addIntArrayAttribute( const MString & aFullName, const MString & aBriefName,
	MObject & aAttribute, int aComponentsCount, int aDefault, int aMin, int aMax, int aSoftMin, int aSoftMax );

	///-------------------------------------------------------------------------------------------------
	/// Adds a bool attribute. 
	///
	/// \param	aFullName			Full name.
	/// \param	aBriefName			Brief name.
	/// \param [in,out]	aAttribute	The attribute object. 
	/// \param	aDefault			The default value. 
	///-------------------------------------------------------------------------------------------------
	static void addBoolAttribute( const MString & aFullName, const MString & aBriefName,
		MObject & aAttribute, bool aDefault );

	///-------------------------------------------------------------------------------------------------
	/// Adds a parent attribute. 
	///
	/// \param	aFullName			Full name.
	/// \param	aBriefName			Brief name.
	/// \param [in,out]	aAttribute	The attribute object. 
	/// \param	aChildAttribute1	The child attribute no 1.
	/// \param	aChildAttribute2	The child attribute no 2.
	/// \param	aChildAttribute3	The child attribute no 3.
	///-------------------------------------------------------------------------------------------------
	static void addParentAttribute( const MString & aFullName, const MString & aBriefName,
		MObject & aAttribute, const MObject & aChildAttribute1, const MObject & aChildAttribute2, 
		const MObject & aChildAttribute3 );

	///-------------------------------------------------------------------------------------------------
	/// Fills already created int array attribute. 
	///
	/// \param [in,out]	aAttribute	The attribute object. 
	/// \param	aFillCount			The number of newly created array items.
	/// \param	aDefault			The default value. 
	/// \param	aMin				The minimum value. 
	/// \param	aMax				The maximum value. 
	/// \param	aSoftMin			The soft minimum value. 
	/// \param	aSoftMax			The soft maximum value.
	/// \param	aGroupNamePrefix	The group name prefix.
	///-------------------------------------------------------------------------------------------------
	static void fillIntArrayAttributes( MObject & aAttribute, int aFillCount, int aDefault, int aMin, int aMax, int aSoftMin, int aSoftMax, MString aGroupNamePrefix = "group_" );

	///-------------------------------------------------------------------------------------------------
	/// Fill color array attributes. 
	///
	/// \param [in,out]	aAttribute		The attribute object. 
	/// \param	aInterpolationGroups	Interpolation groups object.
	///-------------------------------------------------------------------------------------------------
	static void fillColorArrayAttributes( MObject & aAttribute, const InterpolationGroups & aInterpolationGroups );

private:

	MObject mSegmentsCountAttr;   ///< The segments count attribute

	MObject mInterpolationGroupsSelectableAttr; ///< The selectable interpolation groups attribute

	Real mScaleFactor;  ///< The scale factor for all size dependent attributes
};

// inline functions implementation

inline void MayaHairProperties::refreshPointersToGuides( const HairComponents::GuidesSegments * aGuidesSegments,
	const HairComponents::RestPositionsUG * aGuidesRestPositionsUG )
{
	mGuidesSegments = aGuidesSegments;
	mGuidesRestPositionsUG = aGuidesRestPositionsUG;
}

inline void MayaHairProperties::setSegmentsCountAttr( const MObject & aSegmentsCountAttr )
{
	mSegmentsCountAttr = aSegmentsCountAttr;
}

inline void MayaHairProperties::setInterpolationGroupsSelectableAttr( const MObject & aInterpolationGroupsSelectableAttr )
{
	mInterpolationGroupsSelectableAttr = aInterpolationGroupsSelectableAttr;
}

inline void MayaHairProperties::setScaleFactor( Real aScaleFactor )
{
	aScaleFactor *= 0.02f; // Reasonable scaling..
	Real change = aScaleFactor / mScaleFactor;
	mScaleFactor = aScaleFactor;
	// Rescale all size depented attributes :
	mRootThickness *= change;
	mTipThickness *= change;
	mRootFrizz *= change;
	mTipFrizz *= change;
	mFrizzXFrequency *= change;
	mFrizzYFrequency *= change;
	mFrizzZFrequency *= change;
	mFrizzAnimSpeed *= change;
	mRootKink *= change;
	mTipKink *= change;
	mKinkXFrequency *= change;
	mKinkYFrequency *= change;
	mKinkZFrequency *= change;
	mRootSplay *= change;
	mTipSplay *= change;
	mCenterSplay *= change;
	mOffset *= change;
}

inline Real MayaHairProperties::getScaleFactor() const
{
	return mScaleFactor;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MAYA_HAIR_PROPERTIES_HPP