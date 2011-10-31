#ifndef STUBBLE_BRUSH_MODE_HPP
#define STUBBLE_BRUSH_MODE_HPP

#include "../../Primitives/Vector3D.hpp"
#include "HairShape/HairComponents/SelectedGuides.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// The interface for all of the brush mode classes.
///----------------------------------------------------------------------------------------------------
class BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Optional move vector transformation function called once before processing a whole bunch of
	/// guides affected by single move vector. The default implementation is identity
	/// 
	/// \param aDX				Move vector to be transformed
	/// \param aTransformInfo	Transform information - any arbitrary object
	/// \return					The transformed move vector
	///----------------------------------------------------------------------------------------------------
	inline virtual Vector3D< double > preBrushTransform( const Vector3D< double > &aDX, void *aTransformInfo);

	///----------------------------------------------------------------------------------------------------
	/// The method in which the derived classes implement the different brushing transformations.
	///
	/// \param aDX	Cursor change in world coordinates
	/// \param aGuideHair	Guide selection affected by the transformation
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( const Vector3D< double > &aDX, HairShape::HairComponents::SelectedGuide &aGuideHair ) = 0;

	///----------------------------------------------------------------------------------------------------
	/// Sets whether the brush falloff is enabled or disabled
	///
	/// \param aValue	New value of the falloff switch
	///----------------------------------------------------------------------------------------------------
	inline void setFalloffSwitch ( bool aValue );
	
	///----------------------------------------------------------------------------------------------------
	/// Sets whether the brush collision detection is enabled or disabled
	///
	/// \param aValue	New value of the collision detection switch
	///----------------------------------------------------------------------------------------------------
	inline void setCollisionDetectionSwitch( bool aValue );

	///----------------------------------------------------------------------------------------------------
	/// Tells whether the collision detection is enabled or disabled
	///
	/// \return	True if enabled, false if disabled
	///----------------------------------------------------------------------------------------------------
	inline bool isCollisionDetectionEnabled();

protected:

	bool mEnableFalloff; ///< Tells whether the brush falloff is enabled or disabled

	bool mEnableCollisionDetection; ///< Tells whether the brush collision detection is enabled or disabled
};

inline Vector3D< double > BrushMode::preBrushTransform( const Vector3D< double > &aDX, void *aTransformInfo)
{
	return aDX;
}

inline void BrushMode::setFalloffSwitch ( bool aValue )
{
	mEnableFalloff = aValue;
}

inline void BrushMode::setCollisionDetectionSwitch( bool aValue )
{
	mEnableCollisionDetection = aValue;
}


inline bool BrushMode::isCollisionDetectionEnabled()
{
	return mEnableCollisionDetection;
}



} // namespace Toolbox

} // namespace Stubble

#endif