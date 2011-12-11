#ifndef STUBBLE_UNDO_STACK_HPP
#define STUBBLE_UNDO_STACK_HPP

#include "HairShape\HairComponents\Segments.hpp"

#include <cassert>
#include <list>

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

///-------------------------------------------------------------------------------------------------
/// Hair guides' segments undo stack. 
/// Enables undo and redo operations on all hair guides' segments in current frame.
/// Stores only segments of modified hair guides.
/// The undo stack depth is limited by constant.
///-------------------------------------------------------------------------------------------------
class UndoStack
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Default empty constructor. 
	///-------------------------------------------------------------------------------------------------
	inline UndoStack();

	///-------------------------------------------------------------------------------------------------
	/// Finaliser. 
	///-------------------------------------------------------------------------------------------------
	inline ~UndoStack();

	///-------------------------------------------------------------------------------------------------
	/// Clears the undo stack and release memory.
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
	/// Performs the undo and returns the undo changes for SegmentsStorage. 
	///
	/// \return	Undo changes for SegmentsStorage. 
	///-------------------------------------------------------------------------------------------------
	inline const PartialStorage & undo();

	///-------------------------------------------------------------------------------------------------
	/// Performs the redo and returns the redo changes for SegmentsStorage 
	///
	/// \return	Redo changes for SegmentsStorage. 
	///-------------------------------------------------------------------------------------------------
	inline const PartialStorage & redo();

	///-------------------------------------------------------------------------------------------------
	/// Updates the current stack element after undo has been called.
	/// The hair segments that differs from segments returned by undo function must be put
	/// into undo stack.
	///
	/// \param [in,out]	aChange	The change of element. 
	///-------------------------------------------------------------------------------------------------
	inline void updateAfterUndo( PartialStorage * aChange );

	///-------------------------------------------------------------------------------------------------
	/// Updates the current stack element after redo has been called.
	/// The hair segments that differs from segments returned by redo function must be put
	/// into undo stack.
	/// 
	/// \param [in,out]	aChange	The change of element. 
	///-------------------------------------------------------------------------------------------------
	inline void updateAfterRedo( PartialStorage * aChange );

	///-------------------------------------------------------------------------------------------------
	/// Adds guides' segments change to undo stack.
	/// After guides' segments has been modified, we store the changed old segments to undo stack.
	/// Not modified segments should not be inputed.
	///
	/// \param [in,out]	aChange		The segments before modification.
	///-------------------------------------------------------------------------------------------------
	void add( PartialStorage * aChange );

private:
	///-------------------------------------------------------------------------------------------------
	/// Notifies Maya that internal data have changed by incrementing opcount.
	/// This is important during save, because maya saves only changed scenes and has no other
	/// way of knowing whether our object has changed internally. 
	///-------------------------------------------------------------------------------------------------
	void incrementOp();

	static const size_t MAX_STACK_SIZE = 100;  ///< The maximum depth of undo stack

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing the segments changes stack.
	///-------------------------------------------------------------------------------------------------
	typedef std::list< PartialStorage * > SegmentsChangesStack;

	///-------------------------------------------------------------------------------------------------
	/// Defines an alias representing the segments changes stack pointer .
	///-------------------------------------------------------------------------------------------------
	typedef SegmentsChangesStack::iterator SegmentsChangesStackPointer;

	SegmentsChangesStack mChanges;  ///< The segments changes stack

	SegmentsChangesStackPointer mCurrent;   ///< The current stack pointer which points to first redo change in stack
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
	mChanges.clear();
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
	incrementOp();
	return **mCurrent;
}

inline const PartialStorage & UndoStack::redo()
{
	assert( canRedo() );
	incrementOp();
	return **( mCurrent++ ); // Return and then move upper in stack
}

inline void UndoStack::updateAfterUndo( PartialStorage * aChange )
{
	// Must change current item
	delete *mCurrent;
	*mCurrent = aChange;
}

inline void UndoStack::updateAfterRedo( PartialStorage * aChange )
{
	// Must change the previous item
	SegmentsChangesStackPointer mPrevious = mCurrent;
	--mPrevious;
	delete *mPrevious;
	*mPrevious = aChange;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_UNDO_STACK_HPP