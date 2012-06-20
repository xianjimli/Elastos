
#include "server/InputWindowList.h"

/**
 * Creates an empty list.
 */
InputWindowList::InputWindowList() :
    mLength(8),
    mCount(0)
{
    mArray = (InputWindow**)calloc(mLength, sizeof(InputWindow*));
}

InputWindowList::~InputWindowList()
{
    free(mArray);
}

/**
 * Clears the list.
 */
void InputWindowList::Clear()
{
    if (mCount == 0) {
        return;
    }

    Int32 count = mCount;
    mCount = 0;
    mArray[count] = mArray[0];
    while (count > 0) {
        count -= 1;
        mArray[count]->Recycle();
    }
    mArray[0] = NULL;
}

/**
 * Adds an uninitialized input window object to the list and returns it.
 */
InputWindow* InputWindowList::Add()
{
    if (mCount + 1 == mLength) {
        InputWindow** oldArray = mArray;
        mLength *= 2;
        mArray = (InputWindow**)calloc(mLength, sizeof(InputWindow*));
        for (Int32 i = 0; i < mCount; ++i) {
            mArray[i] = oldArray[i];
        }
        free(oldArray);
    }

    // Grab object from tail (after used section) if available.
    InputWindow* item = mArray[mCount + 1];
    if (item == NULL) {
        item = new InputWindow();
    }

    mArray[mCount] = item;
    mCount += 1;
    mArray[mCount] = NULL;
    return item;
}

/**
 * Gets the input window objects as a null-terminated array.
 * @return The input window array.
 */
InputWindow** InputWindowList::GetNullTerminatedArray()
{
    return mArray;
}
