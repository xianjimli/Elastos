
#ifdef _FRAMEWORK_CORE
#include "app/Service.h"
#include "content/CComponentName.h"
#else
#include "Service.h"
#endif


Service::Service()
    : mStartCompatibility(FALSE)
{
}

Service::~Service()
{
}

ECode Service::GetApplication(
    /* [out] */ IApplication** application)
{
    assert(application != NULL);
    *application = mApplication;
    if (*application != NULL) {
        (*application)->AddRef();
    }

    return NOERROR;
}

ECode Service::Initialize()
{
    return NOERROR;
}

PInterface Service::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IService*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    if (riid == EIID_IService) {
        return (IService*)this;
    }
    else if (riid == EIID_IContextWrapper) {
       return (IContextWrapper*)this;
    }
    else if (riid == EIID_IContext) {
       return (IContext*)this;
    }
    else if (riid == EIID_IComponentCallbacks) {
       return (IComponentCallbacks*)this;
    }

    return NULL;
}

UInt32 Service::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Service::Release()
{
    return ElRefBase::Release();
}

ECode Service::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IService *)this) {
        *pIID = EIID_IService;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode Service::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode Service::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode Service::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode Service::Attach(
    /* [in] */ IContext* ctx,
    /* [in] */ IApplicationApartment* apartment,
    /* [in] */ const String& className,
    /* [in] */ IBinder* token,
    /* [in] */ IApplication* application,
    /* [in] */ IActivityManager* activityManager)
{
    mBase = ctx;
    mApartment = apartment;           // NOTE:  unused - remove?
    mClassName = className;
    mToken = token;
    mApplication = application;
    mActivityManager = activityManager;
    return NOERROR;
}

ECode Service::Create()
{
    return OnCreate();
}

ECode Service::Start(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 startId)
{
    return OnStart(intent, startId);
}

ECode Service::StartCommand(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 startId,
    /* [out] */ Int32* result)
{
    return OnStartCommand(intent, flags, startId, result);
}

ECode Service::Destroy()
{
    return OnDestroy();
}

ECode Service::Bind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    return OnBind(intent, binder);
}

ECode Service::Unbind(
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* succeeded)
{
    return OnUnbind(intent, succeeded);
}

ECode Service::Rebind(
    /* [in] */ IIntent* intent)
{
    return OnRebind(intent);
}

ECode Service::OnCreate()
{
    return NOERROR;
}

ECode Service::OnStart(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 startId)
{
    return NOERROR;
}

ECode Service::OnStartCommand(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 startId,
    /* [out] */ Int32* result)
{
    if (result == NULL) return E_INVALID_ARGUMENT;

    OnStart(intent, startId);
    *result = mStartCompatibility ? Service_START_STICKY_COMPATIBILITY : Service_START_STICKY;
    return NOERROR;
}

ECode Service::OnDestroy()
{
    return NOERROR;
}

ECode Service::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    return NOERROR;
}

ECode Service::OnUnbind(
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* succeeded)
{
    return NOERROR;
}

ECode Service::OnRebind(
    /* [in] */ IIntent* intent)
{
    return NOERROR;
}

ECode Service::StartActivity(
    /* [in] */ IIntent* intent)
{
    return mBase->StartActivity(intent);
}

ECode Service::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags)
{
    return mBase->StartIntentSender(intent, fillInIntent, flagsMask,
        flagsValues, extraFlags);
}

ECode Service::SendBroadcast(
    /* [in] */ IIntent* intent)
{
    return mBase->SendBroadcast(intent);
}

ECode Service::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    return mBase->StartService(service, name);
}

ECode Service::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    return mBase->StopService(service, succeeded);
}

ECode Service::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    return mBase->BindService(service, conn, flags, succeeded);
}

ECode Service::UnbindService(
    /* [in] */ IServiceConnection* conn)
{
    return mBase->UnbindService(conn);
}

ECode Service::GetSystemService(
    /* [in] */ CString name,
    /* [out] */ IInterface** object)
{
    if (!String(Context_LAYOUT_INFLATER_SERVICE).Compare(name)) {
        if (mInflater == NULL) {
            AutoPtr<ILayoutInflater> inflater;
            mBase->GetSystemService(
                String(Context_LAYOUT_INFLATER_SERVICE), (IInterface**)&inflater);

            if (inflater == NULL) {
                return E_INVALID_ARGUMENT;
            }

            inflater->CloneInContext(
                (IContext*)this->Probe(EIID_IContext),
                (ILayoutInflater**)&mInflater);
        }
        *object = mInflater;
        if (*object) {
            (*object)->AddRef();
        }

        return NOERROR;
    }

    return mBase->GetSystemService(name, object);
}

ECode Service::CreateCapsuleContext(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    return mBase->CreateCapsuleContext(capsuleName, flags, ctx);
}

ECode Service::CheckPermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [out] */ Int32 * result)
{
    return mBase->CheckPermission(permission, pid, uid, result);
}

ECode Service::CheckUriPermission(
    /* [in] */ IUri * uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32 * result)
{
    return mBase->CheckUriPermission(uri, readPermission, writePermission,
                    pid, uid, modeFlags, result);
}

ECode Service::CheckUriPermissionEx(
    /* [in] */ IUri * uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32 * result)
{
    return mBase->CheckUriPermissionEx(uri, pid, uid, modeFlags, result);
}

ECode Service::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* value)
{
    return mBase->CheckCallingPermission(permission, value);
}

ECode Service::EnforceCallingOrSelfPermission(
    /* [in] */ CString permission,
    /* [in] */ CString message)
{
    return mBase->EnforceCallingOrSelfPermission(permission, message);
}

ECode Service::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return mBase->RevokeUriPermission(uri, modeFlags);
}

ECode Service::CheckCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* perm)
{
    return mBase->CheckCallingOrSelfPermission(permission, perm);
}

ECode Service::GrantUriPermission(
    /* [in] */ const String& toCapsule,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return mBase->GrantUriPermission(toCapsule, uri, modeFlags);
}

ECode Service::GetAssets(
    /* [out] */ IAssetManager** assetManager)
{
    return mBase->GetAssets(assetManager);
}

ECode Service::GetResources(
    /* [out] */ IResources** resources)
{
    return mBase->GetResources(resources);
}

ECode Service::GetCapsuleManager(
    /* [out] */ ILocalCapsuleManager** capsuleManager)
{
    return mBase->GetCapsuleManager(capsuleManager);
}

ECode Service::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    return mBase->GetContentResolver(resolver);
}

ECode Service::GetApplicationContext(
    /* [out] */ IContext** ctx)
{
    return mBase->GetApplicationContext(ctx);
}

ECode Service::GetText(
    /* [in] */ Int32 resId,
    /* [out] */ ICharSequence** text)
{
    return mBase->GetText(resId, text);
}

ECode Service::GetString(
    /* [in] */ Int32 resId,
    /* [out] */ String* str)
{
    if (str == NULL) return E_ILLEGAL_ARGUMENT_EXCEPTION;

    AutoPtr<IResources> resources;
    GetResources((IResources**)&resources);
    return resources->GetString(resId, str);
}

ECode Service::SetTheme(
    /* [in] */ Int32 resid)
{
    mThemeResource = resid;
    return InitializeTheme();
}

ECode Service::GetTheme(
    /* [out] */ ITheme** theme)
{
    if (mTheme == NULL) {
        if (mThemeResource == 0) {
            mThemeResource = 0x01030005/*com.android.internal.R.style.Theme*/;
        }
        InitializeTheme();
    }

    *theme = mTheme;
    mTheme->AddRef();

    return NOERROR;
}

ECode Service::ObtainStyledAttributes(
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributes(attrs, styles);
}

ECode Service::ObtainStyledAttributesEx(
    /* [in] */ Int32 resid,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributesEx(resid, attrs, styles);
}

ECode Service::ObtainStyledAttributesEx2(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributesEx2(set, attrs, styles);
}

ECode Service::ObtainStyledAttributesEx3(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributesEx3(set, attrs,
            defStyleAttr, defStyleRes, styles);
}

ECode Service::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    return mBase->GetClassLoader(loader);
}

ECode Service::GetCapsuleName(
    /* [out] */ String* capsuleName)
{
    return mBase->GetCapsuleName(capsuleName);
}

ECode Service::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    return mBase->GetApplicationInfo(info);
}

ECode Service::GetCapsuleResourcePath(
    /* [out] */ String* path)
{
    return mBase->GetCapsuleResourcePath(path);
}

ECode Service::GetFilesDir(
    /* [out] */ IFile** filesDir)
{
    return mBase->GetFilesDir(filesDir);
}

ECode Service::GetExternalFilesDir(
    /* [in] */ const String& type,
    /* [out] */ IFile** filesDir)
{
    return mBase->GetExternalFilesDir(type, filesDir);
}

ECode Service::GetCacheDir(
    /* [out] */ IFile** cacheDir)
{
    return mBase->GetCacheDir(cacheDir);
}

ECode Service::GetDir(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFile** dir)
{
    return mBase->GetDir(name, mode, dir);
}

/**
 * @return the base context as set by the constructor or setBaseContext
 */
ECode Service::GetBaseContext(
    /* [out] */ IContext** ctx)
{
    if (ctx == NULL) return E_INVALID_ARGUMENT;

    *ctx = mBase;
    if (*ctx) (*ctx)->AddRef();
    return NOERROR;
}

ECode Service::GetClassName(
    /* [out] */ String* className)
{
    if (className == NULL) return E_INVALID_ARGUMENT;

    *className = mClassName;
    return NOERROR;
}

ECode Service::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    return NOERROR;
}

ECode Service::OnLowMemory()
{
    return NOERROR;
}

ECode Service::AttachBaseContext(
    /* [in] */ IContext* newBase)
{
    if (mBase != NULL) {
        //throw new IllegalStateException("Base context already set");
        return E_INVALID_ARGUMENT;
    }
    mBase = newBase;

    return NOERROR;
}

ECode Service::OnApplyThemeResource(
    /* [in] */ ITheme* theme,
    /* [in] */ Int32 resid,
    /* [in] */ Boolean first)
{
    return theme->ApplyStyle(resid, TRUE);
}

ECode Service::InitializeTheme()
{
    Boolean first = mTheme == NULL;
    if (first) {
        AutoPtr<IResources> resources;
        GetResources((IResources**)&resources);
        resources->NewTheme((ITheme**)&mTheme);
        AutoPtr<ITheme> theme;
        mBase->GetTheme((ITheme**)&theme);
        if (theme != NULL) {
            mTheme->SetTo(theme);
        }
    }

    return OnApplyThemeResource(mTheme, mThemeResource, first);
}

/**
 * Stop the service, if it was previously started.  This is the same as
 * calling {@link android.content.Context#stopService} for this particular service.
 *
 * @see #stopSelfResult(int)
 */
ECode Service::StopSelf()
{
    return StopSelfEx(-1);
}

/**
 * Old version of {@link #stopSelfResult} that doesn't return a result.
 *
 * @see #stopSelfResult
 */
ECode Service::StopSelfEx(
    /* [in] */ Int32 startId)
{
    if (mActivityManager == NULL) {
        return NOERROR;
    }

    Boolean res;
    String capsuleName;
    GetCapsuleName(&capsuleName);
    AutoPtr<IComponentName> name;
    CComponentName::New(capsuleName, mClassName, (IComponentName**)&name);

    return mActivityManager->StopServiceToken(
                name, mToken, startId, &res);
}

/**
 * Stop the service if the most recent time it was started was
 * <var>startId</var>.  This is the same as calling {@link
 * android.content.Context#stopService} for this particular service but allows you to
 * safely avoid stopping if there is a start request from a client that you
 * haven't yet seen in {@link #onStart}.
 *
 * <p><em>Be careful about ordering of your calls to this function.</em>.
 * If you call this function with the most-recently received ID before
 * you have called it for previously received IDs, the service will be
 * immediately stopped anyway.  If you may end up processing IDs out
 * of order (such as by dispatching them on separate threads), then you
 * are responsible for stopping them in the same order you received them.</p>
 *
 * @param startId The most recent start identifier received in {@link
 *                #onStart}.
 * @return Returns true if the startId matches the last start request
 * and the service will be stopped, else false.
 *
 * @see #stopSelf()
 */
ECode Service::StopSelfResult(
    /* [in] */ Int32 startId,
    /* [out] */ Boolean* res)
{
    *res = FALSE;
    if (mActivityManager == NULL) {
        return NOERROR;
    }

    String capsuleName;
    GetCapsuleName(&capsuleName);
    AutoPtr<IComponentName> name;
    CComponentName::New(capsuleName, mClassName, (IComponentName**)&name);

    return mActivityManager->StopServiceToken(
        name, mToken, startId, res);
}

/**
 * @deprecated This is a now a no-op, use
 * {@link #startForeground(int, Notification)} instead.  This method
 * has been turned into a no-op rather than simply being deprecated
 * because analysis of numerous poorly behaving devices has shown that
 * increasingly often the trouble is being caused in part by applications
 * that are abusing it.  Thus, given a choice between introducing
 * problems in existing applications using this API (by allowing them to
 * be killed when they would like to avoid it), vs allowing the performance
 * of the entire system to be decreased, this method was deemed less
 * important.
 */
//@Deprecated
ECode Service::SetForeground(
    /* [in] */ Boolean isForeground)
{
    //Log.w(TAG, "setForeground: ignoring old API call on " + getClass().getName());
    return NOERROR;
}

/**
 * Make this service run in the foreground, supplying the ongoing
 * notification to be shown to the user while in this state.
 * By default services are background, meaning that if the system needs to
 * kill them to reclaim more memory (such as to display a large page in a
 * web browser), they can be killed without too much harm.  You can set this
 * flag if killing your service would be disruptive to the user, such as
 * if your service is performing background music playback, so the user
 * would notice if their music stopped playing.
 *
 * <p>If you need your application to run on platform versions prior to API
 * level 5, you can use the following model to call the the older {@link #setForeground}
 * or this modern method as appropriate:
 *
 * {@sample development/samples/ApiDemos/src/com/example/android/apis/app/ForegroundService.java
 *   foreground_compatibility}
 *
 * @param id The identifier for this notification as per
 * {@link NotificationManager#notify(int, Notification)
 * NotificationManager.notify(int, Notification)}.
 * @param notification The Notification to be displayed.
 *
 * @see #stopForeground(boolean)
 */
ECode Service::StartForeground(
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification)
{
    String capsuleName;
    GetCapsuleName(&capsuleName);
    AutoPtr<IComponentName> name;
    CComponentName::New(capsuleName, mClassName, (IComponentName**)&name);

    return mActivityManager->SetServiceForeground(
        name, mToken, id, notification, TRUE);
}

/**
 * Remove this service from foreground state, allowing it to be killed if
 * more memory is needed.
 * @param removeNotification If true, the notification previously provided
 * to {@link #startForeground} will be removed.  Otherwise it will remain
 * until a later call removes it (or the service is destroyed).
 * @see #startForeground(int, Notification)
 */
ECode Service::StopForeground(
    /* [in] */ Boolean removeNotification)
{
    String capsuleName;
    GetCapsuleName(&capsuleName);
    AutoPtr<IComponentName> name;
    CComponentName::New(capsuleName, mClassName, (IComponentName**)&name);

    return mActivityManager->SetServiceForeground(
        name, mToken, 0, NULL, removeNotification);
}
