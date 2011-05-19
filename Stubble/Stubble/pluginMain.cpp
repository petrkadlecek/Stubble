#include <maya\MFnPlugin.h>

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
	MStatus   status;
	MFnPlugin plugin( aObj, "The Stubble Team", "1.0", "Any" );

	// Add plug-in feature registration here

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
	MStatus   status;
	MFnPlugin plugin( aObj );

	// Add plug-in feature deregistration here

	return status;
}