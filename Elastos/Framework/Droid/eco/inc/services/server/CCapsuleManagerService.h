
#ifndef __CCAPSULEMANAGER_H__
#define __CCAPSULEMANAGER_H__

#include "_CCapsuleManagerService.h"

#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>
#include <elastos/AutoFree.h>
#include <elastos/HashMap.h>
#include <elastos/HashSet.h>
#include <elastos/List.h>

#include "server/IntentResolver.h"
#include "server/Installer.h"
#include "capsule/CapsuleParser.h"
#include "content/IntentFilter.h"
#include "os/Process.h"
#include "os/FileObserver.h"
#include "utils/AutoStringArray.h"
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>
#include <StringBuffer.h>
#include <Elastos.Utility.Zip.h>

using namespace Elastos::Core;
using namespace Elastos::Core::Threading;

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IComponentName> >
{
    size_t operator()(AutoPtr<IComponentName> name) const
    {
        Int32 hashCode;
        assert(name != NULL);
        name->GetHashCode(&hashCode);
        return (size_t)hashCode;
    }
};

template<> struct EqualTo<AutoPtr<IComponentName> >
{
    Boolean operator()(const AutoPtr<IComponentName>& x,
                       const AutoPtr<IComponentName>& y) const
    {
        Boolean isEqual;
        assert(x != NULL);
        x->Equals(y, &isEqual);
        return isEqual;
    }
};

_ELASTOS_NAMESPACE_END

extern "C" const InterfaceID EIID_CapsuleSetting;
extern "C" const InterfaceID EIID_SharedUserSetting;

CarClass(CCapsuleManagerService)
{
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
        CCapsuleManagerService* mOwner;
    };

    class InstallArgs;
    class CapsuleInstalledInfo;

    // Recordkeeping of restore-after-install operations that are currently in flight
    // between the Package Manager and the Backup Manager
    class PostInstallData
    {
    public:
        PostInstallData(
            /* [in] */ InstallArgs* a,
            /* [in] */ CapsuleInstalledInfo* r)
            : mArgs(a)
            , mRes(r)
        {}

    public:
        AutoPtr<InstallArgs> mArgs;
        CapsuleInstalledInfo* mRes;
    };

    class HandlerParams : public ElRefBase, public IInterface
    {
    public:
        HandlerParams(
            /* [in] */ CCapsuleManagerService* owner)
            : mRetry(0)
            , mOwner(owner)
        {}

        virtual ~HandlerParams() {};

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI_(void) StartCopy();

        CARAPI_(void) ServiceError();

        virtual CARAPI HandleStartCopy() = 0;

        virtual CARAPI_(void) HandleServiceError() = 0;

        virtual CARAPI_(void) HandleReturnCode() = 0;

    public:
        static const Int32 MAX_RETRIES = 4;
        Int32 mRetry;
        CCapsuleManagerService* mOwner;
    };

    class InstallParams : public HandlerParams
    {
    public:
        InstallParams(
            /* [in] */ IUri* capsuleURI,
            /* [in] */ ICapsuleInstallObserver* observer,
            /* [in] */ Int32 flags,
            /* [in] */ const String& installerCapsuleName,
            /* [in] */ CCapsuleManagerService* owner)
            : HandlerParams(owner)
            , mObserver(observer)
            , mFlags(flags)
            , mCapsuleURI(capsuleURI)
            , mInstallerCapsuleName(installerCapsuleName)
            , mRet(0)
        {}

        ~InstallParams() {};

        CARAPI_(Int32) InstallLocationPolicy(
            /* [in] */ ICapsuleInfoLite* capLite,
            /* [in] */ Int32 flags);

        /*
         * Invoke remote method to get package information and install
         * location values. Override install location based on default
         * policy if needed and then create install arguments based
         * on the install location.
         */
        //@Override
        CARAPI HandleStartCopy();

        //@Override
        CARAPI_(void) HandleReturnCode();

        //@Override
        CARAPI_(void) HandleServiceError();

    public:
        AutoPtr<ICapsuleInstallObserver> mObserver;
        Int32 mFlags;
        AutoPtr<IUri> mCapsuleURI;
        String mInstallerCapsuleName;
        AutoPtr<InstallArgs> mArgs;
        Int32 mRet;
    };

    class MoveParams : public HandlerParams
    {
    public:
        MoveParams(
            /* [in] */ InstallArgs* srcArgs,
            /* [in] */ ICapsuleMoveObserver* observer,
            /* [in] */ Int32 flags,
            /* [in] */ const String& capsuleName,
            /* [in] */ const String& dataDir,
            /* [in] */ CCapsuleManagerService* owner);

        //@Override
        CARAPI HandleStartCopy();

        //@Override
        CARAPI_(void) HandleReturnCode();

        //@Override
        CARAPI_(void) HandleServiceError();

    public:
        AutoPtr<ICapsuleMoveObserver> mObserver;
        Int32 mFlags;
        String mCapsuleName;
        AutoPtr<InstallArgs> mSrcArgs;
        AutoPtr<InstallArgs> mTargetArgs;
        Int32 mRet;
    };

    class InstallArgs : public ElRefBase, public IInterface
    {
    public:
        InstallArgs(
            /* [in] */ IUri* capsuleURI,
            /* [in] */ ICapsuleInstallObserver* observer,
            /* [in] */ Int32 flags,
            /* [in] */ const String& installerCapsuleName,
            /* [in] */ CCapsuleManagerService* owner)
            : mObserver(observer)
            , mFlags(flags)
            , mCapsuleURI(capsuleURI)
            , mInstallerCapsuleName(installerCapsuleName)
            , mOwner(owner)
        {}

        virtual ~InstallArgs() {};

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        virtual CARAPI_(void) CreateCopyFile() = 0;

        virtual CARAPI_(Int32) CopyCap(
            /* [in] */ IMediaContainerService* imcs,
            /* [in] */ Boolean temp) = 0;

        virtual CARAPI_(Int32) DoPreInstall(
            /* [in] */ Int32 status) = 0;

        virtual CARAPI_(Boolean) DoRename(
            /* [in] */ Int32 status,
            /* [in] */ const String& capName,
            /* [in] */ const String& oldCodePath) = 0;

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
        String mInstallerCapsuleName;
        CCapsuleManagerService* mOwner;
    };

    class FileInstallArgs : public InstallArgs
    {
    public:
        FileInstallArgs(
            /* [in] */ InstallParams* params,
            /* [in] */ CCapsuleManagerService* owner)
            : InstallArgs(params->mCapsuleURI, params->mObserver,
                          params->mFlags, params->mInstallerCapsuleName, owner)
            , mCreated(FALSE)
        {}

        FileInstallArgs(
            /* [in] */ const String& fullCodePath,
            /* [in] */ const String& fullResourcePath,
            /* [in] */ const String& nativeLibraryPath,
            /* [in] */ CCapsuleManagerService* owner);

        FileInstallArgs(
            /* [in] */ IUri* capsuleURI,
            /* [in] */ const String& capName,
            /* [in] */ const String& dataDir,
            /* [in] */ CCapsuleManagerService* owner);

        CARAPI_(Boolean) CheckFreeStorage(
            /* [in] */ IMediaContainerService* imcs);

        CARAPI_(String) GetCodePath();

        CARAPI_(void) CreateCopyFile();

        CARAPI_(Int32) CopyCap(
            /* [in] */ IMediaContainerService* imcs,
            /* [in] */ Boolean temp);

        CARAPI_(Int32) DoPreInstall(
            /* [in] */ Int32 status);

        CARAPI_(Boolean) DoRename(
            /* [in] */ Int32 status,
            /* [in] */ const String& capName,
            /* [in] */ const String& oldCodePath);

        CARAPI_(Int32) DoPostInstall(
            /* [in] */ Int32 status);

        CARAPI_(String) GetResourcePath();

        virtual CARAPI_(String) GetResourcePathFromCodePath();

        CARAPI_(String) GetNativeLibraryPath();

        CARAPI_(void) CleanUpResourcesLI();

        CARAPI_(Boolean) DoPostDeleteLI(
            /* [in] */ Boolean del);

    private:
        CARAPI_(Boolean) CleanUp();

        CARAPI_(Boolean) SetPermissions();

        CARAPI_(Boolean) IsFwdLocked();

    public:
        AutoPtr<IFile> mInstallDir;
        String mCodeFileName;
        String mResourceFileName;
        String mLibraryPath;
        Boolean mCreated;
    };

    class SdInstallArgs : public InstallArgs
    {
    public:
        SdInstallArgs(
            /* [in] */ InstallParams* params,
            /* [in] */ CCapsuleManagerService* owner)
            : InstallArgs(params->mCapsuleURI, params->mObserver,
                          params->mFlags, params->mInstallerCapsuleName, owner)
        {}

        SdInstallArgs(
            /* [in] */ const String& fullCodePath,
            /* [in] */ const String& fullResourcePath,
            /* [in] */ const String& nativeLibraryPath,
            /* [in] */ CCapsuleManagerService* owner);

        SdInstallArgs(
            /* [in] */ const String& cid,
            /* [in] */ CCapsuleManagerService* owner);

        SdInstallArgs(
            /* [in] */ IUri* capsuleURI,
            /* [in] */ const String& cid,
            /* [in] */ CCapsuleManagerService* owner)
            : InstallArgs(capsuleURI, NULL, 0/*CapsuleManager::INSTALL_EXTERNAL*/, String(NULL), owner)
            , mCid(cid)
        {}

        CARAPI_(void) CreateCopyFile();

        CARAPI_(Boolean) CheckFreeStorage(
            /* [in] */ IMediaContainerService* imcs);

        CARAPI_(Int32) CopyCap(
            /* [in] */ IMediaContainerService* imcs,
            /* [in] */ Boolean temp);

        CARAPI_(String) GetCodePath();

        CARAPI_(String) GetResourcePath();

        CARAPI_(String) GetNativeLibraryPath();

        CARAPI_(Int32) DoPreInstall(
            /* [in] */ Int32 status);

        CARAPI_(Boolean) DoRename(
            /* [in] */ Int32 status,
            /* [in] */ const String& capName,
            /* [in] */ const String& oldCodePath);

        CARAPI_(Int32) DoPostInstall(
            /* [in] */ Int32 status);

        CARAPI_(void) CleanUpResourcesLI();

        CARAPI_(Boolean) MatchContainer(
            /* [in] */ String app);

        CARAPI_(String) GetCapsuleName();

        CARAPI_(Boolean) DoPostDeleteLI(
            /* [in] */ Boolean del);

    private:
        CARAPI_(void) SetCachePath(
            /* [in] */ String newCachePath);

        CARAPI_(void) CleanUp();

    public:
        static const char* RES_FILE_NAME;

        String mCid;
        String mCapsulePath;
        String mLibraryPath;
    };

    class CapsuleRemovedInfo;

    class CapsuleInstalledInfo
    {
    public:
        CapsuleInstalledInfo()
            : mUid(0)
            , mCap(NULL)
            , mReturnCode(0)
            , mRemovedInfo(NULL) {}

    public:
        String mName;
        Int32 mUid;
        AutoPtr<CapsuleParser::Capsule> mCap;
        Int32 mReturnCode;
        CapsuleRemovedInfo* mRemovedInfo;
    };

    class CapsuleRemovedInfo
    {
    public:
        CapsuleRemovedInfo(
            /* [in] */ CCapsuleManagerService* owner)
            : mUid(-1)
            , mRemovedUid(-1)
            , mIsRemovedCapsuleSystemUpdate(FALSE)
            , mArgs(NULL)
            , mOwner(owner)
        {}

        CARAPI_(void) SendBroadcast(
            /* [in] */ Boolean fullRemove,
            /* [in] */ Boolean replacing);

    public:
        String mRemovedCapsule;
        Int32 mUid;
        Int32 mRemovedUid;
        Boolean mIsRemovedCapsuleSystemUpdate;
        // Clean up resources deleted capsules.
        InstallArgs* mArgs;
        CCapsuleManagerService* mOwner;
    };

    class CapsuleSettingBase;

    class BasePermission
    {
    public:
        BasePermission(
            /* [in] */ const String& name,
            /* [in] */ const String& sourceCapsule,
            /* [in] */ Int32 type)
            : mName(name)
            , mSourceCapsule(sourceCapsule)
            , mCapsuleSetting(NULL)
            , mType(type)
            // Default to most conservative protection level.
            , mProtectionLevel(PermissionInfo_PROTECTION_SIGNATURE)
            , mPerm(NULL)
            , mUid(0)
        {}

        ~BasePermission() {};

        CARAPI_(String) ToString();

    public:
        static const Int32 TYPE_NORMAL = 0;
        static const Int32 TYPE_BUILTIN = 1;
        static const Int32 TYPE_DYNAMIC = 2;

        String mName;
        String mSourceCapsule;
        CapsuleSettingBase* mCapsuleSetting;
        Int32 mType;
        Int32 mProtectionLevel;
        CapsuleParser::Permission* mPerm;
        AutoPtr<IPermissionInfo> mPendingInfo;
        Int32 mUid;
        AutoFree< ArrayOf<Int32> > mGids;
    };

    class CapsuleSignatures : public ElRefBase, public IInterface
    {
    public:
        CapsuleSignatures(
            /* [in] */ const CapsuleSignatures* orig);

        CapsuleSignatures(
            /* [in] */ const ArrayOf< AutoPtr<ISignature> >& sigs);

        CapsuleSignatures();

        ~CapsuleSignatures();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI_(void) WriteXml(
            /* [in] */ IXmlSerializer* serializer,
            /* [in] */ CString tagName,
            /* [in] */ List< AutoPtr<ISignature> >* pastSignatures);

        CARAPI_(void) ReadXml(
            /* [in] */ IXmlPullParser* parser,
            /* [in] */ List< AutoPtr<ISignature> >* pastSignatures);

        CARAPI_(void) AssignSignatures(
            /* [in] */ const ArrayOf< AutoPtr<ISignature> >& sigs);

    public:
        AutoFree< ArrayOf< AutoPtr<ISignature> > > mSignatures;
    };

    class PreferredActivity
        : public ElRefBase
        , public IntentFilter
        , public IIntentFilter
    {
    public:
        PreferredActivity(
            /* [in] */ IIntentFilter* filter,
            /* [in] */ Int32 match,
            /* [in] */ IObjectContainer* set, /*IComponentName*/
            /* [in] */ IComponentName* activity);

        PreferredActivity(
            /* [in] */ IXmlPullParser* parser);

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
            /* [in] */ const String& action);

        CARAPI CountActions(
            /* [out] */ Int32 *count);

        CARAPI GetAction(
            /* [in] */ Int32 index,
            /* [out] */ String *action);

        CARAPI HasAction(
            /* [in] */ const String& action,
            /* [out] */ Boolean* hasAction);

        CARAPI MatchAction(
            /* [in] */ const String& action,
            /* [out] */ Boolean* isMatched);

        CARAPI GetActions(
            /* [out, callee] */ ArrayOf<String>** actions);

        CARAPI AddDataType(
            /* [in] */ const String& type);

        CARAPI HasDataType(
            /* [in] */ const String& type,
            /* [out] */ Boolean* hasDataType);

        CARAPI CountDataTypes(
            /* [out] */ Int32* count);

        CARAPI GetDataType(
            /* [in] */ Int32 index,
            /* [out] */ String* type);

        CARAPI GetTypes(
            /* [out, callee] */ ArrayOf<String>** types);

        CARAPI AddDataScheme(
            /* [in] */ const String& scheme);

        CARAPI CountDataSchemes(
            /* [out] */ Int32* count);

        CARAPI GetDataScheme(
            /* [in] */ Int32 index,
            /* [out] */ String* scheme);

        CARAPI HasDataScheme(
            /* [in] */ const String& scheme,
            /* [out] */ Boolean* result);

        CARAPI GetSchemes(
            /* [out, callee] */ ArrayOf<String>** schemes);

        CARAPI AddDataAuthority(
            /* [in] */ const String& host,
            /* [in] */ const String& port);

        CARAPI CountDataAuthorities(
            /* [out] */ Int32* count);

        CARAPI GetDataAuthority(
            /* [in] */ Int32 index,
            /* [out] */ IAuthorityEntry** authority);

        CARAPI HasDataAuthority(
            /* [in] */ IUri* data,
            /* [out] */ Boolean* result);

        CARAPI AddDataPath(
            /* [in] */ const String& path,
            /* [in] */ Int32 type);

        CARAPI CountDataPaths(
            /* [out] */ Int32* count);

        CARAPI GetDataPath(
            /* [in] */ Int32 index,
            /* [out] */ IPatternMatcher** path);

        CARAPI HasDataPath(
            /* [in] */ const String& data,
            /* [out] */ Boolean* result);

        CARAPI MatchDataAuthority(
            /* [in] */ IUri* data,
            /* [out] */ Int32* result);

        CARAPI MatchData(
            /* [in] */ const String& type,
            /* [in] */ const String& scheme,
            /* [in] */ IUri* data,
            /* [out] */ Int32* result);

        CARAPI AddCategory(
            /* [in] */ const String& category);

        CARAPI CountCategories(
            /* [out] */ Int32* count);

        CARAPI GetCategory(
            /* [in] */ Int32 index,
            /* [out] */ String* category);

        CARAPI HasCategory(
            /* [in] */ const String& category,
            /* [out] */ Boolean* hasCategory);

        CARAPI MatchCategories(
            /* [in] */ ArrayOf<String>* categories,
            /* [out] */ String* result);

        CARAPI Match(
            /* [in] */ const String& action,
            /* [in] */ const String& type,
            /* [in] */ const String& scheme,
            /* [in] */ IUri* data,
            /* [in] */ ArrayOf<String>* categories,
            /* [in] */ CString logTag,
            /* [out] */ Int32* result);

        CARAPI MatchEx(
            /* [in] */ IContentResolver* resolver,
            /* [in] */ IIntent* intent,
            /* [in] */ Boolean resolve,
            /* [in] */ CString logTag,
            /* [out] */ Int32* result);

        CARAPI WriteToXml(
            /* [in] */ IXmlSerializer* serializer);

        CARAPI ReadFromXml(
            /* [in] */ IXmlPullParser* parser);

        CARAPI ReadFromParcel(
            /* [in] */ IParcel *source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel *dest);

        Boolean SameSet(
            /* [in] */ IObjectContainer* query,
            /* [in] */ Int32 priority);

    public:
        AutoPtr<IIntentFilter> mFilter;
        Int32 mMatch;
        AutoStringArray mSetCapsules;
        AutoStringArray mSetClasses;
        AutoStringArray mSetComponents;
        AutoPtr<IComponentName> mActivity;
        String mShortActivity;
        String mParseError;
    };

    class GrantedPermissions : public ElRefBase, public IInterface
    {
    public:
        GrantedPermissions(
            /* [in] */ Int32 capFlags);

        GrantedPermissions(
            /* [in] */ const GrantedPermissions* base);

        virtual ~GrantedPermissions();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI_(void) SetFlags(
            /* [in] */ Int32 capFlags);

    public:
        Int32 mCapFlags;

        HashSet<String> mGrantedPermissions;
        AutoFree< ArrayOf<Int32> > mGids;
    };

    /**
     * Settings base class for pending and resolved classes.
     */
    class CapsuleSettingBase : public GrantedPermissions
    {
    public:
        CapsuleSettingBase(
            /* [in] */ const String& name,
            /* [in] */ const String& realName,
            /* [in] */ IFile* codePath,
            /* [in] */ IFile* resourcePath,
            /* [in] */ const String& nativeLibraryPathString,
            /* [in] */ Int32 cVersionCode,
            /* [in] */ Int32 capFlags);

        /**
         * New instance of PackageSetting with one-level-deep cloning.
         */
        CapsuleSettingBase(
            /* [in] */ const CapsuleSettingBase* base);

        ~CapsuleSettingBase();

        CARAPI_(void) Init(
            /* [in] */ IFile* codePath,
            /* [in] */ IFile* resourcePath,
            /* [in] */ const String& nativeLibraryPathString,
            /* [in] */ Int32 cVersionCode);

        CARAPI_(void) SetInstallerCapsuleName(
            /* [in] */ const String& capsuleName);

        CARAPI_(String) GetInstallerCapsuleName();

        CARAPI_(void) SetInstallStatus(
            /* [in] */ Int32 newStatus);

        CARAPI_(Int32) GetInstallStatus();

        CARAPI_(void) SetTimeStamp(
            /* [in] */ Int64 newStamp);

        /**
         * Make a shallow copy of this package settings.
         */
        CARAPI_(void) CopyFrom(
            /* [in] */ const CapsuleSettingBase* base);

        CARAPI_(Boolean) EnableComponentLP(
            /* [in] */ const String& componentClassName);

        CARAPI_(Boolean) DisableComponentLP(
            /* [in] */ const String& componentClassName);

        CARAPI_(Boolean) RestoreComponentLP(
            /* [in] */ const String& componentClassName);

        CARAPI_(Int32) CurrentEnabledStateLP(
            /* [in] */ const String& componentName);

    public:
        String mName;
        String mRealName;
        AutoPtr<IFile> mCodePath;
        String mCodePathString;
        AutoPtr<IFile> mResourcePath;
        String mResourcePathString;
        String mNativeLibraryPathString;
        Int64 mTimeStamp;
        Int64 mFirstInstallTime;
        Int64 mLastUpdateTime;
        Int32 mVersionCode;

        Boolean mUidError;

        AutoPtr<CapsuleSignatures> mSignatures;

        Boolean mPermissionsFixed;
        Boolean mHaveGids;

        /* Explicitly disabled components */
        HashSet<String> mDisabledComponents;
        /* Explicitly enabled components */
        HashSet<String> mEnabledComponents;
        Int32 mEnabled;
        Int32 mInstallStatus;

        AutoPtr<CapsuleSettingBase> mOrigCapsule;

        /* package name of the app that installed this package */
        String mInstallerCapsuleName;
    };

    class SharedUserSetting;

    /**
     * Settings data for a particular package we know about.
     */
    class CapsuleSetting : public CapsuleSettingBase
    {
    public:
        CapsuleSetting(
            /* [in] */ const String& name,
            /* [in] */ const String& realName,
            /* [in] */ IFile* codePath,
            /* [in] */ IFile* resourcePath,
            /* [in] */ const String& nativeLibraryPathString,
            /* [in] */ Int32 pVersionCode,
            /* [in] */ Int32 capFlags);

        /**
         * New instance of PackageSetting replicating the original settings.
         * Note that it keeps the same PackageParser.Package instance.
         */
        CapsuleSetting(
            /* [in] */ const CapsuleSetting* orig);

        ~CapsuleSetting();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(String) ToString();

        struct HashKey
        {
            size_t operator()(const CapsuleSetting* s) const
            {
                return (size_t)s->GetHashCode();
            }
        };

        CARAPI_(Int32) GetHashCode() const;

    public:
        Int32 mUserId;
        AutoPtr<CapsuleParser::Capsule> mCap;
        AutoPtr<SharedUserSetting> mSharedUser;
    };

    /**
     * Settings data for a particular shared user ID we know about.
     */
    class SharedUserSetting : public GrantedPermissions
    {
    public:
        SharedUserSetting(
            /* [in] */ const String& name,
            /* [in] */ Int32 capFlags);

        ~SharedUserSetting();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(String) ToString();

    public:
        String mName;
        Int32 mUserId;
        HashSet<AutoPtr<CapsuleSetting>, CapsuleSetting::HashKey> mCapsules;
        AutoPtr<CapsuleSignatures> mSignatures;
    };

private:
    class ActivityIntentResolver :
        public IntentResolver<CapsuleParser::ActivityIntentInfo,
                              AutoPtr<IResolveInfo> >
    {
    public:
        typedef IntentResolver<CapsuleParser::ActivityIntentInfo, \
                AutoPtr<IResolveInfo> >     Super;

    public:
        ActivityIntentResolver(
            /* [in] */ CCapsuleManagerService* owner);

        ~ActivityIntentResolver();

        CARAPI_(List<AutoPtr<IResolveInfo>*>*) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Boolean defaultOnly);

        CARAPI_(List<AutoPtr<IResolveInfo>*>*) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Int32 flags);

        CARAPI_(List<AutoPtr<IResolveInfo>*>*) QueryIntentForCapsule(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Int32 flags,
            /* [in] */ List<CapsuleParser::Activity*>* capsuleActivities);

        CARAPI_(void) AddActivity(
            /* [in] */ CapsuleParser::Activity* activity,
            /* [in] */ const char* type);

        CARAPI_(void) RemoveActivity(
            /* [in] */ CapsuleParser::Activity* activity,
            /* [in] */ const char* type);

    protected:
        //@Override
        CARAPI_(Boolean) AllowFilterResult(
            /* [in] */ CapsuleParser::ActivityIntentInfo* filter,
            /* [in] */ List<AutoPtr<IResolveInfo>*>* dest);

        //@Override
        CARAPI_(String) CapsuleForFilter(
            /* [in] */ CapsuleParser::ActivityIntentInfo* info);

        //@Override
        CARAPI NewResult(
            /* [in] */ CapsuleParser::ActivityIntentInfo* info,
            /* [in] */ Int32 match,
            /* [out] */ AutoPtr<IResolveInfo>** ret);

        //@Override
        CARAPI_(void) SortResults(
            /* [in] */ List<AutoPtr<IResolveInfo>*>* results);

    public:
        // Keys are String (activity class name), values are Activity.
        HashMap<AutoPtr<IComponentName>, CapsuleParser::Activity*> mActivities;
        Int32 mFlags;
        CCapsuleManagerService* mOwner;
    };

    class ServiceIntentResolver :
        public IntentResolver<CapsuleParser::ServiceIntentInfo,
                              AutoPtr<IResolveInfo> >
    {
    public:
        typedef IntentResolver<CapsuleParser::ServiceIntentInfo, \
                AutoPtr<IResolveInfo> >     Super;

    public:
        ServiceIntentResolver(
            /* [in] */ CCapsuleManagerService* owner);

        ~ServiceIntentResolver();

        CARAPI_(List<AutoPtr<IResolveInfo>*>*) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Boolean defaultOnly);

        CARAPI_(List<AutoPtr<IResolveInfo>*>*) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Int32 flags);

        CARAPI_(List<AutoPtr<IResolveInfo>*>*) QueryIntentForCapsule(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Int32 flags,
            /* [in] */ List<CapsuleParser::Service*>* capsuleServices);

        CARAPI_(void) AddService(
            /* [in] */ CapsuleParser::Service* service);

        CARAPI_(void) RemoveService(
            /* [in] */ CapsuleParser::Service* service);

    protected:
        //@Override
        CARAPI_(Boolean) AllowFilterResult(
            /* [in] */ CapsuleParser::ServiceIntentInfo* filter,
            /* [in] */ List<AutoPtr<IResolveInfo>*>* dest);

        //@Override
        CARAPI_(String) CapsuleForFilter(
            /* [in] */ CapsuleParser::ServiceIntentInfo* info);

        //@Override
        CARAPI NewResult(
            /* [in] */ CapsuleParser::ServiceIntentInfo* info,
            /* [in] */ Int32 match,
            /* [out] */ AutoPtr<IResolveInfo>** ret);

        //@Override
        CARAPI_(void) SortResults(
            /* [in] */ List<AutoPtr<IResolveInfo>*>* results);

    public:
        // Keys are String (activity class name), values are Activity.
        HashMap<AutoPtr<IComponentName>, CapsuleParser::Service*> mServices;
        Int32 mFlags;
        CCapsuleManagerService* mOwner;
    };

    class AppDirObserver : public ElRefBase, public IFileObserver, public FileObserver
    {
    public:
        AppDirObserver(
            /* [in] */ const String& path,
            /* [in] */ Int32 mask,
            /* [in] */ Boolean isrom,
            /* [in] */ CCapsuleManagerService* owner);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI StartWatching();

        CARAPI StopWatching();

        CARAPI OnEvent(
            /* [in] */ Int32 event,
            /* [in] */ const String& path);

    private:
        String mRootDir;
        Boolean mIsRom;
        CCapsuleManagerService* mOwner;
    };

    /**
     * Holds information about dynamic settings.
     */
    class Settings
    {
        friend class CCapsuleManagerService;

    private:
        class SettingsIntentResolver
            : public IntentResolver<PreferredActivity, PreferredActivity>
        {
        protected:
            virtual CARAPI_(String) CapsuleForFilter(
                /* [in] */ PreferredActivity* filter);

//            virtual CARAPI DumpFilter(
//                /* [in] */ IPrintWriter* out,
//                /* [in] */ const String& prefix,
//                /* [in] */ PreferredActivity* filter);
        };

        class PendingCapsule : public CapsuleSettingBase
        {
        public:
            PendingCapsule(
                /* [in] */ String name,
                /* [in] */ String realName,
                /* [in] */ IFile* codePath,
                /* [in] */ IFile* resourcePath,
                /* [in] */ String nativeLibraryPathString,
                /* [in] */ Int32 sharedId,
                /* [in] */ Int32 pVersionCode,
                /* [in] */ Int32 capFlags)
                :  CapsuleSettingBase(name, realName, codePath, resourcePath,
                   nativeLibraryPathString, pVersionCode, capFlags)
                , mSharedId(sharedId)
            {}

        public:
            Int32 mSharedId;
        };

    public:
        Settings(
            /* [in] */ CCapsuleManagerService* owner);

        ~Settings();

        CARAPI_(AutoPtr<CapsuleSetting>) GetCapsuleLP(
            /* [in] */ CapsuleParser::Capsule* cap,
            /* [in] */ CapsuleSetting* origCapsule,
            /* [in] */ const String& realName,
            /* [in] */ SharedUserSetting* sharedUser,
            /* [in] */ IFile* codePath,
            /* [in] */ IFile* resourcePath,
            /* [in] */ const String& nativeLibraryPathString,
            /* [in] */ Int32 capFlags,
            /* [in] */ Boolean create,
            /* [in] */ Boolean add);

        CARAPI_(AutoPtr<CapsuleSetting>) PeekCapsuleLP(
            /* [in] */ const String& name);

        CARAPI_(void) SetInstallStatus(
            /* [in] */ String capName,
            /* [in] */ Int32 status);

        CARAPI_(void) SetInstallerCapsuleName(
            /* [in] */ String capName,
            /* [in] */ String installerCapName);

        CARAPI_(String) GetInstallerCapsuleName(
            /* [in] */ String capName);

        CARAPI_(Int32) GetInstallStatus(
            /* [in] */ String capName);

        CARAPI_(AutoPtr<SharedUserSetting>) GetSharedUserLP(
            /* [in] */ const String& name,
            /* [in] */ Int32 capFlags,
            /* [in] */ Boolean create);

        CARAPI_(Boolean) DisableSystemCapsuleLP(
            /* [in] */ String name);

        CARAPI_(AutoPtr<CapsuleSetting>) EnableSystemCapsuleLP(
            /* [in] */ const String& name);

        CARAPI_(AutoPtr<CapsuleSetting>) AddCapsuleLP(
            /* [in] */ const String& name,
            /* [in] */ const String& realName,
            /* [in] */ IFile* codePath,
            /* [in] */ IFile* resourcePath,
            /* [in] */ const String& nativeLibraryPathString,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 vc,
            /* [in] */ Int32 capFlags);

        CARAPI_(AutoPtr<SharedUserSetting>) AddSharedUserLP(
            /* [in] */ const String& name,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 capFlags);

        CARAPI_(void) WriteLP();

        CARAPI_(void) WriteDisabledSysCapsule(
            /* [in] */ IXmlSerializer* serializer,
            /* [in] */ const CapsuleSetting* cap);

        CARAPI_(void) WriteCapsule(
            /* [in] */ IXmlSerializer* serializer,
            /* [in] */ const CapsuleSetting* cap);

        CARAPI_(void) WritePermission(
            /* [in] */ IXmlSerializer* serializer,
            /* [in] */ BasePermission* bp);

        CARAPI_(String) GetReadMessagesLP();

        CARAPI_(List< AutoPtr<CapsuleSetting> >*) GetListOfIncompleteInstallCapsules();

        CARAPI_(Boolean) ReadLP();

        CARAPI_(AutoPtr<CapsuleSetting>) GetDisabledSystemCap(
            /* [in] */ const String& name);

        CARAPI_(Boolean) IsEnabledLP(
            /* [in] */ IComponentInfo* componentInfo,
            /* [in] */ Int32 flags);

    private:
        // Transfer ownership of permissions from one package to another.
        CARAPI_(void) TransferPermissions(
            /* [in] */ const String& origCap,
            /* [in] */ const String& newCap);

        CARAPI_(AutoPtr<CapsuleSetting>) GetCapsuleLP(
            /* [in] */ const String& name,
            /* [in] */ CapsuleSetting* origCapsule,
            /* [in] */ const String& realName,
            /* [in] */ SharedUserSetting* sharedUser,
            /* [in] */ IFile* codePath,
            /* [in] */ IFile* resourcePath,
            /* [in] */ const String& nativeLibraryPathString,
            /* [in] */ Int32 vc,
            /* [in] */ Int32 capFlags,
            /* [in] */ Boolean create,
            /* [in] */ Boolean add);

        CARAPI_(void) InsertCapsuleSettingLP(
            /* [in] */ CapsuleSetting* c,
            /* [in] */ CapsuleParser::Capsule* cap);

        CARAPI_(void) AddCapsuleSettingLP(
            /* [in] */ CapsuleSetting* c,
            /* [in] */ const String& name,
            /* [in] */ SharedUserSetting* sharedUser);

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
            /* [in] */ ArrayOf<Int32>* globalGids);

        CARAPI_(Int32) RemoveCapsuleLP(
            /* [in] */ const String& name);

        CARAPI_(void) ReplaceCapsuleLP(
            /* [in] */ const String& name,
            /* [in] */ CapsuleSetting* newp);

        CARAPI_(Boolean) AddUserIdLP(
            /* [in] */ Int32 uid,
            /* [in] */ IInterface* obj,
            /* [in] */ IInterface* name);

        CARAPI_(AutoPtr<IInterface>) GetUserIdLP(
            /* [in] */ Int32 uid);

        CARAPI_(HashSet<String>*) FindCapsulesWithFlag(
            /* [in] */ Int32 flag);

        CARAPI_(void) RemoveUserIdLP(
            /* [in] */ Int32 uid);

        CARAPI_(void) ReplaceUserIdLP(
            /* [in] */ Int32 uid,
            /* [in] */ IInterface* obj);

        CARAPI_(Int32) ReadInt(
            /* [in] */ IXmlPullParser* parser,
            /* [in] */ String ns,
            /* [in] */ String name,
            /* [in] */ Int32 defValue);

        CARAPI_(void) ReadPermissionsLP(
            /* [in] */ HashMap<String, BasePermission*>* out,
            /* [in] */ IXmlPullParser* parser);

        CARAPI_(void) ReadDisabledSysCapsuleLP(
            /* [in] */ IXmlPullParser* parser);

        CARAPI_(void) ReadCapsuleLP(
            /* [in] */ IXmlPullParser* parser);

        CARAPI_(void) ReadDisabledComponentsLP(
            /* [in] */ CapsuleSettingBase* capsuleSetting,
            /* [in] */ IXmlPullParser* parser);

        CARAPI_(void) ReadEnabledComponentsLP(
            /* [in] */ CapsuleSettingBase* capsuleSetting,
            /* [in] */ IXmlPullParser* parser);

        CARAPI_(void) ReadSharedUserLP(
            /* [in] */ IXmlPullParser* parser);

        CARAPI_(void) ReadGrantedPermissionsLP(
            /* [in] */ IXmlPullParser* parser,
            /* [in] */ HashSet<String>* outPerms);

        CARAPI_(void) ReadPreferredActivitiesLP(
            /* [in] */ IXmlPullParser* parser);

        CARAPI_(Int32) NewUserIdLP(
            /* [in] */ IInterface* obj);

    public:
        AutoPtr<IFile> mSettingsFilename;
        AutoPtr<IFile> mBackupSettingsFilename;
        AutoPtr<IFile> mCapsuleListFilename;
        HashMap<String, AutoPtr<CapsuleSetting> > mCapsules;
        Mutex mCapsulesLock;
        // List of replaced system applications
        HashMap<String, AutoPtr<CapsuleSetting> > mDisabledSysCapsules;

        // These are the last platform API version we were using for
        // the apps installed on internal and external storage.  It is
        // used to grant newer permissions one time during a system upgrade.
        Int32 mInternalSdkPlatform;
        Int32 mExternalSdkPlatform;

        // The user's preferred activities associated with particular intent
        // filters.
        SettingsIntentResolver* mPreferredActivities;

        HashMap<String, AutoPtr<SharedUserSetting> > mSharedUsers;
        Vector< AutoPtr<IInterface> > mUserIds;
        HashMap<Int32, AutoPtr<IInterface> > mOtherUserIds;

        // For reading/writing settings file.
        List< AutoPtr<ISignature> > mPastSignatures;

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
        HashMap<String, String> mRenamedCapsules;

        StringBuffer mReadMessages;

        List< AutoPtr<PendingCapsule> > mPendingCapsules;

        CCapsuleManagerService* mOwner;
    };

public:
    CCapsuleManagerService();

    CCapsuleManagerService(
        /* [in] */ IContext* context,
        /* [in] */ Boolean factoryTest);

    ~CCapsuleManagerService();

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

    CARAPI GetCapsuleUid(
        /* [in] */ const String& capsuleName,
        /* [out] */ Int32* uid);

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
        /* [out, callee] */ IObjectContainer** infos);

    CARAPI GetPermissionGroupInfo(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags,
        /* [out] */ IPermissionGroupInfo** info);

    CARAPI GetAllPermissionGroups(
        /* [in] */ Int32 flags,
        /* [out, callee] */ IObjectContainer** infos);

    CARAPI GetApplicationInfo(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 flags,
        /* [out] */ IApplicationInfo** appInfo);

    CARAPI FreeStorageAndNotify(
        /* [in] */ Int64 freeStorageSize,
        /* [in] */ ICapsuleDataObserver* observer);

    CARAPI_(void) HandleFreeStorageAndNotify(
        /* [in] */ Int64 freeStorageSize,
        /* [in] */ ICapsuleDataObserver* observer);

    CARAPI FreeStorage(
        /* [in] */ Int64 freeStorageSize,
        /* [in] */ IIntentSender* pi);

    CARAPI_(void) HandleFreeStorage(
        /* [in] */ Int64 freeStorageSize,
        /* [in] */ IIntentSender* pi);

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

    CARAPI CheckUidPermission(
        /* [in] */ const String& permName,
        /* [in] */ Int32 uid,
        /* [out] */ Int32* perm);

    CARAPI AddPermission(
        /* [in] */ IPermissionInfo* info,
        /* [out] */ Boolean* isAdded);

    CARAPI AddPermissionAsync(
        /* [in] */ IPermissionInfo* info,
        /* [out] */ Boolean* isAdded);

    CARAPI RemovePermission(
        /* [in] */ const String& name);

    CARAPI IsProtectedBroadcast(
        /* [in] */ const String& actionName,
        /* [out] */ Boolean* result);

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

    CARAPI ResolveIntent(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [out] */ IResolveInfo** resolveInfo);

    CARAPI QueryIntentActivities(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [out, callee] */ IObjectContainer** infos);

//    //todo: should be removed
//    CARAPI_(List<AutoPtr<IResolveInfo>*>*) QueryIntentActivities(
//        /* [in] */ IIntent* intent,
//        /* [in] */ const String& resolvedType,
//        /* [in] */ int flags);

    CARAPI QueryIntentActivityOptions(
        /* [in] */ IComponentName* caller,
        /* [in] */ ArrayOf<IIntent*>* specifics,
        /* [in] */ ArrayOf<String>* specificTypes,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** infos);

    CARAPI QueryIntentReceivers(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** receivers);

    CARAPI ResolveService(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [out] */ IResolveInfo** resolveInfo);

    CARAPI QueryIntentServices(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** services);

    CARAPI GetInstalledCapsules(
        /* [in] */ Int32 flags,
        /* [in] */ const String& lastRead,
        /* [out] */ IParceledListSlice** slice);

    CARAPI GetInstalledApplications(
        /* [in] */ Int32 flags,
        /* [in] */ const String& lastRead,
        /* [out] */ IParceledListSlice** slice);

    CARAPI GetPersistentApplications(
        /* [in] */ Int32 flags,
        /* [out, callee] */ IObjectContainer** infos);

    CARAPI ResolveContentProvider(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags,
        /* [out] */ IContentProviderInfo** info);

    CARAPI QuerySyncProviders(
        /* [in, out] */ IObjectContainer* outNames,
        /* [in, out] */ IObjectContainer* outInfo);

    CARAPI QueryContentProviders(
        /* [in] */ const String& processName,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** providers);

    CARAPI GetInstrumentationInfo(
        /* [in] */ IComponentName* name,
        /* [in] */ Int32 flags,
        /* [out] */ IInstrumentationInfo** instInfo);

    CARAPI QueryInstrumentation(
        /* [in] */ const String& targetCapsule,
        /* [in] */ Int32 flags,
        /* [out, callee] */ IObjectContainer** infos);

    CARAPI NextCapsuleToClean(
        /* [in] */ const String& lastCapsule,
        /* [out] */ String* nextCapsule);

    /* Called when a downloaded package installation has been confirmed by the user */
    CARAPI InstallCapsuleEx(
        /* [in] */ IUri* capsuleURI,
        /* [in] */ ICapsuleInstallObserver* observer,
        /* [in] */ Int32 flags);

    CARAPI InstallCapsule(
        /* [in] */ IUri* capsuleURI,
        /* [in] */ ICapsuleInstallObserver* observer,
        /* [in] */ Int32 flags,
        /* [in] */ const String& installerCapsuleName);

    //todo: temporary
    CARAPI InstallCapsuleEx2(
        /* [in] */ const String& path);

    CARAPI FinishCapsuleInstall(
        /* [in] */ Int32 token);

    CARAPI DeleteCapsule(
        /* [in] */ const String& capsuleName,
        /* [in] */ ICapsuleDeleteObserver* observer,
        /* [in] */ Int32 flags);

    CARAPI ClearApplicationUserData(
        /* [in] */ const String& capsuleName,
        /* [in] */ ICapsuleDataObserver* observer);

    CARAPI_(void) HandleClearApplicationUserData(
        /* [in] */ const String& capsuleName,
        /* [in] */ ICapsuleDataObserver* observer);

    CARAPI DeleteApplicationCacheFiles(
        /* [in] */ const String& capsuleName,
        /* [in] */ ICapsuleDataObserver* observer);

    CARAPI_(void) HandleDeleteApplicationCacheFiles(
        /* [in] */ const String& capsuleName,
        /* [in] */ ICapsuleDataObserver* observer);

    CARAPI GetCapsuleSizeInfo(
        /* [in] */ const String& capsuleName,
        /* [in] */ ICapsuleStatsObserver* observer);

    CARAPI_(void) HandleGetCapsuleSizeInfo(
        /* [in] */ const String& capsuleName,
        /* [in] */ ICapsuleStatsObserver* observer);

    CARAPI AddCapsuleToPreferred(
        /* [in] */ const String& capsuleName);

    CARAPI RemoveCapsuleFromPreferred(
        /* [in] */ const String& capsuleName);

    CARAPI GetPreferredCapsules(
        /* [in] */ Int32 flags,
        /* [out, callee] */ IObjectContainer** infos);

    CARAPI AddPreferredActivity(
        /* [in] */ IIntentFilter* filter,
        /* [in] */ Int32 match,
        /* [in] */ IObjectContainer* set, /*IComponentName*/
        /* [in] */ IComponentName* activity);

    CARAPI ReplacePreferredActivity(
        /* [in] */ IIntentFilter* filter,
        /* [in] */ Int32 match,
        /* [in] */ IObjectContainer* set, /*IComponentName*/
        /* [in] */ IComponentName* activity);

    CARAPI ClearCapsulePreferredActivities(
        /* [in] */ const String& capsuleName);

    CARAPI GetPreferredActivities(
        /* [in, out] */ IObjectContainer* outFilters,
        /* [in, out] */ IObjectContainer* outActivities,
        /* [in] */ const String& capsuleName,
        /* [out] */ Int32* count);

    CARAPI SetApplicationEnabledSetting(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 newState,
        /* [in] */ Int32 flags);

    CARAPI SetComponentEnabledSetting(
        /* [in] */ IComponentName* componentName,
        /* [in] */ Int32 newState,
        /* [in] */ Int32 flags);

    CARAPI GetInstallerCapsuleName(
        /* [in] */ const String& capsuleName,
        /* [out] */ String* name);

    CARAPI GetApplicationEnabledSetting(
        /* [in] */ const String& capsuleName,
        /* [out] */ Int32* setting);

    CARAPI GetComponentEnabledSetting(
        /* [in] */ IComponentName* componentName,
        /* [out] */ Int32* setting);

    CARAPI EnterSafeMode();

    CARAPI SystemReady();

    CARAPI IsSafeMode(
        /* [out] */ Boolean* isSafeMode);

    CARAPI HasSystemUidErrors(
        /* [out] */ Boolean* result);

    CARAPI UpdateExternalMediaStatus(
        /* [in] */ Boolean mediaStatus,
        /* [in] */ Boolean reportStatus);

    CARAPI_(void) HandleUpdateExternalMediaStatus(
        /* [in] */ Boolean mediaStatus,
        /* [in] */ Boolean reportStatus);

    CARAPI MoveCapsule(
        /* [in] */ const String& capsuleName,
        /* [in] */ ICapsuleMoveObserver* observer,
        /* [in] */ Int32 flags);

    CARAPI SetInstallLocation(
        /* [in] */ Int32 loc,
        /* [out] */ Boolean* isSetted);

    CARAPI GetInstallLocation(
        /* [out] */ Int32* loc);

    //todo: should be removed
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Boolean factoryTest);

    //todo: should be removed
    CARAPI_(void) ScanDir(
        /* [in] */ CString path);

    //todo: should be removed
    CARAPI_(CapsuleParser::Capsule*) ScanCapsule(
        /* [in] */ const String& capfile);

private:
    struct HashSd
    {
        size_t operator()(const SdInstallArgs* s) const
        {
            return (size_t)s;
        }
    };

    struct SdEq
    {
        Boolean operator()(const SdInstallArgs* x,
                           const SdInstallArgs* y) const
        {
            return x == y;
        }
    };

    CARAPI_(void) ScheduleWriteSettingsLocked();

    static CARAPI_(Boolean) InstallOnSd(
        /* [in] */ Int32 flags);

    static CARAPI_(void) SplitString(
        /* [in] */ const String& str,
        /* [in] */ Char32 sep,
        /* [out] */ ArrayOf<String>** res);

    CARAPI_(void) CleanupInstallFailedCapsule(
        /* [in] */ CapsuleSetting* cs);

    CARAPI_(void) ReadPermissions();

    CARAPI_(void) ReadPermissionsFromXml(
        /* [in] */ IFile* permFile);

    CARAPI_(void) ReadPermission(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ const String& name);

    static CARAPI_(ArrayOf<Int32>*) AppendInt(
        /* [in] */ ArrayOf<Int32>* cur,
        /* [in] */ Int32 val);

    static CARAPI_(ArrayOf<Int32>*) AppendInts(
        /* [in] */ ArrayOf<Int32>* cur,
        /* [in] */ ArrayOf<Int32>* add);

    static CARAPI_(ArrayOf<Int32>*) RemoveInt(
        /* [in] */ ArrayOf<Int32>* cur,
        /* [in] */ Int32 val);

    static CARAPI_(ArrayOf<Int32>*) RemoveInts(
        /* [in] */ ArrayOf<Int32>* cur,
        /* [in] */ ArrayOf<Int32>* rem);

    CARAPI_(AutoPtr<ICapsuleInfo>) GenerateCapsuleInfo(
        /* [in] */ CapsuleParser::Capsule* c,
        /* [in] */ Int32 flags);

    static CARAPI_(AutoPtr<IPermissionInfo>) GeneratePermissionInfo(
        /* [in] */ BasePermission* bp,
        /* [in] */ Int32 flags);

    CARAPI_(AutoPtr<IApplicationInfo>) GenerateApplicationInfoFromSettingsLP(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 flags);

    CARAPI_(AutoPtr<ICapsuleInfo>) GenerateCapsuleInfoFromSettingsLP(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 flags);

    CARAPI_(BasePermission*) FindPermissionTreeLP(
        /* [in] */ const String& permName);

    CARAPI CheckPermissionTreeLP(
        /* [in] */ const String& permName,
        /* [out] */ BasePermission** permission);

    static CARAPI_(Boolean) CompareStrings(
        /* [in] */ ICharSequence* s1,
        /* [in] */ ICharSequence* s2);

    static CARAPI_(Boolean) CompareStrings(
        /* [in] */ const String& s1,
        /* [in] */ const String& s2);

    static CARAPI_(Boolean) ComparePermissionInfos(
        /* [in] */ IPermissionInfo* pi1,
        /* [in] */ IPermissionInfo* pi2);

    CARAPI AddPermissionLocked(
        /* [in] */ IPermissionInfo* info,
        /* [in] */ Boolean async,
        /* [out] */ Boolean* isAdded);

    CARAPI_(Int32) CheckSignaturesLP(
        /* [in] */ ArrayOf< AutoPtr<ISignature> >* s1,
        /* [in] */ ArrayOf< AutoPtr<ISignature> >* s2);

    CARAPI ChooseBestActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ IObjectContainer* query,
        /* [out] */ IResolveInfo** resolveInfo);

    CARAPI_(AutoPtr<IResolveInfo>) FindPreferredActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ IObjectContainer* query,
        /* [in] */ Int32 priority);

    CARAPI_(Int32) GetContinuationPoint(
        /* [in] */ const ArrayOf<String>& keys,
        /* [in] */ const String& key);

    CARAPI ScanDirLI(
        /* [in] */ IFile* dir,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 scanMode,
        /* [in] */ Int64 currentTime);

    static CARAPI_(AutoPtr<IFile>) GetSettingsProblemFile();

    static CARAPI_(void) ReportSettingsProblem(
        /* [in] */ Int32 priority,
        /* [in] */ CString msg);

    CARAPI_(Boolean) CollectCertificatesLI(
        /* [in] */ CapsuleParser* cp,
        /* [in] */ CapsuleSetting* cs,
        /* [in] */ CapsuleParser::Capsule* cap,
        /* [in] */ IFile* srcFile,
        /* [in] */ Int32 parseFlags);

    CARAPI_(CapsuleParser::Capsule*) ScanCapsuleLI(
        /* [in] */ IFile* scanFile,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanMode,
        /* [in] */ Int64 currentTime);

    CARAPI_(void) SetApplicationInfoPaths(
        /* [in] */ CapsuleParser::Capsule* cap,
        /* [in] */ const String& destCodePath,
        /* [in] */ const String& destResPath);

    static CARAPI_(String) FixProcessName(
        /* [in] */ const String& defProcessName,
        /* [in] */ const String& processName,
        /* [in] */ Int32 uid);

    CARAPI_(Boolean) VerifySignaturesLP(
        /* [in] */ CapsuleSetting* capSetting,
        /* [in] */ CapsuleParser::Capsule* cap);

    static CARAPI_(Boolean) UseEncryptedFilesystemForCapsule(
        /* [in] */ CapsuleParser::Capsule* cap);

    CARAPI_(Boolean) VerifyCapsuleUpdate(
        /* [in] */ CapsuleSetting* oldCap,
        /* [in] */ CapsuleParser::Capsule* newCap);

    CARAPI_(AutoPtr<IFile>) GetDataPathForCapsule(
        /* [in] */ CapsuleParser::Capsule* cap);

    CARAPI_(CapsuleParser::Capsule*) ScanCapsuleLI(
        /* [in] */ CapsuleParser::Capsule* cap,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanMode,
        /* [in] */ Int64 currentTime);

    CARAPI_(void) KillApplication(
        /* [in] */ const String& capName,
        /* [in] */ Int32 uid);

    CARAPI_(AutoPtr<IFile>) GetNativeBinaryDirForCapsule(
        /* [in] */ CapsuleParser::Capsule* cap);

    CARAPI_(void) RemoveCapsuleLI(
        /* [in] */ CapsuleParser::Capsule* cap,
        /* [in] */ Boolean chatty);

    static CARAPI_(Boolean) IsCapsuleFilename(
        /* [in] */ const String& name);

    static CARAPI_(Boolean) HasPermission(
        /* [in] */ CapsuleParser::Capsule* capInfo,
        /* [in] */ const String& perm);

    CARAPI_(void) UpdatePermissionsLP(
        /* [in] */ const String& changingCap,
        /* [in] */ CapsuleParser::Capsule* capInfo,
        /* [in] */ Boolean grantPermissions,
        /* [in] */ Boolean replace,
        /* [in] */ Boolean replaceAll);

    CARAPI_(void) GrantPermissionsLP(
        /* [in] */ CapsuleParser::Capsule* cap,
        /* [in] */ Boolean replace);

    CARAPI_(void) SendCapsuleBroadcast(
        /* [in] */ const String& action,
        /* [in] */ const String& cap,
        /* [in] */ IBundle* extras,
        /* [in] */ IIntentReceiver* finishedReceiver);

    CARAPI_(void) ScheduleCapsuleCleaning(
        /* [in] */ const String& capsuleName);

    CARAPI_(void) StartCleaningCapsules();

    CARAPI_(void) ProcessPendingInstall(
        /* [in] */ InstallArgs* args,
        /* [in] */ Int32 currentStatus);

    CARAPI_(void) HandleProcessPendingInstall(
        /* [in] */ InstallArgs* args,
        /* [in] */ Int32 currentStatus);

    CARAPI_(InstallArgs*) CreateInstallArgs(
        /* [in] */ InstallParams* params);

    CARAPI_(InstallArgs*) CreateInstallArgs(
        /* [in] */ Int32 flags,
        /* [in] */ const String& fullCodePath,
        /* [in] */ const String& fullResourcePath,
        /* [in] */ const String& nativeLibraryPath);

    CARAPI_(InstallArgs*) CreateInstallArgs(
        /* [in] */ IUri* capsuleURI,
        /* [in] */ Int32 flags,
        /* [in] */ const String& capName,
        /* [in] */ String dataDir);

    static CARAPI_(String) GetNextCodePath(
        /* [in] */ const String& oldCodePath,
        /* [in] */ const String& prefix,
        /* [in] */ const String& suffix);

    // Utility method used to ignore ADD/REMOVE events
    // by directory observer.
    static CARAPI_(Boolean) IgnoreCodePath(
        /* [in] */ const String& fullPathStr);

    // Utility method that returns the relative package path with respect
    // to the installation directory. Like say for /data/data/com.test-1.apk
    // string com.test-1 is returned.
    static CARAPI_(String) GetCapName(
        /* [in] */ const String& codePath);

    /*
     * Install a non-existing package.
     */
    CARAPI_(void) InstallNewCapsuleLI(
        /* [in] */ CapsuleParser::Capsule* cap,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanMode,
        /* [in] */ const String& installerCapsuleName,
        /* [in] */ CapsuleInstalledInfo* res);

    CARAPI_(void) ReplaceCapsuleLI(
        /* [in] */ CapsuleParser::Capsule* cap,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanMode,
        /* [in] */ const String& installerCapsuleName,
        /* [in] */ CapsuleInstalledInfo* res);

    CARAPI_(void) ReplaceNonSystemCapsuleLI(
        /* [in] */ CapsuleParser::Capsule* deletedCapsule,
        /* [in] */ CapsuleParser::Capsule* cap,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanMode,
        /* [in] */ const String& installerCapsuleName,
        /* [in] */ CapsuleInstalledInfo* res);

    CARAPI_(void) ReplaceSystemCapsuleLI(
        /* [in] */ CapsuleParser::Capsule* deletedCapsule,
        /* [in] */ CapsuleParser::Capsule* cap,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanMode,
        /* [in] */ const String& installerCapsuleName,
        /* [in] */ CapsuleInstalledInfo* res);

    CARAPI_(void) UpdateSettingsLI(
        /* [in] */ CapsuleParser::Capsule* newCapsule,
        /* [in] */ const String& installerCapsuleName,
        /* [in] */ CapsuleInstalledInfo* res);

    CARAPI_(void) InstallCapsuleLI(
        /* [in] */ InstallArgs* args,
        /* [in] */ Boolean newInstall,
        /* [in] */ CapsuleInstalledInfo* res);

    CARAPI_(Int32) SetPermissionsLI(
        /* [in] */ CapsuleParser::Capsule* newCapsule);

    static CARAPI_(Boolean) IsForwardLocked(
        /* [in] */ CapsuleParser::Capsule* cap);

    static CARAPI_(Boolean) IsExternal(
        /* [in] */ CapsuleParser::Capsule* cap);

    static CARAPI_(Boolean) IsSystemApp(
        /* [in] */ CapsuleParser::Capsule* cap);

    static CARAPI_(Boolean) IsSystemApp(
        /* [in] */ IApplicationInfo* info);

    static CARAPI_(Boolean) IsUpdatedSystemApp(
        /* [in] */ CapsuleParser::Capsule* cap);

    CARAPI ExtractPublicFiles(
        /* [in] */ CapsuleParser::Capsule* newCapsule,
        /* [in] */ IFile* publicZipFile);

    static CARAPI_(void) CopyZipEntry(
        /* in */ IZipEntry* zipEntry,
        /* in */ IZipFile* inZipFile,
        /* in */ IZipOutputStream* outZipStream);

    CARAPI_(void) DeleteTempCapsuleFiles();

    CARAPI_(AutoPtr<IFile>) CreateTempCapsuleFile(
        /* [in] */ IFile* installDir);

    CARAPI_(void) HandleDeleteCapsule(
        /* [in] */ const String& capsuleName,
        /* [in] */ ICapsuleDeleteObserver* observer,
        /* [in] */ Int32 flags);

    CARAPI_(Boolean) DeleteCapsuleX(
        /* [in] */ const String& capsuleName,
        /* [in] */ Boolean sendBroadCast,
        /* [in] */ Boolean deleteCodeAndResources,
        /* [in] */ Int32 flags);

    CARAPI_(void) RemoveCapsuleDataLI(
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
        /* [in] */ const String& capsuleName,
        /* [in] */ Boolean deleteCodeAndResources,
        /* [in] */ Int32 flags,
        /* [in] */ CapsuleRemovedInfo* outInfo,
        /* [in] */ Boolean writeSettings);

    CARAPI_(Boolean) ClearApplicationUserDataLI(
        /* [in] */ const String& capsuleName);

    CARAPI_(Boolean) DeleteApplicationCacheFilesLI(
        /* [in] */ const String& capsuleName);

    CARAPI_(Boolean) GetCapsuleSizeInfoLI(
        /* [in] */ const String& capsuleName,
        /* [in] */ ICapsuleStats* stats);

    CARAPI_(Int32) GetUidTargetSdkVersionLockedLP(
        /* [in] */ Int32 uid);

    CARAPI_(Boolean) ClearCapsulePreferredActivitiesLP(
        /* [in] */ const String& capsuleName);

    CARAPI SetEnabledSetting(
        /* [in] */ const String& capsuleName,
        /* [in] */ const String& className,
        /* [in] */ Int32 newState,
        /* [in] */ Int32 flags);

    CARAPI_(void) SendCapsuleChangedBroadcast(
        /* [in] */ const String& capsuleName,
        /* [in] */ Boolean killFlag,
        /* [in] */ const List<String>& componentNames,
        /* [in] */ Int32 capsuleUid);

    CARAPI_(String) ArrayToString(
        /* [in] */ const ArrayOf<Int32>& array);

    CARAPI_(String) GetEncryptKey();

    static CARAPI_(String) GetTempContainerId();

    CARAPI UpdateExternalMediaStatusInner(
        /* [in] */ Boolean mediaStatus,
        /* [in] */ Boolean reportStatus);

    CARAPI_(void) SendResourcesChangedBroadcast(
        /* [in] */ Boolean mediaStatus,
        /* [in] */ const List<String>& capList,
        /* [in] */ const ArrayOf<Int32>* uidArr,
        /* [in] */ IIntentReceiver* finishedReceiver);

    CARAPI_(void) LoadMediaCapsules(
        /* [in] */ HashMap<SdInstallArgs*, String, HashSd, SdEq>& processCids,
        /* [in] */ const ArrayOf<Int32>& uidArr,
        /* [in] */ List<String>* removeCids);

    /*
     * Utility method to unload a list of specified containers
     */
    CARAPI_(void) UnloadAllContainers(
        /* [in] */ Set<SdInstallArgs*>& cidArgs);

    CARAPI UnloadMediaCapsules(
        /* [in] */ HashMap<SdInstallArgs*, String, HashSd, SdEq>& processCids,
        /* [in] */ const ArrayOf<Int32>& uidArr,
        /* [in] */ Boolean reportStatus);

    CARAPI_(void) ProcessPendingMove(
        /* [in] */ MoveParams* mp,
        /* [in] */ Int32 currentStatus);

    CARAPI_(void) HandleProcessPendingMove(
        /* [in] */ MoveParams* mp,
        /* [in] */ Int32 currentStatus);

    CARAPI SendMessage(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params);

    //from PackageHandler
    CARAPI_(Boolean) ConnectToService();

    CARAPI_(void) DisconnectService();

    CARAPI_(void) HandleInitCopy(
        /* [in] */ HandlerParams* params);

    CARAPI_(void) HandleMCSBound(
        /* [in] */ IMediaContainerService* mcs);

    CARAPI_(void) HandleMCSReconnect();

    CARAPI_(void) HandleMCSUnbind();

    CARAPI_(void) HandleMCSGiveUp();

    CARAPI_(void) HandleSendPendingBroadcast();

    CARAPI_(void) HandleStartCleaningCapsule(
        /* [in] */ const String& capsuleName);

    CARAPI_(void) HandlePostInstall(
        /* [in] */ Int32 token);

    CARAPI_(void) HandleUpdatedMediaStatus(
        /* [in] */ Boolean reportStatus,
        /* [in] */ Set<SdInstallArgs*>* keys);

    CARAPI_(void) HandleWriteSettings();


public:/*package*/
    static const Int32 SCAN_MONITOR = 1 << 0;
    static const Int32 SCAN_NO_DEX = 1 << 1;
    static const Int32 SCAN_FORCE_DEX = 1 << 2;
    static const Int32 SCAN_UPDATE_SIGNATURE = 1 << 3;
    static const Int32 SCAN_NEW_INSTALL = 1 << 4;
    static const Int32 SCAN_NO_PATHS = 1 << 5;
    static const Int32 SCAN_UPDATE_TIME = 1 << 6;

    static const Int32 REMOVE_CHATTY = 1 << 16;

    static CString DEFAULT_CONTAINER_CAPSULE;

    static const AutoPtr<IComponentName> DEFAULT_CONTAINER_COMPONENT;

    static CString mTempContainerPrefix;

//	    final HandlerThread mHandlerThread = new HandlerThread("PackageManager",
//	            Process.THREAD_PRIORITY_BACKGROUND);

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
    FileObserver* mFrameworkInstallObserver;

    // This is the object monitoring the system app dir.
    FileObserver* mSystemInstallObserver;

    // This is the object monitoring the system app dir.
    FileObserver* mVendorInstallObserver;

    // This is the object monitoring mAppInstallDir.
    FileObserver* mAppInstallObserver;

    // This is the object monitoring mDrmAppPrivateInstallDir.
    FileObserver* mDrmAppInstallObserver;

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

    AutoFree< ArrayOf<Int32> > mOutPermissions;

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
    HashMap<Int32, HashSet<String>* > mSystemPermissions;

    // These are the built-in shared libraries that were read from the
    // etc/permissions.xml file.
    HashMap<String, String> mSharedLibraries;

    // Temporary for building the final shared libraries for an .apk.
    AutoStringArray mTmpSharedLibraries;

    // These are the features this devices supports that were read from the
    // etc/permissions.xml file.
    HashMap<String, AutoPtr<IFeatureInfo> > mAvailableFeatures;

    // All available activities, for your resolving pleasure.
    ActivityIntentResolver* mActivities;

    // All available receivers, for your resolving pleasure.
    ActivityIntentResolver* mReceivers;

    // All available services, for your resolving pleasure.
    ServiceIntentResolver* mServices;

    // Keys are String (provider class name), values are Provider.
    HashMap<AutoPtr<IComponentName>, CapsuleParser::ContentProvider*> mProvidersByComponent;

    // Mapping from provider base names (first directory in content URI codePath)
    // to the provider information.
    HashMap<String, CapsuleParser::ContentProvider*> mContentProviders;

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

    AutoPtr<IApplicationInfo> mElastosApplication;
    AutoPtr<IActivityInfo> mResolveActivity;
    AutoPtr<IResolveInfo> mResolveInfo;
    AutoPtr<IComponentName> mResolveComponentName;
    AutoPtr<CapsuleParser::Capsule> mPlatformCapsule;

    // Set of pending broadcasts for aggregating enable/disable of components.
    HashMap< String, List<String>* > mPendingBroadcasts;

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

    static const Int32 WRITE_SETTINGS_DELAY = 10 * 1000;  // 10 seconds

    // Delay time in millisecs
    static const Int32 BROADCAST_DELAY = 10 * 1000;

    HashMap<Int32, PostInstallData*> mRunningInstalls;
    Int32 mNextInstallToken; // nonzero; will be wrapped back to 1 when ++ overflows

    // ------- apps on sdcard specific code -------
    static const Boolean DEBUG_SD_INSTALL = FALSE;

    static const Int32 MAX_CONTAINERS = 250;

private:
    //todo: should be removed
    friend class CapsuleHandler;

    static CString TAG;
    static const Boolean DEBUG_SETTINGS = FALSE;
    static const Boolean DEBUG_PREFERRED = FALSE;
    static const Boolean DEBUG_UPGRADE = FALSE;
    static const Boolean DEBUG_INSTALL = FALSE;

    static const Boolean MULTIPLE_APPLICATION_UIDS = TRUE;
    static const Int32 RADIO_UID = Process::PHONE_UID;
    static const Int32 LOG_UID = Process::LOG_UID;
    static const Int32 NFC_UID = Process::NFC_UID;
    static const Int32 FIRST_APPLICATION_UID = Process::FIRST_APPLICATION_UID;
    static const Int32 MAX_APPLICATION_UIDS = 1000;

    static const Boolean SHOW_INFO = FALSE;

    static const Boolean GET_CERTIFICATES = TRUE;

    static CString SYSTEM_PROPERTY_EFS_ENABLED;

    static const Int32 REMOVE_EVENTS =
        FileObserver_CLOSE_WRITE | FileObserver_DELETE | FileObserver_MOVED_FROM;
    static const Int32 ADD_EVENTS =
        FileObserver_CLOSE_WRITE /*| FileObserver_CREATE*/ | FileObserver_MOVED_TO;

    static const Int32 OBSERVER_EVENTS = REMOVE_EVENTS | ADD_EVENTS;
    // Suffix used during package installation when copying/moving
    // package apks to install directory.
    static CString INSTALL_CAPSULE_SUFFIX;

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

    static CString LIB_DIR_NAME;

    // Service Connection to remote media container service to copy
    // package uri's from external media onto secure containers
    // or internal storage.
    AutoPtr<IMediaContainerService> mContainerService;

    AutoPtr<DefaultContainerConnection> mDefContainerConn;

//	    private static final Comparator<ResolveInfo> mResolvePrioritySorter =
//	            new Comparator<ResolveInfo>() {
//
//	    private static final Comparator<ProviderInfo> mProviderInitOrderSorter =
//	            new Comparator<ProviderInfo>() {

    static CString SD_ENCRYPTION_KEYSTORE_NAME;
    static CString SD_ENCRYPTION_ALGORITHM;

    Boolean mMediaMounted;

    //todo: maybe should be removed
    static CString DEFAULT_RESOURCES_FILE_NAME;

    // one param means Message*
    AutoPtr<IApartment> mApartment;

    //from PackageHandler
    Boolean mBound;
    List< AutoPtr<HandlerParams> > mPendingInstalls;
};


#endif // __CCAPSULEMANAGER_H__
