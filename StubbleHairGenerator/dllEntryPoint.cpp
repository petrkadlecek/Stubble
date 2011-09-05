/* This dll must be stored in DL_PROCEDURALS_PATH folder */
#include "ri.h"

#if defined ( _WIN32 )
#define DLLEXPORT __declspec( dllexport )
#else
#define DLLEXPORT
#endif
	
#ifdef __cplusplus
extern "C" {
#endif
	
/* Declarations */
RtPointer DLLEXPORT ConvertParameters( RtString aParamString );
RtVoid DLLEXPORT Subdivide( RtPointer aData, float aDetailSize );
RtVoid DLLEXPORT Free( RtPointer aData );
	
#ifdef __cplusplus
}
#endif

///-------------------------------------------------------------------------------------------------
/// Convert parameters to binary representation. 
///
/// \param	aParamString	The parameter in string format. 
///
/// \return	parameters in binary format. 
///-------------------------------------------------------------------------------------------------
RtPointer DLLEXPORT ConvertParameters( RtString aParamString )
{
	return 0;
}

///-------------------------------------------------------------------------------------------------
/// Subdivides procedural command to other renderman commands. 
///
/// \param	aData		Parameters in binary format. 
/// \param	aDetailSize	Size of a detail. 
///-------------------------------------------------------------------------------------------------
RtVoid DLLEXPORT Subdivide( RtPointer aData, RtFloat aDetailSize )
{
}

///-------------------------------------------------------------------------------------------------
/// Frees memory allocated by ConvertParameters . 
///
/// \param	aData	Parameters in binary format. 
///-------------------------------------------------------------------------------------------------
RtVoid DLLEXPORT Free( RtPointer aData )
{
}

