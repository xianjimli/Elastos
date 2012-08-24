#ifndef __CSERVICE_H__
#define __CSERVICE_H__

#include <elastos.h>
#include <Elastos.Framework.h>
#include "CBaseObject.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

class CService : public CBaseObject, public IService
{
public:
    CService();

    virtual ~CService();

    virtual CARAPI Initialize();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

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
        /* [in] */ const String& name,
        /* [out] */ IInterface** object);

    CARAPI CreateCapsuleContext(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 flags,
        /* [out] */ IContext** ctx);

    CARAPI CheckCallingPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* value);

    CARAPI EnforceCallingOrSelfPermission(
        /* [in] */ CString permission,
        /* [in] */ CString message);

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

    CARAPI GetContentResolver(
        /* [out] */ IContentResolver** resolver);

    CARAPI GetText(
        /* [in] */ Int32 resId,
        /* [out] */ ICharSequence** text);

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

    /**
     * @return the base context as set by the constructor or setBaseContext
     */
    CARAPI GetBaseContext(
        /* [out] */ IContext** ctx);

    CARAPI GetClassName(
        /* [out] */ String* className);

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

#endif //__CSERVICE_H__
