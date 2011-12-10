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

#include "HairShape/UserInterface/HairShape.hpp"
#include "HairShape/UserInterface/HairShapeUI.hpp"
#include "HairShape/UserInterface/SelectCommand.hpp"
#include "HairShape/UserInterface/ReinitCommand.hpp"
#include "HairShape/UserInterface/ResetCommand.hpp"
#include "HairShape/UserInterface/HistoryCommands.hpp"

#include "HairShape\HairComponents\CommandsNURBS.hpp"
#include "HairShape/HairComponents/CommandsTextures.hpp"

#include "RibExport/RenderManCacheCommand.hpp"

#include "Toolbox/Tools/TabletSettingsTool.hpp"
#include "Toolbox/Tools/HapticSettingsTool.hpp"
#include "Toolbox/Tools/HapticListener.hpp"
#include "Toolbox/Tools/BrushTool/BrushTool.hpp"
#include "Toolbox/Tools/CutTool/CutTool.hpp"

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

	// register Stubble main menu
	status = plugin.registerUI("StubbleCreateUI", "StubbleDeleteUI");

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "Could not register StubbleCreateUI." );
		return status;
	}

	// register BrushToolCommand
	status = plugin.registerContextCommand( Stubble::Toolbox::BrushToolCommand::sCommandName, 
		Stubble::Toolbox::BrushToolCommand::creator );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "Could not register BrushToolCommand." );
		return status;
	}

	// register CutToolCommand
	status = plugin.registerContextCommand( Stubble::Toolbox::CutToolCommand::sCommandName, 
		Stubble::Toolbox::CutToolCommand::creator );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "Could not register CutToolCommand." );
		return status;
	}

	// register HapticSettingsToolCommand
	status = plugin.registerContextCommand( Stubble::Toolbox::HapticSettingsToolCommand::sCommandName, 
		Stubble::Toolbox::HapticSettingsToolCommand::creator );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "Could not register HapticSettingsToolCommand." );
		return status;
	}

	// register TabletSettingsToolCommand
	status = plugin.registerContextCommand( Stubble::Toolbox::TabletSettingsToolCommand::sCommandName, 
		Stubble::Toolbox::TabletSettingsToolCommand::creator );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "Could not register TabletSettingsToolCommand." );
		return status;
	}

  // register HapticListener
  status = plugin.registerNode( Stubble::Toolbox::HapticListener::typeName,
		              Stubble::Toolbox::HapticListener::typeId,
		              Stubble::Toolbox::HapticListener::creator,
		              Stubble::Toolbox::HapticListener::initialize,
		              MPxNode::kLocatorNode );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "Could not register HapticListener." );
		return status;
	}

	status = plugin.registerShape( Stubble::HairShape::HairShape::typeName, // Node name
								  Stubble::HairShape::HairShape::typeId, // Unique ID of our node
								  Stubble::HairShape::HairShape::creator, // Handler to creator function (just creates node)
								  Stubble::HairShape::HairShape::initialize, // Handler to initialize function (initializes all attributes)
								  Stubble::HairShape::HairShapeUI::creator ); //Creator of UI

	// check for error
	if( status != MS::kSuccess )
	{
		status.perror( "could not register the HairShape node" );
		return status;
	}

	// register Stubble3DelightCacheCommand command
	status = plugin.registerCommand( "Stubble3DelightCacheCommand", Stubble::RibExport::RenderManCacheCommand::creator );
	
	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not register the Stubble3DelightCacheCommand command" );
		return status;
	}

	// register StubbleImportNURBS command
	status = plugin.registerCommand( "StubbleImportNURBS", Stubble::HairShape::HairComponents::ImportNURBSCommand::creator );
	
	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not register the StubbleImportNURBS command" );
		return status;
	}

	// register StubbleExportToNURBS command
	status = plugin.registerCommand( "StubbleExportToNURBS", Stubble::HairShape::HairComponents::ExportToNURBSCommand::creator );
	
	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not register the StubbleExportToNURBS command" );
		return status;
	}

	// register StubbleResetTextures command
	status = plugin.registerCommand( "StubbleResetTextures", Stubble::HairShape::HairComponents::ResetTexturesCommand::creator );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not register the StubbleResetTextures command" );
		return status;
	}

	// register StubbleSelectCommand command
	status = plugin.registerCommand( "StubbleSelectCommand", Stubble::HairShape::SelectCommand::creator );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not register the StubbleSelectCommand command" );
		return status;
	}
	
	// register StubbleUndoCommand command
	status = plugin.registerCommand( "StubbleUndoCommand", Stubble::HairShape::UndoCommand::creator );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not register the StubbleUndoCommand command" );
		return status;
	}
	
	// register StubbleRedoCommand command
	status = plugin.registerCommand( "StubbleRedoCommand", Stubble::HairShape::RedoCommand::creator );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not register the StubbleRedoCommand command" );
		return status;
	}

	// register StubbleReinitCommand command
	status = plugin.registerCommand( "StubbleReinitCommand", Stubble::HairShape::ReinitCommand::creator );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not register the StubbleReinitCommand command" );
		return status;
	}

	// register StubbleResetCommand command
	status = plugin.registerCommand( "StubbleResetCommand", Stubble::HairShape::ResetCommand::creator );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not register the StubbleResetCommand command" );
		return status;
	}

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
	{
		status.perror( "Could not deregister BrushToolCommand." );
	}

	// deregister CutToolCommand
	status = plugin.deregisterContextCommand( Stubble::Toolbox::CutToolCommand::sCommandName );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "Could not deregister CutToolCommand." );
	}

	// deregister HapticSettingsToolCommand
	status = plugin.deregisterContextCommand( Stubble::Toolbox::HapticSettingsToolCommand::sCommandName );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "Could not deregister HapticSettingsToolCommand." );
	}

	// remove the HairShape callbacks
	Stubble::HairShape::HairShape::removeCallbacks();

	// deregister the HairShape node
	status = plugin.deregisterNode( Stubble::HairShape::HairShape::typeId );

	// check for error
	if( status != MS::kSuccess )
	{
		status.perror( "could not unregister the HairShape node" );
	}

	// deregister Stubble3DelightCacheCommand command
	status = plugin.deregisterCommand( "Stubble3DelightCacheCommand" );
	
	if ( status != MS::kSuccess )
	{
		status.perror( "could not unregister the Stubble3DelightCacheCommand command" );
	}

	// deregister StubbleExportToNURBS command
	status = plugin.deregisterCommand( "StubbleExportToNURBS" );
	
	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not unregister the StubbleExportToNURBS command" );
	}

	// deregister StubbleImportNURBS command
	status = plugin.deregisterCommand( "StubbleImportNURBS" );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not unregister the StubbleImportNURBS command" );
	}

	// deregister StubbleResetTextures command
	status = plugin.deregisterCommand( "StubbleResetTextures" );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not unregister the StubbleResetTextures command" );
	}

	// deregister StubbleSelectCommand command
	status = plugin.deregisterCommand( "StubbleSelectCommand" );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not unregister the StubbleSelectCommand command" );
	}

	// deregister StubbleUndoCommand command
	status = plugin.deregisterCommand( "StubbleUndoCommand" );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not unregister the StubbleUndoCommand command" );
	}

	// deregister StubbleRedoCommand command
	status = plugin.deregisterCommand( "StubbleRedoCommand" );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not unregister the StubbleRedoCommand command" );
	}

	// deregister StubbleReinitCommand command
	status = plugin.deregisterCommand( "StubbleReinitCommand" );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not unregister the StubbleReinitCommand command" );
	}

	// deregister StubbleResetCommand command
	status = plugin.deregisterCommand( "StubbleResetCommand" );

	// check for error
	if ( status != MS::kSuccess )
	{
		status.perror( "could not unregister the StubbleResetCommand command" );
	}

	// Clean up the brush worker thread
	Stubble::Toolbox::HairTaskProcessor::destroyInstance();

	return status;
}
