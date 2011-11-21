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
	getRotationAxis(aTask->mMousePos[ 0 ], aTask->mMousePos[ 1 ], aTask->mView, axis, position);
	Matrix< Real > R, T, Tinv;  // Rotation matrix, translation matrix to the origin and its inverse

	// For each guide
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aTask->mAffectedGuides->begin(); it != aTask->mAffectedGuides->end(); ++it)
	{
		// For each p of the guide: p' = T^-1 * R * T * p
		HairShape::HairComponents::SelectedGuide *guide = *it; // Guide alias
		HairShape::HairComponents::Segments &hairVertices = guide->mGuideSegments.mSegments; // Local alias
		const size_t SEGMENT_COUNT = hairVertices.size();
		Vector3D< Real > axisLocal = Vector3D< Real >::transform(axis, guide->mPosition.mLocalTransformMatrix); // Local coordinate axis
		Vector3D< Real > positionLocal = Vector3D< Real >::transformPoint(position, guide->mPosition.mLocalTransformMatrix); // Local coordinate position

		getTranslationMatrices( positionLocal, T, Tinv );
		Matrix< Real > A; // A = Tinv * R * T => p' = A * p
		if ( !mEnableFalloff )
		{
			R = getRotationMatrix( aTask->mDx.x, axisLocal );
			A = Tinv * (R * T); // Note that * is left associative
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
				A = Tinv * (R * T); // Note that * is left associative
			}
			
			hairVertices[ i ] = Vector3D< Real >::transformPoint(hairVertices[ i ], A);
		}

		guide->mDirtyFlag = true;
		guide->mDirtyRedrawFlag = true;
	} // For each guide
}

Matrix< Real > RotateBrushMode::getRotationMatrix ( Real aMeasure, const Vector3D< Real > &aAxis )
{
	// R = uu^T + (cos theta)(I - uu^T) + (sin theta)S, see OpenGL Red Book 2.0, appendix F
	// u is unit vector of the axis, S is 3x3 matrix as defined bellow
	Real theta = 3.14 * aMeasure;
	const Real cosT = cos(theta); // Cosine theta
	const Real sinT = sin(theta); // Sine theta
	Matrix< Real > M1, M2, S;

	// M1 = uu^T
	// First column
	M1[ 0 ] = aAxis.x * aAxis.x;
	M1[ 1 ] = aAxis.x * aAxis.y;
	M1[ 2 ] = aAxis.x * aAxis.z;
	// Second column
	M1[ 4 ] = aAxis.y * aAxis.x;
	M1[ 5 ] = aAxis.y * aAxis.y;
	M1[ 6 ] = aAxis.y * aAxis.z;
	// Third column
	M1[ 8 ] = aAxis.z * aAxis.x;
	M1[ 9 ] = aAxis.z * aAxis.y;
	M1[ 10 ] = aAxis.z * aAxis.z;

	// M2 = (cos theta)(I - M1)
	// First column
	M2[ 0 ] = cosT * (1.0 - M1[ 0 ]);
	M2[ 1 ] = -cosT * M1[ 1 ];
	M2[ 2 ] = -cosT * M1[ 2 ];
	// Second column
	M2[ 4 ] = -cosT * M1[ 4 ];
	M2[ 5 ] = cosT * (1.0 - M1[ 5 ]);
	M2[ 6 ] = -cosT * M1[ 6 ];
	// Third column
	M2[ 8 ] = -cosT * M1[ 8 ];
	M2[ 9 ] = -cosT * M1[ 9 ];
	M2[ 10 ] = cosT * (1.0 - M1[ 10 ]);

	// S matrix
	// First column
	S[ 1 ] = sinT * aAxis.z;
	S[ 2 ] = -sinT * aAxis.y;
	// Second column
	S[ 4 ] = -sinT * aAxis.z;
	S[ 6 ] = sinT * aAxis.x;
	// Third column
	S[ 8 ] = sinT * aAxis.y;
	S[ 9 ] = -sinT * aAxis.x;
	
	Matrix<Real> R = M1 + M2 + S;
	R[ 15 ] = 1.0; // Make it proper homogenic 4x4 matrix
	return R;
}

void RotateBrushMode::getRotationAxis ( short aX, short aY, M3dView &aView, Vector3D< Real > &aAxis, Vector3D< Real>  &aPosition )
{
	// Obtain the camera information
	//MDagPath cameraPath;
	//aView.getCamera(cameraPath);
	//MFnCamera camera(cameraPath);

	// Get the viewing vector in world coordinates
	MStatus status;
	//MVector view = camera.viewDirection(MSpace::kWorld, &status);
	// Get the cursor position in world coordinates
	MPoint pos;
	MVector dir;
	status = aView.viewToWorld(aX, aY, pos, dir);
	
	Vector3D< Real > axis(dir.x, dir.y, dir.z);
	axis.normalize();
	Vector3D< Real > position(pos.x, pos.y, pos.z);

	aAxis = axis;
	aPosition = position;
}

void RotateBrushMode::getTranslationMatrices ( const Vector3D< Real > &aPosition, Matrix< Real > &aToOrigin, Matrix< Real > &aFromOrigin)
{
	// Transposed translate matrices because of the left-side multiplication
	Matrix< Real > T;
	T[ 0 ] = 1.0;
	T[ 5 ] = 1.0;
	T[ 10 ] = 1.0;
	T[ 3 ] = aPosition.x;
	T[ 7 ] = aPosition.y;
	T[ 11 ] = aPosition.z;
	T[ 15 ] = 1.0;

	Matrix< Real > Tinv(T);
	Tinv[ 3 ] = -Tinv[ 3 ];
	Tinv[ 7 ] = -Tinv[ 7 ];
	Tinv[ 11 ] = -Tinv[ 11 ];

	aToOrigin = T;
	aFromOrigin = Tinv;
}


} // namespace Toolbox

} // namespace Stubble