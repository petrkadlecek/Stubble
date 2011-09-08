#ifndef STUBBLE_HAIR_PROPERTIES_HPP
#define STUBBLE_HAIR_PROPERTIES_HPP

#include "HairShape/Texture/Texture.hpp"

#include <string>

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Hair properties. Stores all properties of the interpolated hair.
///-------------------------------------------------------------------------------------------------
class HairProperties
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Constructor. Loads properties from frame file.
	///
	/// \param	aFrameFileName	Filename of the frame file. 
	///-------------------------------------------------------------------------------------------------
	HairProperties( const std::string & aFrameFileName );

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	~HairProperties();
	
	/* HERE WILL BE ALL HAIR PROPERTIES ACCESSIBLE BY GETTER */
protected:
	///-------------------------------------------------------------------------------------------------
	/// Default constructor. Properties will be set by class deriving from HairProperties.
	///-------------------------------------------------------------------------------------------------
	inline HairProperties();
	
	/* HERE WILL BE STORED ALL HAIR PROPERTIES */
};

// inline functions implementation

inline HairProperties::HairProperties()
{
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_POSITION_GENERATOR_HPP