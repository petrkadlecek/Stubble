#include "SelectCommand.hpp"

#include "maya/MArgList.h"
#include "maya/MArgDatabase.h"

namespace Stubble
{

namespace HairShape
{
	
void *SelectCommand::creator()
{
	return new SelectCommand();
}

MStatus SelectCommand::doIt( const MArgList &aArgList )
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
		if ( mpxNode != 0 && mpxNode->typeId() == Stubble::HairShape::HairShape::typeId ) // our plugin?
		{
			HairShape *hairShape = dynamic_cast< HairShape * >( mpxNode );
			hairShape->setAsActiveObject();
			setResult( hairShape->name() ); // Returns selected hair shape name as result
			return MStatus::kSuccess;			
		}
	}
	// if no selected hairShape was found, return active name
	HairShape *hairShape = HairShape::getActiveObject();
	if ( hairShape != 0 )
	{
		setResult( hairShape->name() );
	}
	return MStatus::kSuccess;
}

} // namespace HairShape

} // namespace Stubble