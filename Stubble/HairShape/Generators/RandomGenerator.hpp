#ifndef STUBBLE_RANDOM_GENERATOR_HPP
#define STUBBLE_RANDOM_GENERATOR_HPP

#include "Common\CommonTypes.hpp"

namespace Stubble
{

namespace HairShape
{

class RandomGenerator
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///----------------------------------------------------------------------------------------------------
	inline RandomGenerator();

	///----------------------------------------------------------------------------------------------------
	/// Deterministic reset of random generator with default start values. 
	///----------------------------------------------------------------------------------------------------
	void reset() 
	{
		reset( 1802, 9373 );
	}

	///----------------------------------------------------------------------------------------------------
	/// Deterministic reset of random generator with given start values. 
	///
	/// \param	aIJ	a random generator start value #1. 
	/// \param	aKL	a random generator start value #2. 
	///----------------------------------------------------------------------------------------------------
	void reset( int aIJ, int aKL );

	///----------------------------------------------------------------------------------------------------
	/// Gets the uniform random number from [0,1]. 
	///
	/// \return	Uniform random number from [0,1]. 
	///----------------------------------------------------------------------------------------------------
	inline Real uniformNumber();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Gets the uniform random Real from [aMin,aMax]. 
	///
	/// \param	aMin	a minimum. 
	/// \param	aMax	a maximum. 
	///
	/// \return	Uniform random Real from [aMin,aMax]. 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline Real randomReal( Real aMin, Real aMax );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Gets the uniform random integer from [aMin,aMax]. 
	///
	/// \param	aMin	a minimum. 
	/// \param	aMax	a maximum. 
	///
	/// \return	Uniform random integer from [aMin,aMax]. 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline int randomInteger( int aMin, int aMax );

	///----------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///----------------------------------------------------------------------------------------------------
	~RandomGenerator() {}

private:
	Real u[ 97 ]; ///< Random generator data
	Real c, cd, cm; ///< Random generator data
	int i97, j97; ///< Random generator data
};

// inline functions implementation

inline RandomGenerator::RandomGenerator()
{
	reset(); // Resets James random generator with default start values
}

inline Real RandomGenerator::uniformNumber()
{
	Real uni = u[i97] - u[j97];
	
	if ( uni < 0.0 )
		uni += 1.0;

	u[i97] = uni;
	if( --i97 < 0 )
		i97 = 96;

	if( --j97 < 0 )
		j97 = 96;

	if( (c -= cd) < 0.0 )
		c += cm;

	if( (uni -= c) < 0.0 )
		uni += 1.0;

	return uni;
}

inline Real RandomGenerator::randomReal( Real aMin, Real aMax ) 
{
	return ( aMin >= aMax ) ? ( aMin  ) : 
			( aMin + (aMax - aMin) * uniformNumber() );
}

inline int RandomGenerator::randomInteger( int aMin, int aMax ) 
{
	return ( aMin >= aMax ) ? ( aMin  ) : 
			( aMin + ( int ) ( (aMax + 1 - aMin) * uniformNumber() ) );
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_RANDOM_GENERATOR_HPP