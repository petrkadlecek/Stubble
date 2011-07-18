#ifndef STUBBLE_HAIR_GUIDES_HPP
#define STUBBLE_HAIR_GUIDES_HPP

#include "GuideHair.hpp"
#include "HairShape\Generators\RandomGenerator.hpp"
#include "HairShape\Generators\UVPointGenerator.hpp"

#include <maya/MFnMesh.h>

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// Class stores hair guides, implements undo stack and selection.
///----------------------------------------------------------------------------------------------------
class HairGuides
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Defines an alias representing the guide hairs
	///----------------------------------------------------------------------------------------------------
	typedef GuideHair * GuideHairs;

	///----------------------------------------------------------------------------------------------------
	/// Constructor
	///----------------------------------------------------------------------------------------------------
	HairGuides();

	///----------------------------------------------------------------------------------------------------
	/// Finalizer
	///----------------------------------------------------------------------------------------------------
	~HairGuides();

private:
	// TODO - temporary placement
	GuideHairs mGuideHairs; ///< Array of guide hairs.	

	int mGuideHairsCount;
};

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_GUIDES_HPP