#ifndef STUBBLE_STUBBLE_EXCEPTION_HPP
#define STUBBLE_STUBBLE_EXCEPTION_HPP

#include <stdexcept>

namespace Stubble
{

///-------------------------------------------------------------------------------------------------
/// Exception for signalling stubble errors. 
///-------------------------------------------------------------------------------------------------
class StubbleException : public std::exception
{
public:
	///-------------------------------------------------------------------------------------------------
	/// Constructor. 
	///
	/// \param	aMessage	Message describing exception. 
	///-------------------------------------------------------------------------------------------------
	StubbleException( const char * const & aMessage ): exception( aMessage ) 
	{
	}
};
	
} //namespace Stubble

#endif // STUBBLE_STUBBLE_EXCEPTION_HPP