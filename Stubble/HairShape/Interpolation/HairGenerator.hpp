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

	tPositionGenerator & mPositionGenerator;	///< The position generator

	tOutputGenerator & mOutputGenerator;	///< The output generator
		
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