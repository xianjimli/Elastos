#include "CService.h"

CService::CService() :
    mStartCompatibility(FALSE)
{
}

CService::~CService()
{
}

ECode CService::Initialize()
{
    return NOERROR;
}

PInterface CService::Probe(
    /* [in] */ REIID riid)
{
   if (riid == EIID_IService) {
        return (IService *)this;
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
   else {
       return CBaseObject::Probe(riid);
   }
}

UInt32 CService::AddRef()
{
    Int32 nRef = atomic_inc(&mRef);
    return (UInt32)nRef;
}

UInt32 CService::Release()
{
    Int32 nRef;

    nRef = atomic_dec(&mRef);
    if (nRef == 0) {
        delete this;
    }
    return (UInt32)nRef;
}

ECode CService::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IBroadcastReceiver *)this) {
        *pIID = EIID_IBroadcastReceiver;
        return NOERROR;
    }
    else {
        return CBaseObject::GetInterfaceID(pObject, pIID);
    }
}

ECode CService::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CService::Attach(
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

ECode CService::Create()
{
    return OnCreate();
}

ECode CService::Start(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 startId)
{
    return OnStart(intent, startId);
}

ECode CService::StartCommand(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 startId,
    /* [out] */ Int32* result)
{
    return OnStartCommand(intent, flags, startId, result);
}

ECode CService::Destroy()
{
    return OnDestroy();
}

ECode CService::Bind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    return OnBind(intent, binder);
}

ECode CService::Unbind(
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* succeeded)
{
    return OnUnbind(intent, succeeded);
}

ECode CService::Rebind(
    /* [in] */ IIntent* intent)
{
    return OnRebind(intent);
}

ECode CService::OnCreate()
{
    return NOERROR;
}

ECode CService::OnStart(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 startId)
{
    return NOERROR;
}

ECode CService::OnStartCommand(
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

ECode CService::OnDestroy()
{
    return NOERROR;
}

ECode CService::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    return NOERROR;
}

ECode CService::OnUnbind(
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* succeeded)
{
    return NOERROR;
}

ECode CService::OnRebind(
    /* [in] */ IIntent* intent)
{
    return NOERROR;
}

ECode CService::StartActivity(
    /* [in] */ IIntent* intent)
{
    return mBase->StartActivity(intent);
}

ECode CService::SendBroadcast(
    /* [in] */ IIntent* intent)
{
    return mBase->SendBroadcast(intent);
}

ECode CService::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    return mBase->StartService(service, name);
}

ECode CService::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    return mBase->StopService(service, succeeded);
}

ECode CService::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    return mBase->BindService(service, conn, flags, succeeded);
}

ECode CService::UnbindService(
    /* [in] */ IServiceConnection* conn)
{
    return mBase->UnbindService(conn);
}

ECode CService::GetSystemService(
    /* [in] */ const String& name,
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

ECode CService::CreateCapsuleContext(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    return mBase->CreateCapsuleContext(capsuleName, flags, ctx);
}

ECode CService::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CService::EnforceCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    return E_NOT_IMPLEMENTED;
}

ECode CService::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return E_NOT_IMPLEMENTED;
}

ECode CService::CheckCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* perm)
{
    return E_NOT_IMPLEMENTED;
}

ECode CService::GrantUriPermission(
    /* [in] */ const String& toCapsule,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return E_NOT_IMPLEMENTED;
}

ECode CService::GetAssets(
    /* [out] */ IAssetManager** assetManager)
{
    return mBase->GetAssets(assetManager);
}

ECode CService::GetResources(
    /* [out] */ IResources** resources)
{
    return mBase->GetResources(resources);
}

ECode CService::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    return mBase->GetContentResolver(resolver);
}

ECode CService::GetText(
    /* [in] */ Int32 resId,
    /* [out] */ ICharSequence** text)
{
    return mBase->GetText(resId, text);
}

ECode CService::SetTheme(
    /* [in] */ Int32 resid)
{
    mThemeResource = resid;
    return InitializeTheme();
}

ECode CService::GetTheme(
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

ECode CService::ObtainStyledAttributes(
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributes(attrs, styles);
}

ECode CService::ObtainStyledAttributesEx(
    /* [in] */ Int32 resid,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributesEx(resid, attrs, styles);
}

ECode CService::ObtainStyledAttributesEx2(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributesEx2(set, attrs, styles);
}

ECode CService::ObtainStyledAttributesEx3(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributesEx3(set, attrs,
            defStyleAttr, defStyleRes, styles);
}

ECode CService::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    return mBase->GetClassLoader(loader);
}

ECode CService::GetCapsuleName(
    /* [out] */ String* capsuleName)
{
    return mBase->GetCapsuleName(capsuleName);
}

/**
 * @return the base context as set by the constructor or setBaseContext
 */
ECode CService::GetBaseContext(
    /* [out] */ IContext** ctx)
{
    if (ctx == NULL) return E_INVALID_ARGUMENT;

    *ctx = mBase;
    if (*ctx) (*ctx)->AddRef();
    return NOERROR;
}

ECode CService::GetClassName(
    /* [out] */ String* className)
{
    if (className == NULL) return E_INVALID_ARGUMENT;

    *className = mClassName;
    return NOERROR;
}

ECode CService::AttachBaseContext(
    /* [in] */ IContext* newBase)
{
    if (mBase != NULL) {
        //throw new IllegalStateException("Base context already set");
        return E_INVALID_ARGUMENT;
    }
    mBase = newBase;

    return NOERROR;
}

ECode CService::OnApplyThemeResource(
    /* [in] */ ITheme* theme,
    /* [in] */ Int32 resid,
    /* [in] */ Boolean first)
{
    return theme->ApplyStyle(resid, TRUE);
}

ECode CService::InitializeTheme()
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
