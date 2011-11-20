#ifndef STUBBLE_CLUMP_BRUSH_MODE_HPP
#define STUBBLE_CLUMP_BRUSH_MODE_HPP

#include "../BrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// The class implementing the clump brush transformations.
///----------------------------------------------------------------------------------------------------
class ClumpBrushMode :
	public BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// The method which implements the actual brush transformations.
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( HairTask *aTask );

private:
	///----------------------------------------------------------------------------------------------------
	/// From given guides calculates position and normal of a clump guide (imaginary).
	///
	/// \param aGuides		Guides to be clumped together
	/// \param aPosition	Out - position of the clump guide
	/// \param aNormal		Out - normal of the clump guide
	///----------------------------------------------------------------------------------------------------
	void findClumpCenter( const HairShape::HairComponents::SelectedGuides &aGuides, Vector3D< Real > &aPosition, Vector3D< Real > &aNormal );
};

} // namespace Toolbox

} // namespace Stubble

#endif