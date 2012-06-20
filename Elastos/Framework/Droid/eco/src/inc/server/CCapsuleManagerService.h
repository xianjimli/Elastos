
#ifndef __CCAPSULEMANAGER_H__
#define __CCAPSULEMANAGER_H__

#include "_CCapsuleManagerService.h"

#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>
#include <elastos/AutoFree.h>
#include <elastos/HashMap.h>
#include <elastos/HashSet.h>

#include "capsule/CapsuleParser.h"
#include "capsule/CActivityInfo.h"
#include "server/IntentResolver.h"
#include "server/Installer.h"
#include "content/CResolveInfo.h"
#include "content/CComponentName.h"
#include "content/Signature.h"
#include "os/Message.h"
#include "os/Handler.h"
#include "os/Process.h"
#include "utils/AutoString.h"
#include "utils/AutoStringArray.h"
#include "utils/ElRefBase.h"
#include <elastos/Mutex.h>

using namespace Elastos::System::Threading;

CarClass(CCapsuleManagerService)
{
public:
    static const String TAG;
    static const Boolean DEBUG_SETTINGS = FALSE;
    static const Boolean DEBUG_PREFERRED = FALSE;
    static const Boolean DEBUG_UPGRADE = FALSE;
    static const Boolean DEBUG_INSTALL = FALSE;

public:
    class DefaultContainerConnection : public ElRefBase, public IServiceConnection
    {
    public:
        DefaultContainerConnection(
            /* [in] */ CCapsuleManagerService* owner)
            : mOwner(owner)
            {}

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnServiceConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI OnServiceDisconnected(
            /* [in] */ IComponentName* name);

    private:
        AutoPtr<CCapsuleManagerService> mOwner;
    };

    class ActivityIntentResolver :
        public IntentResolver<CapsuleParser::ActivityIntentInfo,
                              AutoPtr<CResolveInfo> >
    {
    public:
        typedef IntentResolver<CapsuleParser::ActivityIntentInfo, \
                AutoPtr<CResolveInfo> >     Super;

    public:
        ActivityIntentResolver();

        ~ActivityIntentResolver();

        CARAPI_(List<AutoPtr<CResolveInfo>*>*) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ String resolvedType,
            /* [in] */ Int32 flags);

        CARAPI_(List<AutoPtr<CResolveInfo>*>*) QueryIntentForCapsule(
            /* [in] */ IIntent* intent,
            /* [in] */ String resolvedType,
            /* [in] */ Int32 flags,
            /* [in] */ List<CapsuleParser::Activity*>* capsuleActivities);

        CARAPI_(void) AddActivity(
            /* [in] */ CapsuleParser::Activity* activity,
            /* [in] */ String type);

        CARAPI_(void) RemoveActivity(
            /* [in] */ CapsuleParser::Activity* activity,
            /* [in] */ String type);

        CARAPI NewResult(
            /* [in] */ CapsuleParser::ActivityIntentInfo* info,
            /* [in] */ Int32 match,
            /* [out] */ AutoPtr<CResolveInfo>** ret);

        CARAPI_(void) SortResults(
            /* [in] */ List<AutoPtr<CResolveInfo>*>* results);

    public:
        // Keys are String (activity class name), values are Activity.
        HashMap<AutoPtr<IComponentName>, CapsuleParser::Activity*> mActivities;

        Int32 mFlags;
    };

    class ServiceIntentResolver :
        public IntentResolver<CapsuleParser::ServiceIntentInfo,
                              AutoPtr<CResolveInfo> >
    {
    public:
        typedef IntentResolver<CapsuleParser::ServiceIntentInfo, \
                AutoPtr<CResolveInfo> >     Super;

    public:
        ServiceIntentResolver();

        ~ServiceIntentResolver();

        CARAPI_(List<AutoPtr<CResolveInfo>*>*) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ String resolvedType,
            /* [in] */ Int32 flags);

        CARAPI_(List<AutoPtr<CResolveInfo>*>*) QueryIntentForCapsule(
            /* [in] */ IIntent* intent,
            /* [in] */ String resolvedType,
            /* [in] */ Int32 flags,
            /* [in] */ List<CapsuleParser::Service*>* capsuleServices);

        CARAPI_(void) AddService(
            /* [in] */ CapsuleParser::Service* service);

        CARAPI_(void) RemoveService(
            /* [in] */ CapsuleParser::Service* service);

        CARAPI NewResult(
            /* [in] */ CapsuleParser::ServiceIntentInfo* info,
            /* [in] */ Int32 match,
            /* [out] */ AutoPtr<CResolveInfo>** ret);

        CARAPI_(void) SortResults(
            /* [in] */ List<AutoPtr<CResolveInfo>*>* results);

    public:
        // Keys are String (activity class name), values are Activity.
        HashMap<AutoPtr<IComponentName>, CapsuleParser::Service*> mServices;
        Int32 mFlags;
    };

    class CapsuleSettingBase;

    class BasePermission
    {
    public:
        BasePermission(
            /* [in] */ String _name,
            /* [in] */ String _sourcePackage,
            /* [in] */ int _type);

        ~BasePermission();

        CARAPI_(String) GetDescription();

    public:
        static const Int32 TYPE_NORMAL = 0;
        static const Int32 TYPE_BUILTIN = 1;
        static const Int32 TYPE_DYNAMIC = 2;

        AutoString mName;
        AutoString mSourceCapsule;
        CapsuleSettingBase* mCapsuleSetting;
        Int32 mType;
        Int32 mProtectionLevel;
        CapsuleParser::Permission* mPerm;
        AutoPtr<IPermissionInfo> mPendingInfo;
        Int32 mUid;
        AutoFree<ArrayOf<Int32> > mGids;
    };
//todo: Signature should be declared as CAR Object.
    class CapsuleSignatures
    {
    public:
        CapsuleSignatures(
            /* [in] */ const CapsuleSignatures& orig);

        CapsuleSignatures(
            /* [in] */ const ArrayOf<Signature>& sigs);

        CapsuleSignatures();

        ~CapsuleSignatures();

    private:
        AutoFree<ArrayOf<Signature> > mSignatures;
    };

    class PreferredActivity
        : public ElRefBase
        , public IntentFilter
        , public IIntentFilter
    {
    public:
        PreferredActivity()
            : mMatch(0)
        {}

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [out] */ InterfaceID* pIID);

        CARAPI SetPriority(
            /* [in] */ Int32 priority);

        CARAPI GetPriority(
            /* [out] */ Int32* priority);

        CARAPI AddAction(
            /* [in] */ String action);

        CARAPI CountActions(
            /* [out] */ Int32 *count);

        CARAPI GetAction(
            /* [in] */ Int32 index,
            /* [out] */ String *action);

        CARAPI AddDataType(
            /* [in] */ String type);

        CARAPI AddDataScheme(
            /* [in] */ String scheme);

        CARAPI AddDataAuthority(
            /* [in] */ String host,
            /* [in] */ String port);

        CARAPI AddDataPath(
            /* [in] */ String path,
            /* [in] */ Int32 type);

        CARAPI AddCategory(
            /* [in] */ String category);

        CARAPI HasCategory(
            /* [in] */ String category,
            /* [out] */ Boolean* hasCategory);

        CARAPI Match(
            /* [in] */ String action,
            /* [in] */ String type,
            /* [in] */ String scheme,
            /* [in] */ IUri* data,
            /* [in] */ ArrayOf<String>* categories,
            /* [in] */ String logTag,
            /* [out] */ Int32* result);

        CARAPI ReadFromParcel(
            /* [in] */ IParcel *source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel *dest);

    public:
        Boolean SameSet(
            /* [in] */ List<AutoPtr<CResolveInfo>*>* query,
            /* [in] */ Int32 priority) { return FALSE; }

    public:
        Int32 mMatch;
        AutoStringArray mSetPackages;
        AutoStringArray mSetClasses;
        AutoStringArray mSetComponents;
        AutoPtr<IComponentName> mActivity;
        AutoString mShortActivity;
        AutoString mParseError;
    };

    class MyIntentResolver
        : public IntentResolver<PreferredActivity, PreferredActivity>
    {
    protected:
        virtual CARAPI_(String) CapsuleForFilter(
            /* [in] */ PreferredActivity* filter) { return NULL; }

        virtual CARAPI DumpFilter(
            /* [in] */ IPrintWriter* out,
            /* [in] */ String prefix,
            /* [in] */ PreferredActivity* filter) { return E_NOT_IMPLEMENTED; }
    };

    class GrantedPermissions
    {
    public:
        GrantedPermissions(
            /* [in] */ Int32 capFlags);

        GrantedPermissions(
            /* [in] */ const GrantedPermissions& base);

        virtual ~GrantedPermissions();

        CARAPI_(void) SetFlags(
            /* [in] */ Int32 capFlags);

    public:
        Int32 mCapFlags;

        HashSet<String> mGrantedPermissions;
        AutoFree<ArrayOf<Int32> > mGids;
    };

    /**
     * Settings base class for pending and resolved classes.
     */
    class CapsuleSettingBase : public GrantedPermissions
    {
    public:
        CapsuleSettingBase(
            /* [in] */ String name,
            /* [in] */ String realName,
            /* [in] */ IFile* codePath,
            /* [in] */ IFile* resourcePath,
            /* [in] */ String nativeLibraryPathString,
            /* [in] */ Int32 pVersionCode,
            /* [in] */ Int32 capFlags);

        /**
         * New instance of PackageSetting with one-level-deep cloning.
         */
        CapsuleSettingBase(
            /* [in] */ const CapsuleSettingBase& base);

        ~CapsuleSettingBase();

        CARAPI_(void) Init(
            /* [in] */ IFile* codePath,
            /* [in] */ IFile* resourcePath,
            /* [in] */ String nativeLibraryPathString,
            /* [in] */ Int32 pVersionCode);

        CARAPI_(void) SetInstallerCapsuleName(
            /* [in] */ String packageName);

        CARAPI_(String) GetInstallerPackageName();

        CARAPI_(void) SetInstallStatus(
            /* [in] */ Int32 newStatus);

        CARAPI_(String) GetInstallStatus();

        CARAPI_(void) SetTimeStamp(
            /* [in] */ Int64 newStamp);

        /**
         * Make a shallow copy of this package settings.
         */
        CARAPI_(void) CopyFrom(
            /* [in] */ CapsuleSettingBase& base);

        CARAPI_(Boolean) EnableComponentLP(
            /* [in] */ String componentClassName);

        CARAPI_(Boolean) DisableComponentLP(
            /* [in] */ String componentClassName);

        CARAPI_(Boolean) RestoreComponentLP(
            /* [in] */ String componentClassName);

        CARAPI_(Int32) CurrentEnabledStateLP(
            /* [in] */ String componentName);

    public:
        AutoString mName;
        AutoString mRealName;
        AutoPtr<IFile> mCodePath;
        AutoString mCodePathString;
        AutoPtr<IFile> mResourcePath;
        AutoString mResourcePathString;
        AutoString mNativeLibraryPathString;
        Int64 mTimeStamp;
        Int64 mFirstInstallTime;
        Int64 mLastUpdateTime;
        Int32 mVersionCode;

        Boolean mUidError;

//	        PackageSignatures signatures = new PackageSignatures();

        Boolean mPermissionsFixed;
        Boolean mHaveGids;

        /* Explicitly disabled components */
        HashSet<String> mDisabledComponents;
        /* Explicitly enabled components */
        HashSet<String> mEnabledComponents;
        Int32 mEnabled;
        Int32 mInstallStatus;

        CapsuleSettingBase* mOrigCapsule;

        /* package name of the app that installed this package */
        AutoString mInstallerPackageName;
    };

    class SharedUserSetting;

    /**
     * Settings data for a particular package we know about.
     */
    class CapsuleSetting : public CapsuleSettingBase
    {
    public:
        CapsuleSetting(
            /* [in] */ String name,
            /* [in] */ String realName,
            /* [in] */ IFile* codePath,
            /* [in] */ IFile* resourcePath,
            /* [in] */ String nativeLibraryPathString,
            /* [in] */ Int32 pVersionCode,
            /* [in] */ Int32 capFlags);

        /**
         * New instance of PackageSetting replicating the original settings.
         * Note that it keeps the same PackageParser.Package instance.
         */
        CapsuleSetting(
            /* [in] */ const CapsuleSetting& orig);

        ~CapsuleSetting();

        CARAPI_(String) GetDescription();

        struct HashKey
        {
            size_t operator()(const CapsuleSetting* s) const
            {
                return (size_t)s->GetHashCode();
            }
        };

        CARAPI_(Int32) GetHashCode() const
        {
            return (Int32)this;
        }

    public:
        Int32 mUserId;
        CapsuleParser::Capsule* mCap;
        SharedUserSetting* mSharedUser;
    };

    /**
     * Settings data for a particular shared user ID we know about.
     */
    class SharedUserSetting : public GrantedPermissions
    {
    public:
        SharedUserSetting(
            /* [in] */ String _name,
            /* [in] */ Int32 _pkgFlags);

        ~SharedUserSetting();

        CARAPI_(String) GetDescription();

    public:
        AutoString mName;
        Int32 mUserId;
        HashSet<CapsuleSetting*, CapsuleSetting::HashKey> mCapsules;
        CapsuleSignatures* mSignatures;
    };

private:
    class HandlerParams;

    class CapsuleHandler : public Handler
    {
    public:
        CapsuleHandler(
            /* [in] */ CCapsuleManagerService* owner)
            : mBound(FALSE)
            , mOwner(owner)
            , mRemovedMsgCode(-1) {}

        CARAPI_(Boolean) ConnectToService();

        CARAPI_(void) DisconnectService();

        CARAPI_(void) HandleMessage(
            /* [in] */ Message* msg);

        CARAPI_(void) DoHandleMessage(
            /* [in] */ Message* msg);

        // send message to this handler.
        CARAPI SendMessage(
            /* [in] */ Message* msg);

    private:
        CARAPI_(Boolean) IsMessageRemoved(
            /* [in] */ IParcel* params);

        CARAPI RemoveMessages(
            /* [in] */ Int32 msgCode);

    public:
        Boolean mBound;

        List<HandlerParams*> mPendingInstalls;

        AutoPtr<CCapsuleManagerService> mOwner;

        Mutex mLock;
        Int32 mRemovedMsgCode;
    };

    class HandlerParams
    {
    public:
        HandlerParams()
            : mRetry(0) {}

        ~HandlerParams() {}

        CARAPI_(void) StartCopy();

        CARAPI_(void) ServiceError();

        virtual CARAPI_(void) HandleStartCopy() = 0;

        virtual CARAPI_(void) HandleServiceError() = 0;

        virtual CARAPI_(void) HandleReturnCode() = 0;

    public:
        static const Int32 MAX_RETRIES = 4;

        Int32 mRetry;
    };

    class InstallArgs
    {
    public:
        InstallArgs() {}

        InstallArgs(
            /* [in] */ IUri* capsuleURI,
            /* [in] */ ICapsuleInstallObserver* observer,
            /* [in] */ Int32 flags,
            /* [in] */ String installerCapsuleName)
            : mObserver(observer)
            , mFlags(flags)
            , mCapsuleURI(capsuleURI)
            , mInstallerCapsuleName(String::Duplicate(installerCapsuleName))
        {}

        virtual CARAPI_(void) CreateCopyFile() = 0;

        virtual CARAPI_(Int32) CopyApk(
            /* [in] */ IMediaContainerService* imcs,
            /* [in] */ Boolean temp) = 0;

        virtual CARAPI_(Int32) DoPreInstall(
            /* [in] */ Int32 status) = 0;

        virtual CARAPI_(Boolean) DoRename(
            /* [in] */ Int32 status,
            /* [in] */ String capName,
            /* [in] */ String oldCodePath) = 0;

        virtual CARAPI_(Int32) DoPostInstall(
            /* [in] */ Int32 status) = 0;

        virtual CARAPI_(String) GetCodePath() = 0;

        virtual CARAPI_(String) GetResourcePath() = 0;

        virtual CARAPI_(String) GetNativeLibraryPath() = 0;

        // Need installer lock especially for dex file removal.
        virtual CARAPI_(void) CleanUpResourcesLI() = 0;

        virtual CARAPI_(Boolean) DoPostDeleteLI(
            /* [in] */ Boolean del) = 0;

        virtual CARAPI_(Boolean) CheckFreeStorage(
            /* [in] */ IMediaContainerService* imcs) = 0;

    public:
        AutoPtr<ICapsuleInstallObserver> mObserver;
        // Always refers to PackageManager flags only
        Int32 mFlags;
        AutoPtr<IUri> mCapsuleURI;
        AutoString mInstallerCapsuleName;
    };

    class InstallParams;

    class SdInstallArgs : public InstallArgs {
    public:
        SdInstallArgs(
            /* [in] */ InstallParams* params) {}

        SdInstallArgs(
            /* [in] */ String fullCodePath,
            /* [in] */ String fullResourcePath,
            /* [in] */ String nativeLibraryPath) {}

        SdInstallArgs(
            /* [in] */ String cid) {}

        SdInstallArgs(
            /* [in] */ IUri* capsuleURI,
            /* [in] */ String cid) {}

        virtual CARAPI_(void) CreateCopyFile() {}

        virtual CARAPI_(Int32) CopyApk(
            /* [in] */ IMediaContainerService* imcs,
            /* [in] */ Boolean temp) { return -1; }

        virtual CARAPI_(Int32) DoPreInstall(
            /* [in] */ Int32 status) { return -1; }

        virtual CARAPI_(Boolean) DoRename(
            /* [in] */ Int32 status,
            /* [in] */ String capName,
            /* [in] */ String oldCodePath) { return FALSE; }

        virtual CARAPI_(Int32) DoPostInstall(
            /* [in] */ Int32 status) { return -1; }

        virtual CARAPI_(String) GetCodePath() { return String(NULL); }

        virtual CARAPI_(String) GetResourcePath() { return String(NULL); }

        virtual CARAPI_(String) GetNativeLibraryPath() { return String(NULL); }

        // Need installer lock especially for dex file removal.
        virtual CARAPI_(void) CleanUpResourcesLI() {}

        virtual CARAPI_(Boolean) DoPostDeleteLI(
            /* [in] */ Boolean del) { return FALSE; }

        virtual CARAPI_(Boolean) CheckFreeStorage(
            /* [in] */ IMediaContainerService* imcs) { return FALSE; }

        CARAPI GetCapsuleName(
            /* [out] */ String* name) { return E_NOT_IMPLEMENTED; }

    public:
        static const String RES_FILE_NAME;

        AutoString mCid;
        AutoString mCapsulePath;
        AutoString mLibraryPath;
    };

    class InstallParams : public HandlerParams
    {
    public:
        InstallParams(
            /* [in] */ IUri* capsuleURI,
            /* [in] */ ICapsuleInstallObserver* observer,
            /* [in] */ Int32 flags,
            /* [in] */ String installerCapsuleName)
            : mObserver(observer)
            , mFlags(flags)
            , mCapsuleURI(capsuleURI)
            , mInstallerCapsuleName(String::Duplicate(installerCapsuleName))
            , mArgs(NULL)
            , mRet(0) {}

        CARAPI_(Int32) InstallLocationPolicy(
            /* [in] */ ICapsuleInfoLite* capLite,
            /* [in] */ Int32 flags);

        /*
         * Invoke remote method to get package information and install
         * location values. Override install location based on default
         * policy if needed and then create install arguments based
         * on the install location.
         */
        CARAPI_(void) HandleStartCopy();

        CARAPI_(void) HandleReturnCode();

        CARAPI_(void) HandleServiceError();

    public:
        AutoPtr<ICapsuleInstallObserver> mObserver;
        Int32 mFlags;
        AutoPtr<IUri> mCapsuleURI;
        AutoString mInstallerCapsuleName;
        InstallArgs* mArgs;
        Int32 mRet;
    };

    class MoveParams : public HandlerParams
    {
    public:
        MoveParams(
            /* [in] */ InstallArgs* srcArgs,
            /* [in] */ ICapsuleMoveObserver* observer,
            /* [in] */ Int32 flags,
            /* [in] */ String capsuleName,
            /* [in] */ String dataDir)
            : mFlags(0)
            , mSrcArgs(NULL)
            , mTargetArgs(NULL)
            , mRet(NULL)
        {}

        virtual CARAPI_(void) HandleStartCopy() {}

        virtual CARAPI_(void) HandleServiceError() {}

        virtual CARAPI_(void) HandleReturnCode() {}

    public:
        AutoPtr<ICapsuleMoveObserver> mObserver;
        Int32 mFlags;
        AutoString mCapsuleName;
        InstallArgs* mSrcArgs;
        InstallArgs* mTargetArgs;
        Int32 mRet;
    };

    class CapsuleRemovedInfo {
    public:
        CapsuleRemovedInfo()
            : mUid(-1)
            , mRemovedUid(-1)
            , mIsRemovedCapsuleSystemUpdate(FALSE)
            , mArgs(NULL) {}

        CARAPI_(void) SendBroadcast(
            /* [in] */ Boolean fullRemove,
            /* [in] */ Boolean replacing);

    public:
        AutoString mRemovedCapsule;
        Int32 mUid;
        Int32 mRemovedUid;
        Boolean mIsRemovedCapsuleSystemUpdate;
        // Clean up resources deleted capsules.
        InstallArgs* mArgs;
    };

    class CapsuleInstalledInfo {
    public:
        CapsuleInstalledInfo()
            : mUid(0)
            , mCap(NULL)
            , mReturnCode(0)
            , mRemovedInfo(NULL) {}

    public:
        AutoString mName;
        Int32 mUid;
        CapsuleParser::Capsule* mCap;
        Int32 mReturnCode;
        CapsuleRemovedInfo* mRemovedInfo;
    };

    // Recordkeeping of restore-after-install operations that are currently in flight
    // between the Package Manager and the Backup Manager
    class PostInstallData {
    public:
        PostInstallData(
            /* [in] */ InstallArgs* a,
            /* [in] */ CapsuleInstalledInfo* r)
            : mArgs(a)
            , mRes(r) {}

    public:
        InstallArgs* mArgs;
        CapsuleInstalledInfo* mRes;
    };

    /**
     * Holds information about dynamic settings.
     */
    class Settings
    {
    public:
        Settings();

        ~Settings();

        CARAPI_(CapsuleSetting*) EnableSystemCapsuleLP(
            /* [in] */ String name) { return NULL; }

        CARAPI_(SharedUserSetting*) AddSharedUserLP(
            /* [in] */ String name,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 capFlags) { return NULL; }

        /*
         * Update the shared user setting when a package using
         * specifying the shared user id is removed. The gids
         * associated with each permission of the deleted package
         * are removed from the shared user's gid list only if its
         * not in use by other permissions of packages in the
         * shared user setting.
         */
        CARAPI UpdateSharedUserPermsLP(
            /* [in] */ CapsuleSetting* deletedPs,
            /* [in] */ const ArrayOf<Int32>& globalGids)
        { return E_NOT_IMPLEMENTED; }

        CARAPI_(Int32) RemoveCapsuleLP(
            /* [in] */ String name) { return -1; }

        SharedUserSetting* GetSharedUserLP(
            /* [in] */ String name,
            /* [in] */ Int32 capFlags,
            /* [in] */ Boolean create) { return NULL; }

        CARAPI_(GrantedPermissions*) GetUserIdLP(
            /* [in] */ Int32 uid) { return NULL; }

        CARAPI_(void) WriteLP() {}

        CARAPI_(Boolean) ReadLP() { return FALSE; }

        CARAPI_(CapsuleSetting*) GetDisabledSystemCap(
            /* [in] */ String name) { return NULL; }

        CARAPI_(Boolean) IsEnabledLP(
            /* [in] */ IComponentInfo* componentInfo,
            /* [in] */ Int32 flags) { return FALSE; }

    public:
        AutoPtr<IFile> mSettingsFilename;
        AutoPtr<IFile> mBackupSettingsFilename;
        AutoPtr<IFile> mPackageListFilename;
        HashMap<String, CapsuleSetting*> mCapsules;
        // List of replaced system applications
        HashMap<String, CapsuleSetting*> mDisabledSysCapsules;

        // These are the last platform API version we were using for
        // the apps installed on internal and external storage.  It is
        // used to grant newer permissions one time during a system upgrade.
        Int32 mInternalSdkPlatform;
        Int32 mExternalSdkPlatform;

        // The user's preferred activities associated with particular intent
        // filters.
        MyIntentResolver* mPreferredActivities;

        HashMap<String, SharedUserSetting*> mSharedUsers;
//	        private final ArrayList<Object> mUserIds = new ArrayList<Object>();
//	        private final SparseArray<Object> mOtherUserIds =
//	                new SparseArray<Object>();

        // For reading/writing settings file.
//	        private final ArrayList<Signature> mPastSignatures =
//	                new ArrayList<Signature>();

        // Mapping from permission names to info about them.
        HashMap<String, BasePermission*> mPermissions;

        // Mapping from permission tree names to info about them.
        HashMap<String, BasePermission*> mPermissionTrees;

        // Packages that have been uninstalled and still need their external
        // storage data deleted.
        List<String> mCapsulesToBeCleaned;

        // Packages that have been renamed since they were first installed.
        // Keys are the new names of the packages, values are the original
        // names.  The packages appear everwhere else under their original
        // names.
        HashMap<String, String> mRenamedPackages;

//	        private final StringBuilder mReadMessages = new StringBuilder();

//	        private final ArrayList<PendingPackage> mPendingPackages
//	                = new ArrayList<PendingPackage>();
    };

public:
    CCapsuleManagerService();

    ~CCapsuleManagerService();

    CARAPI GetCapsuleInfo(
        /* [in] */ String capsuleName,
        /* [in] */ Int32 flags,
        /* [out] */ ICapsuleInfo** capInfo);

    CARAPI GetCapsuleUid(
        /* [in] */ String capsuleName,
        /* [out] */ Int32* uid);

    CARAPI GetCapsuleGids(
        /* [in] */ String capsuleName,
        /* [out, callee] */ ArrayOf<Int32>** gids);

    CARAPI CurrentToCanonicalCapsuleNames(
        /* [in] */ const ArrayOf<String>& names,
        /* [out, callee] */ ArrayOf<String>** cnames);

    CARAPI CanonicalToCurrentCapsuleNames(
        /* [in] */ const ArrayOf<String>& names,
        /* [out, callee] */ ArrayOf<String>** cnames);

    CARAPI GetPermissionInfo(
        /* [in] */ String name,
        /* [in] */ Int32 flags,
        /* [out] */ IPermissionInfo** info);

    CARAPI QueryPermissionsByGroup(
        /* [in] */ String group,
        /* [in] */ Int32 flags,
        /* [out, callee] */ IObjectContainer** infos);

    CARAPI GetPermissionGroupInfo(
        /* [in] */ String name,
        /* [in] */ Int32 flags,
        /* [out] */ IPermissionGroupInfo** info);

    CARAPI GetAllPermissionGroups(
        /* [in] */ Int32 flags,
        /* [out, callee] */ IObjectContainer** infos);

    CARAPI GetApplicationInfo(
        /* [in] */ String capsuleName,
        /* [in] */ Int32 flags,
        /* [out] */ IApplicationInfo** appInfo);

    CARAPI GetActivityInfo(
        /* [in] */ IComponentName* component,
        /* [in] */ Int32 flags,
        /* [out] */ IActivityInfo** info);

    CARAPI GetReceiverInfo(
        /* [in] */ IComponentName* component,
        /* [in] */ Int32 flags,
        /* [out] */ IActivityInfo** info);

    CARAPI GetServiceInfo(
        /* [in] */ IComponentName* component,
        /* [in] */ Int32 flags,
        /* [out] */ IServiceInfo** info);

    CARAPI GetContentProviderInfo(
        /* [in] */ IComponentName* component,
        /* [in] */ Int32 flags,
        /* [out] */ IContentProviderInfo** info);

    CARAPI CheckPermission(
        /* [in] */ String permName,
        /* [in] */ String capName,
        /* [out] */ Int32* perm);

    CARAPI CheckUidPermission(
        /* [in] */ String permName,
        /* [in] */ Int32 uid,
        /* [out] */ Int32* perm);

    CARAPI AddPermission(
        /* [in] */ IPermissionInfo* info,
        /* [out] */ Boolean* isAdded);

    CARAPI RemovePermission(
        /* [in] */ String name);

    CARAPI IsProtectedBroadcast(
        /* [in] */ String actionName,
        /* [out] */ Boolean* result);

    CARAPI CheckSignatures(
        /* [in] */ String cap1,
        /* [in] */ String cap2,
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
        /* [in] */ String sharedUserName,
        /* [out] */ Int32* uid);

    CARAPI ResolveIntent(
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 flags,
        /* [out] */ IResolveInfo** resolveInfo);

    CARAPI QueryIntentActivities(
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 flags,
        /* [out, callee] */ IObjectContainer** infos);

    CARAPI_(List<AutoPtr<CResolveInfo>*>*) QueryIntentActivities(
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ int flags);

    CARAPI QueryIntentActivityOptions(
        /* [in] */ IComponentName* caller,
        /* [in] */ const ArrayOf<IIntent*>& specifics,
        /* [in] */ const ArrayOf<String>& specificTypes,
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** infos);

    CARAPI QueryIntentReceivers(
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** receivers);

    CARAPI ChooseBestActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ List<AutoPtr<CResolveInfo>*>* query,
        /* [out] */ IResolveInfo** resolveInfo);

    CARAPI_(CResolveInfo*) FindPreferredActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ List<AutoPtr<CResolveInfo>*>* query,
        /* [in] */ Int32 priority);

    CARAPI ResolveService(
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 flags,
        /* [out] */ IResolveInfo** resolveInfo);

    CARAPI QueryIntentServices(
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** services);

    CARAPI GetInstalledCapsules(
        /* [in] */ Int32 flags,
        /* [in] */ String lastRead,
        /* [out] */ IParceledListSlice** slice);

    CARAPI GetInstalledApplications(
        /* [in] */ Int32 flags,
        /* [in] */ String lastRead,
        /* [out] */ IParceledListSlice** slice);

    CARAPI GetPersistentApplications(
        /* [in] */ Int32 flags,
        /* [out, callee] */ IObjectContainer** infos);

    CARAPI ResolveContentProvider(
        /* [in] */ String name,
        /* [in] */ Int32 flags,
        /* [out] */ IContentProviderInfo** info);

    CARAPI QuerySyncProviders(
        /* [in, out] */ IObjectContainer* outNames,
        /* [in, out] */ IObjectContainer* outInfo);

    CARAPI QueryContentProviders(
        /* [in] */ String processName,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** providers);

    CARAPI GetInstrumentationInfo(
        /* [in] */ IComponentName * name,
        /* [in] */ Int32 flags,
        /* [out] */ IInstrumentationInfo** instInfo);

    CARAPI QueryInstrumentation(
        /* [in] */ String targetCapsule,
        /* [in] */ Int32 flags,
        /* [out, callee] */ IObjectContainer** infos);

    CARAPI InstallCapsuleEx(
        /* [in] */ String path);

    CARAPI InstallCapsule(
        /* [in] */ IUri* capsuleURI,
        /* [in] */ ICapsuleInstallObserver* observer,
        /* [in] */ Int32 flags,
        /* [in] */ String installerCapsuleName);

    CARAPI FinishCapsuleInstall(
        /* [in] */ Int32 token);

    CARAPI DeleteCapsule(
        /* [in] */ String capsuleName,
        /* [in] */ ICapsuleDeleteObserver* observer,
        /* [in] */ Int32 flags);

    CARAPI GetInstallerCapsuleName(
        /* [in] */ String capsuleName,
        /* [out] */ String* name);

    CARAPI AddCapsuleToPreferred(
        /* [in] */ String capsuleName);

    CARAPI RemoveCapsuleFromPreferred(
        /* [in] */ String capsuleName);

    CARAPI GetPreferredCapsules(
        /* [in] */ Int32 flags,
        /* [out, callee] */ IObjectContainer** infos);

    CARAPI AddPreferredActivity(
        /* [in] */ IIntentFilter* filter,
        /* [in] */ Int32 match,
        /* [in] */ const ArrayOf<IComponentName>& set,
        /* [in] */ IComponentName* activity);

    CARAPI ReplacePreferredActivity(
        /* [in] */ IIntentFilter* filter,
        /* [in] */ Int32 match,
        /* [in] */ const ArrayOf<IComponentName>& set,
        /* [in] */ IComponentName* activity);

    CARAPI ClearCapsulePreferredActivities(
        /* [in] */ String capsuleName);

    CARAPI_(Boolean) ClearCapsulePreferredActivitiesLP(
        /* [in] */ String capsuleName);

    CARAPI GetPreferredActivities(
        /* [in, out] */ IObjectContainer* outFilters,
        /* [in, out] */ IObjectContainer* outActivities,
        /* [in] */ String capsuleName,
        /* [out] */ Int32* count);

    CARAPI SetComponentEnabledSetting(
        /* [in] */ IComponentName* componentName,
        /* [in] */ Int32 newState,
        /* [in] */ Int32 flags);

    CARAPI GetComponentEnabledSetting(
        /* [in] */ IComponentName* componentName,
        /* [out] */ Int32* setting);

    CARAPI SetApplicationEnabledSetting(
        /* [in] */ String capsuleName,
        /* [in] */ Int32 newState,
        /* [in] */ Int32 flags);

    CARAPI GetApplicationEnabledSetting(
        /* [in] */ String capsuleName,
        /* [out] */ Int32* setting);

    CARAPI FreeStorageAndNotify(
        /* [in] */ Int64 freeStorageSize,
        /* [in] */ ICapsuleDataObserver* observer);

    CARAPI FreeStorage(
        /* [in] */ Int64 freeStorageSize,
        /* [in] */ IIntentSender* pi);

    CARAPI DeleteApplicationCacheFiles(
        /* [in] */ String capsuleName,
        /* [in] */ ICapsuleDataObserver* observer);

    CARAPI ClearApplicationUserData(
        /* [in] */ String capsuleName,
        /* [in] */ ICapsuleDataObserver* observer);

    CARAPI GetCapsuleSizeInfo(
        /* [in] */ String capsuleName,
        /* [in] */ ICapsuleStatsObserver* observer);

    CARAPI GetSystemSharedLibraryNames(
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI GetSystemAvailableFeatures(
        /* [out] */ IObjectContainer** infos);

    CARAPI HasSystemFeature(
        /* [in] */ String name,
        /* [out] */ Boolean* result);

    CARAPI EnterSafeMode();

    CARAPI IsSafeMode(
        /* [out] */ Boolean* isSafeMode);

    CARAPI SystemReady();

    CARAPI HasSystemUidErrors(
        /* [out] */ Boolean* result);

    CARAPI UpdateExternalMediaStatus(
        /* [in] */ Boolean mediaStatus,
        /* [in] */ Boolean reportStatus);

    CARAPI NextCapsuleToClean(
        /* [in] */ String lastCapsule,
        /* [out] */ String* nextCapsule);

    CARAPI MoveCapsule(
        /* [in] */ String capsuleName,
        /* [in] */ ICapsuleMoveObserver* observer,
        /* [in] */ Int32 flags);

    CARAPI AddPermissionAsync(
        /* [in] */ IPermissionInfo* info,
        /* [out] */ Boolean* isAdded);

    CARAPI SetInstallLocation(
        /* [in] */ Int32 loc,
        /* [out] */ Boolean* isSetted);

    CARAPI GetInstallLocation(
        /* [out] */ Int32* loc);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Boolean factoryTest);

public:
    CARAPI GenerateCapsuleInfo(
        /* [in] */ CapsuleParser::Capsule* c,
        /* [in] */ Int32 flags,
        /* [out] */ ICapsuleInfo** capInfo);

    static CARAPI GeneratePermissionInfo(
        /* [in] */ BasePermission* bp,
        /* [in] */ Int32 flags,
        /* [out] */ IPermissionInfo** info);

private:
    CARAPI SplitString(
        /* [in] */ String str,
        /* [in] */ Char8 sep,
        /* [out] */ ArrayOf<String>** outStrs);

    CARAPI ReadPermissions();

    CARAPI CheckSignaturesLP(
        /* [in] */ List<Signature*>& s1,
        /* [in] */ List<Signature*>& s2,
        /* [out] */ Int32* sig);

    CARAPI_(void) ScanDir(
        /* [in] */ String path);

    CARAPI_(CapsuleParser::Capsule*) ScanCapsule(
        /* [in] */ String capfile);

    CARAPI_(String) FixProcessName(
        /* [in] */ String defProcessName,
        /* [in] */ String processName,
        /* [in] */ Int32 uid);

    CARAPI_(Int32) GetContinuationPoint(
        /* [in] */ const ArrayOf<String>& keys,
        /* [in] */ const String& key);

    static CARAPI_(Boolean) IsForwardLocked(
        /* [in] */ CapsuleParser::Capsule* cap);

    static CARAPI_(Boolean) IsExternal(
        /* [in] */ CapsuleParser::Capsule* cap);

    static CARAPI_(Boolean) IsSystemApp(
        /* [in] */ CapsuleParser::Capsule* cap);

    static CARAPI_(Boolean) IsSystemApp(
        /* [in] */ CApplicationInfo* info);

    CARAPI CreateInstallArgs(
        /* [in] */ Int32 flags,
        /* [in] */ String fullCodePath,
        /* [in] */ String fullResourcePath,
        /* [in] */ String nativeLibraryPath,
        /* [out] */ InstallArgs** args);

    CARAPI_(Boolean) DeleteCapsuleX(
        /* [in] */ String capsuleName,
        /* [in] */ Boolean sendBroadCast,
        /* [in] */ Boolean deleteCodeAndResources,
        /* [in] */ Int32 flags);

    CARAPI RemoveCapsuleDataLI(
        /* [in] */ CapsuleParser::Capsule* p,
        /* [in] */ CapsuleRemovedInfo* outInfo,
        /* [in] */ Int32 flags,
        /* [in] */ Boolean writeSettings);

    CARAPI_(Boolean) DeleteSystemCapsuleLI(
        /* [in] */ CapsuleParser::Capsule* p,
        /* [in] */ Int32 flags,
        /* [in] */ CapsuleRemovedInfo* outInfo,
        /* [in] */ Boolean writeSettings);

    CARAPI_(Boolean) DeleteInstalledCapsuleLI(
        /* [in] */ CapsuleParser::Capsule* p,
        /* [in] */ Boolean deleteCodeAndResources,
        /* [in] */ Int32 flags,
        /* [in] */ CapsuleRemovedInfo* outInfo,
        /* [in] */ Boolean writeSettings);

    CARAPI_(Boolean) DeleteCapsuleLI(
        /* [in] */ String capsuleName,
        /* [in] */ Boolean deleteCodeAndResources,
        /* [in] */ Int32 flags,
        /* [in] */ CapsuleRemovedInfo* outInfo,
        /* [in] */ Boolean writeSettings);

    CARAPI ScanDirLI(
        /* [in] */ IFile* dir,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 scanMode,
        /* [in] */ Int64 currentTime);

    CARAPI KillApplication(
        /* [in] */ String capName,
        /* [in] */ Int32 uid);

    CARAPI_(CapsuleParser::Capsule*) ScanCapsuleLI(
        /* [in] */ IFile* scanFile,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanMode,
        /* [in] */ Int64 currentTime);

    CARAPI_(void) SetApplicationInfoPaths(
        /* [in] */ CapsuleParser::Capsule* cap,
        /* [in] */ String destCodePath,
        /* [in] */ String destResPath);

    static CARAPI_(Boolean) UseEncryptedFilesystemForCapsule(
        /* [in] */ CapsuleParser::Capsule* cap);

    CARAPI GetDataPathForCapsule(
        /* [in] */ const CapsuleParser::Capsule& cap,
        /* [out] */ IFile** path);

    CARAPI_(CapsuleParser::Capsule*) ScanCapsuleLI(
        /* [in] */ CapsuleParser::Capsule* cap,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanMode,
        /* [in] */ Int64 currentTime);

    CARAPI RemoveCapsuleLI(
        /* [in] */ CapsuleParser::Capsule* cap,
        /* [in] */ Boolean chatty);

    static CARAPI_(Boolean) IsCapsuleFilename(
        /* [in] */ String name);

    CARAPI UpdatePermissionsLP(
        /* [in] */ String changingPkg,
        /* [in] */ CapsuleParser::Capsule* capInfo,
        /* [in] */ Boolean grantPermissions,
        /* [in] */ Boolean replace,
        /* [in] */ Boolean replaceAll);

    CARAPI GenerateCapsuleInfoFromSettingsLP(
        /* [in] */ String capsuleName,
        /* [in] */ Int32 flags,
        /* [out] */ ICapsuleInfo** capInfo);

    CARAPI GenerateApplicationInfoFromSettingsLP(
        /* [in] */ String capsuleName,
        /* [in] */ Int32 flags,
        /* [out] */ IApplicationInfo** info);

    CARAPI_(void) SendCapsuleChangedBroadcast(
        /* [in] */ String capsuleName,
        /* [in] */ Boolean killFlag,
        /* [in] */ List<String>& componentNames,
        /* [in] */ Int32 capsuleUid);

    CARAPI_(void) SendCapsuleBroadcast(
        /* [in] */ String action,
        /* [in] */ String cap,
        /* [in] */ IBundle* extras,
        /* [in] */ IIntentReceiver* finishedReceiver);

    CARAPI_(void) ScheduleCapsuleCleaning(
        /* [in] */ String capsuleName);

    CARAPI_(void) StartCleaningCapsules();

    /*
     * Utility method to unload a list of specified containers
     */
    CARAPI_(void) UnloadAllContainers(
        /* [in] */ Set<SdInstallArgs*>& cidArgs);

    CARAPI GenerateComponentNameKey(
        /* [in] */ String capsuleName,
        /* [in] */ String name,
        /* [out] */ String* compName);

    CARAPI SetEnabledSetting(
        /* [in] */ String capsuleName,
        /* [in] */ String className,
        /* [in] */ Int32 newState,
        /* [in] */ Int32 flags);

    CARAPI_(void) HandleFreeStorageAndNotify(
        /* [in] */ Int64 freeStorageSize,
        /* [in] */ ICapsuleDataObserver* observer);

    CARAPI_(void) HandleFreeStorage(
        /* [in] */ Int64 freeStorageSize,
        /* [in] */ IIntentSender* pi);

    CARAPI_(void) HandleDeleteApplicationCacheFiles(
        /* [in] */ String capsuleName,
        /* [in] */ ICapsuleDataObserver* observer);

    CARAPI_(Boolean) DeleteApplicationCacheFilesLI(
        /* [in] */ String capsuleName);

    CARAPI_(void) HandleClearApplicationUserData(
        /* [in] */ String capsuleName,
        /* [in] */ ICapsuleDataObserver* observer);

    CARAPI_(Boolean) ClearApplicationUserDataLI(
        /* [in] */ String capsuleName);

    CARAPI_(void) HandleGetCapsuleSizeInfo(
        /* [in] */ String capsuleName,
        /* [in] */ ICapsuleStatsObserver* observer);

    CARAPI_(Boolean) GetCapsuleSizeInfoLI(
        /* [in] */ String capsuleName,
        /* [in] */ ICapsuleStats* stats);

    CARAPI_(void) HandleUpdateExternalMediaStatus(
        /* [in] */ Boolean mediaStatus,
        /* [in] */ Boolean reportStatus);

    CARAPI UpdateExternalMediaStatusInner(
        /* [in] */ Boolean mediaStatus,
        /* [in] */ Boolean reportStatus);

    CARAPI LoadMediaCapsules(
        /* [in] */ HashMap<String, SdInstallArgs*>& processCids,
        /* [in] */ const ArrayOf<Int32>& uidArr,
        /* [in] */ List<String>& removeCids);

    CARAPI UnloadMediaCapsules(
        /* [in] */ HashMap<String, SdInstallArgs*>& processCids,
        /* [in] */ const ArrayOf<Int32>& uidArr,
        /* [in] */ Boolean reportStatus);

    CARAPI_(void) SendResourcesChangedBroadcast(
        /* [in] */ Boolean mediaStatus,
        /* [in] */ List<String>& capList,
        /* [in] */ const ArrayOf<Int32>& uidArr,
        /* [in] */ IIntentReceiver* finishedReceiver);

    CARAPI_(void) HandleProcessPendingMove(
        /* [in] */ MoveParams* mp,
        /* [in] */ Int32 currentStatus);

    CARAPI_(void) ProcessPendingMove(
        /* [in] */ MoveParams* mp,
        /* [in] */ Int32 currentStatus);

    CARAPI AddPermissionLocked(
        /* [in] */ IPermissionInfo* info,
        /* [in] */ Boolean async,
        /* [out] */ Boolean* isAdded);

    CARAPI CheckPermissionTreeLP(
        /* [in] */ String permName,
        /* [out] */ BasePermission** basePerm);

    CARAPI FindPermissionTreeLP(
        /* [in] */ String permName,
        /* [out] */ BasePermission** basePerm);

    static CARAPI_(Boolean) ComparePermissionInfos(
        /* [in] */ IPermissionInfo* pi1,
        /* [in] */ IPermissionInfo* pi2);

    CARAPI_(void) ScheduleWriteSettingsLocked();

    static CARAPI_(Boolean) CompareStrings(
        /* [in] */ String s1,
        /* [in] */ String s2);

private:
    static const Boolean MULTIPLE_APPLICATION_UIDS = TRUE;
    static const Int32 RADIO_UID = Process::PHONE_UID;
    static const Int32 LOG_UID = Process::LOG_UID;
    static const Int32 NFC_UID = Process::NFC_UID;
    static const Int32 FIRST_APPLICATION_UID = Process::FIRST_APPLICATION_UID;
    static const Int32 MAX_APPLICATION_UIDS = 1000;

    static const Boolean SHOW_INFO = FALSE;

    static const Boolean GET_CERTIFICATES = TRUE;

    static const String SYSTEM_PROPERTY_EFS_ENABLED;

//	    static const int REMOVE_EVENTS =
//	        FileObserver.CLOSE_WRITE | FileObserver.DELETE | FileObserver.MOVED_FROM;
//	    static const int ADD_EVENTS =
//	        FileObserver.CLOSE_WRITE /*| FileObserver.CREATE*/ | FileObserver.MOVED_TO;

//	    static const int OBSERVER_EVENTS = REMOVE_EVENTS | ADD_EVENTS;
    // Suffix used during package installation when copying/moving
    // package apks to install directory.
    static const String INSTALL_PACKAGE_SUFFIX;

    /**
     * Indicates the state of installation. Used by PackageManager to
     * figure out incomplete installations. Say a package is being installed
     * (the state is set to PKG_INSTALL_INCOMPLETE) and remains so till
     * the package installation is successful or unsuccesful lin which case
     * the PackageManager will no longer maintain state information associated
     * with the package. If some exception(like device freeze or battery being
     * pulled out) occurs during installation of a package, the PackageManager
     * needs this information to clean up the previously failed installation.
     */
    static const Int32 CAP_INSTALL_INCOMPLETE = 0;
    static const Int32 CAP_INSTALL_COMPLETE = 1;

public: /*package*/
    static const Int32 SCAN_MONITOR = 1<<0;
    static const Int32 SCAN_NO_DEX = 1<<1;
    static const Int32 SCAN_FORCE_DEX = 1<<2;
    static const Int32 SCAN_UPDATE_SIGNATURE = 1<<3;
    static const Int32 SCAN_NEW_INSTALL = 1<<4;
    static const Int32 SCAN_NO_PATHS = 1<<5;
    static const Int32 SCAN_UPDATE_TIME = 1<<6;

    static const Int32 REMOVE_CHATTY = 1<<16;

    static const String DEFAULT_CONTAINER_CAPSULE;

    static const AutoPtr<IComponentName> DEFAULT_CONTAINER_COMPONENT;

private:
    static const String LIB_DIR_NAME;

public: /*package*/
    static const String mTempContainerPrefix;

//	    final HandlerThread mHandlerThread = new HandlerThread("PackageManager",
//	            Process.THREAD_PRIORITY_BACKGROUND);
    CapsuleHandler* mHandler;

    Int32 mSdkVersion; // = Build.VERSION.SDK_INT;
//	    final String mSdkCodename = "REL".equals(Build.VERSION.CODENAME)
//	            ? null : Build.VERSION.CODENAME;

    AutoPtr<IContext> mContext;
    Boolean mFactoryTest;
    Boolean mNoDexOpt;
    AutoPtr<IDisplayMetrics> mMetrics;
    Int32 mDefParseFlags;
    AutoStringArray mSeparateProcesses;

    // This is where all application persistent data goes.
    AutoPtr<IFile> mAppDataDir;

    // If Encrypted File System feature is enabled, all application persistent data
    // should go here instead.
    AutoPtr<IFile> mSecureAppDataDir;

    // This is the object monitoring the framework dir.
//	    final FileObserver mFrameworkInstallObserver;

    // This is the object monitoring the system app dir.
//	    final FileObserver mSystemInstallObserver;

    // This is the object monitoring the system app dir.
//	    final FileObserver mVendorInstallObserver;

    // This is the object monitoring mAppInstallDir.
//	    final FileObserver mAppInstallObserver;

    // This is the object monitoring mDrmAppPrivateInstallDir.
//	    final FileObserver mDrmAppInstallObserver;

    // Used for priviledge escalation.  MUST NOT BE CALLED WITH mPackages
    // LOCK HELD.  Can be called with mInstallLock held.
    Installer* mInstaller;

    AutoPtr<IFile> mFrameworkDir;
    AutoPtr<IFile> mSystemAppDir;
    AutoPtr<IFile> mVendorAppDir;
    AutoPtr<IFile> mAppInstallDir;
    AutoPtr<IFile> mDalvikCacheDir;

    // Directory containing the private parts (e.g. code and non-resource assets) of forward-locked
    // apps.
    AutoPtr<IFile> mDrmAppPrivateInstallDir;

    // ----------------------------------------------------------------

    // Lock for state used when installing and doing other long running
    // operations.  Methods that must be called with this lock held have
    // the prefix "LI".
    Mutex mInstallLock;

    // These are the directories in the 3rd party applications installed dir
    // that we have currently loaded packages from.  Keys are the application's
    // installed zip file (absolute codePath), and values are Package.
    HashMap<String, CapsuleParser::Capsule*> mAppDirs;

    // Information for the parser to write more useful error messages.
    AutoPtr<IFile> mScanningPath;
    Int32 mLastScanError;

//	    final int[] mOutPermissions = new int[3];

    // ----------------------------------------------------------------

    // Keys are String (package name), values are Package.  This also serves
    // as the lock for the global state.  Methods that must be called with
    // this lock held have the prefix "LP".
    HashMap<String, CapsuleParser::Capsule*> mCapsules;
    Mutex mCapsulesLock;

    Settings* mSettings;
    Boolean mRestoredSettings;

    // Group-ids that are given to all packages as read from etc/permissions/*.xml.
    AutoFree< ArrayOf<Int32> > mGlobalGids;

    // These are the built-in uid -> permission mappings that were read from the
    // etc/permissions.xml file.
//	    final SparseArray<HashSet<String>> mSystemPermissions =
//	            new SparseArray<HashSet<String>>();

    // These are the built-in shared libraries that were read from the
    // etc/permissions.xml file.
    HashMap<String, String> mSharedLibraries;

    // Temporary for building the final shared libraries for an .apk.
    AutoStringArray mTmpSharedLibraries;

    // These are the features this devices supports that were read from the
    // etc/permissions.xml file.
    HashMap<String, IFeatureInfo*> mAvailableFeatures;

    // All available activities, for your resolving pleasure.
    ActivityIntentResolver* mActivities;

    // All available receivers, for your resolving pleasure.
    ActivityIntentResolver* mReceivers;

    // All available services, for your resolving pleasure.
    ServiceIntentResolver* mServices;

    // Keys are String (provider class name), values are Provider.
    HashMap<String, CapsuleParser::ContentProvider*> mProvidersByComponent;

    // Mapping from provider base names (first directory in content URI codePath)
    // to the provider information.
    HashMap<String, CapsuleParser::ContentProvider*> mProviders;

    // Mapping from instrumentation class names to info about them.
    HashMap<AutoPtr<IComponentName>, CapsuleParser::Instrumentation*> mInstrumentation;

    // Mapping from permission names to info about them.
    HashMap<String, CapsuleParser::PermissionGroup*> mPermissionGroups;
//	            new HashMap<String, PackageParser.PermissionGroup>();

    // Packages whose data we have transfered into another package, thus
    // should no longer exist.
    HashSet<String> mTransferedCapsules;

    // Broadcast actions that are only available to the system.
    HashSet<String> mProtectedBroadcasts;

    Boolean mSystemReady;
    Boolean mSafeMode;
    Boolean mHasSystemUidErrors;

    AutoPtr<CApplicationInfo> mElastosApplication;
    AutoPtr<CActivityInfo> mResolveActivity;
    AutoPtr<CResolveInfo> mResolveInfo;
    AutoPtr<IComponentName> mResolveComponentName;
    CapsuleParser::Capsule* mPlatformCapsule;

    HashMap<Int32, PostInstallData*> mRunningInstalls;
    Int32 mNextInstallToken; // nonzero; will be wrapped back to 1 when ++ overflows

    // Set of pending broadcasts for aggregating enable/disable of components.
    HashMap< String, List<String> > mPendingBroadcasts;

private:
    // Service Connection to remote media container service to copy
    // package uri's from external media onto secure containers
    // or internal storage.
    AutoPtr<IMediaContainerService> mContainerService;

public: /*package*/
    static const Int32 SEND_PENDING_BROADCAST = 1;
    static const Int32 MCS_BOUND = 3;
    static const Int32 END_COPY = 4;
    static const Int32 INIT_COPY = 5;
    static const Int32 MCS_UNBIND = 6;
    static const Int32 START_CLEANING_CAPSULE = 7;
    static const Int32 FIND_INSTALL_LOC = 8;
    static const Int32 POST_INSTALL = 9;
    static const Int32 MCS_RECONNECT = 10;
    static const Int32 MCS_GIVE_UP = 11;
    static const Int32 UPDATED_MEDIA_STATUS = 12;
    static const Int32 WRITE_SETTINGS = 13;

    static const Int32 DELETE_CAPSULE = 14;

    static const Int32 WRITE_SETTINGS_DELAY = 10*1000;  // 10 seconds

    // Delay time in millisecs
    static const Int32 BROADCAST_DELAY = 10 * 1000;

private:
    AutoPtr<DefaultContainerConnection> mDefContainerConn;

    static const Int32 DEX_OPT_SKIPPED = 0;
    static const Int32 DEX_OPT_PERFORMED = 1;
    static const Int32 DEX_OPT_FAILED = -1;

private:
//	    private static final Comparator<ResolveInfo> mResolvePrioritySorter =
//	            new Comparator<ResolveInfo>() {
//
//	    private static final Comparator<ProviderInfo> mProviderInitOrderSorter =
//	            new Comparator<ProviderInfo>() {

public: /*package*/
    // ------- apps on sdcard specific code -------
    static const Boolean DEBUG_SD_INSTALL = FALSE;
private:
    static const String SD_ENCRYPTION_KEYSTORE_NAME;
    static const String SD_ENCRYPTION_ALGORITHM;
public: /*package*/
    static const Int32 MAX_CONTAINERS = 250;
private:
    Boolean mMediaMounted;

    static const String DEFAULT_RESOURCES_FILE_NAME;

private:
    // one param means Message*
    AutoPtr<IApartment> mApartment;

    friend class CapsuleHandler;
};


#endif // __CCAPSULEMANAGER_H__
