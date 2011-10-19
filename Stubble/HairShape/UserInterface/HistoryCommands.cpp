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
 	HairShape * active = HairShape::getActiveObject();
	if ( active != 0 && active->canUndo() )
	{
		active->undo();
		return MStatus::kSuccess;	
	}
	return MStatus::kFailure;
}

void *RedoCommand::creator()
{
	return new RedoCommand();
}

MStatus RedoCommand::doIt( const MArgList &aArgList )
{
 	HairShape * active = HairShape::getActiveObject();
	if ( active != 0 && active->canRedo() )
	{
		active->redo();
		return MStatus::kSuccess;	
	}
	return MStatus::kFailure;
}

} // namespace HairShape

} // namespace Stubble