#ifndef HAPTIC_LISTENER_HPP
#define HAPTIC_LISTENER_HPP

#include "GenericTool.hpp"
#include <maya\MPxLocatorNode.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include "../../Primitives/Vector3D.hpp"

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

		/// haptic button1 last state
		bool mHapticButton1Last;

		/// haptic button2 last state
		bool mHapticButton2Last;

		/// compute vector[4] matrix[4][4] multiplication
		inline void VectorMatrixMul4f(float pVector[4], float pMat[16]);
	};

} // namespace Toolbox

} // namespace Stubble

#endif  // HAPTIC_LISTENER_HPP
