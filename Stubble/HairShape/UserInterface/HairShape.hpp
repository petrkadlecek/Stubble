#ifndef STUBBLE_HAIR_SHAPE_HPP
#define STUBBLE_HAIR_SHAPE_HPP

#include "HairShape/Mesh/MayaMesh.hpp"
#include "HairShape/Generators/UVPointGenerator.hpp"
#include "HairShape/HairComponents/HairGuides.hpp"
#include "HairShape/Interpolation/Maya/InterpolatedHair.hpp"
#include "HairShape/Interpolation/Maya/MayaHairProperties.hpp"
#include "HairShape/Interpolation/Maya/Voxelization.hpp"

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
/// The main class that encapsulates informations about hair guides and interpolated hair in Maya.
/// Can modify, save & load, export hair guides or interpolation hair properties.
/// Both hair guides and interpolated hair can be displayed directly in Maya through this class.
/// It can be serialized/deserialized.
///----------------------------------------------------------------------------------------------------
class HairShape: public MPxSurfaceShape, public Interpolation::Maya::MayaHairProperties 
{
public:
	
	static const unsigned int MAX_GUIDE_SEGMENTS_COUNT = 30; ///< Maximum allowed segments per guide

	static const MTypeId typeId; ///< The unique type ID of our custom node

	static const MString typeName;  ///< The unique type name of our custom node

	// handles to the attributes added to the node

	static MObject countAttr; ///< The guides count attribute

	static MObject genCountAttr; ///< The generated hair count attribute

	static MObject surfaceAttr; ///< The connected surface attribute

	static MObject surfaceChangeAttr; ///< The surface changed attribute ( set whenever surface is changed )

	static MObject voxelsResolutionAttr; ///< The voxels resolution attribute

	static MObject voxelsXResolutionAttr;   ///< The voxels x coordinate resolution attribute

	static MObject voxelsYResolutionAttr;   ///< The voxels y coordinate resolution attribute

	static MObject voxelsZResolutionAttr;   ///< The voxels z coordinate resolution attribute

	static MObject timeAttr;	///< The time attribute

	static MObject timeChangeAttr; ///< The time changed attribute ( set whenever time is changed )

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
	/// Some Maya properties may only be set in postConstructor.
	///----------------------------------------------------------------------------------------------------
	virtual void HairShape::postConstructor();

	///----------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///----------------------------------------------------------------------------------------------------
	~HairShape();

	///----------------------------------------------------------------------------------------------------
	/// Reacts on non-internal attribute changes.
	/// Called whenever surface on which hair grow has been changed or time has been changed.
	/// 
	/// \return Maya status code.
	///----------------------------------------------------------------------------------------------------
	MStatus compute();

	///----------------------------------------------------------------------------------------------------
	/// Returns always true, telling Maya that are object is always inside bounding box.
	///----------------------------------------------------------------------------------------------------
	virtual bool isBounded() const;

	///----------------------------------------------------------------------------------------------------
	/// This function calculates and returns Node bounding box.
	/// Bounding box is calculated from hair guides and is used by Maya for node selection.
	///
	/// \return The bounding box of the shape
	///----------------------------------------------------------------------------------------------------
	virtual MBoundingBox boundingBox() const;

	///----------------------------------------------------------------------------------------------------
	/// Computes new attributes of our Node based on other updated attributes.
	///	Called whenever surface on which hair grow has been changed or time has been changed.
	///	
	/// \param aPlug				Which attribute was changed.
	/// \param [in,out] aDataBlock	The data block with attributes values.
	///
	/// \return Maya status code.
	///----------------------------------------------------------------------------------------------------
	virtual MStatus compute( const MPlug &aPlug, MDataBlock &aDataBlock );

	///----------------------------------------------------------------------------------------------------
	/// Gets an internal value in context. 
	/// Called whenever any internal attribute is required by Maya.
	///
	/// \param	aPlug				Which attribute is being quiered.
	/// \param [in,out]	aDataHandle	Handle to data storage.
	/// \param [in,out]	aContext	Current context.
	///
	/// \return	true if value was loaded from user storage, false if it should be loaded defaultly.
	///----------------------------------------------------------------------------------------------------
	virtual bool getInternalValueInContext( const MPlug & aPlug, MDataHandle & aDataHandle, MDGContext & aContext );  

	///----------------------------------------------------------------------------------------------------
	/// Sets an internal value in context.
	/// Called whenever any internal value is changed ( except surface and time all other attributes
	/// we are interested in are internal ).
	/// If the change is expensive, it may be delayed to accumulate all changes happening on the same
	/// attribute.
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
	/// Reacts on breaking connection to our node's attribute.
	///
	/// \param aPlug				Which plug lost his connection.
	/// \param aOtherPlug			Plug on the other side of the broken connection.
	/// \param aAsSrc				Determines if this plug is source.
	///
	/// \return Maya status code.
	///----------------------------------------------------------------------------------------------------
	virtual MStatus connectionBroken( const MPlug & aPlug, const MPlug & aOtherPlug,bool aAsSrc );

	///----------------------------------------------------------------------------------------------------
	/// Creates new HairShape node.
	/// 
	/// \return	New Hair Shape as void pointer.
	///----------------------------------------------------------------------------------------------------
	static void *creator();

	///----------------------------------------------------------------------------------------------------
	/// Draws hair guides and interpolated hair asociated with this HairShape node.
	///----------------------------------------------------------------------------------------------------
	void draw();

	///----------------------------------------------------------------------------------------------------
	/// Synchronize the node's selection with Maya's (in case the keyboard modifiers were used).
	///----------------------------------------------------------------------------------------------------
	void syncSelection();

	///----------------------------------------------------------------------------------------------------
	/// Checks if the given interpolation group is currently selectable.
	/// 
	/// \param	aGroupIndex		The index of the interpolation group.
	///----------------------------------------------------------------------------------------------------
	bool isInterpolationGroupSelectable( unsigned __int32 aGroupIndex );

	///----------------------------------------------------------------------------------------------------
	/// Gets the selected guides segments uniform grid. This grid is not set as dirty when external tool
	/// changes hair segments.
	///
	/// \return	The selected guides segments uniform grid. 
	///----------------------------------------------------------------------------------------------------
	inline const HairComponents::SegmentsDS & getSelectedGuidesDS();

	///----------------------------------------------------------------------------------------------------
	/// Updates the guides after the brush or any other tool was used to change hair guides segments. 
	/// 
	/// \param	aStoreUpdate	if true, stores updates of selected guides to stack, 
	/// 						otherwise update is only displayed, not stored.
	///----------------------------------------------------------------------------------------------------
	inline void updateGuides( bool aStoreUpdate );

	///----------------------------------------------------------------------------------------------------
	/// Associates the components (guides or vertices) with the node's attributes (and corresponding plugs). 
	/// 
	/// \param	aComponent	the component that is passed in (with type of e.g. kMeshVertComponent)
	/// \param	aList	the selection list to which the corresponding plugs will be added
	///----------------------------------------------------------------------------------------------------
	virtual void componentToPlugs( MObject &aComponent,  MSelectionList &aList ) const;

	///----------------------------------------------------------------------------------------------------
	/// Validates component names and indices which are specified as a string and adds the corresponding
	/// component to the passed in selection list. (e.g."select shape1.vtx[0:7]")   
	/// 
	/// \param	aItem	DAG selection item for the object being matched
	/// \param	aSpec	attribute specification object
	/// \param	aList	list to add components to
	///----------------------------------------------------------------------------------------------------
	virtual MatchResult matchComponent( const MSelectionList& aItem, 
										const MAttributeSpecArray& aSpec, 
										MSelectionList& aList );

	///----------------------------------------------------------------------------------------------------
	/// Check for matches between selection type / component list, and the type of this shape or its components.  
	/// 
	/// \param	aMask			selection type mask
	/// \param	aComponentList	possible component list
	///----------------------------------------------------------------------------------------------------
	virtual bool match(	const MSelectionMask & aMask, const MObjectArray& aComponentList ) const;

	///----------------------------------------------------------------------------------------------------
	/// Undoes changes to hair guides segments.
	///----------------------------------------------------------------------------------------------------
	void undo();

	///----------------------------------------------------------------------------------------------------
	/// Redoes changes to hair guides segments.
	///----------------------------------------------------------------------------------------------------
	void redo();

	///-------------------------------------------------------------------------------------------------
	/// Queries if we can undo changes to hair guides segments. 
	///
	/// \return	true if we can call undo command. 
	///-------------------------------------------------------------------------------------------------
	inline bool canUndo() const;

	///-------------------------------------------------------------------------------------------------
	/// Queries if we can redo changes to hair guides segments.
	///
	/// \return	true if we can call redo command.
	///-------------------------------------------------------------------------------------------------
	inline bool canRedo() const;

	///----------------------------------------------------------------------------------------------------
	/// This method initilizes static attributes for all HairShape nodes.
	/// 
	/// \return Maya status code.
	///----------------------------------------------------------------------------------------------------
    static MStatus initialize();

	///-------------------------------------------------------------------------------------------------
	/// Sample HairShape at given time to files ( with prefix aFileName ).
	/// These files will be then used to display interpolated hair in extern renderer ( RenderMan ),
	/// Interpolated hair are splitted to voxels which will be rendered separately.
	/// Hair guides and interpolation properties are stored in one file and current and rest pose mesh  
	/// are voxelized and stored in separate files for each voxel.
	/// Bounding boxes of each voxel are calculated using all interpolated hair geometry.
	///
	/// \param	aSampleTime					Time of the sample. 
	/// \param	aFileName					Filename of the file. 
	/// \param [in,out]	aVoxelBoundingBoxes	The voxel bounding boxes. 
	///-------------------------------------------------------------------------------------------------
	void sampleTime( Time aSampleTime, const std::string & aFileName, BoundingBoxes & aVoxelBoundingBoxes );

	///----------------------------------------------------------------------------------------------------
	/// Resamples all dirty textures and reacts to any texture change.
	/// If density is changed, resamples hair guides positions and interpolate hair segments.
	/// If interpolation groups are changed, interpolation groups object must be change, also hair segments
	/// count must be refreshed.
	/// Finally generates interpolated hair if necessary.
	/// 
	/// \param aForceRefresh	Should we refresh all textures ?
	///----------------------------------------------------------------------------------------------------
	void refreshTextures( bool aForceRefresh = false );
		
	///----------------------------------------------------------------------------------------------------
	/// Sets the node's current selection status (selected / not selected).
	/// 
	/// \param aFlag	True when the node is in the selected state.
	///----------------------------------------------------------------------------------------------------
	void setCurrentlySelected( bool aFlag );

	///----------------------------------------------------------------------------------------------------
	/// Is the shape currently selected in maya?
	/// 
	/// \return true, if the shape (or some of its components) is in maya's active selection list.
	///----------------------------------------------------------------------------------------------------
	bool isCurrentlySelected();

	///----------------------------------------------------------------------------------------------------
	/// Notifies the shape when its list of selected components might have changed.
	/// 
	/// \param aFlag	True when the selection list changes.
	///----------------------------------------------------------------------------------------------------
	void setSelectionModified( bool aFlag );

	///----------------------------------------------------------------------------------------------------
	/// Has the shape's list of selected components been modified?
	/// 
	/// \return true, if shape's list of selected components has been modified.
	///----------------------------------------------------------------------------------------------------
	bool isSelectionModified();

	///----------------------------------------------------------------------------------------------------
	/// Is the shape currently in Maya's active selection list?
	/// 
	/// \return true, if the shape is currently in Maya's active selection list.
	///----------------------------------------------------------------------------------------------------
	bool isSelectedInMaya();

	///----------------------------------------------------------------------------------------------------
	/// Sets this HairShape as active object. 
	/// Only active object reacts on external events such as UI commands calls and brushes.
	///----------------------------------------------------------------------------------------------------
	inline void setAsActiveObject();

	///----------------------------------------------------------------------------------------------------
	/// Import NURBS curves to hair guides.
	///----------------------------------------------------------------------------------------------------
	inline void importNURBS();

	///----------------------------------------------------------------------------------------------------
	/// Export hair guides to NURBS curves.
	///----------------------------------------------------------------------------------------------------
	inline void exportToNURBS();

	///-------------------------------------------------------------------------------------------------
	/// Reinits all cutted hair with zero length to initial length multiplied by scale factor.
	///-------------------------------------------------------------------------------------------------
	inline void reinitCuttedHair();

	///-------------------------------------------------------------------------------------------------
	/// Resets all guides to their initial configuration.
	///-------------------------------------------------------------------------------------------------
	inline void resetGuides();

	///----------------------------------------------------------------------------------------------------
	/// Gets the current maya mesh object.
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
	/// Gets the current maya mesh inclusive matrix.
	/// Inclusive matrix stores all transforms which are applied on surface connected to this node.
	/// 
	/// \return	Get current maya mesh inclusive matrix.
	///----------------------------------------------------------------------------------------------------
	inline MMatrix getCurrentInclusiveMatrix() const;

	///-------------------------------------------------------------------------------------------------
	/// Return this node as Maya MObject.
	/// 
	/// \return	Return this HairShape as MObject.
	///-------------------------------------------------------------------------------------------------
	inline MObject asMObject();

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
	/// Sets the object's voxels resolution. 
	///
	/// \param aNewVoxelsResolution	New voxels resolution.
	/// \param aOldVoxelsResolution	Place to store the old voxels resolution.
	///----------------------------------------------------------------------------------------------------
	inline void setVoxelsResolution(Dimensions3 aNewVoxelsResolution, Dimensions3 & aOldVoxelsResolution);

private:
	
	friend class HairShapeUI;

	// Private methods

	///----------------------------------------------------------------------------------------------------
	/// Mesh has been changed.
	/// Reacts on mesh change by repositioning hair guides and interpolated hair.
	/// If topology has been change, some hair guides may be destroyed inside HairGuides object. 
	///
	/// \param	aMeshObj	The mesh object.
	///----------------------------------------------------------------------------------------------------
	void meshChange( MObject aMeshObj );

	///----------------------------------------------------------------------------------------------------
	/// Sets a current time.
	/// Load new hair guides segments for currenly selected time and interpolates hair. 
	///
	/// \param	aTime	The current time. 
	///----------------------------------------------------------------------------------------------------
	void setCurrentTime( Time aTime );

	///----------------------------------------------------------------------------------------------------
	/// Refresh pointers to guides for interpolation class.
	/// InterpolatedHair class only access guides through HairProperties interface which stores
	/// constant pointer to hair guides segments and rest positions. If any of these are reallocated,
	/// pointers inside HairProperties must be refreshed.
	///----------------------------------------------------------------------------------------------------
	inline void refreshPointersToGuidesForInterpolation();

	///-------------------------------------------------------------------------------------------------
	/// Updates the segments count attributes. Must be called after Interpolation groups have been 
	/// changed. Segments count array length will be set to interpolation groups count and segments 
	/// count elements will store groups segments count. For each segments count element color of 
	/// corresponding group will be displayed.
	/// 
	/// \param	aFirstUpdate	If first update is selected, then the old segments count don't need to 
	/// 						be removed.
	///-------------------------------------------------------------------------------------------------
	inline void updateSegmentsCountAttributes( bool aFirstUpdate );

	///-------------------------------------------------------------------------------------------------
	/// Updates the selectable interpolation groups attribute. Must be called after Interpolation groups 
	/// have been changed.
	/// 
	/// \param	aFirstUpdate	If first update is selected, then the old segments count doesn't need to 
	/// 						be removed.
	///-------------------------------------------------------------------------------------------------
	inline void updateInterpolationGroupsSelectableAttributes( bool aFirstUpdate );

	///----------------------------------------------------------------------------------------------------
	/// Registers the mesh topology callback ( mesh connected to this HairShape ).
	///
	/// \return Maya status code.
	///----------------------------------------------------------------------------------------------------
	MStatus registerTopologyCallback();

	///----------------------------------------------------------------------------------------------------
	/// Synchronize UI- and true attribute versions after a delay.
	/// Should be invoked for values that are too expensive to change in real time.
	///
	/// \param	elapsedTime	The amount of time since the callback was last called.
	/// \param	aLastTime	The execution time at the previous call to this callback.
	/// \param	aThis	"this" pointer (supplied when the callback was registered).
	///----------------------------------------------------------------------------------------------------
	static void delayedSetInternalValueInContext(float aElapsedTime, float aLastTime, void* aThis);

	///----------------------------------------------------------------------------------------------------
	/// Is there a delayedSetInternalValueInContext call pending? I.e. is the timer running?
	///
	/// \return true when there's a call pending
	///----------------------------------------------------------------------------------------------------
	inline bool delayedIsPending();

	///----------------------------------------------------------------------------------------------------
	/// Cancel the timer for delayedSetInternalValueInContext.
	/// This is needed because Maya's timer can't be invoked in single-fire mode.
	///----------------------------------------------------------------------------------------------------
	inline void clearDelayed();

	///----------------------------------------------------------------------------------------------------
	/// Set up a delayedSetInternalValueInContext call with a delay.
	///----------------------------------------------------------------------------------------------------
	inline void setupDelayed();

	/*TODO Test and replace with proper documentation*/
	bool					value( int pntInd, int vlInd, double & val ) const;
	bool					value( int pntInd, MPoint & val ) const;
	bool					setValue( int pntInd, int vlInd, double val );
	bool					setValue( int pntInd, const MPoint & val );


	// Inner objects 

	MBoundingBox mBoundingBox; ///< The bounding box of our node

	RandomGenerator mRandom;	///< The random generator of numbers

	UVPointGenerator *mUVPointGenerator; ///< UV point generator ( hair sampler )

	MayaMesh *mMayaMesh; ///< Maya mesh on which hair grows

	HairComponents::HairGuides *mHairGuides; ///< Object for storing hair guides segments

	Interpolation::Maya::Voxelization * mVoxelization;   ///< The voxelization class for external interpolation of hair

	Interpolation::Maya::InterpolatedHair mInterpolatedHair;	///< The object for displaying interpolate hair in Maya

	MDagPath mConnectedMeshPath; ///< Path to connected mesh object

	MCallbackId mDelayedCallbackId; ///< Timer id for delayed setInternalValueInContext call (-1 when there's none)

	// Stored attributes values

	unsigned __int32 mGuidesHairCount;  ///< Number of guides hairs

	unsigned __int32 mGeneratedHairCount;	///< Number of generated hair

	Dimensions3 mVoxelsResolution;  ///< The voxels resolution

	Time mTime; ///< The current time

	bool mDisplayGuides;   ///< Should guides be displayed ?

	bool mDisplayInterpolated; ///< Should interpolated hair be displayed ?

	unsigned __int32 mGenDisplayCountWanted;	///< The number of generated hair to be displayed (to be shown in the UI)
	unsigned __int32 mGenDisplayCount;	///< The number of generated hair to be displayed (true value)

	unsigned __int32 mSampleTextureDimension; ///< Texture dimension for sampling attribute

	// Mesh topology callback

	bool mIsTopologyCallbackRegistered; ///< true if mesh topology callback is registered

	bool mIsTopologyModified;   ///< true if topology is modified

	bool mIsCurrentlySelected; ///< true if the node is currently selected in maya

	bool mIsSelectionModified; ///< true if the selection has changed

	static MCallbackIdArray mCallbackIds;	///< List of identifiers for the callbacks

	// Active hair shape object

	static HairShape * mActiveHairShapeNode;   ///< The active node
};

// inline functions implementation

inline const HairComponents::SegmentsDS & HairShape::getSelectedGuidesDS()
{
	return mHairGuides->getSelectedGuidesDS();
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

inline void HairShape::setVoxelsResolution(Dimensions3 aNewVoxelsResolution, Dimensions3 & aOldVoxelsResolution)
{
	aOldVoxelsResolution[ 0 ] = mVoxelsResolution[ 0 ];
	aOldVoxelsResolution[ 1 ] = mVoxelsResolution[ 1 ];
	aOldVoxelsResolution[ 2 ] = mVoxelsResolution[ 2 ];

	// If the new voxelization is the same, there's no need to recreate it.
	if ( mVoxelsResolution[ 0 ] == aNewVoxelsResolution[ 0 ] &&
		 mVoxelsResolution[ 1 ] == aNewVoxelsResolution[ 1 ] &&
		 mVoxelsResolution[ 2 ] == aNewVoxelsResolution[ 2 ] )
	{
		return;
	}

	mVoxelsResolution[ 0 ] = aNewVoxelsResolution[ 0 ];
	mVoxelsResolution[ 1 ] = aNewVoxelsResolution[ 1 ];
	mVoxelsResolution[ 2 ] = aNewVoxelsResolution[ 2 ];
	
	delete mVoxelization; // Throw away old voxelization
	mVoxelization = 0;
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

inline void HairShape::resetGuides()
{
	mHairGuides->resetGuides( getScaleFactor() );
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