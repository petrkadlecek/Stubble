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
 	MSelectionList selection;
	MGlobal::getActiveSelectionList( selection );
	
	MItSelectionList pluginIt( selection, MFn::kPluginDependNode );		

	for ( ; !pluginIt.isDone(); pluginIt.next() )
	{
		MDagPath path;
		pluginIt.getDagPath( path );
		MFnDependencyNode node( path.node() );
		MPxNode *mpxNode = node.userNode();		
		if ( mpxNode->typeId() == Stubble::HairShape::HairShape::typeId ) // our plugin?
		{
			Stubble::HairShape::HairShape *hairShape = dynamic_cast< HairShape * >( mpxNode );
			hairShape->importNURBS();
			return MStatus::kSuccess;			
		}
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
 	MSelectionList selection;
	MGlobal::getActiveSelectionList( selection );
	
	MItSelectionList pluginIt( selection, MFn::kPluginDependNode );		

	for ( ; !pluginIt.isDone(); pluginIt.next() )
	{
		MDagPath path;
		pluginIt.getDagPath( path );
		MFnDependencyNode node( path.node() );
		MPxNode *mpxNode = node.userNode();		
		if ( mpxNode->typeId() == Stubble::HairShape::HairShape::typeId ) // our plugin?
		{
			Stubble::HairShape::HairShape *hairShape = dynamic_cast< HairShape * >( mpxNode );
			hairShape->exportToNURBS();
			return MStatus::kSuccess;
		}
	}

	return MStatus::kFailure;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble