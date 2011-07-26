#ifndef STUBBLE_UNDO_STACK_HPP
#define STUBBLE_UNDO_STACK_HPP

#include "HairShape\HairComponents\Segments.hpp"

#include <cassert>

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

///-------------------------------------------------------------------------------------------------
/// Guides segments undo stack. 
///-------------------------------------------------------------------------------------------------
class UndoStack
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Default constructor. 
	///-------------------------------------------------------------------------------------------------
	inline UndoStack();

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	inline ~UndoStack();

	///-------------------------------------------------------------------------------------------------
	/// Clears this object to its blank/initial state. 
	///-------------------------------------------------------------------------------------------------
	inline void clear();

	///-------------------------------------------------------------------------------------------------
	/// Queries if we can undo. 
	///
	/// \return	true if it succeeds, false if it fails. 
	///-------------------------------------------------------------------------------------------------
	inline bool canUndo() const;

	///-------------------------------------------------------------------------------------------------
	/// Queries if we can redo. 
	///
	/// \return	true if it succeeds, false if it fails. 
	///-------------------------------------------------------------------------------------------------
	inline bool canRedo() const;

	///-------------------------------------------------------------------------------------------------
	/// Performs the undo and returns the undo command for SegmentsStorage 
	///
	/// \return	Undo changes for SegmentsStorage. 
	///-------------------------------------------------------------------------------------------------
	inline const PartialStorage & undo();

	///-------------------------------------------------------------------------------------------------
	/// Performs the redo and returns the undo command for SegmentsStorage 
	///
	/// \return	Redo changes for SegmentsStorage. 
	///-------------------------------------------------------------------------------------------------
	inline const PartialStorage & redo();

	///-------------------------------------------------------------------------------------------------
	/// Adds guides segments change to undo stack.
	///
	/// \param [in,out]	aChange	If non-null, the PartialStorage * to add. 
	///-------------------------------------------------------------------------------------------------
	inline void add( PartialStorage * aChange );

private:
	SegmentsChangesStack mChanges;  ///< The segments changes stack

	SegmentsChangesStackPointer mCurrent;   ///< The current stack pointer
};

// inline functions implementation

inline UndoStack::UndoStack()
{
}

inline UndoStack::~UndoStack()
{
	clear();
}

inline void UndoStack::clear()
{
	// Clear memory
	for ( SegmentsChangesStackPointer p = mChanges.begin(); p != mChanges.end(); ++p )
		delete * p;
	mCurrent = mChanges.end();
}

inline bool UndoStack::canUndo() const
{
	return mCurrent != mChanges.begin() && mChanges.size() > 0; 
}

inline bool UndoStack::canRedo() const
{
	return mCurrent != mChanges.end();
}

inline const PartialStorage & UndoStack::undo()
{
	assert( canUndo() );
	--mCurrent; // Move lower in stack
	return **mCurrent;
}

inline const PartialStorage & UndoStack::redo()
{
	assert( canRedo() );
	return **( mCurrent++ ); // Return and then move upper in stack
}

inline void UndoStack::add( PartialStorage * aChange )
{
	// First remove all redo steps
	mChanges.erase( mCurrent, mChanges.end() );
	// Add change to stack
	mChanges.push_back( aChange );
	// Finally set pointer to stack end
	mCurrent = mChanges.end();
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_UNDO_STACK_HPP