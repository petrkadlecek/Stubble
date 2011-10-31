#ifndef STUBBLE_TRANSLATE_BRUSH_MODE_HPP
#define STUBBLE_TRANSLATE_BRUSH_MODE_HPP

#include "../BrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// The class implementing the translate brush transformations.
///----------------------------------------------------------------------------------------------------
class TranslateBrushMode :
	public BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Transforms camera space vector into the world coordinates vector.
	///----------------------------------------------------------------------------------------------------
	virtual Vector3D< double > preBrushTransform( const Vector3D< double > &aDX, void *aTransformInfo);

	///----------------------------------------------------------------------------------------------------
	/// The method which implements the actual brush transformations.
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( const Vector3D< double > &aDX, HairShape::HairComponents::SelectedGuide &aGuideHair );
};

} // namespace Toolbox

} // namespace Stubble

#endif