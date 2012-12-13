
#ifdef _FRAMEWORK_CORE
#include "app/Service.h"
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
    else if (riid == EIID_IContextThemeWrapper) {
       return (IContextThemeWrapper*)this;
    }
    else if (riid == EIID_IContextWrapper) {
       return (IContextWrapper*)this;
    }
    else if (riid == EIID_IContext) {
       return (IContext*)this;
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

ECode Service::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Service::EnforceCallingOrSelfPermission(
    /* [in] */ CString permission,
    /* [in] */ CString message)
{
    return E_NOT_IMPLEMENTED;
}

ECode Service::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return E_NOT_IMPLEMENTED;
}

ECode Service::CheckCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* perm)
{
    return E_NOT_IMPLEMENTED;
}

ECode Service::GrantUriPermission(
    /* [in] */ const String& toCapsule,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return E_NOT_IMPLEMENTED;
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
