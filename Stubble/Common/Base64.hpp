#ifndef STUBBLE_BASE64_HPP
#define STUBBLE_BASE64_HPP

#include <string>

namespace Stubble 
{

///-------------------------------------------------------------------------------------------------
/// Encodes binary data to string using base 64 encode algorithm.
///
/// \param bytes_to_encode	The data to be encoded.
/// \param in_len			The length of input data.
///
/// \return	Encoded data in string format. 
///-------------------------------------------------------------------------------------------------
std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);

///-------------------------------------------------------------------------------------------------
/// Decodes string to binary data using base 64 decode algorithm.
///
/// \param	encoded_string	The encoded string. 
///
/// \return	Binary data. 
///-------------------------------------------------------------------------------------------------
std::string base64_decode(std::string const& encoded_string);

};

#endif	// STUBBLE_BASE64_HPP