#ifndef STUBBLE_RANDOM_GENERATOR_HPP
#define STUBBLE_RANDOM_GENERATOR_HPP

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
	double uniformNumber();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Gets the uniform random double from [aMin,aMax]. 
	///
	/// \param	aMin	a minimum. 
	/// \param	aMax	a maximum. 
	///
	/// \return	Uniform random double from [aMin,aMax]. 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline double randomDouble( double aMin, double aMax );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Gets the uniform random float from [aMin,aMax]. 
	///
	/// \param	aMin	a minimum. 
	/// \param	aMax	a maximum. 
	///
	/// \return	Uniform random float from [aMin,aMax]. 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline float randomFloat( float aMin, float aMax );

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
	double u[ 97 ]; ///< Random generator data
	double c, cd, cm; ///< Random generator data
	int i97, j97; ///< Random generator data
};

// inline functions implementation
inline double RandomGenerator::randomDouble( double aMin, double aMax ) 
{
	return ( aMin >= aMax ) ? ( aMin  ) : 
			( aMin + (aMax - aMin) * uniformNumber() );
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_RANDOM_GENERATOR_HPP