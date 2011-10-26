#include "RenderManCacheCommand.hpp"

#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MDagPath.h>

namespace Stubble
{

namespace RibExport
{

// Cache initialization
RenderManCacheCommand::Cache RenderManCacheCommand::cache;

MStatus RenderManCacheCommand::doIt( const MArgList & aArgumentsList )
{
	MStatus status;
	// Parse arguments using syntax
	MArgDatabase argDatabase( syntax, aArgumentsList, &status );
	if ( status != MStatus::kSuccess )
	{
		return status;
	}
	// For each flag :
	if ( argDatabase.isFlagSet( "-st" ) && argDatabase.isFlagSet( "-a" ) ) 
	{
		return sampleTime( argDatabase );
	}
	if ( argDatabase.isFlagSet( "-r" ) ) 
	{	
		return remove( argDatabase );
	}
	if ( argDatabase.isFlagSet( "-e" ) ) 
	{
		return emit( argDatabase );
	}
	if ( argDatabase.isFlagSet( "-f" ) ) 
	{
		return flush( argDatabase );
	}
	if ( argDatabase.isFlagSet( "-c" ) ) 
	{
		return contains( argDatabase );
	}
	if ( argDatabase.isFlagSet( "-l" ) ) 
	{
		return list( argDatabase );
	}
	// Unknown command
	return MStatus::kFailure;
}

void * RenderManCacheCommand::creator()
{
	return new RenderManCacheCommand();
}

RenderManCacheCommand::RenderManCacheCommand()
{
	// Set required syntax for 3Delight renderman
	syntax.addFlag( "-st", "-sampleTime", MSyntax::kDouble );
	syntax.addFlag( "-a", "-addstep" );
	syntax.addFlag( "-r", "-remove" );
	syntax.addFlag( "-e", "-emit" );
	syntax.addFlag( "-f", "-flush" );
	syntax.addFlag( "-c", "-contains" );
	syntax.addFlag( "-l", "-list" );
	syntax.setObjectType( MSyntax::kSelectionList, 0, 1 );
}

MStatus RenderManCacheCommand::sampleTime( const MArgDatabase & aArgDatabase )
{
	MStatus status;
	MSelectionList sel;
	status = aArgDatabase.getObjects( sel ); // Get selected objects
	if ( status != MStatus::kSuccess )
	{
		return status;
	}
	double time;
	status = aArgDatabase.getFlagArgument( "-st", 0, time );
	if ( status != MStatus::kSuccess )
	{
		return status;
	}
	MItSelectionList it( sel, MFn::kPluginDependNode );
	for ( ; !it.isDone(); it.next() ) // Iterate through all selected objects
	{
		MDagPath path;
		it.getDagPath( path );
		MFnDependencyNode node( path.node() );
		MPxNode *mpxNode = node.userNode(); // Select node
		if ( mpxNode->typeId() != HairShape::HairShape::typeId ) // Test node id
		{
			continue; // not our plugin
		}
		HairShape::HairShape * hairShape = dynamic_cast< HairShape::HairShape * >( mpxNode );
		// Search for node in cache
		Cache::iterator it = cache.find( path.fullPathName().asChar() );
		// Not yet in cache
		if ( it == cache.end() )
		{
			// Insert in cache
			cache.insert( CacheItem( path.fullPathName().asChar(), 
				new CachedFrame( *hairShape, node.name().asChar(), static_cast< Time >( time ) ) ) );
		}
		else
		{
			// Add another time sample
			it->second->AddTimeSample( *hairShape, node.name().asChar(), static_cast< Time >( time ) );
		}
	}
	return MStatus::kSuccess;
}

MStatus RenderManCacheCommand::remove( const MArgDatabase & aArgDatabase )
{
	MStatus status;
	MSelectionList sel;
	status = aArgDatabase.getObjects( sel ); // Get selected objects
	if ( status != MStatus::kSuccess )
	{
		return status;
	}
	MItSelectionList it( sel, MFn::kPluginDependNode );
	for ( ; !it.isDone(); it.next() ) // Iterate through all selected objects
	{
		MDagPath path;
		it.getDagPath( path );
		// Found ?
		Cache::iterator cIt = cache.find( path.fullPathName().asChar() );
		if ( cIt != cache.end() )
		{
			// Remove
			delete cIt->second;
			cache.erase( cIt );
		}
	}
	return MStatus::kSuccess;
}

MStatus RenderManCacheCommand::emit( const MArgDatabase & aArgDatabase )
{
	MStatus status;
	MSelectionList sel;
	status = aArgDatabase.getObjects( sel ); // Get selected objects
	if ( status != MStatus::kSuccess )
	{
		return status;
	}
	MItSelectionList it( sel, MFn::kPluginDependNode );
	for ( ; !it.isDone(); it.next() ) // Iterate through all selected objects
	{
		MDagPath path;
		it.getDagPath( path );
		cache.find( path.fullPathName().asChar() )->second->emit(); // Render
	}
	return MStatus::kSuccess;
}

MStatus RenderManCacheCommand::flush( const MArgDatabase & aArgDatabase )
{
	// Free everything from cache
	for ( Cache::iterator it = cache.begin(); it != cache.end(); ++it )
	{
		delete it->second;
	}
	cache.clear();
	return MStatus::kSuccess;
}

MStatus RenderManCacheCommand::contains( const MArgDatabase & aArgDatabase )
{
	MStatus status;
	MSelectionList sel;
	status = aArgDatabase.getObjects( sel ); // Get selected objects
	if ( status != MStatus::kSuccess )
	{
		return status;
	}
	MItSelectionList it( sel, MFn::kPluginDependNode );
	for ( ; !it.isDone(); it.next() ) // Iterate through all selected objects
	{
		MDagPath path;
		it.getDagPath( path );
		// Found ? Return result of search
		setResult( cache.find( path.fullPathName().asChar() ) != cache.end() );
		return MStatus::kSuccess;	
	}
	return MStatus::kSuccess;	
}

MStatus RenderManCacheCommand::list( const MArgDatabase & aArgDatabase )
{
	MStringArray arr;
	// For every cache item
	for ( Cache::const_iterator it = cache.begin(); it != cache.end(); ++it )
	{
		// Write stored node name
		arr.append( MString(it->first.c_str()) );
	}
	// Return list of names
	setResult( arr );
	return MStatus::kSuccess;
}


} // namespace RibExport

} // namespace Stubble
