#include "CommandsTextures.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

///----------------------------------------------------------------------------------------------------
/// ResetTexturesCommand
///----------------------------------------------------------------------------------------------------

void *ResetTexturesCommand::creator()
{
	return new ResetTexturesCommand();
}

MStatus ResetTexturesCommand::doIt( const MArgList &aArgList )
{
	if ( HairShape::getActiveObject() != 0 )
	{
		HairShape::getActiveObject()->refreshTextures();
		return MStatus::kSuccess;	
	}
	return MStatus::kFailure;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble