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
	///
	/// \param	aMaxHairCount	Number of a maximum hair. 
	/// \param	aMaxPointsCount	Number of a maximum points. 
	/// \param	aUseNormals		True if explicit normals should be stored
	///-------------------------------------------------------------------------------------------------
	inline void beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount,
		 bool aUseNormals );

	///----------------------------------------------------------------------------------------------------
	/// Ends an output.
	///----------------------------------------------------------------------------------------------------
	inline void endOutput();

	///-------------------------------------------------------------------------------------------------
	/// Begins an output of single interpolated hair. 
	///
	/// \param	aMaxPointsCount	Number of a maximum points on current hair. 
	///-------------------------------------------------------------------------------------------------
	inline void beginHair( unsigned __int32 aMaxPointsCount );

	///-------------------------------------------------------------------------------------------------
	/// Ends an output of single interpolated hair. 
	/// 
	/// \param	aPointsCount	Number of points on finished hair. 
	///-------------------------------------------------------------------------------------------------
	inline void endHair( unsigned __int32 aPointsCount );

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points positions. 
	///
	/// \return	null if positions are not supported, else return position pointer. 
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::PositionType * positionPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points colors. 
	///
	/// \return	null if colors are not supported, else return color pointer. 
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::ColorType * colorPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points normals. 
	///
	/// \return	null if normals are not supported, else return normal pointer. 
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::NormalType * normalPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points widths. 
	///
	/// \return	null if widths are not supported, else return width pointer. 
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::WidthType * widthPointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points opacities. 
	///
	/// \return	null if opacities are not supported, else return opacity pointer. 
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::OpacityType * opacityPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair UV coordinates. 
	///
	/// \return	null if UV coordinates are not supported, else return UV coordinate pointer. 
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::UVCoordinateType * hairUVCoordinatePointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair strand UV coordinates. 
	///
	/// \return	null if UV coordinates are not supported, else return strand UV coordinate pointer. 
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::UVCoordinateType * strandUVCoordinatePointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair indices. 
	///
	/// \return	null if hair indices are not supported, else return hair index pointer. 
	///-------------------------------------------------------------------------------------------------
	inline typename tOutputGeneratorTypes::IndexType * hairIndexPointer();

	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to strand indices.
	///
	/// \return	null if strand indices are not supported, else return strand index pointer. 
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
	( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount, 
	bool aUseNormals )
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