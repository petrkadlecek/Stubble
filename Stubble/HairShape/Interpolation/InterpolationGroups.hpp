#ifndef STUBBLE_INTERPOLATION_GROUPS_HPP
#define STUBBLE_INTERPOLATION_GROUPS_HPP

#include "Common\CommonTypes.hpp"

namespace Stubble
{

namespace HairShape
{

namespace Interpolation
{

typedef UNKNOWN Texture;

///-------------------------------------------------------------------------------------------------
/// Class for managing interpolation groups
///-------------------------------------------------------------------------------------------------
class InterpolationGroups
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Constructor. 
	///
	/// \param	aInterpolationGroupsTexture	The interpolation groups texture. 
	/// \param	aSegmentsCount				Number of the segments for all groups. 
	///-------------------------------------------------------------------------------------------------
	InterpolationGroups( const Texture & aInterpolationGroupsTexture, unsigned int aSegmentsCount );

	///----------------------------------------------------------------------------------------------------
	/// Finalizer
	///----------------------------------------------------------------------------------------------------
	inline ~InterpolationGroups();

	///-------------------------------------------------------------------------------------------------
	/// Updates interpolation groups
	///
	/// \param	aInterpolationGroupsTexture	The interpolation groups texture. 
	/// \param	aSegmentsCount				Number of the segments for all groups. 
	///-------------------------------------------------------------------------------------------------
	void updateGroups( const Texture & aInterpolationGroupsTexture, unsigned int aSegmentsCount );

	///-------------------------------------------------------------------------------------------------
	/// Gets the interpolation groups count. 
	/// 
	/// \return	The interpolation groups count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned int getGroupsCount() const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the interpolation group segments count. 
	///
	/// \param	aGroupId	Identifier for a group. 
	///
	/// \return	The interpolation group segments count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned int getGroupSegmentsCount( unsigned int aGroupId ) const;

	///-------------------------------------------------------------------------------------------------
	/// Sets the interpolation group segments count. 
	///
	/// \param	aGroupId		Identifier for a group. 
	/// \param	aSegmentsCount	Number of the segments for selected group. 
	///-------------------------------------------------------------------------------------------------
	inline void getGroupSegmentsCount( unsigned int aGroupId, unsigned int aSegmentsCount );

	///-------------------------------------------------------------------------------------------------
	/// Gets a group identifier. 
	///
	/// \param	aU	the u texture coordinate. 
	/// \param	aV	the v texture coordinate. 
	///
	/// \return	The group identifier. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned int getGroupId( Real aU, Real aV ) const;

	///-------------------------------------------------------------------------------------------------
	/// Gets the segments count. 
	///
	/// \param	aU	the u texture coordinate. 
	/// \param	aV	the v texture coordinate. 
	///
	/// \return	The segments count. 
	///-------------------------------------------------------------------------------------------------
	inline unsigned int getSegmentsCount( Real aU, Real aV ) const;

};

// inline functions implementation

inline InterpolationGroups::~InterpolationGroups()
{
}

inline unsigned int InterpolationGroups::getGroupsCount() const
{
}

inline unsigned int InterpolationGroups::getGroupSegmentsCount( unsigned int aGroupId ) const
{
}

inline void InterpolationGroups::getGroupSegmentsCount( unsigned int aGroupId, unsigned int aSegmentsCount )
{
}

inline unsigned int InterpolationGroups::getGroupId( Real aU, Real aV ) const
{
}

inline unsigned int InterpolationGroups::getSegmentsCount( Real aU, Real aV ) const
{
}

} // namespace Interpolation

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_INTERPOLATION_GROUPS_HPP