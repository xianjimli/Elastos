
#ifndef __INPUTWINDOW_H__
#define __INPUTWINDOW_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

/**
 * Describes input-related window properties for use by the input dispatcher.
 *
 * @hide
 */
class InputWindow
{
public:
    ~InputWindow()
    {}

    CARAPI_(void) Recycle()
    {
        mInputChannel = NULL;
    }

public:
    // The input channel associated with the window.
    AutoPtr<IInputChannel> mInputChannel;

    // The window name.
    String mName;

    // Window layout params attributes.  (WindowManager.LayoutParams)
    Int32 mLayoutParamsFlags;
    Int32 mLayoutParamsType;

    // Dispatching timeout.
    Int64 mDispatchingTimeoutNanos;

    // Window frame area.
    Int32 mFrameLeft;
    Int32 mFrameTop;
    Int32 mFrameRight;
    Int32 mFrameBottom;

    // Window visible frame area.
    Int32 mVisibleFrameLeft;
    Int32 mVisibleFrameTop;
    Int32 mVisibleFrameRight;
    Int32 mVisibleFrameBottom;

    // Window touchable area.
    Int32 mTouchableAreaLeft;
    Int32 mTouchableAreaTop;
    Int32 mTouchableAreaRight;
    Int32 mTouchableAreaBottom;

    // Window is visible.
    Boolean mVisible;

    // Window can receive keys.
    Boolean mCanReceiveKeys;

    // Window has focus.
    Boolean mHasFocus;

    // Window has wallpaper.  (window is the current wallpaper target)
    Boolean mHasWallpaper;

    // Input event dispatching is paused.
    Boolean mPaused;

    // Window layer.
    Int32 mLayer;

    // Id of process and user that owns the window.
    Int32 mOwnerPid;
    Int32 mOwnerUid;
};

#endif //__INPUTWINDOW_H__
