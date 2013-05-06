
#include "server/CAccessibilityServiceConnection.h"

CAccessibilityServiceConnection::CAccessibilityServiceConnection()
    : mId(0)
    , mEventTypes(0)
    , mFeedbackType(0)
    , mIsDefault(FALSE)
    , mNotificationTimeout(0)
    , mIsActive(FALSE)
{}

ECode CAccessibilityServiceConnection::constructor(
    /* [in] */ IAccessibilityManager* manager,
    /* [in] */ IComponentName* componentName)
{
    mHost = (CAccessibilityManagerService*)manager;//->Probe(EIID_CAccessibilityManagerService);
    mId = CAccessibilityManagerService::sIdCounter++;
    mComponentName = componentName;

    ASSERT_SUCCEEDED(CIntent::New((IIntent**)&mIntent));
    mIntent->SetComponent(mComponentName);
    mIntent->PutInt32Extra(String(Intent_EXTRA_CLIENT_LABEL), 0x0104036f
            /*com.android.internal.R.string.accessibility_binding_label*/);
    AutoPtr<IPendingIntentHelper> helper;
    ASSERT_SUCCEEDED(CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&helper));
    AutoPtr<IIntent> newIntent;
    ASSERT_SUCCEEDED(CIntent::New((IIntent**)&newIntent));
    AutoPtr<IPendingIntent> a;
    helper->GetActivity(mHost->mContext, 0, newIntent, 0, (IPendingIntent**)&a);
    //mIntent->PutExtra(String(Intent_EXTRA_CLIENT_INTENT), a);

    return NOERROR;
}

/**
 * Binds to the accessibility service.
 */
void CAccessibilityServiceConnection::Bind()
{
    if (mService == NULL) {
        AutoPtr<IServiceConnection> con = (IServiceConnection*)this->Probe(EIID_IServiceConnection);
        Boolean result;
        mHost->mContext->BindService(mIntent, con, Context_BIND_AUTO_CREATE, &result);
    }
}

/**
 * Unbinds form the accessibility service and removes it from the data
 * structures for service management.
 */
void CAccessibilityServiceConnection::Unbind()
{
    if (mService != NULL) {
        AutoPtr<IServiceConnection> con = (IServiceConnection*)this->Probe(EIID_IServiceConnection);
        mHost->mContext->UnbindService(con);
    }
}

/**
 * Returns if the service is configured i.e. at least event types of interest
 * and feedback type must be set.
 *
 * @return True if the service is configured, false otherwise.
 */
Boolean CAccessibilityServiceConnection::IsConfigured()
{
    return (mEventTypes != 0 && mFeedbackType != 0);
}

ECode CAccessibilityServiceConnection::SetServiceInfo(
    /* [in] */ IAccessibilityServiceInfo* info)
{
    void (STDCALL CAccessibilityManagerService::*pHandlerFunc)
            (IAccessibilityServiceInfo*, CAccessibilityServiceConnection*);
    pHandlerFunc = &CAccessibilityManagerService::HandleDoSetServiceInfo;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(info);
    params->WriteInt32((Int32)this);

    mHost->mHandler->PostCppCallback(
        (Handle32)mHost.Get(), *(Handle32*)&pHandlerFunc, params, 0);
    return NOERROR;
}

ECode CAccessibilityServiceConnection::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    mService = service;
    mServiceInterface = IEventListener::Probe(service);
    assert(mServiceInterface != NULL);

    //try {
    mServiceInterface->SetConnection(
            (IAccessibilityServiceConnection*)this->Probe(EIID_IAccessibilityServiceConnection));
    Mutex::Autolock lock(mHost->mLock);
    List<AutoPtr<CAccessibilityServiceConnection> >::Iterator it;
    for (it = mHost->mServices.Begin(); it != mHost->mServices.End(); ++it) {
        AutoPtr<CAccessibilityServiceConnection> con = *it;
        if (con.Get() == this) {
            return NOERROR;
        }
    }

    //if (!mHost->mServices.contains(this)) {
    mHost->mServices.PushBack(this);
    mHost->mComponentNameToServiceMap[name] = this;
    //}
    // } catch (RemoteException re) {
    //     Slog.w(LOG_TAG, "Error while setting Controller for service: " + service, re);
    // }

    return NOERROR;
}

ECode CAccessibilityServiceConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    Mutex::Autolock lock(mHost->mLock);
    AutoPtr<CAccessibilityServiceConnection> service;
    HashMap<AutoPtr<IComponentName>, AutoPtr<CAccessibilityServiceConnection> >::Iterator
            it = mHost->mComponentNameToServiceMap.Find(name);
    if (it != mHost->mComponentNameToServiceMap.End()) {
        service = it->mSecond;
        mHost->mComponentNameToServiceMap.Erase(name);
    }

    if (service) {
        mHost->mServices.Remove(service);
    }

    return NOERROR;
}
