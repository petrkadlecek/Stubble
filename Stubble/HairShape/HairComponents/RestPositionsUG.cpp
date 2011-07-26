#include "RestPositionsUG.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

///-------------------------------------------------------------------------------------------------
/// Default constructor. 
///-------------------------------------------------------------------------------------------------
RestPositionsUG::RestPositionsUG():
	mDirtyBit( false )
{
}

///-------------------------------------------------------------------------------------------------
/// Finaliser. 
///-------------------------------------------------------------------------------------------------
RestPositionsUG::~RestPositionsUG()
{
}

///-------------------------------------------------------------------------------------------------
/// Builds the uniform grid. 
///
/// \author	Sr 4ck 0cuc
/// \date	26.7.2011
///
/// \param	aGuidesRestPositions	The guides rest positions. 
/// \param	aInterpolationGroups	The interpolation groups.
///-------------------------------------------------------------------------------------------------
void RestPositionsUG::build( const GuidesRestPositions & aGuidesRestPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	mDirtyBit = false;
}

///-------------------------------------------------------------------------------------------------
/// Gets the n closest guides from aPosition. 
///
/// \param	aPosition				the position. 
/// \param	aInterpolationGroupId	Identifier for a interpolation group. 
/// \param	aN						Number of closest guides to return. 
/// \param [in,out]	aGuidesIds		List of identifiers for a closest guides. 
///-------------------------------------------------------------------------------------------------
void RestPositionsUG::getNClosestGuides( const Vector3D< Real > & aPosition, unsigned int aInterpolationGroupId,
	unsigned int aN, GuidesIds & aGuidesIds )
{
}

///-------------------------------------------------------------------------------------------------
/// Export to file. 
///
/// \param [in,out]	aOutputStream	the output stream. 
///-------------------------------------------------------------------------------------------------
void RestPositionsUG::exportToFile( std::ostream & aOutputStream )
{
}

///-------------------------------------------------------------------------------------------------
/// Import from file. 
///
/// \param [in,out]	aInputStream	the input stream. 
///-------------------------------------------------------------------------------------------------
void RestPositionsUG::importFromFile( std::istream & aInputStream )
{
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble