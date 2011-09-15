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
/// Interface of generator of finished interpolated hair. No virtual functions are used !
///-------------------------------------------------------------------------------------------------
template< typename tPositionType, typename tColorType, typename tNormalType, typename tWidthType >
class OutputGenerator
{
public:
	/* Any class that will be used as OutputGenerator must defined next types :
	 *		PositionType	-	type used to store one component of the 3 components of the 3D position
			ColorType		-	type used to store one component of the 3 components of the color
			NormalType		-	type used to store one component of the 3 components of the normal
			WidthType		-	type used to store width
	 */
	
	///-------------------------------------------------------------------------------------------------
	/// Begins an output of interpolated hair. 
	///
	/// \param	aMaxHairCount	Number of a maximum hair. 
	/// \param	aMaxPointsCount	Number of a maximum points. 
	/// \param	aUseColors		Colors will be outputed
	/// \param	aUseNormals		Normals will be outputed
	/// \param	aUseWidths		Widths will be outputed
	///-------------------------------------------------------------------------------------------------
	inline void beginOutput( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount,
		bool aUseColors, bool aUseNormals, bool aUseWidths );

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
	/// \return	null if it fails, else return position pointer. 
	///-------------------------------------------------------------------------------------------------
	inline tPositionType * positionPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points colors. 
	///
	/// \return	null if it fails, else return color pointer. 
	///-------------------------------------------------------------------------------------------------
	inline tColorType * colorPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points normals. 
	///
	/// \return	null if it fails, else return normal pointer. 
	///-------------------------------------------------------------------------------------------------
	inline tNormalType * normalPointer();
	
	///-------------------------------------------------------------------------------------------------
	/// Gets the pointer to hair points widths. 
	///
	/// \return	null if it fails, else return width pointer. 
	///-------------------------------------------------------------------------------------------------
	inline tWidthType * widthPointer();
	
protected:
	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	inline OutputGenerator();
};

// inline functions implementation

template< typename tPositionType, typename tColorType, typename tNormalType, typename tWidthType >
inline void OutputGenerator< tPositionType, tColorType, tNormalType, tWidthType >::beginOutput
	( unsigned __int32 aMaxHairCount, unsigned __int32 aMaxPointsCount, 
	bool aUseColors, bool aUseNormals, bool aUseWidths )
{
	throw StubbleException( "OutputGenerator::beginOutput : this method is not implemented !" ); 
}

template< typename tPositionType, typename tColorType, typename tNormalType, typename tWidthType >
inline void OutputGenerator< tPositionType, tColorType, tNormalType, tWidthType >::endOutput()
{
	throw StubbleException( "OutputGenerator::endOutput : this method is not implemented !" ); 
}


template< typename tPositionType, typename tColorType, typename tNormalType, typename tWidthType >
inline void OutputGenerator< tPositionType, tColorType, tNormalType, tWidthType >::beginHair
	( unsigned __int32 aMaxPointsCount )
{
	throw StubbleException( "OutputGenerator::beginHair : this method is not implemented !" ); 
}

template< typename tPositionType, typename tColorType, typename tNormalType, typename tWidthType >
inline void OutputGenerator< tPositionType, tColorType, tNormalType, tWidthType >::endHair
	( unsigned __int32 aPointsCount )
{
	throw StubbleException( "OutputGenerator::endHair : this method is not implemented !" ); 
}

template< typename tPositionType, typename tColorType, typename tNormalType, typename tWidthType >
inline tPositionType * OutputGenerator< tPositionType, tColorType, tNormalType, tWidthType >::positionPointer()
{
	throw StubbleException( "OutputGenerator::positionPointer : this method is not implemented !" ); 
}

template< typename tPositionType, typename tColorType, typename tNormalType, typename tWidthType >
inline tColorType * OutputGenerator< tPositionType, tColorType, tNormalType, tWidthType >::colorPointer()
{
	throw StubbleException( "OutputGenerator::colorPointer : this method is not implemented !" ); 
}

template< typename tPositionType, typename tColorType, typename tNormalType, typename tWidthType >
inline tNormalType * OutputGenerator< tPositionType, tColorType, tNormalType, tWidthType >::normalPointer()
{
	throw StubbleException( "OutputGenerator::normalPointer : this method is not implemented !" ); 
}

template< typename tPositionType, typename tColorType, typename tNormalType, typename tWidthType >
inline tWidthType * OutputGenerator< tPositionType, tColorType, tNormalType, tWidthType >::widthPointer()
{
	throw StubbleException( "OutputGenerator::widthPointer : this method is not implemented !" ); 
}

template< typename tPositionType, typename tColorType, typename tNormalType, typename tWidthType >
inline OutputGenerator< tPositionType, tColorType, tNormalType, tWidthType >::OutputGenerator()
{
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_OUTPUT_GENERATOR_HPP