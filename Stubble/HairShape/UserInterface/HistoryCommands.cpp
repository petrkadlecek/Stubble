#include "HistoryCommands.hpp"

namespace Stubble
{

namespace HairShape
{

void *UndoCommand::creator()
{
	return new UndoCommand();
}

MStatus UndoCommand::doIt( const MArgList &aArgList )
{
	try 
	{
		HairShape * active = HairShape::getActiveObject();
		if ( active != 0 && active->canUndo() )
		{
			active->undo();
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

void *RedoCommand::creator()
{
	return new RedoCommand();
}

MStatus RedoCommand::doIt( const MArgList &aArgList )
{
	try 
	{
		HairShape * active = HairShape::getActiveObject();
		if ( active != 0 && active->canRedo() )
		{
			active->redo();
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

} // namespace HairShape

} // namespace Stubble