
#include "content/ContextWrapper.h"

ContextWrapper::ContextWrapper()
{}

ContextWrapper::ContextWrapper(
    /* [in] */ IContext* base)
    : mBase(base)
{}

ECode ContextWrapper::GetAssets(
    /* [out] */ IAssetManager** assetManager)
{
    return mBase->GetAssets(assetManager);
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

ECode ContextWrapper::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    return mBase->GetApplicationInfo(info);
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
    /* [in] */ CString name,
    /* [out] */ IInterface** object)
{
    return mBase->GetSystemService(name, object);
}

ECode ContextWrapper::CreateCapsuleContext(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    return mBase->CreateCapsuleContext(capsuleName, flags, ctx);
}

ECode ContextWrapper::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* value)
{
    return mBase->CheckCallingPermission(permission, value);
}

ECode ContextWrapper::EnforceCallingOrSelfPermission(
    /* [in] */ CString permission,
    /* [in] */ CString message)
{
    return mBase->EnforceCallingOrSelfPermission(permission, message);
}

ECode ContextWrapper::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return mBase->RevokeUriPermission(uri, modeFlags);
}

ECode ContextWrapper::CheckCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* perm)
{
    return mBase->CheckCallingOrSelfPermission(permission, perm);
}

ECode ContextWrapper::GrantUriPermission(
    /* [in] */ const String& toCapsule,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return mBase->GrantUriPermission(toCapsule, uri, modeFlags);
}

/**
 * Set the base context for this ContextWrapper.  All calls will then be
 * delegated to the base context.  Throws
 * IllegalStateException if a base context has already been set.
 *
 * @param base The new base context for this wrapper.
 */
ECode ContextWrapper::AttachBaseContext(
    /* [in] */ IContext* base)
{
    if (mBase != NULL) {
        //throw new IllegalStateException("Base context already set");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mBase = base;

    return NOERROR;
}

/**
 * @return the base context as set by the constructor or setBaseContext
 */
ECode ContextWrapper::GetBaseContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context);
    *context = mBase;
    if (*context) {
        (*context)->AddRef();
    }

    return NOERROR;
}

ECode ContextWrapper::Init(
    /* [in] */ IContext* base)
{
    mBase = base;
    return NOERROR;
}
