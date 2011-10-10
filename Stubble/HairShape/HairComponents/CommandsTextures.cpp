#include "CommandsTextures.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

///----------------------------------------------------------------------------------------------------
/// ResetTexturesCommand
///----------------------------------------------------------------------------------------------------

void *ResetTexturesCommand::creator()
{
	return new ResetTexturesCommand();
}

MStatus ResetTexturesCommand::doIt( const MArgList &aArgList )
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
			hairShape->refreshTextures();
			return MStatus::kSuccess;			
		}
	}

	return MStatus::kFailure;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble