#include "HairShape.hpp"

#include <maya\MFnNumericAttribute.h>
#include <maya\MFnStringData.h>
#include <maya\MFnTypedAttribute.h>

namespace Stubble
{

namespace HairShape
{

const MTypeId HairShape::typeId( 0x80002 );
const MString HairShape::typeName( "HairShape" );

/// attributes for the locator
MObject HairShape::countAttr;
MObject HairShape::genCountAttr;
MObject HairShape::lengthAttr;
MObject HairShape::surfaceAttr;
MObject HairShape::randomXAttr;
MObject HairShape::randomYAttr;
MObject HairShape::randomZAttr;
MObject HairShape::genRandomXAttr;
MObject HairShape::genRandomYAttr;
MObject HairShape::genRandomZAttr;
MObject HairShape::redrawAttr;
MObject HairShape::segmentsAttr;
MObject HairShape::textureAttr;
MObject HairShape::displayNormalsAndTangentsAttr;

HairShape::HairShape()
{
}

bool HairShape::isBounded() const
{
	return true;
}

MBoundingBox HairShape::boundingBox() const
{
	MObject thisNode = thisMObject();
	int sent = 0;

	// get sentinel plug, causes update
	MPlug RedrawPlug(thisNode, HairShape::redrawAttr );
	RedrawPlug.getValue( sent );

	// returns updated bounding box
	return mBoundingBox;
}

MStatus HairShape::compute(const MPlug &plug, MDataBlock &dataBlock)
{
	MStatus status;

	//we only care about the sentinelAttr, ignore stuff like localScale
	if(plug == redrawAttr)
	{
		dataBlock.setClean(redrawAttr);

		MObject meshObj = dataBlock.inputValue(surfaceAttr).asMesh(); //get input mesh
		if(meshObj == MObject::kNullObj)
		{
			return MS::kFailure;
		}

		// getting hair attributes
		const int count = dataBlock.inputValue(countAttr).asInt();
		const float length = dataBlock.inputValue(lengthAttr).asFloat();
		const int genCount = dataBlock.inputValue(genCountAttr).asInt();
		const float randomX = dataBlock.inputValue(randomXAttr).asFloat();
		const float randomY = dataBlock.inputValue(randomYAttr).asFloat();
		const float randomZ = dataBlock.inputValue(randomZAttr).asFloat();
		const float getRandomX = dataBlock.inputValue(genRandomXAttr).asFloat();
		const float getRandomY = dataBlock.inputValue(genRandomYAttr).asFloat();
		const float getRandomZ = dataBlock.inputValue(genRandomZAttr).asFloat();
		const int segments = dataBlock.inputValue(segmentsAttr).asInt();
		MString texture = dataBlock.inputValue(textureAttr).asString();
		bool displayNormalsAndTangents = dataBlock.inputValue(displayNormalsAndTangentsAttr).asBool();
	}

	return MS::kSuccess;
}

void* HairShape::creator()
{
	return new HairShape();
}

void HairShape::draw()
{
	// TODO
}

MStatus HairShape::initialize()
{	
	MStatus status;

	//define surface attribute
	MFnTypedAttribute tAttr;
	surfaceAttr = tAttr.create( "surface", "srf", MFnData::kMesh, MObject::kNullObj, &status );
	tAttr.setWritable( true );
	if( !status )
	{
		status.perror( "Creation of a mesh has failed" );
		return status;
	}

	addAttribute( surfaceAttr );

	//define count attribute
	MFnNumericAttribute nAttr;
	countAttr = nAttr.create( "count", "cnt", MFnNumericData::kInt, 100 );
	nAttr.setKeyable( true );
	addAttribute( countAttr );

	//define gen. count attribute
	genCountAttr = nAttr.create( "Interpolated_Hair", "gcnt", MFnNumericData::kInt, 10000 );
	nAttr.setKeyable( true );
	addAttribute( genCountAttr );

	//define length attribute
	lengthAttr = nAttr.create( "length", "len", MFnNumericData::kFloat, 0.2 );
	nAttr.setMin( 0.0000001 );
	nAttr.setKeyable( true );
	addAttribute( lengthAttr );

	//define random X attribute
	randomXAttr = nAttr.create("random_X", "rX", MFnNumericData::kFloat, 0.0);
	nAttr.setMin(0);
	nAttr.setKeyable(true);
	addAttribute(randomXAttr);
	
	//define random Y attribute
	randomYAttr = nAttr.create("random_Y", "rY", MFnNumericData::kFloat, 0.0);
	nAttr.setMin(0);
	nAttr.setKeyable(true);
	addAttribute(randomYAttr);

	//define random Z attribute
	randomZAttr = nAttr.create("random_Z", "rZ", MFnNumericData::kFloat, 0.0);
	nAttr.setMin(0);
	nAttr.setKeyable(true);
	addAttribute(randomZAttr);

	//define gen. random X attribute
	genRandomXAttr = nAttr.create("Interpolated_random_X", "irX", MFnNumericData::kFloat, 0.0);
	nAttr.setMin(0);
	nAttr.setKeyable(true);
	addAttribute(genRandomXAttr);
	
	//define gen. random Y attribute
	genRandomYAttr = nAttr.create("Interpolated_random_Y", "irY", MFnNumericData::kFloat, 0.0);
	nAttr.setMin(0);
	nAttr.setKeyable(true);
	addAttribute(genRandomYAttr);

	//define gen. random Z attribute
	genRandomZAttr = nAttr.create("Interpolated_random_Z", "irZ", MFnNumericData::kFloat, 0.0);
	nAttr.setMin(0);
	nAttr.setKeyable(true);
	addAttribute(genRandomZAttr);

	//define segments attribute
	segmentsAttr = nAttr.create("segments_count", "bpc", MFnNumericData::kInt, 5);
	nAttr.setMin(1);
	nAttr.setKeyable(true);
	addAttribute(segmentsAttr);

	//define display normals and tangents attribute
	displayNormalsAndTangentsAttr = nAttr.create("display_normals_and_tangents", "dnt", MFnNumericData::kBoolean,false);
	nAttr.setKeyable(true);
	addAttribute(displayNormalsAndTangentsAttr);

	// Declare the function set and a MObject for the attribute data.
	MFnStringData fnStringData;
	MObject defaultString = fnStringData.create( "" );

	// Create the attribute and specify its default.
	textureAttr = tAttr.create( "texture", "txt", MFnData::kString, defaultString );

	tAttr.setStorable(true);
	tAttr.setKeyable(false);

	addAttribute( textureAttr );

	//redraw attr
	redrawAttr = nAttr.create("redraw", "rdrw", MFnNumericData::kInt, 0);
	nAttr.setHidden(true);
	addAttribute(redrawAttr);

	//this tells maya what inputs affect what outputs
	attributeAffects(surfaceAttr, redrawAttr);
	attributeAffects(countAttr, redrawAttr);
	attributeAffects(genCountAttr, redrawAttr);
	attributeAffects(lengthAttr, redrawAttr);
	attributeAffects(randomXAttr, redrawAttr);
	attributeAffects(randomYAttr, redrawAttr);
	attributeAffects(randomZAttr, redrawAttr);
	attributeAffects(genRandomXAttr, redrawAttr);
	attributeAffects(genRandomYAttr, redrawAttr);
	attributeAffects(genRandomZAttr, redrawAttr);
	attributeAffects(segmentsAttr, redrawAttr);
	attributeAffects(displayNormalsAndTangentsAttr, redrawAttr);
	attributeAffects(textureAttr, redrawAttr);

	return MS::kSuccess;
}

} // namespace HairShape

} // namespace Stubble