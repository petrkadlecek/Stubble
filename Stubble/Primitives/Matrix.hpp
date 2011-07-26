#ifndef STUBBLE_MATRIX_HPP
#define STUBBLE_MATRIX_HPP

namespace Stubble
{

///----------------------------------------------------------------------------------------------------
/// Representation of Matrix. 
///----------------------------------------------------------------------------------------------------
template < typename Type >
class Matrix
{ 
public:
	Type m[ 16 ];
};

} //namespace Stubble

#endif //STUBBLE_MATRIX_HPP