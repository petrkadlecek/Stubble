#include "CommandsNURBS.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

///----------------------------------------------------------------------------------------------------
/// ImportNURBSCommand
///----------------------------------------------------------------------------------------------------

void *ImportNURBSCommand::creator()
{
	return new ImportNURBSCommand();
}

MStatus ImportNURBSCommand::doIt( const MArgList &aArgList )
{
	if ( HairShape::getActiveObject() != 0 )
	{
		HairShape::getActiveObject()->importNURBS();
		return MStatus::kSuccess;	
	}
	return MStatus::kFailure;
}

///----------------------------------------------------------------------------------------------------
/// ExportToNURBSCommand
///----------------------------------------------------------------------------------------------------

void *ExportToNURBSCommand::creator()
{
	return new ExportToNURBSCommand();
}

MStatus ExportToNURBSCommand::doIt( const MArgList &aArgList )
{
 	if ( HairShape::getActiveObject() != 0 )
	{
		HairShape::getActiveObject()->exportToNURBS();
		return MStatus::kSuccess;	
	}
	return MStatus::kFailure;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble