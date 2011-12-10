#ifndef STUBBLE_CLUMP_BRUSH_MODE_HPP
#define STUBBLE_CLUMP_BRUSH_MODE_HPP

#include "../BrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Class implementing the clump brush transformation, i.e. clumping selected hair together by
/// choosing centroid of selected hair roots, calculating average normal and moving hair vertices
/// onto this normal.
///----------------------------------------------------------------------------------------------------
class ClumpBrushMode :
	public BrushMode
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Calculates mean position of the guides and average normal and then pushes all vertices toward
	/// positions on the calculated normal using linear interpolation.
	///
	/// \param aTask Hair task object containing transformation details, see HairTask.hpp
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( HairTask *aTask );

private:

	///----------------------------------------------------------------------------------------------------
	/// From given guides calculates position and normal of a clump guide (imaginary).
	///
	/// \param aGuides Guides to be clumped together
	/// \param[out] aPosition Position of the clump virtual guide
	/// \param[out] aNormal Normal of the clump virtual guide
	///----------------------------------------------------------------------------------------------------
	void findClumpCenter( const HairShape::HairComponents::SelectedGuides &aGuides, Vector3D< Real > &aPosition, Vector3D< Real > &aNormal );
};

} // namespace Toolbox

} // namespace Stubble

#endif
