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
	try 
	{
		HairShape * active = HairShape::getActiveObject();
		if ( active != 0 )
		{
			active->importNURBS();
			return MStatus::kSuccess;	
		}
	}
	catch( const StubbleException & ex )
	{
		MStatus s;
		s.perror( ex.what() );
		return s;
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
	try 
	{
		HairShape * active = HairShape::getActiveObject();
		if ( active != 0 )
		{
			active->exportToNURBS();
			return MStatus::kSuccess;	
		}
	}
	catch( const StubbleException & ex )
	{
		MStatus s;
		s.perror( ex.what() );
		return s;
	}
	return MStatus::kFailure;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble