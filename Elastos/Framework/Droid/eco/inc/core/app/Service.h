#ifndef __SERVICE_H__
#define __SERVICE_H__

#ifdef _FRAMEWORK_CORE
#include "ext/frameworkext.h"
#else
#define __USE_MALLOC
#include "Elastos.Framework.Core.h"
#endif
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class Service
    : public ElRefBase
    , public IObject
    , public IService
    , public IComponentCallbacks
{
public:
    Service();

    virtual ~Service();

    CARAPI GetApplication(
        /* [out] */ IApplication** application);

    virtual CARAPI Initialize();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Aggregate(
        /* [in] */ AggrType aggrType,
        /* [in] */ PInterface pObject);

    CARAPI GetDomain(
        /* [out] */ PInterface *ppObject);

    CARAPI GetClassID(
        /* [out] */ ClassID *pCLSID);

    CARAPI Attach(
        /* [in] */ IContext* ctx,
        /* [in] */ IApplicationApartment* apartment,
        /* [in] */ const String& className,
        /* [in] */ IBinder* token,
        /* [in] */ IApplication* application,
        /* [in] */ IActivityManager* activityManager);

    CARAPI Create();

    CARAPI Start(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 startId);

    CARAPI StartCommand(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 startId,
        /* [out] */ Int32* result);

    CARAPI Destroy();

    CARAPI Bind(
        /* [in] */ IIntent* intent,
        /* [out] */ IBinder** binder);

    CARAPI Unbind(
        /* [in] */ IIntent* intent,
        /* [out] */ Boolean* succeeded);

    CARAPI Rebind(
        /* [in] */ IIntent* intent);

    CARAPI StartActivity(
        /* [in] */ IIntent* intent);

    CARAPI StartIntentSender(
        /* [in] */ IIntentSender* intent,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ Int32 extraFlags);

    CARAPI SendBroadcast(
        /* [in] */ IIntent* intent);

    CARAPI StartService(
        /* [in] */ IIntent* service,
        /* [out] */ IComponentName** name);

    CARAPI StopService(
        /* [in] */ IIntent* service,
        /* [out] */ Boolean* succeeded);

    CARAPI BindService(
        /* [in] */ IIntent* service,
        /* [in] */ IServiceConnection* conn,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

    CARAPI UnbindService(
        /* [in] */ IServiceConnection* conn);

    CARAPI GetSystemService(
        /* [in] */ CString name,
        /* [out] */ IInterface** object);

    CARAPI CreateCapsuleContext(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 flags,
        /* [out] */ IContext** ctx);

    CARAPI CheckPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [out] */ Int32 * result);

    CARAPI CheckUriPermission(
        /* [in] */ IUri * uri,
        /* [in] */ const String& readPermission,
        /* [in] */ const String& writePermission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32 * result);

    CARAPI CheckUriPermissionEx(
        /* [in] */ IUri * uri,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32 * result);

    CARAPI CheckCallingPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* value);

    CARAPI EnforceCallingOrSelfPermission(
        /* [in] */ const String& permission,
        /* [in] */ const String& message);

    CARAPI RevokeUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI CheckCallingOrSelfPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* perm);

    CARAPI GrantUriPermission(
        /* [in] */ const String& toCapsule,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI GetAssets(
        /* [out] */ IAssetManager** assetManager);

    CARAPI GetResources(
        /* [out] */ IResources** resources);

    CARAPI GetCapsuleManager(
        /* [out] */ ILocalCapsuleManager** capsuleManager);

    CARAPI GetContentResolver(
        /* [out] */ IContentResolver** resolver);

    CARAPI GetApplicationContext(
        /* [out] */ IContext** ctx);

    CARAPI GetText(
        /* [in] */ Int32 resId,
        /* [out] */ ICharSequence** text);

    CARAPI GetString(
        /* [in] */ Int32 resId,
        /* [out] */ String* str);

    CARAPI SetTheme(
        /* [in] */ Int32 resid);

    CARAPI GetTheme(
        /* [out] */ ITheme** theme);

    CARAPI ObtainStyledAttributes(
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributesEx(
        /* [in] */ Int32 resid,
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributesEx2(
        /* [in] */ IAttributeSet* set,
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributesEx3(
        /* [in] */ IAttributeSet* set,
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes,
        /* [out] */ ITypedArray** styles);

    CARAPI GetClassLoader(
        /* [out] */ IClassLoader** loader);

    CARAPI GetCapsuleName(
        /* [out] */ String* capsuleName);

    CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    CARAPI GetCapsuleResourcePath(
        /* [out] */ String* path);

    CARAPI GetFilesDir(
        /* [out] */ IFile** filesDir);

    CARAPI GetExternalFilesDir(
        /* [in] */ const String& type,
        /* [out] */ IFile** filesDir);

    CARAPI GetCacheDir(
        /* [out] */ IFile** cacheDir);

    CARAPI GetDir(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [out] */ IFile** dir);

    /**
     * @return the base context as set by the constructor or setBaseContext
     */
    CARAPI GetBaseContext(
        /* [out] */ IContext** ctx);

    CARAPI GetClassName(
        /* [out] */ String* className);

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI OnLowMemory();

    CARAPI StopSelf();

    CARAPI StopSelfEx(
        /* [in] */ Int32 startId);

    CARAPI StopSelfResult(
        /* [in] */ Int32 startId,
        /* [out] */ Boolean* res);

    CARAPI SetForeground(
        /* [in] */ Boolean isForeground);

    CARAPI StartForeground(
        /* [in] */ Int32 id,
        /* [in] */ INotification* notification);

    CARAPI StopForeground(
        /* [in] */ Boolean removeNotification);

protected:
    virtual CARAPI OnCreate();

    virtual CARAPI OnStart(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 startId);

    virtual CARAPI OnStartCommand(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 startId,
        /* [out] */ Int32* result);

    virtual CARAPI OnDestroy();

    virtual CARAPI OnBind(
        /* [in] */ IIntent* intent,
        /* [out] */ IBinder** binder);

    virtual CARAPI OnUnbind(
        /* [in] */ IIntent* intent,
        /* [out] */ Boolean* succeeded);

    virtual CARAPI OnRebind(
        /* [in] */ IIntent* intent);

    virtual CARAPI AttachBaseContext(
        /* [in] */ IContext* base);

    virtual CARAPI OnApplyThemeResource(
        /* [in] */ ITheme* theme,
        /* [in] */ Int32 resid,
        /* [in] */ Boolean first);

private:
    CARAPI InitializeTheme();

private:
    // set by the thread after the constructor and before onCreate(Bundle icicle) is called.
    AutoPtr<IApplicationApartment> mApartment;
    String mClassName;
    AutoPtr<IBinder> mToken;
    AutoPtr<IApplication> mApplication;
    AutoPtr<IActivityManager> mActivityManager;

    AutoPtr<IContext> mBase;
    Int32 mThemeResource;
    AutoPtr<ITheme> mTheme;
    AutoPtr<ILayoutInflater> mInflater;

    Boolean mStartCompatibility;
};

#endif //__SERVICE_H__
