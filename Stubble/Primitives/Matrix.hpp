#ifndef STUBBLE_MATRIX_HPP
#define STUBBLE_MATRIX_HPP

#include <memory>

namespace Stubble
{

///----------------------------------------------------------------------------------------------------
/// Representation of Matrix. 
///----------------------------------------------------------------------------------------------------
template < typename Type >
class Matrix
{ 
public:

	inline Matrix();

	inline Matrix( const Matrix< Type > &aM );

	inline Matrix< Type > & operator= ( const Matrix< Type > &aM );

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

	///-------------------------------------------------------------------------------------------------
	/// Multiplication operator. 
	///
	/// \param	aMat	The matrix. 
	///
	/// \return	The result of the operation. 
	///-------------------------------------------------------------------------------------------------
	inline Matrix< Type > operator* ( const Matrix< Type > & aMat ) const;

private:

	static const unsigned __int32 SIZE = 16;

	Type m[ Matrix::SIZE ];   ///< The matrix data
};

template < typename Type >
inline Matrix< Type >::Matrix()
{
	std::uninitialized_fill_n(m, Matrix::SIZE, 0);
}

template < typename Type >
inline Matrix< Type >::Matrix( const Matrix< Type > &aM )
{
	std::uninitialized_copy(aM.m, aM.m + Matrix::SIZE, m);
}

template < typename Type >
inline Matrix< Type > & Matrix< Type >::operator= ( const Matrix< Type > &aM )
{
	std::uninitialized_copy(aM.m, aM.m + Matrix::SIZE, m);

	return *this;
}

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

template < typename Type >
inline Matrix< Type > Matrix< Type >::operator* ( const Matrix< Type > & aMat ) const
{
	Matrix< Type > tmp;
	// First column
	tmp.m[ 0 ] = aMat.m[ 0 ] * m[ 0 ] + aMat.m[ 1 ] * m[ 4 ] + aMat.m[ 2 ] * m[ 8 ] + aMat.m[ 3 ] * m[ 12 ];
	tmp.m[ 1 ] = aMat.m[ 0 ] * m[ 1 ] + aMat.m[ 1 ] * m[ 5 ] + aMat.m[ 2 ] * m[ 9 ] + aMat.m[ 3 ] * m[ 13 ];
	tmp.m[ 2 ] = aMat.m[ 0 ] * m[ 2 ] + aMat.m[ 1 ] * m[ 6 ] + aMat.m[ 2 ] * m[ 10 ] + aMat.m[ 3 ] * m[ 14 ];
	tmp.m[ 3 ] = aMat.m[ 0 ] * m[ 3 ] + aMat.m[ 1 ] * m[ 7 ] + aMat.m[ 2 ] * m[ 11 ] + aMat.m[ 3 ] * m[ 15 ];
	// Second column
	tmp.m[ 4 ] = aMat.m[ 4 ] * m[ 0 ] + aMat.m[ 5 ] * m[ 4 ] + aMat.m[ 6 ] * m[ 8 ] + aMat.m[ 7 ] * m[ 12 ];
	tmp.m[ 5 ] = aMat.m[ 4 ] * m[ 1 ] + aMat.m[ 5 ] * m[ 5 ] + aMat.m[ 6 ] * m[ 9 ] + aMat.m[ 7 ] * m[ 13 ];
	tmp.m[ 6 ] = aMat.m[ 4 ] * m[ 2 ] + aMat.m[ 5 ] * m[ 6 ] + aMat.m[ 6 ] * m[ 10 ] + aMat.m[ 7 ] * m[ 14 ];
	tmp.m[ 7 ] = aMat.m[ 4 ] * m[ 3 ] + aMat.m[ 5 ] * m[ 7 ] + aMat.m[ 6 ] * m[ 11 ] + aMat.m[ 7 ] * m[ 15 ];
	// Third column
	tmp.m[ 8 ] = aMat.m[ 8 ] * m[ 0 ] + aMat.m[ 9 ] * m[ 4 ] + aMat.m[ 10 ] * m[ 8 ] + aMat.m[ 11 ] * m[ 12 ];
	tmp.m[ 9 ] = aMat.m[ 8 ] * m[ 1 ] + aMat.m[ 9 ] * m[ 5 ] + aMat.m[ 10 ] * m[ 9 ] + aMat.m[ 11 ] * m[ 13 ];
	tmp.m[ 10 ] = aMat.m[ 8 ] * m[ 2 ] + aMat.m[ 9 ] * m[ 6 ] + aMat.m[ 10 ] * m[ 10 ] + aMat.m[ 11 ] * m[ 14 ];
	tmp.m[ 11 ] = aMat.m[ 8 ] * m[ 3 ] + aMat.m[ 9 ] * m[ 7 ] + aMat.m[ 10 ] * m[ 11 ] + aMat.m[ 11 ] * m[ 15 ];
	// Fourth column
	tmp.m[ 12 ] = aMat.m[ 12 ] * m[ 0 ] + aMat.m[ 13 ] * m[ 4 ] + aMat.m[ 14 ] * m[ 8 ] + aMat.m[ 15 ] * m[ 12 ];
	tmp.m[ 13 ] = aMat.m[ 12 ] * m[ 1 ] + aMat.m[ 13 ] * m[ 5 ] + aMat.m[ 14 ] * m[ 9 ] + aMat.m[ 15 ] * m[ 13 ];
	tmp.m[ 14 ] = aMat.m[ 12 ] * m[ 2 ] + aMat.m[ 13 ] * m[ 6 ] + aMat.m[ 14 ] * m[ 10 ] + aMat.m[ 15 ] * m[ 14 ];
	tmp.m[ 15 ] = aMat.m[ 12 ] * m[ 3 ] + aMat.m[ 13 ] * m[ 7 ] + aMat.m[ 14 ] * m[ 11 ] + aMat.m[ 15 ] * m[ 15 ];
	return tmp;
}

} //namespace Stubble

#endif //STUBBLE_MATRIX_HPP