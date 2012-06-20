
#include "content/ContextWrapper.h"

ContextWrapper::ContextWrapper(
    /* [in] */ IContext* base) :
    mBase(base)
{}

IInterface* ContextWrapper::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IContext) {
        return (IContext*)this;
    }

    return NULL;
}

UInt32 ContextWrapper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ContextWrapper::Release()
{
    return ElRefBase::Release();
}

ECode ContextWrapper::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IContext *)this) {
        *pIID = EIID_IContext;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode ContextWrapper::CreateCapsuleContext(
    /* [in] */ String capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    return mBase->CreateCapsuleContext(capsuleName, flags, ctx);
}

ECode ContextWrapper::CheckCallingPermission(
    /* [in] */ String permission,
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContextWrapper::EnforceCallingOrSelfPermission(
    /* [in] */ String permission,
    /* [in] */ String message)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContextWrapper::GetResources(
    /* [out] */ IResources** resources)
{
    return mBase->GetResources(resources);
}

ECode ContextWrapper::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    return mBase->GetContentResolver(resolver);
}

ECode ContextWrapper::SetTheme(
    /* [in] */ Int32 resid)
{
    return mBase->SetTheme(resid);
}

ECode ContextWrapper::GetTheme(
    /* [out] */ ITheme** theme)
{
    return mBase->GetTheme(theme);
}

ECode ContextWrapper::ObtainStyledAttributes(
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributes(attrs, styles);
}

ECode ContextWrapper::ObtainStyledAttributesEx(
    /* [in] */ Int32 resid,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributesEx(resid, attrs, styles);
}

ECode ContextWrapper::ObtainStyledAttributesEx2(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributesEx2(set, attrs, styles);
}

ECode ContextWrapper::ObtainStyledAttributesEx3(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributesEx3(
            set, attrs, defStyleAttr, defStyleRes, styles);
}

ECode ContextWrapper::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    return mBase->GetClassLoader(loader);
}

ECode ContextWrapper::GetCapsuleName(
    /* [out] */ String* capsuleName)
{
    return mBase->GetCapsuleName(capsuleName);
}

ECode ContextWrapper::SendBroadcast(
    /* [in] */ IIntent* intent)
{
    return mBase->SendBroadcast(intent);
}

ECode ContextWrapper::StartActivity(
    /* [in] */ IIntent* intent)
{
    return mBase->StartActivity(intent);
}

ECode ContextWrapper::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    return mBase->StartService(service, name);
}

ECode ContextWrapper::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    return mBase->StopService(service, succeeded);
}

ECode ContextWrapper::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    return mBase->BindService(service, conn, flags, succeeded);
}

ECode ContextWrapper::UnbindService(
    /* [in] */ IServiceConnection* conn)
{
    return mBase->UnbindService(conn);
}

ECode ContextWrapper::GetSystemService(
    /* [in] */ String name,
    /* [out] */ IInterface** object)
{
    return mBase->GetSystemService(name, object);
}
