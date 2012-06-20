
#include "app/CApplication.h"

ECode CApplication::GetResources(
    /* [out] */ IResources** resources)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::SetTheme(
    /* [in] */ Int32 resid)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::GetTheme(
    /* [out] */ ITheme** theme)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::ObtainStyledAttributes(
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::ObtainStyledAttributesEx(
    /* [in] */ Int32 resid,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::ObtainStyledAttributesEx2(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::ObtainStyledAttributesEx3(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::GetCapsuleName(
    /* [out] */ String* capsuleName)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::SendBroadcast(
    /* [in] */ IIntent *intent)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::StartActivity(
    /* [in] */ IIntent *intent)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::UnbindService(
    /* [in] */ IServiceConnection* conn)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::GetSystemService(
    /* [in] */ String name,
    /* [out] */ IInterface** object)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::CreateCapsuleContext(
    /* [in] */ String capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::CheckCallingPermission(
    /* [in] */ String permission,
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::EnforceCallingOrSelfPermission(
    /* [in] */ String permission,
    /* [in] */ String message)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::GetBaseContext(
    /* [out] */ IContext** ctx)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplication::Create()
{
    return OnCreate();
}

ECode CApplication::OnCreate()
{
    return NOERROR;
}
