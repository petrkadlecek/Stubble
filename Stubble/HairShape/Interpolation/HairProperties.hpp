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

	///-------------------------------------------------------------------------------------------------
	/// Gets the current time. 
	///
	/// \return	The current time. 
	///-------------------------------------------------------------------------------------------------
	inline Time getCurrentTime() const;

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

	///-------------------------------------------------------------------------------------------------
	/// Query if normals should be calculated. 
	///
	/// \return	true if normals should be  calculated, false if not. 
	///-------------------------------------------------------------------------------------------------
	inline bool areNormalsCalculated() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the cut texture. 
	///
	/// \return	The cut texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getCutTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the scale texture. 
	///
	/// \return	The scale texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getScaleTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the scale. 
	///
	/// \return	The scale. 
	///-------------------------------------------------------------------------------------------------
	inline Real getScale() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the rand scale texture. 
	///
	/// \return	The rand scale texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getRandScaleTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the rand scale. 
	///
	/// \return	The rand scale. 
	///-------------------------------------------------------------------------------------------------
	inline Real getRandScale() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the root thickness texture. 
	///
	/// \return	The root thickness texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getRootThicknessTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the root thickness. 
	///
	/// \return	The root thickness. 
	///-------------------------------------------------------------------------------------------------
	inline Real getRootThickness() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the tip thickness texture. 
	///
	/// \return	The tip thickness texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getTipThicknessTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the tip thickness. 
	///
	/// \return	The tip thickness. 
	///-------------------------------------------------------------------------------------------------
	inline Real getTipThickness() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the displacement texture. 
	///
	/// \return	The displacement texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getDisplacementTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the displacement. 
	///
	/// \return	The displacement. 
	///-------------------------------------------------------------------------------------------------
	inline Real getDisplacement() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the skip threshold, which controls minimum angle between tangents for skipping hair point.
	///
	/// \return	The skip threshold. 
	///-------------------------------------------------------------------------------------------------
	inline Real getSkipThreshold() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the root opacity texture. 
	///
	/// \return	The root opacity texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getRootOpacityTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the root opacity. 
	///
	/// \return	The root opacity. 
	///-------------------------------------------------------------------------------------------------
	inline Real getRootOpacity() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the tip opacity texture. 
	///
	/// \return	The tip opacity texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getTipOpacityTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the tip opacity. 
	///
	/// \return	The tip opacity. 
	///-------------------------------------------------------------------------------------------------
	inline Real getTipOpacity() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the root color texture. 
	///
	/// \return	The root color texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getRootColorTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the root color. 
	///
	/// \return	The root color. 
	///-------------------------------------------------------------------------------------------------
	inline const Real3 & getRootColor() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the tip color texture. 
	///
	/// \return	The tip color texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getTipColorTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the tip color. 
	///
	/// \return	The tip color. 
	///-------------------------------------------------------------------------------------------------
	inline const Real3 & getTipColor() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the hue variation texture. 
	///
	/// \return	The hue variation texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getHueVariationTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the hue variation. 
	///
	/// \return	The hue variation. 
	///-------------------------------------------------------------------------------------------------
	inline Real getHueVariation() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the value variation texture. 
	///
	/// \return	The value variation texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getValueVariationTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the value variation. 
	///
	/// \return	The value variation. 
	///-------------------------------------------------------------------------------------------------
	inline Real getValueVariation() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the mutant hair color texture. 
	///
	/// \return	The mutant hair color texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getMutantHairColorTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the mutant hair color. 
	///
	/// \return	The mutant hair color. 
	///-------------------------------------------------------------------------------------------------
	inline const Real3 & getMutantHairColor() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the percent mutant hair texture. 
	///
	/// \return	The percent mutant hair texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getPercentMutantHairTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the percent mutant hair. 
	///
	/// \return	The percent mutant hair. 
	///-------------------------------------------------------------------------------------------------
	inline Real getPercentMutantHair() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the root frizz texture. 
	///
	/// \return	The root frizz texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getRootFrizzTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the root frizz. 
	///
	/// \return	The root frizz. 
	///-------------------------------------------------------------------------------------------------
	inline Real getRootFrizz() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the tip frizz texture. 
	///
	/// \return	The tip frizz texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getTipFrizzTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the tip frizz. 
	///
	/// \return	The tip frizz. 
	///-------------------------------------------------------------------------------------------------
	inline Real getTipFrizz() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the frizz x coordinate frequency texture. 
	///
	/// \return	The frizz x coordinate frequency texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getFrizzXFrequencyTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the frizz x coordinate frequency. 
	///
	/// \return	The frizz x coordinate frequency. 
	///-------------------------------------------------------------------------------------------------
	inline Real getFrizzXFrequency() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the frizz y coordinate frequency texture. 
	///
	/// \return	The frizz y coordinate frequency texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getFrizzYFrequencyTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the frizz y coordinate frequency. 
	///
	/// \return	The frizz y coordinate frequency. 
	///-------------------------------------------------------------------------------------------------
	inline Real getFrizzYFrequency() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the frizz z coordinate frequency texture. 
	///
	/// \return	The frizz z coordinate frequency texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getFrizzZFrequencyTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the frizz z coordinate frequency. 
	///
	/// \return	The frizz z coordinate frequency. 
	///-------------------------------------------------------------------------------------------------
	inline Real getFrizzZFrequency() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the frizz animation texture. 
	///
	/// \return	The frizz animation texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getFrizzAnimTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the frizz animation. 
	///
	/// \return	The frizz animation. 
	///-------------------------------------------------------------------------------------------------
	inline Real getFrizzAnim() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the frizz animation speed texture. 
	///
	/// \return	The frizz animation speed texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getFrizzAnimSpeedTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the frizz animation speed. 
	///
	/// \return	The frizz animation speed. 
	///-------------------------------------------------------------------------------------------------
	inline Real getFrizzAnimSpeed() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the frizz animation direction. 
	///
	/// \return	The frizz animation direction. 
	///-------------------------------------------------------------------------------------------------
	inline const Vector3D< Real > & getFrizzAnimDirection() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the root kink texture. 
	///
	/// \return	The root kink texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getRootKinkTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the root kink. 
	///
	/// \return	The root kink. 
	///-------------------------------------------------------------------------------------------------
	inline Real getRootKink() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the tip kink texture. 
	///
	/// \return	The tip kink texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getTipKinkTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the tip kink. 
	///
	/// \return	The tip kink. 
	///-------------------------------------------------------------------------------------------------
	inline Real getTipKink() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the kink x coordinate frequency texture. 
	///
	/// \return	The kink x coordinate frequency texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getKinkXFrequencyTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the kink x coordinate frequency. 
	///
	/// \return	The kink x coordinate frequency. 
	///-------------------------------------------------------------------------------------------------
	inline Real getKinkXFrequency() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the kink y coordinate frequency texture. 
	///
	/// \return	The kink y coordinate frequency texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getKinkYFrequencyTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the kink y coordinate frequency. 
	///
	/// \return	The kink y coordinate frequency. 
	///-------------------------------------------------------------------------------------------------
	inline Real getKinkYFrequency() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the kink z coordinate frequency texture. 
	///
	/// \return	The kink z coordinate frequency texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getKinkZFrequencyTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the kink z coordinate frequency. 
	///
	/// \return	The kink z coordinate frequency. 
	///-------------------------------------------------------------------------------------------------
	inline Real getKinkZFrequency() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the hair count in multi strand. 
	///
	/// \return	The hair count in multi strand. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 getMultiStrandCount() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the root splay texture. 
	///
	/// \return	The root splay texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getRootSplayTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the root splay. 
	///
	/// \return	The root splay. 
	///-------------------------------------------------------------------------------------------------
	inline Real getRootSplay() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the tip splay texture. 
	///
	/// \return	The tip splay texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getTipSplayTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the tip splay. 
	///
	/// \return	The tip splay. 
	///-------------------------------------------------------------------------------------------------
	inline Real getTipSplay() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the center splay texture. 
	///
	/// \return	The center splay texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getCenterSplayTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the center splay. 
	///
	/// \return	The center splay. 
	///-------------------------------------------------------------------------------------------------
	inline Real getCenterSplay() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the twist texture. 
	///
	/// \return	The twist texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getTwistTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the twist.
	///
	/// \return	The twist. 
	///-------------------------------------------------------------------------------------------------
	inline Real getTwist() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the offset texture. 
	///
	/// \return	The offset texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getOffsetTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the offset. 
	///
	/// \return	The offset. 
	///-------------------------------------------------------------------------------------------------
	inline Real getOffset() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the aspect texture.
	///
	/// \return	The aspect texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getAspectTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the aspect. 
	///
	/// \return	The aspect. 
	///-------------------------------------------------------------------------------------------------
	inline Real getAspect() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the randomize strand texture. 
	///
	/// \return	The randomize strand texture. 
	///-------------------------------------------------------------------------------------------------
	inline const Texture & getRandomizeStrandTexture() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the randomize strand. 
	///
	/// \return	The randomize strand. 
	///-------------------------------------------------------------------------------------------------
	inline Real getRandomizeStrand() const;

protected:
	
	///-------------------------------------------------------------------------------------------------
	/// Default constructor. Properties will be set by class deriving from HairProperties.
	///-------------------------------------------------------------------------------------------------
	HairProperties();

	static const unsigned __int32 DEFAULT_SEGMENTS_COUNT = 5;

	/* HERE WILL BE STORED ALL HAIR PROPERTIES */

	Time mCurrentTime;  ///< The current time

	Texture * mDensityTexture;  ///< The density texture

	Texture * mInterpolationGroupsTexture;  ///< The interpolation groups texture

	InterpolationGroups * mInterpolationGroups;  ///< Interpolation groups object

	unsigned __int32 mNumberOfGuidesToInterpolateFrom;  ///< Number of guides to interpolate from

	const HairComponents::GuidesSegments * mGuidesSegments;   ///< The guides segments

	const HairComponents::RestPositionsUG * mGuidesRestPositionsUG;   ///< The guides rest positions uniform grid

	bool mAreNormalsCalculated; ///< true if normals should be calculated

	Texture * mCutTexture;  ///< The cut texture

	Texture * mScaleTexture;	///< The scale texture

	Real mScale;	///< The scale

	Texture * mRandScaleTexture;	///< The rand scale texture

	Real mRandScale;	///< The rand scale

	Texture * mRootThicknessTexture;	///< The root thickness texture

	Real mRootThickness;	///< The root thickness

	Texture * mTipThicknessTexture; ///< The tip thickness texture

	Real mTipThickness; ///< The tip thickness

	Texture * mDisplacementTexture; ///< The displacement texture

	Real mDisplacement; ///< The displacement

	Real mSkipThreshold; ///< The skip threshold [-1,1 ]

	Texture * mRootOpacityTexture;  ///< The root opacity texture

	Real mRootOpacity;  ///< The root opacity

	Texture * mTipOpacityTexture;   ///< The tip opacity texture

	Real mTipOpacity;   ///< The tip opacity

	Texture * mRootColorTexture;	///< The root color texture

	Real3 mRootColor;   ///< The root color

	Texture * mTipColorTexture; ///< The tip color texture

	Real3 mTipColor;	///< The tip color

	Texture * mHueVariationTexture; ///< The hue variation texture

	Real mHueVariation; ///< The hue variation

	Texture * mValueVariationTexture;   ///< The value variation texture

	Real mValueVariation;   ///< The value variation

	Texture * mMutantHairColorTexture;  ///< The mutant hair color texture

	Real3 mMutantHairColor; ///< The mutant hair color

	Texture * mPercentMutantHairTexture;	///< The percent mutant hair texture

	Real mPercentMutantHair;	///< The percent mutant hair

	Texture * mRootFrizzTexture;	///< The root frizz texture

	Real mRootFrizz;	///< The root frizz

	Texture * mTipFrizzTexture; ///< The tip frizz texture

	Real mTipFrizz; ///< The tip frizz

	Texture * mFrizzXFrequencyTexture;  ///< The frizz x coordinate frequency texture

	Real mFrizzXFrequency;  ///< The frizz x coordinate frequency

	Texture * mFrizzYFrequencyTexture;  ///< The frizz y coordinate frequency texture

	Real mFrizzYFrequency;  ///< The frizz y coordinate frequency

	Texture * mFrizzZFrequencyTexture;  ///< The frizz z coordinate frequency texture

	Real mFrizzZFrequency;  ///< The frizz z coordinate frequency

	Texture * mFrizzAnimTexture;	///< The frizz animation texture

	Real mFrizzAnim;	///< The frizz animation

	Texture * mFrizzAnimSpeedTexture;   ///< The frizz animation speed texture

	Real mFrizzAnimSpeed;   ///< The frizz animation speed

	Vector3D< Real > mFrizzAnimDirection;   ///< The frizz animation direction

	Texture * mRootKinkTexture; ///< The root kink texture

	Real mRootKink; ///< The root kink

	Texture * mTipKinkTexture;  ///< The tip kink texture

	Real mTipKink;  ///< The tip kink

	Texture * mKinkXFrequencyTexture;   ///< The kink x coordinate frequency texture

	Real mKinkXFrequency;   ///< The kink x coordinate frequency

	Texture * mKinkYFrequencyTexture;   ///< The kink y coordinate frequency texture

	Real mKinkYFrequency;   ///< The kink y coordinate frequency

	Texture * mKinkZFrequencyTexture;   ///< The kink z coordinate frequency texture

	Real mKinkZFrequency;   ///< The kink z coordinate frequency

	unsigned __int32 mMultiStrandCount; ///< Number of hair in one strand

	Texture * mRootSplayTexture;	///< The root splay texture

	Real mRootSplay;	///< The root splay

	Texture * mTipSplayTexture; ///< The tip splay texture

	Real mTipSplay; ///< The tip splay

	Texture * mCenterSplayTexture;  ///< The center splay texture

	Real mCenterSplay;  ///< The center splay

	Texture * mTwistTexture;	///< The twist texture

	Real mTwist;	///< The twist

	Texture * mOffsetTexture;   ///< The offset texture

	Real mOffset;   ///< The offset

	Texture * mAspectTexture;   ///< The aspect texture

	Real mAspect;   ///< The aspect

	Texture * mRandomizeStrandTexture;  ///< The randomize strand texture

	Real mRandomizeStrand;  ///< The randomize strand
};

// inline functions implementation

inline Time HairProperties::getCurrentTime() const
{
	return mCurrentTime;
}

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

inline bool HairProperties::areNormalsCalculated() const
{
	return mAreNormalsCalculated;
}

inline const Texture & HairProperties::getCutTexture() const
{
	return *mCutTexture;
}

inline const Texture & HairProperties::getScaleTexture() const
{
return *mScaleTexture;
}

inline Real HairProperties::getScale() const
{
return mScale;
}

inline const Texture & HairProperties::getRandScaleTexture() const
{
return *mRandScaleTexture;
}

inline Real HairProperties::getRandScale() const
{
return mRandScale;
}

inline const Texture & HairProperties::getRootThicknessTexture() const
{
return *mRootThicknessTexture;
}

inline Real HairProperties::getRootThickness() const
{
return mRootThickness;
}

inline const Texture & HairProperties::getTipThicknessTexture() const
{
return *mTipThicknessTexture;
}

inline Real HairProperties::getTipThickness() const
{
return mTipThickness; 
}

inline const Texture & HairProperties::getDisplacementTexture() const
{
return *mDisplacementTexture;
}

inline Real HairProperties::getDisplacement() const
{
	return mDisplacement;
}

inline Real HairProperties::getSkipThreshold() const
{
	return mSkipThreshold;
}

inline const Texture & HairProperties::getRootOpacityTexture() const
{
	return *mRootOpacityTexture;
}

inline Real HairProperties::getRootOpacity() const
{
	return mRootOpacity;
}

inline const Texture & HairProperties::getTipOpacityTexture() const
{
	return *mTipOpacityTexture;
}

inline Real HairProperties::getTipOpacity() const
{
	return mTipOpacity;
}

inline const Texture & HairProperties::getRootColorTexture() const
{
	return *mRootColorTexture;
}

inline const Real3 & HairProperties::getRootColor() const
{
	return mRootColor;
}

inline const Texture & HairProperties::getTipColorTexture() const
{
	return *mTipColorTexture;
}

inline const Real3 & HairProperties::getTipColor() const
{
	return mTipColor;
}

inline const Texture & HairProperties::getHueVariationTexture() const
{
	return *mHueVariationTexture;
}

inline Real HairProperties::getHueVariation() const
{
	return mHueVariation;
}

inline const Texture & HairProperties::getValueVariationTexture() const
{
	return *mValueVariationTexture;
}

inline Real HairProperties::getValueVariation() const
{
	return mValueVariation;
}

inline const Texture & HairProperties::getMutantHairColorTexture() const
{
	return *mMutantHairColorTexture;
}

inline const Real3 & HairProperties::getMutantHairColor() const
{
	return mMutantHairColor;
}

inline const Texture & HairProperties::getPercentMutantHairTexture() const
{
	return *mPercentMutantHairTexture;
}

inline Real HairProperties::getPercentMutantHair() const
{
	return mPercentMutantHair;
}

inline const Texture & HairProperties::getRootFrizzTexture() const
{
	return *mRootFrizzTexture;
}

inline Real HairProperties::getRootFrizz() const
{
	return mRootFrizz;
}

inline const Texture & HairProperties::getTipFrizzTexture() const
{
	return *mTipFrizzTexture;
}

inline Real HairProperties::getTipFrizz() const
{
	return mTipFrizz;
}

inline const Texture & HairProperties::getFrizzXFrequencyTexture() const
{
	return *mFrizzXFrequencyTexture;
}

inline Real HairProperties::getFrizzXFrequency() const
{
	return mFrizzXFrequency;
}

inline const Texture & HairProperties::getFrizzYFrequencyTexture() const
{
	return *mFrizzYFrequencyTexture;
}

inline Real HairProperties::getFrizzYFrequency() const
{
	return mFrizzYFrequency;
}

inline const Texture & HairProperties::getFrizzZFrequencyTexture() const
{
	return *mFrizzZFrequencyTexture;
}

inline Real HairProperties::getFrizzZFrequency() const
{
	return mFrizzZFrequency;
}


inline const Texture & HairProperties::getFrizzAnimTexture() const
{
	return *mFrizzAnimTexture;
}

inline Real HairProperties::getFrizzAnim() const
{
	return mFrizzAnim;
}

inline const Texture & HairProperties::getFrizzAnimSpeedTexture() const
{
	return *mFrizzAnimSpeedTexture;
}

inline Real HairProperties::getFrizzAnimSpeed() const
{
	return mFrizzAnimSpeed;
}

inline const Vector3D< Real > & HairProperties::getFrizzAnimDirection() const
{
	return mFrizzAnimDirection;
}

inline const Texture & HairProperties::getRootKinkTexture() const
{
	return *mRootKinkTexture;
}

inline Real HairProperties::getRootKink() const
{
	return mRootKink;
}

inline const Texture & HairProperties::getTipKinkTexture() const
{
	return *mTipKinkTexture;
}

inline Real HairProperties::getTipKink() const
{
	return mTipKink;
}

inline const Texture & HairProperties::getKinkXFrequencyTexture() const
{
	return *mKinkXFrequencyTexture;
}

inline Real HairProperties::getKinkXFrequency() const
{
	return mKinkXFrequency;
}

inline const Texture & HairProperties::getKinkYFrequencyTexture() const
{
	return *mKinkYFrequencyTexture;
}

inline Real HairProperties::getKinkYFrequency() const
{
	return mKinkYFrequency;
}

inline const Texture & HairProperties::getKinkZFrequencyTexture() const
{
	return *mKinkZFrequencyTexture;
}

inline Real HairProperties::getKinkZFrequency() const
{
	return mKinkZFrequency;
}

inline unsigned __int32 HairProperties::getMultiStrandCount() const
{
	return mMultiStrandCount;
}

inline const Texture & HairProperties::getRootSplayTexture() const
{
	return *mRootSplayTexture;
}

inline Real HairProperties::getRootSplay() const
{
	return mRootSplay;
}

inline const Texture & HairProperties::getTipSplayTexture() const
{
	return *mTipSplayTexture;
}

inline Real HairProperties::getTipSplay() const
{
	return mTipSplay;
}

inline const Texture & HairProperties::getCenterSplayTexture() const
{
	return *mCenterSplayTexture;
}

inline Real HairProperties::getCenterSplay() const
{
	return mCenterSplay;
}

inline const Texture & HairProperties::getTwistTexture() const
{
	return *mTwistTexture;
}

inline Real HairProperties::getTwist() const
{
	return mTwist;
}

inline const Texture & HairProperties::getOffsetTexture() const
{
	return *mOffsetTexture;
}

inline Real HairProperties::getOffset() const
{
	return mOffset;
}

inline const Texture & HairProperties::getAspectTexture() const
{
	return *mAspectTexture;
}

inline Real HairProperties::getAspect() const
{
	return mAspect;
}

inline const Texture & HairProperties::getRandomizeStrandTexture() const
{
	return *mRandomizeStrandTexture;
}

inline Real HairProperties::getRandomizeStrand() const
{
	return mRandomizeStrand;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_PROPERTIES_HPP