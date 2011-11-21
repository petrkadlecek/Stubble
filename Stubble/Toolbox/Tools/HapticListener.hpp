#ifndef HAPTIC_LISTENER_HPP
#define HAPTIC_LISTENER_HPP

#include "GenericTool.hpp"
#include <maya\MPxLocatorNode.h>

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// UI class defines the UI part of a hair shape node.
///----------------------------------------------------------------------------------------------------
class HapticListener : public MPxLocatorNode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Default constructor.
	///----------------------------------------------------------------------------------------------------
  HapticListener( );

	static void setTool( GenericTool *aOwner );

  ///----------------------------------------------------------------------------------------------------
	/// MPxLocatorNode
	///----------------------------------------------------------------------------------------------------
  virtual MStatus compute( const MPlug& plug, MDataBlock& dataBlock );

  virtual void draw( M3dView&, const MDagPath&, M3dView::DisplayStyle, M3dView::DisplayStatus);
  
  virtual bool isBounded() const;
  
  virtual MBoundingBox boundingBox() const;
  
  static void* creator();
  
  static MStatus initialize();

 	/// the unique type ID of our custom node. Used internally by maya for fileIO.
	static const MTypeId typeId;

	/// the unique type name of our custom node. Mainly for mel purposes.
	static const MString typeName;

	static MObject inputAttr;

	///----------------------------------------------------------------------------------------------------
	/// Destructor
	///----------------------------------------------------------------------------------------------------
    virtual ~HapticListener();

private:
   static GenericTool *sTool;
};

} // namespace Toolbox

} // namespace Stubble

#endif  // HAPTIC_LISTENER_HPP
