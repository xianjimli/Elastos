
#ifndef __CACCESSIBILITYMANAGERSERVICE_H__
#define __CACCESSIBILITYMANAGERSERVICE_H__

#include "_CAccessibilityManagerService.h"
#include "server/CAccessibilityServiceConnection.h"
#include "text/TextUtils.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/List.h>
#include <elastos/HashMap.h>
#include <elastos/Set.h>
#include <elastos/ElRefBase.h>

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IComponentName> >
{
    size_t operator()(AutoPtr<IComponentName> name) const
    {
        Int32 hashCode;
        assert(name != NULL);
        name->GetHashCode(&hashCode);
        return (size_t)hashCode;
    }
};

template<> struct EqualTo<AutoPtr<IComponentName> >
{
    Boolean operator()(const AutoPtr<IComponentName>& x,
                       const AutoPtr<IComponentName>& y) const
    {
        Boolean isEqual;
        assert(x != NULL);
        x->Equals(y, &isEqual);
        return isEqual;
    }
};

_ELASTOS_NAMESPACE_END

using namespace Elastos::Core::Threading;

CarClass(CAccessibilityManagerService)
{
private:
    class EnabledChangedObserver
        : public ElRefBase
        , public IContentObserver
    {
    public:
        EnabledChangedObserver(
            /* [in] */ CAccessibilityManagerService* host,
            /* [in] */ ISettingsSecure* settingsSecure);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        AutoPtr<CAccessibilityManagerService> mHost;
        AutoPtr<ISettingsSecure> mSettingsSecure;
    };

    class ServicesChangedObserver
        : public ElRefBase
        , public IContentObserver
    {
    public:
        ServicesChangedObserver(
            /* [in] */ CAccessibilityManagerService* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange);
    private:
        AutoPtr<CAccessibilityManagerService> mHost;
    };

public:
    friend class EnabledChangedObserver;
    friend class ServicesChangedObserver;

    CAccessibilityManagerService();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI AddClient(
        /* [in] */ IAccessibilityManagerClient* client);

    CARAPI SendAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event,
        /* [out] */ Boolean* result);

    CARAPI GetAccessibilityServiceList(
        /* [out] */ IObjectContainer** serviceInfos);

    CARAPI Interrupt();

    CARAPI ExecuteMessage(
        /* [in] */ Int32 msgType,
        /* [in] */ IAccessibilityServiceInfo* info,
        /* [in] */ CAccessibilityServiceConnection* service);

private:
    /**
     * Registers a {@link BroadcastReceiver} for the events of
     * adding/changing/removing/restarting a package and boot completion.
     */
    CARAPI_(void) RegisterCauluseChangeAndBootCompletedBroadcastReceiver();

    /**
     * {@link ContentObserver}s for {@link Settings.Secure#ACCESSIBILITY_ENABLED}
     * and {@link Settings.Secure#ENABLED_ACCESSIBILITY_SERVICES} settings.
     */
    CARAPI_(void) RegisterSettingsContentObservers();

    /**
     * Populates the cached list of installed {@link AccessibilityService}s.
     */
    CARAPI_(void) PopulateAccessibilityServiceListLocked();

    /**
     * Performs {@link AccessibilityService}s delayed notification. The delay is configurable
     * and denotes the period after the last event before notifying the service.
     *
     * @param event The event.
     * @param isDefault True to notify default listeners, not default services.
     */
    CARAPI_(void) NotifyAccessibilityServicesDelayedLocked(
        /* [in] */ IAccessibilityEvent* event,
        /* [in] */ Boolean isDefault);

    /**
     * Performs an {@link AccessibilityService} delayed notification. The delay is configurable
     * and denotes the period after the last event before notifying the service.
     *
     * @param service The service.
     * @param event The event.
     */
    CARAPI_(void) NotifyAccessibilityServiceDelayedLocked(
        /* [in] */ CAccessibilityServiceConnection* service,
        /* [in] */ IAccessibilityEvent* event);

    /**
     * Recycles an event if it can be safely recycled. The condition is that no
     * not notified service is interested in the event.
     *
     * @param event The event.
     */
    CARAPI_(void) TryRecycleLocked(
        /* [in] */ IAccessibilityEvent* event);

    /**
     * Notifies a service for a scheduled event given the event type.
     *
     * @param service The service.
     * @param eventType The type of the event to dispatch.
     */
    CARAPI_(void) NotifyEventListenerLocked(
        /* [in] */ CAccessibilityServiceConnection* service,
        /* [in] */ Int32 eventType);

    /**
     * Removes a dead service.
     *
     * @param service The service.
     * @return True if the service was removed, false otherwise.
     */
    CARAPI_(Boolean) RemoveDeadServiceLocked(
        /* [in] */ CAccessibilityServiceConnection* service);

    /**
     * Determines if given event can be dispatched to a service based on the package of the
     * event source and already notified services for that event type. Specifically, a
     * service is notified if it is interested in events from the package and no other service
     * providing the same feedback type has been notified. Exception are services the
     * provide generic feedback (feedback type left as a safety net for unforeseen feedback
     * types) which are always notified.
     *
     * @param service The potential receiver.
     * @param event The event.
     * @param handledFeedbackTypes The feedback types for which services have been notified.
     * @return True if the listener should be notified, false otherwise.
     */
    CARAPI_(Boolean) CanDispathEventLocked(
        /* [in] */ CAccessibilityServiceConnection* service,
        /* [in] */ IAccessibilityEvent* event,
        /* [in] */ Int32 handledFeedbackTypes);

    /**
     * Manages services by starting enabled ones and stopping disabled ones.
     */
    CARAPI_(void) ManageServicesLocked();

    /**
     * Unbinds all bound services.
     */
    CARAPI_(void) UnbindAllServicesLocked();

    /**
     * Populates a list with the {@link ComponentName}s of all enabled
     * {@link AccessibilityService}s.
     *
     * @param enabledServices The list.
     */
    CARAPI_(void) PopulateEnabledServicesLocked(
        /* [in] */ Set<AutoPtr<IComponentName> > enabledServices);

    /**
     * Updates the state of each service by starting (or keeping running) enabled ones and
     * stopping the rest.
     *
     * @param installedServices All installed {@link AccessibilityService}s.
     * @param enabledServices The {@link ComponentName}s of the enabled services.
     */
    CARAPI_(void) UpdateServicesStateLocked(
        /* [in] */ List<AutoPtr<IServiceInfo> > installedServices,
        /* [in] */ Set<AutoPtr<IComponentName> > enabledServices);

    /**
     * Updates the state of {@link android.view.accessibility.AccessibilityManager} clients.
     */
    CARAPI_(void) UpdateClientsLocked();

    /**
     * Handler for delayed event dispatch.
     */
    CARAPI_(void) HandleDelayedEventDispatch(
        /* [in] */ CAccessibilityServiceConnection* service,
        /* [in] */ Int32 eventType);

private:
    static const CString LOG_TAG;

    static const Int32 OWN_PROCESS_ID = -1;// = android.os.Process.myPid();

    static const Int32 DO_SET_SERVICE_INFO = 10;

public:
    //final HandlerCaller mCaller;

    AutoPtr<IContext> mContext;

    Mutex mLock;

    List<AutoPtr<CAccessibilityServiceConnection> > mServices;

    List<AutoPtr<IAccessibilityManagerClient> > mClients;

    HashMap<AutoPtr<IComponentName>, AutoPtr<CAccessibilityServiceConnection> > mComponentNameToServiceMap;

private:
    static Int32 sIdCounter;

    List<AutoPtr<IServiceInfo> > mInstalledServices;

    Set<AutoPtr<IComponentName> > mEnabledServices;

    TextUtils::SimpleStringSplitter* mStringColonSplitter;

    AutoPtr<ICapsuleManager> mCapsuleManager;

    Int32 mHandledFeedbackTypes;

    Boolean mIsEnabled;

    AutoPtr<IApartment> mHandler;
};

#endif //__CACCESSIBILITYMANAGERSERVICE_H__
