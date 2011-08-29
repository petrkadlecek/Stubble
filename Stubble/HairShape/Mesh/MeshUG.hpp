#ifndef STUBBLE_MESH_UG_HPP
#define STUBBLE_MESH_UG_HPP

#include "HairShape\Mesh\Triangle.hpp"

namespace Stubble
{

namespace HairShape
{

///-------------------------------------------------------------------------------------------------
/// Mesh triangles uniform grid used for collisions
///-------------------------------------------------------------------------------------------------
class MeshUG
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	MeshUG();

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	~MeshUG();

	///-------------------------------------------------------------------------------------------------
	///  Builds the uniform grid. 
	///
	/// \param [in,out]	aTriangles	the mesh triangles ( aTriangles are destroyed in process ). 
	///-------------------------------------------------------------------------------------------------
	void build( Triangles & aTriangles );

	/* TODO Add methods for collisions detection */

	///-------------------------------------------------------------------------------------------------
	/// Sets the dirty bit. 
	///-------------------------------------------------------------------------------------------------
	inline void setDirty();

	///-------------------------------------------------------------------------------------------------
	/// Query if this object is dirty. 
	///
	/// \return	true if dirty, false if not. 
	///-------------------------------------------------------------------------------------------------
	inline bool isDirty() const;

private:
	bool mDirtyBit; ///< true to dirty bit
};

// inline functions implementation

inline void MeshUG::setDirty()
{
	mDirtyBit = true;
}

inline bool MeshUG::isDirty() const
{
	return mDirtyBit;
}

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_MESH_UG_HPP