/*
To run RenderManCacheCommand properly, environment variables STUBBLE_WORKDIR and DL_PROCEDURALS_PATH must be set !!!!
*/
#ifndef STUBBLE_RENDERMAN_CACHE_COMMAND_HPP
#define STUBBLE_RENDERMAN_CACHE_COMMAND_HPP

#include "CachedFrame.hpp"

#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

#include <map>
#include <string>

namespace Stubble
{

namespace RibExport
{

///-------------------------------------------------------------------------------------------------
/// RenderMan cache command class. 
///-------------------------------------------------------------------------------------------------
class RenderManCacheCommand : public MPxCommand
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Executes the cache command.
	///
	/// \param	aArgumentsList	List of the arguments. 
	///
	/// \return	status of the execution.
	///-------------------------------------------------------------------------------------------------
	virtual MStatus doIt( const MArgList & aArgumentsList );

	///-------------------------------------------------------------------------------------------------
	/// Creates cache command class. 
	///
	/// \return	pointer to cache command class.
	///-------------------------------------------------------------------------------------------------
	static void *creator();
	
private:

	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	RenderManCacheCommand();

	/* Internal commands follow ...*/

	///-------------------------------------------------------------------------------------------------
	/// Syntax : cache_command -addStep -sampleTime [double] [shape]
	/// The command is expected to keep a sample of the specified object at the current time, 
	/// which is passed via the -sampleTime flag; the command can assume that Maya's current time is 
	/// already set to this value when it is called. The command should store a combination 
	/// of the object's name, topology, sample time. No return value is expected.
	///
	/// \param	aArgDatabase	The argument database. 
	///
	/// \return	status of the execution. 
	///-------------------------------------------------------------------------------------------------
	MStatus sampleTime( const MArgDatabase & aArgDatabase );

	///-------------------------------------------------------------------------------------------------
	/// Syntax : cache_command -remove [shape]
	/// Removes the specified object from the cache. No return value is expected.
	///
	/// \param	aArgDatabase	The argument database. 
	///
	/// \return	status of the execution.  
	///-------------------------------------------------------------------------------------------------
	MStatus remove( const MArgDatabase & aArgDatabase );

	///-------------------------------------------------------------------------------------------------
	/// Syntax : cache_command -emit [shape]
	/// Issues the Ri calls that will go inside the ObjectBegin/End or ArchiveBegin/End block 
	/// for the specified object. If the object can be deformation blurred, it should produce the proper 
	/// motion blocks. 
	/// No return value is expected.
	///
	/// \param	aArgDatabase	The argument database. 
	///
	/// \return	status of the execution.  
	///-------------------------------------------------------------------------------------------------
	MStatus emit( const MArgDatabase & aArgDatabase );

	///-------------------------------------------------------------------------------------------------
	/// Syntax : cache_command -flush
	/// Clear all cached data. No return value is expected.
	///
	/// \param	aArgDatabase	The argument database. 
	///
	/// \return	status of the execution.  
	///-------------------------------------------------------------------------------------------------
	MStatus flush( const MArgDatabase & aArgDatabase );

	///-------------------------------------------------------------------------------------------------
	/// Syntax : cache_command -contains [shape]
	/// Returns true if cache contains given shape.
	///
	/// \param	aArgDatabase	The argument database. 
	///
	/// \return	status of the execution.  
	///-------------------------------------------------------------------------------------------------
	MStatus contains( const MArgDatabase & aArgDatabase );
	
	///-------------------------------------------------------------------------------------------------
	/// Syntax : cache_command -list
	/// Return the names of the shapes that have been cached by this command in a string array.
	///
	/// \param	aArgDatabase	The argument database. 
	///
	/// \return	status of the execution.  
	///-------------------------------------------------------------------------------------------------
	MStatus list( const MArgDatabase & aArgDatabase );
	
	MSyntax syntax; ///< The syntax of the command

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing the cache .
	///-------------------------------------------------------------------------------------------------
	typedef std::map< std::string, CachedFrame *> Cache;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing the cache item .
	///-------------------------------------------------------------------------------------------------
	typedef std::pair< std::string, CachedFrame *> CacheItem;

	static Cache cache; ///< The cache (must be stored for all RenderManCacheCommands)
};

} // namespace RibExport

} // namespace Stubble

#endif // STUBBLE_RENDERMAN_CACHE_COMMAND_HPP