#ifndef STUBBLE_HAIR_GENERATOR_HPP
#define STUBBLE_HAIR_GENERATOR_HPP

#include "HairProperties.hpp"
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

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing the point.
	///-------------------------------------------------------------------------------------------------
	typedef Vector3D< typename tOutputGenerator::PositionType > Point;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing the vector.
	///-------------------------------------------------------------------------------------------------
	typedef Vector3D< typename tOutputGenerator::NormalType > Normal;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing the matrix .
	///-------------------------------------------------------------------------------------------------
	typedef Matrix< typename tOutputGenerator::PositionType > Matrix;

	///-------------------------------------------------------------------------------------------------
	/// Applies the scale to hair points. 
	///
	/// \param [in,out]	aPoints	If non-null, a hair points. 
	/// \param	aCount			Number of hair points. 
	/// \param	aRestPosition	The rest position of hair. 
	///-------------------------------------------------------------------------------------------------
	inline void applyScale( Point * aPoints, unsigned __int32 aCount, const MeshPoint &aRestPosition );

	///-------------------------------------------------------------------------------------------------
	/// Transforms points. 
	///
	/// \param [in,out]	aPoints		If non-null, a points. 
	/// \param	aCount				Number of points. 
	/// \param	aTransformMatrix	The transform matrix. 
	///-------------------------------------------------------------------------------------------------
	inline void transformPoints( Point * aPoints, unsigned __int32 aCount, const Matrix & aTransformMatrix );

	tPositionGenerator & mPositionGenerator;	///< The position generator

	tOutputGenerator & mOutputGenerator;	///< The output generator

	const HairProperties * mHairProperties; ///< The hair properties
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