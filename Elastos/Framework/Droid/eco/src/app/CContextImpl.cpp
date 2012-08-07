
#include "app/CContextImpl.h"
#include "app/ActivityManagerNative.h"
#include "content/CIntent.h"
#include "impl/CPolicyManager.h"
#include "view/CWindowManagerImpl.h"
#include <unistd.h>
#include <assert.h>
#include <Slogger.h>
#include <stdio.h>

using namespace Elastos::Utility::Logging;

const char* CContextImpl::TAG  = "CContextImpl";

ICONTEXT_METHODS_IMPL(ReceiverRestrictedContext, ContextWrapper, ContextWrapper);

ReceiverRestrictedContext::ReceiverRestrictedContext(
    /* [in] */ IContext* base)
    : ContextWrapper(base)
{}

IInterface* ReceiverRestrictedContext::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IContext) {
        return (IContext*)this;
    }
    else if (riid == EIID_IContextWrapper) {
        return (IContextWrapper*)this;
    }

    return NULL;
}

UInt32 ReceiverRestrictedContext::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReceiverRestrictedContext::Release()
{
    return ElRefBase::Release();
}

ECode ReceiverRestrictedContext::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(IContextWrapper*)this) {
        *pIID = EIID_IContextWrapper;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode ReceiverRestrictedContext::GetBaseContext(
    /* [out] */ IContext** context)
{
    return ContextWrapper::GetBaseContext(context);
}

CContextImpl::CContextImpl() :
    mCapsuleInfo(NULL),
    mThemeResource(0)
{}

CContextImpl::~CContextImpl()
{}

PInterface CContextImpl::Probe(
    /* [in] */ REIID riid)
{
    return _CContextImpl::Probe(riid);
}

ECode CContextImpl::GetAssets(
    /* [out] */ IAssetManager** assetManager)
{
    return mResources->GetAssets(assetManager);
}

ECode CContextImpl::GetResources(
    /* [out] */ IResources** resources)
{
    if (resources == NULL) return E_INVALID_ARGUMENT;

    *resources = mResources.Get();
    if (*resources != NULL) (*resources)->AddRef();
    return NOERROR;
}

ECode CContextImpl::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    if (resolver == NULL) return E_INVALID_ARGUMENT;

    *resolver = (IContentResolver*)
            (CApplicationContentResolver*)mContentResolver;
    (*resolver)->AddRef();

    return NOERROR;
}

ECode CContextImpl::GetText(
    /* [in] */ Int32 resId,
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    return Context::GetText(resId, text);
}

ECode CContextImpl::SetTheme(
    /* [in] */ Int32 resid)
{
    mThemeResource = resid;
    return NOERROR;
}

ECode CContextImpl::GetTheme(
    /* [out] */ ITheme** theme)
{
    VALIDATE_NOT_NULL(theme);

    if (mTheme == NULL) {
        if (mThemeResource == 0) {
            //todo:
            mThemeResource = 0x01030005/*com.android.internal.R.style.Theme*/;
        }
        ASSERT_SUCCEEDED(mResources->NewTheme((ITheme**)&mTheme));
        mTheme->ApplyStyle(mThemeResource, TRUE);
    }
    *theme = mTheme.Get();
    (*theme)->AddRef();
    return NOERROR;
}

ECode CContextImpl::ObtainStyledAttributes(
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributes(attrs, styles);
}

ECode CContextImpl::ObtainStyledAttributesEx(
    /* [in] */ Int32 resid,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributesEx(resid, attrs, styles);
}

ECode CContextImpl::ObtainStyledAttributesEx2(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributesEx2(set, attrs, styles);
}

ECode CContextImpl::ObtainStyledAttributesEx3(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributesEx3(set, attrs, defStyleAttr, defStyleRes, styles);
}

ECode CContextImpl::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    VALIDATE_NOT_NULL(loader);

    if (mCapsuleInfo != NULL) {
        return mCapsuleInfo->GetClassLoader(loader);
    }
    else {
//        return ClassLoader::GetSystemClassLoader(loader);
        assert(0);
        return E_NOT_IMPLEMENTED;
    }
}

ECode CContextImpl::GetCapsuleName(
    /* [out] */ String* capsuleName)
{
    if (mCapsuleInfo != NULL) {
        return mCapsuleInfo->GetCapsuleName(capsuleName);
    }
    return E_DOES_NOT_EXIST;

//    throw new RuntimeException("Not supported in system context");
}

ECode CContextImpl::StartActivity(
    /* [in] */ IIntent *intent)
{
    Int32 flags;
    intent->GetFlags(&flags);
    if ((flags & Intent_FLAG_ACTIVITY_NEW_TASK) == 0) {
//            throw new AndroidRuntimeException(
//                  "Calling startActivity() from outside of an Activity "
//                  + " context requires the FLAG_ACTIVITY_NEW_TASK flag."
//                  + " Is this really what you want?");
    }
    AutoPtr<IActivityResult> result;
    mApartment->GetInstrumentation()->ExecStartActivity(
            GetOuterContext(), (IApplicationApartment*)(CApplicationApartment*)mApartment,
            NULL, NULL, intent, -1, (IActivityResult**)&result);
    return NOERROR;
}

ECode CContextImpl::SendBroadcast(
    /* [in] */ IIntent *pIntent)
{
//    String resolvedType = intent.resolveTypeIfNeeded(getContentResolver());
//    try {
    AutoPtr<IActivityManager> mgr;
    ECode ec = ActivityManagerNative::GetDefault(
            (IActivityManager**)&mgr);
    if (FAILED(ec)) return ec;
    Int32 result;
    return mgr->BroadcastIntent(
            (IApplicationApartment*)mApartment, pIntent, String(NULL) /*resolvedType*/, NULL,
            Activity_RESULT_OK, String(NULL), NULL, String(NULL), FALSE, FALSE, getpid(), getuid(), &result);
//    } catch (RemoteException e) {
//    }
}

ECode CContextImpl::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    if (name == NULL) return E_INVALID_ARGUMENT;

    AutoPtr<IActivityManager> activityManager;
    ActivityManagerNative::GetDefault((IActivityManager**)&activityManager);
    AutoPtr<IContentResolver> resolver;
    GetContentResolver((IContentResolver**)&resolver);
    String type;
    service->ResolveTypeIfNeeded(resolver, &type);
    ECode ec = activityManager->StartService(
            (IApplicationApartment*)(CApplicationApartment*)mApartment,
            service, type, name);
    if (FAILED(ec)) {
        *name = NULL;
        return ec;
    }
    if (*name != NULL) {
        String capsuleName;
        (*name)->GetCapsuleName(&capsuleName);
        if (!capsuleName.Compare("!")) {
//            throw new SecurityException(
//                    "Not allowed to start service " + service
//                    + " without permission " + cn.getClassName());
            return E_SECURITY_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CContextImpl::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    if (succeeded == NULL) return E_INVALID_ARGUMENT;

    AutoPtr<IActivityManager> activityManager;
    ActivityManagerNative::GetDefault((IActivityManager**)&activityManager);
    AutoPtr<IContentResolver> resolver;
    GetContentResolver((IContentResolver**)&resolver);
    String type;
    service->ResolveTypeIfNeeded(resolver, &type);
    Int32 res;
    ECode ec = activityManager->StopService(
            (IApplicationApartment*)(CApplicationApartment*)mApartment,
            service, type, &res);
    if (FAILED(ec)) {
        *succeeded = FALSE;
        return NOERROR;
    }

    if ((res < 0)) {
//        throw new SecurityException(
//                    "Not allowed to stop service " + service);
        return E_SECURITY_EXCEPTION;
    }
    else {
        *succeeded = res != 0;
        return NOERROR;
    }
}

ECode CContextImpl::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    if (succeeded == NULL) return E_INVALID_ARGUMENT;

    AutoPtr<IServiceConnectionInner> sd;
    if (mCapsuleInfo != NULL) {
        sd = mCapsuleInfo->GetServiceDispatcher(conn, GetOuterContext(),
                (IApplicationApartment*)(CApplicationApartment*)mApartment,
                flags);
    } else {
//        throw new RuntimeException("Not supported in system context");
        return E_RUNTIME_EXCEPTION;
    }
    AutoPtr<IActivityManager> activityManager;
    ActivityManagerNative::GetDefault((IActivityManager**)&activityManager);
    AutoPtr<IContentResolver> resolver;
    GetContentResolver((IContentResolver**)&resolver);
    String type;
    service->ResolveTypeIfNeeded(resolver, &type);
    Int32 res;
    ECode ec = activityManager->BindService(
        (IApplicationApartment*)(CApplicationApartment*)mApartment,
        GetActivityToken(), service, type, sd, flags, getpid(), getuid(), &res);
    if (FAILED(ec)) {
        *succeeded = FALSE;
        return NOERROR;
    }

    if (res < 0) {
//        throw new SecurityException(
//                "Not allowed to bind to service " + service);
        return E_SECURITY_EXCEPTION;
    }
    else {
        *succeeded = res != 0;
        return NOERROR;
    }
}

ECode CContextImpl::UnbindService(
    /* [in] */ IServiceConnection* conn)
{
    if (mCapsuleInfo != NULL) {
        IServiceConnectionInner* sd = mCapsuleInfo->ForgetServiceDispatcher(
                GetOuterContext(), conn);
//        try {
        AutoPtr<IActivityManager> activityManager;
        ActivityManagerNative::GetDefault((IActivityManager**)&activityManager);
        Boolean succeeded;
        activityManager->UnbindService(sd, &succeeded);
//        } catch (RemoteException e) {
//        }
        return NOERROR;
    } else {
//        throw new RuntimeException("Not supported in system context");
        return E_RUNTIME_EXCEPTION;
    }
}

ECode CContextImpl::GetSystemService(
    /* [in] */ const String& name,
    /* [out] */ IInterface** object)
{
    ECode ec = NOERROR;
    if (!String(Context_WINDOW_SERVICE).Compare(name)) {
        AutoPtr<IWindowManager> wm;
        CWindowManagerImpl::AcquireSingleton((IWindowManager**)&wm);
        *object = (IInterface*)wm.Get();
        (*object)->AddRef();
        return NOERROR;
    } else if (!String(Context_LAYOUT_INFLATER_SERVICE).Compare(name)) {
        Mutex::Autolock lock(mSync);

        if (mLayoutInflater != NULL) {
            *object = (IInterface*)mLayoutInflater.Get();
            (*object)->AddRef();
            return NOERROR;
        }

        AutoPtr<IPolicyManager> pm;
        ec = CPolicyManager::AcquireSingleton((IPolicyManager**)&pm);
        if (FAILED(ec)) {
            return ec;
        }

        ec = pm->MakeNewLayoutInflater(GetOuterContext(), (ILayoutInflater**)&mLayoutInflater);
        if (FAILED(ec)) {
            return ec;
        }

        *object = (IInterface*)mLayoutInflater.Get();
        (*object)->AddRef();
        return NOERROR;
    } else if (!String(Context_ACTIVITY_SERVICE).Compare(name)) {
//        return getActivityManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_INPUT_METHOD_SERVICE).Compare(name)) {
//        return InputMethodManager.getInstance(this);
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_ALARM_SERVICE).Compare(name)) {
//        return getAlarmManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_ACCOUNT_SERVICE).Compare(name)) {
//        return getAccountManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_POWER_SERVICE).Compare(name)) {
//        return getPowerManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_CONNECTIVITY_SERVICE).Compare(name)) {
//        return getConnectivityManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_THROTTLE_SERVICE).Compare(name)) {
//        return getThrottleManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_WIFI_SERVICE).Compare(name)) {
//        return getWifiManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_NOTIFICATION_SERVICE).Compare(name)) {
//        return getNotificationManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_KEYGUARD_SERVICE).Compare(name)) {
//        return new KeyguardManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_ACCESSIBILITY_SERVICE).Compare(name)) {
//        return AccessibilityManager.getInstance(this);
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_LOCATION_SERVICE).Compare(name)) {
//        return getLocationManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_SEARCH_SERVICE).Compare(name)) {
//        return getSearchManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_SENSOR_SERVICE).Compare(name)) {
//        return getSensorManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_STORAGE_SERVICE).Compare(name)) {
//        return getStorageManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_USB_SERVICE).Compare(name)) {
//        return getUsbManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_VIBRATOR_SERVICE).Compare(name)) {
//        return getVibrator();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_STATUS_BAR_SERVICE).Compare(name)) {
//        synchronized (mSync) {
//            if (mStatusBarManager == null) {
//                mStatusBarManager = new StatusBarManager(getOuterContext());
//            }
//            return mStatusBarManager;
//        }
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_AUDIO_SERVICE).Compare(name)) {
//        return getAudioManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_TELEPHONY_SERVICE).Compare(name)) {
//        return getTelephonyManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_CLIPBOARD_SERVICE).Compare(name)) {
//        return getClipboardManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_WALLPAPER_SERVICE).Compare(name)) {
//        return getWallpaperManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_DROPBOX_SERVICE).Compare(name)) {
//        return getDropBoxManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_DEVICE_POLICY_SERVICE).Compare(name)) {
//        return getDevicePolicyManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_UI_MODE_SERVICE).Compare(name)) {
//        return getUiModeManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_DOWNLOAD_SERVICE).Compare(name)) {
//        return getDownloadManager();
        return E_NOT_IMPLEMENTED;
    } else if (!String(Context_NFC_SERVICE).Compare(name)) {
//        return getNfcManager();
        return E_NOT_IMPLEMENTED;
    }

    return E_DOES_NOT_EXIST;
}

ECode CContextImpl::Init(
    /* [in] */ LoadedCap* capsuleInfo,
    /* [in] */ IBinder* activityToken,
    /* [in] */ CApplicationApartment* apartment)
{
    return Init(capsuleInfo, activityToken, apartment, NULL);
}

ECode CContextImpl::Init(
    /* [in] */ LoadedCap* capsuleInfo,
    /* [in] */ IBinder* activityToken,
    /* [in] */ CApplicationApartment* apartment,
    /* [in] */ IResources* container)
{
    assert(capsuleInfo);

    mCapsuleInfo = capsuleInfo;
    AutoPtr<CResources> res;
    ECode ec = mCapsuleInfo->GetResources(apartment, (CResources**)&res);
    if (FAILED(ec)) return ec;
    mResources = res.Get();

//    if (mResources != null && container != null
//            && container.getCompatibilityInfo().applicationScale !=
//                    mResources.getCompatibilityInfo().applicationScale) {
//        if (DEBUG) {
//            Log.d(TAG, "loaded context has different scaling. Using container's" +
//                    " compatiblity info:" + container.getDisplayMetrics());
//        }
//        mResources = mainThread.getTopLevelResources(
//                mPackageInfo.getResDir(), container.getCompatibilityInfo().copy());
//    }
    mApartment = apartment;
    CApplicationContentResolver::NewByFriend(this, apartment,
            (CApplicationContentResolver**)&mContentResolver);
    SetActivityToken(activityToken);
    return NOERROR;
}

ECode CContextImpl::CreateCapsuleContext(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** context)
{
//    if (packageName.equals("system") || packageName.equals("android")) {
//        return new ContextImpl(mMainThread.getSystemContext());
//    }

    LoadedCap* pi = mApartment->GetCapsuleInfo(capsuleName, flags);
    if (pi != NULL) {
        AutoPtr<CContextImpl> c;
        CContextImpl::NewByFriend((CContextImpl**)&c);
        //c.mRestricted = (flags & CONTEXT_RESTRICTED) == CONTEXT_RESTRICTED;
        c->Init(pi, NULL, mApartment, mResources);
        if (c->mResources != NULL) {
            *context = (IContext*)(CContextImpl*)c;
            if (*context) (*context)->AddRef();
            return NOERROR;
        }
    }

//    // Should be a better exception.
//        throw new PackageManager.NameNotFoundException(
//            "Application package " + packageName + " not found");

    *context = NULL;
    return E_DOES_NOT_EXIST;
}

ECode CContextImpl::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::EnforceCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::CheckCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* perm)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::GrantUriPermission(
    /* [in] */ const String& toCapsule,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return E_NOT_IMPLEMENTED;
}

void CContextImpl::ScheduleFinalCleanup(
    /* [in] */ const String& who,
    /* [in] */ const String& what)
{
     mApartment->ScheduleContextCleanup(this, who, what);
}

ECode CContextImpl::PerformFinalCleanup(
    /* [in] */ const String& who,
    /* [in] */ const String& what)
{
    //Slog.i(TAG, "Cleanup up context: " + this);
    ECode ec = mCapsuleInfo->RemoveContextRegistrations(GetOuterContext(), who, what);
    return ec;
}

IContext* CContextImpl::GetReceiverRestrictedContext()
{
    if (mReceiverRestrictedContext != NULL) {
        return (IContext*)mReceiverRestrictedContext;
    }
    return mReceiverRestrictedContext = new ReceiverRestrictedContext(GetOuterContext());
}

void CContextImpl::SetActivityToken(
    /* [in] */ IBinder* token)
{
    mActivityToken = token;
}

void CContextImpl::SetOuterContext(
    /* [in] */ IContext* context)
{
    mOuterContext = context;
}

IContext* CContextImpl::GetOuterContext()
{
    return (IContext*)mOuterContext;
}

IBinder* CContextImpl::GetActivityToken()
{
    return mActivityToken;
}
