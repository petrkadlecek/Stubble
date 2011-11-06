#ifndef STUBBLE_SEGMENTS_HPP
#define STUBBLE_SEGMENTS_HPP

#include "Common\CommonTypes.hpp"
#include "Primitives\Vector3D.hpp"
#include "Common\CommonConstants.hpp"
#include "Common\CommonFunctions.hpp"

#include <sstream>
#include <string>
#include <map>
#include <vector>

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the guide segments ( as vertices of hair ).
///-------------------------------------------------------------------------------------------------
typedef std::vector< Vector3D< Real > > Segments;

///----------------------------------------------------------------------------------------------------
/// Store segments of one guide
///----------------------------------------------------------------------------------------------------
struct OneGuideSegments
{
	Real mSegmentLength;	///< Length of the segment
	Segments mSegments; ///< The segments

	///-------------------------------------------------------------------------------------------------
	/// Serialize object.
	///-------------------------------------------------------------------------------------------------
	inline std::string serialize() const;

	///-------------------------------------------------------------------------------------------------
	/// Deserialize object.	
	///
	/// \param	aStr	String from which to read.
	/// \param	aPos	Position at which to start.
	///-------------------------------------------------------------------------------------------------
	inline size_t deserialize( const std::string &aStr, size_t aPos );
};

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing the segments of all guides .
///-------------------------------------------------------------------------------------------------
typedef std::vector< OneGuideSegments > GuidesSegments;

///-------------------------------------------------------------------------------------------------
/// Segments of all guides in one time frame
///-------------------------------------------------------------------------------------------------
struct FrameSegments
{
	GuidesSegments mSegments;  ///< The segments of all guides

	Time mFrame; ///< The time frame

	///-------------------------------------------------------------------------------------------------
	/// Serialize object.
	///-------------------------------------------------------------------------------------------------
	inline std::string serialize() const;

	///-------------------------------------------------------------------------------------------------
	/// Deserialize object.	
	///
	/// \param	aStr	String from which to read.
	/// \param	aPos	Position at which to start.
	///-------------------------------------------------------------------------------------------------
	inline size_t deserialize( const std::string &aStr, size_t aPos );
};

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing all frames segments .
///-------------------------------------------------------------------------------------------------
typedef std::map< Time, FrameSegments > AllFramesSegments;

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing identifier for the guide .
///-------------------------------------------------------------------------------------------------
typedef unsigned __int32 GuideId;

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing list of identifiers for the guides .
///-------------------------------------------------------------------------------------------------
typedef std::vector< GuideId > GuidesIds;

///-------------------------------------------------------------------------------------------------
/// Partial storage of all segments (used for undo stack).
///-------------------------------------------------------------------------------------------------
struct PartialStorage
{
	GuidesSegments mSegments;  ///< The segments of selected guides

	GuidesIds mIds; ///< The identifiers of guides
};

// implementations of inline functions

inline std::string OneGuideSegments::serialize() const
{
	std::ostringstream oss;		
	oss << Stubble::serialize< Real >( mSegmentLength )
		<< Stubble::serializeObjects< Vector3D< Real > >( mSegments );
	return oss.str();
}

inline size_t OneGuideSegments::deserialize( const std::string &aStr, size_t aPos )
{	
	mSegmentLength = Stubble::deserialize< Real >( aStr, aPos );
	mSegments = Stubble::deserializeObjects< Vector3D< Real > >( aStr, aPos );
	return aPos;	
}

inline std::string FrameSegments::serialize() const
{
	std::ostringstream oss;	
	oss << Stubble::serialize< Time >( mFrame )
		<< Stubble::serializeObjects< OneGuideSegments >( mSegments );
	return oss.str();
}

inline size_t FrameSegments::deserialize( const std::string &aStr, size_t aPos )
{	
	mFrame = Stubble::deserialize< Time >( aStr, aPos );
	mSegments = Stubble::deserializeObjects< OneGuideSegments >( aStr, aPos );
	return aPos;	
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_SEGMENTS_HPP