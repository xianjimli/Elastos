
#ifndef __CCONTEXTIMPL_H__
#define __CCONTEXTIMPL_H__

#include "_CContextImpl.h"
#include <elastos/AutoPtr.h>
#include "app/CApplicationApartment.h"
#include "app/LoadedCap.h"
#include "app/CApplicationContentResolver.h"
#include "content/Context.h"
#include "content/ContextWrapper.h"
#include "content/ContextMacro.h"
#include <elastos/Mutex.h>
#include <elastos/ElRefBase.h>

class ReceiverRestrictedContext
    : public ElRefBase
    , public ContextWrapper
    , public IContextWrapper
{
public:
    ICONTEXT_METHODS_DECL();

    ReceiverRestrictedContext(
        /* [in] */ IContext* context);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetBaseContext(
        /* [out] */ IContext** context);

//    ReceiverRestrictedContext(Context base) {
//        super(base);
//    }
//
//    @Override
//    public Intent registerReceiver(BroadcastReceiver receiver, IntentFilter filter) {
//        return registerReceiver(receiver, filter, null, null);
//    }
//
//    @Override
//    public Intent registerReceiver(BroadcastReceiver receiver, IntentFilter filter,
//            String broadcastPermission, Handler scheduler) {
//        throw new ReceiverCallNotAllowedException(
//                "IntentReceiver components are not allowed to register to receive intents");
//        //ex.fillInStackTrace();
//        //Log.e("IntentReceiver", ex.getMessage(), ex);
//        //return mContext.registerReceiver(receiver, filter, broadcastPermission,
//        //        scheduler);
//    }
//
//    @Override
//    public boolean bindService(Intent service, ServiceConnection conn, int flags) {
//        throw new ReceiverCallNotAllowedException(
//                "IntentReceiver components are not allowed to bind to services");
//        //ex.fillInStackTrace();
//        //Log.e("IntentReceiver", ex.getMessage(), ex);
//        //return mContext.bindService(service, interfaceName, conn, flags);
//    }
};

CarClass(CContextImpl), public Context
{
public:
    CContextImpl();

    ~CContextImpl();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

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

    CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    CARAPI StartActivity(
        /* [in] */ IIntent *intent);

    CARAPI SendBroadcast(
        /* [in] */ IIntent *pIntent);

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
        /* [out] */ IContext** context);

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

public:
    CARAPI Init(
        /* [in] */ LoadedCap* capsuleInfo,
        /* [in] */ IBinder* activityToken,
        /* [in] */ CApplicationApartment* apartment);

    CARAPI Init(
        /* [in] */ LoadedCap* capsuleInfo,
        /* [in] */ IBinder* activityToken,
        /* [in] */ CApplicationApartment* apartment,
        /* [in] */ IResources* container);

    CARAPI_(IContext*) GetReceiverRestrictedContext();

    CARAPI_(void) SetOuterContext(
        /* [in] */ IContext* context);

    CARAPI_(IContext*) GetOuterContext();

    CARAPI_(void) SetActivityToken(
        /* [in] */ IBinder* token);

    CARAPI_(IBinder*) GetActivityToken();

    CARAPI_(void) ScheduleFinalCleanup(
        /* [in] */ const String& who,
        /* [in] */ const String& what);

    CARAPI PerformFinalCleanup(
        /* [in] */ const String& who,
        /* [in] */ const String& what);

private:
    static const char* TAG;

    LoadedCap* mCapsuleInfo;
    AutoPtr<IResources> mResources;
    AutoPtr<CApplicationApartment> mApartment;
    AutoPtr<IContext> mOuterContext;
    AutoPtr<IBinder> mActivityToken;
    AutoPtr<CApplicationContentResolver> mContentResolver;
    Int32 mThemeResource;
    AutoPtr<ITheme> mTheme;
    AutoPtr<IContext> mReceiverRestrictedContext;
    AutoPtr<ILayoutInflater> mLayoutInflater;

    Mutex mSync;
};

#endif // __CCONTEXTIMPL_H__
