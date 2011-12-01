#ifndef STUBBLE_HAIR_SHAPE_HPP
#define STUBBLE_HAIR_SHAPE_HPP

#include "HairShape/Generators/UVPointGenerator.hpp"
#include "HairShape/Interpolation/InterpolatedHair.hpp"
#include "HairShape/Interpolation/MayaHairProperties.hpp"
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
class HairShape: public MPxSurfaceShape, public Interpolation::MayaHairProperties 
{
public:
	
	static const MTypeId typeId; ///< The unique type ID of our custom node

	static const MString typeName;  ///< The unique type name of our custom node

	// handles to the attributes added to the node

	static MObject countAttr; ///< The guides count attribute

	static MObject genCountAttr; ///< The generated hair count attribute

	static MObject surfaceAttr; ///< The connected surface attribute

	static MObject surfaceChangeAttr; ///< The surface changed attribute

	static MObject voxelsResolutionAttr; ///< The voxels resolution attribute

	static MObject voxelsXResolutionAttr;   ///< The voxels x coordinate resolution attribute

	static MObject voxelsYResolutionAttr;   ///< The voxels y coordinate resolution attribute

	static MObject voxelsZResolutionAttr;   ///< The voxels z coordinate resolution attribute

	static MObject timeAttr;	///< The time attribute

	static MObject timeChangeAttr; ///< The time changed attribute

	static MObject displayGuidesAttr;   ///< Should guides be displayed ? attribute

	static MObject displayInterpolatedAttr; ///< Should interpolated hair be displayed ? attribute

	static MObject genDisplayCountAttr;	///< The number of generated hair to be displayed attribute

	static MObject sampleTextureDimensionAttr; ///< Texture dimension for sampling attribute

	static MObject serializedDataAttr; ///< Serialized plugin data (used for scene load/save)

	static MObject operationCountAttr; ///< Notify Maya that plugin data have changed

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
	/// Reacts on braking connection to our node.
	///
	/// \param aPlug				Which plug lost his connection.
	/// \param aOtherPlug			Plug on the other side of the broken connection.
	/// \param aAsSrc		Determines if this plug is source.
	///
	/// \return Maya status code.
	///----------------------------------------------------------------------------------------------------
	virtual MStatus connectionBroken( const MPlug & aPlug, const MPlug & aOtherPlug,bool aAsSrc );

	///----------------------------------------------------------------------------------------------------
	/// Creates new HairShape.
	/// 
	/// \return	New Hair Shape as void pointer.
	///----------------------------------------------------------------------------------------------------
	static void *creator();

	///----------------------------------------------------------------------------------------------------
	/// Draw method called my Maya
	///----------------------------------------------------------------------------------------------------
	void draw();

	///----------------------------------------------------------------------------------------------------
	/// Gets the selected guides segments uniform grid. This grid is only updated after selection or on 
	/// demand and it's dirty flag is never set by HairGuides class.
	///
	/// \return	The selected guides segments uniform grid. 
	///----------------------------------------------------------------------------------------------------
	inline const HairComponents::SegmentsUG & getSelectedGuidesUG();

	///----------------------------------------------------------------------------------------------------
	/// Updates the guides after the brush or any other tool was used. 
	/// 
	/// \param	aStoreUpdate	if true, stores updates of selected guides to stack, 
	/// 						otherwise draw update only
	///----------------------------------------------------------------------------------------------------
	inline void updateGuides( bool aStoreUpdate );

	///----------------------------------------------------------------------------------------------------
	/// Undoes changed to hair guides.
	///----------------------------------------------------------------------------------------------------
	void undo();

	///----------------------------------------------------------------------------------------------------
	/// Redoes changes to hair guides.
	///----------------------------------------------------------------------------------------------------
	void redo();

	///-------------------------------------------------------------------------------------------------
	/// Queries if we can undo. 
	///
	/// \return	true if it succeeds, false if it fails. 
	///-------------------------------------------------------------------------------------------------
	inline bool canUndo() const;

	///-------------------------------------------------------------------------------------------------
	/// Queries if we can redo. 
	///
	/// \return	true if it succeeds, false if it fails. 
	///-------------------------------------------------------------------------------------------------
	inline bool canRedo() const;

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
	/// Refresh all dirty textures. 
	/// 
	/// \param aForceRefresh	Should we refresh all textures ?
	///----------------------------------------------------------------------------------------------------
	void refreshTextures( bool aForceRefresh = false );

	///----------------------------------------------------------------------------------------------------
	/// Sets this HairShape as active object. 
	///----------------------------------------------------------------------------------------------------
	inline void setAsActiveObject();

	// Static methods
	
	///----------------------------------------------------------------------------------------------------
	/// Removes the callbacks. Should be called shortly before HairShape deregistration. 
	///----------------------------------------------------------------------------------------------------
	static void removeCallbacks();

	///----------------------------------------------------------------------------------------------------
	/// Gets the active object node. 
	///
	/// \return	null if it fails, else the active object. 
	///----------------------------------------------------------------------------------------------------
	inline static HairShape * getActiveObject();

	///----------------------------------------------------------------------------------------------------
	/// Import NURBS.
	///----------------------------------------------------------------------------------------------------
	inline void importNURBS();

	///----------------------------------------------------------------------------------------------------
	/// Export to NURBS.
	///----------------------------------------------------------------------------------------------------
	inline void exportToNURBS();

	///-------------------------------------------------------------------------------------------------
	/// Reinits all cutted hair with zero length to initial length multiplied by scale factor.
	///-------------------------------------------------------------------------------------------------
	inline void reinitCuttedHair();

	///----------------------------------------------------------------------------------------------------
	/// Gets the currect maya mesh.
	/// 
	/// \return	Current maya mesh.
	///----------------------------------------------------------------------------------------------------
	inline const MayaMesh & getCurrentMesh() const;
		
	///-------------------------------------------------------------------------------------------------
	/// Serialize plugin data.
	/// 
	/// \return	Serialized Hair Shape.
	///-------------------------------------------------------------------------------------------------
	std::string serialize() const;

	///-------------------------------------------------------------------------------------------------
	/// Deserialize and reconstruct plugin data.
	/// 
	/// \param	Serialized Hair Shape.
	///-------------------------------------------------------------------------------------------------
	void deserialize( const std::string & aData );

	///----------------------------------------------------------------------------------------------------
	/// Gets the current maya mesh inclusive matrix
	/// 
	/// \return	Get current maya mesh inclusive matrix.
	///----------------------------------------------------------------------------------------------------
	inline MMatrix getCurrentInclusiveMatrix() const;

	///-------------------------------------------------------------------------------------------------
	/// Return this as MObject
	/// 
	/// \return	Return this Hair Shape as MObject.
	///-------------------------------------------------------------------------------------------------
	inline MObject asMObject();

private:
	
	friend class HairShapeUI;

	// Private methods

	///----------------------------------------------------------------------------------------------------
	/// Mesh has been changed. 
	///
	/// \param	aMeshObj	The mesh object.
	///----------------------------------------------------------------------------------------------------
	void meshChange( MObject aMeshObj );

	///----------------------------------------------------------------------------------------------------
	/// Sets a current time. 
	///
	/// \param	aTime	The current time. 
	///----------------------------------------------------------------------------------------------------
	void setCurrentTime( Time aTime );

	///----------------------------------------------------------------------------------------------------
	/// Refresh pointers to guides for interpolation class. 
	///----------------------------------------------------------------------------------------------------
	inline void refreshPointersToGuidesForInterpolation();

	///-------------------------------------------------------------------------------------------------
	/// Updates the segments count attributes. ( Essential after Interpolation groups change )
	/// 
	/// \param	aFirstUpdate	If first update is selected, then the old segments count don't need to 
	/// 						be removed.
	///-------------------------------------------------------------------------------------------------
	inline void updateSegmentsCountAttributes( bool aFirstUpdate );

	///----------------------------------------------------------------------------------------------------
	/// Registers the topology callback. 
	///
	/// \return Maya status code.
	///----------------------------------------------------------------------------------------------------
	MStatus registerTopologyCallback();

	// Inner objects 

	MBoundingBox mBoundingBox; ///< The bounding box of our node

	RandomGenerator mRandom;	///< The random generator

	UVPointGenerator *mUVPointGenerator; ///< UV point generator

	MayaMesh *mMayaMesh; ///< Maya mesh

	HairComponents::HairGuides *mHairGuides; ///< HairGuides compoment

	Voxelization * mVoxelization;   ///< The voxelization of rest pose mesh

	Interpolation::InterpolatedHair mInterpolatedHair;	///< The interpolated hair

	MDagPath mConnectedMeshPath; ///< Path to connected mesh object

	// Stored attributes values

	unsigned __int32 mGuidesHairCount;  ///< Number of guides hairs

	unsigned __int32 mGeneratedHairCount;	///< Number of generated hair

	Dimensions3 mVoxelsResolution;  ///< The voxels resolution

	Time mTime; ///< The current time

	bool mDisplayGuides;   ///< Should guides be displayed ?

	bool mDisplayInterpolated; ///< Should interpolated hair be displayed ?

	unsigned __int32 mGenDisplayCount;	///< The number of generated hair to be displayed

	unsigned __int32 mSampleTextureDimension; ///< Texture dimension for sampling attribute

	// Mesh topology callback

	bool mIsTopologyCallbackRegistered; ///< true if is topology callback registered

	bool mIsTopologyModified;   ///< true if is topology modified

	static MCallbackIdArray mCallbackIds;	///< List of identifiers for the callbacks

	// Active hair shape object

	static HairShape * mActiveHairShapeNode;   ///< The active node
};

// inline functions implementation

inline const HairComponents::SegmentsUG & HairShape::getSelectedGuidesUG()
{
	return mHairGuides->getSelectedGuidesUG();
}

inline void HairShape::updateGuides( bool aStoreUpdate )
{
	mHairGuides->updateGuides( aStoreUpdate );
	if ( mDisplayInterpolated && aStoreUpdate )
	{
		mInterpolatedHair.propertiesUpdate( *this );
	}
}

inline void HairShape::undo()
{
	mHairGuides->undo();
	if ( mDisplayInterpolated )
	{
		mInterpolatedHair.propertiesUpdate( *this );
	}
}

inline void HairShape::redo()
{
	mHairGuides->redo();
	if ( mDisplayInterpolated )
	{
		mInterpolatedHair.propertiesUpdate( *this );
	}
}

inline bool HairShape::canUndo() const
{
	return mHairGuides->canUndo();
}

inline bool HairShape::canRedo() const
{
	return mHairGuides->canRedo();
}

inline void HairShape::setAsActiveObject()
{
	mActiveHairShapeNode = this;
}

inline HairShape * HairShape::getActiveObject()
{
	return mActiveHairShapeNode;
}

inline void HairShape::importNURBS()
{
	mHairGuides->importNURBS( *mInterpolationGroups );
}

inline void HairShape::exportToNURBS()
{
	mHairGuides->exportToNURBS();
}

inline void HairShape::reinitCuttedHair()
{
	mHairGuides->reinitCuttedHair( getScaleFactor() );
}

inline const MayaMesh & HairShape::getCurrentMesh() const
{
	return *mMayaMesh;
}

inline MMatrix HairShape::getCurrentInclusiveMatrix() const
{
	return mConnectedMeshPath.inclusiveMatrix();
}

inline MObject HairShape::asMObject()
{
	return thisMObject();
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_HAIR_SHAPE_HPP