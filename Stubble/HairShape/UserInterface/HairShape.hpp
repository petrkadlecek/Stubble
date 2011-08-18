#ifndef STUBBLE_HAIR_SHAPE_HPP
#define STUBBLE_HAIR_SHAPE_HPP

#include "HairShape\Generators\UVPointGenerator.hpp"
#include "HairShape\HairComponents\HairGuides.hpp"
#include "HairShape\Mesh\MayaMesh.hpp"

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
	/// the unique type ID of our custom node
	static const MTypeId typeId;

	/// the unique type name of our custom node
	static const MString typeName;

	// handles to the attributes added to the node

	///The count attribute
	static MObject countAttr;

	///The generated count attribute
	static MObject genCountAttr;

	/// The length attribute
	static MObject lengthAttr;

	/// The surface attribute
	static MObject surfaceAttr;

	/// The segments count
	static MObject segmentsAttr;

	/// The redraw attribute
	static MObject redrawAttr;

	///< The density texture attribute
	static MObject densityTextureAttr;

	///< The interpolation groups texture attribute
	static MObject interpolationGroupsTextureAttr;

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

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Computes new attributes of our Node based on other updated attributes.
	///
	/// \param Node plug - which attribute was changed.
	/// \param [in,out] The data block with attributes values.
	///
	/// \return Maya status code.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual MStatus compute( const MPlug &plug, MDataBlock &dataBlock );

	///----------------------------------------------------------------------------------------------------
	/// Creates new HairShape.
	///----------------------------------------------------------------------------------------------------
	static void *creator();

	///----------------------------------------------------------------------------------------------------
	/// Draw method called my Maya
	///----------------------------------------------------------------------------------------------------
	void draw();

	///----------------------------------------------------------------------------------------------------
	/// Calling updating guides on HairGuids.
	///----------------------------------------------------------------------------------------------------
	void updateGuides();

	///----------------------------------------------------------------------------------------------------
	/// This function creates a description of our node.
	///----------------------------------------------------------------------------------------------------
    static MStatus initialize();

private:
	
	MBoundingBox mBoundingBox; ///< The bounding box of our node

	UVPointGenerator *mUVPointGenerator; ///< UV point generator

	MayaMesh *mMayaMesh; ///< Maya mesh

	HairComponents::HairGuides *mHairGuides; ///< HairGuides compoment

	UNKNOWN mInterpolatedHairs; ///< Interpolated hairs

};

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_SHAPE_HPP