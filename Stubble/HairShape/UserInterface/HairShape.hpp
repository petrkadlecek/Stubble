#ifndef STUBBLE_HAIR_SHAPE_HPP
#define STUBBLE_HAIR_SHAPE_HPP

#include <maya/MStatus.h>
#include <maya/MBoundingBox.h>
#include <maya/MPxSurfaceShape.h>

namespace Stubble
{

namespace HairShape
{

///----------------------------------------------------------------------------------------------------
/// The main class that encapsulates informations about hairs
///----------------------------------------------------------------------------------------------------
class HairShape: public MPxSurfaceShape
{
public:
	HairShape();

	///----------------------------------------------------------------------------------------------------
	/// Reacts on attribute changes, mesh changes, etc.
	///----------------------------------------------------------------------------------------------------
	MStatus compute();

	///----------------------------------------------------------------------------------------------------
	/// Getter for bounding box of HairShape.
	///----------------------------------------------------------------------------------------------------
	virtual bool isBounded() const;

	///----------------------------------------------------------------------------------------------------
	/// This function calculates and returns Node volume.
	///
	/// \return The bounding box of the shape
	///----------------------------------------------------------------------------------------------------
	virtual MBoundingBox boundingBox() const;

	/// TODO - comment
	virtual MStatus compute( const MPlug &plug, MDataBlock &dataBlock );

	///----------------------------------------------------------------------------------------------------
	/// Function draws guide hairs and also interporated hairs.
	///----------------------------------------------------------------------------------------------------
	void draw();

	///----------------------------------------------------------------------------------------------------
	/// Selecting hairs that calls applying selection on HairGuids.
	///----------------------------------------------------------------------------------------------------
	void select();

	///----------------------------------------------------------------------------------------------------
	/// Creates new HairShape.
	///----------------------------------------------------------------------------------------------------
	static void *creator();

	///----------------------------------------------------------------------------------------------------
	/// Calling updating guides on HairGuids.
	///----------------------------------------------------------------------------------------------------
	void updateGuides();

	///----------------------------------------------------------------------------------------------------
	/// This function creates a description of our node.
	///----------------------------------------------------------------------------------------------------
    static MStatus initialize();

private:


};

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_SHAPE_HPP