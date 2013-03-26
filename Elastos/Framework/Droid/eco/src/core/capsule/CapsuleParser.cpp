
#include "ext/frameworkext.h"
#include "capsule/CapsuleParser.h"
#ifdef _FRAMEWORK_CORE
#include "capsule/CActivityInfo.h"
#include "content/CAssetManager.h"
#include "content/CResources.h"
#include "content/CInstrumentationInfo.h"
#include "os/CPatternMatcher.h"
#include "utils/CTypedValue.h"
#endif
#include "content/cm/CapsuleManager.h"
#include "os/Build.h"
#include "utils/AutoStringArray.h"
#include "utils/XmlUtils.h"
#include "utils/ListUtils.h"
#include "utils/Config.h"
#include <elastos/Algorithm.h>
#include <Logger.h>
#include <Slogger.h>
#include <StringBuffer.h>
#include <stdlib.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

///////////////////////////////////////////////////////////////////////////////
// CapsuleParser::Component<II>

template <typename II>
CapsuleParser::Component<II>::Component(
    /* [in] */ ParseCapsuleItemArgs* args,
    /* [in, out] */ ICapsuleItemInfo* outInfo)
{
    ECode ec = NOERROR;
    mOwner = args->mOwner;
    mIntents = new List<II*>();
    ec = args->mSa->GetNonConfigurationString(args->mNameRes, 0, &mClassName);
    if (FAILED(ec)) {
        (*args->mOutError)[0] = (const char*)(StringBuffer(args->mTag)
                + " does not specify android:name");
        return;
    }

//        outInfo.name
//            = buildClassName(owner.applicationInfo.packageName, name, args.outError);
    outInfo->SetName(mClassName);
//        if (outInfo.name == null) {
//            className = null;
//            args.outError[0] = args.tag + " does not have valid android:name";
//            return;
//        }
//
//        className = outInfo.name;
//
//        int iconVal = args.sa.getResourceId(args.iconRes, 0);
//        if (iconVal != 0) {
//            outInfo.icon = iconVal;
//            outInfo.nonLocalizedLabel = null;
//        }
//
//        TypedValue v = args.sa.peekValue(args.labelRes);
//        if (v != null && (outInfo.labelRes=v.resourceId) == 0) {
//            outInfo.nonLocalizedLabel = v.coerceToString();
//        }
//
//        outInfo.packageName = owner.packageName;
}

template <typename II>
ECode CapsuleParser::Component<II>::GetComponentName(
    /* [out] */ IComponentName** component)
{
    if (component == NULL) return E_INVALID_ARGUMENT;

    if (mComponentName != NULL) {
        *component = mComponentName;
        (*component)->AddRef();
        return NOERROR;
    }
    if (!mClassName.IsNull()) {
        String cname;
        mOwner->mApplicationInfo->GetCapsuleName(&cname);
        CComponentName::New(cname, mClassName,
                (IComponentName**)&mComponentName);
    }
    *component = mComponentName;
    if (*component) (*component)->AddRef();
    return NOERROR;
}

template <typename II>
void CapsuleParser::Component<II>::SetCapsuleName(
    /* [in] */ const String& capsuleName)
{
    mComponentName = NULL;
    mComponentShortName = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// CapsuleParser::Permission

CapsuleParser::Permission::Permission(
    /* [in] */ Capsule* _owner)
    : mTree(FALSE)
    , mGroup(NULL)
{
    // E_NOT_IMPLEMENTED
}

CapsuleParser::Permission::Permission(
    /* [in] */ Capsule* _owner,
    /* [in] */ IPermissionInfo* _info)
    : mTree(FALSE)
    , mGroup(NULL)
{
    // E_NOT_IMPLEMENTED
}

CapsuleParser::Permission::~Permission()
{
}

void CapsuleParser::Permission::SetCapsuleName(
    /* [in] */ const String& capsuleName)
{
    // E_NOT_IMPLEMENTED
}

String CapsuleParser::Permission::GetDescription()
{
    return String(NULL); // E_NOT_IMPLEMENTED
}

///////////////////////////////////////////////////////////////////////////////
// CapsuleParser::PermissionGroup
CapsuleParser::PermissionGroup::PermissionGroup(
    /* [in] */ Capsule* owner)
    : Component<IntentInfo>(owner)
{
    CPermissionGroupInfo::New((IPermissionGroupInfo**)&mInfo);
}

CapsuleParser::PermissionGroup::PermissionGroup(
    /* [in] */ Capsule* owner,
    /* [in] */ IPermissionGroupInfo* info)
    : Component<IntentInfo>(owner)
    , mInfo(info)
{
}

CapsuleParser::PermissionGroup::~PermissionGroup()
{
}

void CapsuleParser::PermissionGroup::SetCapsuleName(
    /* [in] */ const String& capsuleName)
{
    Component<IntentInfo>::SetCapsuleName(capsuleName);
    mInfo->SetCapsuleName(capsuleName);
}

ECode CapsuleParser::PermissionGroup::GetDescription(
    /* [out] */ String* des)
{
    assert(des);
    String name;
    mInfo->GetName(&name);
    *des = (const char*)(StringBuffer("PermissionGroup{")
            + this /*Integer::toHexString(System->IdentityHashCode(this))*/
            + " " + (const char*)name + "}");
    return NOERROR;
}

///////////////////////////////////////////////////////////////////////////////
// CapsuleParser::IntentInfo

CapsuleParser::IntentInfo::IntentInfo()
{
    assert(SUCCEEDED(CIntentFilter::New(
            (IIntentFilter**)&mFilter)));
}

ECode CapsuleParser::IntentInfo::SetPriority(
    /* [in] */ Int32 priority)
{
    return mFilter->SetPriority(priority);
}

ECode CapsuleParser::IntentInfo::GetPriority(
    /* [out] */ Int32* priority)
{
    return mFilter->GetPriority(priority);
}

ECode CapsuleParser::IntentInfo::AddAction(
    /* [in] */ const String& action)
{
    return mFilter->AddAction(action);
}

ECode CapsuleParser::IntentInfo::CountActions(
    /* [out] */ Int32 *count)
{
    return mFilter->CountActions(count);
}

ECode CapsuleParser::IntentInfo::GetAction(
    /* [in] */ Int32 index,
    /* [out] */ String *action)
{
    return mFilter->GetAction(index, action);
}

ECode CapsuleParser::IntentInfo::AddCategory(
    /* [in] */ const String& category)
{
    return mFilter->AddCategory(category);
}

ECode CapsuleParser::IntentInfo::AddDataType(
    /* [in] */ const String& type)
{
    return mFilter->AddDataType(type);
}

ECode CapsuleParser::IntentInfo::AddDataScheme(
    /* [in] */ const String& scheme)
{
    return mFilter->AddDataScheme(scheme);
}

ECode CapsuleParser::IntentInfo::AddDataAuthority(
    /* [in] */ const String& host,
    /* [in] */ const String& port)
{
    return mFilter->AddDataAuthority(host, port);
}

ECode CapsuleParser::IntentInfo::AddDataPath(
    /* [in] */ const String& path,
    /* [in] */ Int32 type)
{
    return mFilter->AddDataPath(path, type);
}

ECode CapsuleParser::IntentInfo::HasCategory(
    /* [in] */ const String& category,
    /* [out] */ Boolean* hasCategory)
{
    return mFilter->HasCategory(category, hasCategory);
}

CapsuleParser::ActivityIntentInfo::ActivityIntentInfo(
    /* [in] */ CapsuleParser::Activity* activity) :
    mActivity(activity)
{}

CapsuleParser::Activity::Activity(
    /* [in] */ ParseComponentArgs* args,
    /* [in] */ IActivityInfo* info)
    : Component<ActivityIntentInfo>(args, info)
{
    mInfo = info;
    mInfo->SetApplicationInfo(args->mOwner->mApplicationInfo);
}

CapsuleParser::Service::Service(
    /* [in] */ ParseComponentArgs* args,
    /* [in] */ IServiceInfo* info)
    : Component<ServiceIntentInfo>(args, info)
{
    mInfo = info;
    mInfo->SetApplicationInfo(args->mOwner->mApplicationInfo);
}

void CapsuleParser::Service::SetCapsuleName(
    /* [in] */ const String& capName)
{
    assert(mInfo != NULL);
    Component<ServiceIntentInfo>::SetCapsuleName(capName);
    mInfo->SetCapsuleName(capName);
}

CapsuleParser::ContentProvider::ContentProvider(
    /* [in] */ ContentProvider* existingProvider)
    : Component<IntentInfo>(existingProvider)
{
    this->mInfo = existingProvider->mInfo;
    this->mSyncable = existingProvider->mSyncable;
}

CapsuleParser::ContentProvider::ContentProvider(
    /* [in] */ ParseComponentArgs* args,
    /* [in] */ IContentProviderInfo* info)
    : Component<IntentInfo>(args, info)
    , mSyncable(FALSE)
{
    mInfo = info;
    mInfo->SetApplicationInfo(args->mOwner->mApplicationInfo);
}

///////////////////////////////////////////////////////////////////////////////
// CapsuleParser::Capsule

PInterface CapsuleParser::Capsule::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    return NULL;
}

UInt32 CapsuleParser::Capsule::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CapsuleParser::Capsule::Release()
{
    return ElRefBase::Release();
}

ECode CapsuleParser::Capsule::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)this) {
        *pIID = EIID_IInterface;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

void CapsuleParser::Capsule::SetCapsuleName(
    /* [in] */ const String& newName)
{
    mCapsuleName = newName;
    mApplicationInfo->SetCapsuleName(newName);
//        for (int i=permissions.size()-1; i>=0; i--) {
//            permissions.get(i).setPackageName(newName);
//        }
//        for (int i=permissionGroups.size()-1; i>=0; i--) {
//            permissionGroups.get(i).setPackageName(newName);
//        }
    List<Activity*>::ReverseIterator arit;
    for (arit = mActivities.RBegin(); arit != mActivities.REnd(); ++arit) {
        Activity* s = *arit;
        assert(s != NULL);
        s->SetCapsuleName(newName);
    }
//        for (int i=receivers.size()-1; i>=0; i--) {
//            receivers.get(i).setPackageName(newName);
//        }
//        for (int i=providers.size()-1; i>=0; i--) {
//            providers.get(i).setPackageName(newName);
//        }
    List<Service*>::ReverseIterator brit;
    for (brit = mServices.RBegin(); brit != mServices.REnd(); ++brit) {
        Service* s = *brit;
        assert(s != NULL);
        s->SetCapsuleName(newName);
    }
//        for (int i=instrumentation.size()-1; i>=0; i--) {
//            instrumentation.get(i).setPackageName(newName);
//        }
}

///////////////////////////////////////////////////////////////////////////////
// CapsuleParser

const CapsuleParser::NewPermissionInfo* CapsuleParser::NEW_PERMISSIONS[] = {
    new NewPermissionInfo("android.permission.WRITE_EXTERNAL_STORAGE"
        /*android.Manifest.permission.WRITE_EXTERNAL_STORAGE*/,
        Build::VERSION_CODES::DONUT, 0),
    new NewPermissionInfo("android.permission.READ_PHONE_STATE"
        /*android.Manifest.permission.READ_PHONE_STATE*/,
        Build::VERSION_CODES::DONUT, 0),
};

Int32 CapsuleParser::GetNewPermissionsLength()
{
    return sizeof(NEW_PERMISSIONS)
         / sizeof(CapsuleParser::NewPermissionInfo*);
}

const Int32 CapsuleParser::SDK_VERSION = Build::VERSION::SDK_INT;
const String CapsuleParser::SDK_CODENAME(NULL);
//todo: we cann't control the satic init sequence. so the following program will enter debug.
//    String("REL").Equals(Build::VERSION::CODENAME)
//            ? String(NULL) : Build::VERSION::CODENAME;
Boolean CapsuleParser::sCompatibilityModeEnabled = TRUE;
const Int32 CapsuleParser::PARSE_DEFAULT_INSTALL_LOCATION;
const Boolean CapsuleParser::RIGID_PARSER;
const char* CapsuleParser::TAG = "CapsuleParser";
const char* CapsuleParser::ANDROID_RESOURCES = "http://schemas.android.com/apk/res/android";

CapsuleParser::CapsuleParser()
    : mParseError(CapsuleManager::INSTALL_SUCCEEDED)
    , mParseInstrumentationArgs(NULL)
    , mParseActivityArgs(NULL)
    , mParseActivityAliasArgs(NULL)
    , mParseServiceArgs(NULL)
    , mParseProviderArgs(NULL)
{
}

CapsuleParser::CapsuleParser(
    /* [in] */ const String& archiveSourcePath)
    : mArchiveSourcePath(archiveSourcePath)
    , mParseError(CapsuleManager::INSTALL_SUCCEEDED)
    , mParseInstrumentationArgs(NULL)
    , mParseActivityArgs(NULL)
    , mParseActivityAliasArgs(NULL)
    , mParseServiceArgs(NULL)
    , mParseProviderArgs(NULL)
{
}

CapsuleParser::~CapsuleParser()
{
    // TODO:
}

void CapsuleParser::SetSeparateProcesses(
    /* [in] */ const ArrayOf<String>& procs)
{
//    mSeparateProcesses = procs;
}

Boolean CapsuleParser::IsCapsuleFilename(
    /* [in] */ const String& name)
{
    return name.EndWith(String(".cap"));
}

void CapsuleParser::ValidateName(
    /* [in] */ const String& name,
    /* [in] */ Boolean requiresSeparator,
    /* [out] */ String* error)
{
    const Int32 N = name.GetCharCount();
    Boolean hasSep = FALSE;
    Boolean front = TRUE; // char parsered is the front one.
    for (int i = 0; i < N; i++) {
        //todo: the implementation is inefficient because of str.GetChar(index)
        const Char32 c = name.GetChar(i);
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            front = FALSE;
            continue;
        }
        if (!front) {
            if ((c >= '0' && c <= '9') || c == '_') {
                continue;
            }
        }
        if (c == '.') {
            hasSep = TRUE;
            front = TRUE;
            continue;
        }
        *error = (const char*)(StringBuffer("bad character '") + c + "'");
        return;
    }

    if (requiresSeparator && !hasSep) {
        *error = "must have at least one '.' separator";
    }
}

AutoPtr<ICapsuleInfo> CapsuleParser::GenerateCapsuleInfo(
    /* [in] */ Capsule* c,
    /* [in] */ const ArrayOf<Int32>* gids,
    /* [in] */ Int32 flags,
    /* [in] */ Int64 firstInstallTime,
    /* [in] */ Int64 lastUpdateTime)
{
    AutoPtr<ICapsuleInfo> ci;
    CCapsuleInfo::New((ICapsuleInfo**)&ci);
    ci->SetCapsuleName(c->mCapsuleName);
    ci->SetVersionCode(c->mVersionCode);
    ci->SetVersionName(c->mVersionName);
    ci->SetSharedUserId(c->mSharedUserId);
    ci->SetSharedUserLabel(c->mSharedUserLabel);
    ci->SetApplicationInfo(GenerateApplicationInfo(c, flags));
    ci->SetInstallLocation(c->mInstallLocation);
    ci->SetFirstInstallTime(firstInstallTime);
    ci->SetLastUpdateTime(lastUpdateTime);
    if ((flags & CapsuleManager_GET_GIDS) != 0) {
        ci->SetGids((ArrayOf<Int32>*)gids);
    }
    if ((flags & CapsuleManager_GET_CONFIGURATIONS) != 0) {
        if (c->mConfigPreferences.Begin() != c->mConfigPreferences.End()) {
            List< AutoPtr<IConfigurationInfo> >::Iterator itor;
            for (itor = c->mConfigPreferences.Begin();
                 itor != c->mConfigPreferences.End();
                 ++itor) {
                ci->AddConfigPreference(*itor);
            }
        }
        if (c->mReqFeatures.Begin() != c->mReqFeatures.End()) {
            List< AutoPtr<IFeatureInfo> >::Iterator itor;
            for (itor = c->mReqFeatures.Begin();
                 itor != c->mReqFeatures.End();
                 ++itor) {
                ci->AddReqFeature(*itor);
            }
        }
    }
    if ((flags & CapsuleManager_GET_ACTIVITIES) != 0) {
        if (c->mActivities.Begin() != c->mActivities.End()) {
            List<Activity*>::Iterator itor;
            for (itor = c->mActivities.Begin();
                 itor != c->mActivities.End();
                 ++itor) {
                Activity* activity = *itor;
                Boolean isEnabled;
                activity->mInfo->IsEnabled(&isEnabled);
                if (isEnabled || (flags & CapsuleManager_GET_DISABLED_COMPONENTS) != 0) {
                    AutoPtr<IActivityInfo> info = GenerateActivityInfo(activity, flags);
                    ci->AddActivity(info);
                }
            }
        }
    }
    if ((flags & CapsuleManager_GET_RECEIVERS) != 0) {
        if (c->mReceivers.Begin() != c->mReceivers.End()) {
            List<Activity*>::Iterator itor;
            for (itor = c->mReceivers.Begin();
                 itor != c->mReceivers.End();
                 ++itor) {
                Activity* activity = *itor;
                Boolean isEnabled;
                activity->mInfo->IsEnabled(&isEnabled);
                if (isEnabled || (flags & CapsuleManager_GET_DISABLED_COMPONENTS) != 0) {
                    AutoPtr<IActivityInfo> info = GenerateActivityInfo(activity, flags);
                    ci->AddReceiver(info);
                }
            }
        }
    }
    if ((flags & CapsuleManager_GET_SERVICES) != 0) {
        if (c->mServices.Begin() != c->mServices.End()) {
            List<Service*>::Iterator itor;
            for (itor = c->mServices.Begin();
                 itor != c->mServices.End();
                 ++itor) {
                Service* service = *itor;
                Boolean isEnabled;
                service->mInfo->IsEnabled(&isEnabled);
                if (isEnabled || (flags & CapsuleManager_GET_DISABLED_COMPONENTS) != 0) {
                    AutoPtr<IServiceInfo> info = GenerateServiceInfo(service, flags);
                    ci->AddService(info);
                }
            }
        }
    }
    if ((flags & CapsuleManager_GET_PROVIDERS) != 0) {
        if (c->mContentProviders.Begin() != c->mContentProviders.End()) {
            List<ContentProvider*>::Iterator itor;
            for (itor = c->mContentProviders.Begin();
                 itor != c->mContentProviders.End();
                 ++itor) {
                ContentProvider* provider = *itor;
                Boolean isEnabled;
                provider->mInfo->IsEnabled(&isEnabled);
                if (isEnabled || (flags & CapsuleManager_GET_DISABLED_COMPONENTS) != 0) {
                    AutoPtr<IContentProviderInfo> info = GenerateContentProviderInfo(provider, flags);
                    ci->AddContentProvider(info);
                }
            }
        }
    }
    if ((flags & CapsuleManager_GET_INSTRUMENTATION) != 0) {
        if (c->mInstrumentation.Begin() != c->mInstrumentation.End()) {
            List<Instrumentation*>::Iterator itor;
            for (itor = c->mInstrumentation.Begin();
                 itor != c->mInstrumentation.End();
                 ++itor) {
                AutoPtr<IInstrumentationInfo> ins = GenerateInstrumentationInfo(*itor, flags);
                ci->AddInstrumentation(ins);
            }
        }
    }
    if ((flags & CapsuleManager_GET_PERMISSIONS) != 0) {
        if (c->mPermissions.Begin() != c->mPermissions.End()) {
            List<Permission*>::Iterator itor;
            for (itor = c->mPermissions.Begin();
                 itor != c->mPermissions.End();
                 ++itor) {
                AutoPtr<IPermissionInfo> info= GeneratePermissionInfo(*itor, flags);
                ci->AddPermission(info);
            }
        }
        if (c->mRequestedPermissions.Begin() != c->mRequestedPermissions.End()) {
            List<String>::Iterator itor;
            for (itor = c->mRequestedPermissions.Begin();
                 itor != c->mRequestedPermissions.End();
                 ++itor) {
                ci->AddRequestedPermission(*itor);
            }
        }
    }
    if ((flags & CapsuleManager_GET_SIGNATURES) != 0) {
        for (Int32 i = 0; i < c->mSignatures->GetLength(); ++i) {
            ci->AddSignature((*c->mSignatures)[i]);
        }
    }
    return ci;
}

AutoPtr<IPermissionInfo> CapsuleParser::GeneratePermissionInfo(
    /* [in] */ Permission* p,
    /* [in] */ Int32 flags)
{
    if (p == NULL) return NULL;

    if ((flags & CapsuleManager_GET_META_DATA) == 0) {
        return p->mInfo;
    }

    AutoPtr<IPermissionInfo> pi;
    ASSERT_SUCCEEDED(CPermissionInfo::New(
            p->mInfo.Get(), (IPermissionInfo**)&pi));
    pi->SetMetaData(p->mMetaData);
    return pi;
}

AutoPtr<IPermissionGroupInfo> CapsuleParser::GeneratePermissionGroupInfo(
    /* [in] */ PermissionGroup* pg,
    /* [in] */ Int32 flags)
{
    if (pg == NULL) return NULL;

    if ((flags & CapsuleManager_GET_META_DATA) == 0) {
        return pg->mInfo;
    }

    AutoPtr<IPermissionGroupInfo> pgi;
    ASSERT_SUCCEEDED(CPermissionGroupInfo::New(
        pg->mInfo.Get(), (IPermissionGroupInfo**)&pgi));
    pgi->SetMetaData(pg->mMetaData);
    return pgi;
}

Int32 CapsuleParser::GetParseError()
{
    return mParseError;
}

ECode CapsuleParser::ParseCapsuleName(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* outError,
    /* [out] */ String* name)
{
    assert(parser);
    assert(attrs);
    assert(outError);
    assert(name);

    ECode ec = NOERROR;
    Int32 type = 0;
    while ((parser->Next(&type), type) != IXmlPullParser_START_TAG
               && type != IXmlPullParser_END_DOCUMENT) {
        ;
    }

    if (type != IXmlPullParser_START_TAG) {
        (*outError)[0] = "No start tag found";
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    if ((flags & PARSE_CHATTY) != 0 && Config::LOGV) {
        String name;
        parser->GetName(&name);
        Logger::V(TAG, StringBuffer("Root element name: '") + name + "'");
    }

    String n;
    parser->GetName(&n);
    if (!n.Equals("manifest")) {
        (*outError)[0] = "No <manifest> tag";
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    ec = attrs->GetAttributeValueEx(NULL, "package", name);
    if (FAILED(ec) || name->IsNullOrEmpty()) {
        (*outError)[0] = "<manifest> does not specify package";
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    String nameError;
    ValidateName(*name, false, &nameError);
    if (!nameError.IsNull() /*&& !"android".equals(pkgName)*/) {
        (*outError)[0] = (const char*)(
            StringBuffer("<manifest> specifies bad package name \"")
            + *name + "\": " + nameError);
        return E_RUNTIME_EXCEPTION;
    }

    /* return pkgName.intern(); */
    return NOERROR;
}

CapsuleParser::Capsule* CapsuleParser::ParseCapsule(
    /* [in] */ IFile* sourceFile,
    /* [in] */ const String& destCodePath,
    /* [in] */ IDisplayMetrics* metrics,
    /* [in] */ Int32 flags)
{
    mParseError = CapsuleManager::INSTALL_SUCCEEDED;

    sourceFile->GetPath(&mArchiveSourcePath);
    Boolean isFile = FALSE;
    if (sourceFile->IsFile(&isFile), !isFile) {
        // Log.w(TAG, "Skipping dir: " + mArchiveSourcePath);
        Logger::W(TAG, StringBuffer("Skipping dir: ") + (String)mArchiveSourcePath);
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_NOT_APK;
        return NULL;
    }
    String name;
    sourceFile->GetName(&name);
    if (!IsCapsuleFilename(name)
            && (flags & PARSE_MUST_BE_APK) != 0) {
        if ((flags & PARSE_IS_SYSTEM) == 0) {
            // We expect to have non-.apk files in the system dir,
            // so don't warn about them.
            Logger::W(TAG, StringBuffer("SSkipping non-package file: ") + (String)mArchiveSourcePath);
        }
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_NOT_APK;
        return NULL;
    }

    // if ((flags&PARSE_CHATTY) != 0 && Config.LOGD) Log.d(
    //     TAG, "Scanning package: " + mArchiveSourcePath);

    AutoPtr<IXmlResourceParser> parser;
    AutoPtr<IAssetManager> assmgr;
    Boolean assetError = TRUE;
    // try {
    CAssetManager::New((IAssetManager**)&assmgr);
    Int32 cookie = 0;
    assmgr->AddAssetPath(mArchiveSourcePath, &cookie);
    if(cookie != 0) {
        assmgr->OpenXmlResourceParserEx(cookie, String("AndroidManifest.xml"),
            (IXmlResourceParser**)&parser);
        assetError = FALSE;
    }
    else {
        Logger::W(TAG, StringBuffer("Failed adding asset path: ") + (String)mArchiveSourcePath);
    }
    // } catch (Exception e) {
    //     Log.w(TAG, "Unable to read AndroidManifest.xml of "
    //             + mArchiveSourcePath, e);
    // }
    if(assetError) {
        if (assmgr != NULL) assmgr->Close();
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_BAD_MANIFEST;
        return NULL;
    }
    AutoStringArray errorText = ArrayOf<String>::Alloc(1);
    CapsuleParser::Capsule* cap = NULL;
    // Exception errorException = NULL;
    // try {
    // XXXX todo: need to figure out correct configuration.
    AutoPtr<IResources> res;
    CResources::New(assmgr, metrics, NULL, (IResources**)&res);
    ECode ec = ParseCapsule(res.Get(), parser.Get(), flags, errorText, &cap);
    if (FAILED(ec)) {
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_UNEXPECTED_EXCEPTION;
    }
    // } catch (Exception e) {
    //     errorException = e;
    //     mParseError = PackageManager.INSTALL_PARSE_FAILED_UNEXPECTED_EXCEPTION;
    // }

    if (cap == NULL) {
        // if (errorException != NULL) {
        //     Log.w(TAG, mArchiveSourcePath, errorException);
        // } else {
        //     Log.w(TAG, mArchiveSourcePath + " (at "
        //             + parser.getPositionDescription()
        //             + "): " + errorText[0]);
        // }
        parser->Close();
        assmgr->Close();
        if (mParseError == CapsuleManager::INSTALL_SUCCEEDED) {
            mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        }
        return NULL;
    }

    parser->Close();
    assmgr->Close();

    // Set code and resource paths
    cap->mPath = destCodePath;
    cap->mScanPath = mArchiveSourcePath;
    //pkg.applicationInfo.sourceDir = destCodePath;
    //pkg.applicationInfo.publicSourceDir = destRes;
    cap->mSignatures = NULL;

    return cap;
}

ECode CapsuleParser::BuildClassName(
    /* [in] */ const String& cap,
    /* [in] */ ICharSequence* clsSeq,
    /* [in] */ ArrayOf<String>* outError,
    /* [out] */ String* name)
{
//      if (clsSeq == null || clsSeq->Length() <= 0) {
//          outError[0] = "Empty class name in package " + pkg;
//          return null;
//      }
//      String cls = clsSeq->ToString();
//      char c = cls->CharAt(0);
//      if (c == '->m') {
//          return (pkg + cls)->Intern();
//      }
//      if (cls->IndexOf('->m') < 0) {
//          StringBuilder b = new StringBuilder(pkg);
//          b->Append('->m');
//          b->Append(cls);
//          return b->ToString()->Intern();
//      }
//      if (c >= 'a' && c <= 'z') {
//          return cls->Intern();
//      }
//      outError[0] = "Bad class name " + cls + " in package " + pkg;
    return E_NOT_IMPLEMENTED;
}

ECode CapsuleParser::BuildProcessName(
    /* [in] */ const String& cap,
    /* [in] */ const String& defProc,
    /* [in] */ ICharSequence* procSeq,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* separateProcesses,
    /* [in] */ ArrayOf<String>* outError,
    /* [out] */ String* name)
{
//	    if ((flags&PARSE_IGNORE_PROCESSES) != 0 && !"system".equals(procSeq)) {
//	        return defProc != null ? defProc : pkg;
//	    }
//	    if (separateProcesses != null) {
//	        for (int i=separateProcesses.length-1; i>=0; i--) {
//	            String sp = separateProcesses[i];
//	            if (sp.equals(pkg) || sp.equals(defProc) || sp.equals(procSeq)) {
//	                return pkg;
//	            }
//	        }
//	    }
//	    if (procSeq == null || procSeq.length() <= 0) {
//	        return defProc;
//	    }
//	    return buildCompoundName(pkg, procSeq, "process", outError);
    //todo:
    *name = cap;
    return E_NOT_IMPLEMENTED;
}

ECode CapsuleParser::BuildTaskAffinityName(
    /* [in] */ const String& cap,
    /* [in] */ const String& defProc,
    /* [in] */ ICharSequence* procSeq,
    /* [in] */ ArrayOf<String>* outError,
    /* [out] */ String* name)
{
//         if (procSeq == NULL) {
//            *name = String::Duplicate(defProc);
//             return NOERROR;
//        }
//         if (procSeq->GetLength() <= 0) {
//             return NULL;
//        }
//         return buildCompoundName(pkg, procSeq, "taskAffinity", outError);
    return E_NOT_IMPLEMENTED;
}

// TODO: replace this by ParseCapsule with four parameters.
ECode CapsuleParser::ParseCapsule(
    /* [in] */ const String& capPath,
    /* [out, callee] */ CapsuleParser::Capsule** capsule,
    /* [out] */ String* errMsg)
{
    return ParseCapsule(capPath, String(NULL), NULL, 0, capsule);
}

ECode CapsuleParser::ParseCapsule(
    /* [in] */ const String& capPath,
    /* [in] */ const String& destCodePath,
    /* [in] */ IDisplayMetrics* metrics,
    /* [in] */ Int32 flags,
    /* [out, callee] */ CapsuleParser::Capsule** capsule)
{
    assert(capsule);

    mParseError = CapsuleManager::INSTALL_SUCCEEDED;

    // mArchiveSourcePath = sourceFile.getPath();
    mArchiveSourcePath = capPath;
    AutoPtr<IFile> sourceFile;
    FAIL_RETURN(CFile::New(mArchiveSourcePath, (IFile**)&sourceFile));

    Boolean isFile = FALSE;
    sourceFile->IsFile(&isFile);
    if (!isFile) {
        Logger::W(TAG, StringBuffer("Skipping dir: ") + mArchiveSourcePath);
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_NOT_APK;
        return E_RUNTIME_EXCEPTION;
    }

    String name;
    sourceFile->GetName(&name);
    if (!IsCapsuleFilename(name) && (flags & PARSE_MUST_BE_APK) != 0) {
        if ((flags & PARSE_IS_SYSTEM) == 0) {
            // We expect to have non-.apk files in the system dir,
            // so don't warn about them.
            Logger::W(TAG, StringBuffer("Skipping non-package file: ") + mArchiveSourcePath);
        }
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_NOT_APK;
        return E_RUNTIME_EXCEPTION;
    }

    if ((flags & PARSE_CHATTY) != 0 && Config::LOGD) {
        Logger::D(TAG, StringBuffer("Scanning package: ") + mArchiveSourcePath);
    }

    AutoPtr<IXmlResourceParser> parser;
    AutoPtr<IAssetManager> assmgr;
    Boolean assetError = TRUE;
    ECode ec = NOERROR;

    FAIL_RETURN(CAssetManager::New((IAssetManager**)&assmgr));

    Int32 cookie = 0;
    ec = assmgr->AddAssetPath(mArchiveSourcePath, &cookie);
    if(SUCCEEDED(ec)) {
        ec = assmgr->OpenXmlResourceParserEx(
            cookie, String("AndroidManifest.xml"), (IXmlResourceParser**)&parser);
        if (FAILED(ec)) {
            Logger::W(TAG, StringBuffer("Unable to read AndroidManifest.xml of ")
                    + mArchiveSourcePath);
            return E_RUNTIME_EXCEPTION;
        }
        assetError = FALSE;
    } else {
        Logger::W(TAG, StringBuffer("Failed adding asset path:") + mArchiveSourcePath);
    }

    if(assetError) {
        if (assmgr != NULL) assmgr->Close();
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_BAD_MANIFEST;
        return E_RUNTIME_EXCEPTION;
    }

    AutoStringArray errorText = ArrayOf<String>::Alloc(1);
    if (errorText == NULL) return E_OUT_OF_MEMORY_ERROR;

    // XXXX todo: need to figure out correct configuration.
    AutoPtr<IResources> res;
    FAIL_RETURN(CResources::New(assmgr.Get(), metrics, NULL, (IResources**)&res));

    FAIL_RETURN(ParseCapsule(res.Get(), parser.Get(), flags, errorText, capsule));

    parser->Close();
    assmgr->Close();
    if (FAILED(ec)) {
        String des;
        parser->GetPositionDescription(&des); // TODO: need probe ?
        Logger::W(TAG, StringBuffer(mArchiveSourcePath) + " (at "
                + (String)des + "): " + (*errorText)[0]);

        if (mParseError == CapsuleManager::INSTALL_SUCCEEDED) {
            mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        }
        else {
            mParseError = CapsuleManager::INSTALL_PARSE_FAILED_UNEXPECTED_EXCEPTION;
        }

        return ec;
    }

    // Set code and resource paths
    (*capsule)->mPath = destCodePath;
    (*capsule)->mScanPath = mArchiveSourcePath;
    //(*capsule).applicationInfo.sourceDir = destCodePath;
    //(*capsule).applicationInfo.publicSourceDir = destRes;
    (*capsule)->mSignatures = NULL;

    return NOERROR;
}

static
Int32 R_Styleable_AndroidManifest[] = {
    0x0101000b, 0x0101021b, 0x0101021c, 0x01010261,
    0x010102b7
};

static
Int32 R_Styleable_AndroidManifestUsesConfiguration[] = {
    0x01010227, 0x01010228, 0x01010229, 0x0101022a,
    0x01010232
};

static
Int32 R_Styleable_AndroidManifestUsesPermission[] = {
    0x01010003
};

static
Int32 R_Styleable_AndroidManifestUsesFeature[] = {
    0x01010003, 0x01010281, 0x0101028e
};

static
Int32 R_Styleable_AndroidManifestUsesSdk[] = {
    0x0101020c, 0x01010270, 0x01010271
};

static
Int32 R_Styleable_AndroidManifestSupportsScreens[] = {
    0x0101026c, 0x01010284, 0x01010285, 0x01010286,
    0x0101028d, 0x010102bf
};

static
Int32 R_Styleable_AndroidManifestProtectedBroadcast[] = {
    0x01010003
};

static
Int32 R_Styleable_AndroidManifestOriginalCapsule[] = {
    0x01010003
};

ECode CapsuleParser::ParseCapsule(
    /* [in] */ IResources* res,
    /* [in] */ IXmlResourceParser* parser,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* outError,
    /* [out, callee] */ CapsuleParser::Capsule** capsule)
{
    ECode ec = NOERROR;
    AutoPtr<IAttributeSet> attrs = (IAttributeSet*)parser->Probe(EIID_IAttributeSet);
    assert(attrs);

    mParseInstrumentationArgs = NULL;
    if (mParseActivityArgs) {
        delete mParseActivityArgs;
        mParseActivityArgs = NULL;
    }
    mParseServiceArgs = NULL;
    mParseProviderArgs = NULL;

    String capName;
    ec = ParseCapsuleName(parser, attrs.Get(), flags, outError, &capName);
    if (FAILED(ec)) {
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_BAD_PACKAGE_NAME;
        return E_RUNTIME_EXCEPTION;
    }

    // final Package pkg = new Package(pkgName);
    const AutoPtr<Capsule> cap = new Capsule(capName);
    cap->SetCapsuleName(capName);
    Boolean foundApp = FALSE;

    AutoPtr<ITypedArray> sa;
    FAIL_RETURN(res->ObtainAttributes(
        attrs, ArrayOf<Int32>(R_Styleable_AndroidManifest,
        sizeof(R_Styleable_AndroidManifest) / sizeof(Int32)),/*com.android.internal.R.styleable.AndroidManifest*/
        (ITypedArray**)&sa));
    sa->GetInteger(
            1 /* com.android.internal.R.styleable.AndroidManifest_versionCode*/, 0,
            &cap->mVersionCode);
    sa->GetNonConfigurationString(
            2 /*com.android.internal.R.styleable.AndroidManifest_versionName*/, 0,
            &cap->mVersionName);

    String str;
    sa->GetNonConfigurationString(
            0 /* com.android.internal.R.styleable.AndroidManifest_sharedUserId*/, 0,
            &str);
    if (!str.IsNullOrEmpty()) {
        String nameError;
        ValidateName(str, true, &nameError);
        if (!nameError.IsNull() /*&& !"android".equals(pkgName)*/) {
            (*outError)[0] = (const char*)(
                StringBuffer("<manifest> specifies bad sharedUserId name \"")
                + str + "\": " + nameError);
            mParseError = CapsuleManager::INSTALL_PARSE_FAILED_BAD_SHARED_USER_ID;
            *capsule = NULL;
            return E_RUNTIME_EXCEPTION;
        }
        cap->mSharedUserId = str;
        sa->GetResourceId(
                3 /*com.android.internal.R.styleable.AndroidManifest_sharedUserLabel*/, 0,
                &cap->mSharedUserLabel);
    }
    sa->Recycle();

    sa->GetInteger(
            4 /*com.android.internal.R.styleable.AndroidManifest_installLocation*/,
            PARSE_DEFAULT_INSTALL_LOCATION, &cap->mInstallLocation);
    cap->mApplicationInfo->SetInstallLocation(cap->mInstallLocation);

    // Resource boolean are -1, so 1 means we don't know the value.
    Int32 supportsSmallScreens = 1;
    Int32 supportsNormalScreens = 1;
    Int32 supportsLargeScreens = 1;
    Int32 supportsXLargeScreens = 1;
    Int32 resizeable = 1;
    Int32 anyDensity = 1;

    Int32 outerDepth = 0;
    parser->GetDepth(&outerDepth);

    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser_END_DOCUMENT
           && (type != IXmlPullParser_END_TAG
           || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser_END_TAG || type == IXmlPullParser_TEXT) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals("application")) {
            if (foundApp) {
                if (RIGID_PARSER) {
                    (*outError)[0] = "<manifest> has more than one <application>";
                    mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                    *capsule = NULL;
                    return E_RUNTIME_EXCEPTION;
                }
                else {
                    Logger::W(TAG, "<manifest> has more than one <application>");
                    XmlUtils::SkipCurrentTag((IXmlPullParser*)parser);
                    continue;
                }
            }

            foundApp = TRUE;
            if (!ParseApplication(cap, res, (IXmlPullParser*)parser, attrs, flags, outError)) {
                *capsule = NULL;
                return E_XML_PULL_PARSER_EXCEPTION;
            }
        }
        else if (tagName.Equals("permission-group")) {
            if (ParsePermissionGroup(cap, res, parser, attrs, outError) == NULL) {
                *capsule = NULL;
                return E_XML_PULL_PARSER_EXCEPTION;
            }
        }
        else if (tagName.Equals("permission")) {
            if (ParsePermission(cap, res, parser, attrs, outError) == NULL) {
                *capsule = NULL;
                return E_XML_PULL_PARSER_EXCEPTION;
            }
        }
        else if (tagName.Equals("permission-tree")) {
            if (ParsePermissionTree(cap, res, parser, attrs, outError) == NULL) {
                *capsule = NULL;
                return E_XML_PULL_PARSER_EXCEPTION;
            }
        }
        else if (tagName.Equals("uses-permission")) {
            if (sa != NULL) sa->Release();
            FAIL_RETURN(res->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_AndroidManifestUsesPermission,
                    sizeof(R_Styleable_AndroidManifestUsesPermission) / sizeof(Int32))
                /*com.android.internal.R.styleable.AndroidManifestUsesPermission*/,
                (ITypedArray**)&sa));

            // Note: don't allow this value to be a reference to a resource
            // that may change
            String name;
            sa->GetNonResourceString(
                0 /*com.android.internal.R.styleable.AndroidManifestUsesPermission_name*/,
                &name);

            sa->Recycle();

            if (!name.IsNull()
                && Find(cap->mRequestedPermissions.Begin(), cap->mRequestedPermissions.End(), name)
                    == cap->mRequestedPermissions.End()) {
                cap->mRequestedPermissions.PushBack(name);
            }

            XmlUtils::SkipCurrentTag(parser);

        }
        else if (tagName.Equals("uses-configuration")) {
            AutoPtr<IConfigurationInfo> cPref;
            FAIL_RETURN(CConfigurationInfo::New((IConfigurationInfo**)&cPref));
            if (sa != NULL) sa->Release();
            FAIL_RETURN(res->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_AndroidManifestUsesConfiguration,
                    sizeof(R_Styleable_AndroidManifestUsesConfiguration) / sizeof(Int32))
                /*com.android.internal.R.styleable.AndroidManifestUsesConfiguration*/,
                (ITypedArray**)&sa));
            Int32 ivalue;
            sa->GetInt32(
                0 /*com.android.internal.R.styleable.AndroidManifestUsesConfiguration_reqTouchScreen*/,
                Configuration_TOUCHSCREEN_UNDEFINED, &ivalue);
            cPref->SetReqTouchScreen(ivalue);
            sa->GetInt32(
                1 /*com.android.internal.R.styleable.AndroidManifestUsesConfiguration_reqKeyboardType*/,
                Configuration_KEYBOARD_UNDEFINED, &ivalue);
            cPref->SetReqKeyboardType(ivalue);
            Boolean value = FALSE;
            sa->GetBoolean(
                2 /*com.android.internal.R.styleable.AndroidManifestUsesConfiguration_reqHardKeyboard*/,
                FALSE, &value);
            if (value) {
                Int32 req;
                cPref->GetReqInputFeatures(&req);
                req |= ConfigurationInfo_INPUT_FEATURE_HARD_KEYBOARD;
                cPref->SetReqInputFeatures(req);
            }
            sa->GetInt32(
                3 /*com.android.internal.R.styleable.AndroidManifestUsesConfiguration_reqNavigation*/,
                Configuration_NAVIGATION_UNDEFINED, &ivalue);
            cPref->SetReqNavigation(ivalue);
            sa->GetBoolean(
                4 /*com.android.internal.R.styleable.AndroidManifestUsesConfiguration_reqFiveWayNav*/,
                FALSE, &value);
            if (value) {
                Int32 req;
                cPref->GetReqInputFeatures(&req);
                req |= ConfigurationInfo_INPUT_FEATURE_FIVE_WAY_NAV;
                cPref->SetReqInputFeatures(req);
            }
            sa->Recycle();
            cap->mConfigPreferences.PushBack(cPref);

            XmlUtils::SkipCurrentTag(parser);

        }
        else if (tagName.Equals("uses-feature")) {
            AutoPtr<IFeatureInfo> fi;
            FAIL_RETURN(CFeatureInfo::New((IFeatureInfo**)&fi));
            if (sa != NULL) sa->Release();
            FAIL_RETURN(res->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_AndroidManifestUsesFeature,
                    sizeof(R_Styleable_AndroidManifestUsesFeature) / sizeof(Int32))
                /*com.android.internal.R.styleable.AndroidManifestUsesFeature*/,
                (ITypedArray**)&sa));
            // Note: don't allow this value to be a reference to a resource
            // that may change
            String fiName;
            sa->GetNonResourceString(
                0 /*com.android.internal.R.styleable.AndroidManifestUsesFeature_name*/,
                &fiName);
            fi->SetName(fiName);
            if (fiName.IsNull()) {
                Int32 ivalue;
                sa->GetInt32(
                    1 /*com.android.internal.R.styleable.AndroidManifestUsesFeature_glEsVersion*/,
                    FeatureInfo_GL_ES_VERSION_UNDEFINED, &ivalue);
                fi->SetReqGlEsVersion(ivalue);
            }
            Boolean value = FALSE;
            sa->GetBoolean(
                2 /*com.android.internal.R.styleable.AndroidManifestUsesFeature_required*/,
                TRUE, &value);
            if (value) {
                Int32 fiFlags;
                fi->GetFlags(&fiFlags);
                fiFlags |= FeatureInfo_FLAG_REQUIRED;
                fi->SetFlags(fiFlags);
            }
            sa->Recycle();
            cap->mReqFeatures.PushBack(fi);

            if (fiName.IsNull()) {
                AutoPtr<IConfigurationInfo> cPref;
                FAIL_RETURN(CConfigurationInfo::New((IConfigurationInfo**)&cPref));
                Int32 version;
                fi->GetReqGlEsVersion(&version);
                cPref->SetReqGlEsVersion(version);
                cap->mConfigPreferences.PushBack(cPref);
            }

            XmlUtils::SkipCurrentTag(parser);

        }
        else if (tagName.Equals("uses-sdk")) {
            if (SDK_VERSION > 0) {
                if (sa != NULL) sa->Release();
                FAIL_RETURN(res->ObtainAttributes(attrs,
                    ArrayOf<Int32>(R_Styleable_AndroidManifestUsesSdk,
                        sizeof(R_Styleable_AndroidManifestUsesSdk) / sizeof(Int32))
                    /*com.android.internal.R.styleable.AndroidManifestUsesSdk*/,
                    (ITypedArray**)&sa));

                Int32 minVers = 0;
                String minCode(NULL);
                Int32 targetVers = 0;
                String targetCode(NULL);

                AutoPtr<ITypedValue> val;
                sa->PeekValue(
                    0 /*com.android.internal.R.styleable.AndroidManifestUsesSdk_minSdkVersion*/,
                    (ITypedValue**)&val);
                if (val != NULL) {
                    Int32 valType;
                    val->GetType(&valType);
                    AutoPtr<ICharSequence> valString;
                    val->GetString((ICharSequence**)&valString);
                    if (valType == TypedValue_TYPE_STRING && valString != NULL) {
                        String str;
                        valString->ToString(&str);
                        minCode = str;
                        targetCode = str;
                    }
                    else {
                        // If it's not a string, it's an integer.
                        Int32 valData;
                        val->GetData(&valData);
                        targetVers = minVers = valData;
                    }
                }

                val->Release();
                sa->PeekValue(
                    1 /*com.android.internal.R.styleable.AndroidManifestUsesSdk_targetSdkVersion*/,
                    (ITypedValue**)&val);
                if (val != NULL) {
                    Int32 valType;
                    val->GetType(&valType);
                    AutoPtr<ICharSequence> valString;
                    val->GetString((ICharSequence**)&valString);
                    if (valType == TypedValue_TYPE_STRING && valString != NULL) {
                        String str;
                        valString->ToString(&str);
                        minCode = str;
                        targetCode = str;
                    }
                    else {
                        // If it's not a string, it's an integer.
                        Int32 valData;
                        val->GetData(&valData);
                        targetVers = valData;
                    }
                }

                sa->Recycle();

                if (!minCode.IsNull()) {
                    if (!minCode.Equals(SDK_CODENAME)) {
                        if (SDK_CODENAME != NULL) {
                            (*outError)[0] = (const char*)(
                                StringBuffer("Requires development platform ") + minCode
                                    + " (current platform is " + SDK_CODENAME + ")");
                        } else {
                            (*outError)[0] = (const char*)(
                                StringBuffer("Requires development platform ") + minCode
                                    + " but this is a release platform->m");
                        }
                        mParseError = CapsuleManager::INSTALL_FAILED_OLDER_SDK;
                        *capsule = NULL;
                        return E_RUNTIME_EXCEPTION;
                    }
                } else if (minVers > SDK_VERSION) {
                    (*outError)[0] = (const char*)(
                        StringBuffer("Requires newer sdk version #") + minVers
                            + " (current version is #" + SDK_VERSION + ")");
                    mParseError = CapsuleManager::INSTALL_FAILED_OLDER_SDK;
                    *capsule = NULL;
                    return E_RUNTIME_EXCEPTION;
                }

                if (!targetCode.IsNull()) {
                    if (!targetCode.Equals(SDK_CODENAME)) {
                        if (!SDK_CODENAME.IsNull()) {
                            (*outError)[0] = (const char*)(
                                StringBuffer("Requires development platform ") + targetCode
                                    + " (current platform is " + SDK_CODENAME + ")");
                        } else {
                            (*outError)[0] = (const char*)(
                                StringBuffer("Requires development platform ") + targetCode
                                    + " but this is a release platform->m");
                        }
                        mParseError = CapsuleManager::INSTALL_FAILED_OLDER_SDK;
                        *capsule = NULL;
                        return E_RUNTIME_EXCEPTION;
                    }
                    // If the code matches, it definitely targets this SDK.
//	                    cap->mApplicationInfo->mTargetSdkVersion
//	                        = android->mOs->mBuild::VERSION_CODES->mCUR_DEVELOPMENT;
                } else {
                    cap->mApplicationInfo->SetTargetSdkVersion(targetVers);
                }
            }

            XmlUtils::SkipCurrentTag(parser);

        }
        else if (tagName.Equals("supports-screens")) {
            if (sa != NULL) sa->Release();
            FAIL_RETURN(res->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_AndroidManifestSupportsScreens,
                    sizeof(R_Styleable_AndroidManifestSupportsScreens) / sizeof(Int32))
                /*com.android.internal.R.styleable.AndroidManifestSupportsScreens*/,
                (ITypedArray**)&sa));

            // This is a trick to get a Boolean and still able to detect
            // if a value was actually set
            sa->GetInteger(
                1 /*com.android.internal.R.styleable.AndroidManifestSupportsScreens_smallScreens*/,
                supportsSmallScreens, &supportsSmallScreens);
            sa->GetInteger(
                2 /*com.android.internal.R.styleable.AndroidManifestSupportsScreens_normalScreens*/,
                supportsNormalScreens, &supportsNormalScreens);
            sa->GetInteger(
                3 /*com.android.internal.R.styleable.AndroidManifestSupportsScreens_largeScreens*/,
                supportsLargeScreens, &supportsLargeScreens);
            sa->GetInteger(
                5 /*com.android.internal.R.styleable.AndroidManifestSupportsScreens_xlargeScreens*/,
                supportsXLargeScreens, &supportsXLargeScreens);
            sa->GetInteger(
                4 /*com.android.internal.R.styleable.AndroidManifestSupportsScreens_resizeable*/,
                resizeable, &resizeable);
            sa->GetInteger(
                0 /*com.android.internal.R.styleable.AndroidManifestSupportsScreens_anyDensity*/,
                anyDensity, &anyDensity);

            sa->Recycle();

            XmlUtils::SkipCurrentTag(parser);

        }
        else if (tagName.Equals("protected-broadcast")) {
            if (sa != NULL) sa->Release();
            FAIL_RETURN(res->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_AndroidManifestProtectedBroadcast,
                    sizeof(R_Styleable_AndroidManifestProtectedBroadcast) / sizeof(Int32))
                /*com.android.internal.R.styleable.AndroidManifestProtectedBroadcast*/,
                (ITypedArray**)&sa));

            // Note: don't allow this value to be a reference to a resource
            // that may change
            String name;
            sa->GetNonResourceString(
                0 /*com.android.internal.R.styleable.AndroidManifestProtectedBroadcast_name*/,
                &name);

            sa->Recycle();

            if (!name.IsNull() && (flags & PARSE_IS_SYSTEM) != 0) {
                if (cap->mProtectedBroadcasts == NULL) {
                    cap->mProtectedBroadcasts = new List<String>();
                }
                if (Find(cap->mProtectedBroadcasts->Begin(), cap->mProtectedBroadcasts->End(), name)
                    == cap->mProtectedBroadcasts->End()) {
                    cap->mProtectedBroadcasts->PushBack(name);
                }
            }

            XmlUtils::SkipCurrentTag(parser);

        }
        else if (tagName.Equals("instrumentation")) {
            if (ParseInstrumentation(cap, res, parser, attrs, outError) == NULL) {
                *capsule = NULL;
                return E_XML_PULL_PARSER_EXCEPTION;
            }

        }
        else if (tagName.Equals("original-package")) {
            if (sa != NULL) sa->Release();
            FAIL_RETURN(res->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_AndroidManifestOriginalCapsule,
                    sizeof(R_Styleable_AndroidManifestOriginalCapsule) / sizeof(Int32))
                /*com.android.internal.R.styleable.AndroidManifestOriginalPackage*/,
                (ITypedArray**)&sa));

            String orig;
            sa->GetNonConfigurationString(
                0 /*com.android.internal.R.styleable.AndroidManifestOriginalPackage_name*/,
                0, &orig);
            if (!cap->mCapsuleName.Equals(orig)) {
                if (cap->mOriginalCapsules == NULL) {
                    cap->mOriginalCapsules = new List<String>();
                    cap->mRealCapsule = cap->mCapsuleName;
                }
                cap->mOriginalCapsules->PushBack(orig);
            }

            sa->Recycle();

            XmlUtils::SkipCurrentTag(parser);

        }
        else if (tagName.Equals("adopt-permissions")) {
            if (sa != NULL) sa->Release();
            FAIL_RETURN(res->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_AndroidManifestOriginalCapsule,
                    sizeof(R_Styleable_AndroidManifestOriginalCapsule) / sizeof(Int32))
                /*com.android.internal.R.styleable.AndroidManifestOriginalPackage*/,
                (ITypedArray**)&sa));

            String name;
            sa->GetNonConfigurationString(
                0 /*com.android.internal.R.styleable.AndroidManifestOriginalPackage_name*/,
                0, &name);

            sa->Recycle();

            if (!name.IsNull()) {
                if (cap->mAdoptPermissions == NULL) {
                    cap->mAdoptPermissions = new List<String>();
                }
                cap->mAdoptPermissions->PushBack(name);
            }

            XmlUtils::SkipCurrentTag(parser);

        }
        else if (tagName.Equals("uses-gl-texture")) {
            // Just skip this tag
            XmlUtils::SkipCurrentTag(parser);
            continue;

        }
        else if (tagName.Equals("compatible-screens")) {
            // Just skip this tag
            XmlUtils::SkipCurrentTag(parser);
            continue;

        }
        else if (tagName.Equals("eat-comment")) {
            // Just skip this tag
            XmlUtils::SkipCurrentTag(parser);
            continue;

        }
        else if (RIGID_PARSER) {
            (*outError)[0] = (const char*)(
                StringBuffer("Bad element under <manifest>: ") + tagName);
            mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
            *capsule = NULL;
            return E_XML_PULL_PARSER_EXCEPTION;

        }
        else {
            String des;
            parser->GetPositionDescription(&des);
            Logger::W(TAG, StringBuffer("Unknown element under <manifest>: ") + tagName
                + " at " + mArchiveSourcePath + " " + des);
            XmlUtils::SkipCurrentTag(parser);
            continue;
        }
    }

    if (!foundApp && cap->mInstrumentation.Begin() == cap->mInstrumentation.End()) {
        (*outError)[0] = "<manifest> does not contain an <application> or <instrumentation>";
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_EMPTY;
    }

    const Int32 NP = sizeof(NEW_PERMISSIONS) / sizeof(NewPermissionInfo*);
    StringBuffer implicitPerms;
    for (Int32 ip = 0; ip < NP; ip++) {
        const NewPermissionInfo* npi
                = CapsuleParser::NEW_PERMISSIONS[ip];
        Int32 sdkVersion;
        cap->mApplicationInfo->GetTargetSdkVersion(&sdkVersion);
        if (sdkVersion >= npi->mSdkVersion) {
            break;
        }
        if (Find(cap->mRequestedPermissions.Begin(), cap->mRequestedPermissions.End(), npi->mName)
            == cap->mRequestedPermissions.End()) {
            if (implicitPerms.IsNull()) {
                implicitPerms += cap->mCapsuleName;
                implicitPerms += ": compat added ";
            }
            else {
                implicitPerms += " ";
            }
            implicitPerms += npi->mName;
            cap->mRequestedPermissions.PushBack(npi->mName);
        }
    }
    if (!implicitPerms.IsNull()) {
        Logger::I(TAG, implicitPerms);
    }

    Int32 capSdkVersion, capFlags;
    cap->mApplicationInfo->GetTargetSdkVersion(&capSdkVersion);
    cap->mApplicationInfo->GetFlags(&capFlags);
    if (supportsSmallScreens < 0 || (supportsSmallScreens > 0
        && capSdkVersion >= Build::VERSION_CODES::DONUT)) {
        capFlags |= ApplicationInfo_FLAG_SUPPORTS_SMALL_SCREENS;
        cap->mApplicationInfo->SetFlags(capFlags);
    }
    if (supportsNormalScreens != 0) {
        capFlags |= ApplicationInfo_FLAG_SUPPORTS_NORMAL_SCREENS;
        cap->mApplicationInfo->SetFlags(capFlags);
    }
    if (supportsLargeScreens < 0 || (supportsLargeScreens > 0
        && capSdkVersion >= Build::VERSION_CODES::DONUT)) {
        capFlags |= ApplicationInfo_FLAG_SUPPORTS_LARGE_SCREENS;
        cap->mApplicationInfo->SetFlags(capFlags);
    }
    if (supportsXLargeScreens < 0 || (supportsXLargeScreens > 0
        && capSdkVersion >= Build::VERSION_CODES::GINGERBREAD)) {
        capFlags |= ApplicationInfo_FLAG_SUPPORTS_XLARGE_SCREENS;
        cap->mApplicationInfo->SetFlags(capFlags);
    }
    if (resizeable < 0 || (resizeable > 0
        && capSdkVersion >= Build::VERSION_CODES::DONUT)) {
        capFlags |= ApplicationInfo_FLAG_RESIZEABLE_FOR_SCREENS;
        cap->mApplicationInfo->SetFlags(capFlags);
    }
    if (anyDensity < 0 || (anyDensity > 0
        && capSdkVersion >= Build::VERSION_CODES::DONUT)) {
        capFlags |= ApplicationInfo_FLAG_SUPPORTS_SCREEN_DENSITIES;
        cap->mApplicationInfo->SetFlags(capFlags);
    }

    //Todo: add code
    cap->SetCapsuleName(capName);

    *capsule = cap;
    (*capsule)->AddRef();
    return NOERROR;
}

static
Int32 R_Styleable_AndroidManifestPermissionGroup[] = {
    0x01010001, 0x01010002, 0x01010003, 0x01010020
};

CapsuleParser::PermissionGroup* CapsuleParser::ParsePermissionGroup(
    /* [in] */ Capsule* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ ArrayOf<String>* outError)
{
    PermissionGroup* perm = new PermissionGroup(owner);

    AutoPtr<ITypedArray> sa;
    FAIL_RETURN_NULL(res->ObtainAttributes(attrs,
        ArrayOf<Int32>(R_Styleable_AndroidManifestPermissionGroup,
            sizeof(R_Styleable_AndroidManifestPermissionGroup) / sizeof(Int32))
        /*com.android.internal.R.styleable.AndroidManifestPermissionGroup*/,
        (ITypedArray**)&sa));

    if (!ParseCapsuleItemInfo(owner, perm->mInfo, outError,
        "<permission-group>", sa,
        2 /*com.android.internal.R.styleable.AndroidManifestPermissionGroup_name*/,
        0 /*com.android.internal.R.styleable.AndroidManifestPermissionGroup_label*/,
        1 /*com.android.internal.R.styleable.AndroidManifestPermissionGroup_icon*/,
        0)) {
        sa->Recycle();
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    Int32 desRes;
    sa->GetResourceId(
        3 /*com.android.internal.R.styleable.AndroidManifestPermissionGroup_description*/,
        0, &desRes);
    perm->mInfo->SetDescriptionRes(desRes);

    sa->Recycle();

    if (!ParseAllMetaData(res, parser, attrs, String("<permission-group>"), perm,
        outError)) {
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    owner->mPermissionGroups.PushBack(perm);

    return perm;
}

CapsuleParser::Permission* CapsuleParser::ParsePermission(
    /* [in] */ Capsule* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ ArrayOf<String>* outError)
{
    Permission* perm = new Permission(owner);

    AutoPtr<ITypedArray> sa;
    FAIL_RETURN_NULL(res->ObtainAttributes(attrs,
        ArrayOf<Int32>(R_Styleable_AndroidManifestPermissionGroup,
            sizeof(R_Styleable_AndroidManifestPermissionGroup) / sizeof(Int32))
        /*com.android.internal.R.styleable.AndroidManifestPermission*/,
        (ITypedArray**)&sa));

    if (!ParseCapsuleItemInfo(owner, perm->mInfo, outError,
        "<permission>", sa,
        2 /*com.android.internal.R.styleable.AndroidManifestPermission_name*/,
        0 /*com.android.internal.R.styleable.AndroidManifestPermission_label*/,
        1 /*com.android.internal.R.styleable.AndroidManifestPermission_icon*/,
        0)) {
        sa->Recycle();
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    // Note: don't allow this value to be a reference to a resource
    // that may change.
    String group;
    sa->GetNonResourceString(
        4 /*com.android.internal.R.styleable.AndroidManifestPermission_permissionGroup*/,
        &group);
    perm->mInfo->SetGroup(group);

    Int32 desRes;
    sa->GetResourceId(
        5 /*com.android.internal.R.styleable.AndroidManifestPermission_description*/,
        0, &desRes);
    perm->mInfo->SetDescriptionRes(desRes);

    Int32 level;
    sa->GetInt32(
        3 /*com.android.internal.R.styleable.AndroidManifestPermission_protectionLevel*/,
        PermissionInfo_PROTECTION_NORMAL, &level);
    perm->mInfo->SetProtectionLevel(level);

    sa->Recycle();

    if (level == -1) {
        (*outError)[0] = "<permission> does not specify protectionLevel";
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    if (!ParseAllMetaData(res, parser, attrs, "<permission>", perm,
        outError)) {
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    owner->mPermissions.PushBack(perm);

    return perm;
}

static
Int32 R_Styleable_AndroidManifestPermissionTree[] = {
    0x01010001, 0x01010002, 0x01010003
};

CapsuleParser::Permission* CapsuleParser::ParsePermissionTree(
    /* [in] */ Capsule* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ ArrayOf<String>* outError)
{
    Permission* perm = new Permission(owner);

    AutoPtr<ITypedArray> sa;
    FAIL_RETURN_NULL(res->ObtainAttributes(attrs,
        ArrayOf<Int32>(R_Styleable_AndroidManifestPermissionTree,
            sizeof(R_Styleable_AndroidManifestPermissionTree) / sizeof(Int32))
        /*com.android.internal.R.styleable.AndroidManifestPermissionTree*/,
        (ITypedArray**)&sa));

    if (!ParseCapsuleItemInfo(owner, perm->mInfo, outError,
        "<permission-tree>", sa,
        2 /*com.android.internal.R.styleable.AndroidManifestPermissionTree_name*/,
        0 /*com.android.internal.R.styleable.AndroidManifestPermissionTree_label*/,
        1 /*com.android.internal.R.styleable.AndroidManifestPermissionTree_icon*/,
        0)) {
        sa->Recycle();
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    sa->Recycle();

    String permName;
    perm->mInfo->GetName(&permName);
    Int32 index = permName.IndexOf('.');
    if (index > 0) {
        index = permName.Substring(index + 1).IndexOf('.');
    }
    if (index < 0) {
        (*outError)[0] = (const char*)(
            StringBuffer("<permission-tree> name has less than three segments: ")
            + permName);
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    perm->mInfo->SetDescriptionRes(0);
    perm->mInfo->SetProtectionLevel(PermissionInfo_PROTECTION_NORMAL);
    perm->mTree = TRUE;

    if (!ParseAllMetaData(res, parser, attrs, "<permission-tree>", perm,
        outError)) {
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    owner->mPermissions.PushBack(perm);

    return perm;
}

static
Int32 R_Styleable_AndroidManifestInstrumentation[] = {
    0x01010001, 0x01010002, 0x01010003, 0x01010021,
    0x01010022, 0x01010023
};

CapsuleParser::Instrumentation* CapsuleParser::ParseInstrumentation(
    /* [in] */ Capsule* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ ArrayOf<String>* outError)
{
    AutoPtr<ITypedArray> sa;
    FAIL_RETURN_NULL(res->ObtainAttributes(attrs,
        ArrayOf<Int32>(R_Styleable_AndroidManifestInstrumentation,
            sizeof(R_Styleable_AndroidManifestInstrumentation) / sizeof(Int32))
        /*com.android.internal.R.styleable.AndroidManifestInstrumentation*/,
        (ITypedArray**)&sa));

    if (mParseInstrumentationArgs == NULL) {
        mParseInstrumentationArgs = new ParseCapsuleItemArgs(owner, outError,
            2 /*com.android.internal.R.styleable.AndroidManifestInstrumentation_name*/,
            0 /*com.android.internal.R.styleable.AndroidManifestInstrumentation_label*/,
            1 /*com.android.internal.R.styleable.AndroidManifestInstrumentation_icon*/,
            0);
        mParseInstrumentationArgs->mTag = "<instrumentation>";
    }

    mParseInstrumentationArgs->mSa = sa;

    AutoPtr<IInstrumentationInfo> info;
    FAIL_RETURN_NULL(CInstrumentationInfo::New((IInstrumentationInfo**)&info));
    Instrumentation* a = new Instrumentation(mParseInstrumentationArgs, info.Get());
    if (!(*outError)[0].IsNull()) {
        sa->Recycle();
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    // Note: don't allow this value to be a reference to a resource
    // that may change.
    String targetCapsule;
    sa->GetNonResourceString(
        3 /*com.android.internal.R.styleable.AndroidManifestInstrumentation_targetCapsule*/,
        &targetCapsule);
    a->mInfo->SetTargetCapsule(targetCapsule);

    Boolean bvalue;
    sa->GetBoolean(
        4 /*com.android.internal.R.styleable.AndroidManifestInstrumentation_handleProfiling*/,
        FALSE, &bvalue);
    a->mInfo->SetHandleProfiling(bvalue);

    sa->GetBoolean(
        5 /*com.android.internal.R.styleable.AndroidManifestInstrumentation_functionalTest*/,
        FALSE, &bvalue);
    a->mInfo->SetFunctionalTest(bvalue);

    sa->Recycle();

    if (targetCapsule.IsNull()) {
        (*outError)[0] = "<instrumentation> does not specify targetCapsule";
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    if (!ParseAllMetaData(res, parser, attrs, "<instrumentation>", a,
        outError)) {
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    owner->mInstrumentation.PushBack(a);

    return a;
}

Boolean CapsuleParser::ParseCapsuleItemInfo(
    /* [in] */ Capsule* owner,
    /* [in] */ ICapsuleItemInfo* outInfo,
    /* [in] */ ArrayOf<String>* outError,
    /* [in] */ const char* tag,
    /* [in] */ ITypedArray* sa,
    /* [in] */ Int32 nameRes,
    /* [in] */ Int32 labelRes,
    /* [in] */ Int32 iconRes,
    /* [in] */ Int32 logoRes)
{
    String name;
    sa->GetNonConfigurationString(nameRes, 0, &name);
    if (name.IsNull()) {
        (*outError)[0] = (const char*)(
            StringBuffer(tag) + " does not specify android:name");
        return FALSE;
    }

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(name, (ICharSequence**)&cs);
    String ownerCName;
    owner->mApplicationInfo->GetCapsuleName(&ownerCName);
    BuildClassName(ownerCName, cs, outError, &name);
    outInfo->SetName(name);
    if (name.IsNull()) {
        return FALSE;
    }

    Int32 iconVal;
    sa->GetResourceId(iconRes, 0, &iconVal);
    if (iconVal != 0) {
        outInfo->SetIcon(iconVal);
        outInfo->SetNonLocalizedLabel(NULL);
    }

    AutoPtr<ITypedValue> v;
    sa->PeekValue(labelRes, (ITypedValue**)&v);
    Int32 vResId;
    if (v != NULL
        && (v->GetResourceId(&vResId), outInfo->SetLabelRes(vResId), vResId) == 0) {
        AutoPtr<ICharSequence> label;
        v->CoerceToString((ICharSequence**)&label);
        outInfo->SetNonLocalizedLabel(label);
    }

    outInfo->SetCapsuleName(owner->mCapsuleName);

    return TRUE;
}

static
Int32 R_Styleable_AndroidManifestApplication[] = {
    0x01010000, 0x01010001, 0x01010002, 0x01010003,
    0x01010004, 0x01010005, 0x01010006, 0x0101000c,
    0x0101000d, 0x0101000e, 0x0101000f, 0x01010011,
    0x01010012, 0x01010020, 0x01010204, 0x01010272,
    0x0101027f, 0x01010280, 0x0101029c, 0x0101029d,
    0x010102b8, 0x010102ba, 0x010102db, 0x010102dc
};

static
Int32 R_Styleable_AndroidManifestUsesLibrary[] = {
    0x01010003, 0x0101028e
};

Boolean CapsuleParser::ParseApplication(
    /* [in] */ Capsule* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* outError)
{
    IApplicationInfo* ai = owner->mApplicationInfo;
    String capName;
    ai->GetCapsuleName(&capName);

    AutoPtr<ITypedArray> sa;
    FAIL_RETURN(res->ObtainAttributes(
        attrs, ArrayOf<Int32>(R_Styleable_AndroidManifestApplication,
        sizeof(R_Styleable_AndroidManifestApplication) / sizeof(Int32)),/*com.android.internal.R.styleable.AndroidManifestApplication*/
        (ITypedArray**)&sa));

    String name;
    sa->GetNonConfigurationString(
        3 /*com.android.internal.R.styleable.AndroidManifestApplication_name*/, 0,
        &name);
    if (!name.IsNull()) {
        AutoPtr<ICharSequence> cname;
        CStringWrapper::New(name, (ICharSequence**)&cname);
        String clsName;
        BuildClassName(capName, cname.Get(), outError, &clsName);
        ai->SetClassName(clsName);
        if (clsName.IsNull()) {
            sa->Recycle();
            mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
            return FALSE;
        }
    }

    String manageSpaceActivity;
    sa->GetNonConfigurationString(
        4 /*com.android.internal.R.styleable.AndroidManifestApplication_manageSpaceActivity*/, 0,
        &manageSpaceActivity);
    if (!manageSpaceActivity.IsNull()) {
        AutoPtr<ICharSequence> ca;
        CStringWrapper::New(manageSpaceActivity, (ICharSequence**)&ca);
        String name;
        BuildClassName(capName, ca.Get(), outError, &name);
        ai->SetManageSpaceActivityName(name);
    }

    Int32 aiFlags;
    Boolean allowBackup = FALSE;
    sa->GetBoolean(
        17 /*com.android.internal.R.styleable.AndroidManifestApplication_allowBackup*/, TRUE,
        &allowBackup);
    if (allowBackup) {
        ai->GetFlags(&aiFlags);
        aiFlags |= ApplicationInfo_FLAG_ALLOW_BACKUP;
        ai->SetFlags(aiFlags);

        // backupAgent, killAfterRestore, and restoreAnyVersion are only relevant
        // if backup is possible for the given application.
        String backupAgent;
        sa->GetNonConfigurationString(
            16 /*com.android.internal.R.styleable.AndroidManifestApplication_backupAgent*/, 0,
            &backupAgent);
        if (!backupAgent.IsNull()) {
            AutoPtr<ICharSequence> cb;
            CStringWrapper::New(backupAgent, (ICharSequence**)&cb);
            String agentName;
            BuildClassName(capName, cb.Get(), outError, &agentName);
            ai->SetBackupAgentName(agentName);
            if (FALSE) {
                String cbStr;
                cb->ToString(&cbStr);
                Logger::V(TAG, StringBuffer("android:backupAgent = ") + agentName
                        + " from " + capName + "+" + cbStr);
            }

            Boolean value = FALSE;
            sa->GetBoolean(
                18 /*com.android.internal.R.styleable.AndroidManifestApplication_killAfterRestore*/,
                TRUE, &value);
            if (value) {
                aiFlags |= ApplicationInfo_FLAG_KILL_AFTER_RESTORE;
                ai->SetFlags(aiFlags);
            }

            sa->GetBoolean(
                21 /*com.android.internal.R.styleable.AndroidManifestApplication_restoreAnyVersion*/,
                FALSE, &value);
            if (value) {
                aiFlags |= ApplicationInfo_FLAG_RESTORE_ANY_VERSION;
                ai->SetFlags(aiFlags);
            }
        }
    }

    AutoPtr<ITypedValue> v;
    FAIL_RETURN(sa->PeekValue(
        1 /*com.android.internal.R.styleable.AndroidManifestApplication_label*/, (ITypedValue**)&v));
    Int32 ivalue;
    if (v != NULL && (v->GetResourceId(&ivalue), ai->SetLabelRes(ivalue), ivalue) == 0) {
        AutoPtr<ICharSequence> label;
        v->CoerceToString((ICharSequence**)&label);
        ai->SetNonLocalizedLabel(label);
    }

    sa->GetResourceId(
        2 /*com.android.internal.R.styleable.AndroidManifestApplication_icon*/, 0,
        &ivalue);
    ai->SetIcon(ivalue);
    sa->GetResourceId(
        0 /*com.android.internal.R.styleable.AndroidManifestApplication_theme*/, 0,
        &ivalue);
    ai->SetTheme(ivalue);
    sa->GetResourceId(
        13 /*com.android.internal.R.styleable.AndroidManifestApplication_description*/, 0,
        &ivalue);
    ai->SetDescriptionRes(ivalue);

    if ((flags & PARSE_IS_SYSTEM) != 0) {
        Boolean value = FALSE;
        sa->GetBoolean(
            8 /*com.android.internal.R.styleable.AndroidManifestApplication_persistent*/,
            FALSE, &value);
        if (value) {
            aiFlags |= ApplicationInfo_FLAG_PERSISTENT;
            ai->SetFlags(aiFlags);
        }
    }

    if ((flags & PARSE_FORWARD_LOCK) != 0) {
        aiFlags |= ApplicationInfo_FLAG_FORWARD_LOCK;
        ai->SetFlags(aiFlags);
    }

    if ((flags & PARSE_ON_SDCARD) != 0) {
        aiFlags |= ApplicationInfo_FLAG_EXTERNAL_STORAGE;
        ai->SetFlags(aiFlags);
    }

    Boolean bvalue = FALSE;

    sa->GetBoolean(
        10 /*com.android.internal.R.styleable.AndroidManifestApplication_debuggable*/,
        FALSE, &bvalue);
    if (bvalue) {
        aiFlags |= ApplicationInfo_FLAG_DEBUGGABLE;
        ai->SetFlags(aiFlags);
    }

    sa->GetBoolean(
        20 /*com.android.internal.R.styleable.AndroidManifestApplication_vmSafeMode*/,
        FALSE, &bvalue);
    if (bvalue) {
        aiFlags |= ApplicationInfo_FLAG_VM_SAFE_MODE;
        ai->SetFlags(aiFlags);
    }

    sa->GetBoolean(
        7 /*com.android.internal.R.styleable.AndroidManifestApplication_hasCode*/,
        TRUE, &bvalue);
    if (bvalue) {
        aiFlags |= ApplicationInfo_FLAG_HAS_CODE;
        ai->SetFlags(aiFlags);
    }

    sa->GetBoolean(
        14 /*com.android.internal.R.styleable.AndroidManifestApplication_allowTaskReparenting*/,
        FALSE, &bvalue);
    if (bvalue) {
        aiFlags |= ApplicationInfo_FLAG_ALLOW_TASK_REPARENTING;
        ai->SetFlags(aiFlags);
    }

    sa->GetBoolean(
        5 /*com.android.internal.R.styleable.AndroidManifestApplication_allowClearUserData*/,
        TRUE, &bvalue);
    if (bvalue) {
        aiFlags |= ApplicationInfo_FLAG_ALLOW_CLEAR_USER_DATA;
        ai->SetFlags(aiFlags);
    }

    sa->GetBoolean(
        15 /*com.android.internal.R.styleable.AndroidManifestApplication_testOnly*/,
        FALSE, &bvalue);
    if (bvalue) {
        aiFlags |= ApplicationInfo_FLAG_TEST_ONLY;
        ai->SetFlags(aiFlags);
    }

    sa->GetBoolean(
        22 /*com.android.internal.R.styleable.AndroidManifestApplication_neverEncrypt*/,
        FALSE, &bvalue);
    if (bvalue) {
        aiFlags |= ApplicationInfo_FLAG_NEVER_ENCRYPT;
        ai->SetFlags(aiFlags);
    }

    String str;
    sa->GetNonConfigurationString(
        6 /*com.android.internal.R.styleable.AndroidManifestApplication_permission*/, 0, &str);
    ai->SetPermission((!str.IsNullOrEmpty()) ? str : String(NULL));
    Int32 ownerSdkVersion;
    owner->mApplicationInfo->GetTargetSdkVersion(&ownerSdkVersion);
    if (ownerSdkVersion >= Build::VERSION_CODES::FROYO) {
        sa->GetNonConfigurationString(
            12 /*com.android.internal.R.styleable.AndroidManifestApplication_taskAffinity*/, 0, &str);
    }
    else {
        // Some older apps have been seen to use a resource reference
        // here that on older builds was ignored (with a warning).  We
        // need to continue to do this for them so they don't break.
        sa->GetNonResourceString(
            12 /*com.android.internal.R.styleable.AndroidManifestApplication_taskAffinity*/, &str);
    }
    AutoPtr<ICharSequence> cStr;
    CStringWrapper::New(str, (ICharSequence**)&cStr);
    String aiTask;
    BuildTaskAffinityName(capName, capName,
            cStr.Get(), outError, &aiTask);
    ai->SetTaskAffinity(aiTask);

    if ((*outError)[0].IsNull()) {
        String pname;
        if (ownerSdkVersion >= Build::VERSION_CODES::FROYO) {
            sa->GetNonConfigurationString(
                11 /*com.android.internal.R.styleable.AndroidManifestApplication_process*/, 0,
                &pname);
        }
        else {
            // Some older apps have been seen to use a resource reference
            // here that on older builds was ignored (with a warning).  We
            // need to continue to do this for them so they don't break.
            sa->GetNonResourceString(
                11 /*com.android.internal.R.styleable.AndroidManifestApplication_process*/,
                &pname);
        }
        AutoPtr<ICharSequence> cpname;
        CStringWrapper::New(pname, (ICharSequence**)&cpname);
        String aiPName;
        BuildProcessName(capName, String(NULL),
            cpname.Get(), flags, mSeparateProcesses.Get(), outError, &aiPName);
        ai->SetProcessName(aiPName);

        Boolean aiEnabled;
        sa->GetBoolean(
            9 /*com.android.internal.R.styleable.AndroidManifestApplication_enabled*/, TRUE,
            &aiEnabled);
        ai->SetEnabled(aiEnabled);

        if (FALSE) {
            Boolean value = FALSE;
            sa->GetBoolean(
                23 /*com.android.internal.R.styleable.AndroidManifestApplication_cantSaveState*/,
                FALSE, &value);
            if (value) {
                aiFlags |= ApplicationInfo_FLAG_CANT_SAVE_STATE;
                ai->SetFlags(aiFlags);

                // A heavy-weight application can not be in a custom process.
                // We can do direct compare because we intern all strings.
                if (!aiPName.IsNull() && !aiPName.Equals(capName)) {
                    (*outError)[0] = "cantSaveState applications can not use custom processes";
                }
            }
        }
    }

    sa->Recycle();

    if (!(*outError)[0].IsNull()) {
        mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return FALSE;
    }

    Int32 innerDepth = 0;
    parser->GetDepth(&innerDepth);

    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser_END_DOCUMENT
           && (type != IXmlPullParser_END_TAG
           || (parser->GetDepth(&depth), depth) > innerDepth)) {
        if (type == IXmlPullParser_END_TAG || type == IXmlPullParser_TEXT) {
            continue;
        }

        String tagName;
        String tmpName;
        parser->GetName(&tagName);
        if (tagName.Equals("activity")) {
            Activity* a = NULL;
            if (FAILED(ParseActivity(
                    owner, res, parser, attrs, flags, outError, FALSE, &a))) {
                mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner->mActivities.PushBack(a);

        } else if (tagName.Equals("receiver")) {
            Activity* a = NULL;
            if (FAILED(ParseActivity(
                    owner, res, parser, attrs, flags, outError, TRUE, &a))) {
                mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner->mReceivers.PushBack(a);

        } else if (tagName.Equals("service")) {
            Service* s = NULL;
            if (FAILED(ParseService(owner, res, parser, attrs, flags, outError, &s))) {
                mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner->mServices.PushBack(s);

        } else if (tagName.Equals("provider")) {
            ContentProvider* p = NULL;
            if (FAILED(ParseContentProvider(
                    owner, res, parser, attrs, flags, outError, &p))) {
                mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner->mContentProviders.PushBack(p);

        } else if (tagName.Equals("activity-alias")) {
            Activity* a = NULL;
            if (FAILED(ParseActivityAlias(
                    owner, res, parser, attrs, flags, outError, &a))) {
                mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner->mActivities.PushBack(a);

        } else if ((parser->GetName(&tmpName), tmpName).Equals("meta-data")) {
            // note: application meta-data is stored off to the side, so it can
            // remain null in the primary copy (we like to avoid extra copies because
            // it can be large)
            if ((owner->mAppMetaData = ParseMetaData(res, parser, attrs, owner->mAppMetaData,
                    outError)) == NULL) {
                mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

        } else if (tagName.Equals("uses-library")) {
            sa->Release();
            res->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_AndroidManifestUsesLibrary,
                    sizeof(R_Styleable_AndroidManifestUsesLibrary) / sizeof(Int32))
                /*com.android.internal.R.styleable.AndroidManifestUsesLibrary*/,
                (ITypedArray**)&sa);

            // Note: don't allow this value to be a reference to a resource
            // that may change
            String lname;
            sa->GetNonResourceString(
                0 /*com.android.internal.R.styleable.AndroidManifestUsesLibrary_name*/, &lname);
            Boolean req = FALSE;
            sa->GetBoolean(
                1 /*com.android.internal.R.styleable.AndroidManifestUsesLibrary_required*/,
                TRUE, &req);

            sa->Recycle();

            if (!lname.IsNull()) {
                if (req) {
                    if (owner->mUsesLibraries == NULL) {
                        owner->mUsesLibraries = new List<String>();
                    }
                    if (Find(owner->mUsesLibraries->Begin(), owner->mUsesLibraries->End(), lname)
                        == owner->mUsesLibraries->End()) {
                        owner->mUsesLibraries->PushBack(lname);
                    }
                }
                else {
                    if (owner->mUsesOptionalLibraries == NULL) {
                        owner->mUsesOptionalLibraries = new List<String>();
                    }
                    if (Find(owner->mUsesOptionalLibraries->Begin(),
                            owner->mUsesOptionalLibraries->End(), lname)
                        == owner->mUsesOptionalLibraries->End()) {
                        owner->mUsesOptionalLibraries->PushBack(lname);
                    }
                }
            }

            XmlUtils::SkipCurrentTag(parser);

        } else if (tagName.Equals("uses-package")) {
            // Dependencies for app installers; we don't currently try to
            // enforce this.
            XmlUtils::SkipCurrentTag(parser);

        } else {
            if (!RIGID_PARSER) {
                String des;
                parser->GetPositionDescription(&des);
                Logger::W(TAG, StringBuffer("Unknown element under <application>: ") + tagName
                        + " at " + mArchiveSourcePath + " " + des);
                XmlUtils::SkipCurrentTag(parser);
                continue;
            } else {
                (*outError)[0] = (const char*)(StringBuffer("Bad element under <application>: ") + tagName);
                mParseError = CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }
        }
    }

    return TRUE;
}

static Int32 R_Styleable_AndroidManifestActivity[] = {
    0x01010000, 0x01010001, 0x01010002, 0x01010003,
    0x01010006, 0x0101000e, 0x01010010, 0x01010011,
    0x01010012, 0x01010013, 0x01010014, 0x01010015,
    0x01010016, 0x01010017, 0x0101001d, 0x0101001e,
    0x0101001f, 0x01010020, 0x01010203, 0x01010204,
    0x0101022b, 0x0101022d, 0x010102a7
};

ECode CapsuleParser::ParseActivity(
    /* [in] */ CapsuleParser::Capsule* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* outError,
    /* [in] */ Boolean receiver,
    /* [out] */ Activity** activity)
{
    assert(res);
    assert(attrs);
    assert(activity);
    *activity = NULL;

    AutoPtr<ITypedArray> sa;

    FAIL_RETURN(res->ObtainAttributes(attrs, ArrayOf<Int32>(R_Styleable_AndroidManifestActivity,
            sizeof(R_Styleable_AndroidManifestActivity) / sizeof(Int32)),
            (ITypedArray**)&sa));

    if (mParseActivityArgs == NULL) {
        mParseActivityArgs = new ParseComponentArgs(owner, outError,
                3, // com.android.internal.R.styleable.AndroidManifestActivity_name,
                1, // com.android.internal.R.styleable.AndroidManifestActivity_label,
                2, // com.android.internal.R.styleable.AndroidManifestActivity_icon,
                0,
                NULL, // mSeparateProcesses,
                7,  // com.android.internal.R.styleable.AndroidManifestActivity_process,
                17, // com.android.internal.R.styleable.AndroidManifestActivity_description,
                5   // com.android.internal.R.styleable.AndroidManifestActivity_enabled
                );
    }

    mParseActivityArgs->mTag = receiver ? "<receiver>" : "<activity>";
    mParseActivityArgs->mSa = sa;
    mParseActivityArgs->mFlags = flags;

    AutoPtr<IActivityInfo> info;
    FAIL_RETURN(CActivityInfo::New((IActivityInfo**)&info));

    Activity* a = new Activity(mParseActivityArgs, info);
    if (!mParseActivityArgs) {
        sa->Recycle();
        return E_OUT_OF_MEMORY_ERROR;
    }

    Boolean setExported = FALSE;
    sa->HasValue(6/*com.android.internal.R.styleable.AndroidManifestActivity_exported*/,
        &setExported);
    if (setExported) {
        Boolean isExported;
        sa->GetBoolean(6 /*com.android.internal.R.styleable.AndroidManifestActivity_exported*/,
            FALSE, &isExported);
        a->mInfo->SetExported(isExported);
    }

    Int32 ivalue;
    sa->GetResourceId(
        0 /*com.android.internal.R.styleable.AndroidManifestActivity_theme*/, 0,
        &ivalue);
    a->mInfo->SetTheme(ivalue);

    String str;
    sa->GetNonConfigurationString(
        4 /*com.android.internal.R.styleable.AndroidManifestActivity_permission*/, 0,
        &str);
    if (str.IsNull()) {
        String perm;
        owner->mApplicationInfo->GetPermission(&perm);
        a->mInfo->SetPermission(perm);
    }
    else {
        a->mInfo->SetPermission(str.GetLength() > 0 ? str : String(NULL));
    }

    sa->GetNonConfigurationString(
        8 /*com.android.internal.R.styleable.AndroidManifestActivity_taskAffinity*/, 0,
        &str);
    AutoPtr<ICharSequence> cStr;
    CStringWrapper::New(str, (ICharSequence**)&cStr);
    String ownerCName, ownerTaskAffinity;
    owner->mApplicationInfo->GetCapsuleName(&ownerCName);
    owner->mApplicationInfo->GetTaskAffinity(&ownerTaskAffinity);
    BuildTaskAffinityName(ownerCName, ownerTaskAffinity, cStr.Get(), outError, &str);
    a->mInfo->SetTaskAffinity(str);

    Boolean bvalue;
    ivalue = 0;
    a->mInfo->SetFlags(ivalue);
    sa->GetBoolean(
        9 /*com.android.internal.R.styleable.AndroidManifestActivity_multiprocess*/,
        FALSE, &bvalue);
    if (bvalue) {
        ivalue |= ActivityInfo_FLAG_MULTIPROCESS;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        10 /*com.android.internal.R.styleable.AndroidManifestActivity_finishOnTaskLaunch*/,
        FALSE, &bvalue);
    if (bvalue) {
        ivalue |= ActivityInfo_FLAG_FINISH_ON_TASK_LAUNCH;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        11 /*com.android.internal.R.styleable.AndroidManifestActivity_clearTaskOnLaunch*/,
        FALSE, &bvalue);
    if (bvalue) {
        ivalue |= ActivityInfo_FLAG_CLEAR_TASK_ON_LAUNCH;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        21 /*com.android.internal.R.styleable.AndroidManifestActivity_noHistory*/,
        FALSE, &bvalue);
    if (bvalue) {
        ivalue |= ActivityInfo_FLAG_NO_HISTORY;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        18 /*com.android.internal.R.styleable.AndroidManifestActivity_alwaysRetainTaskState*/,
        FALSE, &bvalue);
    if (bvalue) {
        ivalue |= ActivityInfo_FLAG_ALWAYS_RETAIN_TASK_STATE;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        12 /*com.android.internal.R.styleable.AndroidManifestActivity_stateNotNeeded*/,
        FALSE, &bvalue);
    if (bvalue) {
        ivalue |= ActivityInfo_FLAG_STATE_NOT_NEEDED;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        13 /*com.android.internal.R.styleable.AndroidManifestActivity_excludeFromRecents*/,
        FALSE, &bvalue);
    if (bvalue) {
        ivalue |= ActivityInfo_FLAG_EXCLUDE_FROM_RECENTS;
        a->mInfo->SetFlags(ivalue);
    }

    Int32 ownerFlags;
    owner->mApplicationInfo->GetFlags(&ownerFlags);
    sa->GetBoolean(
        19 /*com.android.internal.R.styleable.AndroidManifestActivity_allowTaskReparenting*/,
        (ownerFlags & ActivityInfo_FLAG_ALLOW_TASK_REPARENTING) != 0, &bvalue);
    if (bvalue) {
        ivalue |= ActivityInfo_FLAG_ALLOW_TASK_REPARENTING;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        22 /*com.android.internal.R.styleable.AndroidManifestActivity_finishOnCloseSystemDialogs*/,
        FALSE, &bvalue);
    if (bvalue) {
        ivalue |= ActivityInfo_FLAG_FINISH_ON_CLOSE_SYSTEM_DIALOGS;
        a->mInfo->SetFlags(ivalue);
    }

    if (!receiver) {
        sa->GetInt32(
            14 /*com.android.internal.R.styleable.AndroidManifestActivity_launchMode*/,
            ActivityInfo_LAUNCH_MULTIPLE, &ivalue);
        a->mInfo->SetLaunchMode(ivalue);
        sa->GetInt32(
            15 /*com.android.internal.R.styleable.AndroidManifestActivity_screenOrientation*/,
            ActivityInfo_SCREEN_ORIENTATION_UNSPECIFIED, &ivalue);
        a->mInfo->SetScreenOrientation(ivalue);
        sa->GetInt32(
            16 /*com.android.internal.R.styleable.AndroidManifestActivity_configChanges*/,
            0, &ivalue);
        a->mInfo->SetConfigChanges(ivalue);
        sa->GetInt32(
            20 /*com.android.internal.R.styleable.AndroidManifestActivity_windowSoftInputMode*/,
            0, &ivalue);
        a->mInfo->SetSoftInputMode(ivalue);
    }
    else {
        a->mInfo->SetLaunchMode(ActivityInfo_LAUNCH_MULTIPLE);
        a->mInfo->SetConfigChanges(0);
    }

    sa->Recycle();

    if (receiver && (ownerFlags & ApplicationInfo_FLAG_CANT_SAVE_STATE) != 0) {
        // A heavy-weight application can not have receives in its main process
        // We can do direct compare because we intern all strings.
        String pname;
        a->mInfo->GetProcessName(&pname);
        if (pname.Equals(owner->mCapsuleName)) {
            (*outError)[0] = "Heavy-weight applications can not have receivers in main process";
        }
    }

    if (!(*outError)[0].IsNull()) {
        return E_RUNTIME_EXCEPTION;
    }

    Int32 outerDepth;
    parser->GetDepth(&outerDepth);

    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser_END_DOCUMENT
           && (type != IXmlPullParser_END_TAG
                   || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser_END_TAG || type == IXmlPullParser_TEXT) {
            continue;
        }

        String name;
        parser->GetName(&name);
        if (name.Equals("intent-filter")) {
            ActivityIntentInfo* intent = new ActivityIntentInfo(a);
            if (!ParseIntent(res, parser, attrs, flags, intent, outError, !receiver)) {
                return E_XML_PULL_PARSER_EXCEPTION;
            }
            Int32 count;
            intent->CountActions(&count);
            if (count == 0) {
                String des;
                parser->GetPositionDescription(&des);
                Logger::W(TAG, StringBuffer("No actions in intent filter at ")
                        + mArchiveSourcePath + " " + des);
            }
            else {
                a->mIntents->PushBack(intent);
            }
        }
        else if (name.Equals("meta-data")) {
            if ((a->mMetaData = ParseMetaData(res, parser, attrs, a->mMetaData,
                    outError)) == NULL) {
                return E_XML_PULL_PARSER_EXCEPTION;
            }
        }
        else {
            String name;
            parser->GetName(&name);
            if (!RIGID_PARSER) {
                Logger::W(TAG, StringBuffer("Problem in package ") + mArchiveSourcePath + ":");
                String des;
                parser->GetPositionDescription(&des);
                if (receiver) {
                    Logger::W(TAG, StringBuffer("Unknown element under <receiver>: ") + name
                        + " at " + mArchiveSourcePath + " " + des);
                }
                else {
                    Logger::W(TAG, StringBuffer("Unknown element under <activity>: ") + name
                            + " at " + mArchiveSourcePath + " " + des);
                }
                XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            if (receiver) {
                (*outError)[0] = (const char*)(StringBuffer("Bad element under <receiver>: ") + name);
            }
            else {
                (*outError)[0] = (const char*)(StringBuffer("Bad element under <activity>: ") + name);
            }
            return E_XML_PULL_PARSER_EXCEPTION;
        }
    }

    if (!setExported) {
        a->mInfo->SetExported(a->mIntents->Begin() != a->mIntents->End());
    }

    *activity = a;
    return NOERROR;
}

static
Int32 R_Styleable_AndroidManifestActivityAlias[] = {
    0x01010001, 0x01010002, 0x01010003, 0x01010006,
    0x0101000e, 0x01010010, 0x01010020, 0x01010202
};

ECode CapsuleParser::ParseActivityAlias(
    /* [in] */ Capsule* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* outError,
    /* [out] */ Activity** alias)
{
    assert(alias);
    *alias = NULL;

    AutoPtr<ITypedArray> sa;
    FAIL_RETURN(res->ObtainAttributes(attrs,
        ArrayOf<Int32>(R_Styleable_AndroidManifestActivityAlias,
            sizeof(R_Styleable_AndroidManifestActivityAlias) / sizeof(Int32)
        /*com.android.internal.R.styleable.AndroidManifestActivityAlias*/),
        (ITypedArray**)&sa));

    String targetActivity;
    sa->GetNonConfigurationString(
        7 /*com.android.internal.R.styleable.AndroidManifestActivityAlias_targetActivity*/, 0,
        &targetActivity);
    if (targetActivity.IsNull()) {
        (*outError)[0] = "<activity-alias> does not specify android:targetActivity";
        sa->Recycle();
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<ICharSequence> c;
    CStringWrapper::New(targetActivity, (ICharSequence**)&c);
    String ownerCName;
    owner->mApplicationInfo->GetCapsuleName(&ownerCName);
    BuildClassName(ownerCName, c.Get(), outError, &targetActivity);
    if (targetActivity.IsNull()) {
        sa->Recycle();
        return E_RUNTIME_EXCEPTION;
    }

    if (mParseActivityAliasArgs == NULL) {
        mParseActivityAliasArgs = new ParseComponentArgs(owner, outError,
                2 /*com.android.internal.R.styleable.AndroidManifestActivityAlias_name*/,
                0 /*com.android.internal.R.styleable.AndroidManifestActivityAlias_label*/,
                1 /*com.android.internal.R.styleable.AndroidManifestActivityAlias_icon*/,
                0,
                mSeparateProcesses.Get(),
                0,
                6 /*com.android.internal.R.styleable.AndroidManifestActivityAlias_description*/,
                4 /*com.android.internal.R.styleable.AndroidManifestActivityAlias_enabled*/);
        mParseActivityAliasArgs->mTag = "<activity-alias>";
    }

    mParseActivityAliasArgs->mSa = sa;
    mParseActivityAliasArgs->mFlags = flags;

    Activity* target = NULL;

    List<Activity*>::Iterator itor;
    for (itor = owner->mActivities.Begin();
         itor != owner->mActivities.End();
         ++itor) {
        String name;
        (*itor)->mInfo->GetName(&name);
        if (targetActivity.Equals(name)) {
            target = *itor;
            break;
        }
    }

    if (target == NULL) {
        (*outError)[0] = (const char*)(
            StringBuffer("<activity-alias> target activity ") + targetActivity
            + " not found in manifest");
        sa->Recycle();
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<IActivityInfo> info;
    FAIL_RETURN(CActivityInfo::New((IActivityInfo**)&info));
    info->SetTargetActivity(targetActivity);
    Int32 ivalue;
    target->mInfo->GetConfigChanges(&ivalue);
    info->SetConfigChanges(ivalue);
    target->mInfo->GetFlags(&ivalue);
    info->SetFlags(ivalue);
    target->mInfo->GetIcon(&ivalue);
    info->SetIcon(ivalue);
    target->mInfo->GetLogo(&ivalue);
    info->SetLogo(ivalue);
    target->mInfo->GetLabelRes(&ivalue);
    info->SetLabelRes(ivalue);
    AutoPtr<ICharSequence> label;
    target->mInfo->GetNonLocalizedLabel((ICharSequence**)&label);
    info->SetNonLocalizedLabel(label);
    target->mInfo->GetLaunchMode(&ivalue);
    info->SetLaunchMode(ivalue);
    String svalue;
    target->mInfo->GetProcessName(&svalue);
    info->SetProcessName(svalue);
    info->GetDescriptionRes(&ivalue);
    if (ivalue == 0) {
        target->mInfo->GetDescriptionRes(&ivalue);
        info->SetDescriptionRes(ivalue);
    }
    target->mInfo->GetScreenOrientation(&ivalue);
    info->SetScreenOrientation(ivalue);
    target->mInfo->GetTaskAffinity(&svalue);
    info->SetTaskAffinity(svalue);
    target->mInfo->GetTheme(&ivalue);
    info->SetTheme(ivalue);

    Activity* a = new Activity(mParseActivityAliasArgs, info.Get());
    if (!(*outError)[0].IsNull()) {
        sa->Recycle();
        return E_RUNTIME_EXCEPTION;
    }

    Boolean setExported;
    sa->HasValue(
        5 /*com.android.internal.R.styleable.AndroidManifestActivityAlias_exported*/,
        &setExported);
    if (setExported) {
        Boolean isExported;
        sa->GetBoolean(
            5 /*com.android.internal.R.styleable.AndroidManifestActivityAlias_exported*/,
            FALSE, &isExported);
        a->mInfo->SetExported(isExported);
    }

    String str;
    sa->GetNonConfigurationString(
        3 /*com.android.internal.R.styleable.AndroidManifestActivityAlias_permission*/,
        0, &str);
    if (!str.IsNull()) {
        a->mInfo->SetPermission(str.GetLength() > 0 ? str : String(NULL));
    }

    sa->Recycle();

    if (!(*outError)[0].IsNull()) {
        return E_RUNTIME_EXCEPTION;
    }

    Int32 outerDepth = 0;
    parser->GetDepth(&outerDepth);
    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser_END_DOCUMENT
           && (type != IXmlPullParser_END_TAG
                   || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser_END_TAG || type == IXmlPullParser_TEXT) {
            continue;
        }

        String name;
        parser->GetName(&name);
        if (name.Equals("intent-filter")) {
            ActivityIntentInfo* intent = new ActivityIntentInfo(a);
            if (!ParseIntent(res, parser, attrs, flags, intent, outError, TRUE)) {
                return E_XML_PULL_PARSER_EXCEPTION;
            }
            Int32 actions = 0;
            intent->CountActions(&actions);
            if (actions == 0) {
                String des;
                parser->GetPositionDescription(&des);
                Logger::W(TAG, StringBuffer("No actions in intent filter at ")
                        + mArchiveSourcePath + " " + des);
            } else {
                a->mIntents->PushBack(intent);
            }
        } else if (name.Equals("meta-data")) {
            if ((a->mMetaData = ParseMetaData(res, parser, attrs, a->mMetaData,
                    outError)) == NULL) {
                return E_XML_PULL_PARSER_EXCEPTION;
            }
        } else {
            if (!RIGID_PARSER) {
                String des;
                parser->GetPositionDescription(&des);
                Logger::W(TAG, StringBuffer("Unknown element under <activity-alias>: ")
                        + name + " at " + mArchiveSourcePath + " " + des);
                XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            (*outError)[0] = (const char*)(
                StringBuffer("Bad element under <activity-alias>: ") + name);
            return E_RUNTIME_EXCEPTION;
        }
    }

    if (!setExported) {
        a->mInfo->SetExported(a->mIntents->Begin() != a->mIntents->End());
    }

    *alias = a;
    return NOERROR;
}

static
Int32 R_Styleable_AndroidManifestProvider[] = {
    0x01010001, 0x01010002, 0x01010003, 0x01010006,
    0x01010007, 0x01010008, 0x0101000e, 0x01010010,
    0x01010011, 0x01010013, 0x01010018, 0x01010019,
    0x0101001a, 0x0101001b, 0x01010020
};

ECode CapsuleParser::ParseContentProvider(
    /* [in] */ Capsule* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* outError,
    /* [out] */ ContentProvider** provider)
{
    assert(provider);
    *provider = NULL;

    AutoPtr<ITypedArray> sa;
    FAIL_RETURN(res->ObtainAttributes(attrs,
        ArrayOf<Int32>(R_Styleable_AndroidManifestProvider,
            sizeof(R_Styleable_AndroidManifestProvider) / sizeof(Int32)
        /*com.android.internal.R.styleable.AndroidManifestProvider*/),
        (ITypedArray**)&sa));

    if (mParseProviderArgs == NULL) {
        mParseProviderArgs = new ParseComponentArgs(owner, outError,
                2 /*com.android.internal.R.styleable.AndroidManifestProvider_name*/,
                0 /*com.android.internal.R.styleable.AndroidManifestProvider_label*/,
                1 /*com.android.internal.R.styleable.AndroidManifestProvider_icon*/,
                0,
                mSeparateProcesses.Get(),
                8 /*com.android.internal.R.styleable.AndroidManifestProvider_process*/,
                14 /*com.android.internal.R.styleable.AndroidManifestProvider_description*/,
                6 /*com.android.internal.R.styleable.AndroidManifestProvider_enabled*/);
        mParseProviderArgs->mTag = "<provider>";
    }

    mParseProviderArgs->mSa = sa;
    mParseProviderArgs->mFlags = flags;

    AutoPtr<IContentProviderInfo> info;
    FAIL_RETURN(CContentProviderInfo::New((IContentProviderInfo**)&info));
    ContentProvider* p = new ContentProvider(mParseProviderArgs, info);
    if (!(*outError)[0].IsNull()) {
        sa->Recycle();
        return E_RUNTIME_EXCEPTION;
    }

    Boolean bvalue;
    sa->GetBoolean(
            7 /*com.android.internal.R.styleable.AndroidManifestProvider_exported*/,
            TRUE, &bvalue);
    p->mInfo->SetExported(bvalue);

    String cpname;
    sa->GetNonConfigurationString(
            10 /*com.android.internal.R.styleable.AndroidManifestProvider_authorities*/,
            0, &cpname);

    sa->GetBoolean(
            11 /*com.android.internal.R.styleable.AndroidManifestProvider_syncable*/,
            FALSE, &bvalue);
    p->mInfo->SetSyncable(bvalue);

    String permission;
    sa->GetNonConfigurationString(
            3 /*com.android.internal.R.styleable.AndroidManifestProvider_permission*/,
            0, &permission);
    String str;
    sa->GetNonConfigurationString(
            4 /*com.android.internal.R.styleable.AndroidManifestProvider_readPermission*/,
            0, &str);
    if (str.IsNull()) {
        str = permission;
    }
    if (str.IsNull()) {
        String perm;
        owner->mApplicationInfo->GetPermission(&perm);
        p->mInfo->SetReadPermission(perm);
    }
    else {
        p->mInfo->SetReadPermission(str.GetLength() > 0 ? str : String(NULL));
    }
    sa->GetNonConfigurationString(
            5 /*com.android.internal.R.styleable.AndroidManifestProvider_writePermission*/,
            0, &str);
    if (str.IsNull()) {
        str = permission;
    }
    if (str.IsNull()) {
        String perm;
        owner->mApplicationInfo->GetPermission(&perm);
        p->mInfo->SetWritePermission(perm);
    }
    else {
        p->mInfo->SetWritePermission(str.GetLength() > 0 ? str : String(NULL));
    }

    sa->GetBoolean(
            13 /*com.android.internal.R.styleable.AndroidManifestProvider_grantUriPermissions*/,
            FALSE, &bvalue);
    p->mInfo->SetGrantUriPermissions(bvalue);

    sa->GetBoolean(
            9 /*com.android.internal.R.styleable.AndroidManifestProvider_multiprocess*/,
            FALSE, &bvalue);
    p->mInfo->SetMultiprocess(bvalue);

    Int32 initOrder;
    sa->GetInt32(
            12 /*com.android.internal.R.styleable.AndroidManifestProvider_initOrder*/,
            0, &initOrder);
    p->mInfo->SetInitOrder(initOrder);

    sa->Recycle();

    Int32 ownerFlags;
    owner->mApplicationInfo->GetFlags(&ownerFlags);
    if ((ownerFlags & ApplicationInfo_FLAG_CANT_SAVE_STATE) != 0) {
        // A heavy-weight application can not have providers in its main process
        // We can do direct compare because we intern all strings.
        String pname;
        p->mInfo->GetProcessName(&pname);
        if (pname.Equals(owner->mCapsuleName)) {
            (*outError)[0] = "Heavy-weight applications can not have providers in main process";
            return E_RUNTIME_EXCEPTION;
        }
    }

    if (cpname.IsNull()) {
        (*outError)[0] = "<provider> does not incude authorities attribute";
        return E_RUNTIME_EXCEPTION;
    }
    p->mInfo->SetAuthority(cpname);

    if (!ParseContentProviderTags(res, parser, attrs, p, outError)) {
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    *provider = p;
    return NOERROR;
}

static
Int32 R_Styleable_AndroidManifestGrantUriPermission[] = {
    0x0101002a, 0x0101002b, 0x0101002c
};

static
Int32 R_Styleable_AndroidManifestPathPermission[] = {
    0x01010006, 0x01010007, 0x01010008, 0x0101002a,
    0x0101002b, 0x0101002c
};

Boolean CapsuleParser::ParseContentProviderTags(
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ ContentProvider* outInfo,
    /* [in] */ ArrayOf<String>* outError)
{
    Int32 outerDepth;
    parser->GetDepth(&outerDepth);
    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser_END_DOCUMENT
           && (type != IXmlPullParser_END_TAG
                   || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser_END_TAG || type == IXmlPullParser_TEXT) {
            continue;
        }

        String name;
        parser->GetName(&name);
        if (name.Equals("meta-data")) {
            if ((outInfo->mMetaData = ParseMetaData(res, parser, attrs,
                    outInfo->mMetaData, outError)) == NULL) {
                return FALSE;
            }

        }
        else if (name.Equals("grant-uri-permission")) {
            AutoPtr<ITypedArray> sa;
            res->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_AndroidManifestGrantUriPermission,
                    sizeof(R_Styleable_AndroidManifestGrantUriPermission) / sizeof(Int32))
                /*com.android.internal.R.styleable.AndroidManifestGrantUriPermission*/,
                (ITypedArray**)&sa);

            AutoPtr<IPatternMatcher> pa;

            String str;
            sa->GetNonConfigurationString(
                    0 /*com.android.internal.R.styleable.AndroidManifestGrantUriPermission_path*/,
                    0, &str);
            if (!str.IsNull()) {
                CPatternMatcher::New(str, PatternMatcher_PATTERN_LITERAL,
                    (IPatternMatcher**)&pa);
            }

            sa->GetNonConfigurationString(
                1 /*com.android.internal.R.styleable.AndroidManifestGrantUriPermission_pathPrefix*/,
                0, &str);
            if (!str.IsNull()) {
                CPatternMatcher::New(str, PatternMatcher_PATTERN_PREFIX,
                    (IPatternMatcher**)&pa);
            }

            sa->GetNonConfigurationString(
                2 /*com.android.internal.R.styleable.AndroidManifestGrantUriPermission_pathPattern*/,
                0, &str);
            if (!str.IsNull()) {
                CPatternMatcher::New(str, PatternMatcher_PATTERN_SIMPLE_GLOB,
                    (IPatternMatcher**)&pa);
            }

            sa->Recycle();

            if (pa != NULL) {
                AutoPtr<IObjectContainer> uriPermissionPatterns;
                outInfo->mInfo->GetUriPermissionPatterns((IObjectContainer**)&uriPermissionPatterns);
                if (uriPermissionPatterns == NULL) {
                    outInfo->mInfo->InitUriPermissionPatterns();
                }
                outInfo->mInfo->AddUriPermissionPatterns(pa);
                outInfo->mInfo->SetGrantUriPermissions(TRUE);
            }
            else {
                if (!RIGID_PARSER) {
                    String des;
                    parser->GetPositionDescription(&des);
                    Logger::W(TAG, StringBuffer("Unknown element under <path-permission>: ")
                            + name + " at " + mArchiveSourcePath + " " + des);
                    XmlUtils::SkipCurrentTag(parser);
                    continue;
                }
                (*outError)[0] = "No path, pathPrefix, or pathPattern for <path-permission>";
                return FALSE;
            }
            XmlUtils::SkipCurrentTag(parser);

        }
        else if (name.Equals("path-permission")) {
            AutoPtr<ITypedArray> sa;
            res->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_AndroidManifestPathPermission,
                    sizeof(R_Styleable_AndroidManifestPathPermission) / sizeof(Int32))
                /*com.android.internal.R.styleable.AndroidManifestPathPermission*/,
                (ITypedArray**)&sa);

            AutoPtr<IPathPermission> pa;

            String permission;
            sa->GetNonConfigurationString(
                0 /*com.android.internal.R.styleable.AndroidManifestPathPermission_permission*/,
                0, &permission);
            String readPermission;
            sa->GetNonConfigurationString(
                1 /*com.android.internal.R.styleable.AndroidManifestPathPermission_readPermission*/,
                0, &readPermission);
            if (readPermission.IsNull()) {
                readPermission = permission;
            }
            String writePermission;
            sa->GetNonConfigurationString(
                2 /*com.android.internal.R.styleable.AndroidManifestPathPermission_writePermission*/,
                0, &writePermission);
            if (writePermission.IsNull()) {
                writePermission = permission;
            }

            Boolean havePerm = FALSE;
            if (!readPermission.IsNull()) {
                havePerm = TRUE;
            }
            if (!writePermission.IsNull()) {
                havePerm = TRUE;
            }

            if (!havePerm) {
                if (!RIGID_PARSER) {
                    String des;
                    parser->GetPositionDescription(&des);
                    Logger::W(TAG, StringBuffer("No readPermission or writePermssion for <path-permission>: ")
                            + name + " at " + mArchiveSourcePath + " " + des);
                    XmlUtils::SkipCurrentTag(parser);
                    continue;
                }
                (*outError)[0] = "No readPermission or writePermssion for <path-permission>";
                return FALSE;
            }

            String path;
            sa->GetNonConfigurationString(
                3 /*com.android.internal.R.styleable.AndroidManifestPathPermission_path*/,
                0, &path);
            if (!path.IsNull()) {
                CPathPermission::New(path,
                    PatternMatcher_PATTERN_LITERAL, readPermission, writePermission,
                    (IPathPermission**)&pa);
            }

            sa->GetNonConfigurationString(
                4 /*com.android.internal.R.styleable.AndroidManifestPathPermission_pathPrefix*/,
                0, &path);
            if (!path.IsNull()) {
                CPathPermission::New(path,
                    PatternMatcher_PATTERN_PREFIX, readPermission, writePermission,
                    (IPathPermission**)&pa);
            }

            sa->GetNonConfigurationString(
                5 /*com.android.internal.R.styleable.AndroidManifestPathPermission_pathPattern*/,
                0, &path);
            if (!path.IsNull()) {
                CPathPermission::New(path,
                    PatternMatcher_PATTERN_SIMPLE_GLOB, readPermission, writePermission,
                    (IPathPermission**)&pa);
            }

            sa->Recycle();

            if (pa != NULL) {
                AutoPtr<IObjectContainer> pathPermissions;
                outInfo->mInfo->GetPathPermissions((IObjectContainer**)&pathPermissions);
                if (pathPermissions == NULL) {
                    outInfo->mInfo->InitPathPermissions();
                }
                outInfo->mInfo->AddPathPermission(pa);
            }
            else {
                if (!RIGID_PARSER) {
                    String des;
                    parser->GetPositionDescription(&des);
                    Logger::W(TAG, StringBuffer("No path, pathPrefix, or pathPattern for <path-permission>: ")
                            + name + " at " + mArchiveSourcePath + " " + des);
                    XmlUtils::SkipCurrentTag(parser);
                    continue;
                }
                (*outError)[0] = "No path, pathPrefix, or pathPattern for <path-permission>";
                return FALSE;
            }
            XmlUtils::SkipCurrentTag(parser);

        }
        else {
            if (!RIGID_PARSER) {
                String des;
                parser->GetPositionDescription(&des);
                Logger::W(TAG, StringBuffer("Unknown element under <provider>: ")
                        + name + " at " + mArchiveSourcePath + " " + des);
                XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            (*outError)[0] = (const char*)(
                StringBuffer("Bad element under <provider>: ") + name);
            return FALSE;
        }
    }
    return TRUE;
}

static
Int32 R_Styleable_AndroidManifestService[] = {
    0x01010001, 0x01010002, 0x01010003, 0x01010006,
    0x0101000e, 0x01010010, 0x01010011, 0x01010020
};

ECode CapsuleParser::ParseService(
    /* [in] */ Capsule* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* outError,
    /* [out] */ Service** service)
{
    assert(service);
    *service = NULL;

    AutoPtr<ITypedArray> sa;
    res->ObtainAttributes(attrs,
        ArrayOf<Int32>(R_Styleable_AndroidManifestService,
            sizeof(R_Styleable_AndroidManifestService) / sizeof(Int32))
        /*com.android.internal.R.styleable.AndroidManifestService*/,
        (ITypedArray**)&sa);

    if (mParseServiceArgs == NULL) {
        mParseServiceArgs = new ParseComponentArgs(owner, outError,
            2 /*com.android.internal.R.styleable.AndroidManifestService_name*/,
            0 /*com.android.internal.R.styleable.AndroidManifestService_label*/,
            1 /*com.android.internal.R.styleable.AndroidManifestService_icon*/,
            0,
            mSeparateProcesses.Get(),
            6 /*com.android.internal.R.styleable.AndroidManifestService_process*/,
            7 /*com.android.internal.R.styleable.AndroidManifestService_description*/,
            4 /*com.android.internal.R.styleable.AndroidManifestService_enabled*/);
        mParseServiceArgs->mTag = "<service>";
    }

    mParseServiceArgs->mSa = sa;
    mParseServiceArgs->mFlags = flags;

    AutoPtr<IServiceInfo> si;
    CServiceInfo::New((IServiceInfo**)&si);
    Service* s = new Service(mParseServiceArgs, si);
    if (!(*outError)[0].IsNull()) {
        sa->Recycle();
        return E_RUNTIME_EXCEPTION;
    }

    Boolean setExported = FALSE;
    sa->HasValue(
        5 /*com.android.internal.R.styleable.AndroidManifestService_exported*/,
        &setExported);
    if (setExported) {
        Boolean isExported;
        sa->GetBoolean(
            5 /*com.android.internal.R.styleable.AndroidManifestService_exported*/,
            FALSE, &isExported);
        s->mInfo->SetExported(isExported);
    }

    String str;
    sa->GetNonConfigurationString(
        3 /*com.android.internal.R.styleable.AndroidManifestService_permission*/,
        0, &str);
    if (str.IsNull()) {
        String ownerPerm;
        owner->mApplicationInfo->GetPermission(&ownerPerm);
        s->mInfo->SetPermission(ownerPerm);
    }
    else {
        s->mInfo->SetPermission(str.GetLength() > 0 ? str : String(NULL));
    }

    sa->Recycle();

    Int32 ownerFlags;
    owner->mApplicationInfo->GetFlags(&ownerFlags);
    if ((ownerFlags & ApplicationInfo_FLAG_CANT_SAVE_STATE) != 0) {
        // A heavy-weight application can not have services in its main process
        // We can do direct compare because we intern all strings
        String pname;
        s->mInfo->GetProcessName(&pname);
        if (pname.Equals(owner->mCapsuleName)) {
            (*outError)[0] = "Heavy-weight applications can not have services in main process";
            return E_RUNTIME_EXCEPTION;
        }
    }

    Int32 outerDepth;
    parser->GetDepth(&outerDepth);
    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser_END_DOCUMENT
           && (type != IXmlPullParser_END_TAG
                   || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser_END_TAG || type == IXmlPullParser_TEXT) {
            continue;
        }

        String name;
        parser->GetName(&name);
        if (name.Equals("intent-filter")) {
            ServiceIntentInfo* intent = new ServiceIntentInfo(s);
            if (!ParseIntent(res, parser, attrs, flags, intent, outError, FALSE)) {
                return E_XML_PULL_PARSER_EXCEPTION;
            }

            s->mIntents->PushBack(intent);
        }
        else if (name.Equals("meta-data")) {
            if ((s->mMetaData = ParseMetaData(res, parser, attrs, s->mMetaData,
                    outError)) == NULL) {
                return E_XML_PULL_PARSER_EXCEPTION;
            }
        }
        else {
            if (!RIGID_PARSER) {
                String des;
                parser->GetPositionDescription(&des);
                Logger::W(TAG, StringBuffer("Unknown element under <service>: ")
                        + name + " at " + mArchiveSourcePath + " " + des);
                XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            (*outError)[0] = (const char*)(
                StringBuffer("Bad element under <service>: ") + name);
            return E_RUNTIME_EXCEPTION;
        }
    }

    if (!setExported) {
        s->mInfo->SetExported(s->mIntents->Begin() != s->mIntents->End());
    }

    *service = s;
    return NOERROR;
}

template <typename T>
Boolean CapsuleParser::ParseAllMetaData(
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ const char* tag,
    /* [in] */ Component<T>* outInfo,
    /* [in] */ ArrayOf<String>* outError)
{
    Int32 outerDepth = 0;
    parser->GetDepth(&outerDepth);
    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser_END_DOCUMENT
        && (type != IXmlPullParser_END_TAG
        || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser_END_TAG || type == IXmlPullParser_TEXT) {
            continue;
        }

        String name;
        parser->GetName(&name);
        if (name.Equals("meta-data")) {
            if ((outInfo->mMetaData = ParseMetaData(res, parser, attrs,
                outInfo->mMetaData, outError)) == NULL) {
                return FALSE;
            }
        } else {
            if (!RIGID_PARSER) {
                String des;
                parser->GetPositionDescription(&des);
                Logger::W(TAG, StringBuffer("Unknown element under ") + tag + ": "
                    + name + " at " + mArchiveSourcePath + " "+ des);
                XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            (*outError)[0] = (const char*)(
                StringBuffer("Bad element under ") + tag + ": " + name);
            return FALSE;
        }
    }
    return TRUE;
}

static Int32 R_Styleable_AndroidManifestMetaData[] = {
    0x01010003, 0x01010024, 0x01010025
};

AutoPtr<IBundle> CapsuleParser::ParseMetaData(
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IBundle* data,
    /* [in] */ ArrayOf<String>* outError)
{
    AutoPtr<ITypedArray> sa;
    FAIL_RETURN_NULL(res->ObtainAttributes(attrs,
        ArrayOf<Int32>(R_Styleable_AndroidManifestMetaData,
            sizeof(R_Styleable_AndroidManifestMetaData) / sizeof(Int32))
        /*com.android.internal.R.styleable.AndroidManifestMetaData*/,
        (ITypedArray**)&sa));

    if (!data) {
        FAIL_RETURN_NULL(CBundle::New(&data));
        assert(data != NULL);
    }

    String name;
    sa->GetNonConfigurationString(
        0 /*com.android.internal.R.styleable.AndroidManifestMetaData_name*/, 0,
        &name);
    if (name.IsNull()) {
        (*outError)[0] = "<meta-data> requires an android:name attribute";
        sa->Recycle();
        return NULL;
    }

    AutoPtr<ITypedValue> v;
    sa->PeekValue(
        2 /*com.android.internal.R.styleable.AndroidManifestMetaData_resource*/,
        (ITypedValue**)&v);
    Int32 vResId;
    if (v != NULL && (v->GetResourceId(&vResId), vResId != 0)) {
        //Log::i(TAG, "Meta data ref " + name + ": " + v);
        data->PutInt32(name, vResId);
    }
    else {
        sa->PeekValue(
            1 /*com.android.internal.R.styleable.AndroidManifestMetaData_value*/,
            (ITypedValue**)&v);
        //Log::i(TAG, "Meta data " + name + ": " + v);
        if (v != NULL) {
            Int32 vType;
            v->GetType(&vType);
            if (vType == TypedValue_TYPE_STRING) {
                AutoPtr<ICharSequence> cs;
                v->CoerceToString((ICharSequence**)&cs);
                String csStr;
                cs->ToString(&csStr);
                data->PutString(name, csStr);
            }
            else if (vType == TypedValue_TYPE_INT_BOOLEAN) {
                Int32 vData;
                v->GetData(&vData);
                data->PutBoolean(name, vData != 0);
            }
            else if (vType >= TypedValue_TYPE_FIRST_INT
                && vType <= TypedValue_TYPE_LAST_INT) {
                Int32 vData;
                v->GetData(&vData);
                data->PutInt32(name, vData);
            }
            else if (vType == TypedValue_TYPE_FLOAT) {
                Float f = 0;
                v->GetFloat(&f);
                data->PutFloat(name, f);
            }
            else {
                if (!RIGID_PARSER) {
                    String name;
                    String des;
                    parser->GetName(&name);
                    parser->GetPositionDescription(&des);
                    Logger::W(TAG,
                        StringBuffer("<meta-data> only supports string, integer, float, color, boolean, and resource reference types: ")
                        + name + " at " + (String)mArchiveSourcePath + " " + des);
                } else {
                    (*outError)[0] =
                            "<meta-data> only supports string, integer, float, color, boolean, and resource reference types";
                    data = NULL;
                }
            }
        } else {
            (*outError)[0] = "<meta-data> requires an android:value or android:resource attribute";
            data = NULL;
        }
    }

    sa->Recycle();

    XmlUtils::SkipCurrentTag(parser);

    return data;
}

static Int32 R_Styleable_AndroidManifestIntentFilter[] = {
    0x01010001, 0x01010002, 0x0101001c
};

static Int32 R_Styleable_AndroidManifestData[] = {
    0x01010026, 0x01010027, 0x01010028, 0x01010029,
    0x0101002a, 0x0101002b, 0x0101002c
};

Boolean CapsuleParser::ParseIntent(
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [in] */ IntentInfo* outInfo,
    /* [in] */ ArrayOf<String>* outError,
    /* [in] */ Boolean isActivity)
{
    AutoPtr<ITypedArray> sa;
    FAIL_RETURN(res->ObtainAttributes(attrs,
        ArrayOf<Int32>(R_Styleable_AndroidManifestIntentFilter,
            sizeof(R_Styleable_AndroidManifestIntentFilter) / sizeof(Int32))
        /*com.android.internal.R.styleable.AndroidManifestIntentFilter*/,
        (ITypedArray**)&sa));

    Int32 priority = 0;
    sa->GetInt32(
        2 /*com.android.internal.R.styleable.AndroidManifestIntentFilter_priority*/, 0,
        &priority);
    outInfo->SetPriority(priority);

    AutoPtr<ITypedValue> v;
    sa->PeekValue(
        0 /*com.android.internal.R.styleable.AndroidManifestIntentFilter_label*/,
        (ITypedValue**)&v);
    if (v != NULL && (v->GetResourceId(&outInfo->mLabelRes), outInfo->mLabelRes) == 0) {
        v->CoerceToString((ICharSequence**)&outInfo->mNonLocalizedLabel);
    }

    sa->GetResourceId(
        1 /*com.android.internal.R.styleable.AndroidManifestIntentFilter_icon*/, 0,
        &outInfo->mIcon);

    sa->Recycle();

    ECode ec = NOERROR;
    Int32 outerDepth;
    parser->GetDepth(&outerDepth);

    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser_END_DOCUMENT
           && (type != IXmlPullParser_END_TAG
           || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser_END_TAG || type == IXmlPullParser_TEXT) {
            continue;
        }

        String nodeName;
        parser->GetName(&nodeName);
        if (nodeName.Equals("action")) {
            String value;
            ec = attrs->GetAttributeValueEx(ANDROID_RESOURCES, "name", &value);
            if (FAILED(ec) || value.IsNullOrEmpty()) {
                (*outError)[0] = "No value supplied for <android:name>";
                return FALSE;
            }
            XmlUtils::SkipCurrentTag(parser);

            outInfo->AddAction(value);

        } else if (nodeName.Equals("category")) {
            String value;
            ec = attrs->GetAttributeValueEx(ANDROID_RESOURCES, "name", &value);
            if (FAILED(ec) || value.IsNullOrEmpty()) {
                (*outError)[0] = "No value supplied for <android:name>";
                return FALSE;
            }
            XmlUtils::SkipCurrentTag(parser);

            outInfo->AddCategory(value);

        } else if (nodeName.Equals("data")) {
            res->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_AndroidManifestData,
                    sizeof(R_Styleable_AndroidManifestData) / sizeof(Int32))
                /*com.android.internal.R.styleable.AndroidManifestData*/, (ITypedArray**)&sa);

            String str;
            sa->GetNonConfigurationString(
                0 /*com.android.internal.R.styleable.AndroidManifestData_mimeType*/, 0, &str);
            if (!str.IsNull()) {
                if (FAILED(outInfo->AddDataType(str))) {
                    (*outError)[0] = "E_RUNTIME_EXCEPTION";
                    sa->Recycle();
                    return FALSE;
                }
            }

            sa->GetNonConfigurationString(
                1 /*com.android.internal.R.styleable.AndroidManifestData_scheme*/, 0, &str);
            if (!str.IsNull()) {
                outInfo->AddDataScheme(str);
            }

            String host;
            sa->GetNonConfigurationString(
                2 /*com.android.internal.R.styleable.AndroidManifestData_host*/, 0, &host);
            String port;
            sa->GetNonConfigurationString(
                3 /*com.android.internal.R.styleable.AndroidManifestData_port*/, 0, &port);
            if (!host.IsNull()) {
                outInfo->AddDataAuthority(host, port);
            }

            sa->GetNonConfigurationString(
                4 /*com.android.internal.R.styleable.AndroidManifestData_path*/, 0, &str);
            if (!str.IsNull()) {
                outInfo->AddDataPath(str, PatternMatcher_PATTERN_LITERAL);
            }

            sa->GetNonConfigurationString(
                5 /*com.android.internal.R.styleable.AndroidManifestData_pathPrefix*/, 0, &str);
            if (!str.IsNull()) {
                outInfo->AddDataPath(str, PatternMatcher_PATTERN_PREFIX);
            }

            sa->GetNonConfigurationString(
                6 /*com.android.internal.R.styleable.AndroidManifestData_pathPattern*/, 0, &str);
            if (!str.IsNull()) {
                outInfo->AddDataPath(str, PatternMatcher_PATTERN_SIMPLE_GLOB);
            }

            sa->Recycle();
            XmlUtils::SkipCurrentTag(parser);
        }
        else if (!RIGID_PARSER) {
            String name;
            String des;
            parser->GetName(&name);
            parser->GetPositionDescription(&des);
            Logger::W(TAG, StringBuffer("Unknown element under <intent-filter>: ")
                    + name + " at " + (String)mArchiveSourcePath + " " + des);
            XmlUtils::SkipCurrentTag(parser);
        }
        else {
            String name;
            parser->GetName(&name);
            (*outError)[0] = (const char*)(
                StringBuffer("Bad element under <intent-filter>: ") + name);
            return FALSE;
        }
    }

    outInfo->HasCategory(String(Intent_CATEGORY_DEFAULT),
            &outInfo->mHasDefault);

//	    if (false) {
//	        String cats = "";
//	        Iterator<String> it = outInfo.categoriesIterator();
//	        while (it != null && it.hasNext()) {
//	            cats += " " + it.next();
//	        }
//	        System.out.println("Intent d=" +
//	                outInfo.hasDefault + ", cat=" + cats);
//	    }

    return TRUE;
}

Boolean CapsuleParser::CopyNeeded(
    /* [in] */ Int32 flags,
    /* [in] */ CapsuleParser::Capsule* capsule,
    /* [in] */ IBundle* metaData)
{
    if ((flags & CapsuleManager_GET_META_DATA) != 0
        && (metaData != NULL || capsule->mAppMetaData != NULL)) {
        return TRUE;
    }
    if ((flags & CapsuleManager_GET_SHARED_LIBRARY_FILES) != 0
        && capsule->mUsesLibraryFiles != NULL
        && capsule->mUsesLibraryFiles->GetLength() > 0) {
        return TRUE;
    }
    return FALSE;
}

AutoPtr<IApplicationInfo> CapsuleParser::GenerateApplicationInfo(
    /* [in] */ Capsule* capsule,
    /* [in] */ Int32 flags)
{
    if (capsule == NULL) {
        return NULL;
    }
    if (!CopyNeeded(flags, capsule, NULL)) {
        // CompatibilityMode is global state. It's safe to modify the instance
        // of the package.
        if (!sCompatibilityModeEnabled) {
            capsule->mApplicationInfo->DisableCompatibilityMode();
        }
        return capsule->mApplicationInfo;
    }

    // Make shallow copy so we can store the metadata/libraries safely
    AutoPtr<IApplicationInfo> ai;
    ASSERT_SUCCEEDED(CApplicationInfo::New(
        capsule->mApplicationInfo, (IApplicationInfo**)&ai));
    if ((flags & CapsuleManager_GET_META_DATA) != 0) {
        ai->SetMetaData(capsule->mAppMetaData.Get());
    }
    if ((flags & CapsuleManager_GET_SHARED_LIBRARY_FILES) != 0
        && capsule->mUsesLibraryFiles != NULL) {
        ai->SetSharedLibraryFiles(capsule->mUsesLibraryFiles->Clone());
    }
    if (!sCompatibilityModeEnabled) {
        ai->DisableCompatibilityMode();
    }
    return ai;
}

AutoPtr<IActivityInfo> CapsuleParser::GenerateActivityInfo(
    /* [in] */ Activity* activity,
    /* [in] */ Int32 flags)
{
    if (activity == NULL) {
        return NULL;
    }
    if (!CopyNeeded(flags, activity->mOwner, activity->mMetaData)) {
        return activity->mInfo;
    }

    // Make shallow copies so we can store the metadata safely
    AutoPtr<IActivityInfo> info;
    ASSERT_SUCCEEDED(CActivityInfo::New(
            (IActivityInfo*)activity->mInfo.Get(), (IActivityInfo**)&info));
    info->SetMetaData(activity->mMetaData);
    info->SetApplicationInfo(GenerateApplicationInfo(activity->mOwner, flags));
    return info;
}

AutoPtr<IServiceInfo> CapsuleParser::GenerateServiceInfo(
    /* [in] */ Service* service,
    /* [in] */ Int32 flags)
{
    if (service == NULL) {
        return NULL;
    }
    if (!CopyNeeded(flags, service->mOwner, service->mMetaData)) {
        return service->mInfo;
    }

    // Make shallow copies so we can store the metadata safely
    AutoPtr<IServiceInfo> info;
    ASSERT_SUCCEEDED(CServiceInfo::New(
            (IServiceInfo*)service->mInfo.Get(), (IServiceInfo**)&info));
    info->SetMetaData(service->mMetaData);
    info->SetApplicationInfo(GenerateApplicationInfo(service->mOwner, flags));
    return info;
}

AutoPtr<IContentProviderInfo> CapsuleParser::GenerateContentProviderInfo(
    /* [in] */ ContentProvider* provider,
    /* [in] */ Int32 flags)
{
    if (provider == NULL) {
        return NULL;
    }
    AutoPtr<IObjectContainer> uriPermissions;
    provider->mInfo->GetUriPermissionPatterns((IObjectContainer**)&uriPermissions);
    if (!CopyNeeded(flags, provider->mOwner, provider->mMetaData)
        && ((flags & CapsuleManager_GET_URI_PERMISSION_PATTERNS) != 0
        || uriPermissions == NULL)) {
        return provider->mInfo;
    }

    // Make shallow copies so we can store the metadata safely
    AutoPtr<IContentProviderInfo> pi;
    ASSERT_SUCCEEDED(CContentProviderInfo::New(
        (IContentProviderInfo*)provider->mInfo.Get(), (IContentProviderInfo**)&pi));
    pi->SetMetaData(provider->mMetaData);
    if ((flags & CapsuleManager_GET_URI_PERMISSION_PATTERNS) == 0) {
        pi->ClearUriPermissionPatterns();
    }
    pi->SetApplicationInfo(GenerateApplicationInfo(provider->mOwner, flags));
    return pi;
}

AutoPtr<IInstrumentationInfo> CapsuleParser::GenerateInstrumentationInfo(
    /* [in] */ CapsuleParser::Instrumentation* i,
    /* [in] */ Int32 flags)
{
    if (i == NULL) {
        return NULL;
    }
    if ((flags & CapsuleManager_GET_META_DATA) == 0) {
        return i->mInfo;
    }
    AutoPtr<IInstrumentationInfo> info;
    ASSERT_SUCCEEDED(CInstrumentationInfo::New(
        (IInstrumentationInfo*)i->mInfo.Get(), (IInstrumentationInfo**)&info));
    info->SetMetaData(i->mMetaData);
    return info;
}

void CapsuleParser::SetCompatibilityModeEnabled(
    /* [in] */ Boolean compatibilityModeEnabled)
{
}

Boolean CapsuleParser::CollectCertificates(
    /* [in] */ Capsule* cap,
    /* [in] */ Int32 flags)
{
    return FALSE;
}


