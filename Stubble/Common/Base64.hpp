#ifndef STUBBLE_BASE64_HPP
#define STUBBLE_BASE64_HPP

#include <string>

namespace Stubble 
{

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

};

#endif	// STUBBLE_BASE64_HPP