#ifndef STUBBLE_HAIR_GENERATOR_HPP
#define STUBBLE_HAIR_GENERATOR_HPP

#include "Primitives/BoundingBox.hpp"
#include "HairProperties.hpp"
#include "HairShape/Generators/RandomGenerator.hpp"
#include "OutputGenerator.hpp"
#include "PositionGenerator.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Class for generating interpolated hair. 
/// Class is able not only able to generate interpolated hair but also calculate interpolated hair
/// bounding box.
/// Template parameter tPositionGenerator must implement PositionGenerator interface.
/// Template parameter tOutputGenerator must implement OutputGenerator interface.
/// For optimalization purposes templating is used rather than virtual methods.
///-------------------------------------------------------------------------------------------------
template< typename tPositionGenerator, typename tOutputGenerator >
class HairGenerator
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Constructor. 
	///
	/// \param [in,out]	aPositionGenerator	The position generator. 
	/// \param [in,out]	aOutputGenerator	The output generator. 
	///-------------------------------------------------------------------------------------------------
	inline HairGenerator( tPositionGenerator & aPositionGenerator, tOutputGenerator & aOutputGenerator );

	///-------------------------------------------------------------------------------------------------
	/// Generates interpolated hair.
	/// Uses position generator to generate hair positions and output generator to output finished hair. 
	///
	/// \param	aHairProperties	The hair properties. 
	///-------------------------------------------------------------------------------------------------
	void generate( const HairProperties & aHairProperties );

	///-------------------------------------------------------------------------------------------------
	/// Calculates the bounding box of hair.
	/// Uses position generator to generate hair positions, output generator is not used.
	///
	/// \param	aHairProperties			The hair properties. 
	/// \param	aHairGenerateRatio		The hair generate ratio [0,1], defines how much of actual hair
	/// 								is generated for bounding box calculation.
	/// \param [in,out]	aBoundingBox	The bounding box. 
	///-------------------------------------------------------------------------------------------------
	void calculateBoundingBox( const HairProperties & aHairProperties, float aHairGenerateRatio,
		BoundingBox & aBoundingBox );

	///-------------------------------------------------------------------------------------------------
	/// Gets the bounding box of generated hair points. Only calculated if CALCULATE_BBOX is defined.
	/// This method is mainly used for debug purposes.
	///
	/// \return	The bounding box. 
	///-------------------------------------------------------------------------------------------------
	inline const BoundingBox & getBoundingBox() const;

private:

	/* For easier usage, we will create aliases for output types */

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type of the position .
	///-------------------------------------------------------------------------------------------------
	typedef typename tOutputGenerator::PositionType PositionType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type of the color .
	///-------------------------------------------------------------------------------------------------
	typedef typename tOutputGenerator::ColorType ColorType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type of the opacity .
	///-------------------------------------------------------------------------------------------------
	typedef typename tOutputGenerator::OpacityType OpacityType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type of the normal .
	///-------------------------------------------------------------------------------------------------
	typedef typename tOutputGenerator::NormalType NormalType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type of the width .
	///-------------------------------------------------------------------------------------------------
	typedef typename tOutputGenerator::WidthType WidthType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type of the uv coordinate .
	///-------------------------------------------------------------------------------------------------
	typedef typename tOutputGenerator::UVCoordinateType UVCoordinateType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing type of the index .
	///-------------------------------------------------------------------------------------------------
	typedef typename tOutputGenerator::IndexType IndexType;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing the point.
	///-------------------------------------------------------------------------------------------------
	typedef Vector3D<PositionType > Point;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing the vector.
	///-------------------------------------------------------------------------------------------------
	typedef Vector3D< NormalType > Vector;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing the matrix .
	///-------------------------------------------------------------------------------------------------
	typedef Matrix< PositionType > Matrix;

	///-------------------------------------------------------------------------------------------------
	/// Interpolate hair segments from N closest guides. 
	///
	/// \param [in,out]	aPoints			The hair points. 
	/// \param	aCount					Number of points. 
	/// \param	aRestPosition			The rest position of hair. 
	/// \param	aInterpolationGroupId	Identifier for a interpolation group. 
	///-------------------------------------------------------------------------------------------------
	inline void interpolateFromGuides( Point * aPoints, unsigned __int32 aCount, const MeshPoint &aRestPosition, 
		unsigned __int32 aInterpolationGroupId );

	///-------------------------------------------------------------------------------------------------
	/// Checks for hair degeneration. Deletes duplicate points and returns true if hair has degenerated
	/// to single point.
	///
	/// \param [in,out]	aPoints				The hair points. 
	/// \param [in,out] aCount				Number of points. 
	/// \param [in,out] aCurvePointsCount	Number of curve points, used for curve parameter t calculation. 
	/// 				
	/// \return true if hair has degenerated to single point.
	///-------------------------------------------------------------------------------------------------
	inline bool checkDegenerateHair( Point * aPoints, unsigned __int32 & aCount, 
		unsigned __int32 & aCurvePointsCount );

	///-------------------------------------------------------------------------------------------------
	/// Applies the scale to hair points. 
	///
	/// \param [in,out]	aPoints	The hair points. 
	/// \param	aCount			Number of hair points. 
	/// \param	aRestPosition	The rest position of hair. 
	///-------------------------------------------------------------------------------------------------
	inline void applyScale( Point * aPoints, unsigned __int32 aCount, const MeshPoint &aRestPosition );

	///-------------------------------------------------------------------------------------------------
	/// Applies the frizz to hair points. 
	///
	/// \param [in,out]	aPoints		The hair points. 
	/// \param	aCount				Number of points. 
	/// \param	aCurvePointsCount	Number of curve points, used for curve parameter t calculation. 
	/// \param	aRestPosition		The rest position of hair. 
	///-------------------------------------------------------------------------------------------------
	inline void applyFrizz( Point * aPoints, unsigned __int32 aCount, unsigned __int32 aCurvePointsCount, 
		const MeshPoint &aRestPosition );

	///-------------------------------------------------------------------------------------------------
	/// Applies the kink to hair points. 
	///
	/// \param [in,out]	aPoints		The hair points. 
	/// \param	aCount				Number of points. 
	/// \param	aCurvePointsCount	Number of curve points, used for curve parameter t calculation. 
	/// \param	aRestPosition		The rest position of hair. 
	///-------------------------------------------------------------------------------------------------
	inline void applyKink( Point * aPoints, unsigned __int32 aCount, unsigned __int32 aCurvePointsCount, 
		const MeshPoint &aRestPosition );

	///-------------------------------------------------------------------------------------------------
	/// Transforms points by requested transform matrix. 
	///
	/// \param [in,out]	aPoints		The hair points. 
	/// \param	aCount				Number of points. 
	/// \param	aTransformMatrix	The transform matrix. 
	///-------------------------------------------------------------------------------------------------
	inline void transformPoints( Point * aPoints, unsigned __int32 aCount, const Matrix & aTransformMatrix );

	///-------------------------------------------------------------------------------------------------
	/// Calculates tangents in requested hair points. 
	///
	/// \param [in,out]	aTangents	The calculated tangents. 
	/// \param	aPoints				The hair points. 
	/// \param	aCount				Number of hair points. 
	///-------------------------------------------------------------------------------------------------
	inline void calculateTangents( Vector * aTangents, const Point * aPoints, unsigned __int32 aCount );

	///-------------------------------------------------------------------------------------------------
	/// Calculates the normal of selected hair point. aPoints (aTangents) must be part of an array,
	/// in which previous/next hair point (tangent) is/will be stored.
	///
	/// \param	aPoints			Pointer to selected hair point.
	/// \param	aTangents		Pointer to selected hair point tangent.
	/// \param	aPreviousNormal	The previous point normal. 
	///
	/// \return calculated normal.
	///-------------------------------------------------------------------------------------------------
	inline Vector calculateNormal( const Point * aPoints, const Vector * aTangents,
		const Vector & aPreviousNormal );

	///-------------------------------------------------------------------------------------------------
	/// Calculates normals and binormals in selected hair points.
	///
	/// \param [in,out]	aNormals	The calculated normals. 
	/// \param [in,out]	aBinormals	The calculated binormals. 
	/// \param	aPoints				The hair points. 
	/// \param	aTangents			The hair tangents. 
	/// \param	aCount				Number of hair points. 
	///-------------------------------------------------------------------------------------------------
	inline void calculateNormalsAndBinormals( Vector * aNormals, Vector * aBinormals, const Point * aPoints, 
		const Vector * aTangents, unsigned __int32 aCount );

	///-------------------------------------------------------------------------------------------------
	/// Applies the hue value shift described to selected color in RGB. 
	/// The value is converted to HSV and never back.
	/// Color will be transformed back to RGB in generateHair method.
	///
	/// \param [in,out]	mColor	Selected color in RGB, will be converted to HSV and shifted. 
	/// \param	aValueShift		The value shift of color.
	/// \param	aHueShift		The hue shift of color.
	///-------------------------------------------------------------------------------------------------
	inline void applyHueValueShift( ColorType * mColor, ColorType aValueShift, ColorType aHueShift );

	///-------------------------------------------------------------------------------------------------
	/// Select hair color, opacity and width. Result is stored in HairGenerator object variables.
	///
	/// \param	aRestPosition	The rest position of hair. 
	///-------------------------------------------------------------------------------------------------
	inline void selectHairColorOpacityWidth( const MeshPoint & aRestPosition );

	///-------------------------------------------------------------------------------------------------
	/// Only calls the same number of random values generation as selectHairColorOpacityWidth. This 
	/// method is used during bounding box calculation.
	///-------------------------------------------------------------------------------------------------
	inline void fakeSelectHairColorOpacityWidth();

	///-------------------------------------------------------------------------------------------------
	/// Skips point if it is not necessary to output it ( it can be interpolated in renderer from
	/// two neighbour points ). aPoints (aTangents) must be part of an array, in which previous/next 
	/// hair point (tangent) is/will be stored.
	///
	/// \param	aPoints		The selected hair point. 
	/// \param	aTangents	The selected hair tangent. 
	///
	/// \return	true if point should be skipped
	///-------------------------------------------------------------------------------------------------
	inline bool skipPoint( const Point * aPoints, const Vector * aTangents );

	///-------------------------------------------------------------------------------------------------
	/// Generates final hair points positions, normals, colors, opacities, widths.
	/// Uses output generator pointer to output all hair properties.
	/// Method beginHair of output generator must precede calling of this method.
	/// This method expects all colors to be in HSV and converts them back to RGB before outputing.
	///
	/// \param [in,out] aPoints		Hair points ( may be modified if cut is applied ). 
	/// \param [in,out] aTangents	Hair tangents ( may be modified if cut is applied ).
	/// \param	aCount				Number of points. 
	/// \param	aCurvePointsCount	Number of curve points, used for curve parameter t calculation. 
	/// \param	aRestPosition		The rest position of hair. 
	/// \param  aCutFactor			Parametric representation of hair cut.
	/// 
	/// \return final number of hair points.
	///-------------------------------------------------------------------------------------------------
	inline unsigned __int32 generateHair( Point * aPoints, Vector * aTangents, unsigned __int32 aCount, 
		unsigned __int32 aCurvePointsCount, const MeshPoint &aRestPosition, PositionType aCutFactor );

	///-------------------------------------------------------------------------------------------------
	/// Output hair index and u v coordinates. 
	///
	/// \param	aHairIndex		Zero-based index of a hair. 
	/// \param	aStrandIndex	Zero-based index of a strand. 
	/// \param	aRestPosition	The hair rest position. 
	///-------------------------------------------------------------------------------------------------
	inline void outputHairIndexAndUVs( IndexType aHairIndex, IndexType aStrandIndex, const MeshPoint &aRestPosition );

	///-------------------------------------------------------------------------------------------------
	/// Samples disk. Uses two samples from [-1,1]^2 to sample disk and returns phi angle and radius.
	///
	/// \param	aSampleX		The sample x coordinate. 
	/// \param	aSampleY		The sample y coordinate. 
	/// \param [in,out]	aCosPhi	The cosine phi. 
	/// \param [in,out]	aSinPhi	The sine phi. 
	/// \param [in,out]	aRadius	The sample radius.
	///-------------------------------------------------------------------------------------------------
	inline void sampleDisk( PositionType aSampleX, PositionType aSampleY, PositionType & aCosPhi, 
		PositionType & aSinPhi, PositionType & aRadius );

	///-------------------------------------------------------------------------------------------------
	/// Selects multi strand properties. 
	///
	/// \param	aRestPosition		The hair rest position.
	/// \param	aCurvePointsCount	Number of curve points, used for twist calculation.  
	///-------------------------------------------------------------------------------------------------
	inline void selectMultiStrandProperties( const MeshPoint &aRestPosition, unsigned __int32 aCurvePointsCount );

	///-------------------------------------------------------------------------------------------------
	/// Generates a hair positions in strand (main hair) local space. 
	///
	/// \param [in,out]	aPoints		The new hair points. 
	/// \param	aCount				Number of points. 
	/// \param	aCurvePointsCount	Number of curve points, used for curve parameter t calculation. 
	/// \param	aMainHairPoints		The main hair points. 
	/// \param	aMainHairNormals	The main hair normals. 
	/// \param	aMainHairBinormals	The main hair binormals. 
	///-------------------------------------------------------------------------------------------------
	inline void generateHairInStrand( Point * aPoints, unsigned __int32 aCount, unsigned __int32 aCurvePointsCount,
		const Point * aMainHairPoints, const Vector * aMainHairNormals, const Vector * aMainHairBinormals );

	///-------------------------------------------------------------------------------------------------
	/// Calculates the bounding box of single hair.
	///
	/// \param [in,out] aPoints			Hair points ( may be modified if cut is applied ). 
	/// \param [in,out] aTangents		Hair tangents ( may be modified if cut is applied ).
	/// \param	aCount					Number of points. 
	/// \param	aCurvePointsCount		Number of curve points, used for curve parameter t calculation. 
	/// \param [in,out]	aBoundingBox	The bounding box. 
	/// \param  aCutFactor				Parametric representation of hair cut.
	/// 
	/// \return final number of hair points.
	///-------------------------------------------------------------------------------------------------
	inline void updateBoundingBox( Point * aPoints, Vector * aTangents, unsigned __int32 aCount, 
		unsigned __int32 aCurvePointsCount, BoundingBox & aBoundingBox, PositionType aCutFactor );

	tPositionGenerator & mPositionGenerator;	///< The position generator

	tOutputGenerator & mOutputGenerator;	///< The output generator

	const HairProperties * mHairProperties; ///< The hair properties

	RandomGenerator mRandom;	///< The random generator

	// Generated hair tmp properties

	ColorType mRootColor[ 3 ];  ///< The root color

	ColorType mTipColor[ 3 ];   ///< The tip color

	ColorType mHueDistance;   ///< The distance in hue between root and tip color 

	OpacityType mRootOpacity;  ///< The root opacity

	OpacityType mTipOpacity;   ///< The tip opacity

	WidthType mRootWidth;   ///< Width of the root

	WidthType mTipWidth;	///< Width of the tip

	// Multi-strands properties

	PositionType mCosTwist; ///< The cosine of twist angle

	PositionType mSinTwist; ///< The sine of twist angle

	PositionType mTipSplay; ///< The tip splay

	PositionType mCenterSplay; ///< The center splay

	PositionType mRootSplay; ///< The root splay

	PositionType mRandomizeScale;	///< The randomize scale factor

	PositionType mOffset;   ///< The offset of tips

	PositionType mAspect;   ///< The aspect ratio of disk samples

	// Debug info

	BoundingBox mBoundingBox;   ///< The bounding box of generated hair points
};

// inline functions implementation

template< typename tPositionGenerator, typename tOutputGenerator >
inline HairGenerator< tPositionGenerator, tOutputGenerator >::HairGenerator
	( tPositionGenerator & aPositionGenerator, tOutputGenerator & aOutputGenerator ):
	mPositionGenerator( aPositionGenerator ),
	mOutputGenerator( aOutputGenerator )
{
}

template< typename tPositionGenerator, typename tOutputGenerator >
inline const BoundingBox & HairGenerator< tPositionGenerator, tOutputGenerator >::
getBoundingBox() const
{
	return mBoundingBox;
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_GENERATOR_HPP