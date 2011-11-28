#include "ReinitCommand.hpp"

namespace Stubble
{

namespace HairShape
{
	
void *ReinitCommand::creator()
{
	return new ReinitCommand();
}

MStatus ReinitCommand::doIt( const MArgList &aArgList )
{
	try 
	{
		HairShape * active = HairShape::getActiveObject();
		if ( active != 0 )
		{
			active->reinitCuttedHair();
			return MStatus::kSuccess;	
		}
	}
	catch( const StubbleException & ex )
	{
		MStatus s;
		s.perror( ex.what() );
		return s;
	}
	return MStatus::kFailure;
}

} // namespace HairShape

} // namespace Stubble