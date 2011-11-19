#ifndef STUBBLE_BRUSH_MODE_HPP
#define STUBBLE_BRUSH_MODE_HPP

#include "../../Primitives/Vector3D.hpp"
#include "HairShape/HairComponents/SelectedGuides.hpp"

namespace Stubble
{

namespace Toolbox
{

struct HairTask; // Forward declaration

///----------------------------------------------------------------------------------------------------
/// The interface for all of the brush mode classes.
///----------------------------------------------------------------------------------------------------
class BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// The method in which the derived classes implement the different brushing transformations.
	///
	/// \param aTask	Structure holding necessary information to apply transformation
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( HairTask *aTask ) = 0;

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