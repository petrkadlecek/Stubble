#include "SwitchSelectionModeCommand.hpp"

#include "maya/MArgList.h"
#include "maya/MArgDatabase.h"
#include "maya/MItDependencyNodes.h"

#include "HairShapeUI.hpp"

namespace Stubble
{

namespace HairShape
{
	
void *SwitchSelectionModeCommand::creator()
{
	return new SwitchSelectionModeCommand();
}

MStatus SwitchSelectionModeCommand::doIt( const MArgList &aArgList )
{
	// let all the hair shape nodes know that the selection mode has changed
 	HairShapeUI::syncSelectionMode();

	// the selection never really changes when we go to "All vertices" selection mode
	if ( HairShapeUI::getSelectionMode() == HairShapeUI::kSelectAllVertices )
	{
		return MS::kSuccess;
	}

	MItDependencyNodes pluginIt(MFn::kPluginDependNode);
	//MItDependencyNodes pluginIt(MFn::kPluginShape);

	/*MSelectionList selection;
	MGlobal::getActiveSelectionList( selection );
	
	MItSelectionList pluginIt( selection, MFn::kPluginDependNode );	*/

	for ( ; !pluginIt.isDone(); pluginIt.next() )
	{		
		MFnDependencyNode node( pluginIt.thisNode() );
		MPxNode *mpxNode = node.userNode();		
		/*MDagPath path;
		MObject	component;
		pluginIt.getDagPath( path, component );
		MFnDependencyNode node( path.node() );
		MPxNode *mpxNode = node.userNode();	*/	
		if ( !( mpxNode != 0 && mpxNode->typeId() == Stubble::HairShape::HairShape::typeId ) ) // not our plugin?
		{
			continue;
		}
		HairShape *hairShape = dynamic_cast< HairShape * >( mpxNode );
		std::cout << hairShape->getFullPathNameAsString();
		if ( hairShape->isCurrentlySelected() )
		{
			std::cout << " - selected" << std::endl;
			hairShape->switchSelectedComponents();
		}		
		
	}
	std::cout << "==========================================================" << std::endl;
	return MStatus::kSuccess;
}

} // namespace HairShape

} // namespace Stubble