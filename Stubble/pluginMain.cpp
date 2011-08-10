/*
	Usage:

		if ( !`contextInfo -exists stubbleBrushTool` )
		{
			StubbleBrushToolCommand stubbleBrushTool;
			print ( "Created the stubbleBrushTool context.\n" );
		}
		setToolTo stubbleBrushTool;
*/


#include <maya/MFnPlugin.h>

#include "HairShape\UserInterface\HairShape.hpp"
#include "HairShape\UserInterface\HairShapeUI.hpp"

#include "Toolbox/Tools/BrushTool/BrushTool.hpp"

// Export the functions to make them visible in Maya
#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

///----------------------------------------------------------------------------------------------------
/// This function is called once when the plugin is loaded. It is used for registration of
/// custom nodes, mel commands etc.
///
/// \param aObj A handle to the plug-in object.
/// \return Maya status code.
///----------------------------------------------------------------------------------------------------
EXPORT MStatus initializePlugin( MObject aObj )
{ 
	MStatus status;
	MFnPlugin plugin( aObj, "The Stubble Team", "0.11", "Any" );

	// Add plug-in feature registration here
	// register BrushToolCommand
	status = plugin.registerContextCommand( Stubble::Toolbox::BrushToolCommand::sCommandName, 
		Stubble::Toolbox::BrushToolCommand::creator );

	// check for error
	if ( status != MS::kSuccess )
		status.perror( "Could not register BrushToolCommand." );

	status = plugin.registerShape( Stubble::HairShape::HairShape::typeName, // Node name
								  Stubble::HairShape::HairShape::typeId, // Unique ID of our node
								  Stubble::HairShape::HairShape::creator, // Handler to creator function (just creates node)
								  Stubble::HairShape::HairShape::initialize, // Handler to initialize function (initializes all attributes)
								  Stubble::HairShape::HairShapeUI::creator ); //Creator of UI

	// check for error
	if( status != MS::kSuccess )
		status.perror( "could not register the HairShape node" );

	return status;
}

///----------------------------------------------------------------------------------------------------
/// This method is called when the plug-in is unloaded from Maya. It 
///	deregisters all of the services that it was providing.
///
/// \param aObj A handle to the plug-in object.
/// \return Maya status code.
///----------------------------------------------------------------------------------------------------
EXPORT MStatus uninitializePlugin( MObject aObj )
{
	MStatus status;
	MFnPlugin plugin( aObj );

	// Add plug-in feature deregistration here
	status = plugin.deregisterContextCommand( Stubble::Toolbox::BrushToolCommand::sCommandName );

	// check for error
	if ( status != MS::kSuccess )
		status.perror( "Could not deregister BrushToolCommand." );

	return status;
}