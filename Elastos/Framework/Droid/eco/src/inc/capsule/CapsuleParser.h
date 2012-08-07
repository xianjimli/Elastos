
#ifndef __CAPSULEMANIFESTPARSER_H__
#define __CAPSULEMANIFESTPARSER_H__

#include "ext/frameworkdef.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

#include "utils/CBundle.h"
#include "content/CComponentName.h"
#include "content/CIntentFilter.h"
#include "content/CPermissionInfo.h"
#include "content/Signature.h"
#include "content/CConfigurationInfo.h"
#include "content/CFeatureInfo.h"
#include "capsule/CCapsuleInfo.h"
#include "capsule/CActivityInfo.h"
#include "capsule/CServiceInfo.h"
#include "capsule/CContentProviderInfo.h"
#include "content/CApplicationInfo.h"
#include "content/CInstrumentationInfo.h"
#include "content/CPermissionGroupInfo.h"
#include "os/Build.h"

using namespace Elastos;

class CapsuleParser
{
public:
    class NewPermissionInfo {
        public:
            NewPermissionInfo(
                /* [in] */ const String& name,
                /* [in] */ Int32 sdkVersion,
                /* [in] */ Int32 fileVersion)
            {
                mName = name;
                mSdkVersion = sdkVersion;
                mFileVersion = fileVersion;
            }

            NewPermissionInfo(
                /* [in] */ const char* name,
                /* [in] */ Int32 sdkVersion,
                /* [in] */ Int32 fileVersion)
            {
                mName = name;
                mSdkVersion = sdkVersion;
                mFileVersion = fileVersion;
            }

        public:
            String mName;
            Int32 mSdkVersion;
            Int32 mFileVersion;
    };

    class Capsule;
    class ParseCapsuleItemArgs;

    template <typename II>
    class Component
    {
    public:
        Component()
            : mOwner(NULL)
            , mIntents(NULL) {}

        Component(
            /* [in] */ Capsule* owner)
            : mOwner(owner)
            , mIntents(NULL) {}

        Component(
            /* [in] */ Component<II>* clone)
        {
            mOwner = clone->mOwner;
            //todo: mIntents should be copied deeply;
            mIntents = clone->mIntents;
            mClassName = clone->mClassName;
            mComponentName = clone->mComponentName;
            mComponentShortName = clone->mComponentShortName;
        }

        Component(
            /* [in] */ ParseCapsuleItemArgs* args,
            /* [in, out] */ CapsuleItemInfo* outInfo);

        virtual ~Component()
        {
            if (mIntents != NULL) {
                mIntents->Clear();
                mIntents = NULL;
            }
        }

        virtual CARAPI GetComponentName(
            /* [out] */ IComponentName** component);

        virtual CARAPI_(void) SetCapsuleName(
            /* [in] */ const String& capsuleName);

    public:
        Capsule* mOwner;
        List<II*>* mIntents;
        String mClassName;
        AutoPtr<CBundle> mMetaData;
        AutoPtr<IComponentName> mComponentName;
        String mComponentShortName;
    };

    class IntentInfo;
    class PermissionGroup;

    class Permission : public Component<IntentInfo>
    {
    public:
        Permission(
            /* [in] */ Capsule* _owner);

        Permission(
            /* [in] */ Capsule* _owner,
            /* [in] */ IPermissionInfo* _info);

        ~Permission();

        CARAPI_(void) SetCapsuleName(
            /* [in] */ const String& capsuleName);

        CARAPI_(String) GetDescription();

    public:
        AutoPtr<CPermissionInfo> mInfo;
        Boolean mTree;
        PermissionGroup* mGroup;
    };

    class PermissionGroup : public Component<IntentInfo>
    {
    public:
        PermissionGroup(
            /* [in] */ Capsule* owner);

        PermissionGroup(
            /* [in] */ Capsule* owner,
            /* [in] */ CPermissionGroupInfo* info);

        ~PermissionGroup();

        CARAPI_(void) SetCapsuleName(
            /* [in] */ const String& capsuleName);

        CARAPI GetDescription(
            /* [out] */ String* des);

    public:
        AutoPtr<CPermissionGroupInfo> mInfo;
    };

    class IntentInfo
    {
    public:
        IntentInfo();

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

        CARAPI AddCategory(
            /* [in] */ const String& category);

        CARAPI AddDataType(
            /* [in] */ const String& type);

        CARAPI AddDataScheme(
            /* [in] */ const String& scheme);

        CARAPI AddDataAuthority(
            /* [in] */ const String& host,
            /* [in] */ const String& port);

        CARAPI AddDataPath(
            /* [in] */ const String& path,
            /* [in] */ Int32 type);

        CARAPI HasCategory(
            /* [in] */ const String& category,
            /* [out] */ Boolean* hasCategory);

    public:
        AutoPtr<CIntentFilter> mFilter;
        Boolean mHasDefault;
        Int32 mLabelRes;
        AutoPtr<ICharSequence> mNonLocalizedLabel;
        Int32 mIcon;
        Int32 mLogo;
    };

    class Activity;

    class ActivityIntentInfo : public IntentInfo
    {
    public:
        ActivityIntentInfo(
            /* [in] */ Activity* activity);

    public:
        Activity* mActivity;
    };

    class Service;

    class ServiceIntentInfo : public IntentInfo
    {
    public:
        ServiceIntentInfo(
            /* [in] */ Service* service) {}

    public:
        Service* mService;
    };

    class ParseComponentArgs;

    class Activity : public Component<ActivityIntentInfo>
    {
    public:
        Activity(
            /* [in] */ ParseComponentArgs* args,
            /* [in] */ CActivityInfo* info)
            : Component<ActivityIntentInfo>(args, info)
        {
            mInfo = info;
            mInfo->mApplicationInfo = args->mOwner->mApplicationInfo;
        }

    public:
        AutoPtr<CActivityInfo> mInfo;
    };

    class Service : public Component<ServiceIntentInfo>
    {
    public:
        Service(
            /* [in] */ ParseComponentArgs* args,
            /* [in] */ CServiceInfo* info)
            : Component<ServiceIntentInfo>(args, info)
        {
            mInfo = info;
            mInfo->mApplicationInfo = args->mOwner->mApplicationInfo;
        }

    public:
        AutoPtr<CServiceInfo> mInfo;
    };

    class ContentProvider : public Component<IntentInfo>
    {
    public:
        ContentProvider() : mSyncable(FALSE) {}

        ContentProvider(
            /* [in] */ ContentProvider* existingProvider)
            : Component<IntentInfo>(existingProvider)
        {
            this->mInfo = existingProvider->mInfo;
            this->mSyncable = existingProvider->mSyncable;
        }

        ContentProvider(
            /* [in] */ ParseComponentArgs* args,
            /* [in] */ CContentProviderInfo* info)
            : Component<IntentInfo>(args, info)
            , mSyncable(FALSE)
        {
            mInfo = info;
            info->mApplicationInfo = args->mOwner->mApplicationInfo;
        }

    public:
        AutoPtr<CContentProviderInfo> mInfo;
        Boolean mSyncable;
    };

    class Instrumentation : public Component<IntentInfo>
    {
    public:
        Instrumentation(
            /* [in] */ ParseCapsuleItemArgs* args,
            /* [in] */ CInstrumentationInfo* info)
            : Component<IntentInfo>(args, info)
            , mInfo(info)
        {
        }

    public:
        AutoPtr<CInstrumentationInfo> mInfo;
    };

    class Capsule;

    class ParseCapsuleItemArgs
    {
    public:
        ParseCapsuleItemArgs(
            /* [in] */ Capsule* owner,
            /* [in] */ ArrayOf<String>* outError,
            /* [in] */ Int32 nameRes,
            /* [in] */ Int32 labelRes,
            /* [in] */ Int32 iconRes,
            /* [in] */ Int32 logoRes)
            : mOwner(owner)
            , mOutError(outError)
            , mNameRes(nameRes)
            , mLabelRes(labelRes)
            , mIconRes(iconRes)
            , mLogoRes(logoRes)
        {
            //todo: Can outError be assigned to mOutError directly?
        }

        virtual ~ParseCapsuleItemArgs()
        {
            //todo: should free mOutError?
        }

    public:
        Capsule* mOwner;
        ArrayOf<String>* mOutError;
        Int32 mNameRes;
        Int32 mLabelRes;
        Int32 mIconRes;
        Int32 mLogoRes;

        String mTag;
        AutoPtr<ITypedArray> mSa;
    };

    class ParseComponentArgs : public ParseCapsuleItemArgs
    {
    public:
        ParseComponentArgs(
            /* [in] */ CapsuleParser::Capsule* owner,
            /* [in] */ ArrayOf<String>* outError,
            /* [in] */ int nameRes,
            /* [in] */ int labelRes,
            /* [in] */ int iconRes,
            /* [in] */ int logoRes,
            /* [in] */ ArrayOf<String>* sepProcesses,
            /* [in] */ int processRes,
            /* [in] */ int descriptionRes,
            /* [in] */ int enabledRes)
            : ParseCapsuleItemArgs(owner,
                    outError, nameRes, labelRes, iconRes, logoRes)
            , mSepProcesses(sepProcesses)
            , mProcessRes(processRes)
            , mDescriptionRes(descriptionRes)
            , mEnabledRes(enabledRes)
            , mFlags(0)
        {
            //todo: Can outError be assigned to mOutError directly and sepProcesses?
        }

        ~ParseComponentArgs()
        {
            //todo: should free mOutError and sepProcesses?
        }

    public:
        ArrayOf<String>* mSepProcesses;
        Int32 mProcessRes;
        Int32 mDescriptionRes;
        Int32 mEnabledRes;
        Int32 mFlags;
    };

    class Capsule : public ElRefBase, public IInterface
    {
    public:
        Capsule()
            : mCapsuleName(NULL)
            , mProtectedBroadcasts(NULL)
            , mUsesLibraries(NULL)
            , mUsesOptionalLibraries(NULL)
            , mOriginalCapsules(NULL)
            , mAdoptPermissions(NULL)
            , mVersionCode(0)
            , mSharedUserLabel(0)
            , mPreferredOrder(0)
            , mSetEnabled(CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT)
            , mExtras(NULL)
            , mOperationPending(FALSE)
            , mInstallLocation(0)
        {
            AutoPtr<CApplicationInfo> ainfo;
            CApplicationInfo::NewByFriend((CApplicationInfo**)&ainfo);
            mApplicationInfo = ainfo;
        }

        Capsule(
            /* [in] */ const String& name)
            : mCapsuleName(name)
            , mProtectedBroadcasts(NULL)
            , mUsesLibraries(NULL)
            , mUsesOptionalLibraries(NULL)
            , mOriginalCapsules(NULL)
            , mAdoptPermissions(NULL)
            , mVersionCode(0)
            , mSharedUserLabel(0)
            , mPreferredOrder(0)
            , mSetEnabled(CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT)
            , mExtras(NULL)
            , mOperationPending(FALSE)
            , mInstallLocation(0)
        {}

        ~Capsule()
        {
            if (mProtectedBroadcasts != NULL) {
                mProtectedBroadcasts->Clear();
                delete mProtectedBroadcasts;
            }
            if (mUsesLibraries != NULL) {
                mUsesLibraries->Clear();
                delete mUsesLibraries;
            }
            if (mUsesOptionalLibraries != NULL) {
                mUsesOptionalLibraries->Clear();
                delete mUsesOptionalLibraries;
            }
            if (mOriginalCapsules != NULL) {
                mOriginalCapsules->Clear();
                delete mOriginalCapsules;
            }
            if (mAdoptPermissions != NULL) {
                mAdoptPermissions->Clear();
                delete mAdoptPermissions;
            }
        }

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI_(void) SetCapsuleName(
            /* [in] */ const String& newName);

    public:
        String mCapsuleName;

        AutoPtr<CApplicationInfo> mApplicationInfo;

        List<Permission*> mPermissions;
        List<PermissionGroup*> mPermissionGroups;
        List<Activity*> mActivities;
        List<Activity*> mReceivers;
        List<ContentProvider*> mContentProviders;
        List<Service*> mServices;
        List<Instrumentation*> mInstrumentation;

        List<String> mRequestedPermissions;

        List<String>* mProtectedBroadcasts;

        List<String>* mUsesLibraries;
        List<String>* mUsesOptionalLibraries;
        AutoFree< ArrayOf<String> > mUsesLibraryFiles;

        List<String>* mOriginalCapsules;
        String mRealCapsule;
        List<String>* mAdoptPermissions;

        // We store the application meta-data independently to avoid multiple unwanted references
        AutoPtr<CBundle> mAppMetaData;

        // If this is a 3rd party app, this is the path of the zip file.
        String mPath;

        // The version code declared for this package.
        Int32 mVersionCode;

        // The version name declared for this package.
        String mVersionName;

        // The shared user id that this package wants to use.
        String mSharedUserId;

        // The shared user label that this package wants to use.
        Int32 mSharedUserLabel;

        // Signatures that were read from the package.
        AutoFree< ArrayOf< AutoPtr<ISignature> > > mSignatures;

        // For use by package manager service for quick lookup of
        // preferred up order.
        Int32 mPreferredOrder;

        // For use by the package manager to keep track of the path to the
        // file an app came from.
        String mScanPath;

        // For use by package manager to keep track of where it has done dexopt.
//	        public boolean mDidDexOpt;

        // User set enabled state.
        Int32 mSetEnabled;

        // Additional data supplied by callers.
        AutoPtr<IInterface> mExtras;

        // Whether an operation is currently pending on this package
        Boolean mOperationPending;

        /*
         *  Applications hardware preferences
         */
        List< AutoPtr<CConfigurationInfo> > mConfigPreferences;

        /*
         *  Applications requested features
         */
        List< AutoPtr<CFeatureInfo> > mReqFeatures;

        Int32 mInstallLocation;
    };

public:
    CapsuleParser();

    CapsuleParser(
        /* [in] */ const String& archiveSourcePath);

    ~CapsuleParser();

    CARAPI_(void) SetSeparateProcesses(
        /* [in] */ const ArrayOf<String>& procs);

    /**
     * Generate and return the {@link PackageInfo} for a parsed package.
     *
     * @param p the parsed package.
     * @param flags indicating which optional information is included.
     */
    static CARAPI_(AutoPtr<CCapsuleInfo>) GenerateCapsuleInfo(
        /* [in] */ Capsule* c,
        /* [in] */ const ArrayOf<Int32>* gids,
        /* [in] */ Int32 flags,
        /* [in] */ Int64 firstInstallTime,
        /* [in] */ Int64 lastUpdateTime);

    CARAPI_(Int32) GetParseError();

    CARAPI_(CapsuleParser::Capsule*) ParseCapsule(
        /* [in] */ IFile* sourceFile,
        /* [in] */ const String& destCodePath,
        /* [in] */ IDisplayMetrics* metrics,
        /* [in] */ Int32 flags);

    CARAPI ParseCapsule(
        /* [in] */ const String& capPath,
        /* [in] */ CapsuleParser::Capsule* capsule,
        /* [out] */ String* errMsg);

    static CARAPI_(Boolean) CopyNeeded(
        /* [in] */ Int32 flags,
        /* [in] */ CapsuleParser::Capsule* capsule,
        /* [in] */ CBundle* metaData);

    static CARAPI_(AutoPtr<CApplicationInfo>) GenerateApplicationInfo(
        /* [in] */ Capsule* capsule,
        /* [in] */ Int32 flags);

    static CARAPI_(AutoPtr<CPermissionInfo>) GeneratePermissionInfo(
        /* [in] */ Permission* p,
        /* [in] */ Int32 flags);

    static CARAPI_(AutoPtr<CPermissionGroupInfo>) GeneratePermissionGroupInfo(
        /* [in] */ PermissionGroup* pg,
        /* [in] */ Int32 flags);

    static CARAPI_(AutoPtr<CActivityInfo>) GenerateActivityInfo(
        /* [in] */ Activity* activity,
        /* [in] */ Int32 flags);

    static CARAPI_(AutoPtr<CServiceInfo>) GenerateServiceInfo(
        /* [in] */ Service* service,
        /* [in] */ Int32 flags);

    static CARAPI_(AutoPtr<CContentProviderInfo>) GenerateContentProviderInfo(
        /* [in] */ ContentProvider* provider,
        /* [in] */ Int32 flags);

    static CARAPI_(AutoPtr<CInstrumentationInfo>) GenerateInstrumentationInfo(
        /* [in] */ CapsuleParser::Instrumentation* i,
        /* [in] */ Int32 flags);

    static CARAPI_(void) SetCompatibilityModeEnabled(
        /* [in] */ Boolean compatibilityModeEnabled);

    CARAPI_(Boolean) CollectCertificates(
        /* [in] */ Capsule* cap,
        /* [in] */ Int32 flags);

private:
    static CARAPI_(Boolean) IsCapsuleFilename(
        /* [in] */ const String& name);

    static CARAPI_(void) ValidateName(
        /* [in] */ const String& name,
        /* [in] */ Boolean requiresSeparator,
        /* [out] */ String* error);

    CARAPI BuildClassName(
        /* [in] */ const String& cap,
        /* [in] */ ICharSequence* clsSeq,
        /* [in] */ ArrayOf<String>* outError,
        /* [out] */ String* name);

    CARAPI BuildProcessName(
        /* [in] */ const String& cap,
        /* [in] */ const String& defProc,
        /* [in] */ ICharSequence* procSeq,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<String>* separateProcesses,
        /* [in] */ ArrayOf<String>* outError,
        /* [out] */ String* name);

    CARAPI BuildTaskAffinityName(
        /* [in] */ const String& cap,
        /* [in] */ const String& defProc,
        /* [in] */ ICharSequence* procSeq,
        /* [in] */ ArrayOf<String>* outError,
        /* [out] */ String* name);

    static CARAPI ParseCapsuleName(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<String>* outError,
        /* [out] */ String* name);

    CARAPI ParseCapsule(
        /* [in] */ CapsuleParser::Capsule* capsule,
        /* [in] */ const String& capPath,
        /* [in] */ const String& destCodePath,
        /* [in] */ IDisplayMetrics* metrics,
        /* [in] */ Int32 flags);

    CARAPI ParseCapsule(
        /* [in] */ CapsuleParser::Capsule* capsule,
        /* [in] */ IResources* res,
        /* [in] */ IXmlResourceParser* parser,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<String>* outError);

    CARAPI_(PermissionGroup*) ParsePermissionGroup(
        /* [in] */ Capsule* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ ArrayOf<String>* outError);

    CARAPI_(Permission*) ParsePermission(
        /* [in] */ Capsule* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ ArrayOf<String>* outError);

    CARAPI_(Permission*) ParsePermissionTree(
        /* [in] */ Capsule* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ ArrayOf<String>* outError);

    CARAPI_(Instrumentation*) ParseInstrumentation(
        /* [in] */ Capsule* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ ArrayOf<String>* outError);

    CARAPI_(Boolean) ParseCapsuleItemInfo(
        /* [in] */ Capsule* owner,
        /* [in] */ CapsuleItemInfo* outInfo,
        /* [in] */ ArrayOf<String>* outError,
        /* [in] */ const char* tag,
        /* [in] */ ITypedArray* sa,
        /* [in] */ Int32 nameRes,
        /* [in] */ Int32 labelRes,
        /* [in] */ Int32 iconRes,
        /* [in] */ Int32 logoRes);

    CARAPI_(Boolean) ParseApplication(
        /* [in] */ CapsuleParser::Capsule* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<String>* outError);

    CARAPI ParseActivity(
        /* [in] */ CapsuleParser::Capsule* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<String>* outError,
        /* [in] */ Boolean receiver,
        /* [out] */ Activity** activity);

    CARAPI ParseActivityAlias(
        /* [in] */ Capsule* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<String>* outError,
        /* [out] */ Activity** alias);

    CARAPI ParseContentProvider(
        /* [in] */ Capsule* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<String>* outError,
        /* [out] */ ContentProvider** provider);

    CARAPI_(Boolean) ParseContentProviderTags(
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ ContentProvider* outInfo,
        /* [in] */ ArrayOf<String>* outError);

    CARAPI ParseService(
        /* [in] */ Capsule* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<String>* outError,
        /* [out] */ Service** service);

    template <typename T>
    CARAPI_(Boolean) ParseAllMetaData(
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ const char* tag,
        /* [in] */ Component<T>* outInfo,
        /* [in] */ ArrayOf<String>* outError);

    CARAPI_(AutoPtr<CBundle>) ParseMetaData(
    	/* [in] */ IResources* res,
    	/* [in] */ IXmlPullParser* parser,
    	/* [in] */ IAttributeSet* attrs,
    	/* [in] */ CBundle* data,
    	/* [in] */ ArrayOf<String>* outError);

    CARAPI_(Boolean) ParseIntent(
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [in] */ IntentInfo* outInfo,
        /* [in] */ ArrayOf<String>* outError,
        /* [in] */ Boolean isActivity);

public:
    static const Int32 PARSE_IS_SYSTEM = 1<<0;
    static const Int32 PARSE_CHATTY = 1<<1;
    static const Int32 PARSE_MUST_BE_APK = 1<<2;
    static const Int32 PARSE_IGNORE_PROCESSES = 1<<3;
    static const Int32 PARSE_FORWARD_LOCK = 1<<4;
    static const Int32 PARSE_ON_SDCARD = 1<<5;
    static const Int32 PARSE_IS_SYSTEM_DIR = 1<<6;

    /**
     * List of new permissions that have been added since 1.0.
     * NOTE: These must be declared in SDK version order, with permissions
     * added to older SDKs appearing before those added to newer SDKs.
     * @hide
     */
    static const NewPermissionInfo* NEW_PERMISSIONS[];

    static Int32 GetNewPermissionsLength();

private:
    String mArchiveSourcePath;

    AutoFree< ArrayOf<String> > mSeparateProcesses;

    static const Int32 SDK_VERSION;
    static const String SDK_CODENAME;

    Int32 mParseError;

    static Boolean sCompatibilityModeEnabled;

    static const Int32 PARSE_DEFAULT_INSTALL_LOCATION = CCapsuleInfo::INSTALL_LOCATION_UNSPECIFIED;

    ParseCapsuleItemArgs* mParseInstrumentationArgs;
    ParseComponentArgs* mParseActivityArgs;
    ParseComponentArgs* mParseActivityAliasArgs;
    ParseComponentArgs* mParseServiceArgs;
    ParseComponentArgs* mParseProviderArgs;

    /** If set to true, we will only allow package files that exactly match
     *  the DTD.  Otherwise, we try to get as much from the package as we
     *  can without failing.  This should normally be set to false, to
     *  support extensions to the DTD in future versions. */
    static const Boolean RIGID_PARSER = FALSE;

    static const char* TAG;

    static const char* ANDROID_RESOURCES;
};

#endif //__CAPSULEMANIFESTPARSER_H__
