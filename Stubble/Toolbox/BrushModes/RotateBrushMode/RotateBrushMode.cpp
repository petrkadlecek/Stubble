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
	Vector3D< Real> axis, position; // Axis of rotation and its position respectively
	getRotationAxis(aTask->mView, axis, position);
	Matrix< Real > R, T, Tinv;  // Rotation matrix, translation matrix to the origin and its inverse

	// For each guide
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aTask->mAffectedGuides->begin(); it != aTask->mAffectedGuides->end(); ++it)
	{
		// For each p of the guide: p' = T^-1 * R * T * p
		HairShape::HairComponents::SelectedGuide *guide = *it; // Guide alias
		Vector3D< Real > axisLocal = Vector3D< Real >::transform(axis, guide->mPosition.mLocalTransformMatrix); // Local coordinate axis
		Vector3D< Real > positionLocal = Vector3D< Real >::transformPoint(axis, guide->mPosition.mLocalTransformMatrix); // Local coordinate position
		const size_t SEGMENT_COUNT = guide->mGuideSegments.mSegments.size();

		getTranslationMatrices( positionLocal, T, Tinv );
		R = getRotationMatrix( aTask->mDx.x, axisLocal );
		Matrix< Real > A; // A = Tinv * R * T => p' = A * p
		if ( !mEnableFalloff )
		{
			A = T * R * Tinv; // Note that * is left associative
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
				R = getRotationMatrix( aTask->mDx.x * guide->mSegmentsAdditionalInfo[ i ].mFallOff, axisLocal );
				A = T * R * Tinv; // Note that * is left associative
			}
			
			guide->mGuideSegments.mSegments[ i ] = Vector3D< Real >::transformPoint(guide->mGuideSegments.mSegments[ i ], A);
		}

		guide->mDirtyFlag = true;
		guide->mDirtyRedrawFlag = true;
	} // For each guide
}

Matrix< Real > RotateBrushMode::getRotationMatrix ( Real aMeasure, const Vector3D< Real > &aAxis )
{
	Real theta = 6.28 * aMeasure;
	// R = cos a * I + (1 - cos a) * A1 + sin a * A2
	const Real cosA = cos(theta); // Cosine alpha
	const Real cCosA = 1.0 - cosA; // Cosine alpha complement
	const Real sinA = sin(theta); // Sine alpha
	Matrix< Real > IcosA, A1, A2; // Unit matrix multiplied by cos a and two matrices containing axis of rotation

	// Unit matrix multiplied by cos alpha
	IcosA[ 0 ] = cosA;
	IcosA[ 5 ] = cosA;
	IcosA[ 10 ] = cosA;
	IcosA[ 15 ] = 1.0;

	// First column
	A1[ 0 ] = aAxis.x * aAxis.x * cCosA;
	A1[ 1 ] = aAxis.x * aAxis.y * cCosA;
	A1[ 2 ] = aAxis.x * aAxis.z * cCosA;
	// Second column
	A1[ 4 ] = aAxis.y * aAxis.x * cCosA;
	A1[ 5 ] = aAxis.y * aAxis.y * cCosA;
	A1[ 6 ] = aAxis.y * aAxis.z * cCosA;
	// Third column
	A1[ 8 ] = aAxis.z * aAxis.x * cCosA;
	A1[ 9 ] = aAxis.z * aAxis.y * cCosA;
	A1[ 10 ] = aAxis.z * aAxis.z * cCosA;
	
	// First column
	A2[ 1 ] = aAxis.z * sinA;
	A2[ 2 ] = -aAxis.y * sinA;
	// Second column
	A2[ 4 ] = -aAxis.z * sinA;
	A2[ 6 ] = aAxis.x * sinA;
	// Third column
	A2[ 8 ] = aAxis.y * sinA;
	A2[ 9 ] = -aAxis.x * sinA;

	//dumpToFile(IcosA);
	//dumpToFile(A1);
	//dumpToFile(A2);
	
	Matrix<Real> R = IcosA + A1 + A2;
	//dumpToFile(R);
	return R;
}

void RotateBrushMode::getRotationAxis ( M3dView &aView, Vector3D< Real > &aAxis, Vector3D< Real>  &aPosition )
{
	// Obtain the camera information
	MDagPath cameraPath;
	aView.getCamera(cameraPath);
	MFnCamera camera(cameraPath);

	// Get the viewing vector in world coordinates
	MStatus status;
	MVector view = camera.viewDirection(MSpace::kWorld, &status);
	MPoint eye = camera.eyePoint(MSpace::kWorld, &status);
	
	Vector3D< Real > axis(view.x, view.y, view.z);
	axis.normalize();
	Vector3D< Real > position(eye.x, eye.y, eye.z);

	aAxis = axis;
	aPosition = position;
}

void RotateBrushMode::getTranslationMatrices ( const Vector3D< Real > &aPosition, Matrix< Real > &aToOrigin, Matrix< Real > &aFromOrigin)
{
	Matrix< Real > T;
	T[ 0 ] = 1.0;
	T[ 5 ] = 1.0;
	T[ 10 ] = 1.0;
	T[ 12 ] = -aPosition.x;
	T[ 13 ] = -aPosition.y;
	T[ 14 ] = -aPosition.z;
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