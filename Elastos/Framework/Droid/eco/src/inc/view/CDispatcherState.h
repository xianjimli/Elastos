
#ifndef __CDISPATCHERSTATE_H__
#define __CDISPATCHERSTATE_H__

#include "_CDispatcherState.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Set.h>

/**
* Use with {@link KeyEvent#dispatch(Callback, DispatcherState, Object)}
* for more advanced key dispatching, such as Int64 presses.
*/
CarClass(CDispatcherState)
{
public:
    CARAPI Reset();

    CARAPI ResetEx(
        /* [in] */ IInterface* target);

    CARAPI StartTracking(
        /* [in] */ IKeyEvent* event,
        /* [in] */ IInterface* target);

    CARAPI IsTracking(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* isTracking);

    CARAPI PerformedLongPress(
        /* [in] */ IKeyEvent* event);

    CARAPI HandleUpEvent(
        /* [in] */ IKeyEvent* event);

private:
    Int32 mDownKeyCode;
    AutoPtr<IInterface> mDownTarget;
    Set<Int32> mActiveLongPresses;
};
#endif //__CDISPATCHERSTATE_H__
