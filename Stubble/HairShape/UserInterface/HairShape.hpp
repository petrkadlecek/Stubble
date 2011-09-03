#ifndef STUBBLE_HAIR_SHAPE_HPP
#define STUBBLE_HAIR_SHAPE_HPP

#include "HairShape/Generators/UVPointGenerator.hpp"
#include "HairShape/HairComponents/HairGuides.hpp"
#include "HairShape/Mesh/MayaMesh.hpp"
#include "HairShape/Mesh/Voxelization.hpp"

#include <maya/MBoundingBox.h>
#include <maya/MCallbackIdArray.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MStatus.h>

#include <ostream>

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
	
	static const MTypeId typeId; ///< The unique type ID of our custom node

	static const MString typeName;  ///< The unique type name of our custom node

	// handles to the attributes added to the node

	static MObject countAttr; ///< The guides count attribute

	static MObject genCountAttr; ///< The generated hair count attribute

	static MObject surfaceAttr; ///< The connected surface attribute

	static MObject segmentsAttr; ///< The segments attribute

	static MObject surfaceChangeAttr; ///< The surface changed attribute

	static MObject densityTextureAttr; ///< The density texture attribute

	static MObject interpolationGroupsTextureAttr; ///< The interpolation groups texture attribute

	static MObject voxelsResolutionAttr; ///< The voxels resolution attribute

	static MObject timeAttr;	///< The time attribute

	///----------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///----------------------------------------------------------------------------------------------------
	HairShape();

	///----------------------------------------------------------------------------------------------------
	/// Post constructor. 
	///----------------------------------------------------------------------------------------------------
	virtual void HairShape::postConstructor();

	///----------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///----------------------------------------------------------------------------------------------------
	~HairShape();

	///----------------------------------------------------------------------------------------------------
	/// Reacts on attribute changes, mesh changes, etc.
	/// 
	/// \return Maya status code.
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

	///----------------------------------------------------------------------------------------------------
	/// Computes new attributes of our Node based on other updated attributes.
	///
	/// \param aPlug				Which attribute was changed.
	/// \param [in,out] aDataBlock	The data block with attributes values.
	///
	/// \return Maya status code.
	///----------------------------------------------------------------------------------------------------
	virtual MStatus compute( const MPlug &aPlug, MDataBlock &aDataBlock );

	///----------------------------------------------------------------------------------------------------
	/// Gets an internal value in contex. 
	///
	/// \param	aPlug				Which attribute is being quiered.
	/// \param [in,out]	aDataHandle	Handle to data storage.
	/// \param [in,out]	aContext	Current context; 
	///
	/// \return	true if value was loaded from user storage, false if it should be loaded defaultly.
	///----------------------------------------------------------------------------------------------------
	virtual bool getInternalValueInContext( const MPlug & aPlug, MDataHandle & aDataHandle, MDGContext & aContext );  

	///----------------------------------------------------------------------------------------------------
	/// Sets an internal value in contex. 
	///
	/// \param	aPlug				Which attribute is being set.
	/// \param [in,out]	aDataHandle	Handle to data storage.
	/// \param [in,out]	aContext	Current context; 
	///
	/// \return	true if value was saved in user storage, false if it should be stored defaultly.
	///----------------------------------------------------------------------------------------------------
	virtual bool setInternalValueInContext( const MPlug & aPlug, const MDataHandle & aDataHandle, 
		MDGContext & aContext );
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
	/// 
	/// \return Maya status code.
	///----------------------------------------------------------------------------------------------------
    static MStatus initialize();

	///-------------------------------------------------------------------------------------------------
	/// Sample HairShape at given time. 
	///
	/// \param	aSampleTime					Time of the sample. 
	/// \param	aFileName					Filename of the file. 
	/// \param [in,out]	aVoxelBoundingBoxes	The voxel bounding boxes. 
	///-------------------------------------------------------------------------------------------------
	void sampleTime( Time aSampleTime, const std::string & aFileName, BoundingBoxes & aVoxelBoundingBoxes );

	///----------------------------------------------------------------------------------------------------
	/// Removes the callbacks. Should be called shortly before HairShape deregistration. 
	///----------------------------------------------------------------------------------------------------
	static void removeCallbacks();

private:
	
	// Private methods

	///----------------------------------------------------------------------------------------------------
	/// Mesh has been changed. 
	///
	/// \param	aMeshObj	The mesh object.
	///----------------------------------------------------------------------------------------------------
	void meshChange( MObject aMeshObj );

	///----------------------------------------------------------------------------------------------------
	/// Refresh all dirty textures. 
	///----------------------------------------------------------------------------------------------------
	void refreshTextures();

	///----------------------------------------------------------------------------------------------------
	/// Sets a current time. 
	///
	/// \param	aTime	The curren time. 
	///----------------------------------------------------------------------------------------------------
	void setCurrentTime( Time aTime );

	///----------------------------------------------------------------------------------------------------
	/// Export textures to file. 
	///
	/// \param [in,out]	aOutputStream	The output stream. 
	///----------------------------------------------------------------------------------------------------
	void exportTextures( std::ostream & aOutputStream ) const;

	///----------------------------------------------------------------------------------------------------
	/// Registers the topology callback. 
	///
	/// \return Maya status code.
	///----------------------------------------------------------------------------------------------------
	MStatus registerTopologyCallback();

	// Inner objects 

	MBoundingBox mBoundingBox; ///< The bounding box of our node

	UVPointGenerator *mUVPointGenerator; ///< UV point generator

	MayaMesh *mMayaMesh; ///< Maya mesh

	HairComponents::HairGuides *mHairGuides; ///< HairGuides compoment

	UNKNOWN *mInterpolatedHairs; ///< Interpolated hairs

	Interpolation::InterpolationGroups * mInterpolationGroups;  ///< Interpolation groups object

	Voxelization * mVoxelization;   ///< The voxelization of rest pose mesh

	// Stored attributes values

	unsigned __int32 mGuidesHairCount;  ///< Number of guides hairs

	unsigned __int32 mGeneratedHairCount;	///< Number of generated hair

	Texture * mDensityTexture;  ///< The density texture

	Texture * mInterpolationGroupsTexture;  ///< The interpolation groups texture

	Dimensions3 mVoxelsResolution;  ///< The voxels resolution

	Time mTime; ///< The current time

	// Mesh topology callback

	bool mIsTopologyCallbackRegistered; ///< true if is topology callback registered

	bool mIsTopologyModified;   ///< true if is topology modified

	static MCallbackIdArray mCallbackIds;	///< List of identifiers for the callbacks

};

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_SHAPE_HPP