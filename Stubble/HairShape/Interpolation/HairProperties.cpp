#include "HairProperties.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

HairProperties::HairProperties():
	mCurrentTime( 0 ),
	mDensityTexture( 0 ),
	mInterpolationGroups( 0 ),
	mInterpolationGroupsTexture( 0 ),
	mNumberOfGuidesToInterpolateFrom( 3 ),
	mGuidesRestPositionsUG( 0 ),
	mGuidesSegments( 0 ),
	mAreNormalsCalculated( false ),
	mCutTexture( 0 ),
	mScaleTexture( 0 ),
	mScale( 1 ),
	mRandScaleTexture( 0 ),
	mRandScale( 0 ),
	mRootThicknessTexture( 0 ),
	mRootThickness( 0.1f ),
	mTipThicknessTexture( 0 ),
	mTipThickness( 0.1f ),
	mDisplacementTexture( 0 ),
	mDisplacement( 0 ),
	mRootOpacityTexture( 0 ),
	mRootOpacity( 1 ),
	mTipOpacityTexture( 0 ),
	mTipOpacity( 1 ),
	mRootColorTexture( 0 ),
	mTipColorTexture( 0 ),
	mHueVariationTexture( 0 ),
	mHueVariation( 0 ),
	mValueVariationTexture( 0 ),
	mValueVariation( 0 ),
	mMutantHairColorTexture( 0 ),
	mPercentMutantHairTexture( 0 ),
	mPercentMutantHair( 0 ),
	mRootFrizzTexture( 0 ),
	mRootFrizz( 1 ),
	mTipFrizzTexture( 0 ),
	mTipFrizz( 1 ),
	mFrizzXFrequencyTexture( 0 ),
	mFrizzXFrequency( 0.5f ),
	mFrizzYFrequencyTexture( 0 ),
	mFrizzYFrequency( 0.5f ),
	mFrizzZFrequencyTexture( 0 ),
	mFrizzZFrequency( 0.5f ),
	mFrizzAnimTexture( 0 ),
	mFrizzAnim( 0 ),
	mFrizzAnimSpeedTexture( 0 ),
	mFrizzAnimSpeed( 0 ),
	mFrizzAnimDirection( 1, 1, 1),
	mRootKinkTexture( 0 ),
	mRootKink( 1 ),
	mTipKinkTexture( 0 ),
	mTipKink( 1 ),
	mKinkXFrequencyTexture( 0 ),
	mKinkXFrequency( 0.5f ),
	mKinkYFrequencyTexture( 0 ),
	mKinkYFrequency( 0.5f ),
	mKinkZFrequencyTexture( 0 ),
	mKinkZFrequency( 0.5f ),
	mMultiStrandCount( 0 ),
	mRootSplayTexture( 0 ),
	mRootSplay( 0 ),
	mTipSplayTexture( 0 ),
	mTipSplay( 0 ),
	mCenterSplayTexture( 0 ),
	mCenterSplay( 0 ),
	mTwistTexture( 0 ),
	mTwist( 0 ),
	mOffsetTexture( 0 ),
	mOffset( 0 ),
	mAspectTexture( 0 ),
	mAspect( 0 ),
	mRandomizeStrandTexture( 0 ),
	mRandomizeStrand( 0 )
{
	 mRootColor[ 0 ] = mRootColor[ 1 ] = mRootColor[ 2 ] = 1;
	 mTipColor[ 0 ] = mTipColor[ 1 ] = mTipColor[ 2 ] = 1;
	 mMutantHairColor[ 0 ] = mMutantHairColor[ 1 ] = mMutantHairColor[ 2 ] = 1;
}

HairProperties::~HairProperties()
{
	delete mDensityTexture;

	delete mInterpolationGroups;

	delete mInterpolationGroupsTexture;

	delete mCutTexture;

	delete mScaleTexture;

	delete mRandScaleTexture;

	delete mRootThicknessTexture;

	delete mTipThicknessTexture;

	delete mDisplacementTexture;

	delete mRootOpacityTexture;

	delete mTipOpacityTexture;

	delete mRootColorTexture;

	delete mTipColorTexture;

	delete mHueVariationTexture;

	delete mValueVariationTexture;

	delete mMutantHairColorTexture;

	delete mPercentMutantHairTexture;

	delete mRootFrizzTexture;

	delete mTipFrizzTexture;

	delete mFrizzXFrequencyTexture;

	delete mFrizzYFrequencyTexture;

	delete mFrizzZFrequencyTexture;

	delete mFrizzAnimTexture;

	delete mFrizzAnimSpeedTexture;

	delete mTipKinkTexture;

	delete mKinkXFrequencyTexture;

	delete mKinkYFrequencyTexture;

	delete mKinkZFrequencyTexture;

	delete mRootSplayTexture;

	delete mTipSplayTexture;

	delete mCenterSplayTexture;

	delete mTwistTexture;

	delete mOffsetTexture;

	delete mAspectTexture;

	delete mRandomizeStrandTexture;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble