#ifndef STUBBLE_MAYA_HAIR_PROPERTIES_HPP
#define STUBBLE_MAYA_HAIR_PROPERTIES_HPP

#include "../HairProperties.hpp"

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

namespace Maya
{

///-------------------------------------------------------------------------------------------------
/// Stores all properties of the interpolated hair used in Maya.
/// Also creates maya attributes asociated with interpolated hair properties and
/// handles events caused by attributes change.
/// Can be exported to binary file.
///-------------------------------------------------------------------------------------------------
class MayaHairProperties : public HairProperties
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Export hair properties to file.
	/// This is necessary for rendering hair in for example render man ( RMHairProperties will import the
	/// hair properties ).
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

	/* TEXTURE DIMENSIONS FOR SAMPLING ATTRIBUTES */

	/// The density texture sampling dimesion in U attribute
	static MObject densityTextureSamplingUDimensionAttr;

	/// The density texture sampling dimesion in V attribute
	static MObject densityTextureSamplingVDimensionAttr;

	/// The interpolation groups texture sampling dimesion in U attribute
	static MObject interpolationGroupsTextureSamplingUDimensionAttr;

	/// The interpolation groups texture sampling dimesion in V attribute
	static MObject interpolationGroupsTextureSamplingVDimensionAttr;

	/// The cut texture sampling dimesion in U attribute
	static MObject cutTextureSamplingUDimensionAttr;

	/// The cut texture sampling dimesion in V attribute
	static MObject cutTextureSamplingVDimensionAttr;

	/// The scale texture sampling dimension in U atribute
	static MObject scaleTextureSamplingUDimensionAttr;

	/// The scale texture sampling dimension in V atribute
	static MObject scaleTextureSamplingVDimensionAttr;

	/// The rand scale texture sampling dimension in U atribute
	static MObject randScaleTextureSamplingUDimensionAttr;

	/// The rand scale texture sampling dimension in V atribute
	static MObject randScaleTextureSamplingVDimensionAttr;

	/// The root thickness texture sampling dimension in U atribute
	static MObject rootThicknessTextureSamplingUDimensionAttr;

	/// The root thickness texture sampling dimension in V atribute
	static MObject rootThicknessTextureSamplingVDimensionAttr;

	/// The tip thickness texture sampling dimension in U attribute
	static MObject tipThicknessTextureSamplingUDimensionAttr;

	/// The tip thickness texture sampling dimension in V attribute
	static MObject tipThicknessTextureSamplingVDimensionAttr;

	/// The displacement texture sampling dimension in U atribute
	static MObject displacementTextureSamplingUDimensionAttr;

	/// The displacement texture sampling dimension in V atribute
	static MObject displacementTextureSamplingVDimensionAttr;

	/* MAYA COLOR PROPERTIES */                                                                                                 

	/// The root opacity texture sampling dimension in U atribute
	static MObject rootOpacityTextureSamplingUDimensionAttr;

	/// The root opacity texture sampling dimension in V atribute
	static MObject rootOpacityTextureSamplingVDimensionAttr;

	/// The tip opacity texture sampling dimension in U atribute
	static MObject tipOpacityTextureSamplingUDimensionAttr;

	/// The tip opacity texture sampling dimension in V atribute
	static MObject tipOpacityTextureSamplingVDimensionAttr;

	/// The root color texture sampling dimension in U atribute
	static MObject rootColorTextureSamplingUDimensionAttr;

	/// The root color texture sampling dimension in V atribute
	static MObject rootColorTextureSamplingVDimensionAttr;

	/// The tip color texture sampling dimension in U atribute
	static MObject tipColorTextureSamplingUDimensionAttr;

	/// The tip color texture sampling dimension in V atribute
	static MObject tipColorTextureSamplingVDimensionAttr;

	/// The hue variation texture sampling dimension in U atribute
	static MObject hueVariationTextureSamplingUDimensionAttr;

	/// The hue variation texture sampling dimension in V atribute
	static MObject hueVariationTextureSamplingVDimensionAttr;

	/// The value variation texture sampling dimension in U atribute
	static MObject valueVariationTextureSamplingUDimensionAttr;

	/// The value variation texture sampling dimension in V atribute
	static MObject valueVariationTextureSamplingVDimensionAttr;

	/// The mutant hair color texture sampling dimension in U atribute
	static MObject mutantHairColorTextureSamplingUDimensionAttr;

	/// The mutant hair color texture sampling dimension in V atribute
	static MObject mutantHairColorTextureSamplingVDimensionAttr;

	/// The percent mutant hair texture sampling dimension in U atribute
	static MObject percentMutantHairTextureSamplingUDimensionAttr;

	/// The percent mutant hair texture sampling dimension in V atribute
	static MObject percentMutantHairTextureSamplingVDimensionAttr;

	/* MAYA FRIZZ PROPERTIES */

	/// The root frizz texture sampling dimension in U atribute
	static MObject rootFrizzTextureSamplingUDimensionAttr;

	/// The root frizz texture sampling dimension in V atribute
	static MObject rootFrizzTextureSamplingVDimensionAttr;

	/// The tip frizz texture sampling dimension in U atribute
	static MObject tipFrizzTextureSamplingUDimensionAttr;

	/// The tip frizz texture sampling dimension in V atribute
	static MObject tipFrizzTextureSamplingVDimensionAttr;

	/// The frizz x coordinate frequency texture sampling dimension in U atribute
	static MObject frizzXFrequencyTextureSamplingUDimensionAttr;

	/// The frizz x coordinate frequency texture sampling dimension in V atribute
	static MObject frizzXFrequencyTextureSamplingVDimensionAttr;

	/// The frizz y coordinate frequency texture sampling dimension in U atribute
	static MObject frizzYFrequencyTextureSamplingUDimensionAttr;

	/// The frizz y coordinate frequency texture sampling dimension in V atribute
	static MObject frizzYFrequencyTextureSamplingVDimensionAttr;

	/// The frizz z coordinate frequency texture sampling dimension in U atribute
	static MObject frizzZFrequencyTextureSamplingUDimensionAttr;

	/// The frizz z coordinate frequency texture sampling dimension in V atribute
	static MObject frizzZFrequencyTextureSamplingVDimensionAttr;

	/// The frizz animation texture sampling dimension in U atribute
	static MObject frizzAnimTextureSamplingUDimensionAttr;

	/// The frizz animation texture sampling dimension in V atribute
	static MObject frizzAnimTextureSamplingVDimensionAttr;

	/// The frizz animation speed texture sampling dimension in U atribute
	static MObject frizzAnimSpeedTextureSamplingUDimensionAttr;

	/// The frizz animation speed texture sampling dimension in V atribute
	static MObject frizzAnimSpeedTextureSamplingVDimensionAttr;

	/* MAYA KINK PROPERTIES */

	/// The root kink texture sampling dimension in U atribute
	static MObject rootKinkTextureSamplingUDimensionAttr;

	/// The root kink texture sampling dimension in V atribute
	static MObject rootKinkTextureSamplingVDimensionAttr;

	/// The tip kink texture sampling dimension in U atribute
	static MObject tipKinkTextureSamplingUDimensionAttr;

	/// The tip kink texture sampling dimension in V atribute
	static MObject tipKinkTextureSamplingVDimensionAttr;

	/// The kink x coordinate frequency texture sampling dimension in U atribute
	static MObject kinkXFrequencyTextureSamplingUDimensionAttr;

	/// The kink x coordinate frequency texture sampling dimension in V atribute
	static MObject kinkXFrequencyTextureSamplingVDimensionAttr;

	/// The kink y coordinate frequency texture sampling dimension in U atribute
	static MObject kinkYFrequencyTextureSamplingUDimensionAttr;

	/// The kink y coordinate frequency texture sampling dimension in V atribute
	static MObject kinkYFrequencyTextureSamplingVDimensionAttr;

	/// The kink z coordinate frequency texture sampling dimension in U atribute
	static MObject kinkZFrequencyTextureSamplingUDimensionAttr;

	/// The kink z coordinate frequency texture sampling dimension in V atribute
	static MObject kinkZFrequencyTextureSamplingVDimensionAttr;

	/* MAYA MULTI STRAND PROPERTIES */

	/// The root splay texture sampling dimension in U atribute
	static MObject rootSplayTextureSamplingUDimensionAttr;

	/// The root splay texture sampling dimension in V atribute
	static MObject rootSplayTextureSamplingVDimensionAttr;

	/// The tip splay texture sampling dimension in U atribute
	static MObject tipSplayTextureSamplingUDimensionAttr;

	/// The tip splay texture sampling dimension in V atribute
	static MObject tipSplayTextureSamplingVDimensionAttr;

	/// The center splay texture sampling dimension in U atribute
	static MObject centerSplayTextureSamplingUDimensionAttr;

	/// The center splay texture sampling dimension in V atribute
	static MObject centerSplayTextureSamplingVDimensionAttr;

	/// The twist texture sampling dimension in U atribute
	static MObject twistTextureSamplingUDimensionAttr;

	/// The twist texture sampling dimension in V atribute
	static MObject twistTextureSamplingVDimensionAttr;

	/// The offset texture sampling dimension in U atribute
	static MObject offsetTextureSamplingUDimensionAttr;

	/// The offset texture sampling dimension in V atribute
	static MObject offsetTextureSamplingVDimensionAttr;

	/// The aspect texture sampling dimension in U atribute
	static MObject aspectTextureSamplingUDimensionAttr;

	/// The aspect texture sampling dimension in V atribute
	static MObject aspectTextureSamplingVDimensionAttr;

	/// The randomizeStrand texture sampling dimension in U atribute
	static MObject randomizeStrandTextureSamplingUDimensionAttr;

	/// The randomizeStrand texture sampling dimension in V atribute
	static MObject randomizeStrandTextureSamplingVDimensionAttr;

protected:
	
	///-------------------------------------------------------------------------------------------------
	/// Default constructor.
	/// MayaHairProperties can not exist on its own, it must be inherited.
	///-------------------------------------------------------------------------------------------------
	MayaHairProperties();

	///----------------------------------------------------------------------------------------------------
	/// Initializes attributes for Maya.
	/// This method is static, because it initializes attributes for all HairShapes in current scene.
	///
	/// \return	Maya status code. 
	///----------------------------------------------------------------------------------------------------
	static MStatus initializeAttributes();

	///----------------------------------------------------------------------------------------------------
	/// Sets the attributes values.
	/// Catches event from maya and changes internal properties according to them. 
	/// This method also returns several flags, which informs caller if any critical attributes has been
	/// changed.
	///
	/// \param	aPlug											Which attribute is being set.
	/// \param	aDataHandle										Handle to data storage of attribute. 
	/// \param [in,out]	aSegmentsCountChanged					The segments count has changed.
	/// \param [in,out]	aInterpolationGroupsSelectableChanged	The selectable interpolation groups attribute has changed.
	/// \param [in,out]	aHairPropertiesChanged					The hair properties has changed. 
	/// 	
	/// \return Returns true, only if any texture was changed ( prevent Maya errors )
	///----------------------------------------------------------------------------------------------------
	bool setAttributesValues( const MPlug& aPlug, const MDataHandle& aDataHandle,
		bool & aSegmentsCountChanged, bool & aInterpolationGroupsSelectableChanged, bool & aHairPropertiesChanged);

	///----------------------------------------------------------------------------------------------------
	/// Brakes a connection to plug on our node.
	/// Used only with texture attributes, texture will be set as dirty and reseted to default value later.
	///
	/// \param aPlug				Which plug should be disconnected.
	///
	/// \return Maya status code.
	///----------------------------------------------------------------------------------------------------
	MStatus brakeConnection( const MPlug & aPlug );

	///----------------------------------------------------------------------------------------------------
	/// Refresh textures. 
	/// Resamples all textures, that has been set as dirty ( unless aForceRefresh is set then all textures
	/// are resampled ). This method also returns several flags, which informs caller if any critical 
	/// textures has been changed.
	///
	/// \param aTextureSamples						The number of samples in one dimension of sampled texture.
	/// \param aForceRefresh						Should we refresh all textures ?
	/// \param [in,out]	aDensityChanged				The density has changed. 
	/// \param [in,out]	aInterpolationGroupsChanged	The interpolation groups has changed. 
	/// \param [in,out]	aHairPropertiesChanged		The hair properties has changed. 
	///----------------------------------------------------------------------------------------------------
	void refreshTextures( unsigned __int32 aTextureSamples, bool aForceRefresh, bool & aDensityChanged,
		bool & aInterpolationGroupsChanged, bool & aHairPropertiesChanged );

	///----------------------------------------------------------------------------------------------------
	/// Sets the current time.
	/// Time is stored only for export. 
	///
	/// \param	aTime	The current time.
	///----------------------------------------------------------------------------------------------------
	void setCurrentTime( Time aTime );

	///----------------------------------------------------------------------------------------------------
	/// Refresh pointers to hair guides. 
	/// For interpolating hair, pointers to guides segments and rest positions data structure must be kept
	/// among hair properties. 
	///
	/// \param	aGuidesSegments			The guides segments. 
	/// \param	aGuidesRestPositionsDS	The guides rest positions data structure. 
	///----------------------------------------------------------------------------------------------------
	inline void refreshPointersToGuides( const HairComponents::GuidesSegments * aGuidesSegments,
		const HairComponents::RestPositionsDS * aGuidesRestPositionsDS );

	///-------------------------------------------------------------------------------------------------
	/// Sets the segments count attribute.
	/// Updates the segment count attribute handler so maya events concerning segments count can be 
	/// cought. This method is called after interpolation groups change.
	///
	/// \param	aSegmentsCountPlug	The segments count attribute. 
	///-------------------------------------------------------------------------------------------------
	inline void setSegmentsCountAttr( const MObject & aSegmentsCountAttr );

	///-------------------------------------------------------------------------------------------------
	/// Sets the selectable interpolation groups attribute.
	/// Updates the interpolation groups selectable attribute handler so maya events concerning this
	/// attribute can be cought. This method is called after interpolation groups change. 
	///
	/// \param	aInterpolationGroupsSelectableAttr	The selectable interpolation groups attribute. 
	///-------------------------------------------------------------------------------------------------
	inline void setInterpolationGroupsSelectableAttr( const MObject & aInterpolationGroupsSelectableAttr );

	///-------------------------------------------------------------------------------------------------
	/// Sets a scale factor.
	/// Some of the hair properties must be scaled for too small or too big underlying meshes.
	///
	/// \param	aScaleFactor	a scale factor. 
	///-------------------------------------------------------------------------------------------------
	inline void setScaleFactor( Real aScaleFactor );

	///-------------------------------------------------------------------------------------------------
	/// Gets the scale factor.
	/// Some of the hair properties must be scaled for too small or too big underlying meshes. 
	///
	/// \return	The scale factor. 
	///-------------------------------------------------------------------------------------------------
	inline Real getScaleFactor() const;

	///-------------------------------------------------------------------------------------------------
	/// Updates components count for an integer array Maya attribute. 
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
	static void MayaHairProperties::updateIntArrayComponentsCount( MObject & aAttribute, unsigned int aComponentsCount,
		int aDefault, int aMin, int aMax, int aSoftMin, int aSoftMax, MString aGroupNamePrefix = "group_" );

	///-------------------------------------------------------------------------------------------------
	/// Adds a color Maya attribute. 
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
	/// Adds a float Maya attribute. 
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
	/// Adds an int Maya attribute. 
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
	/// Adds an int array Maya attribute. 
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
	/// Adds a bool Maya attribute. 
	///
	/// \param	aFullName			Full name.
	/// \param	aBriefName			Brief name.
	/// \param [in,out]	aAttribute	The attribute object. 
	/// \param	aDefault			The default value. 
	///-------------------------------------------------------------------------------------------------
	static void addBoolAttribute( const MString & aFullName, const MString & aBriefName,
		MObject & aAttribute, bool aDefault );

	///-------------------------------------------------------------------------------------------------
	/// Adds a parent Maya attribute. 
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
	/// Fills already created int array Maya attribute. 
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
	/// Fill color array Maya attributes. 
	///
	/// \param [in,out]	aAttribute		The attribute object. 
	/// \param	aInterpolationGroups	Interpolation groups object.
	///-------------------------------------------------------------------------------------------------
	static void fillColorArrayAttributes( MObject & aAttribute, const InterpolationGroups & aInterpolationGroups );

private:
	/* TEXTURE DIMENSIONS FOR SAMPLING ATTRIBUTES */		
		
	/// The density texture sampling dimesion in U attribute	
	unsigned __int32 mDensityTextureSamplingUDimension;
		
	/// The density texture sampling dimesion in V attribute	
	unsigned __int32 mDensityTextureSamplingVDimension;
		
	/// The interpolation groups texture sampling dimesion in U attribute	
	unsigned __int32 mInterpolationGroupsTextureSamplingUDimension;
		
	/// The interpolation groups texture sampling dimesion in V attribute	
	unsigned __int32 mInterpolationGroupsTextureSamplingVDimension;
		
	/// The cut texture sampling dimesion in U attribute	
	unsigned __int32 mCutTextureSamplingUDimension;
		
	/// The cut texture sampling dimesion in V attribute	
	unsigned __int32 mCutTextureSamplingVDimension;
		
	/// The scale texture sampling dimension in U atribute	
	unsigned __int32 mScaleTextureSamplingUDimension;
		
	/// The scale texture sampling dimension in V atribute	
	unsigned __int32 mScaleTextureSamplingVDimension;
		
	/// The rand scale texture sampling dimension in U atribute	
	unsigned __int32 mRandScaleTextureSamplingUDimension;
		
	/// The rand scale texture sampling dimension in V atribute	
	unsigned __int32 mRandScaleTextureSamplingVDimension;
		
	/// The root thickness texture sampling dimension in U atribute	
	unsigned __int32 mRootThicknessTextureSamplingUDimension;
		
	/// The root thickness texture sampling dimension in V atribute	
	unsigned __int32 mRootThicknessTextureSamplingVDimension;
		
	/// The tip thickness texture sampling dimension in U attribute	
	unsigned __int32 mTipThicknessTextureSamplingUDimension;
		
	/// The tip thickness texture sampling dimension in V attribute	
	unsigned __int32 mTipThicknessTextureSamplingVDimension;
		
	/// The displacement texture sampling dimension in U atribute	
	unsigned __int32 mDisplacementTextureSamplingUDimension;
		
	/// The displacement texture sampling dimension in V atribute	
	unsigned __int32 mDisplacementTextureSamplingVDimension;
		
	/* MAYA COLOR PROPERTIES */                                                                                                 	
		
	/// The root opacity texture sampling dimension in U atribute	
	unsigned __int32 mRootOpacityTextureSamplingUDimension;
		
	/// The root opacity texture sampling dimension in V atribute	
	unsigned __int32 mRootOpacityTextureSamplingVDimension;
		
	/// The tip opacity texture sampling dimension in U atribute	
	unsigned __int32 mTipOpacityTextureSamplingUDimension;
		
	/// The tip opacity texture sampling dimension in V atribute	
	unsigned __int32 mTipOpacityTextureSamplingVDimension;
		
	/// The root color texture sampling dimension in U atribute	
	unsigned __int32 mRootColorTextureSamplingUDimension;
		
	/// The root color texture sampling dimension in V atribute	
	unsigned __int32 mRootColorTextureSamplingVDimension;
		
	/// The tip color texture sampling dimension in U atribute	
	unsigned __int32 mTipColorTextureSamplingUDimension;
		
	/// The tip color texture sampling dimension in V atribute	
	unsigned __int32 mTipColorTextureSamplingVDimension;
		
	/// The hue variation texture sampling dimension in U atribute	
	unsigned __int32 mHueVariationTextureSamplingUDimension;
		
	/// The hue variation texture sampling dimension in V atribute	
	unsigned __int32 mHueVariationTextureSamplingVDimension;
		
	/// The value variation texture sampling dimension in U atribute	
	unsigned __int32 mValueVariationTextureSamplingUDimension;
		
	/// The value variation texture sampling dimension in V atribute	
	unsigned __int32 mValueVariationTextureSamplingVDimension;
		
	/// The mutant hair color texture sampling dimension in U atribute	
	unsigned __int32 mMutantHairColorTextureSamplingUDimension;
		
	/// The mutant hair color texture sampling dimension in V atribute	
	unsigned __int32 mMutantHairColorTextureSamplingVDimension;
		
	/// The percent mutant hair texture sampling dimension in U atribute	
	unsigned __int32 mPercentMutantHairTextureSamplingUDimension;
		
	/// The percent mutant hair texture sampling dimension in V atribute	
	unsigned __int32 mPercentMutantHairTextureSamplingVDimension;
		
	/* MAYA FRIZZ PROPERTIES */	
		
	/// The root frizz texture sampling dimension in U atribute	
	unsigned __int32 mRootFrizzTextureSamplingUDimension;
		
	/// The root frizz texture sampling dimension in V atribute	
	unsigned __int32 mRootFrizzTextureSamplingVDimension;
		
	/// The tip frizz texture sampling dimension in U atribute	
	unsigned __int32 mTipFrizzTextureSamplingUDimension;
		
	/// The tip frizz texture sampling dimension in V atribute	
	unsigned __int32 mTipFrizzTextureSamplingVDimension;
		
	/// The frizz x coordinate frequency texture sampling dimension in U atribute	
	unsigned __int32 mFrizzXFrequencyTextureSamplingUDimension;
		
	/// The frizz x coordinate frequency texture sampling dimension in V atribute	
	unsigned __int32 mFrizzXFrequencyTextureSamplingVDimension;
		
	/// The frizz y coordinate frequency texture sampling dimension in U atribute	
	unsigned __int32 mFrizzYFrequencyTextureSamplingUDimension;
		
	/// The frizz y coordinate frequency texture sampling dimension in V atribute	
	unsigned __int32 mFrizzYFrequencyTextureSamplingVDimension;
		
	/// The frizz z coordinate frequency texture sampling dimension in U atribute	
	unsigned __int32 mFrizzZFrequencyTextureSamplingUDimension;
		
	/// The frizz z coordinate frequency texture sampling dimension in V atribute	
	unsigned __int32 mFrizzZFrequencyTextureSamplingVDimension;
		
	/// The frizz animation texture sampling dimension in U atribute	
	unsigned __int32 mFrizzAnimTextureSamplingUDimension;
		
	/// The frizz animation texture sampling dimension in V atribute	
	unsigned __int32 mFrizzAnimTextureSamplingVDimension;
		
	/// The frizz animation speed texture sampling dimension in U atribute	
	unsigned __int32 mFrizzAnimSpeedTextureSamplingUDimension;
		
	/// The frizz animation speed texture sampling dimension in V atribute	
	unsigned __int32 mFrizzAnimSpeedTextureSamplingVDimension;
		
	/* MAYA KINK PROPERTIES */	
		
	/// The root kink texture sampling dimension in U atribute	
	unsigned __int32 mRootKinkTextureSamplingUDimension;
		
	/// The root kink texture sampling dimension in V atribute	
	unsigned __int32 mRootKinkTextureSamplingVDimension;
		
	/// The tip kink texture sampling dimension in U atribute	
	unsigned __int32 mTipKinkTextureSamplingUDimension;
		
	/// The tip kink texture sampling dimension in V atribute	
	unsigned __int32 mTipKinkTextureSamplingVDimension;
		
	/// The kink x coordinate frequency texture sampling dimension in U atribute	
	unsigned __int32 mKinkXFrequencyTextureSamplingUDimension;
		
	/// The kink x coordinate frequency texture sampling dimension in V atribute	
	unsigned __int32 mKinkXFrequencyTextureSamplingVDimension;
		
	/// The kink y coordinate frequency texture sampling dimension in U atribute	
	unsigned __int32 mKinkYFrequencyTextureSamplingUDimension;
		
	/// The kink y coordinate frequency texture sampling dimension in V atribute	
	unsigned __int32 mKinkYFrequencyTextureSamplingVDimension;
		
	/// The kink z coordinate frequency texture sampling dimension in U atribute	
	unsigned __int32 mKinkZFrequencyTextureSamplingUDimension;
		
	/// The kink z coordinate frequency texture sampling dimension in V atribute	
	unsigned __int32 mKinkZFrequencyTextureSamplingVDimension;
		
	/* MAYA MULTI STRAND PROPERTIES */	
		
	/// The root splay texture sampling dimension in U atribute	
	unsigned __int32 mRootSplayTextureSamplingUDimension;
		
	/// The root splay texture sampling dimension in V atribute	
	unsigned __int32 mRootSplayTextureSamplingVDimension;
		
	/// The tip splay texture sampling dimension in U atribute	
	unsigned __int32 mTipSplayTextureSamplingUDimension;
		
	/// The tip splay texture sampling dimension in V atribute	
	unsigned __int32 mTipSplayTextureSamplingVDimension;
		
	/// The center splay texture sampling dimension in U atribute	
	unsigned __int32 mCenterSplayTextureSamplingUDimension;
		
	/// The center splay texture sampling dimension in V atribute	
	unsigned __int32 mCenterSplayTextureSamplingVDimension;
		
	/// The twist texture sampling dimension in U atribute	
	unsigned __int32 mTwistTextureSamplingUDimension;
		
	/// The twist texture sampling dimension in V atribute	
	unsigned __int32 mTwistTextureSamplingVDimension;
		
	/// The offset texture sampling dimension in U atribute	
	unsigned __int32 mOffsetTextureSamplingUDimension;
		
	/// The offset texture sampling dimension in V atribute	
	unsigned __int32 mOffsetTextureSamplingVDimension;
		
	/// The aspect texture sampling dimension in U atribute	
	unsigned __int32 mAspectTextureSamplingUDimension;
		
	/// The aspect texture sampling dimension in V atribute	
	unsigned __int32 mAspectTextureSamplingVDimension;
		
	/// The randomizeStrand texture sampling dimension in U atribute	
	unsigned __int32 mRandomizeStrandTextureSamplingUDimension;
		
	/// The randomizeStrand texture sampling dimension in V atribute	
	unsigned __int32 mRandomizeStrandTextureSamplingVDimension;

	MObject mSegmentsCountAttr;   ///< The segments count for every interpolation group Maya attribute

	MObject mInterpolationGroupsSelectableAttr; ///< The which interpolation groups are selectable ? Maya attribute 

	Real mScaleFactor;  ///< The scale factor for all size dependent attributes
};

// inline functions implementation

inline void MayaHairProperties::refreshPointersToGuides( const HairComponents::GuidesSegments * aGuidesSegments,
	const HairComponents::RestPositionsDS * aGuidesRestPositionsDS )
{
	mGuidesSegments = aGuidesSegments;
	mGuidesRestPositionsDS = aGuidesRestPositionsDS;
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

} // namespace Maya

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MAYA_HAIR_PROPERTIES_HPP