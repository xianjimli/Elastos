
#ifndef __INPUTWINDOWLIST_H__
#define __INPUTWINDOWLIST_H__

#include "server/InputWindow.h"

/**
 * A specialized list of window information objects backed by an array.
 *
 * This class is part of an InputManager optimization to avoid allocating objects and arrays
 * unnecessarily.  Internally, it keeps an array full of demand-allocated objects that it
 * recycles each time the list is cleared.  The used portion of the array is padded with a null.
 *
 * The contents of the list are intended to be Z-ordered from top to bottom.
 *
 * @hide
 */
class InputWindowList
{
public:
    InputWindowList();

    ~InputWindowList();

    CARAPI_(void) Clear();

    CARAPI_(InputWindow*) Add();

    CARAPI_(InputWindow**) GetNullTerminatedArray();

private:
    InputWindow** mArray;
    Int32 mLength;
    Int32 mCount;
};

#endif //__INPUTWINDOWLIST_H__
