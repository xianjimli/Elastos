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
    /* [in] */ String className,
    /* [in] */ IBinder* token,
    /* [in] */ IApplication* application,
    /* [in] */ IActivityManager* activityManager)
{
    mBase = ctx;
    mApartment = apartment;           // NOTE:  unused - remove?
    mClassName = String::Duplicate(className);
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
    /* [in] */ String name,
    /* [out] */ IInterface** object)
{
    return E_NOT_IMPLEMENTED;
}

ECode CService::CreateCapsuleContext(
    /* [in] */ String capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    return mBase->CreateCapsuleContext(capsuleName, flags, ctx);
}

ECode CService::CheckCallingPermission(
    /* [in] */ String permission,
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CService::EnforceCallingOrSelfPermission(
    /* [in] */ String permission,
    /* [in] */ String message)
{
    return E_NOT_IMPLEMENTED;
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

ECode CService::SetTheme(
    /* [in] */ Int32 resid)
{
    return E_NOT_IMPLEMENTED;
}

ECode CService::GetTheme(
    /* [out] */ ITheme** theme)
{
    return E_NOT_IMPLEMENTED;
}

ECode CService::ObtainStyledAttributes(
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return E_NOT_IMPLEMENTED;
}

ECode CService::ObtainStyledAttributesEx(
    /* [in] */ Int32 resid,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return E_NOT_IMPLEMENTED;
}

ECode CService::ObtainStyledAttributesEx2(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return E_NOT_IMPLEMENTED;
}

ECode CService::ObtainStyledAttributesEx3(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    return E_NOT_IMPLEMENTED;
}

ECode CService::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    return E_NOT_IMPLEMENTED;
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

    *className = String::Duplicate(mClassName);
    return NOERROR;
}
