#ifndef STUBBLE_REINIT_COMMAND_HPP
#define STUBBLE_REINIT_COMMAND_HPP

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
/// Command that reinits zero sized hair of selected HairShape to initial length.
///----------------------------------------------------------------------------------------------------
class ReinitCommand
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

#endif // STUBBLE_REINIT_COMMAND_HPP
