#include "RotateBrushMode.hpp"
#include "Toolbox/HairTask.hpp"

#include <fstream> //TODO: remove me

namespace Stubble
{

namespace Toolbox
{

void dumpToFile ( Matrix< Real > &M ) //TODO: remove me
{
	std::fstream log("C:\\Dev\\matrix.log", std::ios_base::app);
	log.setf(std::ios::fixed, std::ios::floatfield);
	log.precision(4);
	for (Uint i = 0; i < 4; ++i)
	{
		for (Uint j = 0; j < 4; ++j)
		{
			log << M[ i + 4 * j ]<< '\t';
		}
		log << "\n";
	}
	log << "\n" << std::flush;
	log.close();
}

void RotateBrushMode::doBrush ( HairTask *aTask )
{
	Matrix< Real > R = getRotationMatrix( aTask->mDx.x, aTask->mView ); // Rotation matrix
	Matrix< Real > T, Tinv; // Translation matrix to the origin and its inverse
	getTranslationMatrices( aTask->mView, T, Tinv);

	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aTask->mAffectedGuides->begin(); it != aTask->mAffectedGuides->end(); ++it)
	{
		// Apply transform: p' = L * T^-1 * R * T * W * p; where L and W are local and world transformations
		// respectively, T and T^-1 are rotation axis to and from origin respectively, R is the rotation
		// transformation. Let A1 be L * T^-1 and A2 be T * W
		HairShape::HairComponents::SelectedGuide *guide = *it; // Guide alias
		const size_t SEGMENT_COUNT = guide->mGuideSegments.mSegments.size();
		Matrix< Real > A1 = Tinv * guide->mPosition.mLocalTransformMatrix;
		Matrix< Real > A2 = guide->mPosition.mWorldTransformMatrix * T;
		Matrix< Real > A; // A = A1 * R * A2
		if ( !mEnableFalloff )
		{
			A = A2 * R * A1; // Note that * is left associative
		}

		// Loop through all guide segments except the first one (that's a follicle and should not move)
		for (size_t i = 1; i < SEGMENT_COUNT; ++i)
		{
			if ( !guide->mSegmentsAdditionalInfo[ i ].mInsideBrush )
			{
				continue;
			}
			if ( mEnableFalloff )
			{
				R = getRotationMatrix( aTask->mDx.x * guide->mSegmentsAdditionalInfo[ i ].mFallOff, aTask->mView );
				A = A2 * R * A1; // Note that * is left associative
			}
			
			guide->mGuideSegments.mSegments[ i ] = Vector3D< Real >::transformPoint(guide->mGuideSegments.mSegments[ i ], A);
		}

		guide->mDirtyFlag = true;
		guide->mDirtyRedrawFlag = true;
	}
}

Matrix< Real > RotateBrushMode::getRotationMatrix ( Real aMeasure, M3dView &aView )
{
	Real theta = 6.28 * aMeasure;
	// R = cos a * I + (1 - cos a) * A1 + sin a * A2
	const Real cosA = cos(theta); // Cosine alpha
	const Real cCosA = 1.0 - cosA; // Cosine alpha complement
	const Real sinA = sin(theta); // Sine alpha
	Matrix< Real > IcosA, A1, A2; // Unit matrix multiplied by cos a and two matrices containing axis of rotation
	Vector3D< Real > a = getRotationAxis( aView ); // Rotation axis

	// Unit matrix multiplied by cos alpha
	IcosA[ 0 ] = cosA;
	IcosA[ 5 ] = cosA;
	IcosA[ 10 ] = cosA;
	IcosA[ 15 ] = cosA;

	// First column
	A1[ 0 ] = a.x * a.x * cCosA;
	A1[ 1 ] = a.x * a.y * cCosA;
	A1[ 2 ] = a.x * a.z * cCosA;
	// Second column
	A1[ 4 ] = a.y * a.x * cCosA;
	A1[ 5 ] = a.y * a.y * cCosA;
	A1[ 6 ] = a.y * a.z * cCosA;
	// Third column
	A1[ 8 ] = a.z * a.x * cCosA;
	A1[ 9 ] = a.z * a.y * cCosA;
	A1[ 10 ] = a.z * a.z * cCosA;
	// Fourth column
	A1[ 15 ] = cCosA;
	
	// First column
	A2[ 1 ] = a.z * sinA;
	A2[ 2 ] = -a.y * sinA;
	// Second column
	A2[ 4 ] = -a.z * sinA;
	A2[ 6 ] = a.x * sinA;
	// Third column
	A2[ 8 ] = a.y * sinA;
	A2[ 9 ] = -a.x * sinA;

	//dumpToFile(IcosA);
	//dumpToFile(A1);
	//dumpToFile(A2);

	Matrix<Real> R = IcosA + A1 + A2;
	return R;
}

Vector3D< Real > RotateBrushMode::getRotationAxis( M3dView &aView )
{
	// Obtain the camera information
	MDagPath cameraPath;
	aView.getCamera(cameraPath);
	MFnCamera camera(cameraPath);

	// Get the viewing vector in world coordinates
	MStatus status;
	MVector view = camera.viewDirection(MSpace::kWorld, &status);
	
	Vector3D< Real > axis(view.x, view.y, view.z);
	axis.normalize();

	return axis;
}

void RotateBrushMode::getTranslationMatrices ( M3dView &aView, Matrix< Real > &aToOrigin, Matrix< Real > &aFromOrigin)
{
	// Obtain the camera information
	MDagPath cameraPath;
	aView.getCamera(cameraPath);
	MFnCamera camera(cameraPath);

	// Get the camera eye point
	MStatus status;
	MPoint eye = camera.eyePoint(MSpace::kWorld, &status);

	Matrix< Real > T;
	T[ 0 ] = 1.0;
	T[ 5 ] = 1.0;
	T[ 10 ] = 1.0;
	T[ 12 ] = -eye.x;
	T[ 13 ] = -eye.y;
	T[ 14 ] = -eye.z;
	T[ 15 ] = 1.0;

	Matrix< Real > Tinv(T);
	Tinv[ 12 ] = -Tinv[ 12 ];
	Tinv[ 13 ] = -Tinv[ 13 ];
	Tinv[ 14 ] = -Tinv[ 14 ];

	aToOrigin = T;
	aFromOrigin = Tinv;
}

} // namespace Toolbox

} // namespace Stubble