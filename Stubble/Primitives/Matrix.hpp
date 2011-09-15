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

	///----------------------------------------------------------------------------------------------------
	/// Const array indexer operator. 
	///
	/// \param	aIndex	Zero-based index of a. 
	///
	/// \return	The indexed value. 
	///----------------------------------------------------------------------------------------------------
	inline Type operator[] ( unsigned __int32 aIndex ) const;

	///----------------------------------------------------------------------------------------------------
	/// Array indexer operator. 
	///
	/// \param	aIndex	Zero-based index of a. 
	///
	/// \return	The indexed value. 
	///----------------------------------------------------------------------------------------------------
	inline Type & operator[] ( unsigned __int32 aIndex );

	Type m[ 16 ];
};

template < typename Type >
inline Type Matrix< Type >::operator[] ( unsigned __int32 aIndex ) const
{
	return m[ aIndex ];
}

template < typename Type >
inline Type & Matrix< Type >::operator[] ( unsigned __int32 aIndex )
{
	return m[ aIndex ];
}

} //namespace Stubble

#endif //STUBBLE_MATRIX_HPP