#ifndef STUBBLE_OUTPUT_GENERATOR_HPP
#define STUBBLE_OUTPUT_GENERATOR_HPP

#include "Common/StubbleException.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

///-------------------------------------------------------------------------------------------------
/// Interface of generator of finished interpolated hair. No virtual functions are used, this class
/// here only represents the interface for classes used as HairGenerator template argument.
/// Output generator purpose is to accumulate generated hair geometry, color and other properties 
/// and then send it to final output destination (OpenGL, RenderMan etc. ).
///-------------------------------------------------------------------------------------------------
template< typename tOutputGeneratorTypes >
class OutputGenerator
{
public:
	/* Any class that will be used as OutputGenerator must defined next types :
	 *		PositionType	-	type used to store one component of the 3 components of the 3D position
			ColorType		-	type used to store one component of the 3 components of the color
			NormalType		-	type used to store one component of the 3 components of the normal
			WidthType		-	type used to store width
			OpacityType		-	type used to store opacity
			UVCoordinateType-	type used to store u or v coordinate
			IndexType		-	type used to store hair and strand index
	 */

	///-------------------------------------------------------------------------------------------------
	/// Begins an output of interpolated hair.
	/// Must be called before any hair is outputed. 
	///
	/// \param	aMaxHairCount	Number of a maximum hair. 
	/// \param	aMaxPointsCount	Number of a maximum points. 
	///-------------------------------------------------------------------------------------------------
	inline void beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount );

	///----------------------------------------------------------------------------------------------------
	/// Ends an output.
	/// After this function no output will be received until beginOutput is called.
	///----------------------------------------------------------------------------------------------------
	inline void endOutput();

	///-------------------------------------------------------------------------------------------------
	/// Begins an output of single interpolated hair.
	/// The upper estimate of points on hair must be known to make sure enough resources are be 
	/// prepared. Afterwards hair geometry, color etc. can be send to OutputGenerator via 
	/// positionPointer, colorPointer etc.
	///
	/// \param	aMaxPointsCount	Number of a maximum points on current hair. 
	///-------------------------------------------------------------------------------------------------
	inline void beginHair( unsigned __int32 aMaxPointsCount );

	///-------------------------------------------------------------------------------------------------
	/// Ends an output of single interpolated hair.
	/// Parameter corresponds to number of outputed positions via positionPointer.	
	/// 
	/// \param	aPointsCount	Number of points on finished hair. 
	///-------------------------------------------------------------------------------------------------
	inline void endHair( unsigned __int32 aPointsCount );

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points positions. 
	/// Caller must output as many hair points as specified in later endHair call.
	///
	/// \return	Pointer to position buffer.
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::PositionType * positionPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points colors.
	/// Caller must output as many colors as hair positions count minus 2.
	///
	/// \return	Pointer to color buffer.
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::ColorType * colorPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points normals. 
	/// Caller must output as many normals as hair positions count minus 2.
	/// 
	/// \return	Pointer to normal buffer.
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::NormalType * normalPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points widths. 
	/// Caller must output as many widths as hair positions count minus 2.
	///
	/// \return	Pointer to width buffer.
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::WidthType * widthPointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points opacities. 
	/// Caller must output as many opacities as hair positions count minus 2.
	///
	/// \return	null Pointer to opacity buffer.
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::OpacityType * opacityPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair UV coordinates.
	/// Caller must output one pair of UV coordinates per hair.
	/// Texture UV Coordinates of hair root should be outputed.
	///
	/// \return	Pointer to UV coordinates buffer.
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::UVCoordinateType * hairUVCoordinatePointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair strand UV coordinates.
	/// Caller must output one pair of UV coordinates per hair.
	/// Texture UV Coordinates of main strand hair root should be outputed. 
	///
	/// \return	Pointer to strand UV coordinates buffer.
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::UVCoordinateType * strandUVCoordinatePointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair indices.
	/// Caller must output one unique index per hair.
	///
	/// \return	Pointer to hair indices buffer.
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::IndexType * hairIndexPointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to strand indices.
	/// Caller must output one strand index per hair. Strand index is unique for every strand,
	/// but is same for hair in one strand.
	///
	/// \return	Pointer to strand indices buffer.
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::IndexType * strandIndexPointer();

protected:
	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	inline OutputGenerator();
};

// inline functions implementation

template< typename tOutputGeneratorTypes >
inline void OutputGenerator< tOutputGeneratorTypes >::beginOutput
	( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount )
{
	throw StubbleException( "OutputGenerator::beginOutput : this method is not implemented !" ); 
}

template< typename tOutputGeneratorTypes >
inline void OutputGenerator< tOutputGeneratorTypes >::endOutput()
{
	throw StubbleException( "OutputGenerator::endOutput : this method is not implemented !" ); 
}


template< typename tOutputGeneratorTypes >
inline void OutputGenerator< tOutputGeneratorTypes >::beginHair
	( unsigned __int32 aMaxPointsCount )
{
	throw StubbleException( "OutputGenerator::beginHair : this method is not implemented !" ); 
}

template< typename tOutputGeneratorTypes >
inline void OutputGenerator< tOutputGeneratorTypes >::endHair
	( unsigned __int32 aPointsCount )
{
	throw StubbleException( "OutputGenerator::endHair : this method is not implemented !" ); 
}

template< typename tOutputGeneratorTypes >
inline typename tOutputGeneratorTypes::PositionType * 
	OutputGenerator< tOutputGeneratorTypes >::positionPointer()
{
	throw StubbleException( "OutputGenerator::positionPointer : this method is not implemented !" ); 
}

template< typename tOutputGeneratorTypes >
inline typename tOutputGeneratorTypes::ColorType * 
	OutputGenerator< tOutputGeneratorTypes >::colorPointer()
{
	throw StubbleException( "OutputGenerator::colorPointer : this method is not implemented !" ); 
}

template< typename tOutputGeneratorTypes >
inline typename tOutputGeneratorTypes::NormalType * 
	OutputGenerator< tOutputGeneratorTypes >::normalPointer()
{
	throw StubbleException( "OutputGenerator::normalPointer : this method is not implemented !" ); 
}

template< typename tOutputGeneratorTypes >
inline typename tOutputGeneratorTypes::WidthType * 
	OutputGenerator< tOutputGeneratorTypes >::widthPointer()
{
	throw StubbleException( "OutputGenerator::widthPointer : this method is not implemented !" ); 
}

template< typename tOutputGeneratorTypes >
inline typename tOutputGeneratorTypes::OpacityType * 
	OutputGenerator< tOutputGeneratorTypes >::opacityPointer()
{
	throw StubbleException( "OutputGenerator::opacityPointer : this method is not implemented !" ); 
}

template< typename tOutputGeneratorTypes >
inline typename tOutputGeneratorTypes::UVCoordinateType * 
	OutputGenerator< tOutputGeneratorTypes >::hairUVCoordinatePointer()
{
	throw StubbleException( "OutputGenerator::hairUVCoordinatePointer : this method is not implemented !" ); 
}

template< typename tOutputGeneratorTypes >
inline typename tOutputGeneratorTypes::UVCoordinateType * 
	OutputGenerator< tOutputGeneratorTypes >::strandUVCoordinatePointer()
{
	throw StubbleException( "OutputGenerator::strandUVCoordinatePointer : this method is not implemented !" ); 
}

template< typename tOutputGeneratorTypes >
inline typename tOutputGeneratorTypes::IndexType * 
	OutputGenerator< tOutputGeneratorTypes >::hairIndexPointer()
{
	throw StubbleException( "OutputGenerator::hairIndexPointer : this method is not implemented !" ); 
}

template< typename tOutputGeneratorTypes >
inline typename tOutputGeneratorTypes::IndexType * 
	OutputGenerator< tOutputGeneratorTypes >::strandIndexPointer()
{
	throw StubbleException( "OutputGenerator::strandIndexPointer : this method is not implemented !" ); 
}

template< typename tOutputGeneratorTypes >
inline OutputGenerator< tOutputGeneratorTypes >::OutputGenerator()
{
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_OUTPUT_GENERATOR_HPP