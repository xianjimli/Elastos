
#include "server/CAccessibilityManagerService.h"
#include "os/Binder.h"

const CString CAccessibilityManagerService::LOG_TAG = "AccessibilityManagerService";

const Int32 CAccessibilityManagerService::OWN_PROCESS_ID;// = android.os.Process.myPid();

const Int32 CAccessibilityManagerService::DO_SET_SERVICE_INFO;

Int32 CAccessibilityManagerService::sIdCounter = 0;

CAccessibilityManagerService::EnabledChangedObserver::EnabledChangedObserver(
    /* [in] */ CAccessibilityManagerService* host,
    /* [in] */ ISettingsSecure* settingsSecure)
    : mHost(host)
    , mSettingsSecure(settingsSecure)
{}

PInterface CAccessibilityManagerService::EnabledChangedObserver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IContentObserver*)this;
    }
    else if (riid == EIID_IContentObserver) {
        return (IContentObserver*)this;
    }
    return NULL;
}

UInt32 CAccessibilityManagerService::EnabledChangedObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAccessibilityManagerService::EnabledChangedObserver::Release()
{
    return ElRefBase::Release();
}

ECode CAccessibilityManagerService::EnabledChangedObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccessibilityManagerService::EnabledChangedObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    //super.onChange(selfChange);

    Int32 value;
    AutoPtr<IContentResolver> contentResolver;
    mHost->mContext->GetContentResolver((IContentResolver**)&contentResolver);
    mSettingsSecure->GetInt32(contentResolver,
        String(SettingsSecure_ACCESSIBILITY_ENABLED), 0, &value);
    mHost->mIsEnabled = value == 1;

    Mutex::Autolock lock(mHost->mLock);
    if (mHost->mIsEnabled) {
        mHost->ManageServicesLocked();
    }
    else {
        mHost->UnbindAllServicesLocked();
    }
    mHost->UpdateClientsLocked();

    return NOERROR;
}

CAccessibilityManagerService::ServicesChangedObserver::ServicesChangedObserver(
    /* [in] */ CAccessibilityManagerService* host)
    : mHost(host)
{}

PInterface CAccessibilityManagerService::ServicesChangedObserver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IContentObserver*)this;
    }
    else if (riid == EIID_IContentObserver) {
        return (IContentObserver*)this;
    }
    return NULL;
}

UInt32 CAccessibilityManagerService::ServicesChangedObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAccessibilityManagerService::ServicesChangedObserver::Release()
{
    return ElRefBase::Release();
}

ECode CAccessibilityManagerService::ServicesChangedObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccessibilityManagerService::ServicesChangedObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    //super.onChange(selfChange);

    Mutex::Autolock lock(mHost->mLock);
    mHost->ManageServicesLocked();

    return NOERROR;
}

CAccessibilityManagerService::CAccessibilityManagerService()
    : mHandledFeedbackTypes(0)
    , mIsEnabled(FALSE)
{
    mStringColonSplitter = new TextUtils::SimpleStringSplitter(':');
    CApartment::New(TRUE, (IApartment**)&mHandler);
    mHandler->Start(ApartmentAttr_New);
}

ECode CAccessibilityManagerService::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    //mCapsuleManager = mContext.getPackageManager();
    //mCaller = new HandlerCaller(context, this);

    //RegisterCauluseChangeAndBootCompletedBroadcastReceiver();
    RegisterSettingsContentObservers();

    return NOERROR;
}

/**
 * Registers a {@link BroadcastReceiver} for the events of
 * adding/changing/removing/restarting a package and boot completion.
 */
void CAccessibilityManagerService::RegisterCauluseChangeAndBootCompletedBroadcastReceiver()
{
    //AutoPtr<IContext> context = mContext;

    // PackageMonitor monitor = new PackageMonitor() {
    //     @Override
    //     public void onSomePackagesChanged() {
    //         synchronized (mLock) {
    //             populateAccessibilityServiceListLocked();
    //             manageServicesLocked();
    //         }
    //     }

    //     @Override
    //     public boolean onHandleForceStop(Intent intent, String[] packages,
    //             int uid, boolean doit) {
    //         synchronized (mLock) {
    //             boolean changed = false;
    //             Iterator<ComponentName> it = mEnabledServices.iterator();
    //             while (it.hasNext()) {
    //                 ComponentName comp = it.next();
    //                 String compPkg = comp.getPackageName();
    //                 for (String pkg : packages) {
    //                     if (compPkg.equals(pkg)) {
    //                         if (!doit) {
    //                             return true;
    //                         }
    //                         it.remove();
    //                         changed = true;
    //                     }
    //                 }
    //             }
    //             if (changed) {
    //                 it = mEnabledServices.iterator();
    //                 StringBuilder str = new StringBuilder();
    //                 while (it.hasNext()) {
    //                     if (str.length() > 0) {
    //                         str.append(':');
    //                     }
    //                     str.append(it.next().flattenToShortString());
    //                 }
    //                 Settings.Secure.putString(mContext.getContentResolver(),
    //                         Settings.Secure.ENABLED_ACCESSIBILITY_SERVICES,
    //                         str.toString());
    //                 manageServicesLocked();
    //             }
    //             return false;
    //         }
    //     }

    //     @Override
    //     public void onReceive(Context context, Intent intent) {
    //         if (intent.getAction() == Intent.ACTION_BOOT_COMPLETED) {
    //             synchronized (mLock) {
    //                 populateAccessibilityServiceListLocked();

    //                 // get the accessibility enabled setting on boot
    //                 mIsEnabled = Settings.Secure.getInt(mContext.getContentResolver(),
    //                         Settings.Secure.ACCESSIBILITY_ENABLED, 0) == 1;

    //                 // if accessibility is enabled inform our clients we are on
    //                 if (mIsEnabled) {
    //                     updateClientsLocked();
    //                 }

    //                 manageServicesLocked();
    //             }

    //             return;
    //         }

    //         super.onReceive(context, intent);
    //     }
    // };

    // package changes
    //monitor.register(context, true);

    // boot completed
    // AutoPtr<IIntentFilter> bootFiler;
    // ASSERT_SUCCEEDED(CIntentFilter::New(Intent_ACTION_BOOT_COMPLETED,
    //         (IIntentFilter**)&bootFiler));
    // mContext->RegisterReceiver(monitor, bootFiler);
}

/**
 * {@link ContentObserver}s for {@link Settings.Secure#ACCESSIBILITY_ENABLED}
 * and {@link Settings.Secure#ENABLED_ACCESSIBILITY_SERVICES} settings.
 */
void CAccessibilityManagerService::RegisterSettingsContentObservers()
{
    AutoPtr<IContentResolver> contentResolver;
    mContext->GetContentResolver((IContentResolver**)&contentResolver);

    AutoPtr<ISettingsSecure> settingsSecure;
    CSettingsSecure::AcquireSingleton((ISettingsSecure**)&settingsSecure);
    AutoPtr<IUri> enabledUri;
    settingsSecure->GetUriFor(String(SettingsSecure_ACCESSIBILITY_ENABLED), (IUri**)&enabledUri);
    EnabledChangedObserver* obj1 = new EnabledChangedObserver(this, settingsSecure);
    AutoPtr<IContentObserver> enabledObserver =
            (IContentObserver*)obj1->Probe(EIID_IContentObserver);
    contentResolver->RegisterContentObserver(enabledUri, FALSE, enabledObserver);

    AutoPtr<IUri> providersUri;
    settingsSecure->GetUriFor(String(SettingsSecure_ENABLED_ACCESSIBILITY_SERVICES),
            (IUri**)&providersUri);
    ServicesChangedObserver* obj2 = new ServicesChangedObserver(this);
    AutoPtr<IContentObserver> servicesObserver =
            (IContentObserver*)obj2->Probe(EIID_IContentObserver);
    contentResolver->RegisterContentObserver(providersUri, FALSE, servicesObserver);
}

ECode CAccessibilityManagerService::AddClient(
    /* [in] */ IAccessibilityManagerClient* client)
{
    Mutex::Autolock lock(mLock);
    //try {
    client->SetEnabled(mIsEnabled);
    mClients.PushBack(client);
    // } catch (RemoteException re) {
    //     Slog.w(LOG_TAG, "Dead AccessibilityManagerClient: " + client, re);
    // }

    return NOERROR;
}

ECode CAccessibilityManagerService::SendAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    {
        Mutex::Autolock lock(mLock);
        NotifyAccessibilityServicesDelayedLocked(event, FALSE);
        NotifyAccessibilityServicesDelayedLocked(event, TRUE);
    }
    // event not scheduled for dispatch => recycle
    if (mHandledFeedbackTypes == 0) {
        event->Recycle();
    }
    else {
        mHandledFeedbackTypes = 0;
    }

    *result = (OWN_PROCESS_ID != Binder::GetCallingPid());

    return NOERROR;
}

ECode CAccessibilityManagerService::GetAccessibilityServiceList(
    /* [out] */ IObjectContainer** serviceInfos)
{
    VALIDATE_NOT_NULL(serviceInfos);

    Mutex::Autolock lock(mLock);

    IObjectContainer* ret = NULL;
    CObjectContainer::New(&ret);
    List<AutoPtr<IServiceInfo> >::Iterator it;
    for (it = mInstalledServices.Begin(); it != mInstalledServices.End(); ++it) {
        ret->Add((IServiceInfo*)*it);
    }
    *serviceInfos = ret;

    return NOERROR;
}

ECode CAccessibilityManagerService::Interrupt()
{
    Mutex::Autolock lock(mLock);

    List<AutoPtr<CAccessibilityServiceConnection> >::Iterator it;
    for (it = mServices.Begin(); it != mServices.End(); ++it) {
        AutoPtr<CAccessibilityServiceConnection> service = *it;
        //try {
        ECode ec = service->mServiceInterface->OnInterrupt();
        if (FAILED(ec)) {
            //ensure the position of the iterator will be currect after the delete operation.
            ++it;
            RemoveDeadServiceLocked(service);
            it--;
        }
        // } catch (RemoteException re) {
        //     if (re instanceof DeadObjectException) {
        //         Slog.w(LOG_TAG, "Dead " + service.mService + ". Cleaning up.");
        //         if (removeDeadServiceLocked(service)) {
        //             count--;
        //             i--;
        //         }
        //     } else {
        //         Slog.e(LOG_TAG, "Error during sending interrupt request to "
        //                 + service.mService, re);
        //     }
        // }
    }

    return NOERROR;
}

/**
 * Populates the cached list of installed {@link AccessibilityService}s.
 */
void CAccessibilityManagerService::PopulateAccessibilityServiceListLocked()
{
    mInstalledServices.Clear();

    AutoPtr<IIntent> intent;
    CIntent::New(String(AccessibilityService_SERVICE_INTERFACE), (IIntent**)&intent);
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    String type;
    intent->ResolveTypeIfNeeded(resolver, &type);
    AutoPtr<IObjectContainer> installedServices;
    mCapsuleManager->QueryIntentServices(
            intent, type, CapsuleManager_GET_SERVICES, (IObjectContainer**)&installedServices);
    if (installedServices != NULL) {
        AutoPtr<IObjectEnumerator> enumerator;
        installedServices->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        Boolean hasNext;
        while(enumerator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IResolveInfo> resolveInfo;
            enumerator->Current((IInterface**)&resolveInfo);
            AutoPtr<IServiceInfo> serviceInfo;
            resolveInfo->GetServiceInfo((IServiceInfo**)&serviceInfo);
            mInstalledServices.PushBack(serviceInfo);
        }
    }
}

/**
 * Performs {@link AccessibilityService}s delayed notification. The delay is configurable
 * and denotes the period after the last event before notifying the service.
 *
 * @param event The event.
 * @param isDefault True to notify default listeners, not default services.
 */
void CAccessibilityManagerService::NotifyAccessibilityServicesDelayedLocked(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Boolean isDefault)
{
    //try {
    List<AutoPtr<CAccessibilityServiceConnection> >::Iterator it;
    for (it = mServices.Begin(); it != mServices.End(); ++it) {
        AutoPtr<CAccessibilityServiceConnection> service = *it;
        if (service->mIsDefault == isDefault) {
            if (CanDispathEventLocked(service, event, mHandledFeedbackTypes)) {
                mHandledFeedbackTypes |= service->mFeedbackType;
                NotifyAccessibilityServiceDelayedLocked(service, event);
            }
        }
    }
    // } catch (IndexOutOfBoundsException oobe) {
    //     // An out of bounds exception can happen if services are going away
    //     // as the for loop is running. If that happens, just bail because
    //     // there are no more services to notify.
    //     return;
    // }
}

/**
 * Performs an {@link AccessibilityService} delayed notification. The delay is configurable
 * and denotes the period after the last event before notifying the service.
 *
 * @param service The service.
 * @param event The event.
 */
void CAccessibilityManagerService::NotifyAccessibilityServiceDelayedLocked(
    /* [in] */ CAccessibilityServiceConnection* service,
    /* [in] */ IAccessibilityEvent* event)
{
    Mutex::Autolock lock(mLock);
    Int32 eventType;
    event->GetEventType(&eventType);
    AutoPtr<IAccessibilityEvent> oldEvent;
    HashMap<Int32, AutoPtr<IAccessibilityEvent> >::Iterator oldIt
            = service->mPendingEvents.Find(eventType);
    if (oldIt != service->mPendingEvents.End()) {
        oldEvent = oldIt->mSecond;
    }
    service->mPendingEvents[eventType] = event;

    //Int32 what = eventType | (service->mId << 16);
    if (oldEvent != NULL) {
        //mHandler.removeMessages(what);
        void (STDCALL CAccessibilityManagerService::*pHandlerFunc)(CAccessibilityServiceConnection*, Int32);
        pHandlerFunc = &CAccessibilityManagerService::HandleDelayedEventDispatch;
        mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
        TryRecycleLocked(oldEvent);
    }

    void (STDCALL CAccessibilityManagerService::*pHandlerFunc)(CAccessibilityServiceConnection*, Int32);
    pHandlerFunc = &CAccessibilityManagerService::HandleDelayedEventDispatch;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Int32)service);
    params->WriteInt32(eventType);

    mHandler->PostCppCallbackDelayed(
        (Handle32)this, *(Handle32*)&pHandlerFunc,
        params, 0, service->mNotificationTimeout);

    // Message message = mHandler.obtainMessage(what, service);
    // message.arg1 = event.getEventType();
    // mHandler.sendMessageDelayed(message, service.mNotificationTimeout);
}

/**
 * Recycles an event if it can be safely recycled. The condition is that no
 * not notified service is interested in the event.
 *
 * @param event The event.
 */
void CAccessibilityManagerService::TryRecycleLocked(
    /* [in] */ IAccessibilityEvent* event)
{
    if (event == NULL) {
        return;
    }
    Int32 eventType;
    event->GetEventType(&eventType);
    List<AutoPtr<CAccessibilityServiceConnection> > services = mServices;

    // linear in the number of service which is not large
    List<AutoPtr<CAccessibilityServiceConnection> >::Iterator it;
    for (it = services.Begin(); it != services.End(); ++it) {
        AutoPtr<CAccessibilityServiceConnection> service = *it;
        HashMap<Int32, AutoPtr<IAccessibilityEvent> >::Iterator eventIt
                = service->mPendingEvents.Find(eventType);
        if ((eventIt != service->mPendingEvents.End())
                && (eventIt->mSecond.Get() == event)) {
            return ;
        }
    }
    event->Recycle();
}

/**
 * Notifies a service for a scheduled event given the event type.
 *
 * @param service The service.
 * @param eventType The type of the event to dispatch.
 */
void CAccessibilityManagerService::NotifyEventListenerLocked(
    /* [in] */ CAccessibilityServiceConnection* service,
    /* [in] */ Int32 eventType)
{
    AutoPtr<IEventListener> listener = service->mServiceInterface;
    AutoPtr<IAccessibilityEvent> event;
    HashMap<Int32, AutoPtr<IAccessibilityEvent> >::Iterator eventIt
            = service->mPendingEvents.Find(eventType);
    if (eventIt != service->mPendingEvents.End()) {
        event = eventIt->mSecond;
    }

    //ry {
    ECode ec = listener->OnAccessibilityEvent(event);
    if (FAILED(ec)) {
        Mutex::Autolock lock(mLock);
        RemoveDeadServiceLocked(service);
    }
    // if (Config.DEBUG) {
    //     Slog.i(LOG_TAG, "Event " + event + " sent to " + listener);
    // }
    // } catch (RemoteException re) {
    //     if (re instanceof DeadObjectException) {
    //         Slog.w(LOG_TAG, "Dead " + service.mService + ". Cleaning up.");
    //         synchronized (mLock) {
    //             removeDeadServiceLocked(service);
    //         }
    //     } else {
    //         Slog.e(LOG_TAG, "Error during sending " + event + " to " + service.mService, re);
    //     }
    // }
}

/**
 * Removes a dead service.
 *
 * @param service The service.
 * @return True if the service was removed, false otherwise.
 */
Boolean CAccessibilityManagerService::RemoveDeadServiceLocked(
    /* [in] */ CAccessibilityServiceConnection* service)
{
    mServices.Remove(service);
    void (STDCALL CAccessibilityManagerService::*pHandlerFunc)(CAccessibilityServiceConnection*, Int32);
    pHandlerFunc = &CAccessibilityManagerService::HandleDelayedEventDispatch;
    mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    // if (Config.DEBUG) {
    //     Slog.i(LOG_TAG, "Dead service " + service.mService + " removed");
    // }

    if (mServices.Begin() ==  mServices.End()) {
        mIsEnabled = FALSE;
        UpdateClientsLocked();
    }

    return TRUE;
}

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
Boolean CAccessibilityManagerService::CanDispathEventLocked(
    /* [in] */ CAccessibilityServiceConnection* service,
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Int32 handledFeedbackTypes)
{
    if (!service->IsConfigured()) {
        return FALSE;
    }

    // if (!service->Service->IsBinderAlive()) {
    //     RemoveDeadServiceLocked(service);
    //     return FALSE;
    // }

    Int32 eventType;
    event->GetEventType(&eventType);
    if ((service->mEventTypes & eventType) != eventType) {
        return FALSE;
    }

    HashSet<String> capsuleNames = service->mCapsuleNames;
    AutoPtr<ICharSequence> capsuleName;
    event->GetCapsuleName((ICharSequence**)&capsuleName);

    String s;
    capsuleName->ToString(&s);
    if (capsuleNames.Begin() == capsuleNames.End() || capsuleNames.Find(s) != capsuleNames.End()) {
        Int32 feedbackType = service->mFeedbackType;
        if ((handledFeedbackTypes & feedbackType) != feedbackType
                || feedbackType == AccessibilityServiceInfo_FEEDBACK_GENERIC) {
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * Manages services by starting enabled ones and stopping disabled ones.
 */
void CAccessibilityManagerService::ManageServicesLocked()
{
    PopulateEnabledServicesLocked(mEnabledServices);
    UpdateServicesStateLocked(mInstalledServices, mEnabledServices);
}

/**
 * Unbinds all bound services.
 */
void CAccessibilityManagerService::UnbindAllServicesLocked()
{
    List<AutoPtr<CAccessibilityServiceConnection> > services = mServices;

    List<AutoPtr<CAccessibilityServiceConnection> >::Iterator it;
    for (it = services.Begin(); it != services.End(); it++) {
        AutoPtr<CAccessibilityServiceConnection> service = *it;
        service->Unbind();
        mComponentNameToServiceMap.Erase(service->mComponentName);
    }
    services.Clear();
}

/**
 * Populates a list with the {@link ComponentName}s of all enabled
 * {@link AccessibilityService}s.
 *
 * @param enabledServices The list.
 */
void CAccessibilityManagerService::PopulateEnabledServicesLocked(
    Set<AutoPtr<IComponentName> > enabledServices)
{
    enabledServices.Clear();

    AutoPtr<IContentResolver> contentResolver;
    mContext->GetContentResolver((IContentResolver**)&contentResolver);
    AutoPtr<ISettingsSecure> settingsSecure;
    CSettingsSecure::AcquireSingleton((ISettingsSecure**)&settingsSecure);
    String servicesValue;
    settingsSecure->GetString(contentResolver,
            String(SettingsSecure_ENABLED_ACCESSIBILITY_SERVICES), &servicesValue);

    if (!servicesValue.IsNull()) {
        TextUtils::SimpleStringSplitter* splitter = mStringColonSplitter;
        splitter->SetString(servicesValue);
        while (splitter->HasNext()) {
            String str;
            splitter->Next(&str);
            if (str.IsNull() || str.GetLength() <= 0) {
                continue;
            }

            AutoPtr<IComponentName> enabledService;
            AutoPtr<IComponentNameHelper> helper;
            CComponentNameHelper::AcquireSingleton((IComponentNameHelper**)&helper);
            helper->UnflattenFromString(str, (IComponentName**)&enabledService);
            if (enabledService != NULL) {
                enabledServices.Insert(enabledService);
            }
        }
    }
}

/**
 * Updates the state of each service by starting (or keeping running) enabled ones and
 * stopping the rest.
 *
 * @param installedServices All installed {@link AccessibilityService}s.
 * @param enabledServices The {@link ComponentName}s of the enabled services.
 */
void CAccessibilityManagerService::UpdateServicesStateLocked(
    /* [in] */ List<AutoPtr<IServiceInfo> > installedServices,
    /* [in] */ Set<AutoPtr<IComponentName> > enabledServices)
{
    HashMap<AutoPtr<IComponentName>, AutoPtr<CAccessibilityServiceConnection> >
            componentNameToServiceMap = mComponentNameToServiceMap;
    List<AutoPtr<CAccessibilityServiceConnection> > services = mServices;
    Boolean isEnabled = mIsEnabled;

    List<AutoPtr<IServiceInfo> >::Iterator it;
    for (it = installedServices.Begin(); it != installedServices.End(); it++) {
        AutoPtr<IServiceInfo> intalledService = *it;
        String capsuleName, name;
        intalledService->GetCapsuleName(&capsuleName);
        intalledService->GetName(&name);
        AutoPtr<IComponentName> componentName;
        CComponentName::New(capsuleName, name, (IComponentName**)&componentName);
        AutoPtr<CAccessibilityServiceConnection> service;
        HashMap<AutoPtr<IComponentName>, AutoPtr<CAccessibilityServiceConnection> >
                ::Iterator serviceIt = componentNameToServiceMap.Find(componentName);
        if (serviceIt != componentNameToServiceMap.End()) {
            service = serviceIt->mSecond;
        }

        if (isEnabled && enabledServices.Find(componentName) != enabledServices.End()) {
            if (service == NULL) {
                AutoPtr<CAccessibilityServiceConnection> newService;
                CAccessibilityServiceConnection::NewByFriend(this, componentName,
                        (CAccessibilityServiceConnection**)&newService);
                newService->Bind();
            }
        }
        else {
            if (service != NULL) {
                service->Unbind();
                componentNameToServiceMap.Erase(componentName);
                services.Remove(service);
            }
        }
    }
}

/**
 * Updates the state of {@link android.view.accessibility.AccessibilityManager} clients.
 */
void CAccessibilityManagerService::UpdateClientsLocked()
{
    List<AutoPtr<IAccessibilityManagerClient> >::Iterator it;
    for (it = mClients.Begin(); it != mClients.End(); it++) {
        //try {
        ECode ec = (*it)->SetEnabled(mIsEnabled);
        if (FAILED(ec)) {
            it = mClients.Erase(it);
            it++;
        }
        // } catch (RemoteException re) {
        //     mClients.remove(i);
        //     count--;
        //     i--;
        // }
    }
}

/**
 * Handler for delayed event dispatch.
 */
void CAccessibilityManagerService::HandleDelayedEventDispatch(
    /* [in] */ CAccessibilityServiceConnection* service,
    /* [in] */ Int32 eventType)
{
    Mutex::Autolock lock(mLock);
    //the service maybe deleted in the call of NotifyEventListenerLocked. so use autoptr
    AutoPtr<CAccessibilityServiceConnection> s = service;
    NotifyEventListenerLocked(s, eventType);
    AutoPtr<IAccessibilityEvent> oldEvent;
    HashMap<Int32, AutoPtr<IAccessibilityEvent> >::Iterator it
            = service->mPendingEvents.Find(eventType);
    if (it != service->mPendingEvents.End()) {
        oldEvent = it->mSecond;
        service->mPendingEvents.Erase(eventType);
    }
    TryRecycleLocked(oldEvent);
}

void CAccessibilityManagerService::HandleDoSetServiceInfo(
    /* [in] */ IAccessibilityServiceInfo* info,
    /* [in] */ CAccessibilityServiceConnection* service)
{
    Mutex::Autolock lock(mLock);
    info->GetEventTypes(&(service->mEventTypes));
    info->GetFeedbackType(&(service->mFeedbackType));
    ArrayOf<String>* names;
    info->GetPackageNames(&names);
    if (names != NULL) {
        for (Int32 i = 0; i < names->GetLength(); ++i) {
            service->mCapsuleNames.Insert((*names)[i]);
        }
    }
    info->GetNotificationTimeout(&(service->mNotificationTimeout));
    Int32 flags;
    info->GetFlags(&flags);
    service->mIsDefault = (flags & AccessibilityServiceInfo_DEFAULT) != 0;
}
