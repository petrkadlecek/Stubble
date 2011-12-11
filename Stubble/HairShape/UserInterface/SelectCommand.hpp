#ifndef STUBBLE_SELECT_COMMAND_HPP
#define STUBBLE_SELECT_COMMAND_HPP

#include "HairShape\UserInterface\HairShape.hpp"

#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MPxCommand.h>
#include <maya/MPxNode.h>

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// Command for internal selection of HairShape.
/// See doIt method for more information about this command.
///----------------------------------------------------------------------------------------------------
class SelectCommand
	: public MPxCommand
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Get an instance of the command.
	///----------------------------------------------------------------------------------------------------
	static void *creator();

	///----------------------------------------------------------------------------------------------------
	/// Execute the command.
	/// Lists through all Maya selected objects and sets pointer to Maya selected HairShape.
	/// If no HairShape is selected by Maya, then it calls Maya selection on internaly selected HairShape.
	///----------------------------------------------------------------------------------------------------
	virtual MStatus doIt( const MArgList &aArgList );	
};

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_SELECT_COMMAND_HPP