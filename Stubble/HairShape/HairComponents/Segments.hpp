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
	/// 
	/// \param	aOutputStream	Output stream
	///-------------------------------------------------------------------------------------------------
	inline void serialize( std::ostream & aOutputStream ) const;

	///-------------------------------------------------------------------------------------------------
	/// Deserialize object.	
	///
	/// \param	aInputStream	Input stream
	///-------------------------------------------------------------------------------------------------
	inline void deserialize( std::istream & aInputStream );
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
	/// 
	/// \param	aOutputStream	Output stream
	///-------------------------------------------------------------------------------------------------
	inline void serialize( std::ostream & aOutputStream ) const;

	///-------------------------------------------------------------------------------------------------
	/// Deserialize object.	
	///
	/// \param	aInputStream	Input stream
	///-------------------------------------------------------------------------------------------------
	inline void deserialize( std::istream & aInputStream );
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

inline void OneGuideSegments::serialize( std::ostream & aOutputStream ) const
{
	Stubble::serialize( mSegmentLength, aOutputStream );
	Stubble::serializePrimitives( mSegments, aOutputStream );
}

inline void OneGuideSegments::deserialize( std::istream & aInputStream )
{	
	Stubble::deserialize( mSegmentLength, aInputStream );
	Stubble::deserializePrimitives( mSegments, aInputStream );
}

inline void FrameSegments::serialize( std::ostream & aOutputStream ) const
{
	Stubble::serialize( mFrame, aOutputStream );
	Stubble::serializeObjects( mSegments, aOutputStream );
}

inline void FrameSegments::deserialize( std::istream & aInputStream )
{	
	Stubble::deserialize( mFrame, aInputStream );
	Stubble::deserializeObjects( mSegments, aInputStream );
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_SEGMENTS_HPP