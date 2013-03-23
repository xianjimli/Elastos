
#ifndef __CACCESSIBILITYSERVICECONNECTION_H__
#define __CACCESSIBILITYSERVICECONNECTION_H__

#include "_CAccessibilityServiceConnection.h"
#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashSet.h>
#include <elastos/Mutex.h>
#include <elastos/HashMap.h>

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IAccessibilityEvent> >
{
    size_t operator()(AutoPtr<IAccessibilityEvent> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

_ELASTOS_NAMESPACE_END

using namespace Elastos::Core::Threading;

CarClass(CAccessibilityServiceConnection)
{
public:
    CAccessibilityServiceConnection();

    CARAPI constructor(
        /* [in] */ IAccessibilityManager* manager,
        /* [in] */ IComponentName* componentName);

    /**
     * Binds to the accessibility service.
     */
    CARAPI_(void) Bind();

    /**
     * Unbinds form the accessibility service and removes it from the data
     * structures for service management.
     */
    CARAPI_(void) Unbind();

    /**
     * Returns if the service is configured i.e. at least event types of interest
     * and feedback type must be set.
     *
     * @return True if the service is configured, false otherwise.
     */
    CARAPI_(Boolean) IsConfigured();

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

    HashSet<String> mCapsuleNames;

    Boolean mIsDefault;

    Int64 mNotificationTimeout;

    Boolean mIsActive;

    AutoPtr<IComponentName> mComponentName;

    AutoPtr<IIntent> mIntent;

    // the events pending events to be dispatched to this service
    HashMap<Int32, AutoPtr<IAccessibilityEvent> > mPendingEvents;
};

#endif //__CACCESSIBILITYSERVICECONNECTION_H__
