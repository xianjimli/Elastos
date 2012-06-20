
#include "view/CDispatcherState.h"
#include "view/CKeyEvent.h"
#include <Logger.h>

using namespace Elastos::Utility;
using namespace Elastos::Utility::Logging;

ECode CDispatcherState::Reset()
{
    //if (DEBUG)
    //Logger::D(CKeyEvent::TAG, "Reset: " + this);

    mDownKeyCode = 0;
    mDownTarget = NULL;
    mActiveLongPresses.Clear();

    return NOERROR;
}

/**
* Stop any tracking associated with this target.
*/
ECode CDispatcherState::ResetEx(
    /* [in] */ IInterface* target)
{
    if (mDownTarget.Get() == target) {
        //if (DEBUG)
        //Logger::D(TAG, "Reset in " + target + ": " + this);
        mDownKeyCode = 0;
        mDownTarget = NULL;
    }

    return NOERROR;
}

/**
* Start tracking the key code associated with the given event.  This
* can only be called on a key down.  It will allow you to see any
* Int64 press associated with the key, and will result in
* {@link KeyEvent#isTracking} return true on the Int64 press and up
* events.
*
* <p>This is only needed if you are directly dispatching events, rather
* than handling them in {@link Callback#onKeyDown}.
*/
ECode CDispatcherState::StartTracking(
    /* [in] */ IKeyEvent* event,
    /* [in] */ IInterface* target)
{
    Int32 action;
    event->GetAction(&action);
    if (action != KeyEvent_ACTION_DOWN) {
        Logger::E(
            CKeyEvent::TAG, "Can only start tracking on a down event");

        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    //if (DEBUG)
    //Logger::D(CKeyEvent::TAG, "Start trackingt in " + target + ": " + this);
    event->GetKeyCode(&mDownKeyCode);
    mDownTarget = target;

    return NOERROR;
}

/**
* Return true if the key event is for a key code that is currently
* being tracked by the dispatcher.
*/
ECode CDispatcherState::IsTracking(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* isTracking)
{
    if (isTracking == NULL) {
        return E_INVALID_ARGUMENT;
    }

    assert(event);

    Int32 keyCode;
    event->GetKeyCode(&keyCode);

    *isTracking = mDownKeyCode == keyCode;

    return NOERROR;
}

/**
* Keep track of the given event's key code as having performed an
* action with a Int64 press, so no action should occur on the up.
* <p>This is only needed if you are directly dispatching events, rather
* than handling them in {@link Callback#onKeyLongPress}.
*/
ECode CDispatcherState::PerformedLongPress(
    /* [in] */ IKeyEvent* event)
{
    assert(event);

    Int32 keyCode;
    event->GetKeyCode(&keyCode);

    mActiveLongPresses.Insert(mActiveLongPresses.Begin(), keyCode);

    return NOERROR;
}

/**
* Handle key up event to stop tracking.  This resets the dispatcher state,
* and updates the key event state based on it.
* <p>This is only needed if you are directly dispatching events, rather
* than handling them in {@link Callback#onKeyUp}.
*/
ECode CDispatcherState::HandleUpEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyCode;
    event->GetKeyCode(&keyCode);

    //if (DEBUG)
    //    Logger::D(CKeyEvent::TAG, "Handle key up " + event + ": " + this);

    Set<Int32>::Iterator iter = mActiveLongPresses.Find(keyCode);
    if (iter != mActiveLongPresses.End()) {
        //if (DEBUG)
        //Log.v(TAG, "  Index: " + index);

        ((CKeyEvent*)event)->mFlags |=
            KeyEvent_FLAG_CANCELED | KeyEvent_FLAG_CANCELED_LONG_PRESS;
        mActiveLongPresses.Erase(iter);
    }

    if (mDownKeyCode == keyCode) {
        if (CKeyEvent::DEBUG)
            Logger::D(CKeyEvent::TAG, "  Tracking!");

        ((CKeyEvent*)event)->mFlags |= KeyEvent_FLAG_TRACKING;
        mDownKeyCode = 0;
        mDownTarget = NULL;
    }

    return NOERROR;
}
