#ifndef STUBBLE_COMMANDS_TEXTURES_HPP
#define STUBBLE_COMMANDS_TEXTURES_HPP

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

namespace HairComponents
{

///----------------------------------------------------------------------------------------------------
/// Command that imports NURBS from Maya into HairGuides.
///----------------------------------------------------------------------------------------------------
class ResetTexturesCommand
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
	
} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_COMMANDS_TEXTURES_HPP