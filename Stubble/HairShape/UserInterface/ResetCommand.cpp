#include "ResetCommand.hpp"

namespace Stubble
{

namespace HairShape
{
	
void *ResetCommand::creator()
{
	return new ResetCommand();
}

MStatus ResetCommand::doIt( const MArgList &aArgList )
{
	try
	{
		HairShape * active = HairShape::getActiveObject();
		if ( active != 0 )
		{
			active->resetGuides();
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
