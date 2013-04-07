
#ifndef __CONTEXTMACRO_H__
#define __CONTEXTMACRO_H__

#define ICONTEXT_METHODS_DECL()                                             \
    CARAPI GetAssets(                                                       \
        /* [out] */ IAssetManager** assetManager);                          \
                                                                            \
    CARAPI GetResources(                                                    \
        /* [out] */ IResources** resources);                                \
                                                                            \
    CARAPI GetCapsuleManager(                                               \
        /* [out] */ ILocalCapsuleManager** capsuleManager);                 \
                                                                            \
    CARAPI GetContentResolver(                                              \
        /* [out] */ IContentResolver** resolver);                           \
                                                                            \
    CARAPI GetApplicationContext(                                           \
        /* [out] */ IContext** ctx);                                        \
                                                                            \
    CARAPI GetText(                                                         \
        /* [in] */ Int32 resId,                                             \
        /* [out] */ ICharSequence** text);                                  \
                                                                            \
    CARAPI GetString(                                                       \
        /* [in] */ Int32 resId,                                             \
        /* [out] */ String* str);                                           \
                                                                            \
    CARAPI SetTheme(                                                        \
        /* [in] */ Int32 resid);                                            \
                                                                            \
    CARAPI GetTheme(                                                        \
        /* [out] */ ITheme** theme);                                        \
                                                                            \
    CARAPI ObtainStyledAttributes(                                          \
        /* [in] */ const ArrayOf<Int32>& attrs,                             \
        /* [out] */ ITypedArray** styles);                                  \
                                                                            \
    CARAPI ObtainStyledAttributesEx(                                        \
        /* [in] */ Int32 resid,                                             \
        /* [in] */ const ArrayOf<Int32>& attrs,                             \
        /* [out] */ ITypedArray** styles);                                  \
                                                                            \
    CARAPI ObtainStyledAttributesEx2(                                       \
        /* [in] */ IAttributeSet* set,                                      \
        /* [in] */ const ArrayOf<Int32>& attrs,                             \
        /* [out] */ ITypedArray** styles);                                  \
                                                                            \
    CARAPI ObtainStyledAttributesEx3(                                       \
        /* [in] */ IAttributeSet* set,                                      \
        /* [in] */ const ArrayOf<Int32>& attrs,                             \
        /* [in] */ Int32 defStyleAttr,                                      \
        /* [in] */ Int32 defStyleRes,                                       \
        /* [out] */ ITypedArray** styles);                                  \
                                                                            \
    CARAPI GetClassLoader(                                                  \
        /* [out] */ IClassLoader** loader);                                 \
                                                                            \
    CARAPI GetCapsuleName(                                                  \
        /* [out] */ String* capsuleName);                                   \
                                                                            \
    CARAPI GetApplicationInfo(                                              \
        /* [out] */ IApplicationInfo** info);                               \
                                                                            \
    CARAPI GetCapsuleResourcePath(                                          \
        /* [out] */ String* path);                                          \
                                                                            \
    CARAPI GetFilesDir(                                                     \
        /* [out] */ IFile** filesDir);                                      \
                                                                            \
    CARAPI GetExternalFilesDir(                                             \
    /* [in] */ const String& type,                                          \
    /* [out] */ IFile** filesDir);                                          \
                                                                            \
    CARAPI GetCacheDir(                                                     \
        /* [out] */ IFile** cacheDir);                                      \
                                                                            \
    CARAPI GetDir(                                                          \
    /* [in] */ const String& name,                                          \
    /* [in] */ Int32 mode,                                                  \
    /* [out] */ IFile** dir);                                               \
                                                                            \
    CARAPI StartActivity(                                                   \
        /* [in] */ IIntent *intent);                                        \
                                                                            \
    CARAPI StartIntentSender(                                               \
        /* [in] */ IIntentSender* intent,                                   \
        /* [in] */ IIntent* fillInIntent,                                   \
        /* [in] */ Int32 flagsMask,                                         \
        /* [in] */ Int32 flagsValues,                                       \
        /* [in] */ Int32 extraFlags);                                       \
                                                                            \
    CARAPI SendBroadcast(                                                   \
        /* [in] */ IIntent *pIntent);                                       \
                                                                            \
    CARAPI StartService(                                                    \
        /* [in] */ IIntent* service,                                        \
        /* [out] */ IComponentName** name);                                 \
                                                                            \
    CARAPI StopService(                                                     \
        /* [in] */ IIntent* service,                                        \
        /* [out] */ Boolean* succeeded);                                    \
                                                                            \
    CARAPI BindService(                                                     \
        /* [in] */ IIntent* service,                                        \
        /* [in] */ IServiceConnection* conn,                                \
        /* [in] */ Int32 flags,                                             \
        /* [out] */ Boolean* succeeded);                                    \
                                                                            \
    CARAPI UnbindService(                                                   \
        /* [in] */ IServiceConnection* conn);                               \
                                                                            \
    CARAPI GetSystemService(                                                \
        /* [in] */ CString name,                                            \
        /* [out] */ IInterface** object);                                   \
                                                                            \
    CARAPI CreateCapsuleContext(                                            \
        /* [in] */ const String& capsuleName,                               \
        /* [in] */ Int32 flags,                                             \
        /* [out] */ IContext** context);                                    \
                                                                            \
    CARAPI CheckCallingPermission(                                          \
        /* [in] */ const String& permission,                                \
        /* [out] */ Int32* value);                                          \
                                                                            \
    CARAPI EnforceCallingOrSelfPermission(                                  \
        /* [in] */ CString permission,                                      \
        /* [in] */ CString message);                                        \
                                                                            \
    CARAPI RevokeUriPermission(                                             \
        /* [in] */ IUri* uri,                                               \
        /* [in] */ Int32 modeFlags);                                        \
                                                                            \
    CARAPI CheckCallingOrSelfPermission(                                    \
        /* [in] */ const String& permission,                                \
        /* [out] */ Int32* perm);                                           \
                                                                            \
    CARAPI GrantUriPermission(                                              \
        /* [in] */ const String& toCapsule,                                 \
        /* [in] */ IUri* uri,                                               \
        /* [in] */ Int32 modeFlags);                                        \
                                                                            \
    CARAPI CheckPermissionEx(                                               \
        /* [in] */ const String& permName,                                  \
        /* [in] */ const String& pkgName,                                   \
        /* [out] */ Int32 * result);                                        \
                                                                            \
    CARAPI CheckPermission(                                                 \
        /* [in] */ const String& permission,                                \
        /* [in] */ Int32 pid,                                               \
        /* [in] */ Int32 uid,                                               \
        /* [out] */ Int32 * result);                                        \
                                                                            \
    CARAPI CheckUriPermission(                                              \
        /* [in] */ IUri * uri,                                              \
        /* [in] */ const String& readPermission,                            \
        /* [in] */ const String& writePermission,                           \
        /* [in] */ Int32 pid,                                               \
        /* [in] */ Int32 uid,                                               \
        /* [in] */ Int32 modeFlags,                                         \
        /* [out] */ Int32 * result);                                        \
                                                                            \
    CARAPI CheckUriPermissionEx(                                            \
        /* [in] */ IUri * uri,                                              \
        /* [in] */ Int32 pid,                                               \
        /* [in] */ Int32 uid,                                               \
        /* [in] */ Int32 modeFlags,                                         \
        /* [out] */ Int32 * result);


#define ICONTEXT_METHODS_IMPL(className, superClass, overRideClass)         \
ECode className::GetAssets(                                                 \
    /* [out] */ IAssetManager** assetManager)                               \
{                                                                           \
    return superClass::GetAssets(assetManager);                             \
}                                                                           \
                                                                            \
ECode className::GetResources(                                              \
    /* [out] */ IResources** resources)                                     \
{                                                                           \
    return superClass::GetResources(resources);                             \
}                                                                           \
                                                                            \
ECode className::GetCapsuleManager(                                         \
    /* [out] */ ILocalCapsuleManager** capsuleManager)                      \
{                                                                           \
    return superClass::GetCapsuleManager(capsuleManager);                   \
}                                                                           \
                                                                            \
ECode className::GetContentResolver(                                        \
    /* [out] */ IContentResolver** resolver)                                \
{                                                                           \
    return superClass::GetContentResolver(resolver);                        \
}                                                                           \
                                                                            \
ECode className::GetApplicationContext(                                     \
    /* [out] */ IContext** ctx)                                             \
{                                                                           \
    return superClass::GetApplicationContext(ctx);                          \
}                                                                           \
                                                                            \
ECode className::GetText(                                                   \
    /* [in] */ Int32 resId,                                                 \
    /* [out] */ ICharSequence** text)                                       \
{                                                                           \
    return superClass::GetText(resId, text);                                \
}                                                                           \
                                                                            \
ECode className::GetString(                                                 \
    /* [in] */ Int32 resId,                                                 \
    /* [out] */ String* str)                                                \
{                                                                           \
    return superClass::GetString(resId, str);                               \
}                                                                           \
                                                                            \
ECode className::SetTheme(                                                  \
    /* [in] */ Int32 resid)                                                 \
{                                                                           \
    return superClass::SetTheme(resid);                                     \
}                                                                           \
                                                                            \
ECode className::GetTheme(                                                  \
    /* [out] */ ITheme** theme)                                             \
{                                                                           \
    return superClass::GetTheme(theme);                                     \
}                                                                           \
                                                                            \
ECode className::ObtainStyledAttributes(                                    \
    /* [in] */ const ArrayOf<Int32>& attrs,                                 \
    /* [out] */ ITypedArray** styles)                                       \
{                                                                           \
    return superClass::ObtainStyledAttributes(attrs, styles);               \
}                                                                           \
                                                                            \
ECode className::ObtainStyledAttributesEx(                                  \
    /* [in] */ Int32 resid,                                                 \
    /* [in] */ const ArrayOf<Int32>& attrs,                                 \
    /* [out] */ ITypedArray** styles)                                       \
{                                                                           \
    return superClass::ObtainStyledAttributesEx(resid, attrs, styles);      \
}                                                                           \
                                                                            \
ECode className::ObtainStyledAttributesEx2(                                 \
    /* [in] */ IAttributeSet* set,                                          \
    /* [in] */ const ArrayOf<Int32>& attrs,                                 \
    /* [out] */ ITypedArray** styles)                                       \
{                                                                           \
    return superClass::ObtainStyledAttributesEx2(set, attrs, styles);       \
}                                                                           \
                                                                            \
ECode className::ObtainStyledAttributesEx3(                                 \
    /* [in] */ IAttributeSet* set,                                          \
    /* [in] */ const ArrayOf<Int32>& attrs,                                 \
    /* [in] */ Int32 defStyleAttr,                                          \
    /* [in] */ Int32 defStyleRes,                                           \
    /* [out] */ ITypedArray** styles)                                       \
{                                                                           \
    return superClass::ObtainStyledAttributesEx3(                           \
        set, attrs, defStyleAttr, defStyleRes, styles);                     \
}                                                                           \
                                                                            \
ECode className::GetClassLoader(                                            \
    /* [out] */ IClassLoader** loader)                                      \
{                                                                           \
    return superClass::GetClassLoader(loader);                              \
}                                                                           \
                                                                            \
ECode className::GetCapsuleName(                                            \
    /* [out] */ String* capsuleName)                                        \
{                                                                           \
    return superClass::GetCapsuleName(capsuleName);                         \
}                                                                           \
                                                                            \
ECode className::GetApplicationInfo(                                        \
    /* [out] */ IApplicationInfo** info)                                    \
{                                                                           \
    return superClass::GetApplicationInfo(info);                            \
}                                                                           \
                                                                            \
ECode className::GetCapsuleResourcePath(                                    \
    /* [out] */ String* path)                                               \
{                                                                           \
    return superClass::GetCapsuleResourcePath(path);                        \
}                                                                           \
                                                                            \
ECode className::GetFilesDir(                                               \
    /* [out] */ IFile** filesDir)                                           \
{                                                                           \
    return superClass::GetFilesDir(filesDir);                               \
}                                                                           \
                                                                            \
ECode className::GetExternalFilesDir(                                       \
    /* [in] */ const String& type,                                          \
    /* [out] */ IFile** filesDir)                                           \
{                                                                           \
    return superClass::GetExternalFilesDir(type, filesDir);                 \
}                                                                           \
                                                                            \
ECode className::GetCacheDir(                                               \
    /* [out] */ IFile** cacheDir)                                           \
{                                                                           \
    return superClass::GetCacheDir(cacheDir);                               \
}                                                                           \
                                                                            \
ECode className::GetDir(                                                    \
    /* [in] */ const String& name,                                          \
    /* [in] */ Int32 mode,                                                  \
    /* [out] */ IFile** dir)                                                \
{                                                                           \
    return superClass::GetDir(name, mode, dir);                             \
}                                                                           \
                                                                            \
ECode className::SendBroadcast(                                             \
    /* [in] */ IIntent* intent)                                             \
{                                                                           \
    return superClass::SendBroadcast(intent);                               \
}                                                                           \
                                                                            \
ECode className::StartActivity(                                             \
    /* [in] */ IIntent* intent)                                             \
{                                                                           \
    return superClass::StartActivity(intent);                               \
}                                                                           \
                                                                            \
ECode className::StartIntentSender(                                         \
    /* [in] */ IIntentSender* intent,                                       \
    /* [in] */ IIntent* fillInIntent,                                       \
    /* [in] */ Int32 flagsMask,                                             \
    /* [in] */ Int32 flagsValues,                                           \
    /* [in] */ Int32 extraFlags)                                            \
{                                                                           \
    return superClass::StartIntentSender(intent, fillInIntent, flagsMask,   \
        flagsValues, extraFlags);                                           \
}                                                                           \
                                                                            \
ECode className::StartService(                                              \
    /* [in] */ IIntent* service,                                            \
    /* [out] */ IComponentName** name)                                      \
{                                                                           \
    return superClass::StartService(service, name);                         \
}                                                                           \
                                                                            \
ECode className::StopService(                                               \
    /* [in] */ IIntent* service,                                            \
    /* [out] */ Boolean* succeeded)                                         \
{                                                                           \
    return superClass::StopService(service, succeeded);                     \
}                                                                           \
                                                                            \
ECode className::BindService(                                               \
    /* [in] */ IIntent* service,                                            \
    /* [in] */ IServiceConnection* conn,                                    \
    /* [in] */ Int32 flags,                                                 \
    /* [out] */ Boolean* succeeded)                                         \
{                                                                           \
    return superClass::BindService(service, conn, flags, succeeded);        \
}                                                                           \
                                                                            \
ECode className::UnbindService(                                             \
    /* [in] */ IServiceConnection* conn)                                    \
{                                                                           \
    return superClass::UnbindService(conn);                                 \
}                                                                           \
                                                                            \
ECode className::GetSystemService(                                          \
    /* [in] */ CString name,                                                \
    /* [out] */ IInterface** object)                                        \
{                                                                           \
    return superClass::GetSystemService(name, object);                      \
}                                                                           \
                                                                            \
ECode className::CreateCapsuleContext(                                      \
    /* [in] */ const String& capsuleName,                                   \
    /* [in] */ Int32 flags,                                                 \
    /* [out] */ IContext** ctx)                                             \
{                                                                           \
    return superClass::CreateCapsuleContext(capsuleName, flags, ctx);       \
}                                                                           \
                                                                            \
ECode className::CheckCallingPermission(                                    \
    /* [in] */ const String& permission,                                    \
    /* [out] */ Int32* value)                                               \
{                                                                           \
    return superClass::CheckCallingPermission(permission, value);           \
}                                                                           \
                                                                            \
ECode className::EnforceCallingOrSelfPermission(                            \
    /* [in] */ CString permission,                                          \
    /* [in] */ CString message)                                             \
{                                                                           \
    return superClass::EnforceCallingOrSelfPermission(permission, message); \
}                                                                           \
                                                                            \
ECode className::RevokeUriPermission(                                       \
    /* [in] */ IUri* uri,                                                   \
    /* [in] */ Int32 modeFlags)                                             \
{                                                                           \
    return superClass::RevokeUriPermission(uri, modeFlags);                 \
}                                                                           \
                                                                            \
ECode className::CheckCallingOrSelfPermission(                              \
    /* [in] */ const String& permission,                                    \
    /* [out] */ Int32* perm)                                                \
{                                                                           \
    return superClass::CheckCallingOrSelfPermission(permission, perm);      \
}                                                                           \
                                                                            \
ECode className::GrantUriPermission(                                        \
    /* [in] */ const String& toCapsule,                                     \
    /* [in] */ IUri* uri,                                                   \
    /* [in] */ Int32 modeFlags)                                             \
{                                                                           \
    return superClass::GrantUriPermission(toCapsule, uri, modeFlags);       \
}                                                                           \
                                                                            \
ECode className::CheckPermissionEx(                                         \
    /* [in] */ const String& permName,                                      \
    /* [in] */ const String& pkgName,                                       \
    /* [out] */ Int32 * result)                                             \
{                                                                           \
    return superClass::CheckPermissionEx(permName, pkgName, result);        \
}                                                                           \
                                                                            \
ECode className::CheckPermission(                                           \
    /* [in] */ const String& permission,                                    \
    /* [in] */ Int32 pid,                                                   \
    /* [in] */ Int32 uid,                                                   \
    /* [out] */ Int32 * result)                                             \
{                                                                           \
    return superClass::CheckPermission(permission, pid, uid, result);       \
}                                                                           \
                                                                            \
ECode className::CheckUriPermission(                                        \
    /* [in] */ IUri * uri,                                                  \
    /* [in] */ const String& readPermission,                                \
    /* [in] */ const String& writePermission,                               \
    /* [in] */ Int32 pid,                                                   \
    /* [in] */ Int32 uid,                                                   \
    /* [in] */ Int32 modeFlags,                                             \
    /* [out] */ Int32 * result)                                             \
{                                                                           \
    return superClass::CheckUriPermission(                                  \
            uri,                                                            \
            readPermission,                                                 \
            writePermission,                                                \
            pid,                                                            \
            uid,                                                            \
            modeFlags,                                                      \
            result);                                                        \
}                                                                           \
                                                                            \
ECode className::CheckUriPermissionEx(                                      \
    /* [in] */ IUri * uri,                                                  \
    /* [in] */ Int32 pid,                                                   \
    /* [in] */ Int32 uid,                                                   \
    /* [in] */ Int32 modeFlags,                                             \
    /* [out] */ Int32 * result)                                             \
{                                                                           \
    return superClass::CheckUriPermissionEx(                                \
            uri,                                                            \
            pid,                                                            \
            uid,                                                            \
            modeFlags,                                                      \
            result);                                                        \
}

#endif //__CONTEXTMACRO_H__
