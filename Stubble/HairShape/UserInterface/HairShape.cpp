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
MObject HairShape::redrawAttr;
MObject HairShape::segmentsAttr;
MObject HairShape::densityTextureAttr;
MObject HairShape::interpolationGroupsTextureAttr;

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
		MFnMesh mesh = meshObj;
		if(meshObj == MObject::kNullObj)
		{
			return MS::kFailure;
		}

		// first time creation of private members
		if(mMayaMesh == 0)
		{
			MString uvSetName;
			mesh.getCurrentUVSetName( uvSetName );

			// maya mesh construction
			mMayaMesh = new MayaMesh( meshObj, uvSetName );

			// UV point generator construction
			const float densityTexture = dataBlock.inputValue( densityTextureAttr ).asFloat();

			mUVPointGenerator = new UVPointGenerator( Texture( densityTexture ),
				mMayaMesh->getRestPose().getTriangleConstIterator(), RandomGenerator());

			// HairGuides construction
			const float interpolationGroupsTexture = dataBlock.inputValue( interpolationGroupsTextureAttr ).asFloat();
			const int count = dataBlock.inputValue( countAttr ).asInt();
			const int segments = dataBlock.inputValue( segmentsAttr ).asInt();

			mHairGuides = new HairComponents::HairGuides();
			mHairGuides->generate( *mUVPointGenerator,
				*mMayaMesh,
				Interpolation::InterpolationGroups( Texture( interpolationGroupsTexture ), segments ),
				count);
		}

		// getting hair attributes
		const int count = dataBlock.inputValue( countAttr ).asInt();
		const float length = dataBlock.inputValue( lengthAttr ).asFloat();
		const int genCount = dataBlock.inputValue( genCountAttr ).asInt();
		const int segments = dataBlock.inputValue( segmentsAttr ).asInt();
		const float densityTexture = dataBlock.inputValue( densityTextureAttr ).asFloat();
		const float interpolationGroupsTexture = dataBlock.inputValue( interpolationGroupsTextureAttr ).asFloat();
	}

	return MS::kSuccess;
}

void* HairShape::creator()
{
	return new HairShape();
}

void HairShape::draw()
{
	mHairGuides->draw();
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

	//define segments attribute
	segmentsAttr = nAttr.create("segments_count", "bpc", MFnNumericData::kInt, 5);
	nAttr.setMin(1);
	nAttr.setKeyable(true);
	addAttribute(segmentsAttr);

	// define density texture attribute
	densityTextureAttr = nAttr.create( "densityTexture", "dtxt", MFnNumericData::kFloat, 1 );
	nAttr.setKeyable(true);
	addAttribute( densityTextureAttr );

	// define interpolation groups texture attribute
	interpolationGroupsTextureAttr = nAttr.create( "interpolationGroupsTexture", "itxt", MFnNumericData::kFloat, 1 );
	nAttr.setKeyable(true);
	addAttribute( interpolationGroupsTextureAttr );

	redrawAttr = nAttr.create("redraw", "rdrw", MFnNumericData::kInt, 0);
	nAttr.setHidden(true);
	addAttribute(redrawAttr);

	//this tells maya what inputs affect what outputs
	attributeAffects(surfaceAttr, redrawAttr);
	attributeAffects(countAttr, redrawAttr);
	attributeAffects(genCountAttr, redrawAttr);
	attributeAffects(lengthAttr, redrawAttr);
	attributeAffects(segmentsAttr, redrawAttr);
	attributeAffects(densityTextureAttr, redrawAttr);
	attributeAffects(interpolationGroupsTextureAttr, redrawAttr);

	return MS::kSuccess;
}

} // namespace HairShape

} // namespace Stubble