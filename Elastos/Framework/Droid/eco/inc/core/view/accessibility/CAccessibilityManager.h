
#ifndef __CACCESSIBILITYMANAGER_H__
#define __CACCESSIBILITYMANAGER_H__

#include "_CAccessibilityManager.h"
#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class CAccessibilityManagerHelper;

CarClass(CAccessibilityManager)
{
public:
    friend class CAccessibilityManagerHelper;

    CAccessibilityManager();

    /**
     * Returns if the {@link AccessibilityManager} is enabled.
     *
     * @return True if this {@link AccessibilityManager} is enabled, false otherwise.
     */
   CARAPI  IsEnabled(
        /* [out] */ Boolean* isEnabled);

    /**
     * Sends an {@link AccessibilityEvent}. If this {@link AccessibilityManager} is not
     * enabled the call is a NOOP.
     *
     * @param event The {@link AccessibilityEvent}.
     *
     * @throws IllegalStateException if a client tries to send an {@link AccessibilityEvent}
     *         while accessibility is not enabled.
     */
    CARAPI SendAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    /**
     * Requests interruption of the accessibility feedback from all accessibility services.
     */
    CARAPI Interrupt();

    /**
     * Returns the {@link ServiceInfo}s of the installed accessibility services.
     *
     * @return An unmodifiable list with {@link ServiceInfo}s.
     */
    CARAPI GetAccessibilityServiceList(
        /* [out] */ IObjectContainer** container);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI_(void) HandleSetEnabled(
        /* [in] */ Boolean enabled);

private:
    static const CString TAG;
    static const Int32 DO_SET_ENABLED = 10;

    static Mutex sInstanceSync;
    static AutoPtr<ILocalAccessibilityManager> sInstance;

public:
    AutoPtr<IAccessibilityManager> mService;

    AutoPtr<IApartment> mHandler;

    Boolean mIsEnabled;

    AutoPtr<IAccessibilityManagerClient> mClient;
};

#endif //__CACCESSIBILITYMANAGER_H__
