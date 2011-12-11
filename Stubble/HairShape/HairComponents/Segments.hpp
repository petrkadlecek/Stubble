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
/// Defines an alias representing the guide segments ( stored as array of vertices ).
///-------------------------------------------------------------------------------------------------
typedef std::vector< Vector3D< Real > > Segments;

///----------------------------------------------------------------------------------------------------
/// Store segments of one hair guide.
/// Segments should have uniform length which is also stored in this structure.
/// This structure can be serialized/deserialized.
///----------------------------------------------------------------------------------------------------
struct OneGuideSegments
{
	Real mSegmentLength;	///< Length of the segment
	
	Segments mSegments;	///< The segments

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
/// Defines an alias representing the array of the segments of all hair guides .
///-------------------------------------------------------------------------------------------------
typedef std::vector< OneGuideSegments > GuidesSegments;

///-------------------------------------------------------------------------------------------------
/// Segments of all guides in one time frame.
/// Holds all the guides' segments and time of the frame.
/// This structure can be serialized/deserialized.
///-------------------------------------------------------------------------------------------------
struct FrameSegments
{
	GuidesSegments mSegments;  ///< The segments of all guides

	Time mFrame; ///< The time of the frame

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
/// Defines an alias representing the segments of all hair guides in all time frames.
/// Supports logarithmic addition/deletion/query.
///-------------------------------------------------------------------------------------------------
typedef std::map< Time, FrameSegments > AllFramesSegments;

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing identifier for the hair guide .
///-------------------------------------------------------------------------------------------------
typedef unsigned __int32 GuideId;


///-------------------------------------------------------------------------------------------------
/// Defines an alias representing identifier for a segment on one given guide .
///-------------------------------------------------------------------------------------------------
typedef unsigned __int32 SegmentId;

///-------------------------------------------------------------------------------------------------
/// Defines an alias representing list of identifiers for the hair guides .
///-------------------------------------------------------------------------------------------------
typedef std::vector< GuideId > GuidesIds;

///-------------------------------------------------------------------------------------------------
/// Store all segments of selected hair guides.
/// This structure is used during operations with hair guides undo stack.
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