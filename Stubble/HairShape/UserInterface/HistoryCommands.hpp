#ifndef STUBBLE_HISTORY_COMMANDS_HPP
#define STUBBLE_HISTORY_COMMANDS_HPP

#include "HairShape\UserInterface\HairShape.hpp"

#include <maya/MPxCommand.h>
#include <maya/MPxNode.h>

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// Command that undos changes on active HairShape.
///----------------------------------------------------------------------------------------------------
class UndoCommand
	: public MPxCommand
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Get an instance of the command.
	///----------------------------------------------------------------------------------------------------
	static void *creator();

	///----------------------------------------------------------------------------------------------------
	/// Execute the command.
	///----------------------------------------------------------------------------------------------------
	virtual MStatus doIt( const MArgList &aArgList );	
};

///----------------------------------------------------------------------------------------------------
/// Command that redos changes on active HairShape.
///----------------------------------------------------------------------------------------------------
class RedoCommand
	: public MPxCommand
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Get an instance of the command.
	///----------------------------------------------------------------------------------------------------
	static void *creator();

	///----------------------------------------------------------------------------------------------------
	/// Execute the command.
	///----------------------------------------------------------------------------------------------------
	virtual MStatus doIt( const MArgList &aArgList );	
};

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HISTORY_COMMANDS_HPP