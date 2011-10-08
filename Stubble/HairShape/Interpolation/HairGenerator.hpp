#ifndef STUBBLE_HAIR_GENERATOR_HPP
#define STUBBLE_HAIR_GENERATOR_HPP

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
	///
	/// \param	aHairProperties	The hair properties. 
	///-------------------------------------------------------------------------------------------------
	void generate( const HairProperties & aHairProperties );
	
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
	/// Interpolate hair from guides. 
	///
	/// \param [in,out]	aPoints			If non-null, a points. 
	/// \param	aCount					Number of points. 
	/// \param	aRestPosition			The rest position of hair. 
	/// \param	aInterpolationGroupId	Identifier for a interpolation group. 
	///-------------------------------------------------------------------------------------------------
	inline void interpolateFromGuides( Point * aPoints, unsigned __int32 aCount, const MeshPoint &aRestPosition, 
		unsigned __int32 aInterpolationGroupId );

	///-------------------------------------------------------------------------------------------------
	/// Applies the scale to hair points. 
	///
	/// \param [in,out]	aPoints	If non-null, a hair points. 
	/// \param	aCount			Number of hair points. 
	/// \param	aRestPosition	The rest position of hair. 
	///-------------------------------------------------------------------------------------------------
	inline void applyScale( Point * aPoints, unsigned __int32 aCount, const MeshPoint &aRestPosition );

	///-------------------------------------------------------------------------------------------------
	/// Applies the frizz to hair points. 
	///
	/// \param [in,out]	aPoints		If non-null, a points. 
	/// \param	aCount				Number of points. 
	/// \param	aCurvePointsCount	Number of curve points, used for curve parameter t calculation. 
	/// \param	aRestPosition		The rest position of hair. 
	///-------------------------------------------------------------------------------------------------
	inline void applyFrizz( Point * aPoints, unsigned __int32 aCount, unsigned __int32 aCurvePointsCount, 
		const MeshPoint &aRestPosition );

	///-------------------------------------------------------------------------------------------------
	/// Applies the kink to hair points. 
	///
	/// \param [in,out]	aPoints		If non-null, a points. 
	/// \param	aCount				Number of points. 
	/// \param	aCurvePointsCount	Number of curve points, used for curve parameter t calculation. 
	/// \param	aRestPosition		The rest position of hair. 
	///-------------------------------------------------------------------------------------------------
	inline void applyKink( Point * aPoints, unsigned __int32 aCount, unsigned __int32 aCurvePointsCount, 
		const MeshPoint &aRestPosition );

	///-------------------------------------------------------------------------------------------------
	/// Applies the frizz to hair points. 
	///
	/// \param [in,out]	aPoints		If non-null, a points. 
	/// \param	aCount				Number of points. 
	/// \param	aCurvePointsCount	Number of curve points, used for curve parameter t calculation. 
	/// \param	aRestPosition		The rest position of hair. 
	///-------------------------------------------------------------------------------------------------
	inline void generateHairInStrand( Point * aPoints, unsigned __int32 aCount, unsigned __int32 aCurvePointsCount, 
		const MeshPoint &aRestPosition );

	///-------------------------------------------------------------------------------------------------
	/// Transforms points. 
	///
	/// \param [in,out]	aPoints		If non-null, a points. 
	/// \param	aCount				Number of points. 
	/// \param	aTransformMatrix	The transform matrix. 
	///-------------------------------------------------------------------------------------------------
	inline void transformPoints( Point * aPoints, unsigned __int32 aCount, const Matrix & aTransformMatrix );

	///-------------------------------------------------------------------------------------------------
	/// Calculates tangents. 
	///
	/// \param [in,out]	aTangents	The calculated tangents. 
	/// \param	aPoints				The hair points. 
	/// \param	aCount				Number of hair points. 
	///-------------------------------------------------------------------------------------------------
	inline void calculateTangents( Vector * aTangents, const Point * aPoints, unsigned __int32 aCount );

	///-------------------------------------------------------------------------------------------------
	/// Calculates the normal of selected hair point. aPoints and aTangents must be part of an array,
	/// in which is stored previous hair point.
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
	/// Calculates normals and binormals of selected hair. The first normal must be already set
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
	///
	/// \param [in,out]	mColor	Selected color in RGB, will be shifted. 
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
	/// Skips point if it is not necessary to output it ( it can be interpolated in render from
	/// two neighbour points ). aPoints and aTangents must be part of an array, in which is stored 
	/// previous hair point.
	///
	/// \param	aPoints		The selected hair point. 
	/// \param	aTangents	The selected hair tangent. 
	///
	/// \return	true if it succeeds, false if it fails. 
	///-------------------------------------------------------------------------------------------------
	inline bool skipPoint( const Point * aPoints, const Vector * aTangents );

	///-------------------------------------------------------------------------------------------------
	/// Generates final hair points positions, normals, colors, opacities, widths.
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
		unsigned __int32 aCurvePointsCount, const MeshPoint &aRestPosition, PositionType aCutFactor  );

	///-------------------------------------------------------------------------------------------------
	/// Output hair index and u v coordinates. 
	///
	/// \param	aHairIndex		Zero-based index of a hair. 
	/// \param	aStrandIndex	Zero-based index of a strand. 
	/// \param	aRestPosition	The hair rest position. 
	///-------------------------------------------------------------------------------------------------
	inline void outputHairIndexAndUVs( IndexType aHairIndex, IndexType aStrandIndex, const MeshPoint &aRestPosition );

	///-------------------------------------------------------------------------------------------------
	/// Samples disk. Uses two samples from [0-1]^2 to sample disk and returns phi angle and radius.
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
	/// Generates a hair positions in strand local space. 
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

	tPositionGenerator & mPositionGenerator;	///< The position generator

	tOutputGenerator & mOutputGenerator;	///< The output generator

	const HairProperties * mHairProperties; ///< The hair properties

	HairShape::RandomGenerator mRandom;	///< The random generator

	// Generated hair tmp properties

	ColorType mRootColor[ 3 ];  ///< The root color

	ColorType mTipColor[ 3 ];   ///< The tip color

	OpacityType mRootOpacity[ 3 ];  ///< The root opacity

	OpacityType mTipOpacity[ 3 ];   ///< The tip opacity

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
};

// inline functions implementation

template< typename tPositionGenerator, typename tOutputGenerator >
inline HairGenerator< tPositionGenerator, tOutputGenerator >::HairGenerator
	( tPositionGenerator & aPositionGenerator, tOutputGenerator & aOutputGenerator ):
	mPositionGenerator( aPositionGenerator ),
	mOutputGenerator( aOutputGenerator )
{
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_GENERATOR_HPP