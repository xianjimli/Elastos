
#ifndef __ACCESSIBILITYSERVICECONNECTION_H__
#define __ACCESSIBILITYSERVICECONNECTION_H__

#include "ext/frameworkdef.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/HashSet.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class AccessibilityServiceConnection
    : public ElRefBase
    , public IAccessibilityServiceConnection
{
public:
    AccessibilityServiceConnection();

    ~AccessibilityServiceConnection();

    CARAPI constructor(
        /* [in] */ IAccessibilityManager* manager,
        /* [in] */ IComponentName* componentName);

    CARAPI SetServiceInfo(
        /* [in] */ IAccessibilityServiceInfo* info);

    CARAPI OnServiceConnected(
        /* [in] */ IComponentName* name,
        /* [in] */ IBinder* service);

    CARAPI OnServiceDisconnected(
        /* [in] */ IComponentName* name);

public:
    Int32 mId;

    AutoPtr<IBinder> mService;

    AutoPtr<IEventListener> mServiceInterface;

    Int32 mEventTypes;

    Int32 mFeedbackType;

    HashSet<String> mPackageNames;

    Boolean mIsDefault;

    Int64 mNotificationTimeout;

    Boolean mIsActive;

    AutoPtr<IComponentName> mComponentName;

    AutoPtr<IIntent> mIntent;

    // the events pending events to be dispatched to this service
    ArrayOf<AutoPtr<IAccessibilityEvent> >* mPendingEvents;
};

#endif //__ACCESSIBILITYSERVICECONNECTION_H__
