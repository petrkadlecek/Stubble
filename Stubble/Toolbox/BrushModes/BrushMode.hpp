#ifndef STUBBLE_BRUSH_MODE_HPP
#define STUBBLE_BRUSH_MODE_HPP

#include "../../Primitives/Vector3D.hpp"
#include "HairShape/HairComponents/SelectedGuides.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// The interface for all of the brush mode classes.
///----------------------------------------------------------------------------------------------------
class BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// The method in which the derived classes implement the different brushing transformations.
	///
	/// \param aDX	Cursor change in world coordinates
	/// \param aGuideHair	Guide selection affected by the transformation
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( const Vector3D< double > &aDX, HairShape::HairComponents::SelectedGuide &aGuideHair ) = 0;

	///----------------------------------------------------------------------------------------------------
	/// Sets whether the brush falloff is enabled or disabled
	///
	/// \param aValue	New value of the falloff switch
	///----------------------------------------------------------------------------------------------------
	inline void setFalloffSwitch ( bool aValue );

protected:

	bool mEnableFalloff;
};

inline void BrushMode::setFalloffSwitch ( bool aValue )
{
	mEnableFalloff = aValue;
}

} // namespace Toolbox

} // namespace Stubble

#endif