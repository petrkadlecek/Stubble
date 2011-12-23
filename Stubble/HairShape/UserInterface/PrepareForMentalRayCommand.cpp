#include "PrepareForMentalRayCommand.hpp"

namespace Stubble
{

namespace HairShape
{

void *PrepareForMentalRayCommand::creator()
{
	return new PrepareForMentalRayCommand();
}

MStatus PrepareForMentalRayCommand::doIt( const MArgList &aArgList )
{
	try
	{
		HairShape * active = HairShape::getActiveObject();
		if ( active != 0 )
		{
			// Set voxelization to 1x1x1
			Dimensions3 oldVoxelsResolution, newVoxelsResolution = { 1, 1, 1 };
			active->setVoxelsResolution( newVoxelsResolution, oldVoxelsResolution );

			// Get Stubble directory (needs environment variable to be set)
			std::string stubbleWorkDir = Stubble::getEnvironmentVariable("STUBBLE_WORKDIR") + "\\";
			// Take a sample
			BoundingBoxes dummyBoundingBoxes;
			active->sampleTime( active->getCurrentTime(), stubbleWorkDir + "stubble_mr_hair", dummyBoundingBoxes );

			// Restore the old voxelization
			active->setVoxelsResolution(oldVoxelsResolution, newVoxelsResolution);

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
