
#ifndef __CCONTEXTIMPL_H__
#define __CCONTEXTIMPL_H__

#include "_CContextImpl.h"
#include "app/CApplicationApartment.h"
#include "app/LoadedCap.h"
#include "app/CApplicationContentResolver.h"
#include "content/Context.h"
#include "content/ContextWrapper.h"
#include "content/ContextMacro.h"
#include "content/CResolveInfo.h"
#include "content/cm/CapsuleManager.h"
#include <elastos/Mutex.h>
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

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
    class ApplicationCapsuleManager
        : public ElRefBase
        , public CapsuleManager
        , public ILocalCapsuleManager
    {
    private:
        class ResourceName : public ElRefBase
        {
        public:
            ResourceName(
                /* [in] */ const String& capsuleName,
                /* [in] */ Int32 iconId);

            ResourceName(
                /* [in] */ CApplicationInfo* aInfo,
                /* [in] */ Int32 iconId);

            ResourceName(
                /* [in] */ ComponentInfo* cInfo,
                /* [in] */ Int32 iconId);

            ResourceName(
                /* [in] */ CResolveInfo* rInfo,
                /* [in] */ Int32 iconId);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            // @Override
            // public boolean equals(Object o) {
            //     if (this == o) return true;
            //     if (o == null || getClass() != o.getClass()) return false;

            //     ResourceName that = (ResourceName) o;

            //     if (iconId != that.iconId) return false;
            //     return !(packageName != null ?
            //             !packageName.equals(that.packageName) : that.packageName != null);

            // }

            // @Override
            // public int hashCode() {
            //     int result;
            //     result = packageName.hashCode();
            //     result = 31 * result + iconId;
            //     return result;
            // }

            // @Override
            // public String toString() {
            //     return "{ResourceName " + packageName + " / " + iconId + "}";
            // }

        public:
            String mCapsuleName;
            Int32 mIconId;
        };

    public:
        ApplicationCapsuleManager(
            /* [in] */ CContextImpl* context,
            /* [in] */ ICapsuleManager* pm);

        ~ApplicationCapsuleManager();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetCapsuleInfo(
            /* [in] */ const String& capsuleName,
            /* [in] */ Int32 flags,
            /* [out] */ ICapsuleInfo** capInfo);

        CARAPI CurrentToCanonicalCapsuleNames(
            /* [in] */ const ArrayOf<String>& names,
            /* [out, callee] */ ArrayOf<String>** cnames);

        CARAPI CanonicalToCurrentCapsuleNames(
            /* [in] */ const ArrayOf<String>& names,
            /* [out, callee] */ ArrayOf<String>** cnames);

        CARAPI GetLaunchIntentForCapsuel(
            /* [in] */ const String& capsuleName,
            /* [out] */ IIntent** intent);

        CARAPI GetCapsuleGids(
            /* [in] */ const String& capsuleName,
            /* [out, callee] */ ArrayOf<Int32>** gids);

        CARAPI GetPermissionInfo(
            /* [in] */ const String& name,
            /* [in] */ Int32 flags,
            /* [out] */ IPermissionInfo** info);

        CARAPI QueryPermissionsByGroup(
            /* [in] */ const String& group,
            /* [in] */ Int32 flags,
            /* [out] */ IObjectContainer** infos);

        CARAPI GetPermissionGroupInfo(
            /* [in] */ const String& name,
            /* [in] */ Int32 flags,
            /* [out] */ IPermissionGroupInfo** info);

        CARAPI GetAllPermissionGroups(
            /* [in] */ Int32 flags,
            /* [out] */ IObjectContainer** infos);

        CARAPI GetApplicationInfo(
            /* [in] */ const String& capsuleName,
            /* [in] */ Int32 flags,
            /* [out] */ IApplicationInfo** appInfo);

        CARAPI GetActivityInfo(
            /* [in] */ IComponentName* component,
            /* [in] */ Int32 flags,
            /* [out] */ IActivityInfo** activityInfo);

        CARAPI GetReceiverInfo(
            /* [in] */ IComponentName* component,
            /* [in] */ Int32 flags,
            /* [out] */ IActivityInfo** info);

        CARAPI GetServiceInfo(
            /* [in] */ IComponentName* component,
            /* [in] */ Int32 flags,
            /* [out] */ IServiceInfo** info);

        CARAPI GetContentProviderInfo(
            /* [in] */ IComponentName* className,
            /* [in] */ Int32 flags,
            /* [out] */ IContentProviderInfo** info);

        CARAPI GetSystemSharedLibraryNames(
            /* [out, callee] */ ArrayOf<String>** names);

        CARAPI GetSystemAvailableFeatures(
            /* [out] */ IObjectContainer** infos);

        CARAPI HasSystemFeature(
            /* [in] */ const String& name,
            /* [out] */ Boolean* result);

        CARAPI CheckPermission(
            /* [in] */ const String& permName,
            /* [in] */ const String& capName,
            /* [out] */ Int32* perm);

        CARAPI AddPermission(
            /* [in] */ IPermissionInfo* info,
            /* [out] */ Boolean* isAdded);

        CARAPI AddPermissionAsync(
            /* [in] */ IPermissionInfo* info,
            /* [out] */ Boolean* isAdded);

        CARAPI RemovePermission(
            /* [in] */ const String& name);

        CARAPI CheckSignatures(
            /* [in] */ const String& cap1,
            /* [in] */ const String& cap2,
            /* [out] */ Int32* sig);

        CARAPI CheckUidSignatures(
            /* [in] */ Int32 uid1,
            /* [in] */ Int32 uid2,
            /* [out] */ Int32* sig);

        CARAPI GetCapsulesForUid(
            /* [in] */ Int32 uid,
            /* [out, callee] */ ArrayOf<String>** capsules);

        CARAPI GetNameForUid(
            /* [in] */ Int32 uid,
            /* [out] */ String* name);

        CARAPI GetUidForSharedUser(
            /* [in] */ const String& sharedUserName,
            /* [out] */ Int32* uid);

        CARAPI GetInstalledCapsules(
            /* [in] */ Int32 flags,
            /* [out] */ IObjectContainer** caps);

        CARAPI GetInstalledApplications(
            /* [in] */ Int32 flags,
            /* [out] */ IObjectContainer** apps);

        CARAPI ResolveActivity(
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 flags,
            /* [out] */ IResolveInfo** resolveInfo);

        CARAPI QueryIntentActivities(
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 flags,
            /* [out] */ IObjectContainer** infos);

        CARAPI QueryIntentActivityOptions(
            /* [in] */ IComponentName* caller,
            /* [in] */ ArrayOf<IIntent*>* specifics,
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 flags,
            /* [out] */ IObjectContainer** infos);

        CARAPI QueryBroadcastReceivers(
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 flags,
            /* [out] */ IObjectContainer** infos);

        CARAPI ResolveService(
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 flags,
            /* [out] */ IResolveInfo** resolveInfo);

        CARAPI QueryIntentServices(
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 flags,
            /* [out] */ IObjectContainer** infos);

        CARAPI ResolveContentProvider(
            /* [in] */ const String& name,
            /* [in] */ Int32 flags,
            /* [out] */ IContentProviderInfo** info);

        CARAPI QueryContentProviders(
            /* [in] */ const String& processName,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 flags,
            /* [out] */ IObjectContainer** providers);

        CARAPI GetInstrumentationInfo(
            /* [in] */ IComponentName* className,
            /* [in] */ Int32 flags,
            /* [out] */ IInstrumentationInfo** info);

        CARAPI QueryInstrumentation(
            /* [in] */ const String& targetCapsule,
            /* [in] */ Int32 flags,
            /* [out] */ IObjectContainer** infos);

        CARAPI GetDrawable(
            /* [in] */ const String& capsuleName,
            /* [in] */ Int32 resid,
            /* [in] */ IApplicationInfo* appInfo,
            /* [out] */ IDrawable** drawable);

        CARAPI GetActivityIcon(
            /* [in] */ IComponentName* activityName,
            /* [out] */ IDrawable** icon);

        CARAPI GetActivityIconEx(
            /* [in] */ IIntent* intent,
            /* [out] */ IDrawable** icon);

        CARAPI GetDefaultActivityIcon(
            /* [out] */ IDrawable** icon);

        CARAPI GetApplicationIcon(
            /* [in] */ IApplicationInfo* info,
            /* [out] */ IDrawable** icon);

        CARAPI GetApplicationIconEx(
            /* [in] */ const String& capsuleName,
            /* [out] */ IDrawable** icon);

        CARAPI GetActivityLogo(
            /* [in] */ IComponentName* activityName,
            /* [out] */ IDrawable** logo);

        CARAPI GetActivityLogoEx(
            /* [in] */ IIntent* intent,
            /* [out] */ IDrawable** logo);

        CARAPI GetApplicationLogo(
            /* [in] */ IApplicationInfo* info,
            /* [out] */ IDrawable** logo);

        CARAPI GetApplicationLogoEx(
            /* [in] */ const String& capsuleName,
            /* [out] */ IDrawable** logo);

        CARAPI GetResourcesForActivity(
            /* [in] */ IComponentName* activityName,
            /* [out] */ IResources** res);

        CARAPI GetResourcesForApplication(
            /* [in] */ IApplicationInfo* app,
            /* [out] */ IResources** res);

        CARAPI GetResourcesForApplicationEx(
            /* [in] */ const String& appCapsuleName,
            /* [out] */ IResources** res);

        CARAPI IsSafeMode(
            /* [out] */ Boolean* isSafeMode);

        /* package */ static CARAPI_(void) ConfigurationChanged();

        /* package */ static CARAPI_(void) HandleCapsuleBroadcast(
            /* [in] */ Int32 cmd,
            /* [in] */ ArrayOf<String>* capList,
            /* [in] */ Boolean hasCapInfo);

        CARAPI GetText(
            /* [in] */ const String& capsuleName,
            /* [in] */ Int32 resid,
            /* [in] */ IApplicationInfo* appInfo,
            /* [out] */ ICharSequence** text);

        CARAPI GetXml(
            /* [in] */ const String& capsuleName,
            /* [in] */ Int32 resid,
            /* [in] */ IApplicationInfo* appInfo,
            /* [out] */ IXmlResourceParser** parser);

        CARAPI GetApplicationLabel(
            /* [in] */ IApplicationInfo* info,
            /* [out] */ ICharSequence** label);

        CARAPI InstallCapsule(
            /* [in] */ IUri* capsuleURI,
            /* [in] */ ICapsuleInstallObserver* observer,
            /* [in] */ Int32 flags,
            /* [in] */ const String& installerCapsuleName);

        CARAPI MoveCapsule(
            /* [in] */ const String& capsuleName,
            /* [in] */ ICapsuleMoveObserver* observer,
            /* [in] */ Int32 flags);

        CARAPI GetInstallerCapsuleName(
            /* [in] */ const String& capsuleName,
            /* [out] */ String* name);

        CARAPI DeleteCapsule(
            /* [in] */ const String& capsuleName,
            /* [in] */ ICapsuleDeleteObserver* observer,
            /* [in] */ Int32 flags);

        CARAPI ClearApplicationUserData(
            /* [in] */ const String& capsuleName,
            /* [in] */ ICapsuleDataObserver* observer);

        CARAPI DeleteApplicationCacheFiles(
            /* [in] */ const String& capsuleName,
            /* [in] */ ICapsuleDataObserver* observer);

        CARAPI FreeStorageAndNotify(
            /* [in] */ Int64 freeStorageSize,
            /* [in] */ ICapsuleDataObserver* observer);

        CARAPI FreeStorage(
            /* [in] */ Int64 freeStorageSize,
            /* [in] */ IIntentSender* pi);

        CARAPI GetCapsuleSizeInfo(
            /* [in] */ const String& capsuleName,
            /* [in, out] */ ICapsuleStatsObserver* observer);

        CARAPI AddCapsuleToPreferred(
            /* [in] */ const String& capsuleName);

        CARAPI RemoveCapsuleFromPreferred(
            /* [in] */ const String& capsuleName);

        CARAPI GetPreferredCapsules(
            /* [in] */ Int32 flags,
            /* [out] */ IObjectContainer** infos);

        CARAPI AddPreferredActivity(
            /* [in] */ IIntentFilter* filter,
            /* [in] */ Int32 match,
            /* [in] */ const ArrayOf<IComponentName*>& set,
            /* [in] */ IComponentName* activity);

        CARAPI ReplacePreferredActivity(
            /* [in] */ IIntentFilter* filter,
            /* [in] */ Int32 match,
            /* [in] */ const ArrayOf<IComponentName*>& set,
            /* [in] */ IComponentName* activity);

        CARAPI ClearCapsulePreferredActivities(
            /* [in] */ const String& capsuleName);

        CARAPI GetPreferredActivities(
            /* [in] */ IObjectContainer* outFilters,
            /* [in] */ IObjectContainer* outActivities,
            /* [in] */ const String& capsuleName,
            /* [out] */ Int32* count);

        CARAPI SetComponentEnabledSetting(
            /* [in] */ IComponentName* componentName,
            /* [in] */ Int32 newState,
            /* [in] */ Int32 flags);

        CARAPI GetComponentEnabledSetting(
            /* [in] */ IComponentName* componentName,
            /* [out] */ Int32* setting);

        CARAPI SetApplicationEnabledSetting(
            /* [in] */ const String& capsuleName,
            /* [in] */ Int32 newState,
            /* [in] */ Int32 flags);

        CARAPI GetApplicationEnabledSetting(
            /* [in] */ const String& capsuleName,
            /* [out] */ Int32* setting);

        CARAPI GetCapsuleArchiveInfo(
            /* [in] */ const String& archiveFilePath,
            /* [in] */ Int32 flags,
            /* [out] */ ICapsuleInfo** info);

    private:
        CARAPI_(AutoPtr<IDrawable>) GetCachedIcon(
            /* [in] */ ResourceName* name);

        CARAPI_(void) PutCachedIcon(
            /* [in] */ ResourceName* name,
            /* [in] */ IDrawable* dr);

        CARAPI_(AutoPtr<ICharSequence>) GetCachedString(
            /* [in] */ ResourceName* name);

        CARAPI_(void) PutCachedString(
            /* [in] */ ResourceName* name,
            /* [in] */ ICharSequence* cs);

    public:
        Int32 mCachedSafeMode;

    private:
        CContextImpl* mContext;
        AutoPtr<ICapsuleManager> mPM;

        // private static final Object sSync = new Object();
        // private static HashMap<ResourceName, WeakReference<Drawable> > sIconCache
        //         = new HashMap<ResourceName, WeakReference<Drawable> >();
        // private static HashMap<ResourceName, WeakReference<CharSequence> > sStringCache
        //         = new HashMap<ResourceName, WeakReference<CharSequence> >();
    };

public:
    CContextImpl();

    ~CContextImpl();

    CARAPI constructor();

    //Only For Get System Context.
    CARAPI constructor(
        /* [in] */ Boolean isSysCtx);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ IBinder* binder);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

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

    CARAPI StartActivity(
        /* [in] */ IIntent* intent);

    CARAPI StartIntentSender(
        /* [in] */ IIntentSender* intent,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ Int32 extraFlags);

    CARAPI SendBroadcast(
        /* [in] */ IIntent* pIntent);

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

    CARAPI CheckPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [out] */ Int32* result);

    CARAPI CheckUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ const String& readPermission,
        /* [in] */ const String& writePermission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* result);

    CARAPI CheckUriPermissionEx(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* result);

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

    CARAPI GetApplicationApartment(
        /* [out] */ IApplicationApartment** apartment);

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
    CARAPI GetDataDirFile(
        /* [out] */ IFile** dirFile);

    static CARAPI_(void) SetFilePermissionsFromMode(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 extraPermissions);

    CARAPI MakeFilename(
        /* [in] */ IFile* base,
        /* [in] */ const String& name,
        /* [out] */ IFile** file);

    CARAPI Enforce(
        /* [in] */ String permission,
        /* [in] */ Int32 resultOfCheck,
        /* [in] */ Boolean selfToo,
        /* [in] */ Int32 uid,
        /* [in] */ String message);

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
    AutoPtr<ILocalCapsuleManager> mCapsuleManager;
    AutoPtr<IContext> mReceiverRestrictedContext;
    AutoPtr<ILayoutInflater> mLayoutInflater;

    Mutex mSync;

    AutoPtr<IFile> mFilesDir;
    AutoPtr<IFile> mCacheDir;
    AutoPtr<IFile> mExternalFilesDir;
};

#endif // __CCONTEXTIMPL_H__
