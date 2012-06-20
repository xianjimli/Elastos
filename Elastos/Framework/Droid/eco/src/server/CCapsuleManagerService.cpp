
#include "ext/frameworkext.h"
#include "server/CCapsuleManagerService.h"
#include "server/Config.h"
#include "capsule/CActivityInfo.h"
#include "capsule/CCapsuleInfo.h"
#include "utils/CObjectContainer.h"
#include "utils/CDisplayMetrics.h"
#include "utils/CApartment.h"
#include "utils/ListUtils.h"
#include "content/CapsuleManager.h"
#include "content/CPermissionInfo.h"
#include "content/CapsuleHelper.h"
#include "content/CIntent.h"
#include "content/CParceledListSlice.h"
#include "content/NativeLibraryHelper.h"
#include "content/CCapsuleStats.h"
#include "content/CFeatureInfo.h"
#include "os/Environment.h"
#include "os/Process.h"
#include "os/SystemClock.h"
#include "os/SystemProperties.h"
#include <dirent.h>
#include "sys/stat.h"
#include <elastos/AutoFree.h>
#include <unistd.h>
#include <Slogger.h>
#include <Logger.h>
#include <StringBuffer.h>

using namespace Elastos;
using namespace Elastos::System;
using namespace Elastos::Utility::Logging;

#define UNUSED(x) (void)x

////////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::DefaultContainerConnection

PInterface CCapsuleManagerService::DefaultContainerConnection::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IServiceConnection) {
        return (IServiceConnection*)this;
    }
    return NULL;
}

UInt32 CCapsuleManagerService::DefaultContainerConnection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCapsuleManagerService::DefaultContainerConnection::Release()
{
    return ElRefBase::Release();
}

ECode CCapsuleManagerService::DefaultContainerConnection::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IServiceConnection*)this) {
        *pIID = EIID_IServiceConnection;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CCapsuleManagerService::DefaultContainerConnection::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    if (CCapsuleManagerService::DEBUG_SD_INSTALL) Logger::I(TAG, "onServiceConnected");
    AutoPtr<IMediaContainerService> imcs = (IMediaContainerService*)service->Probe(EIID_IMediaContainerService);
    if (imcs == NULL) return NOERROR;
    Message* msg = new Message();
    msg->mWhat = CCapsuleManagerService::MCS_BOUND;
    msg->mObj = imcs.Get();
    mOwner->mHandler->SendMessage(msg);
    return NOERROR;
}

ECode CCapsuleManagerService::DefaultContainerConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    if (DEBUG_SD_INSTALL) Logger::I(TAG, "onServiceDisconnected");
    return NOERROR;
}

////////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::ActivityIntentResolver

CCapsuleManagerService::ActivityIntentResolver::ActivityIntentResolver() :
    mActivities(23)
{
}

CCapsuleManagerService::ActivityIntentResolver::~ActivityIntentResolver()
{
    mActivities.Clear();
}

List<AutoPtr<CResolveInfo>*>*
CCapsuleManagerService::ActivityIntentResolver::QueryIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return Super::QueryIntent(intent, resolvedType,
        (flags & CapsuleManager_MATCH_DEFAULT_ONLY) != 0);
}

List<AutoPtr<CResolveInfo>*>*
CCapsuleManagerService::ActivityIntentResolver::QueryIntentForCapsule(
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ List<CapsuleParser::Activity*>* capsuleActivities)
{
    if (capsuleActivities == NULL) {
        return NULL;
    }

    mFlags = flags;
    Boolean defaultOnly = (flags & CapsuleManager_MATCH_DEFAULT_ONLY) != 0;
    List<List<CapsuleParser::ActivityIntentInfo*>* >* listCut =
            new List<List<CapsuleParser::ActivityIntentInfo*>* >();

    List<CapsuleParser::ActivityIntentInfo*>* intentFilters;
    List<CapsuleParser::Activity*>::Iterator it;
    for (it = capsuleActivities->Begin(); it != capsuleActivities->End(); ++it) {
        intentFilters = &((*it)->mIntents);
        if (intentFilters != NULL && intentFilters->GetSize() > 0) {
            listCut->PushBack(intentFilters);
        }
    }
    return Super::QueryIntentFromList(intent, resolvedType, defaultOnly, listCut);
}

void CCapsuleManagerService::ActivityIntentResolver::AddActivity(
    /* [in] */ CapsuleParser::Activity* activity,
    /* [in] */ String type)
{
    assert(activity);
    assert(!type.IsNull());

    Boolean systemApp = CCapsuleManagerService::IsSystemApp(
            activity->mInfo->mApplicationInfo);
    AutoPtr<IComponentName> componentName;
    activity->GetComponentName((IComponentName**)&componentName);
    mActivities[componentName] = activity;
//    if (SHOW_INFO || Config.LOGV) Slog.v(
//        TAG, "  " + type + " " +
//        (a.info.nonLocalizedLabel != null ? a.info.nonLocalizedLabel : a.info.name) + ":");
//    if (SHOW_INFO || Config.LOGV) Slog.v(TAG, "    Class=" + a.info.name);
    List<CapsuleParser::ActivityIntentInfo*>::Iterator it;
    for (it = activity->mIntents.Begin(); it != activity->mIntents.End(); ++it) {
        Int32 pri;
        if (!systemApp && ((*it)->GetPriority(&pri), pri > 0) &&
                String("activity").Equals(type)) {
            (*it)->SetPriority(0);
//            Log.w(TAG, "Package " + a.info.applicationInfo.packageName + " has activity "
//                    + a.className + " with priority > 0, forcing to 0");
        }
//        if (SHOW_INFO || Config.LOGV) {
//            Slog.v(TAG, "    IntentFilter:");
//            intent.dump(new LogPrinter(Slog.VERBOSE, TAG), "      ");
//        }
//        if (!intent.debugCheck()) {
//            Slog.w(TAG, "==> For Activity " + a.info.name);
//        }
        AddFilter(*it);
    }
}

void CCapsuleManagerService::ActivityIntentResolver::RemoveActivity(
    /* [in] */ CapsuleParser::Activity* activity,
    /* [in] */ String type)
{
    AutoPtr<IComponentName> componentName;
    activity->GetComponentName((IComponentName**)&componentName);
    mActivities.Erase(componentName);
//    if (SHOW_INFO || Config.LOGV) Slog.v(
//        TAG, "  " + type + " " +
//        (a.info.nonLocalizedLabel != null ? a.info.nonLocalizedLabel : a.info.name) + ":");
//    if (SHOW_INFO || Config.LOGV) Slog.v(TAG, "    Class=" + a.info.name);
    List<CapsuleParser::ActivityIntentInfo*>::Iterator it;
    for (it = activity->mIntents.Begin(); it != activity->mIntents.End(); ++it) {
//        if (SHOW_INFO || Config.LOGV) {
//            Slog.v(TAG, "    IntentFilter:");
//            intent.dump(new LogPrinter(Slog.VERBOSE, TAG), "      ");
//        }
        RemoveFilter(*it);
    }
}

ECode CCapsuleManagerService::ActivityIntentResolver::NewResult(
    /* [in] */ CapsuleParser::ActivityIntentInfo* info,
    /* [in] */ Int32 match,
    /* [out] */ AutoPtr<CResolveInfo>** res)
{
    assert(res != NULL);
//    if (!mSettings.isEnabledLP(info.activity.info, mFlags)) {
//        return null;
//    }
    CapsuleParser::Activity* activity = info->mActivity;
//    if (mSafeMode && (activity.info.applicationInfo.flags
//            &ApplicationInfo.FLAG_SYSTEM) == 0) {
//        return null;
//    }
    AutoPtr<CResolveInfo> rinfo;
    CResolveInfo::NewByFriend((CResolveInfo**)&rinfo);
    CapsuleParser::GenerateActivityInfo(activity, mFlags,
            (CActivityInfo**)&(rinfo->mActivityInfo));
//    if ((mFlags&PackageManager.GET_RESOLVED_FILTER) != 0) {
//        res.filter = info;
//    }
//    res.priority = info.getPriority();
//    res.preferredOrder = activity.owner.mPreferredOrder;
//    //System.out.println("Result: " + res.activityInfo.className +
//    //                   " = " + res.priority);
//    res.match = match;
//    res.isDefault = info.hasDefault;
//    res.labelRes = info.labelRes;
//    res.nonLocalizedLabel = info.nonLocalizedLabel;
//    res.icon = info.icon;
    *res = new AutoPtr<CResolveInfo>();
    **res = rinfo;
    return NOERROR;
}

void CCapsuleManagerService::ActivityIntentResolver::SortResults(
    /* [in] */ List<AutoPtr<CResolveInfo>*>* results)
{
}

CCapsuleManagerService::ServiceIntentResolver::ServiceIntentResolver() :
    mServices(11)
{
}

CCapsuleManagerService::ServiceIntentResolver::~ServiceIntentResolver()
{
    mServices.Clear();
}

List<AutoPtr<CResolveInfo>*>*
CCapsuleManagerService::ServiceIntentResolver::QueryIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return Super::QueryIntent(intent, resolvedType,
        (flags & CapsuleManager_MATCH_DEFAULT_ONLY) != 0);
}

List<AutoPtr<CResolveInfo>*>*
CCapsuleManagerService::ServiceIntentResolver::QueryIntentForCapsule(
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ List<CapsuleParser::Service*>* capsuleServices)
{
    if (capsuleServices == NULL) {
        return NULL;
    }
    mFlags = flags;
    Boolean defaultOnly = (flags & CapsuleManager_MATCH_DEFAULT_ONLY) != 0;
    List<List<CapsuleParser::ServiceIntentInfo*>* >* listCut =
            new List<List<CapsuleParser::ServiceIntentInfo*>* >();

    List<CapsuleParser::ServiceIntentInfo*>* intentFilters;
    List<CapsuleParser::Service*>::Iterator it;
    for (it = capsuleServices->Begin(); it != capsuleServices->End(); ++it) {
        intentFilters = &((*it)->mIntents);
        if (intentFilters != NULL && intentFilters->GetSize() > 0) {
            listCut->PushBack(intentFilters);
        }
    }
    return Super::QueryIntentFromList(intent, resolvedType, defaultOnly, listCut);
}

void CCapsuleManagerService::ServiceIntentResolver::AddService(
    /* [in] */ CapsuleParser::Service* service)
{
    AutoPtr<IComponentName> componentName;
    service->GetComponentName((IComponentName**)&componentName);
    mServices[componentName] = service;
//    if (SHOW_INFO || Config.LOGV) Slog.v(
//        TAG, "  " + (s.info.nonLocalizedLabel != null
//                ? s.info.nonLocalizedLabel : s.info.name) + ":");
//    if (SHOW_INFO || Config.LOGV) Slog.v(
//            TAG, "    Class=" + s.info.name);
    List<CapsuleParser::ServiceIntentInfo*>::Iterator it;
    for (it = service->mIntents.Begin(); it != service->mIntents.End(); ++it) {
//        if (SHOW_INFO || Config.LOGV) {
//            Slog.v(TAG, "    IntentFilter:");
//            intent.dump(new LogPrinter(Slog.VERBOSE, TAG), "      ");
//        }
//        if (!intent.debugCheck()) {
//            Slog.w(TAG, "==> For Service " + s.info.name);
//        }
        AddFilter(*it);
    }
}

void CCapsuleManagerService::ServiceIntentResolver::RemoveService(
    /* [in] */ CapsuleParser::Service* service)
{
    AutoPtr<IComponentName> componentName;
    service->GetComponentName((IComponentName**)&componentName);
    mServices.Erase(componentName);
//    if (SHOW_INFO || Config.LOGV) Slog.v(
//        TAG, "  " + (s.info.nonLocalizedLabel != null
//                ? s.info.nonLocalizedLabel : s.info.name) + ":");
//    if (SHOW_INFO || Config.LOGV) Slog.v(
//            TAG, "    Class=" + s.info.name);
    List<CapsuleParser::ServiceIntentInfo*>::Iterator it;
    for (it = service->mIntents.Begin(); it != service->mIntents.End(); ++it) {
//        if (SHOW_INFO || Config.LOGV) {
//            Slog.v(TAG, "    IntentFilter:");
//            intent.dump(new LogPrinter(Slog.VERBOSE, TAG), "      ");
//        }
        RemoveFilter(*it);
    }
}

ECode CCapsuleManagerService::ServiceIntentResolver::NewResult(
    /* [in] */ CapsuleParser::ServiceIntentInfo* info,
    /* [in] */ Int32 match,
    /* [out] */ AutoPtr<CResolveInfo>** res)
{
    assert(res != NULL);
//    if (!mSettings.isEnabledLP(info.service.info, mFlags)) {
//        return null;
//    }
    CapsuleParser::Service* service = info->mService;
//    if (mSafeMode && (service.info.applicationInfo.flags
//            &ApplicationInfo.FLAG_SYSTEM) == 0) {
//        return null;
//    }
    AutoPtr<CResolveInfo> rinfo;
    CResolveInfo::NewByFriend((CResolveInfo**)&rinfo);
    CapsuleParser::GenerateServiceInfo(service, mFlags,
            (CServiceInfo**)&(rinfo->mServiceInfo));
//    if ((mFlags&PackageManager.GET_RESOLVED_FILTER) != 0) {
//        res.filter = filter;
//    }
//    res.priority = info.getPriority();
//    res.preferredOrder = service.owner.mPreferredOrder;
//    //System.out.println("Result: " + res.activityInfo.className +
//    //                   " = " + res.priority);
//    res.match = match;
//    res.isDefault = info.hasDefault;
//    res.labelRes = info.labelRes;
//    res.nonLocalizedLabel = info.nonLocalizedLabel;
//    res.icon = info.icon;
    *res = new AutoPtr<CResolveInfo>();
    **res = rinfo;
    return NOERROR;
}

void CCapsuleManagerService::ServiceIntentResolver::SortResults(
    /* [in] */ List<AutoPtr<CResolveInfo>*>* results)
{

}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::BasePermission

const Int32 CCapsuleManagerService::BasePermission::TYPE_NORMAL;
const Int32 CCapsuleManagerService::BasePermission::TYPE_BUILTIN;
const Int32 CCapsuleManagerService::BasePermission::TYPE_DYNAMIC;

CCapsuleManagerService::BasePermission::BasePermission(
    /* [in] */ String _name,
    /* [in] */ String _sourcePackage,
    /* [in] */ int _type)
    : mCapsuleSetting(NULL)
    , mType(0)
    , mProtectionLevel(0)
    , mPerm(NULL)
    , mUid(0)
{
    // E_NOT_IMPLEMENTED
}

CCapsuleManagerService::BasePermission::~BasePermission()
{
}

String CCapsuleManagerService::BasePermission::GetDescription()
{
    return NULL; // E_NOT_IMPLEMENTED
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::CapsuleSignatures

CCapsuleManagerService::CapsuleSignatures::CapsuleSignatures(
    /* [in] */ const CapsuleSignatures& orig)
{
    // E_NOT_IMPLEMENTED
}

CCapsuleManagerService::CapsuleSignatures::CapsuleSignatures(
    /* [in] */ const ArrayOf<Signature>& sigs)
{
    // E_NOT_IMPLEMENTED
}

CCapsuleManagerService::CapsuleSignatures::CapsuleSignatures()
{
    // E_NOT_IMPLEMENTED
}

CCapsuleManagerService::CapsuleSignatures::~CapsuleSignatures()
{
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::PreferredActivity

PInterface CCapsuleManagerService::PreferredActivity::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IIntentFilter*)this;
    }
    else if (riid == EIID_IIntentFilter) {
        return (IIntentFilter*)this;
    }
    return NULL;
}

UInt32 CCapsuleManagerService::PreferredActivity::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCapsuleManagerService::PreferredActivity::Release()
{
    return ElRefBase::Release();
}

ECode CCapsuleManagerService::PreferredActivity::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IIntentFilter*)this) {
        *pIID = EIID_IIntentFilter;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::SetPriority(
    /* [in] */ Int32 priority)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::PreferredActivity::GetPriority(
    /* [out] */ Int32* priority)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::PreferredActivity::AddAction(
    /* [in] */ String action)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::PreferredActivity::CountActions(
    /* [out] */ Int32 *count)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::PreferredActivity::GetAction(
    /* [in] */ Int32 index,
    /* [out] */ String *action)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::PreferredActivity::AddDataType(
    /* [in] */ String type)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::PreferredActivity::AddDataScheme(
	/* [in] */ String scheme)
{
	return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::PreferredActivity::AddDataAuthority(
	/* [in] */ String host,
	/* [in] */ String port)
{
	return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::PreferredActivity::AddDataPath(
	/* [in] */ String path,
	/* [in] */ Int32 type)
{
	return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::PreferredActivity::AddCategory(
    /* [in] */ String category)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::PreferredActivity::HasCategory(
    /* [in] */ String category,
    /* [out] */ Boolean* hasCategory)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::PreferredActivity::Match(
    /* [in] */ String action,
    /* [in] */ String type,
    /* [in] */ String scheme,
    /* [in] */ IUri* data,
    /* [in] */ ArrayOf<String>* categories,
    /* [in] */ String logTag,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::PreferredActivity::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::PreferredActivity::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return E_NOT_IMPLEMENTED;
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::GrantedPermissions

CCapsuleManagerService::GrantedPermissions::GrantedPermissions(
    /* [in] */ Int32 capFlags)
    : mGrantedPermissions(11)
{
    SetFlags(capFlags);
}

CCapsuleManagerService::GrantedPermissions::GrantedPermissions(
    /* [in] */ const GrantedPermissions& base)
    : mGrantedPermissions(11)
{
    mCapFlags = base.mCapFlags;
//	    mGrantedPermissions = (HashSet<String>) base.grantedPermissions.clone();

    if (base.mGids != NULL) {
        mGids = base.mGids->Clone();
    }
}

CCapsuleManagerService::GrantedPermissions::~GrantedPermissions()
{
}

void CCapsuleManagerService::GrantedPermissions::SetFlags(
    /* [in] */ Int32 capFlags)
{
    mCapFlags = capFlags & (
        CApplicationInfo::FLAG_SYSTEM |
        CApplicationInfo::FLAG_FORWARD_LOCK |
        CApplicationInfo::FLAG_EXTERNAL_STORAGE |
        CApplicationInfo::FLAG_NEVER_ENCRYPT);
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::CapsuleSettingBase

CCapsuleManagerService::CapsuleSettingBase::CapsuleSettingBase(
    /* [in] */ String name,
    /* [in] */ String realName,
    /* [in] */ IFile* codePath,
    /* [in] */ IFile* resourcePath,
    /* [in] */ String nativeLibraryPathString,
    /* [in] */ Int32 pVersionCode,
    /* [in] */ Int32 capFlags)
    : GrantedPermissions(capFlags)
    , mTimeStamp(0)
    , mFirstInstallTime(0)
    , mLastUpdateTime(0)
    , mVersionCode(0)
    , mUidError(FALSE)
    , mPermissionsFixed(FALSE)
    , mHaveGids(FALSE)
    , mDisabledComponents(0)
    , mEnabledComponents(0)
    , mEnabled(CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT)
    , mInstallStatus(CCapsuleManagerService::CAP_INSTALL_COMPLETE)
    , mOrigCapsule(NULL)
{
    // E_NOT_IMPLEMENTED
}

CCapsuleManagerService::CapsuleSettingBase::CapsuleSettingBase(
    /* [in] */ const CapsuleSettingBase& base)
    : GrantedPermissions(base)
    , mTimeStamp(0)
    , mFirstInstallTime(0)
    , mLastUpdateTime(0)
    , mVersionCode(0)
    , mUidError(FALSE)
    , mPermissionsFixed(FALSE)
    , mHaveGids(FALSE)
    , mDisabledComponents(0)
    , mEnabledComponents(0)
    , mEnabled(CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT)
    , mInstallStatus(CCapsuleManagerService::CAP_INSTALL_COMPLETE)
    , mOrigCapsule(NULL)
{
    // E_NOT_IMPLEMENTED
}

CCapsuleManagerService::CapsuleSettingBase::~CapsuleSettingBase()
{
}

void CCapsuleManagerService::CapsuleSettingBase::Init(
    /* [in] */ IFile* codePath,
    /* [in] */ IFile* resourcePath,
    /* [in] */ String nativeLibraryPathString,
    /* [in] */ Int32 pVersionCode)
{
    // E_NOT_IMPLEMENTED
}

void CCapsuleManagerService::CapsuleSettingBase::SetInstallerCapsuleName(
    /* [in] */ String packageName)
{
    // E_NOT_IMPLEMENTED
}

String CCapsuleManagerService::CapsuleSettingBase::GetInstallerPackageName()
{
    return NULL; // E_NOT_IMPLEMENTED
}

void CCapsuleManagerService::CapsuleSettingBase::SetInstallStatus(
    /* [in] */ Int32 newStatus)
{
    // E_NOT_IMPLEMENTED
}

String CCapsuleManagerService::CapsuleSettingBase::GetInstallStatus()
{
    return NULL; // E_NOT_IMPLEMENTED
}

void CCapsuleManagerService::CapsuleSettingBase::SetTimeStamp(
    /* [in] */ Int64 newStamp)
{
    // E_NOT_IMPLEMENTED
}

void CCapsuleManagerService::CapsuleSettingBase::CopyFrom(
    /* [in] */ CapsuleSettingBase& base)
{
    // E_NOT_IMPLEMENTED
}

Boolean CCapsuleManagerService::CapsuleSettingBase::EnableComponentLP(
    /* [in] */ String componentClassName)
{
    return FALSE; // E_NOT_IMPLEMENTED
}

Boolean CCapsuleManagerService::CapsuleSettingBase::DisableComponentLP(
    /* [in] */ String componentClassName)
{
    return FALSE; // E_NOT_IMPLEMENTED
}

Boolean CCapsuleManagerService::CapsuleSettingBase::RestoreComponentLP(
    /* [in] */ String componentClassName)
{
    return FALSE; // E_NOT_IMPLEMENTED
}

Int32 CCapsuleManagerService::CapsuleSettingBase::CurrentEnabledStateLP(
    /* [in] */ String componentName)
{
    return 0; // E_NOT_IMPLEMENTED
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::CapsuleSetting

CCapsuleManagerService::CapsuleSetting::CapsuleSetting(
    /* [in] */ String name,
    /* [in] */ String realName,
    /* [in] */ IFile* codePath,
    /* [in] */ IFile* resourcePath,
    /* [in] */ String nativeLibraryPathString,
    /* [in] */ Int32 pVersionCode,
    /* [in] */ Int32 capFlags)
    : CapsuleSettingBase(name, realName, codePath,
        resourcePath, nativeLibraryPathString, pVersionCode, capFlags)
    , mUserId(0)
    , mCap(NULL)
    , mSharedUser(NULL)
{
    // E_NOT_IMPLEMENTED
}

CCapsuleManagerService::CapsuleSetting::CapsuleSetting(
    /* [in] */ const CapsuleSetting& orig)
    : CapsuleSettingBase(orig)
    , mUserId(0)
    , mCap(NULL)
    , mSharedUser(NULL)
{
    // E_NOT_IMPLEMENTED
}

CCapsuleManagerService::CapsuleSetting::~CapsuleSetting()
{
}

String CCapsuleManagerService::CapsuleSetting::GetDescription()
{
    return NULL; // E_NOT_IMPLEMENTED
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::SharedUserSetting

CCapsuleManagerService::SharedUserSetting::SharedUserSetting(
    /* [in] */ String _name,
    /* [in] */ Int32 _pkgFlags)
    : GrantedPermissions(_pkgFlags)
    , mUserId(0)
    , mCapsules(5)
{
    mSignatures = new CCapsuleManagerService::CapsuleSignatures;
    // E_NOT_IMPLEMENTED
}

CCapsuleManagerService::SharedUserSetting::~SharedUserSetting()
{
}

String CCapsuleManagerService::SharedUserSetting::GetDescription()
{
    return NULL; // E_NOT_IMPLEMENTED
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::CapsuleHandler


Boolean CCapsuleManagerService::CapsuleHandler::ConnectToService()
{
    if (CCapsuleManagerService::DEBUG_SD_INSTALL) {
        Logger::I(TAG, StringBuffer("Trying to bind to") + " DefaultContainerService");
    }
    AutoPtr<IIntent> service;
    ECode ec = CIntent::New((IIntent**)&service);
    if (FAILED(ec)) return FALSE;
    service->SetComponent(CCapsuleManagerService::DEFAULT_CONTAINER_COMPONENT.Get());
    Process::SetThreadPriority(Process::THREAD_PRIORITY_DEFAULT);
    Boolean isSucceeded = FALSE;
    mOwner->mContext->BindService(service.Get(),
        (IServiceConnection*)mOwner->mDefContainerConn.Get(),
        Context_BIND_AUTO_CREATE,
        &isSucceeded);
    if (isSucceeded) {
        Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
        mBound = TRUE;
        return TRUE;
    }
    Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
    return FALSE;
}

void CCapsuleManagerService::CapsuleHandler::DisconnectService()
{
    mOwner->mContainerService = NULL;
    mBound = FALSE;
    Process::SetThreadPriority(Process::THREAD_PRIORITY_DEFAULT);
    mOwner->mContext->UnbindService((IServiceConnection*)mOwner->mDefContainerConn.Get());
    Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
}

void CCapsuleManagerService::CapsuleHandler::HandleMessage(
    /* [in] */ Message* msg)
{
    DoHandleMessage(msg);
    Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
}

void CCapsuleManagerService::CapsuleHandler::DoHandleMessage(
    /* [in] */ Message* msg)
{
    switch (msg->mWhat) {
        case CCapsuleManagerService::INIT_COPY: {
            if (DEBUG_SD_INSTALL) Logger::I(TAG, "init_copy");
            HandlerParams* params = (HandlerParams*) msg->mObj;
            Boolean isEmpty = mPendingInstalls.Begin() == mPendingInstalls.End();
            if (DEBUG_SD_INSTALL) {
                Logger::I(TAG, StringBuffer("idx=") + mPendingInstalls.GetSize());
            }
            // If a bind was already initiated we dont really
            // need to do anything. The pending install
            // will be processed later on.
            if (!mBound) {
                // If this is the only one pending we might
                // have to bind to the service again.
                if (!ConnectToService()) {
                    Slogger::E(TAG, "Failed to bind to media container service");
                    params->ServiceError();
                    delete params;
                    delete msg;
                    return;
                }
                else {
                    // Once we bind to the service, the first
                    // pending request will be processed.
                    mPendingInstalls.PushBack(params);
                }
            }
            else {
                mPendingInstalls.PushBack(params);
                // Already bound to the service. Just make
                // sure we trigger off processing the first request.
                if (isEmpty) {
                    Message* msg = new Message();
                    msg->mWhat = CCapsuleManagerService::MCS_BOUND;
                    SendMessage(msg);
                }
            }
            delete params;
            break;
        }
        case CCapsuleManagerService::MCS_BOUND: {
            if (DEBUG_SD_INSTALL) Logger::I(TAG, "mcs_bound");
            if (msg->mObj != NULL) {
                mOwner->mContainerService = (IMediaContainerService*)msg->mObj;
            }
            if (mOwner->mContainerService == NULL) {
                // Something seriously wrong. Bail out
                Slogger::E(TAG, "Cannot bind to media container service");
                List<HandlerParams*>::Iterator itor;
                for (itor = mPendingInstalls.Begin();
                     itor != mPendingInstalls.End();
                     itor++) {
                    // Indicate service bind error
                    (*itor)->ServiceError();
                }
                mPendingInstalls.Clear();
            }
            else if (mPendingInstalls.Begin() != mPendingInstalls.End()) {
                HandlerParams* params = mPendingInstalls.GetFront();
                if (params != NULL) {
                    params->StartCopy();
                }
            }
            else {
                // Should never happen ideally.
                Slogger::W(TAG, "Empty queue");
            }
            break;
        }
        case CCapsuleManagerService::MCS_RECONNECT : {
            if (DEBUG_SD_INSTALL) Logger::I(TAG, "mcs_reconnect");
            if (mPendingInstalls.Begin() != mPendingInstalls.End()) {
                if (mBound) {
                    DisconnectService();
                }
                if (!ConnectToService()) {
                    Slogger::E(TAG, "Failed to bind to media container service");
                    List<HandlerParams*>::Iterator itor;
                    for (itor = mPendingInstalls.Begin();
                         itor != mPendingInstalls.End();
                         itor++) {
                        // Indicate service bind error
                        (*itor)->ServiceError();
                    }
                    mPendingInstalls.Clear();
                }
            }
            break;
        }
        case CCapsuleManagerService::MCS_UNBIND : {
            if (DEBUG_SD_INSTALL) Logger::I(TAG, "mcs_unbind");
            // Delete pending install
            if (mPendingInstalls.Begin() != mPendingInstalls.End()) {
                mPendingInstalls.PopFront();
            }
            if (mPendingInstalls.Begin() == mPendingInstalls.End()) {
                if (mBound) {
                    DisconnectService();
                }
            }
            else {
                // There are more pending requests in queue.
                // Just post MCS_BOUND message to trigger processing
                // of next pending install.
                Message* msg = new Message();
                msg->mWhat = CCapsuleManagerService::MCS_BOUND;
                SendMessage(msg);
            }
            break;
        }
        case CCapsuleManagerService::MCS_GIVE_UP: {
            if (DEBUG_SD_INSTALL) Logger::I(TAG, "mcs_giveup too many retries");
            HandlerParams* params = mPendingInstalls.GetFront();
            (void)params;
            mPendingInstalls.PopFront();
            break;
        }
        case CCapsuleManagerService::SEND_PENDING_BROADCAST : {
            AutoString* capsules = NULL;
            List<String>* components = NULL;
            Int32 size = 0;
            Int32* uids = NULL;
            Process::SetThreadPriority(Process::THREAD_PRIORITY_DEFAULT);
            {
                Mutex::Autolock lock(mOwner->mCapsulesLock);
                size = mOwner->mPendingBroadcasts.GetSize();
                if (size <= 0) {
                    // Nothing to be done. Just return
                    delete msg;
                    return;
                }
                capsules = new AutoString[size];
                components = new List<String>[size];
                uids = new Int32[size];
                Int32 i = 0;
                HashMap< String, List<String> >::Iterator it;
                for (it = mOwner->mPendingBroadcasts.Begin();
                     it != mOwner->mPendingBroadcasts.End();
                     it++) {
                    capsules[i] = String::Duplicate(it->mFirst);
                    components[i] = it->mSecond;
                    HashMap<String, CapsuleSetting*>::Iterator t =
                        mOwner->mSettings->mCapsules.Find(it->mFirst);
                    if (t != mOwner->mSettings->mCapsules.End()
                        && t->mSecond != NULL) {
                        uids[i] = t->mSecond->mUserId;
                    }
                    else {
                        uids[i] = -1;
                    }
                    i++;
                }
                mOwner->mPendingBroadcasts.Clear();
            }
            // Send broadcasts
            for (Int32 i = 0; i < size; i++) {
                mOwner->SendCapsuleChangedBroadcast(
                    capsules[i], TRUE, components[i], uids[i]);
            }
            delete uids;
            delete components;
            delete capsules;
            Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
            break;
        }
        case CCapsuleManagerService::START_CLEANING_CAPSULE: {
            String* capsuleName = (String*)msg->mObj;
            Process::SetThreadPriority(Process::THREAD_PRIORITY_DEFAULT);
            {
                Mutex::Autolock lock(mOwner->mCapsulesLock);
                if (!ListUtils::Contains(mOwner->mSettings->mCapsulesToBeCleaned, *capsuleName)) {
                    mOwner->mSettings->mCapsulesToBeCleaned.PushBack(*capsuleName);
                }
            }
            Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
            mOwner->StartCleaningCapsules();
            delete capsuleName;
            break;
        }
        case CCapsuleManagerService::POST_INSTALL: {
            if (DEBUG_INSTALL) {
                Logger::V(TAG, StringBuffer("Handling post-install for ") + msg->mArg1);
            }
            HashMap<Int32, PostInstallData*>::Iterator it =
                mOwner->mRunningInstalls.Find(msg->mArg1);
            PostInstallData* data = it == mOwner->mRunningInstalls.End() ? NULL : it->mSecond;
            mOwner->mRunningInstalls.Erase(msg->mArg1);
            Boolean deleteOld = FALSE;

            if (data != NULL) {
                InstallArgs* args = data->mArgs;
                CapsuleInstalledInfo* res = data->mRes;

                if (res->mReturnCode == CapsuleManager::INSTALL_SUCCEEDED) {
                    res->mRemovedInfo->SendBroadcast(FALSE, TRUE);
                    AutoPtr<CBundle> extras;
                    CBundle::NewByFriend(1, (CBundle**)&extras);
                    extras->PutInt32(Intent_EXTRA_UID, res->mUid);
                    Boolean update = !res->mRemovedInfo->mRemovedCapsule.IsNull();
                    if (update) {
                        extras->PutBoolean(Intent_EXTRA_REPLACING, TRUE);
                    }
                    mOwner->SendCapsuleBroadcast(Intent_ACTION_CAPSULE_ADDED,
                        res->mCap->mApplicationInfo->mCapsuleName, (IBundle*)extras.Get(), NULL);
                    if (update) {
                        mOwner->SendCapsuleBroadcast(Intent_ACTION_CAPSULE_REPLACED,
                            res->mCap->mApplicationInfo->mCapsuleName, (IBundle*)extras.Get(), NULL);
                    }
                    if (res->mRemovedInfo->mArgs != NULL) {
                        // Remove the replaced capsule's older resources safely now
                        deleteOld = TRUE;
                    }
                }
                // Force a gc to clear up things
//	                Runtime->MGetRuntime()->Gc();
                // We delete after a gc for applications  on sdcard.
                if (deleteOld) {
                    Mutex::Autolock Lock(mOwner->mInstallLock);
                    res->mRemovedInfo->mArgs->DoPostDeleteLI(TRUE);
                }
                if (args->mObserver != NULL) {
                    if (FAILED(args->mObserver->CapsuleInstalled(res->mName, res->mReturnCode))) {
                        Slogger::I(TAG, "Observer no Int64er exists.");
                    }
                }
            }
            else {
                Slogger::E(TAG, StringBuffer("Bogus post-install token ") + msg->mArg1);
            }
            break;
        }
        case CCapsuleManagerService::UPDATED_MEDIA_STATUS: {
            if (DEBUG_SD_INSTALL) Logger::I(TAG, "Got message UPDATED_MEDIA_STATUS");
            Boolean reportStatus = msg->mArg1 == 1;
            Boolean doGc = msg->mArg2 == 1;
            if (DEBUG_SD_INSTALL) {
                Logger::I(TAG,
                    StringBuffer("reportStatus=") + reportStatus + ", doGc = " + doGc);
            }
            if (doGc) {
                // Force a gc to clear up stale containers.
//	                Runtime::GetRuntime().Gc();
            }
            if (msg->mObj != NULL) {
                Set<SdInstallArgs*>* args = (Set<SdInstallArgs*>*) msg->mObj;
                if (DEBUG_SD_INSTALL) Logger::I(TAG, "Unloading all containers");
                // Unload containers
                mOwner->UnloadAllContainers(*args);
            }
            if (reportStatus) {
                if (DEBUG_SD_INSTALL) Logger::I(TAG, "Invoking MountService call back");
                if (FAILED(CapsuleHelper::GetMountService()->FinishMediaUpdate())) {
                    Logger::E(TAG, "MountService not running?");
                }
            }
            break;
        }
        case CCapsuleManagerService::WRITE_SETTINGS: {
            Process::SetThreadPriority(Process::THREAD_PRIORITY_DEFAULT);
            {
                Mutex::Autolock lock(mOwner->mCapsulesLock);
                RemoveMessages(WRITE_SETTINGS);
                mOwner->mSettings->WriteLP();
            }
            Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
            break;
        }
        case CCapsuleManagerService::DELETE_CAPSULE: {
            String* capName = (String*)msg->mArg1;
            Int32 flags = msg->mArg2;
            ICapsuleDeleteObserver* observer = (ICapsuleDeleteObserver*)msg->mObj;
            RemoveMessages(DELETE_CAPSULE);
            Boolean succeded = mOwner->DeleteCapsuleX(*capName, TRUE, TRUE, flags);
            String::Free(*capName);
            delete capName;
            if (observer != NULL) {
                if (FAILED(observer->CapsuleDeleted(succeded))) {
                    Logger::I(TAG, "Observer no Int64er exists.");
                }
                observer->Release();
            }
            break;
        }
    }
    delete msg;
}

ECode CCapsuleManagerService::CapsuleHandler::SendMessage(
    /* [in] */ Message* msg)
{
    void (STDCALL CapsuleHandler::*pHandlerFunc)(Message*);
    pHandlerFunc = &CapsuleHandler::HandleMessage;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)msg);
    return mOwner->mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params.Get());
}

Boolean CCapsuleManagerService::CapsuleHandler::IsMessageRemoved(
    /* [in] */ IParcel* params)
{
    if (!params) return FALSE;

    Int32 size = 0;
    params->GetElementSize(&size);
    if (size != (Int32)sizeof(Message*)) return FALSE;

    Int32 value = 0;
    params->ReadInt32(&value);
    Message* msg = (Message*)value;
    assert(msg);

    return msg->mWhat == mRemovedMsgCode;
}

ECode CCapsuleManagerService::CapsuleHandler::RemoveMessages(
    /* [in] */ Int32 msgCode)
{
    Mutex::Autolock lock(mLock);

    mRemovedMsgCode = msgCode;

    Boolean (STDCALL CapsuleHandler::*pHandlerFunc)(IParcel*);
    pHandlerFunc = &CapsuleHandler::IsMessageRemoved;
    return mOwner->mApartment->RemoveCppCallbacks(
        (Handle32)this, *(Handle32*)&pHandlerFunc);
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::HandlerParams

const Int32 CCapsuleManagerService::HandlerParams::MAX_RETRIES;

void CCapsuleManagerService::HandlerParams::StartCopy()
{
//	    try {
//	        if (DEBUG_SD_INSTALL) Log.i(TAG, "startCopy");
//	        retry++;
//	        if (retry > MAX_RETRIES) {
//	            Slog.w(TAG, "Failed to invoke remote methods on default container service. Giving up");
//	            mHandler.sendEmptyMessage(MCS_GIVE_UP);
//	            handleServiceError();
//	            return;
//	        } else {
//	            handleStartCopy();
//	            if (DEBUG_SD_INSTALL) Log.i(TAG, "Posting install MCS_UNBIND");
//	            mHandler.sendEmptyMessage(MCS_UNBIND);
//	        }
//	    } catch (RemoteException e) {
//	        if (DEBUG_SD_INSTALL) Log.i(TAG, "Posting install MCS_RECONNECT");
//	        mHandler.sendEmptyMessage(MCS_RECONNECT);
//	    }
//	    handleReturnCode();
}

void CCapsuleManagerService::HandlerParams::ServiceError()
{
//	    if (DEBUG_SD_INSTALL) Log.i(TAG, "serviceError");
//	        handleServiceError();
//	        handleReturnCode();
}

////////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::SdInstallArgs

const String CCapsuleManagerService::SdInstallArgs::RES_FILE_NAME = "cap.apk";

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::InstallParams

Int32 CCapsuleManagerService::InstallParams::InstallLocationPolicy(
    /* [in] */ ICapsuleInfoLite* capLite,
    /* [in] */ Int32 flags)
{
//	    String packageName = pkgLite.packageName;
//	    int installLocation = pkgLite.installLocation;
//	    boolean onSd = (flags & PackageManager.INSTALL_EXTERNAL) != 0;
//	    synchronized (mPackages) {
//	        PackageParser.Package pkg = mPackages.get(packageName);
//	        if (pkg != null) {
//	            if ((flags & PackageManager.INSTALL_REPLACE_EXISTING) != 0) {
//	                // Check for updated system application.
//	                if ((pkg.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) != 0) {
//	                    if (onSd) {
//	                        Slog.w(TAG, "Cannot install update to system app on sdcard");
//	                        return PackageHelper.RECOMMEND_FAILED_INVALID_LOCATION;
//	                    }
//	                    return PackageHelper.RECOMMEND_INSTALL_INTERNAL;
//	                } else {
//	                    if (onSd) {
//	                        // Install flag overrides everything.
//	                        return PackageHelper.RECOMMEND_INSTALL_EXTERNAL;
//	                    }
//	                    // If current upgrade specifies particular preference
//	                    if (installLocation == PackageInfo.INSTALL_LOCATION_INTERNAL_ONLY) {
//	                        // Application explicitly specified internal.
//	                        return PackageHelper.RECOMMEND_INSTALL_INTERNAL;
//	                    } else if (installLocation == PackageInfo.INSTALL_LOCATION_PREFER_EXTERNAL) {
//	                        // App explictly prefers external. Let policy decide
//	                    } else {
//	                        // Prefer previous location
//	                        if (isExternal(pkg)) {
//	                            return PackageHelper.RECOMMEND_INSTALL_EXTERNAL;
//	                        }
//	                        return PackageHelper.RECOMMEND_INSTALL_INTERNAL;
//	                    }
//	                }
//	            } else {
//	                // Invalid install. Return error code
//	                return PackageHelper.RECOMMEND_FAILED_ALREADY_EXISTS;
//	            }
//	        }
//	    }
//	    // All the special cases have been taken care of.
//	    // Return result based on recommended install location.
//	    if (onSd) {
//	        return PackageHelper.RECOMMEND_INSTALL_EXTERNAL;
//	    }
//	    return pkgLite.recommendedInstallLocation;
    return -1;
}

void CCapsuleManagerService::InstallParams::HandleStartCopy()
{
//	    int ret = PackageManager.INSTALL_SUCCEEDED;
//	    boolean fwdLocked = (flags & PackageManager.INSTALL_FORWARD_LOCK) != 0;
//	    boolean onSd = (flags & PackageManager.INSTALL_EXTERNAL) != 0;
//	    boolean onInt = (flags & PackageManager.INSTALL_INTERNAL) != 0;
//	    if (onInt && onSd) {
//	        // Check if both bits are set.
//	        Slog.w(TAG, "Conflicting flags specified for installing on both internal and external");
//	        ret = PackageManager.INSTALL_FAILED_INVALID_INSTALL_LOCATION;
//	    } else if (fwdLocked && onSd) {
//	        // Check for forward locked apps
//	        Slog.w(TAG, "Cannot install fwd locked apps on sdcard");
//	        ret = PackageManager.INSTALL_FAILED_INVALID_INSTALL_LOCATION;
//	    } else {
//	        // Remote call to find out default install location
//	        final PackageInfoLite pkgLite;
//	        try {
//	            mContext.grantUriPermission(DEFAULT_CONTAINER_PACKAGE, packageURI,
//	                    Intent.FLAG_GRANT_READ_URI_PERMISSION);
//	            pkgLite = mContainerService.getMinimalPackageInfo(packageURI, flags);
//	        } finally {
//	            mContext.revokeUriPermission(packageURI, Intent.FLAG_GRANT_READ_URI_PERMISSION);
//	        }
//
//	        int loc = pkgLite.recommendedInstallLocation;
//	        if (loc == PackageHelper.RECOMMEND_FAILED_INVALID_LOCATION){
//	            ret = PackageManager.INSTALL_FAILED_INVALID_INSTALL_LOCATION;
//	        } else if (loc == PackageHelper.RECOMMEND_FAILED_ALREADY_EXISTS){
//	            ret = PackageManager.INSTALL_FAILED_ALREADY_EXISTS;
//	        } else if (loc == PackageHelper.RECOMMEND_FAILED_INSUFFICIENT_STORAGE){
//	            ret = PackageManager.INSTALL_FAILED_INSUFFICIENT_STORAGE;
//	        } else if (loc == PackageHelper.RECOMMEND_FAILED_INVALID_APK) {
//	            ret = PackageManager.INSTALL_FAILED_INVALID_APK;
//	        } else if (loc == PackageHelper.RECOMMEND_MEDIA_UNAVAILABLE) {
//	          ret = PackageManager.INSTALL_FAILED_MEDIA_UNAVAILABLE;
//	        } else {
//	            // Override with defaults if needed.
//	            loc = installLocationPolicy(pkgLite, flags);
//	            if (!onSd && !onInt) {
//	                // Override install location with flags
//	                if (loc == PackageHelper.RECOMMEND_INSTALL_EXTERNAL) {
//	                    // Set the flag to install on external media.
//	                    flags |= PackageManager.INSTALL_EXTERNAL;
//	                    flags &= ~PackageManager.INSTALL_INTERNAL;
//	                } else {
//	                    // Make sure the flag for installing on external
//	                    // media is unset
//	                    flags |= PackageManager.INSTALL_INTERNAL;
//	                    flags &= ~PackageManager.INSTALL_EXTERNAL;
//	                }
//	            }
//	        }
//	    }
//	    // Create the file args now.
//	    mArgs = createInstallArgs(this);
//	    if (ret == PackageManager.INSTALL_SUCCEEDED) {
//	        // Create copy only if we are not in an erroneous state.
//	        // Remote call to initiate copy using temporary file
//	        ret = mArgs.copyApk(mContainerService, true);
//	    }
//	    mRet = ret;
}

void CCapsuleManagerService::InstallParams::HandleReturnCode()
{
//	    // If mArgs is null, then MCS couldn't be reached. When it
//	    // reconnects, it will try again to install. At that point, this
//	    // will succeed.
//	    if (mArgs != null) {
//	        processPendingInstall(mArgs, mRet);
//	    }
}

void CCapsuleManagerService::InstallParams::HandleServiceError()
{
//	    mArgs = createInstallArgs(this);
//	    mRet = PackageManager.INSTALL_FAILED_INTERNAL_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::CapsuleRemovedInfo

void CCapsuleManagerService::CapsuleRemovedInfo::SendBroadcast(
    /* [in] */ Boolean fullRemove,
    /* [in] */ Boolean replacing)
{
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::Settings

CCapsuleManagerService::Settings::Settings()
    : mCapsules(5)
    , mDisabledSysCapsules(5)
    , mInternalSdkPlatform(0)
    , mExternalSdkPlatform(0)
    , mPreferredActivities(new MyIntentResolver())
    , mSharedUsers(5)
    , mPermissions(5)
    , mPermissionTrees(5)
    , mRenamedPackages(5)
{
    // E_NOT_IMPLEMENTED
}

CCapsuleManagerService::Settings::~Settings()
{
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService

const String CCapsuleManagerService::TAG = "CCapsuleManagerService";
const Boolean CCapsuleManagerService::DEBUG_SETTINGS;
const Boolean CCapsuleManagerService::DEBUG_PREFERRED;
const Boolean CCapsuleManagerService::DEBUG_UPGRADE;
const Boolean CCapsuleManagerService::DEBUG_INSTALL;
const Boolean CCapsuleManagerService::MULTIPLE_APPLICATION_UIDS;
const Int32 CCapsuleManagerService::RADIO_UID;
const Int32 CCapsuleManagerService::LOG_UID;
const Int32 CCapsuleManagerService::NFC_UID;
const Int32 CCapsuleManagerService::FIRST_APPLICATION_UID;
const Int32 CCapsuleManagerService::MAX_APPLICATION_UIDS;
const Boolean CCapsuleManagerService::SHOW_INFO;
const Boolean CCapsuleManagerService::GET_CERTIFICATES;
const String CCapsuleManagerService::SYSTEM_PROPERTY_EFS_ENABLED = "persist.security.efs.enabled";
//const int REMOVE_EVENTS =
//	  FileObserver.CLOSE_WRITE | FileObserver.DELETE | FileObserver.MOVED_FROM;
//	  const int ADD_EVENTS =
//	      FileObserver.CLOSE_WRITE /*| FileObserver.CREATE*/ | FileObserver.MOVED_TO;

//const int OBSERVER_EVENTS = REMOVE_EVENTS | ADD_EVENTS;
// Suffix used during package installation when copying/moving
// package apks to install directory.
const String CCapsuleManagerService::INSTALL_PACKAGE_SUFFIX = "-";
const Int32 CCapsuleManagerService::CAP_INSTALL_INCOMPLETE;
const Int32 CCapsuleManagerService::CAP_INSTALL_COMPLETE;
const Int32 CCapsuleManagerService::SCAN_MONITOR;
const Int32 CCapsuleManagerService::SCAN_NO_DEX;
const Int32 CCapsuleManagerService::SCAN_FORCE_DEX;
const Int32 CCapsuleManagerService::SCAN_UPDATE_SIGNATURE;
const Int32 CCapsuleManagerService::SCAN_NEW_INSTALL;
const Int32 CCapsuleManagerService::SCAN_NO_PATHS;
const Int32 CCapsuleManagerService::SCAN_UPDATE_TIME;
const Int32 CCapsuleManagerService::REMOVE_CHATTY;
const String CCapsuleManagerService::DEFAULT_CONTAINER_CAPSULE = "com.android.defcontainer";
//	    const ComponentName DEFAULT_CONTAINER_COMPONENT = new ComponentName(
//	            DEFAULT_CONTAINER_PACKAGE,
//	            "com.android.defcontainer.DefaultContainerService");
const String CCapsuleManagerService::LIB_DIR_NAME = "lib";
const String CCapsuleManagerService::mTempContainerPrefix = "smdl2tmp";
const Int32 CCapsuleManagerService::SEND_PENDING_BROADCAST;
const Int32 CCapsuleManagerService::MCS_BOUND;
const Int32 CCapsuleManagerService::END_COPY;
const Int32 CCapsuleManagerService::INIT_COPY;
const Int32 CCapsuleManagerService::MCS_UNBIND;
const Int32 CCapsuleManagerService::START_CLEANING_CAPSULE;
const Int32 CCapsuleManagerService::FIND_INSTALL_LOC;
const Int32 CCapsuleManagerService::POST_INSTALL;
const Int32 CCapsuleManagerService::MCS_RECONNECT;
const Int32 CCapsuleManagerService::MCS_GIVE_UP;
const Int32 CCapsuleManagerService::UPDATED_MEDIA_STATUS;
const Int32 CCapsuleManagerService::WRITE_SETTINGS;
const Int32 CCapsuleManagerService::DELETE_CAPSULE;
const Int32 CCapsuleManagerService::WRITE_SETTINGS_DELAY;
const Int32 CCapsuleManagerService::BROADCAST_DELAY;
const Int32 CCapsuleManagerService::DEX_OPT_SKIPPED;
const Int32 CCapsuleManagerService::DEX_OPT_PERFORMED;
const Int32 CCapsuleManagerService::DEX_OPT_FAILED;
const Int32 CCapsuleManagerService::MAX_CONTAINERS;
//	    private static final Comparator<ResolveInfo> mResolvePrioritySorter =
//	            new Comparator<ResolveInfo>() {
//
//	    private static final Comparator<ProviderInfo> mProviderInitOrderSorter =
//	            new Comparator<ProviderInfo>() {
const Boolean CCapsuleManagerService::DEBUG_SD_INSTALL;
const String CCapsuleManagerService::SD_ENCRYPTION_KEYSTORE_NAME = "AppsOnSD";
const String CCapsuleManagerService::SD_ENCRYPTION_ALGORITHM = "AES";
const String CCapsuleManagerService::DEFAULT_RESOURCES_FILE_NAME = "hello.apk";
const AutoPtr<IComponentName> CCapsuleManagerService::DEFAULT_CONTAINER_COMPONENT;

CCapsuleManagerService::CCapsuleManagerService() :
    mHandler(NULL),
    mSdkVersion(0),
    mFactoryTest(FALSE),
    mNoDexOpt(FALSE),
    mDefParseFlags(0),
    mInstaller(NULL),
    mLastScanError(0),
    mCapsules(23),
    mSettings(NULL),
    mRestoredSettings(FALSE),
    mSharedLibraries(11),
    mProvidersByComponent(11),
    mProviders(11),
    mInstrumentation(11),
    mTransferedCapsules(11),
    mProtectedBroadcasts(11),
    mSystemReady(FALSE),
    mSafeMode(FALSE),
    mHasSystemUidErrors(FALSE),
    mPlatformCapsule(NULL),
    mNextInstallToken(1),
    mDefContainerConn(new CCapsuleManagerService::DefaultContainerConnection(this)),
    mMediaMounted(FALSE)
{
    if (DEFAULT_CONTAINER_COMPONENT == NULL) {
        ASSERT_SUCCEEDED(CComponentName::New(
            DEFAULT_CONTAINER_CAPSULE,
            "com.android.defcontainer.DefaultContainerService",
            (IComponentName**)&DEFAULT_CONTAINER_COMPONENT));
    }

    char cwd[512];

    mActivities = new ActivityIntentResolver();
    mReceivers = new ActivityIntentResolver();
    mServices = new ServiceIntentResolver();
    CActivityInfo::NewByFriend((CActivityInfo**)&mResolveActivity);
    CResolveInfo::NewByFriend((CResolveInfo**)&mResolveInfo);

    getcwd(cwd, 512);
    ScanDir(String(cwd));
}

CCapsuleManagerService::~CCapsuleManagerService()
{
    delete mSettings;
    //free capsuleInfos
    mCapsules.Clear();
    delete mActivities;
    delete mReceivers;
    mProviders.Clear();
    mProvidersByComponent.Clear();
    mInstrumentation.Clear();
}

ECode CCapsuleManagerService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Boolean factoryTest)
{
//	    EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_START,
//	            SystemClock.uptimeMillis());

    if (mSdkVersion <= 0) {
        Slogger::W(TAG, "**** ro->mBuild.version->mSdk not set!");
    }

    mContext = context;
    mFactoryTest = factoryTest;
    AutoString str;
    SystemProperties::Get("ro.build.type", &str);
    mNoDexOpt = str.Equals("eng");
    FAIL_RETURN(CDisplayMetrics::New((IDisplayMetrics**)&mMetrics));
    mSettings = new Settings();
    if (!mSettings) return E_OUT_OF_MEMORY;
    mSettings->AddSharedUserLP("android.uid.system",
            Process::SYSTEM_UID, CApplicationInfo::FLAG_SYSTEM);
    mSettings->AddSharedUserLP("android.uid.phone",
            MULTIPLE_APPLICATION_UIDS ? RADIO_UID : FIRST_APPLICATION_UID,
            CApplicationInfo::FLAG_SYSTEM);
    mSettings->AddSharedUserLP("android.uid.log",
            MULTIPLE_APPLICATION_UIDS ? LOG_UID : FIRST_APPLICATION_UID,
            CApplicationInfo::FLAG_SYSTEM);
    mSettings->AddSharedUserLP("android.uid.nfc",
            MULTIPLE_APPLICATION_UIDS ? NFC_UID : FIRST_APPLICATION_UID,
            CApplicationInfo::FLAG_SYSTEM);

    AutoString separateProcesses;
	SystemProperties::Get("debug->mSeparate_processes", &separateProcesses);
    if (!separateProcesses.IsNull() && separateProcesses.GetLength() > 0) {
        if (separateProcesses.Equals("*")) {
            mDefParseFlags = CapsuleParser::PARSE_IGNORE_PROCESSES;
            mSeparateProcesses = NULL;
            Slogger::W(TAG, "Running with debug->mSeparate_processes: * (ALL)");
        } else {
            mDefParseFlags = 0;
            SplitString(separateProcesses, ',', (ArrayOf<String>**)&mSeparateProcesses);
            Slogger::W(TAG, StringBuffer("Running with debug->mSeparate_processes: ")
                + separateProcesses);
        }
    } else {
        mDefParseFlags = 0;
        mSeparateProcesses = NULL;
    }

    Installer* installer = new Installer();
    if (!installer) return E_OUT_OF_MEMORY_ERROR;
    // Little hacky thing to check if installd is here, to determine
    // whether we are running on the simulator and thus need to take
    // care of building the /data file structure ourself.
    // (apparently the sim now has a working installer)
    if (installer->Ping() && Process::SupportsProcesses()) {
        mInstaller = installer;
    } else {
        mInstaller = NULL;
    }

    AutoPtr<IServiceManager> serviceManager;
    AutoPtr<IWindowManager> wm;
    AutoPtr<IDisplay> d;

    GetServiceManager((IServiceManager**)&serviceManager);

    serviceManager->GetService(Context_WINDOW_SERVICE, (IInterface**)&wm);
    wm->GetDefaultDisplay((IDisplay**)&d);
    d->GetMetrics(mMetrics.Get());

    {
        Mutex::Autolock ilLock(mInstallLock);
        Mutex::Autolock cLock(mCapsulesLock);
        // mHandlerThread.start();
        // mHandler = new PackageHandler(mHandlerThread.getLooper());
        FAIL_RETURN(CApartment::New(FALSE, (IApartment**)&mApartment));
        mApartment->Start(ApartmentAttr_New);
        mHandler = new CapsuleHandler(this);

        AutoPtr<IFile> dataDir;
        Environment::GetDataDirectory((IFile**)&dataDir);
        FAIL_RETURN(CFile::New(dataDir.Get(), "data", (IFile**)&mAppDataDir));
        FAIL_RETURN(CFile::New(dataDir.Get(), "secure/data", (IFile**)&mSecureAppDataDir));
        FAIL_RETURN(CFile::New(dataDir.Get(), "app-private", (IFile**)&mDrmAppPrivateInstallDir));

        if (mInstaller == NULL) {
            // Make sure these dirs exist, when we are running in
            // the simulator.
            // Make a wide-open directory for random misc stuff.
            AutoPtr<IFile> miscDir;
            FAIL_RETURN(CFile::New(dataDir, "misc", (IFile**)&miscDir));
            Boolean isCreated = FALSE;
            miscDir->Mkdirs(&isCreated);
            mAppDataDir->Mkdirs(&isCreated);
            mSecureAppDataDir->Mkdirs(&isCreated);
            mDrmAppPrivateInstallDir->Mkdirs(&isCreated);
        }

        ReadPermissions();

        mRestoredSettings = mSettings->ReadLP();
        Int64 startTime = SystemClock::UptimeMillis();

//	        EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_SYSTEM_SCAN_START,
//	                startTime);

        // Set flag to monitor and not change apk file paths when
        // scanning install directories.
        Int32 scanMode = SCAN_MONITOR | SCAN_NO_PATHS;
        if (mNoDexOpt) {
            Slogger::W(TAG, "Running ENG build: no pre-dexopt!");
            scanMode |= SCAN_NO_DEX;
        }

        //HashSet<String> libFiles(11);
        HashSet<String> libFiles;

        AutoPtr<IFile> rootDir;
        Environment::GetRootDirectory((IFile**)&rootDir);
        FAIL_RETURN(CFile::New(rootDir.Get(), "framework", (IFile**)&mFrameworkDir));
        FAIL_RETURN(CFile::New(dataDir.Get(), "dalvik-cache", (IFile**)&mDalvikCacheDir));

        if (mInstaller != NULL) {
            Boolean didDexOpt = FALSE;
            (void)didDexOpt;

            /**
             * Out of paranoia, ensure that everything in the boot class
             * path has been dexed.
             */
//	            AutoString bootClassPath;
//	            System::GetProperty("java->mBoot.class->mPath", &bootClassPath);
//	            if (!bootClassPath.IsNull()) {
//	                AutoStringArray paths;
//	                SplitString(bootClassPath, ':', (ArrayOf<String>**)&paths);
//	                for (Int32 i=0; i < paths->GetLength(); i++) {
//	                    try {
//	                        if (dalvik->mSystem.DexFile::IsDexOptNeeded(paths[i])) {
//	                            libFiles->Add(paths[i]);
//	                            mInstaller->Dexopt(paths[i], Process::SYSTEM_UID, TRUE);
//	                            didDexOpt = TRUE;
//	                        }
//	                    } catch (FileNotFoundException e) {
//	                        Slogger::W(TAG, "Boot class path not found: " + paths[i]);
//	                    } catch (IOException e) {
//	                        Slogger::W(TAG, "Exception reading boot class path: " + paths[i], e);
//	                    }
//	                }
//	            } else {
//	                Slogger::W(TAG, "No BOOTCLASSPATH found!");
//	            }

//	            /**
//	             * Also ensure all external libraries have had dexopt run on them.
//	             */
//	            if (mSharedLibraries.size() > 0) {
//	                Iterator<String> libs = mSharedLibraries.values().iterator();
//	                while (libs.hasNext()) {
//	                    String lib = libs.next();
//	                    try {
//	                        if (dalvik.system.DexFile.isDexOptNeeded(lib)) {
//	                            libFiles.add(lib);
//	                            mInstaller.dexopt(lib, Process.SYSTEM_UID, true);
//	                            didDexOpt = true;
//	                        }
//	                    } catch (FileNotFoundException e) {
//	                        Slog.w(TAG, "Library not found: " + lib);
//	                    } catch (IOException e) {
//	                        Slog.w(TAG, "Exception reading library: " + lib, e);
//	                    }
//	                }
//	            }

            // Gross hack for now: we know this file doesn't contain any
            // code, so don't dexopt it to avoid the resulting log spew.
            AutoString path;
            mFrameworkDir->GetPath(&path);
            libFiles.Insert(String::Duplicate(StringBuffer(path) + "/framework-res.apk"));

//	            /**
//	             * And there are a number of commands implemented in Java, which
//	             * we currently need to do the dexopt on so that they can be
//	             * run from a non-root shell.
//	             */
//	            String[] frameworkFiles = mFrameworkDir.list();
//	            if (frameworkFiles != null) {
//	                for (int i=0; i<frameworkFiles.length; i++) {
//	                    File libPath = new File(mFrameworkDir, frameworkFiles[i]);
//	                    String path = libPath.getPath();
//	                    // Skip the file if we alrady did it.
//	                    if (libFiles.contains(path)) {
//	                        continue;
//	                    }
//	                    // Skip the file if it is not a type we want to dexopt.
//	                    if (!path.endsWith(".apk") && !path.endsWith(".jar")) {
//	                        continue;
//	                    }
//	                    try {
//	                        if (dalvik.system.DexFile.isDexOptNeeded(path)) {
//	                            mInstaller.dexopt(path, Process.SYSTEM_UID, true);
//	                            didDexOpt = true;
//	                        }
//	                    } catch (FileNotFoundException e) {
//	                        Slog.w(TAG, "Jar not found: " + path);
//	                    } catch (IOException e) {
//	                        Slog.w(TAG, "Exception reading jar: " + path, e);
//	                    }
//	                }
//	            }
//
//	            if (didDexOpt) {
//	                // If we had to do a dexopt of one of the previous
//	                // things, then something on the system has changed.
//	                // Consider this significant, and wipe away all other
//	                // existing dexopt files to ensure we don't leave any
//	                // dangling around.
//	                String[] files = mDalvikCacheDir.list();
//	                if (files != null) {
//	                    for (int i=0; i<files.length; i++) {
//	                        String fn = files[i];
//	                        if (fn.startsWith("data@app@")
//	                                || fn.startsWith("data@app-private@")) {
//	                            Slog.i(TAG, "Pruning dalvik file: " + fn);
//	                            (new File(mDalvikCacheDir, fn)).delete();
//	                        }
//	                    }
//	                }
//	            }
        }

        // Find base frameworks (resource packages without code).
//	        mFrameworkInstallObserver = new AppDirObserver(
//	            mFrameworkDir.getPath(), OBSERVER_EVENTS, true);
//	        mFrameworkInstallObserver.startWatching();
        ScanDirLI(mFrameworkDir.Get(), CapsuleParser::PARSE_IS_SYSTEM
            | CapsuleParser::PARSE_IS_SYSTEM_DIR,
            scanMode | SCAN_NO_DEX, 0);

        // Collect all system packages.
        FAIL_RETURN(CFile::New(rootDir.Get(), "app", (IFile**)&mSystemAppDir));
//	        mSystemInstallObserver = new AppDirObserver(
//	            mSystemAppDir.getPath(), OBSERVER_EVENTS, true);
//	        mSystemInstallObserver.startWatching();
        ScanDirLI(mSystemAppDir.Get(), CapsuleParser::PARSE_IS_SYSTEM
            | CapsuleParser::PARSE_IS_SYSTEM_DIR, scanMode, 0);

        // Collect all vendor packages.
        FAIL_RETURN(CFile::New("/vendor/app", (IFile**)&mVendorAppDir));
//	        mVendorInstallObserver = new AppDirObserver(
//	            mVendorAppDir.getPath(), OBSERVER_EVENTS, true);
//	        mVendorInstallObserver.startWatching();
        ScanDirLI(mVendorAppDir.Get(), CapsuleParser::PARSE_IS_SYSTEM
            | CapsuleParser::PARSE_IS_SYSTEM_DIR, scanMode, 0);

        if (mInstaller != NULL) {
            if (DEBUG_UPGRADE) Logger::V(TAG, "Running installd update commands");
            mInstaller->MoveFiles();
        }

//	        // Prune any system packages that no longer exist.
//	        Iterator<PackageSetting> psit = mSettings.mPackages.values().iterator();
//	        while (psit.hasNext()) {
//	            PackageSetting ps = psit.next();
//	            if ((ps.pkgFlags&ApplicationInfo.FLAG_SYSTEM) != 0
//	                    && !mPackages.containsKey(ps.name)
//	                    && !mSettings.mDisabledSysPackages.containsKey(ps.name)) {
//	                psit.remove();
//	                String msg = "System package " + ps.name
//	                        + " no longer exists; wiping its data";
//	                reportSettingsProblem(Log.WARN, msg);
//	                if (mInstaller != null) {
//	                    // XXX how to set useEncryptedFSDir for packages that
//	                    // are not encrypted?
//	                    mInstaller.remove(ps.name, true);
//	                }
//	            }
//	        }

        FAIL_RETURN(CFile::New(dataDir.Get(), "app", (IFile**)&mAppInstallDir));

        if (mInstaller == NULL) {
            // Make sure these dirs exist, when we are running in
            // the simulator.
            Boolean isCreated = FALSE;
            mAppInstallDir->Mkdirs(&isCreated); // ScanDirLI() assumes this dir exists
        }
//	        //look for any incomplete package installations
//	        ArrayList<PackageSetting> deletePkgsList = mSettings.getListOfIncompleteInstallPackages();
//	        //clean up list
//	        for(int i = 0; i < deletePkgsList.size(); i++) {
//	            //clean up here
//	            cleanupInstallFailedPackage(deletePkgsList.get(i));
//	        }
//	        //delete tmp files
//	        deleteTempPackageFiles();

//	        EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_DATA_SCAN_START,
//	                SystemClock.uptimeMillis());
//	        mAppInstallObserver = new AppDirObserver(
//	            mAppInstallDir.getPath(), OBSERVER_EVENTS, false);
//	        mAppInstallObserver.startWatching();
        ScanDirLI(mAppInstallDir.Get(), 0, scanMode, 0);

//	        mDrmAppInstallObserver = new AppDirObserver(
//	            mDrmAppPrivateInstallDir.getPath(), OBSERVER_EVENTS, false);
//	        mDrmAppInstallObserver.startWatching();
        ScanDirLI(mDrmAppPrivateInstallDir.Get(), CapsuleParser::PARSE_FORWARD_LOCK,
            scanMode, 0);

//	        EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_SCAN_END,
//	                SystemClock.uptimeMillis());

        Slogger::I(TAG, StringBuffer("Time to scan capsules: ")
                + (Int32)(((SystemClock::UptimeMillis() - startTime) / 1000.0f))
                + " seconds");

        // If the platform SDK has changed since the last time we booted,
        // we need to re-grant app permission to catch any new ones that
        // appear.  This is really a hack, and means that apps can in some
        // cases get permissions that the user didn't initially explicitly
        // allow...  it would be nice to have some better way to handle
        // this situation.
        Boolean regrantPermissions = mSettings->mInternalSdkPlatform != mSdkVersion;
        if (regrantPermissions) Slogger::I(TAG, StringBuffer("Platform changed from ")
                + mSettings->mInternalSdkPlatform + " to " + mSdkVersion
                + "; regranting permissions for Int32ernal storage");
        mSettings->mInternalSdkPlatform = mSdkVersion;

        UpdatePermissionsLP(String(NULL), NULL, TRUE, regrantPermissions, regrantPermissions);

        mSettings->WriteLP();

//	        EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_READY,
//	                SystemClock.uptimeMillis());

        // Now after opening every single application zip, make sure they
        // are all flushed.  Not really needed, but keeps things nice and
        // tidy.
//	        Runtime.getRuntime().gc();
    }
    return NOERROR;
}

ECode CCapsuleManagerService::SplitString(
    /* [in] */ String str,
    /* [in] */ Char8 sep,
    /* [out] */ ArrayOf<String>** outStrs)
{
    assert(outStrs);
    assert(str.IsNull());

    Char8 delim[] = { sep, '\0' };
    StringTokenizer* tokens = new StringTokenizer(str, delim);
    Int32 count = tokens->GetCount();
    ArrayOf<String>* res = ArrayOf<String>::Alloc(count);
    Int32 i = 0;
    while (tokens->HasMoreTokens()) {
        (*res)[i++] = String::Duplicate(tokens->NextToken());
    }
    delete tokens;
    *outStrs = res;
    return NOERROR;
}

ECode CCapsuleManagerService::ReadPermissions()
{
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::GenerateCapsuleInfo(
    /* [in] */ CapsuleParser::Capsule* c,
    /* [in] */ Int32 flags,
    /* [out] */ ICapsuleInfo** capInfo)
{
    assert(capInfo);

    if ((flags & CapsuleManager_GET_UNINSTALLED_CAPSULES) != 0) {
        // The package has been uninstalled but has retained data and resources.
        return CapsuleParser::GenerateCapsuleInfo(c, NULL, flags, 0, 0, capInfo);
    }
    CapsuleSetting* cs = (CapsuleSetting*)c->mExtras;
    if (cs == NULL) {
        *capInfo = NULL;
        return E_FAIL;
    }
    GrantedPermissions* gp = cs->mSharedUser != NULL
        ? (GrantedPermissions*)cs->mSharedUser
        : (GrantedPermissions*)cs;
    return CapsuleParser::GenerateCapsuleInfo(c, gp->mGids, flags,
            cs->mFirstInstallTime, cs->mLastUpdateTime, capInfo);
}

ECode CCapsuleManagerService::GetCapsuleInfo(
    /* [in] */ String capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ ICapsuleInfo** capInfo)
{
    VALIDATE_NOT_NULL(capInfo);
    VALIDATE_STRING_NOT_NULL(capsuleName);

    Mutex::Autolock lock(mCapsulesLock);
    HashMap<String, CapsuleParser::Capsule*>::Iterator itor = mCapsules.Find(capsuleName);
    if (Config::LOGV) {
        Slogger::V(TAG, StringBuffer("getPackageInfo ")
            + capsuleName + ": " + (Int32)(*itor).mSecond);
    }
    if (itor != mCapsules.End()) {
        return GenerateCapsuleInfo((*itor).mSecond, flags, capInfo);
    }
    if((flags & CapsuleManager_GET_UNINSTALLED_CAPSULES) != 0) {
        return GenerateCapsuleInfoFromSettingsLP(capsuleName, flags, capInfo);
    }
    *capInfo = NULL;
    return E_FAIL;
}

ECode CCapsuleManagerService::CurrentToCanonicalCapsuleNames(
    /* [in] */ const ArrayOf<String>& names,
    /* [out, callee] */ ArrayOf<String>** pnames)
{
    VALIDATE_NOT_NULL(pnames);

    ArrayOf<String>* out = ArrayOf<String>::Alloc(names.GetLength());
    if (!out) return E_OUT_OF_MEMORY;
    {
        Mutex::Autolock lock(mCapsulesLock);
        for (Int32 i = names.GetLength() - 1; i >= 0; i--) {
            HashMap<String, CapsuleSetting*>::Iterator itor =
                                    mSettings->mCapsules.Find(names[i]);
            (*out)[i] = itor != mSettings->mCapsules.End() && !(*itor).mSecond->mRealName.IsNull()
                ? String::Duplicate((*itor).mSecond->mRealName)
                : String::Duplicate(names[i]);
        }
    }
    *pnames = out;
    return NOERROR;
}

ECode CCapsuleManagerService::CanonicalToCurrentCapsuleNames(
    /* [in] */ const ArrayOf<String>& names,
    /* [out, callee] */ ArrayOf<String>** pnames)
{
    VALIDATE_NOT_NULL(pnames);

    ArrayOf<String>* out = ArrayOf<String>::Alloc(names.GetLength());
    if (!out) return E_OUT_OF_MEMORY;
    {
        Mutex::Autolock lock(mCapsulesLock);
        for (Int32 i = names.GetLength() - 1; i >= 0; i--) {
            HashMap<String, String>::Iterator itor =
                                mSettings->mRenamedPackages.Find(names[i]);
            (*out)[i] = itor != mSettings->mRenamedPackages.End()
                ? String::Duplicate((*itor).mSecond) : String::Duplicate(names[i]);
        }
    }
    *pnames = out;
    return NOERROR;
}

ECode CCapsuleManagerService::GetCapsuleUid(
    /* [in] */ String capsuleName,
    /* [out] */ Int32* uid)
{
    VALIDATE_STRING_NOT_NULL(capsuleName);
    VALIDATE_NOT_NULL(uid);

    Mutex::Autolock lock(mCapsulesLock);
    HashMap<String, CapsuleParser::Capsule*>::Iterator myItor = mCapsules.Find(capsuleName);
    if(myItor != mCapsules.End()) {
        *uid = (*myItor).mSecond->mApplicationInfo->mUid;
        return NOERROR;
    }
    HashMap<String, CapsuleSetting*>::Iterator itor = mSettings->mCapsules.Find(capsuleName);
    if( itor == mSettings->mCapsules.End()
        || (*itor).mSecond == NULL
        || (*itor).mSecond->mCap == NULL
        || (*itor).mSecond->mCap->mApplicationInfo == NULL) {
        *uid = -1;
        return E_FAIL;
    }
    *uid = (*itor).mSecond->mCap->mApplicationInfo->mUid;
    return NOERROR;
}

ECode CCapsuleManagerService::GetCapsuleGids(
    /* [in] */ String capsuleName,
    /* [out, callee] */ ArrayOf<Int32>** gids)
{
    VALIDATE_STRING_NOT_NULL(capsuleName);
    VALIDATE_NOT_NULL(gids);

    Mutex::Autolock lock(mCapsulesLock);
    HashMap<String, CapsuleParser::Capsule*>::Iterator itor = mCapsules.Find(capsuleName);
    if (Config::LOGV) {
        Logger::V(TAG, StringBuffer("getPackageGids")
            + capsuleName + ": " + (Int32)(*itor).mSecond);
    }
    if (itor != mCapsules.End()) {
        CapsuleSetting* cs = (CapsuleSetting*)((*itor).mSecond->mExtras);
        SharedUserSetting* suid = cs->mSharedUser;
        *gids = (suid != NULL ? suid->mGids : cs->mGids)->Clone();
        return NOERROR;
    }
    // stupid thing to indicate an error.
    *gids = NULL;
    return E_FAIL;
}

ECode CCapsuleManagerService::GeneratePermissionInfo(
    /* [in] */ BasePermission* bp,
    /* [in] */ Int32 flags,
    /* [out] */ IPermissionInfo** info)
{
    assert(bp);
    assert(info);

    if (bp->mPerm != NULL) {
        return CapsuleParser::GeneratePermissionInfo(bp->mPerm, flags, info);
    }
    AutoPtr<CPermissionInfo> pi;
    FAIL_RETURN(CPermissionInfo::NewByFriend((CPermissionInfo**)&pi));
    pi->mName = String::Duplicate(bp->mName);
    pi->mCapsuleName = String::Duplicate(bp->mSourceCapsule);
    CStringWrapper::New(String::Duplicate(bp->mName), (ICharSequence**)&pi->mNonLocalizedLabel);
    pi->mProtectionLevel = bp->mProtectionLevel;
    *info = (IPermissionInfo*)pi.Get();
    (*info)->AddRef();
    return NOERROR;
}

ECode CCapsuleManagerService::GetPermissionInfo(
    /* [in] */ String name,
    /* [in] */ Int32 flags,
    /* [out] */ IPermissionInfo** info)
{
    VALIDATE_STRING_NOT_NULL(name);
    VALIDATE_NOT_NULL(info);

    Mutex::Autolock lock(mCapsulesLock);
    HashMap<String, BasePermission*>::Iterator itor =
                                mSettings->mPermissions.Find(name);
    if (itor != mSettings->mPermissions.End()) {
        return GeneratePermissionInfo((*itor).mSecond, flags, info);
    }
    *info = NULL;
    return E_FAIL;
}

ECode CCapsuleManagerService::QueryPermissionsByGroup(
    /* [in] */ String group,
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);
    *infos = NULL;

    Mutex::Autolock lock(mCapsulesLock);

    AutoPtr<IObjectContainer> out;
    FAIL_RETURN( CObjectContainer::New((IObjectContainer**)&out) );

    HashMap<String, BasePermission*>::Iterator itor;
    for (itor = mSettings->mPermissions.Begin();
        itor != mSettings->mPermissions.End(); ++itor) {

        AutoPtr<IPermissionInfo> info;
        BasePermission* p = itor->mSecond;

        if (group.IsNull()) {
            if (p->mPerm == NULL
                || p->mPerm->mInfo->mGroup.IsNull()) {
                FAIL_RETURN( GeneratePermissionInfo(
                    p, flags, (IPermissionInfo**)&info) );
                out->Add(info.Get());
            }
        } else {
            if (p->mPerm == NULL && group.Equals(p->mPerm->mInfo->mGroup)) {
                FAIL_RETURN( CapsuleParser::GeneratePermissionInfo(
                    p->mPerm, flags, (IPermissionInfo**)&info) );
                out->Add(info.Get());
            }
        }
    }

    Int32 count = out->GetObjectCount(&count);
    if (count > 0) {
        *infos = out.Get();
        INTERFACE_ADDREF(*infos);
        return NOERROR;
    }
    HashMap<String, CapsuleParser::PermissionGroup*>::Iterator it =
        mPermissionGroups.Find(group);
    if (it != mPermissionGroups.End()) {
        *infos = out.Get();
        INTERFACE_ADDREF(*infos);
        return NOERROR;
    }
    *infos = NULL;
    return E_RUNTIME_EXCEPTION;
}

ECode CCapsuleManagerService::GetPermissionGroupInfo(
    /* [in] */ String name,
    /* [in] */ Int32 flags,
    /* [out] */ IPermissionGroupInfo** info)
{
    VALIDATE_STRING_NOT_NULL(name);
    VALIDATE_NOT_NULL(info);

    Mutex::Autolock lock(mCapsulesLock);

    HashMap<String, CapsuleParser::PermissionGroup*>::Iterator itor
                                                = mPermissionGroups.Find(name);
    return CapsuleParser::GeneratePermissionGroupInfo(itor->mSecond, flags, info);
}

ECode CCapsuleManagerService::GetAllPermissionGroups(
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);
    *infos = NULL;

    Mutex::Autolock lock(mCapsulesLock);

    AutoPtr<IObjectContainer> out;
    FAIL_RETURN( CObjectContainer::New((IObjectContainer**)&out) );

    HashMap<String, CapsuleParser::PermissionGroup*>::Iterator itor;
    for (itor = mPermissionGroups.Begin();
        itor != mPermissionGroups.End(); ++itor) {
        AutoPtr<IPermissionGroupInfo> g;
        FAIL_RETURN( CapsuleParser::GeneratePermissionGroupInfo(
            itor->mSecond, flags, (IPermissionGroupInfo**)&g) );
        out->Add(g.Get());
    }

    *infos = out.Get();
    (*infos)->AddRef();

    return NOERROR;
}

ECode CCapsuleManagerService::GenerateApplicationInfoFromSettingsLP(
    /* [in] */ String capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IApplicationInfo** info)
{
    VALIDATE_STRING_NOT_NULL(capsuleName);
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    HashMap<String, CapsuleSetting*>::Iterator itor
                            = mSettings->mCapsules.Find(capsuleName);
    CapsuleSetting* cs = itor->mSecond;

    if(cs != NULL) {
        if(cs->mCap == NULL) {
            AutoPtr<ICapsuleInfo> capInfo;

            FAIL_RETURN( GenerateCapsuleInfoFromSettingsLP(
                capsuleName, flags, (ICapsuleInfo**)&capInfo) );
            if(capInfo != NULL) {
                *info = ((CCapsuleInfo*)capInfo.Get())->mApplicationInfo.Get();
                if (*info) (*info)->AddRef();
                return NOERROR;
            }
            return E_FAIL;
        }
        return CapsuleParser
            ::GenerateApplicationInfo(cs->mCap, flags, (CApplicationInfo**)info);
    }

    return E_FAIL;
}

ECode CCapsuleManagerService::GenerateCapsuleInfoFromSettingsLP(
    /* [in] */ String capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ ICapsuleInfo** capInfo)
{
    VALIDATE_STRING_NOT_NULL(capsuleName);
    VALIDATE_NOT_NULL(capInfo);
    *capInfo = NULL;

    HashMap<String, CapsuleSetting*>::Iterator itor
                                = mSettings->mCapsules.Find(capsuleName);
    CapsuleSetting* cs = itor->mSecond;
    if(cs != NULL) {
        if(cs->mCap == NULL) {
            cs->mCap = new CapsuleParser::Capsule(capsuleName);
            cs->mCap->mApplicationInfo->mCapsuleName = String::Duplicate(capsuleName);
            cs->mCap->mApplicationInfo->mFlags = cs->mCapFlags;
            cs->mCap->mApplicationInfo->mPublicSourceDir = String::Duplicate(cs->mResourcePathString);
            cs->mCap->mApplicationInfo->mSourceDir = String::Duplicate(cs->mCodePathString);
            AutoPtr<IFile> path;
            FAIL_RETURN( GetDataPathForCapsule(*cs->mCap, (IFile**)&path) );
            path->GetPath(&cs->mCap->mApplicationInfo->mDataDir);
            cs->mCap->mApplicationInfo->mNativeLibraryDir = cs->mNativeLibraryPathString;
            cs->mCap->mSetEnabled = cs->mEnabled;
        }
        return GenerateCapsuleInfo(cs->mCap, flags, capInfo);
    }
    return E_FAIL;
}

ECode CCapsuleManagerService::GetApplicationInfo(
    /* [in] */ String capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IApplicationInfo** appInfo)
{
    VALIDATE_STRING_NOT_NULL(capsuleName);
    VALIDATE_NOT_NULL(appInfo);
    *appInfo = NULL;

    Mutex::Autolock lock(mCapsulesLock);

    HashMap<String, CapsuleParser::Capsule*>::Iterator itor
                                                = mCapsules.Find(capsuleName);
    CapsuleParser::Capsule* c = itor->mSecond;

    if (Config::LOGV) {
        Slogger::V(TAG, StringBuffer("getApplicationInfo ") + capsuleName + ": " + c);
    }

    if (c != NULL) {
        AutoPtr<CApplicationInfo> cinfo;
        // Note: isEnabledLP() does not apply here - always return info
        FAIL_RETURN( CapsuleParser::GenerateApplicationInfo(c, flags,
                (CApplicationInfo**)&cinfo) );

        *appInfo = (IApplicationInfo*)(CApplicationInfo*)cinfo;
        (*appInfo)->AddRef();
        return NOERROR;
    }

    if (capsuleName.Equals("elastos") ||
        capsuleName.Equals("system")) {
        *appInfo = (IApplicationInfo*)mElastosApplication;
        (*appInfo)->AddRef();
        return NOERROR;
    }

    if((flags & CapsuleManager_GET_UNINSTALLED_CAPSULES) != 0) {
        return GenerateApplicationInfoFromSettingsLP(capsuleName, flags, appInfo);
    }

    return E_FAIL;
}

ECode CCapsuleManagerService::GetActivityInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IActivityInfo** ai)
{
    if (ai == NULL) return E_INVALID_ARGUMENT;

    Mutex::Autolock lock(mCapsulesLock);
    CapsuleParser::Activity* a = (mActivities->mActivities)[component];
    if (Config::LOGV) {
        Slogger::V(TAG, StringBuffer("getActivityInfo ") + component + ": " + a);
    }
    if (a != NULL && mSettings->IsEnabledLP(
            (IComponentInfo*)(IActivityInfo*)a->mInfo.Get(), flags)) {
        AutoPtr<CActivityInfo> info;
        CapsuleParser::GenerateActivityInfo(a, flags, (CActivityInfo**)&info);
        *ai = (IActivityInfo*)(CActivityInfo*)info;
        INTERFACE_ADDREF(*ai);
        return NOERROR;
    }
    Boolean isEqual = FALSE;
    mResolveComponentName->Equals(component, &isEqual);
    if (isEqual) {
        *ai = (IActivityInfo*)mResolveActivity.Get();
        INTERFACE_ADDREF(*ai);
        return NOERROR;
    }

    *ai = NULL;
    return E_DOES_NOT_EXIST;
}

ECode CCapsuleManagerService::GetReceiverInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IActivityInfo** ai)
{
    if (ai == NULL) return E_INVALID_ARGUMENT;

    Mutex::Autolock lock(mCapsulesLock);
    CapsuleParser::Activity* a = (mReceivers->mActivities)[component];
    if (Config::LOGV) {
        Slogger::V(TAG, StringBuffer("getReceiverInfo ") + component + ": " + a);
    }

    if (a != NULL /* && mSettings.isEnabledLP(a.info, flags) */) {
        AutoPtr<CActivityInfo> info;
        ECode ec = CapsuleParser::GenerateActivityInfo(a, flags, (CActivityInfo**)&info);
        *ai = (IActivityInfo*)(CActivityInfo*)info;
        return ec;
    }
    *ai = NULL;
    return E_DOES_NOT_EXIST;
}

ECode CCapsuleManagerService::GetServiceInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IServiceInfo** si)
{
    if (si == NULL) return E_INVALID_ARGUMENT;

    Mutex::Autolock lock(mCapsulesLock);
    CapsuleParser::Service* s = (mServices->mServices)[component];
    if (Config::LOGV) {
        Slogger::V(TAG, StringBuffer("getServiceInfo ") + component + ": " + s);
    }

    if (s != NULL /*&& mSettings.isEnabledLP(s.info, flags)*/) {
        AutoPtr<CServiceInfo> info;
        ECode ec = CapsuleParser::GenerateServiceInfo(s, flags, (CServiceInfo**)&info);
        *si = (IServiceInfo*)(CServiceInfo*)info;
        return ec;
    }
    *si = NULL;
    return E_DOES_NOT_EXIST;
}

ECode CCapsuleManagerService::GetContentProviderInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IContentProviderInfo** info)
{
    VALIDATE_NOT_NULL(component);
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    Mutex::Autolock lock(mCapsulesLock);

    AutoString name;
    component->GetClassName(&name);

    HashMap<String, CapsuleParser::ContentProvider*>::Iterator itor
                                        = mProvidersByComponent.Find(name);
    CapsuleParser::ContentProvider* p = itor->mSecond;

    if (Config::LOGV) {
        Logger::V(TAG, StringBuffer("getProviderInfo ") + component + ": " + p);
    }

    if (p != NULL && mSettings->IsEnabledLP(p->mInfo, flags)) {
        return CapsuleParser::GenerateContentProviderInfo(p, flags, info);
    }

    return E_FAIL;
}

ECode CCapsuleManagerService::CheckPermission(
    /* [in] */ String permName,
    /* [in] */ String capName,
    /* [out] */ Int32* perm)
{
    VALIDATE_STRING_NOT_NULL(permName);
    VALIDATE_STRING_NOT_NULL(capName);
    VALIDATE_NOT_NULL(perm);

    Mutex::Autolock lock(mCapsulesLock);

    HashMap<String, CapsuleParser::Capsule*>::Iterator itor = mCapsules.Find(capName);
    CapsuleParser::Capsule* c = itor->mSecond;

    if (c != NULL && c->mExtras != NULL) {
        CapsuleSetting* cs = (CapsuleSetting*)c->mExtras;
        if (cs->mSharedUser != NULL) {
            HashSet<String>::Iterator t
                            = cs->mSharedUser->mGrantedPermissions.Find(permName);
            if (t != cs->mSharedUser->mGrantedPermissions.End()) {
                *perm = CapsuleManager_PERMISSION_GRANTED;
                return NOERROR;
            }
        } else {
            HashSet<String>::Iterator t
                            = cs->mGrantedPermissions.Find(permName);
            if (t != cs->mGrantedPermissions.End()) {
                *perm = CapsuleManager_PERMISSION_GRANTED;
                return NOERROR;
            }
        }
    }

    *perm = CapsuleManager_PERMISSION_DENIED;
    return NOERROR;
}

ECode CCapsuleManagerService::CheckUidPermission(
    /* [in] */ String permName,
    /* [in] */ Int32 uid,
    /* [out] */ Int32* perm)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::AddPermission(
    /* [in] */ IPermissionInfo* info,
    /* [out] */ Boolean* isAdded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::RemovePermission(
    /* [in] */ String name)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::IsProtectedBroadcast(
    /* [in] */ String actionName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mCapsulesLock);
    HashSet<String>::Iterator it = mProtectedBroadcasts.Find(actionName);
    *result = it != mProtectedBroadcasts.End();
    return NOERROR;
}

ECode CCapsuleManagerService::CheckSignatures(
    /* [in] */ String cap1,
    /* [in] */ String cap2,
    /* [out] */ Int32* sig)
{
    VALIDATE_NOT_NULL(sig);

    Mutex::Autolock Lock(mCapsulesLock);
    HashMap<String, CapsuleParser::Capsule*>::Iterator p1 = mCapsules.Find(cap1);
    HashMap<String, CapsuleParser::Capsule*>::Iterator p2 = mCapsules.Find(cap2);
    if (p1 == mCapsules.End() || p1->mSecond->mExtras == NULL
        || p2 == mCapsules.End() || p2->mSecond->mExtras == NULL) {
        *sig = CapsuleManager_SIGNATURE_UNKNOWN_CAPSULE;
        return NOERROR;
    }
    return CheckSignaturesLP(p1->mSecond->mSignatures, p2->mSecond->mSignatures, sig);
}

ECode CCapsuleManagerService::CheckUidSignatures(
    /* [in] */ Int32 uid1,
    /* [in] */ Int32 uid2,
    /* [out] */ Int32* sig)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::CheckSignaturesLP(
    /* [in] */ List<Signature*>& s1,
    /* [in] */ List<Signature*>& s2,
    /* [out] */ Int32* sig)
{
    if (s1.IsEmpty()) {
        return s2.IsEmpty()
                ? CapsuleManager_SIGNATURE_NEITHER_SIGNED
                : CapsuleManager_SIGNATURE_FIRST_NOT_SIGNED;
    }
    if (s2.IsEmpty()) {
        return CapsuleManager_SIGNATURE_SECOND_NOT_SIGNED;
    }

    Boolean isContained = TRUE;
    List<Signature*>::Iterator it1;
    List<Signature*>::Iterator it2;
    for (it1 = s1.Begin(); it1 != s1.End(); it1++) {
        Boolean is = FALSE;
        for (it2 = s2.Begin(); it2 != s2.End(); it2++) {
            if ((*it1)->Equals(*it2)) {
                is = TRUE;
                break;
            }
        }
        if (!is) {
            isContained = FALSE;
            break;
        }
    }

    // Make sure s2 contains all signatures in s1.
    return isContained
        ? CapsuleManager_SIGNATURE_MATCH
        : CapsuleManager_SIGNATURE_NO_MATCH;
}

ECode CCapsuleManagerService::GetCapsulesForUid(
    /* [in] */ Int32 uid,
    /* [out, callee] */ ArrayOf<String>** capsules)
{
    VALIDATE_NOT_NULL(capsules);

    Mutex::Autolock Lock(mCapsulesLock);
    GrantedPermissions* obj = mSettings->GetUserIdLP(uid);
//	    if (dynamic_cast<SharedUserSetting*>(obj)) {
    if (obj) {
        SharedUserSetting* sus = (SharedUserSetting*)obj;
        const Int32 N = sus->mCapsules.GetSize();
        ArrayOf<String>* res = ArrayOf<String>::Alloc(N);
        assert(res);
        Int32 i = 0;
        HashMap<String, CapsuleParser::Capsule*>::Iterator it;
        for (it = mCapsules.Begin(); it != mCapsules.End(); it++) {
            (*res)[i++] = String::Duplicate(it->mFirst);
        }
        *capsules = res;
        return NOERROR;
//	    } else if (obj instanceof CapsuleSetting) {
//	        CapsuleSetting ps = (CapsuleSetting)obj;
//	        return new String[] { ps->mName };
    }
    *capsules = NULL;
    return E_RUNTIME_EXCEPTION;
}

ECode CCapsuleManagerService::GetNameForUid(
    /* [in] */ Int32 uid,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    Mutex::Autolock Lock(mCapsulesLock);
    GrantedPermissions* obj = mSettings->GetUserIdLP(uid);
//	    if (obj instanceof SharedUserSetting) {
    if (obj) {
        SharedUserSetting* sus = (SharedUserSetting*)obj;
        *name = String::Duplicate(StringBuffer(sus->mName) + ":" + sus->mUserId);
//	    } else if (obj instanceof PackageSetting) {
//	        PackageSetting ps = (PackageSetting)obj;
//	        return ps.name;
    }
    *name = NULL;
    return E_RUNTIME_EXCEPTION;
}

ECode CCapsuleManagerService::GetUidForSharedUser(
    /* [in] */ String sharedUserName,
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid);

    if (sharedUserName.IsNull()) {
        return -1;
    }

    Mutex::Autolock Lock(mCapsulesLock);
    SharedUserSetting* suid = mSettings->GetSharedUserLP(sharedUserName, 0, FALSE);
    if (suid == NULL) {
        *uid = -1;
        return E_RUNTIME_EXCEPTION;
    }
    *uid = suid->mUserId;
    return NOERROR;
}

ECode CCapsuleManagerService::ResolveIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ Int32 flags,
    /* [out] */ IResolveInfo** resolveInfo)
{
    if (resolveInfo == NULL) return E_INVALID_ARGUMENT;

    List<AutoPtr<CResolveInfo>* >* query =
            QueryIntentActivities(intent, resolvedType, flags);
    return ChooseBestActivity(intent, resolvedType, flags, query, resolveInfo);
}

ECode CCapsuleManagerService::QueryIntentActivities(
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(intent);

    AutoPtr<IComponentName> comp;
    intent->GetComponent((IComponentName**)&comp);
    if (comp != NULL) {
        AutoPtr<IObjectContainer> list;
        FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&list));
        AutoPtr<IActivityInfo> ai;
        GetActivityInfo(comp, flags, (IActivityInfo**)&ai);
        if (ai != NULL) {
            AutoPtr<CResolveInfo> ri;
            FAIL_RETURN(CResolveInfo::NewByFriend((CResolveInfo**)&ri));
            ri->mActivityInfo = (CActivityInfo*)ai.Get();
            list->Add((IResolveInfo*)ri.Get());
        }
        *infos = list.Get();
        INTERFACE_ADDREF(*infos);
        return NOERROR;
    }

    {
        Mutex::Autolock Lock(mCapsulesLock);
        String capName;
        intent->GetCapsule(&capName);
        if (capName.IsNull()) {
            List<AutoPtr<CResolveInfo>*>* items =
                mActivities->QueryIntent(intent, resolvedType, flags);
            if (!items) return E_RUNTIME_EXCEPTION;

            AutoPtr<IObjectContainer> list;
            FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&list));
            List<AutoPtr<CResolveInfo>*>::Iterator it;
            for (it = items->Begin(); it != items->End(); it++) {
                list->Add((IResolveInfo*)*it);
            }
            delete items;
            *infos = list.Get();
            INTERFACE_ADDREF(*infos);
            return NOERROR;
        }
        HashMap<String, CapsuleParser::Capsule*>::Iterator cap = mCapsules.Find(capName);
        if (cap != mCapsules.End()) {
            List<AutoPtr<CResolveInfo>*>* items =
                mActivities->QueryIntentForCapsule(
                    intent, resolvedType, flags, &cap->mSecond->mActivities);
            if (!items) return E_RUNTIME_EXCEPTION;

            AutoPtr<IObjectContainer> list;
            FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&list));
            List<AutoPtr<CResolveInfo>*>::Iterator it;
            for (it = items->Begin(); it != items->End(); it++) {
                list->Add((IResolveInfo*)*it);
            }
            delete items;
            *infos = list.Get();
            INTERFACE_ADDREF(*infos);
            return NOERROR;
        }
    }

    *infos = NULL;
    return E_RUNTIME_EXCEPTION;
}

ECode CCapsuleManagerService::QueryIntentActivityOptions(
    /* [in] */ IComponentName* caller,
    /* [in] */ const ArrayOf<IIntent*>& specifics,
    /* [in] */ const ArrayOf<String>& specificTypes,
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(intent);
    VALIDATE_NOT_NULL(infos);

    String resultsAction;
    intent->GetAction(&resultsAction);

    List<AutoPtr<CResolveInfo>*>* results = QueryIntentActivities(
        intent, resolvedType, flags | CapsuleManager_GET_RESOLVED_FILTER);
    if (Config::LOGV) Logger::V(TAG, StringBuffer("Query ") + intent + ": " + results);

    assert(results);

    Int32 specificsPos = 0;

    // todo: note that the algorithm used here is O(N^2).  This
    // isn't a problem in our current environment, but if we start running
    // into situations where we have more than 5 or 10 matches then this
    // should probably be changed to something smarter...

    // First we go through and resolve each of the specific items
    // that were supplied, taking care of removing any corresponding
    // duplicate items in the generic resolve list.
    if (specifics.GetPayload() != NULL) {
        for (Int32 i = 0; i < specifics.GetLength(); i++) {
            AutoPtr<IIntent> sintent = (IIntent*)specifics[i];
            if (sintent == NULL) {
                continue;
            }

            if (Config::LOGV) {
                Logger::V(TAG, StringBuffer("Specific #") + i + ": " + sintent);
            }

            AutoString action;
            sintent->GetAction(&action);
            if (!resultsAction.IsNull() && resultsAction.Equals(action)) {
                // If this action was explicitly requested, then don't
                // remove things that have it.
                String::Free(action);
                action = String(NULL);
            }
            AutoPtr<IComponentName> comp;
            sintent->GetComponent((IComponentName**)&comp);
            AutoPtr<IResolveInfo> ri;
            AutoPtr<IActivityInfo> ai;
            if (comp == NULL) {
                ResolveIntent(sintent,
                    specificTypes.GetPayload()!= NULL ? specificTypes[i] : NULL,
                    flags, (IResolveInfo**)&ri);
                if (ri == NULL) {
                    continue;
                }
                if (ri.Get() == (IResolveInfo*)mResolveInfo.Get()) {
                    // ACK!  Must do something better with this.
                }
                ai = (IActivityInfo*)((CResolveInfo*)ri.Get())->mActivityInfo.Get();
                FAIL_RETURN(CComponentName::New(
                    ((CActivityInfo*)ai.Get())->mApplicationInfo->mCapsuleName,
                    ((CActivityInfo*)ai.Get())->mName, (IComponentName**)&comp));
            } else {
                GetActivityInfo(comp, flags, (IActivityInfo**)&ai);
                if (ai == NULL) {
                    continue;
                }
            }

            // Look for any generic query activities that are duplicates
            // of this specific one, and remove them from the results.
            if (Config::LOGV) {
                Logger::V(TAG, StringBuffer("Specific #") + i + ": " + ai);
            }

            Int32 k = specificsPos;
            List<AutoPtr<CResolveInfo>*>::Iterator it;
            for (it = results->Begin(); it != results->End();) {
                if (k-- > 0) {
                    it++;
                    continue;
                }

                AutoPtr<CResolveInfo> sri = **it;
                AutoString clsName, capName;
                comp->GetClassName(&clsName);
                comp->GetCapsuleName(&capName);
                if ((sri->mActivityInfo->mName.Equals(clsName)
                    && sri->mActivityInfo->mApplicationInfo->mCapsuleName.Equals(capName))
                    || (!action.IsNull() && sri->mFilter->MatchAction(action))) {
                    List<AutoPtr<CResolveInfo>*>::Iterator sit = it++;
                    results->Erase(sit);
                    if (Config::LOGV) Logger::V(
                        TAG, StringBuffer("Removing duplicate item from ")
                             + " due to specific " + specificsPos);
                    if (ri == NULL) {
                        ri = (IResolveInfo*)sri.Get();
                    }
                }
                else {
                    it++;
                }
            }

            // Add this specific item to its proper place.
            if (ri == NULL) {
                CResolveInfo::New((IResolveInfo**)&ri);
                ((CResolveInfo*)ri.Get())->mActivityInfo = (CActivityInfo*)ai.Get();
            }

            k = specificsPos;
            it = results->Begin();
            while (k-- > 0) { it++; }
            AutoPtr<CResolveInfo>* a = new AutoPtr<CResolveInfo>((CResolveInfo*)ri.Get());
            results->Insert(it, a);
            ((CResolveInfo*)ri.Get())->mSpecificIndex = i;
            specificsPos++;
        }
    }

    // Now we go through the remaining generic results and remove any
    // duplicate actions that are found here.
    Int32 k = specificsPos;
    List<AutoPtr<CResolveInfo>*>::Iterator it;
    for (it = results->Begin(); it != results->End();) {
        if (k-- > 0) {
            it++;
            continue;
        }

        AutoPtr<CResolveInfo> rii = **it;
        if (rii->mFilter == NULL) {
            it++;
            continue;
        }

        // Iterate over all of the actions of this result's intent
        // filter...  typically this should be just one.
//	        final Iterator<String> it = rii->mFilter->ActionsIterator();
//	        if (it == NULL) {
//	            continue;
//	        }
//	        while (it->HasNext()) {
//	            final String action = it->Next();
//	            if (resultsAction != NULL && resultsAction->Equals(action)) {
//	                // If this action was explicitly requested, then don't
//	                // remove things that have it.
//	                continue;
//	            }
//	            for (int j=i+1; j<N; j++) {
//	                final ResolveInfo rij = results->Get(j);
//	                if (rij->mFilter != NULL && rij->mFilter->HasAction(action)) {
//	                    results->Remove(j);
//	                    if (Config::LOGV) Logger::V(
//	                        TAG, "Removing duplicate item from " + j
//	                        + " due to action " + action + " at " + i);
//	                    j--;
//	                    N--;
//	                }
//	            }
//	        }

        // If the caller didn't request filter information, drop it now
        // so we don't have to marshall/unmarshall it.
        if ((flags & CapsuleManager_GET_RESOLVED_FILTER) == 0) {
            rii->mFilter = NULL;
        }
        it++;
    }

    // Filter out the caller activity if so requested.
    if (caller != NULL) {
        List<AutoPtr<CResolveInfo>*>::Iterator it;
        for (it = results->Begin(); it != results->End();) {
            AutoPtr<CActivityInfo> ainfo = (**it)->mActivityInfo;
            AutoString capName, clsName;
            caller->GetCapsuleName(&capName);
            caller->GetClassName(&clsName);
            if (capName.Equals(ainfo->mApplicationInfo->mCapsuleName)
                && clsName.Equals(ainfo->mName)) {
                List<AutoPtr<CResolveInfo>*>::Iterator sit = it++;
                results->Erase(sit);
                break;
            }
            else {
                it++;
            }
        }
    }

    // If the caller didn't request filter information,
    // drop them now so we don't have to
    // marshall/unmarshall it.
    if ((flags & CapsuleManager_GET_RESOLVED_FILTER) == 0) {
        List<AutoPtr<CResolveInfo>*>::Iterator it;
        for (it = results->Begin(); it != results->End(); it++) {
            (**it)->mFilter = NULL;
        }
    }

    if (Config::LOGV) {
        Logger::V(TAG, StringBuffer("Result: ") + results);
    }

    AutoPtr<IObjectContainer> container;
    FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&container));

//	    List<AutoPtr<CResolveInfo>*>::Iterator it;
    for (it = results->Begin(); it != results->End(); it++) {
        container->Add(((IResolveInfo*)(**it).Get()));
    }

    *infos = container.Get();
    INTERFACE_ADDREF(*infos);
    return NOERROR;
}

List<AutoPtr<CResolveInfo>*>*
CCapsuleManagerService::QueryIntentActivities(
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ int flags)
{
    AutoPtr<IComponentName> comp;
    intent->GetComponent((IComponentName**)&comp);
    if (comp != NULL) {
        List<AutoPtr<CResolveInfo>*>* list = new List<AutoPtr<CResolveInfo>*>();
        AutoPtr<IActivityInfo> ai;
        GetActivityInfo(comp, flags, (IActivityInfo**)&ai);
        if (ai != NULL) {
            AutoPtr<CResolveInfo>* ri = new AutoPtr<CResolveInfo>();
            CResolveInfo::NewByFriend((CResolveInfo**)ri);
            (*ri)->mActivityInfo = (CActivityInfo*)(IActivityInfo*)ai;
            list->PushBack(ri);
        }
        return list;
    }

    Mutex::Autolock lock(mCapsulesLock);

    AutoString capName;
    intent->GetCapsule(&capName);
    if (capName.IsNull()) {
        return mActivities->QueryIntent(intent,
                resolvedType, flags);
    }
    CapsuleParser::Capsule* cap = NULL;
    HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(capName);
    if (it != mCapsules.End()) {
        cap = it->mSecond;
    }
    if (cap != NULL) {
        return mActivities->QueryIntentForCapsule(intent,
                resolvedType, flags, &(cap->mActivities));
    }

    return NULL;
}

ECode CCapsuleManagerService::ChooseBestActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ List<AutoPtr<CResolveInfo>*>* query,
    /* [out] */ IResolveInfo** resolveInfo)
{
    if (query != NULL) {
        Int32 size = query->GetSize();
        if (size == 1) {
            *resolveInfo = (IResolveInfo*)(CResolveInfo*)(*(*(query->Begin())));
            return NOERROR;
        } else if (size > 1) {
            // If there is more than one activity with the same priority,
            // then let the user decide between them.
            AutoPtr<CResolveInfo>* r0 = *(query->Begin());
            AutoPtr<CResolveInfo>* r1 = *(++query->Begin());
//            if (false) {
//                System.out.println(r0.activityInfo.name +
//                                   "=" + r0.priority + " vs " +
//                                   r1.activityInfo.name +
//                                   "=" + r1.priority);
//            }
            // If the first activity has a higher priority, or a different
            // default, then it is always desireable to pick it.
            if ((*r0)->mPriority != (*r1)->mPriority
                    || (*r0)->mPreferredOrder != (*r1)->mPreferredOrder
                    || (*r0)->mIsDefault != (*r1)->mIsDefault) {
                *resolveInfo = (IResolveInfo*)(CResolveInfo*)(*(query->Begin()));
                return NOERROR;
            }
            // If we have saved a preference for a preferred activity for
            // this Intent, use that.
            CResolveInfo* ri = FindPreferredActivity(intent, resolvedType,
                    flags, query, (*r0)->mPriority);
            if (ri != NULL) {
                *resolveInfo = (IResolveInfo*)ri;
                return NOERROR;
            }
            *resolveInfo = (IResolveInfo*)(CResolveInfo*)mResolveInfo;
            return NOERROR;
        }
    }
    *resolveInfo = NULL;
    return NOERROR;
}

CResolveInfo* CCapsuleManagerService::FindPreferredActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ List<AutoPtr<CResolveInfo>*>* query,
    /* [in] */ Int32 priority)
{
    if (!intent || !query) return NULL;

    Mutex::Autolock lock(mCapsulesLock);

    if (DEBUG_PREFERRED) intent->AddFlags(Intent_FLAG_DEBUG_LOG_RESOLUTION);
//	    List<PreferredActivity*>* prefs =
//	        mSettings->mPreferredActivities->QueryIntent(intent, resolvedType,
//	            (flags & CapsuleManager_MATCH_DEFAULT_ONLY) != 0);
    List<PreferredActivity*>* prefs = NULL;
    if (prefs != NULL && prefs->GetSize() > 0) {
        // First figure out how good the original match set is.
        // We will only allow preferred activities that came
        // from the same match quality.
        Int32 match = 0;
        if (DEBUG_PREFERRED) Slogger::V(TAG, "Figuring out best match...");
        List<AutoPtr<CResolveInfo>*>::Iterator qit;
        for (qit = query->Begin(); qit != query->End(); qit++) {
            AutoPtr<CResolveInfo> ri = **qit;
            if (DEBUG_PREFERRED) {
                Slogger::V(TAG, StringBuffer("Match for ")
                    + ri->mActivityInfo.Get()
                    + ": 0x" + /*Integer::ToHexString(match)*/ match);
            }
            if (ri->mMatch > match) match = ri->mMatch;
        }
        if (DEBUG_PREFERRED) {
            Slogger::V(TAG, StringBuffer("Best match: 0x")
                + /*Integer::ToHexString(match)*/ match);
        }
        match &= IntentFilter_MATCH_CATEGORY_MASK;
        List<PreferredActivity*>::Iterator pit;
        for (pit = prefs->Begin(); pit != prefs->End(); pit++) {
            PreferredActivity* pa = *pit; // prefs->Get(i);
            if (pa->mMatch != match) {
                continue;
            }
            AutoPtr<IActivityInfo> ai;
            GetActivityInfo(pa->mActivity.Get(), flags, (IActivityInfo**)&ai);
            if (DEBUG_PREFERRED) {
                Slogger::V(TAG, "Got preferred activity:");
                if (ai != NULL) {
//	                    ai->Dump(new LogPrinter(Log::VERBOSE, TAG), "  ");
                } else {
                    Logger::V(TAG, "  NULL");
                }
            }
            if (ai != NULL) {
                List<AutoPtr<CResolveInfo>*>::Iterator it;
                for (it = query->Begin(); it != query->End(); it++) {
                    AutoPtr<CResolveInfo> ri = **it;
                    if (!ri->mActivityInfo->mApplicationInfo->mCapsuleName
                        .Equals(((CActivityInfo*)ai.Get())->mApplicationInfo->mCapsuleName)) {
                        continue;
                    }
                    if (!ri->mActivityInfo->mName.Equals(((CActivityInfo*)ai.Get())->mName)) {
                        continue;
                    }

                    // Okay we found a previously set preferred app.
                    // If the result set is different from when this
                    // was created, we need to clear it and re-ask the
                    // user their preference.
                    if (!pa->SameSet(query, priority)) {
                        Slogger::I(TAG, StringBuffer("Result set changed, dropping preferred activity for ")
                            + intent + " type " + resolvedType);
//	                        mSettings->mPreferredActivities->RemoveFilter(pa);
                        return NULL;
                    }

                    // Yay!
                    return ri.Get();
                }
            }
        }
    }
    return NULL;
}

ECode CCapsuleManagerService::QueryIntentReceivers(
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** receivers)
{
    if (receivers == NULL) return E_INVALID_ARGUMENT;

    AutoPtr<IComponentName> comp;
    intent->GetComponent((IComponentName**)&comp);
    if (comp != NULL) {
        CObjectContainer::New(receivers);
        AutoPtr<IActivityInfo> ai;
        GetReceiverInfo(comp, flags, (IActivityInfo**)&ai);
        if (ai != NULL) {
            AutoPtr<CResolveInfo> ri;
            CResolveInfo::NewByFriend((CResolveInfo**)&ri);
            ri->mActivityInfo = (CActivityInfo*)(IActivityInfo*)ai;
            (*receivers)->Add((IResolveInfo*)(CResolveInfo*)ri);
        }
        return NOERROR;
    }

    Mutex::Autolock lock(mCapsulesLock);

    String capName;
    intent->GetCapsule(&capName);
    if (capName.IsNull()) {
        CObjectContainer::New(receivers);
        List<AutoPtr<CResolveInfo>*>* list = mReceivers->QueryIntent(
                intent, resolvedType, flags);
        List<AutoPtr<CResolveInfo>*>::Iterator it1 = list->Begin();
        List<AutoPtr<CResolveInfo>*>::Iterator it2 = list->End();
        for(; it1 != it2; ++it1) {
//            AutoPtr<CResolveInfo>* rinfo = *it1;
//            IResolveInfo* iinfo = (IResolveInfo*)(*rinfo)->Probe(EIID_IResolveInfo);
            (*receivers)->Add((IResolveInfo*)**it1);
        }

        return NOERROR;
    }
    CapsuleParser::Capsule* cap = NULL;
    HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(capName);
    if (it != mCapsules.End()) {
        cap = it->mSecond;
    }
    if (cap != NULL) {
        CObjectContainer::New(receivers);
        List<AutoPtr<CResolveInfo>*>* list = mReceivers->QueryIntentForCapsule(
                intent, resolvedType, flags, &(cap->mReceivers));
        List<AutoPtr<CResolveInfo>*>::Iterator it1 = list->Begin();
        List<AutoPtr<CResolveInfo>*>::Iterator it2 = list->End();
        for(; it1 != it2; ++it1) {
            (*receivers)->Add((IResolveInfo*)**it1);
        }

        return NOERROR;
    }

    *receivers = NULL;
    return NOERROR;
}

ECode CCapsuleManagerService::ResolveService(
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ Int32 flags,
    /* [out] */ IResolveInfo** resolveInfo)
{
    if (resolveInfo == NULL) return E_INVALID_ARGUMENT;
    *resolveInfo = NULL;

    AutoPtr<IObjectContainer> query;
    QueryIntentServices(intent, resolvedType, flags, (IObjectContainer**)&query);
    if (query != NULL) {
        Int32 count;
        if (query->GetObjectCount(&count), count >= 1) {
            // If there is more than one service with the same priority,
            // just arbitrarily pick the first one.
            AutoPtr<IObjectEnumerator> it;
            query->GetObjectEnumerator((IObjectEnumerator**)&it);
            Boolean succeeded;
            it->MoveNext(&succeeded);
            if (succeeded) it->Current((IInterface**)resolveInfo);
        }
    }
    return NOERROR;
}

ECode CCapsuleManagerService::QueryIntentServices(
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** services)
{
    AutoPtr<IComponentName> comp;
    intent->GetComponent((IComponentName**)&comp);
    if (comp != NULL) {
        CObjectContainer::New(services);
        AutoPtr<IServiceInfo> si;
        GetServiceInfo(comp, flags, (IServiceInfo**)&si);
        if (si != NULL) {
            AutoPtr<CResolveInfo> ri;
            CResolveInfo::NewByFriend((CResolveInfo**)&ri);
            ri->mServiceInfo = (CServiceInfo*)(IServiceInfo*)si;
            (*services)->Add((IResolveInfo*)(CResolveInfo*)ri);
        }
        return NOERROR;
    }

    Mutex::Autolock lock(mCapsulesLock);

    String capName;
    intent->GetCapsule(&capName);
    if (capName.IsNull()) {
        CObjectContainer::New(services);
        List<AutoPtr<CResolveInfo>*>* list = mServices->QueryIntent(
                intent, resolvedType, flags);
        List<AutoPtr<CResolveInfo>*>::Iterator it1 = list->Begin();
        List<AutoPtr<CResolveInfo>*>::Iterator it2 = list->End();
        for(; it1 != it2; ++it1) {
//            AutoPtr<CResolveInfo>* rinfo = *it1;
//            IResolveInfo* iinfo = (IResolveInfo*)(*rinfo)->Probe(EIID_IResolveInfo);
            (*services)->Add((IResolveInfo*)**it1);
        }
        return NOERROR;
    }
    CapsuleParser::Capsule* cap = NULL;
    HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(capName);
    if (it != mCapsules.End()) {
        cap = it->mSecond;
    }
    if (cap != NULL) {
        CObjectContainer::New(services);
        List<AutoPtr<CResolveInfo>*>* list = mServices->QueryIntentForCapsule(
                intent, resolvedType, flags, &(cap->mServices));
        List<AutoPtr<CResolveInfo>*>::Iterator it1 = list->Begin();
        List<AutoPtr<CResolveInfo>*>::Iterator it2 = list->End();
        for(; it1 != it2; ++it1) {
            (*services)->Add((IResolveInfo*)**it1);
        }
        return NOERROR;
    }
    *services = NULL;
    return NOERROR;
}

String CCapsuleManagerService::FixProcessName(
    /* [in] */ String defProcessName,
    /* [in] */ String processName,
    /* [in] */ Int32 uid)
{
    if (processName.IsNull()) {
        return defProcessName;
    }
    return processName;
}

Int32 CCapsuleManagerService::GetContinuationPoint(
    /* [in] */ const ArrayOf<String>& keys,
    /* [in] */ const String& key)
{
//	    final int index;
//	    if (key == null) {
//	        index = 0;
//	    } else {
//	        final int insertPoint = Arrays.binarySearch(keys, key);
//	        if (insertPoint < 0) {
//	            index = -insertPoint;
//	        } else {
//	            index = insertPoint + 1;
//	        }
//	    }
//	    return index;
    return -1;
}

ECode CCapsuleManagerService::GetInstalledCapsules(
    /* [in] */ Int32 flags,
    /* [in] */ String lastRead,
    /* [out] */ IParceledListSlice** slice)
{
    VALIDATE_NOT_NULL(slice);

    AutoPtr<IParcelable> capInfo;
    FAIL_RETURN(CCapsuleInfo::New((IParcelable**)&capInfo));
    AutoPtr<IParceledListSlice> list;
    FAIL_RETURN(CParceledListSlice::New(capInfo.Get(), (IParceledListSlice**)&list));
    Boolean listUninstalled = (flags & CapsuleManager_GET_UNINSTALLED_CAPSULES) != 0;
    AutoStringArray keys;

    {
        Mutex::Autolock lock(mCapsulesLock);
        if (listUninstalled) {
            keys = ArrayOf<String>::Alloc(mSettings->mCapsules.GetSize());
            HashMap<String, CapsuleSetting*>::Iterator it;
            Int32 i = 0;
            for (it = mSettings->mCapsules.Begin();
                 it != mSettings->mCapsules.End();
                 it++, i++) {
                (*keys)[i] = String::Duplicate(it->mFirst);
            }
        } else {
            keys = ArrayOf<String>::Alloc(mCapsules.GetSize());
            HashMap<String, CapsuleParser::Capsule*>::Iterator it;
            Int32 i = 0;
            for (it = mCapsules.Begin(); it != mCapsules.End(); it++, i++) {
                (*keys)[i] = String::Duplicate(it->mFirst);
            }
        }

//	        Arrays::Sort(keys);
        Int32 i = GetContinuationPoint(*keys, lastRead);
        const Int32 N = keys->GetLength();

        while (i < N) {
            String capsuleName = (*keys)[i++];

            AutoPtr<ICapsuleInfo> ci;
            if (listUninstalled) {
                HashMap<String, CapsuleSetting*>::Iterator it
                                    = mSettings->mCapsules.Find(capsuleName);
                CapsuleSetting* cs = it->mSecond;
                if (cs != NULL) {
                    GenerateCapsuleInfoFromSettingsLP(cs->mName, flags, (ICapsuleInfo**)&ci);
                }
            } else {
                HashMap<String, CapsuleParser::Capsule*>::Iterator it
                                            = mCapsules.Find(capsuleName);
                CapsuleParser::Capsule* c = it->mSecond;
                if (c != NULL) {
                    GenerateCapsuleInfo(c, flags, (ICapsuleInfo**)&ci);
                }
            }

            if (ci != NULL) {
                Boolean isFull = FALSE;
                list->Append((IParcelable*)ci->Probe(EIID_IParcelable), &isFull);
                if (!isFull) break;
            }
        }

        if (i == N) {
            list->SetLastSlice(TRUE);
        }
    }

    *slice = list.Get();
    INTERFACE_ADDREF(*slice);
    return NOERROR;
}

ECode CCapsuleManagerService::GetInstalledApplications(
    /* [in] */ Int32 flags,
    /* [in] */ String lastRead,
    /* [out] */ IParceledListSlice** slice)
{
    VALIDATE_NOT_NULL(slice);

    AutoPtr<IParcelable> appInfo;
    FAIL_RETURN(CApplicationInfo::New((IParcelable**)&appInfo));
    AutoPtr<IParceledListSlice> list;
    FAIL_RETURN(CParceledListSlice::New(appInfo.Get(), (IParceledListSlice**)&list));
    const Boolean listUninstalled = (flags & CapsuleManager_GET_UNINSTALLED_CAPSULES) != 0;
    AutoStringArray keys;

    {
        Mutex::Autolock lock(mCapsulesLock);

        if (listUninstalled) {
            keys = ArrayOf<String>::Alloc(mSettings->mCapsules.GetSize());
            HashMap<String, CapsuleSetting*>::Iterator it;
            Int32 i = 0;
            for (it = mSettings->mCapsules.Begin();
                 it != mSettings->mCapsules.End();
                 it++, i++) {
                (*keys)[i] = String::Duplicate(it->mFirst);
            }
        } else {
            keys = ArrayOf<String>::Alloc(mCapsules.GetSize());
            HashMap<String, CapsuleParser::Capsule*>::Iterator it;
            Int32 i = 0;
            for (it = mCapsules.Begin(); it != mCapsules.End(); it++, i++) {
                (*keys)[i] = String::Duplicate(it->mFirst);
            }
        }

//	        Arrays::Sort(keys);
        Int32 i = GetContinuationPoint(*keys, lastRead);
        const Int32 N = keys->GetLength();

        while (i < N) {
            String capsuleName = (*keys)[i++];

            AutoPtr<IApplicationInfo> ai;
            if (listUninstalled) {
                HashMap<String, CapsuleSetting*>::Iterator it
                                    = mSettings->mCapsules.Find(capsuleName);
                CapsuleSetting* cs = it->mSecond;
                if (cs != NULL) {
                    GenerateApplicationInfoFromSettingsLP(cs->mName, flags, (IApplicationInfo**)&ai);
                }
            } else {
                HashMap<String, CapsuleParser::Capsule*>::Iterator it
                                            = mCapsules.Find(capsuleName);
                CapsuleParser::Capsule* c = it->mSecond;
                AutoPtr<CApplicationInfo> cai;
                if (c != NULL) {
                    CapsuleParser::GenerateApplicationInfo(c, flags, (CApplicationInfo**)&cai);
                    ai = (IApplicationInfo*)cai.Get();
                }
            }

            if (ai != NULL) {
                Boolean isFull = FALSE;
                list->Append((IParcelable*)ai->Probe(EIID_IParcelable), &isFull);
                if (!isFull) break;
            }
        }

        if (i == N) {
            list->SetLastSlice(TRUE);
        }
    }

    *slice = list.Get();
    INTERFACE_ADDREF(*slice);
    return NOERROR;
}

ECode CCapsuleManagerService::GetPersistentApplications(
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);

    AutoPtr<IObjectContainer> finalList;

    {
        Mutex::Autolock lock(mCapsulesLock);

        HashMap<String, CapsuleParser::Capsule*>::Iterator it;
        for (it = mCapsules.Begin(); it != mCapsules.End(); it++) {
            CapsuleParser::Capsule* c = it->mSecond;
            if (c->mApplicationInfo != NULL
                && (c->mApplicationInfo->mFlags & CApplicationInfo::FLAG_PERSISTENT) != 0
                && (!mSafeMode || IsSystemApp(c))) {
                AutoPtr<CApplicationInfo> cinfo;
                FAIL_RETURN(CapsuleParser::GenerateApplicationInfo(c, flags, (CApplicationInfo**)&cinfo));
                finalList->Add((IApplicationInfo*)cinfo.Get());
            }
        }
    }

    *infos = finalList.Get();
    INTERFACE_ADDREF(*infos);
    return NOERROR;
}

ECode CCapsuleManagerService::ResolveContentProvider(
    /* [in] */ String name,
    /* [in] */ Int32 flags,
    /* [out] */ IContentProviderInfo** info)
{
    VALIDATE_NOT_NULL(info);

    Mutex::Autolock lock(mCapsulesLock);
    HashMap<String, CapsuleParser::ContentProvider*>::Iterator it = mProviders.Find(name);
    CapsuleParser::ContentProvider* provider = it->mSecond;
    if (provider != NULL
        && mSettings->IsEnabledLP(provider->mInfo, flags)
        && (!mSafeMode || (provider->mInfo->mApplicationInfo->mFlags
            & CApplicationInfo::FLAG_SYSTEM) != 0)) {
        return CapsuleParser::GenerateContentProviderInfo(provider, flags, info);
    }

    *info = NULL;
    return E_DOES_NOT_EXIST;
}

ECode CCapsuleManagerService::QuerySyncProviders(
    /* [in] */ IObjectContainer* outNames,
    /* [in] */ IObjectContainer* outInfo)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::QueryContentProviders(
    /* [in] */ String processName,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** providers)
{
    VALIDATE_NOT_NULL(providers);

    *providers = NULL;
    {
        Mutex::Autolock lock(mCapsulesLock);
        HashMap<String, CapsuleParser::ContentProvider*>::Iterator it = \
                mProvidersByComponent.Begin();
        for (; it != mProvidersByComponent.End(); ++it) {
            CapsuleParser::ContentProvider* p = it->mSecond;
    //        if (p.info.authority != null
    //                    && (processName == null ||
    //                            (p.info.processName.equals(processName)
    //                                    && p.info.applicationInfo.uid == uid))
    //                    && mSettings.isEnabledLP(p.info, flags)
    //                    && (!mSafeMode || (p.info.applicationInfo.flags
    //                            &ApplicationInfo.FLAG_SYSTEM) != 0))
    //        {}
            if (processName.IsNull() || (!p->mInfo->mProcessName.Compare(processName))) {
                if (*providers == NULL) {
                    CObjectContainer::New(providers);
                }
                AutoPtr<IContentProviderInfo> provider;
                CapsuleParser::GenerateContentProviderInfo(p, flags,
                        (IContentProviderInfo**)&provider);
                (*providers)->Add((IInterface*)(IContentProviderInfo*)provider);
            }
        }
    }

//    if (finalList != null) {
//        Collections.sort(finalList, mProviderInitOrderSorter);
//    }

    if (*providers == NULL) return E_DOES_NOT_EXIST;
    return NOERROR;
}

ECode CCapsuleManagerService::GetInstrumentationInfo(
    /* [in] */ IComponentName* name,
    /* [in] */ Int32 flags,
    /* [out] */ IInstrumentationInfo** instInfo)
{
    if (instInfo == NULL) return E_INVALID_ARGUMENT;

    Mutex::Autolock lock(mCapsulesLock);
    CapsuleParser::Instrumentation* i = mInstrumentation[name];
    AutoPtr<CInstrumentationInfo> info;
    ECode ec = CapsuleParser::GenerateInstrumentationInfo(i, flags, (CInstrumentationInfo**)&info);
    *instInfo = (IInstrumentationInfo*)(CInstrumentationInfo*)info;
    return ec;
}

ECode CCapsuleManagerService::QueryInstrumentation(
    /* [in] */ String targetCapsule,
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);

    AutoPtr<IObjectContainer> finalList;
    CObjectContainer::New((IObjectContainer**)&finalList);

    {
        HashMap<AutoPtr<IComponentName>, CapsuleParser::Instrumentation*>::Iterator it;
        for (it = mInstrumentation.Begin(); it != mInstrumentation.End(); it++) {
            CapsuleParser::Instrumentation* i = it->mSecond;
            if (targetCapsule.IsNull() || targetCapsule.Equals(i->mInfo->mTargetCapsule)) {
                AutoPtr<CInstrumentationInfo> info;
                FAIL_RETURN(
                    CapsuleParser::GenerateInstrumentationInfo(
                        i, flags, (CInstrumentationInfo**)&info));
                finalList->Add((IInstrumentationInfo*)info.Get());
            }
        }
    }

    *infos = finalList.Get();
    INTERFACE_ADDREF(*infos);
    return NOERROR;
}

ECode CCapsuleManagerService::ScanDirLI(
    /* [in] */ IFile* dir,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 scanMode,
    /* [in] */ Int64 currentTime)
{
    assert(dir);

    ECode ec = NOERROR;
    AutoStringArray files;
    dir->List((ArrayOf<String>**)&files);
    if (files == NULL) {
        AutoString des;
        dir->GetDescription(&des);
        Logger::D(TAG, StringBuffer("No files in app dir ") + des);
        return NOERROR;
    }

    if (FALSE) {
        AutoString des;
        dir->GetDescription(&des);
        Logger::D(TAG, StringBuffer("Scanning app dir ") + des);
    }

    Int32 i;
    for (i = 0; i < files->GetLength(); i++) {
        AutoPtr<IFile> file;

        ec = CFile::New(dir, (*files)[i], (IFile**)&file);
        if (FAILED(ec)) return ec;

        if (!IsCapsuleFilename((*files)[i])) {
            // Ignore entries which are not apk's
            continue;
        }
        CapsuleParser::Capsule* cap = ScanCapsuleLI(file.Get(),
                flags | CapsuleParser::PARSE_MUST_BE_APK, scanMode, currentTime);
        // Don't mess around with apps in system partition.
        if (cap == NULL && (flags & CapsuleParser::PARSE_IS_SYSTEM) == 0 &&
                mLastScanError == CapsuleManager::INSTALL_FAILED_INVALID_APK) {
            // Delete the apk
            AutoString des;
            file->GetDescription(&des);
            Slogger::D(TAG, StringBuffer("Cleaning up failed install of ") + des);

            Boolean isDeleted = FALSE;
            file->Delete(&isDeleted);
        }
    }

    return NOERROR;
}

ECode CCapsuleManagerService::KillApplication(
    /* [in] */ String capName,
    /* [in] */ Int32 uid)
{
    // Request the ActivityManager to kill the process(only for existing packages)
    // so that we do not end up in a confused state while the user is still using the older
    // version of the application while the new one gets installed.
    AutoPtr<IServiceManager> serviceManager;
    AutoPtr<IActivityManager> am;
    Elastos::GetServiceManager((IServiceManager**)&serviceManager);
    assert(serviceManager != NULL);
    serviceManager->GetService("ActivityManagerService", (IInterface**)&am);
    if (am != NULL) {
        am->KillApplicationWithUid(capName, uid);
    }
    return NOERROR;
}

CapsuleParser::Capsule* CCapsuleManagerService::ScanCapsuleLI(
    /* [in] */ IFile* scanFile,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanMode,
    /* [in] */ Int64 currentTime)
{
    assert(scanFile);

    mLastScanError = CapsuleManager::INSTALL_SUCCEEDED;
    AutoString scanPath;
    scanFile->GetPath(&scanPath);
    parseFlags |= mDefParseFlags;
    CapsuleParser pp(scanPath);
//	    pp.setSeparateProcesses(mSeparateProcesses);
    CapsuleParser::Capsule* cap = pp.ParseCapsule(scanFile,
            scanPath, mMetrics.Get(), parseFlags);
    if (cap == NULL) {
        mLastScanError = pp.GetParseError();
        return NULL;
    }
//	    PackageSetting ps = null;
//	    PackageSetting updatedPkg;
//	    synchronized (mPackages) {
//	        // Look to see if we already know about this package.
//	        String oldName = mSettings.mRenamedPackages.get(pkg.packageName);
//	        if (pkg.mOriginalPackages != null && pkg.mOriginalPackages.contains(oldName)) {
//	            // This package has been renamed to its original name.  Let's
//	            // use that.
//	            ps = mSettings.peekPackageLP(oldName);
//	        }
//	        // If there was no original package, see one for the real package name.
//	        if (ps == null) {
//	            ps = mSettings.peekPackageLP(pkg.packageName);
//	        }
//	        // Check to see if this package could be hiding/updating a system
//	        // package.  Must look for it either under the original or real
//	        // package name depending on our state.
//	        updatedPkg = mSettings.mDisabledSysPackages.get(
//	                ps != null ? ps.name : pkg.packageName);
//	    }
//	    // First check if this is a system package that may involve an update
//	    if (updatedPkg != null && (parseFlags&PackageParser.PARSE_IS_SYSTEM) != 0) {
//	        if (ps != null && !ps.codePath.equals(scanFile)) {
//	            // The path has changed from what was last scanned...  check the
//	            // version of the new path against what we have stored to determine
//	            // what to do.
//	            if (pkg.mVersionCode < ps.versionCode) {
//	                // The system package has been updated and the code path does not match
//	                // Ignore entry. Skip it.
//	                Log.i(TAG, "Package " + ps.name + " at " + scanFile
//	                        + " ignored: updated version " + ps.versionCode
//	                        + " better than this " + pkg.mVersionCode);
//	                mLastScanError = PackageManager.INSTALL_FAILED_DUPLICATE_PACKAGE;
//	                return null;
//	            } else {
//	                // The current app on the system partion is better than
//	                // what we have updated to on the data partition; switch
//	                // back to the system partition version.
//	                // At this point, its safely assumed that package installation for
//	                // apps in system partition will go through. If not there won't be a working
//	                // version of the app
//	                synchronized (mPackages) {
//	                    // Just remove the loaded entries from package lists.
//	                    mPackages.remove(ps.name);
//	                }
//	                Slog.w(TAG, "Package " + ps.name + " at " + scanFile
//	                        + "reverting from " + ps.codePathString
//	                        + ": new version " + pkg.mVersionCode
//	                        + " better than installed " + ps.versionCode);
//	                InstallArgs args = new FileInstallArgs(ps.codePathString,
//	                        ps.resourcePathString, ps.nativeLibraryPathString);
//	                args.cleanUpResourcesLI();
//	                mSettings.enableSystemPackageLP(ps.name);
//	            }
//	        }
//	    }
//	    if (updatedPkg != null) {
//	        // An updated system app will not have the PARSE_IS_SYSTEM flag set initially
//	        parseFlags |= PackageParser.PARSE_IS_SYSTEM;
//	    }
//	    // Verify certificates against what was last scanned
//	    if (!collectCertificatesLI(pp, ps, pkg, scanFile, parseFlags)) {
//	        Slog.w(TAG, "Failed verifying certificates for package:" + pkg.packageName);
//	        return null;
//	    }
//	    // The apk is forward locked (not public) if its code and resources
//	    // are kept in different files.
//	    // TODO grab this value from PackageSettings
//	    if (ps != null && !ps.codePath.equals(ps.resourcePath)) {
//	        parseFlags |= PackageParser.PARSE_FORWARD_LOCK;
//	    }

    AutoString codePath;
    AutoString resPath;
//	    if ((parseFlags & CapsuleParser::PARSE_FORWARD_LOCK) != 0) {
//	        if (ps != null && ps.resourcePathString != null) {
//	            resPath = ps.resourcePathString;
//	        } else {
//	            // Should not happen at all. Just log an error.
//	            Slog.e(TAG, "Resource path not set for pkg : " + pkg.packageName);
//	        }
//	    } else {
        resPath = String::Duplicate(cap->mScanPath);
//	    }
    codePath = String::Duplicate(cap->mScanPath);
    // Set application objects path explicitly.
    SetApplicationInfoPaths(cap, codePath, resPath);
    // Note that we invoke the following method only if we are about to unpack an application
    return ScanCapsuleLI(cap, parseFlags, scanMode | SCAN_UPDATE_SIGNATURE, currentTime);
}

void CCapsuleManagerService::SetApplicationInfoPaths(
    /* [in] */ CapsuleParser::Capsule* cap,
    /* [in] */ String destCodePath,
    /* [in] */ String destResPath)
{
    assert(cap);

    String::Free(cap->mScanPath);
    cap->mScanPath = String::Duplicate(destCodePath);
    String::Free(cap->mPath);
    cap->mPath = String::Duplicate(destCodePath);
    String::Free(cap->mApplicationInfo->mSourceDir);
    cap->mApplicationInfo->mSourceDir = String::Duplicate(destCodePath);
    String::Free(cap->mApplicationInfo->mPublicSourceDir);
    cap->mApplicationInfo->mPublicSourceDir = String::Duplicate(destResPath);
}

Boolean CCapsuleManagerService::UseEncryptedFilesystemForCapsule(
    /* [in] */ CapsuleParser::Capsule* cap)
{
    return Environment::IsEncryptedFilesystemEnabled() && cap != NULL;
        ((cap->mApplicationInfo->mFlags & CApplicationInfo::FLAG_NEVER_ENCRYPT) == 0);
}

ECode CCapsuleManagerService::GetDataPathForCapsule(
    /* [in] */ const CapsuleParser::Capsule& cap,
    /* [out] */ IFile** path)
{
    assert(path);

    Boolean useEncryptedFSDir = UseEncryptedFilesystemForCapsule((CapsuleParser::Capsule*)&cap);
    if (useEncryptedFSDir) {
        FAIL_RETURN(CFile::New(mSecureAppDataDir.Get(), cap.mCapsuleName, path));
    } else {
        FAIL_RETURN(CFile::New(mAppDataDir.Get(), cap.mCapsuleName, path));
    }

    return NOERROR;
}

CapsuleParser::Capsule* CCapsuleManagerService::ScanCapsuleLI(
    /* [in] */ CapsuleParser::Capsule* cap,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanMode,
    /* [in] */ Int64 currentTime)
{
    assert(cap);

    AutoPtr<IFile> scanFile;
    ECode ec = CFile::New(cap->mScanPath, (IFile**)&scanFile);
    if (scanFile == NULL || cap->mApplicationInfo->mSourceDir.IsNull() ||
            cap->mApplicationInfo->mPublicSourceDir.IsNull()) {
        // Bail out. The resource and code paths haven't been set.
        Slogger::W(TAG, " Code and resource paths haven't been set correctly");
        mLastScanError = CapsuleManager::INSTALL_FAILED_INVALID_APK;
        return NULL;
    }
    mScanningPath = scanFile;
    if (cap->mCapsuleName.IsNull()) {
        mLastScanError = CapsuleManager::INSTALL_PARSE_FAILED_BAD_PACKAGE_NAME;
        return NULL;
    }

    if ((parseFlags & CapsuleParser::PARSE_IS_SYSTEM) != 0) {
        cap->mApplicationInfo->mFlags |= CApplicationInfo::FLAG_SYSTEM;
    }

    if (cap->mCapsuleName.Equals("elastos")) {
        {
            Mutex::Autolock lock(mCapsulesLock);
            if (mElastosApplication != NULL) {
                Slogger::W(TAG, "*************************************************");
                Slogger::W(TAG, "Core android package being redefined.  Skipping.");
                Slogger::W(TAG, StringBuffer(" file=") + (Int32)mScanningPath.Get());
                Slogger::W(TAG, "*************************************************");
                mLastScanError = CapsuleManager::INSTALL_FAILED_DUPLICATE_PACKAGE;
                return NULL;
            }

            // Set up information for our fall-back user intent resolution
            // activity.
            mPlatformCapsule = cap;
            cap->mVersionCode = mSdkVersion;
            mElastosApplication = cap->mApplicationInfo;
            mResolveActivity->mApplicationInfo = mElastosApplication;
//	            mResolveActivity->mName = ResolverActivity.class.getName();
            mResolveActivity->mCapsuleName = mElastosApplication->mCapsuleName;
            mResolveActivity->mProcessName = mElastosApplication->mProcessName;
            mResolveActivity->mLaunchMode = CActivityInfo::LAUNCH_MULTIPLE;
            mResolveActivity->mFlags = CActivityInfo::FLAG_EXCLUDE_FROM_RECENTS;
//	            mResolveActivity->mTheme = com.android.internal.R.style.Theme_Dialog_Alert;
            mResolveActivity->mExported = TRUE;
            mResolveActivity->mEnabled = TRUE;
            mResolveInfo->mActivityInfo = mResolveActivity;
            mResolveInfo->mPriority = 0;
            mResolveInfo->mPreferredOrder = 0;
            mResolveInfo->mMatch = 0;
//	            mResolveComponentName = new ComponentName(
//	                    mAndroidApplication.packageName, mResolveActivity.name);
            if (mResolveComponentName != NULL) mResolveComponentName->Release();
            ec = CComponentName::New(
                mElastosApplication->mCapsuleName, mResolveActivity->mName,
                (IComponentName**)&mResolveComponentName);
            if (FAILED(ec)) return NULL;
        }
    }

//	    if ((parseFlags&PackageParser.PARSE_CHATTY) != 0 && Config.LOGD) Log.d(
//	            TAG, "Scanning package " + pkg.packageName);
    HashMap<String, CapsuleParser::Capsule*>::Iterator nIter
            = mCapsules.Find(cap->mCapsuleName);
    HashMap<String, String>::Iterator sIter = mSharedLibraries.Find(cap->mCapsuleName);
    if (nIter != mCapsules.End() || sIter != mSharedLibraries.End()) {
        Slogger::W(TAG, StringBuffer("Application package ") + cap->mCapsuleName
                + " already installed.  Skipping duplicate.");
        mLastScanError = CapsuleManager::INSTALL_FAILED_DUPLICATE_PACKAGE;
        return NULL;
    }

    // Initialize package source and resource directories
    AutoPtr<IFile> destCodeFile;
    ec = CFile::New(cap->mApplicationInfo->mSourceDir, (IFile**)&destCodeFile);
    if (FAILED(ec)) return NULL;
    AutoPtr<IFile> destResourceFile;
    ec = CFile::New(cap->mApplicationInfo->mPublicSourceDir, (IFile**)&destResourceFile);
    if (FAILED(ec)) return NULL;

//	    SharedUserSetting suid = null;
//	    PackageSetting pkgSetting = null;

//	    if (!isSystemApp(pkg)) {
//	        // Only system apps can use these features.
//	        pkg.mOriginalPackages = null;
//	        pkg.mRealPackage = null;
//	        pkg.mAdoptPermissions = null;
//	    }

    {
        Mutex::Autolock lock(mCapsulesLock);
        // Check all shared libraries and map to their actual file path.
//	        if (pkg.usesLibraries != null || pkg.usesOptionalLibraries != null) {
//	            if (mTmpSharedLibraries == null ||
//	                    mTmpSharedLibraries.length < mSharedLibraries.size()) {
//	                mTmpSharedLibraries = new String[mSharedLibraries.size()];
//	            }
//	            int num = 0;
//	            int N = pkg.usesLibraries != null ? pkg.usesLibraries.size() : 0;
//	            for (int i=0; i<N; i++) {
//	                String file = mSharedLibraries.get(pkg.usesLibraries.get(i));
//	                if (file == null) {
//	                    Slog.e(TAG, "Package " + pkg.packageName
//	                            + " requires unavailable shared library "
//	                            + pkg.usesLibraries.get(i) + "; failing!");
//	                    mLastScanError = PackageManager.INSTALL_FAILED_MISSING_SHARED_LIBRARY;
//	                    return null;
//	                }
//	                mTmpSharedLibraries[num] = file;
//	                num++;
//	            }
//	            N = pkg.usesOptionalLibraries != null ? pkg.usesOptionalLibraries.size() : 0;
//	            for (int i=0; i<N; i++) {
//	                String file = mSharedLibraries.get(pkg.usesOptionalLibraries.get(i));
//	                if (file == null) {
//	                    Slog.w(TAG, "Package " + pkg.packageName
//	                            + " desires unavailable shared library "
//	                            + pkg.usesOptionalLibraries.get(i) + "; ignoring!");
//	                } else {
//	                    mTmpSharedLibraries[num] = file;
//	                    num++;
//	                }
//	            }
//	            if (num > 0) {
//	                pkg.usesLibraryFiles = new String[num];
//	                System.arraycopy(mTmpSharedLibraries, 0,
//	                        pkg.usesLibraryFiles, 0, num);
//	            }
//	        }

//	        if (pkg.mSharedUserId != null) {
//	            suid = mSettings.getSharedUserLP(pkg.mSharedUserId,
//	                    pkg.applicationInfo.flags, true);
//	            if (suid == null) {
//	                Slog.w(TAG, "Creating application package " + pkg.packageName
//	                        + " for shared user failed");
//	                mLastScanError = PackageManager.INSTALL_FAILED_INSUFFICIENT_STORAGE;
//	                return null;
//	            }
//	            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0 && Config.LOGD) {
//	                Log.d(TAG, "Shared UserID " + pkg.mSharedUserId + " (uid="
//	                        + suid.userId + "): packages=" + suid.packages);
//	            }
//	        }

//	        if (false) {
//	            if (pkg.mOriginalPackages != null) {
//	                Log.w(TAG, "WAITING FOR DEBUGGER");
//	                Debug.waitForDebugger();
//	                Log.i(TAG, "Package " + pkg.packageName + " from original packages"
//	                        + pkg.mOriginalPackages);
//	            }
//	        }

//	        // Check if we are renaming from an original package name.
//	        PackageSetting origPackage = null;
//	        String realName = null;
//	        if (pkg.mOriginalPackages != null) {
//	            // This package may need to be renamed to a previously
//	            // installed name.  Let's check on that...
//	            String renamed = mSettings.mRenamedPackages.get(pkg.mRealPackage);
//	            if (pkg.mOriginalPackages.contains(renamed)) {
//	                // This package had originally been installed as the
//	                // original name, and we have already taken care of
//	                // transitioning to the new one.  Just update the new
//	                // one to continue using the old name.
//	                realName = pkg.mRealPackage;
//	                if (!pkg.packageName.equals(renamed)) {
//	                    // Callers into this function may have already taken
//	                    // care of renaming the package; only do it here if
//	                    // it is not already done.
//	                    pkg.setPackageName(renamed);
//	                }
//
//	            } else {
//	                for (int i=pkg.mOriginalPackages.size()-1; i>=0; i--) {
//	                    if ((origPackage=mSettings.peekPackageLP(
//	                            pkg.mOriginalPackages.get(i))) != null) {
//	                        // We do have the package already installed under its
//	                        // original name...  should we use it?
//	                        if (!verifyPackageUpdate(origPackage, pkg)) {
//	                            // New package is not compatible with original.
//	                            origPackage = null;
//	                            continue;
//	                        } else if (origPackage.sharedUser != null) {
//	                            // Make sure uid is compatible between packages.
//	                            if (!origPackage.sharedUser.name.equals(pkg.mSharedUserId)) {
//	                                Slog.w(TAG, "Unable to migrate data from " + origPackage.name
//	                                        + " to " + pkg.packageName + ": old uid "
//	                                        + origPackage.sharedUser.name
//	                                        + " differs from " + pkg.mSharedUserId);
//	                                origPackage = null;
//	                                continue;
//	                            }
//	                        } else {
//	                            if (DEBUG_UPGRADE) Log.v(TAG, "Renaming new package "
//	                                    + pkg.packageName + " to old name " + origPackage.name);
//	                        }
//	                        break;
//	                    }
//	                }
//	            }
//	        }

//	        if (mTransferedPackages.contains(pkg.packageName)) {
//	            Slog.w(TAG, "Package " + pkg.packageName
//	                    + " was transferred to another, but its .apk remains");
//	        }

//	        // Just create the setting, don't add it yet. For already existing packages
//	        // the PkgSetting exists already and doesn't have to be created.
//	        pkgSetting = mSettings.getPackageLP(pkg, origPackage, realName, suid, destCodeFile,
//	                destResourceFile, pkg.applicationInfo.nativeLibraryDir,
//	                pkg.applicationInfo.flags, true, false);
//	        if (pkgSetting == null) {
//	            Slog.w(TAG, "Creating application package " + pkg.packageName + " failed");
//	            mLastScanError = PackageManager.INSTALL_FAILED_INSUFFICIENT_STORAGE;
//	            return null;
//	        }

//	        if (pkgSetting.origPackage != null) {
//	            // If we are first transitioning from an original package,
//	            // fix up the new package's name now.  We need to do this after
//	            // looking up the package under its new name, so getPackageLP
//	            // can take care of fiddling things correctly.
//	            pkg.setPackageName(origPackage.name);
//
//	            // File a report about this.
//	            String msg = "New package " + pkgSetting.realName
//	                    + " renamed to replace old package " + pkgSetting.name;
//	            reportSettingsProblem(Log.WARN, msg);
//
//	            // Make a note of it.
//	            mTransferedPackages.add(origPackage.name);
//
//	            // No longer need to retain this.
//	            pkgSetting.origPackage = null;
//	        }

//	        if (realName != null) {
//	            // Make a note of it.
//	            mTransferedPackages.add(pkg.packageName);
//	        }

//	        if (mSettings.mDisabledSysPackages.get(pkg.packageName) != null) {
//	            pkg.applicationInfo.flags |= ApplicationInfo.FLAG_UPDATED_SYSTEM_APP;
//	        }

//	        pkg.applicationInfo.uid = pkgSetting.userId;
//	        pkg.mExtras = pkgSetting;

//	        if (!verifySignaturesLP(pkgSetting, pkg)) {
//	            if ((parseFlags&PackageParser.PARSE_IS_SYSTEM_DIR) == 0) {
//	                return null;
//	            }
//	            // The signature has changed, but this package is in the system
//	            // image...  let's recover!
//	            pkgSetting.signatures.mSignatures = pkg.mSignatures;
//	            // However...  if this package is part of a shared user, but it
//	            // doesn't match the signature of the shared user, let's fail.
//	            // What this means is that you can't change the signatures
//	            // associated with an overall shared user, which doesn't seem all
//	            // that unreasonable.
//	            if (pkgSetting.sharedUser != null) {
//	                if (checkSignaturesLP(pkgSetting.sharedUser.signatures.mSignatures,
//	                        pkg.mSignatures) != PackageManager.SIGNATURE_MATCH) {
//	                    Log.w(TAG, "Signature mismatch for shared user : " + pkgSetting.sharedUser);
//	                    mLastScanError = PackageManager.INSTALL_PARSE_FAILED_INCONSISTENT_CERTIFICATES;
//	                    return null;
//	                }
//	            }
//	            // File a report about this.
//	            String msg = "System package " + pkg.packageName
//	                    + " signature changed; retaining data.";
//	            reportSettingsProblem(Log.WARN, msg);
//	        }

//	        // Verify that this new package doesn't have any content providers
//	        // that conflict with existing packages.  Only do this if the
//	        // package isn't already installed, since we don't want to break
//	        // things that are installed.
//	        if ((scanMode&SCAN_NEW_INSTALL) != 0) {
//	            int N = pkg.providers.size();
//	            int i;
//	            for (i=0; i<N; i++) {
//	                PackageParser.Provider p = pkg.providers.get(i);
//	                if (p.info.authority != null) {
//	                    String names[] = p.info.authority.split(";");
//	                    for (int j = 0; j < names.length; j++) {
//	                        if (mProviders.containsKey(names[j])) {
//	                            PackageParser.Provider other = mProviders.get(names[j]);
//	                            Slog.w(TAG, "Can't install because provider name " + names[j] +
//	                                    " (in package " + pkg.applicationInfo.packageName +
//	                                    ") is already used by "
//	                                    + ((other != null && other.getComponentName() != null)
//	                                            ? other.getComponentName().getPackageName() : "?"));
//	                            mLastScanError = PackageManager.INSTALL_FAILED_CONFLICTING_PROVIDER;
//	                            return null;
//	                        }
//	                    }
//	                }
//	            }
//	        }
    }

//	    final String pkgName = pkg.packageName;

//	    if (pkg.mAdoptPermissions != null) {
//	        // This package wants to adopt ownership of permissions from
//	        // another package.
//	        for (int i=pkg.mAdoptPermissions.size()-1; i>=0; i--) {
//	            String origName = pkg.mAdoptPermissions.get(i);
//	            PackageSetting orig = mSettings.peekPackageLP(origName);
//	            if (orig != null) {
//	                if (verifyPackageUpdate(orig, pkg)) {
//	                    Slog.i(TAG, "Adopting permissions from "
//	                            + origName + " to " + pkg.packageName);
//	                    mSettings.transferPermissions(origName, pkg.packageName);
//	                }
//	            }
//	        }
//	    }

//	    final long scanFileTime = scanFile.lastModified();
//	    final boolean forceDex = (scanMode&SCAN_FORCE_DEX) != 0;
//	    final boolean scanFileNewer = forceDex || scanFileTime != pkgSetting.timeStamp;
//	    pkg.applicationInfo.processName = fixProcessName(
//	            pkg.applicationInfo.packageName,
//	            pkg.applicationInfo.processName,
//	            pkg.applicationInfo.uid);

    AutoPtr<IFile> dataPath;
    if (mPlatformCapsule == cap) {
        // The system package is special.
        AutoPtr<IFile> f;
        ASSERT_SUCCEEDED(Environment::GetDataDirectory((IFile**)&f));
        ASSERT_SUCCEEDED(CFile::New(f.Get(), "system", (IFile**)&dataPath));
        String::Free(cap->mApplicationInfo->mDataDir);
        dataPath->GetPath(&cap->mApplicationInfo->mDataDir);
    } else {
        // This is a normal package, need to make its data directory.
//	        boolean useEncryptedFSDir = useEncryptedFilesystemForPackage(pkg);
        GetDataPathForCapsule(*cap, (IFile**)&dataPath);

//	        Boolean uidError = FALSE;

        Boolean isExists = FALSE;
        dataPath->Exists(&isExists);
        if (isExists) {
//	            mOutPermissions[1] = 0;
//	            FileUtils.getPermissions(dataPath.getPath(), mOutPermissions);

//	            // If we have mismatched owners for the data path, we have a
//	            // problem (unless we're running in the simulator.)
//	            if (mOutPermissions[1] != pkg.applicationInfo.uid && Process.supportsProcesses()) {
//	                boolean recovered = false;
//	                if ((parseFlags&PackageParser.PARSE_IS_SYSTEM) != 0) {
//	                    // If this is a system app, we can at least delete its
//	                    // current data so the application will still work.
//	                    if (mInstaller != null) {
//	                        int ret = mInstaller.remove(pkgName, useEncryptedFSDir);
//	                        if (ret >= 0) {
//	                            // Old data gone!
//	                            String msg = "System package " + pkg.packageName
//	                                    + " has changed from uid: "
//	                                    + mOutPermissions[1] + " to "
//	                                    + pkg.applicationInfo.uid + "; old data erased";
//	                            reportSettingsProblem(Log.WARN, msg);
//	                            recovered = true;
//
//	                            // And now re-install the app.
//	                            ret = mInstaller.install(pkgName, useEncryptedFSDir, pkg.applicationInfo.uid,
//	                                    pkg.applicationInfo.uid);
//	                            if (ret == -1) {
//	                                // Ack should not happen!
//	                                msg = "System package " + pkg.packageName
//	                                        + " could not have data directory re-created after delete.";
//	                                reportSettingsProblem(Log.WARN, msg);
//	                                mLastScanError = PackageManager.INSTALL_FAILED_INSUFFICIENT_STORAGE;
//	                                return null;
//	                            }
//	                        }
//	                    }
//	                    if (!recovered) {
//	                        mHasSystemUidErrors = true;
//	                    }
//	                }
//	                if (!recovered) {
//	                    pkg.applicationInfo.dataDir = "/mismatched_uid/settings_"
//	                        + pkg.applicationInfo.uid + "/fs_"
//	                        + mOutPermissions[1];
//	                    pkg.applicationInfo.nativeLibraryDir = pkg.applicationInfo.dataDir;
//	                    String msg = "Package " + pkg.packageName
//	                            + " has mismatched uid: "
//	                            + mOutPermissions[1] + " on disk, "
//	                            + pkg.applicationInfo.uid + " in settings";
//	                    synchronized (mPackages) {
//	                        mSettings.mReadMessages.append(msg);
//	                        mSettings.mReadMessages.append('\n');
//	                        uidError = true;
//	                        if (!pkgSetting.uidError) {
//	                            reportSettingsProblem(Log.ERROR, msg);
//	                        }
//	                    }
//	                }
//	            }
//	            pkg.applicationInfo.dataDir = dataPath.getPath();
        } else {
//	            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0 && Config.LOGV)
//	                Log.v(TAG, "Want this data dir: " + dataPath);
//	            //invoke installer to do the actual installation
//	            if (mInstaller != null) {
//	                int ret = mInstaller.install(pkgName, useEncryptedFSDir, pkg.applicationInfo.uid,
//	                        pkg.applicationInfo.uid);
//	                if(ret < 0) {
//	                    // Error from installer
//	                    mLastScanError = PackageManager.INSTALL_FAILED_INSUFFICIENT_STORAGE;
//	                    return null;
//	                }
//	            } else {
//	                dataPath.mkdirs();
//	                if (dataPath.exists()) {
//	                    FileUtils.setPermissions(
//	                        dataPath.toString(),
//	                        FileUtils.S_IRWXU|FileUtils.S_IRWXG|FileUtils.S_IXOTH,
//	                        pkg.applicationInfo.uid, pkg.applicationInfo.uid);
//	                }
//	            }
//	            if (dataPath.exists()) {
//	                pkg.applicationInfo.dataDir = dataPath.getPath();
//	            } else {
//	                Slog.w(TAG, "Unable to create data directory: " + dataPath);
//	                pkg.applicationInfo.dataDir = null;
//	            }
        }

        /*
         * Set the data dir to the default "/data/data/<package name>/lib"
         * if we got here without anyone telling us different (e.g., apps
         * stored on SD card have their native libraries stored in the ASEC
         * container with the APK).
         *
         * This happens during an upgrade from a package settings file that
         * doesn't have a native library path attribute at all.
         */
//	        if (pkg.applicationInfo.nativeLibraryDir == null && pkg.applicationInfo.dataDir != null) {
//	            if (pkgSetting.nativeLibraryPathString == null) {
//	                final String nativeLibraryPath = new File(dataPath, LIB_DIR_NAME).getPath();
//	                pkg.applicationInfo.nativeLibraryDir = nativeLibraryPath;
//	                pkgSetting.nativeLibraryPathString = nativeLibraryPath;
//	            } else {
//	                pkg.applicationInfo.nativeLibraryDir = pkgSetting.nativeLibraryPathString;
//	            }
//	        }

//	        pkgSetting.uidError = uidError;
    }

//	    // If we're running in the simulator, we don't need to unpack anything.
//	    if (mInstaller != null) {
//	        String path = scanFile.getPath();
//	        /* Note: We don't want to unpack the native binaries for
//	         *        system applications, unless they have been updated
//	         *        (the binaries are already under /system/lib).
//	         *        Also, don't unpack libs for apps on the external card
//	         *        since they should have their libraries in the ASEC
//	         *        container already.
//	         *
//	         *        In other words, we're going to unpack the binaries
//	         *        only for non-system apps and system app upgrades.
//	         */
//	        if (pkg.applicationInfo.nativeLibraryDir != null) {
//	            final File nativeLibraryDir = new File(pkg.applicationInfo.nativeLibraryDir);
//	            final String dataPathString = dataPath.getPath();
//
//	            if (isSystemApp(pkg) && !isUpdatedSystemApp(pkg)) {
//	                /*
//	                 * Upgrading from a previous version of the OS sometimes
//	                 * leaves native libraries in the /data/data/<app>/lib
//	                 * directory for system apps even when they shouldn't be.
//	                 * Recent changes in the JNI library search path
//	                 * necessitates we remove those to match previous behavior.
//	                 */
//	                if (NativeLibraryHelper.removeNativeBinariesFromDirLI(nativeLibraryDir)) {
//	                    Log.i(TAG, "removed obsolete native libraries for system package " + path);
//	                }
//	            } else if (nativeLibraryDir.getParent().equals(dataPathString)) {
//	                /*
//	                 * If this is an internal application or our
//	                 * nativeLibraryPath points to our data directory, unpack
//	                 * the libraries. The native library path pointing to the
//	                 * data directory for an application in an ASEC container
//	                 * can happen for older apps that existed before an OTA to
//	                 * Gingerbread.
//	                 */
//	                Slog.i(TAG, "Unpacking native libraries for " + path);
//	                mInstaller.unlinkNativeLibraryDirectory(dataPathString);
//	                NativeLibraryHelper.copyNativeBinariesLI(scanFile, nativeLibraryDir);
//	            } else {
//	                Slog.i(TAG, "Linking native library dir for " + path);
//	                mInstaller.linkNativeLibraryDirectory(dataPathString,
//	                        pkg.applicationInfo.nativeLibraryDir);
//	            }
//	        }
//	        pkg.mScanPath = path;
//
//	        if ((scanMode&SCAN_NO_DEX) == 0) {
//	            if (performDexOptLI(pkg, forceDex) == DEX_OPT_FAILED) {
//	                mLastScanError = PackageManager.INSTALL_FAILED_DEXOPT;
//	                return null;
//	            }
//	        }
//	    }

//	    if (mFactoryTest && pkg.requestedPermissions.contains(
//	            android.Manifest.permission.FACTORY_TEST)) {
//	        pkg.applicationInfo.flags |= ApplicationInfo.FLAG_FACTORY_TEST;
//	    }

//	    // Request the ActivityManager to kill the process(only for existing packages)
//	    // so that we do not end up in a confused state while the user is still using the older
//	    // version of the application while the new one gets installed.
//	    if ((parseFlags & PackageManager.INSTALL_REPLACE_EXISTING) != 0) {
//	        killApplication(pkg.applicationInfo.packageName,
//	                    pkg.applicationInfo.uid);
//	    }

    {
        Mutex::Autolock lock(mCapsulesLock);
//	        // We don't expect installation to fail beyond this point,
//	        if ((scanMode&SCAN_MONITOR) != 0) {
//	            mAppDirs.put(pkg.mPath, pkg);
//	        }
//	        // Add the new setting to mSettings
//	        mSettings.insertPackageSettingLP(pkgSetting, pkg);
//	        // Add the new setting to mPackages
//	        mPackages.put(pkg.applicationInfo.packageName, pkg);
//	        // Make sure we don't accidentally delete its data.
//	        mSettings.mPackagesToBeCleaned.remove(pkgName);
//
//	        // Take care of first install / last update times.
//	        if (currentTime != 0) {
//	            if (pkgSetting.firstInstallTime == 0) {
//	                pkgSetting.firstInstallTime = pkgSetting.lastUpdateTime = currentTime;
//	            } else if ((scanMode&SCAN_UPDATE_TIME) != 0) {
//	                pkgSetting.lastUpdateTime = currentTime;
//	            }
//	        } else if (pkgSetting.firstInstallTime == 0) {
//	            // We need *something*.  Take time time stamp of the file.
//	            pkgSetting.firstInstallTime = pkgSetting.lastUpdateTime = scanFileTime;
//	        } else if ((parseFlags&PackageParser.PARSE_IS_SYSTEM_DIR) != 0) {
//	            if (scanFileTime != pkgSetting.timeStamp) {
//	                // A package on the system image has changed; consider this
//	                // to be an update.
//	                pkgSetting.lastUpdateTime = scanFileTime;
//	            }
//	        }
//
        Int32 N = cap->mContentProviders.GetSize();
//	        StringBuilder r = null;
        StringBuffer* r = NULL;
        Int32 i = 0;
//	        for (i=0; i<N; i++) {
//	            PackageParser.Provider p = pkg.providers.get(i);
//	            p.info.processName = fixProcessName(pkg.applicationInfo.processName,
//	                    p.info.processName, pkg.applicationInfo.uid);
//	            mProvidersByComponent.put(new ComponentName(p.info.packageName,
//	                    p.info.name), p);
//	            p.syncable = p.info.isSyncable;
//	            if (p.info.authority != null) {
//	                String names[] = p.info.authority.split(";");
//	                p.info.authority = null;
//	                for (int j = 0; j < names.length; j++) {
//	                    if (j == 1 && p.syncable) {
//	                        // We only want the first authority for a provider to possibly be
//	                        // syncable, so if we already added this provider using a different
//	                        // authority clear the syncable flag. We copy the provider before
//	                        // changing it because the mProviders object contains a reference
//	                        // to a provider that we don't want to change.
//	                        // Only do this for the second authority since the resulting provider
//	                        // object can be the same for all future authorities for this provider.
//	                        p = new PackageParser.Provider(p);
//	                        p.syncable = false;
//	                    }
//	                    if (!mProviders.containsKey(names[j])) {
//	                        mProviders.put(names[j], p);
//	                        if (p.info.authority == null) {
//	                            p.info.authority = names[j];
//	                        } else {
//	                            p.info.authority = p.info.authority + ";" + names[j];
//	                        }
//	                        if ((parseFlags&PackageParser.PARSE_CHATTY) != 0 && Config.LOGD)
//	                            Log.d(TAG, "Registered content provider: " + names[j] +
//	                                    ", className = " + p.info.name +
//	                                    ", isSyncable = " + p.info.isSyncable);
//	                    } else {
//	                        PackageParser.Provider other = mProviders.get(names[j]);
//	                        Slog.w(TAG, "Skipping provider name " + names[j] +
//	                                " (in package " + pkg.applicationInfo.packageName +
//	                                "): name already used by "
//	                                + ((other != null && other.getComponentName() != null)
//	                                        ? other.getComponentName().getPackageName() : "?"));
//	                    }
//	                }
//	            }
//	            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//	                if (r == null) {
//	                    r = new StringBuilder(256);
//	                } else {
//	                    r.append(' ');
//	                }
//	                r.append(p.info.name);
//	            }
//	        }
//	        if (r != null) {
//	            if (Config.LOGD) Log.d(TAG, "  Providers: " + r);
//	        }
//
//	        N = pkg.services.size();
//	        r = null;
//	        for (i=0; i<N; i++) {
//	            PackageParser.Service s = pkg.services.get(i);
//	            s.info.processName = fixProcessName(pkg.applicationInfo.processName,
//	                    s.info.processName, pkg.applicationInfo.uid);
//	            mServices.addService(s);
//	            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//	                if (r == null) {
//	                    r = new StringBuilder(256);
//	                } else {
//	                    r.append(' ');
//	                }
//	                r.append(s.info.name);
//	            }
//	        }
//	        if (r != null) {
//	            if (Config.LOGD) Log.d(TAG, "  Services: " + r);
//	        }
//
//	        N = pkg.receivers.size();
//	        r = null;
//	        for (i=0; i<N; i++) {
//	            PackageParser.Activity a = pkg.receivers.get(i);
//	            a.info.processName = fixProcessName(pkg.applicationInfo.processName,
//	                    a.info.processName, pkg.applicationInfo.uid);
//	            mReceivers.addActivity(a, "receiver");
//	            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//	                if (r == null) {
//	                    r = new StringBuilder(256);
//	                } else {
//	                    r.append(' ');
//	                }
//	                r.append(a.info.name);
//	            }
//	        }
//	        if (r != null) {
//	            if (Config.LOGD) Log.d(TAG, "  Receivers: " + r);
//	        }

        N = cap->mActivities.GetSize();
        delete r;
        r = NULL;
        for (i = 0; i < N; i++) {
            CapsuleParser::Activity* a = cap->mActivities[i];
            String n = FixProcessName(cap->mApplicationInfo->mProcessName,
                    a->mInfo->mProcessName, cap->mApplicationInfo->mUid);
            if ((const char*)n != (const char*)a->mInfo->mProcessName) {
                String::Free(a->mInfo->mProcessName);
                a->mInfo->mProcessName = String::Duplicate(n);
            }
            mActivities->AddActivity(a, "activity");
            if ((parseFlags & CapsuleParser::PARSE_CHATTY) != 0) {
                if (r == NULL) {
                    r = new StringBuffer;
                } else {
                    *r += "";
                }
                *r += a->mInfo->mName;
            }
        }
        if (r != NULL) {
            if (Config::LOGD) Logger::D(TAG, StringBuffer("  Activities: ") + *r);
        }

//	        N = pkg.permissionGroups.size();
        delete r;
        r = NULL;
//	        for (i=0; i<N; i++) {
//	            PackageParser.PermissionGroup pg = pkg.permissionGroups.get(i);
//	            PackageParser.PermissionGroup cur = mPermissionGroups.get(pg.info.name);
//	            if (cur == null) {
//	                mPermissionGroups.put(pg.info.name, pg);
//	                if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//	                    if (r == null) {
//	                        r = new StringBuilder(256);
//	                    } else {
//	                        r.append(' ');
//	                    }
//	                    r.append(pg.info.name);
//	                }
//	            } else {
//	                Slog.w(TAG, "Permission group " + pg.info.name + " from package "
//	                        + pg.info.packageName + " ignored: original from "
//	                        + cur.info.packageName);
//	                if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//	                    if (r == null) {
//	                        r = new StringBuilder(256);
//	                    } else {
//	                        r.append(' ');
//	                    }
//	                    r.append("DUP:");
//	                    r.append(pg.info.name);
//	                }
//	            }
//	        }
//	        if (r != null) {
//	            if (Config.LOGD) Log.d(TAG, "  Permission Groups: " + r);
//	        }
//
//	        N = pkg.permissions.size();
//	        r = null;
//	        for (i=0; i<N; i++) {
//	            PackageParser.Permission p = pkg.permissions.get(i);
//	            HashMap<String, BasePermission> permissionMap =
//	                    p.tree ? mSettings.mPermissionTrees
//	                    : mSettings.mPermissions;
//	            p.group = mPermissionGroups.get(p.info.group);
//	            if (p.info.group == null || p.group != null) {
//	                BasePermission bp = permissionMap.get(p.info.name);
//	                if (bp == null) {
//	                    bp = new BasePermission(p.info.name, p.info.packageName,
//	                            BasePermission.TYPE_NORMAL);
//	                    permissionMap.put(p.info.name, bp);
//	                }
//	                if (bp.perm == null) {
//	                    if (bp.sourcePackage == null
//	                            || bp.sourcePackage.equals(p.info.packageName)) {
//	                        BasePermission tree = findPermissionTreeLP(p.info.name);
//	                        if (tree == null
//	                                || tree.sourcePackage.equals(p.info.packageName)) {
//	                            bp.packageSetting = pkgSetting;
//	                            bp.perm = p;
//	                            bp.uid = pkg.applicationInfo.uid;
//	                            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//	                                if (r == null) {
//	                                    r = new StringBuilder(256);
//	                                } else {
//	                                    r.append(' ');
//	                                }
//	                                r.append(p.info.name);
//	                            }
//	                        } else {
//	                            Slog.w(TAG, "Permission " + p.info.name + " from package "
//	                                    + p.info.packageName + " ignored: base tree "
//	                                    + tree.name + " is from package "
//	                                    + tree.sourcePackage);
//	                        }
//	                    } else {
//	                        Slog.w(TAG, "Permission " + p.info.name + " from package "
//	                                + p.info.packageName + " ignored: original from "
//	                                + bp.sourcePackage);
//	                    }
//	                } else if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//	                    if (r == null) {
//	                        r = new StringBuilder(256);
//	                    } else {
//	                        r.append(' ');
//	                    }
//	                    r.append("DUP:");
//	                    r.append(p.info.name);
//	                }
//	                if (bp.perm == p) {
//	                    bp.protectionLevel = p.info.protectionLevel;
//	                }
//	            } else {
//	                Slog.w(TAG, "Permission " + p.info.name + " from package "
//	                        + p.info.packageName + " ignored: no group "
//	                        + p.group);
//	            }
//	        }
//	        if (r != null) {
//	            if (Config.LOGD) Log.d(TAG, "  Permissions: " + r);
//	        }
//
//	        N = pkg.instrumentation.size();
//	        r = null;
//	        for (i=0; i<N; i++) {
//	            PackageParser.Instrumentation a = pkg.instrumentation.get(i);
//	            a.info.packageName = pkg.applicationInfo.packageName;
//	            a.info.sourceDir = pkg.applicationInfo.sourceDir;
//	            a.info.publicSourceDir = pkg.applicationInfo.publicSourceDir;
//	            a.info.dataDir = pkg.applicationInfo.dataDir;
//	            a.info.nativeLibraryDir = pkg.applicationInfo.nativeLibraryDir;
//	            mInstrumentation.put(a.getComponentName(), a);
//	            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//	                if (r == null) {
//	                    r = new StringBuilder(256);
//	                } else {
//	                    r.append(' ');
//	                }
//	                r.append(a.info.name);
//	            }
//	        }
//	        if (r != null) {
//	            if (Config.LOGD) Log.d(TAG, "  Instrumentation: " + r);
//	        }
//
//	        if (pkg.protectedBroadcasts != null) {
//	            N = pkg.protectedBroadcasts.size();
//	            for (i=0; i<N; i++) {
//	                mProtectedBroadcasts.add(pkg.protectedBroadcasts.get(i));
//	            }
//	        }
//
//	        pkgSetting.setTimeStamp(scanFileTime);
    }

    return cap;
}

ECode CCapsuleManagerService::RemoveCapsuleLI(
    /* [in] */ CapsuleParser::Capsule* cap,
    /* [in] */ Boolean chatty)
{
    VALIDATE_NOT_NULL(cap);

    if (chatty && Config::LOGD) {
        Logger::D(TAG, StringBuffer("Removing capsule ") + cap->mApplicationInfo->mCapsuleName);
    }

    {
        Mutex::Autolock lock(mCapsulesLock);

        ClearCapsulePreferredActivitiesLP(cap->mCapsuleName);

        mCapsules.Erase(cap->mApplicationInfo->mCapsuleName);
        if (!cap->mPath.IsNull()) {
            mAppDirs.Erase(cap->mPath);
        }

        List<CapsuleParser::ContentProvider*>::Iterator providerIt;
        StringBuffer* r = NULL;
        for (providerIt = cap->mContentProviders.Begin();
             providerIt != cap->mContentProviders.End(); providerIt++) {
            CapsuleParser::ContentProvider* p = *providerIt;
            AutoString compName;
            GenerateComponentNameKey(p->mInfo->mCapsuleName, p->mInfo->mName, &compName);
            mProvidersByComponent.Erase(compName);
            if (p->mInfo->mAuthority.IsNull()) {

                /* The is another ContentProvider with this authority when
                 * this app was installed so this authority is NULL,
                 * Ignore it as we don't have to unregister the provider->
                 */
                continue;
            }

            Char8 delim[] = { ';', '\0' };
            StringTokenizer* names = new StringTokenizer(p->mInfo->mAuthority, delim);
            while (names->HasMoreTokens()) {
                String name = names->NextToken();
                HashMap<String, CapsuleParser::ContentProvider*>::Iterator it = mProviders.Find(name);
                if (it->mSecond == p) {
                    mProviders.Erase(name);
                    if (chatty && Config::LOGD) {
                        Logger::D(TAG,
                            StringBuffer("Unregistered content provider: ")
                            + name + ", className = " + p->mInfo->mName +
                            ", isSyncable = " + p->mInfo->mIsSyncable);
                    }
                }
            }
            if (chatty) {
                if (r == NULL) {
                    r = new StringBuffer();
                } else {
                    *r += " ";
                }
                *r += p->mInfo->mName;
            }
        }
        if (r != NULL) {
            if (Config::LOGD) Logger::D(TAG, StringBuffer("  Providers: ") + *r);
            delete r;
        }

        r = NULL;
        List<CapsuleParser::Service*>::Iterator serviceIt;
        for (serviceIt = cap->mServices.Begin();
             serviceIt != cap->mServices.End(); serviceIt++) {
            CapsuleParser::Service* s = *serviceIt;
            mServices->RemoveService(s);
            if (chatty) {
                if (r == NULL) {
                    r = new StringBuffer();
                } else {
                    *r += " ";
                }
                *r += s->mInfo->mName;
            }
        }
        if (r != NULL) {
            if (Config::LOGD) Logger::D(TAG, StringBuffer("  Services: ") + *r);
            delete r;
        }

        r = NULL;
        List<CapsuleParser::Activity*>::Iterator receiverIt;
        for (receiverIt = cap->mReceivers.Begin();
             receiverIt != cap->mReceivers.End(); receiverIt++) {
            CapsuleParser::Activity* a = *receiverIt;
            mReceivers->RemoveActivity(a, "receiver");
            if (chatty) {
                if (r == NULL) {
                    r = new StringBuffer();
                } else {
                    *r += " ";
                }
                *r += a->mInfo->mName;
            }
        }
        if (r != NULL) {
            if (Config::LOGD) Logger::D(TAG, StringBuffer("  Receivers: ") + *r);
            delete r;
        }

        r = NULL;
        List<CapsuleParser::Activity*>::Iterator activityIt;
        for (activityIt = cap->mActivities.Begin();
             activityIt != cap->mActivities.End(); activityIt++) {
            CapsuleParser::Activity* a = *activityIt;
            mActivities->RemoveActivity(a, "activity");
            if (chatty) {
                if (r == NULL) {
                    r = new StringBuffer();
                } else {
                    *r += " ";
                }
                *r += a->mInfo->mName;
            }
        }
        if (r != NULL) {
            if (Config::LOGD) Logger::D(TAG, StringBuffer("  Activities: ") + *r);
            delete r;
        }

        r = NULL;
        List<CapsuleParser::Permission*>::Iterator permissionIt;
        for (permissionIt = cap->mPermissions.Begin();
             permissionIt != cap->mPermissions.End(); permissionIt++) {
            CapsuleParser::Permission* p = *permissionIt;
            Boolean tree = FALSE;
            HashMap<String, BasePermission*>::Iterator it =
                                mSettings->mPermissions.Find(p->mInfo->mName);
            BasePermission* bp = it->mSecond;
            if (bp == NULL) {
                tree = TRUE;
                it = mSettings->mPermissionTrees.Find(p->mInfo->mName);
                bp = it->mSecond;
            }
            if (bp != NULL && bp->mPerm == p) {
                bp->mPerm = NULL;
                if (chatty) {
                    if (r == NULL) {
                        r = new StringBuffer();
                    } else {
                        *r += " ";
                    }
                    *r += p->mInfo->mName;
                }
            }
        }
        if (r != NULL) {
            if (Config::LOGD) Logger::D(TAG, StringBuffer("  Permissions: ") + *r);
            delete r;
        }

        r = NULL;
        List<CapsuleParser::Instrumentation*>::Iterator instrumentationIt;
        for (instrumentationIt = cap->mInstrumentation.Begin();
             instrumentationIt != cap->mInstrumentation.End(); instrumentationIt++) {
            CapsuleParser::Instrumentation* a = *instrumentationIt;
            AutoPtr<IComponentName> name;
            a->GetComponentName((IComponentName**)&name);
            mInstrumentation.Erase(name);
            if (chatty) {
                if (r == NULL) {
                    r = new StringBuffer();
                } else {
                    *r += " ";
                }
                *r += a->mInfo->mName;
            }
        }
        if (r != NULL) {
            if (Config::LOGD) Logger::D(TAG, StringBuffer("  Instrumentation: ") + *r);
            delete r;
        }
    }
    return NOERROR;
}

ECode CCapsuleManagerService::GenerateComponentNameKey(
    /* [in] */ String capsuleName,
    /* [in] */ String name,
    /* [out] */ String* compName)
{
    StringBuffer buf;
    buf += capsuleName;
    buf += ".";
    buf += name;
    *compName = String::Duplicate(buf);
    return NOERROR;
}

Boolean CCapsuleManagerService::IsCapsuleFilename(
    /* [in] */ String name)
{
    return !name.IsNull() && name.EndWith(".cap");
}

ECode CCapsuleManagerService::UpdatePermissionsLP(
    /* [in] */ String changingPkg,
    /* [in] */ CapsuleParser::Capsule* capInfo,
    /* [in] */ Boolean grantPermissions,
    /* [in] */ Boolean replace,
    /* [in] */ Boolean replaceAll)
{
//	    // Make sure there are no dangling permission trees.
//	    Iterator<BasePermission> it = mSettings->mPermissionTrees
//	            .values().iterator();
//	    while (it.hasNext()) {
//	        BasePermission bp = it.next();
//	        if (bp.packageSetting == null) {
//	            // We may not yet have parsed the package, so just see if
//	            // we still know about its settings.
//	            bp.packageSetting = mSettings.mPackages.get(bp.sourcePackage);
//	        }
//	        if (bp.packageSetting == null) {
//	            Slog.w(TAG, "Removing dangling permission tree: " + bp.name
//	                    + " from package " + bp.sourcePackage);
//	            it.remove();
//	        } else if (changingPkg != null && changingPkg.equals(bp.sourcePackage)) {
//	            if (pkgInfo == null || !hasPermission(pkgInfo, bp.name)) {
//	                Slog.i(TAG, "Removing old permission tree: " + bp.name
//	                        + " from package " + bp.sourcePackage);
//	                grantPermissions = true;
//	                it.remove();
//	            }
//	        }
//	    }
//
//	    // Make sure all dynamic permissions have been assigned to a package,
//	    // and make sure there are no dangling permissions.
//	    it = mSettings.mPermissions.values().iterator();
//	    while (it.hasNext()) {
//	        BasePermission bp = it.next();
//	        if (bp.type == BasePermission.TYPE_DYNAMIC) {
//	            if (DEBUG_SETTINGS) Log.v(TAG, "Dynamic permission: name="
//	                    + bp.name + " pkg=" + bp.sourcePackage
//	                    + " info=" + bp.pendingInfo);
//	            if (bp.packageSetting == null && bp.pendingInfo != null) {
//	                BasePermission tree = findPermissionTreeLP(bp.name);
//	                if (tree != null) {
//	                    bp.packageSetting = tree.packageSetting;
//	                    bp.perm = new PackageParser.Permission(tree.perm.owner,
//	                            new PermissionInfo(bp.pendingInfo));
//	                    bp.perm.info.packageName = tree.perm.info.packageName;
//	                    bp.perm.info.name = bp.name;
//	                    bp.uid = tree.uid;
//	                }
//	            }
//	        }
//	        if (bp.packageSetting == null) {
//	            // We may not yet have parsed the package, so just see if
//	            // we still know about its settings.
//	            bp.packageSetting = mSettings.mPackages.get(bp.sourcePackage);
//	        }
//	        if (bp.packageSetting == null) {
//	            Slog.w(TAG, "Removing dangling permission: " + bp.name
//	                    + " from package " + bp.sourcePackage);
//	            it.remove();
//	        } else if (changingPkg != null && changingPkg.equals(bp.sourcePackage)) {
//	            if (pkgInfo == null || !hasPermission(pkgInfo, bp.name)) {
//	                Slog.i(TAG, "Removing old permission: " + bp.name
//	                        + " from package " + bp.sourcePackage);
//	                grantPermissions = true;
//	                it.remove();
//	            }
//	        }
//	    }
//
//	    // Now update the permissions for all packages, in particular
//	    // replace the granted permissions of the system packages.
//	    if (grantPermissions) {
//	        for (PackageParser.Package pkg : mPackages.values()) {
//	            if (pkg != pkgInfo) {
//	                grantPermissionsLP(pkg, replaceAll);
//	            }
//	        }
//	    }
//
//	    if (pkgInfo != null) {
//	        grantPermissionsLP(pkgInfo, replace);
//	    }

    return E_NOT_IMPLEMENTED;
}

void CCapsuleManagerService::SendCapsuleBroadcast(
    /* [in] */ String action,
    /* [in] */ String cap,
    /* [in] */ IBundle* extras,
    /* [in] */ IIntentReceiver* finishedReceiver)
{
//	    IActivityManager am = ActivityManagerNative.getDefault();
//	    if (am != null) {
//	        try {
//	            final Intent intent = new Intent(action,
//	                    pkg != null ? Uri.fromParts("package", pkg, null) : null);
//	            if (extras != null) {
//	                intent.putExtras(extras);
//	            }
//	            intent.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
//	            am.broadcastIntent(null, intent, null, finishedReceiver,
//	                    0, null, null, null, finishedReceiver != null, false);
//	        } catch (RemoteException ex) {
//	        }
//	    }
}

void CCapsuleManagerService::ScheduleCapsuleCleaning(
    /* [in] */ String capsuleName)
{
    String* name = new String(String::Duplicate(capsuleName));
    Message* msg = mHandler->ObtainMessage(START_CLEANING_CAPSULE, name);
    mHandler->SendMessage(msg);
}

void CCapsuleManagerService::StartCleaningCapsules()
{
//	    synchronized (mPackages) {
//	        if (!mMediaMounted) {
//	            return;
//	        }
//	        if (mSettings.mPackagesToBeCleaned.size() <= 0) {
//	            return;
//	        }
//	    }
//	    Intent intent = new Intent(PackageManager.ACTION_CLEAN_EXTERNAL_STORAGE);
//	    intent.setComponent(DEFAULT_CONTAINER_COMPONENT);
//	    IActivityManager am = ActivityManagerNative.getDefault();
//	    if (am != null) {
//	        try {
//	            am.startService(null, intent, null);
//	        } catch (RemoteException e) {
//	        }
//	    }
}

ECode CCapsuleManagerService::InstallCapsuleEx(
    /* [in] */ String path)
{
    ECode ec;
    AutoString err;
    CapsuleParser p;
    char* fname;

    fname = (char*)malloc(path.GetLength() + strlen("/capsule.xml") + 1);
    strcpy(fname, path);
    strcat(fname, "/capsule.xml");

    CapsuleParser::Capsule* capsule = new CapsuleParser::Capsule();
    if (SUCCEEDED(p.ParseCapsule(fname, capsule, &err))) {
        mCapsules[String::Duplicate(capsule->mCapsuleName)] = capsule;
        ec = NOERROR;
    }
    else {
        if (DEBUG_INSTALL) {
            Slogger::E(TAG, err);
        }
        ec = E_FAIL;
    }

    free(fname);

    return ec;
}

ECode CCapsuleManagerService::InstallCapsule(
    /* [in] */ IUri* capsuleURI,
    /* [in] */ ICapsuleInstallObserver* observer,
    /* [in] */ Int32 flags,
    /* [in] */ String installerCapsuleName)
{
//	    mContext->EnforceCallingOrSelfPermission(
//	            android.Manifest.permission.INSTALL_PACKAGES, NULL);

    Message* msg = mHandler->ObtainMessage(INIT_COPY);
    msg->mObj = new InstallParams(capsuleURI, observer, flags, installerCapsuleName);
    return mHandler->SendMessage(msg);
}

ECode CCapsuleManagerService::FinishCapsuleInstall(
    /* [in] */ Int32 token)
{
    if (DEBUG_INSTALL) Logger::V(TAG, StringBuffer("BM finishing capsule install for ") + token);
    Message* msg = mHandler->ObtainMessage(POST_INSTALL, token, 0);
    return mHandler->SendMessage(msg);
}

ECode CCapsuleManagerService::CreateInstallArgs(
    /* [in] */ Int32 flags,
    /* [in] */ String fullCodePath,
    /* [in] */ String fullResourcePath,
    /* [in] */ String nativeLibraryPath,
    /* [out] */ InstallArgs** args)
{
//	    if (installOnSd(flags)) {
//	        return new SdInstallArgs(fullCodePath, fullResourcePath, nativeLibraryPath);
//	    } else {
//	        return new FileInstallArgs(fullCodePath, fullResourcePath, nativeLibraryPath);
//	    }
	return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::DeleteCapsule(
    /* [in] */ String capsuleName,
    /* [in] */ ICapsuleDeleteObserver* observer,
    /* [in] */ Int32 flags)
{
    VALIDATE_STRING_NOT_NULL(capsuleName);

//	    mContext.enforceCallingOrSelfPermission(
//	            android.Manifest.permission.DELETE_PACKAGES, null);

    // Queue up an async operation since the package deletion may take a little while.
    if (observer) observer->AddRef();
    Message* msg = mHandler->ObtainMessage(DELETE_CAPSULE,
        (Int32)new String(String::Duplicate(capsuleName)), flags, (void*)observer);
    return mHandler->SendMessage(msg);
}

Boolean CCapsuleManagerService::DeleteCapsuleX(
    /* [in] */ String capsuleName,
    /* [in] */ Boolean sendBroadCast,
    /* [in] */ Boolean deleteCodeAndResources,
    /* [in] */ Int32 flags)
{
    CapsuleRemovedInfo* info = new CapsuleRemovedInfo();
    Boolean res = FALSE;

//	    IDevicePolicyManager dpm = IDevicePolicyManager::Stub::AsInterface(
//	            ServiceManager::GetService(Context::DEVICE_POLICY_SERVICE));
//	    try {
//	        if (dpm != NULL && dpm->CapsuleHasActiveAdmins(capsuleName)) {
//	            Slogger::W(TAG, "Not removing capsule " + capsuleName + ": has active device admin");
//	            return FALSE;
//	        }
//	    } catch (RemoteException e) {
//	    }

    {
        Mutex::Autolock lock(mInstallLock);
        res = DeleteCapsuleLI(capsuleName, deleteCodeAndResources,
                flags | REMOVE_CHATTY, info, TRUE);
    }

    if (res && sendBroadCast) {
        Boolean systemUpdate = info->mIsRemovedCapsuleSystemUpdate;
        info->SendBroadcast(deleteCodeAndResources, systemUpdate);

        // If the removed capsule was a system update, the old system capsuled
        // was re-enabled; we need to broadcast this information
        if (systemUpdate) {
            AutoPtr<CBundle> extras;
            ASSERT_SUCCEEDED(CBundle::NewByFriend(1, (CBundle**)&extras));
            extras->PutInt32(Intent_EXTRA_UID, info->mRemovedUid >= 0 ? info->mRemovedUid : info->mUid);
            extras->PutBoolean(Intent_EXTRA_REPLACING, TRUE);

            SendCapsuleBroadcast(Intent_ACTION_CAPSULE_ADDED, capsuleName, (IBundle*)extras.Get(), NULL);
            SendCapsuleBroadcast(Intent_ACTION_CAPSULE_REPLACED, capsuleName, (IBundle*)extras.Get(), NULL);
        }
    }
    // Force a gc here.
//	    Runtime::GetRuntime().Gc();
    // Delete the resources here after sending the broadcast to let
    // other processes clean up before deleting resources.
    if (info->mArgs != NULL) {
        Mutex::Autolock lock(mInstallLock);
        info->mArgs->DoPostDeleteLI(deleteCodeAndResources);
    }
    return res;
}

ECode CCapsuleManagerService::RemoveCapsuleDataLI(
    /* [in] */ CapsuleParser::Capsule* p,
    /* [in] */ CapsuleRemovedInfo* outInfo,
    /* [in] */ Int32 flags,
    /* [in] */ Boolean writeSettings)
{
    AutoString capsuleName(String::Duplicate(p->mCapsuleName));
    if (outInfo != NULL) {
        outInfo->mRemovedCapsule = String::Duplicate(capsuleName);
    }
    RemoveCapsuleLI(p, (flags & REMOVE_CHATTY) != 0);
    // Retrieve object to delete permissions for shared user later on
    CapsuleSetting* deletedCs = NULL;
    {
        Mutex::Autolock lock(mCapsulesLock);
        HashMap<String, CapsuleSetting*>::Iterator itor
                                = mSettings->mCapsules.Find(capsuleName);
        deletedCs = itor->mSecond;
    }
    if ((flags & CapsuleManager_DONT_DELETE_DATA) == 0) {
        Boolean useEncryptedFSDir = UseEncryptedFilesystemForCapsule(p);
        if (mInstaller != NULL) {
            Int32 retCode = mInstaller->Remove(capsuleName, useEncryptedFSDir);
            if (retCode < 0) {
                Slogger::W(TAG, StringBuffer("Couldn't remove app data or cache directory for capsule: ")
                           + capsuleName + ", retcode=" + retCode);
                // we don't consider this to be a failure of the core capsule deletion
            }
        } else {
            // for simulator
            HashMap<String, CapsuleParser::Capsule*>::Iterator it
                = mCapsules.Find(capsuleName);
            CapsuleParser::Capsule* cap = it->mSecond;
            assert(cap);
            AutoPtr<IFile> dataDir;
            FAIL_RETURN(CFile::New(cap->mApplicationInfo->mDataDir, (IFile**)&dataDir));
            Boolean isDeleted = FALSE;
            dataDir->Delete(&isDeleted);
        }
        ScheduleCapsuleCleaning(capsuleName);
    }
    {
        Mutex::Autolock lock(mCapsulesLock);
        if (deletedCs != NULL) {
            if ((flags & CapsuleManager_DONT_DELETE_DATA) == 0) {
                if (outInfo != NULL) {
                    outInfo->mRemovedUid = mSettings->RemoveCapsuleLP(capsuleName);
                }
                if (deletedCs != NULL) {
                    UpdatePermissionsLP(deletedCs->mName, NULL, FALSE, FALSE, FALSE);
                    if (deletedCs->mSharedUser != NULL) {
                        // remove permissions associated with capsule
                        mSettings->UpdateSharedUserPermsLP(deletedCs, *mGlobalGids);
                    }
                }
            }
            // remove from preferred activities.
            List< AutoPtr<PreferredActivity> > removed;
            List<PreferredActivity*>* list = mSettings->mPreferredActivities->FilterSet();
            if (list) {
                List<PreferredActivity*>::Iterator it;
                for (it = list->Begin(); it != list->End(); it++) {
                    AutoPtr<PreferredActivity> pa = *it;
                    String name;
                    pa->mActivity->GetCapsuleName(&name);
                    if (name.Equals(deletedCs->mName)) {
                        removed.PushBack(pa);
                    }
                }
            }
            List< AutoPtr<PreferredActivity> >::Iterator it;
            for (it = removed.Begin(); it != removed.End(); it++) {
                AutoPtr<PreferredActivity> pa = *it;
                // mSettings->mPreferredActivities->RemoveFilter(pa.Get());
            }
        }
        if (writeSettings) {
            // Save settings now
            mSettings->WriteLP();
        }
    }
    return NOERROR;
}

Boolean CCapsuleManagerService::DeleteSystemCapsuleLI(
    /* [in] */ CapsuleParser::Capsule* c,
    /* [in] */ Int32 flags,
    /* [in] */ CapsuleRemovedInfo* outInfo,
    /* [in] */ Boolean writeSettings)
{
    AutoPtr<CApplicationInfo> applicationInfo = c->mApplicationInfo;
    //applicable for non-partially installed applications only
    if (applicationInfo == NULL) {
        Slogger::W(TAG, StringBuffer("Capsule ") + c->mCapsuleName + " has no applicationInfo.");
        return FALSE;
    }
    CapsuleSetting* cs = NULL;
    // Confirm if the system capsule has been updated
    // An updated system app can be deleted. This will also have to restore
    // the system cap from system partition
    {
    	Mutex::Autolock lock(mCapsulesLock);
        cs = mSettings->GetDisabledSystemCap(c->mCapsuleName);
    }
    if (cs == NULL) {
        Slogger::W(TAG, StringBuffer("Attempt to delete unknown system capsule ") + c->mCapsuleName);
        return FALSE;
    }
	else {
        Logger::I(TAG, "Deleting system cap from data partition");
    }
    // Delete the updated capsule
    outInfo->mIsRemovedCapsuleSystemUpdate = TRUE;
    if (cs->mVersionCode < c->mVersionCode) {
        // Delete data for downgrades
        flags &= ~CapsuleManager_DONT_DELETE_DATA;
    } else {
        // Preserve data by setting flag
        flags |= CapsuleManager_DONT_DELETE_DATA;
    }
    Boolean ret = DeleteInstalledCapsuleLI(c, TRUE, flags, outInfo, writeSettings);
    if (!ret) {
        return FALSE;
    }
    {
		Mutex::Autolock lock(mCapsulesLock);
        // Reinstate the old system capsule
        mSettings->EnableSystemCapsuleLP(c->mCapsuleName);
        // Remove any native libraries from the upgraded capsule.
        NativeLibraryHelper::RemoveNativeBinariesLI(c->mApplicationInfo->mNativeLibraryDir);
    }
    // Install the system capsule
    CapsuleParser::Capsule* newCap = ScanCapsuleLI(cs->mCodePath,
            CapsuleParser::PARSE_MUST_BE_APK | CapsuleParser::PARSE_IS_SYSTEM,
            SCAN_MONITOR | SCAN_NO_PATHS, 0);
    if (newCap == NULL) {
        Slogger::W(TAG, StringBuffer("Failed to restore system capsule:")
						+ c->mCapsuleName + " with error:" + mLastScanError);
        return FALSE;
    }
    {
		Mutex::Autolock lock(mCapsulesLock);
        UpdatePermissionsLP(newCap->mCapsuleName, newCap, TRUE, TRUE, FALSE);
        if (writeSettings) {
            mSettings->WriteLP();
        }
    }
    return TRUE;
}

Boolean CCapsuleManagerService::DeleteInstalledCapsuleLI(
    /* [in] */ CapsuleParser::Capsule* c,
    /* [in] */ Boolean deleteCodeAndResources,
    /* [in] */ Int32 flags,
    /* [in] */ CapsuleRemovedInfo* outInfo,
    /* [in] */ Boolean writeSettings)
{
    AutoPtr<CApplicationInfo> applicationInfo = c->mApplicationInfo;
    if (applicationInfo == NULL) {
        Slogger::W(TAG, StringBuffer("Capsule ") + c->mCapsuleName + " has no applicationInfo.");
        return FALSE;
    }
    if (outInfo != NULL) {
        outInfo->mUid = applicationInfo->mUid;
    }

    // Delete capsule data from Int32ernal structures and also remove data if flag is set
    RemoveCapsuleDataLI(c, outInfo, flags, writeSettings);

    // Delete application code and resources
    if (deleteCodeAndResources) {
        // TODO can pick up from CapsuleSettings as well
        Int32 installFlags = IsExternal(c) ? CapsuleManager::INSTALL_EXTERNAL : 0;
        installFlags |= IsForwardLocked(c) ? CapsuleManager::INSTALL_FORWARD_LOCK : 0;
        CreateInstallArgs(
			installFlags, applicationInfo->mSourceDir,
            applicationInfo->mPublicSourceDir, applicationInfo->mNativeLibraryDir,
            &outInfo->mArgs);
    }
    return TRUE;
}

Boolean CCapsuleManagerService::DeleteCapsuleLI(
    /* [in] */ String capsuleName,
    /* [in] */ Boolean deleteCodeAndResources,
    /* [in] */ Int32 flags,
    /* [in] */ CapsuleRemovedInfo* outInfo,
    /* [in] */ Boolean writeSettings)
{
    if (capsuleName.IsNullOrEmpty()) {
        Slogger::W(TAG, "Attempt to delete NULL capsuleName.");
        return FALSE;
    }
    CapsuleParser::Capsule* p = NULL;
    Boolean dataOnly = FALSE;
    {
        Mutex::Autolock lock(mCapsulesLock);
        HashMap<String, CapsuleParser::Capsule*>::Iterator itor = mCapsules.Find(capsuleName);
        if (itor == mCapsules.End()) {
            //this retrieves partially installed apps
            dataOnly = TRUE;
            HashMap<String, CapsuleSetting*>::Iterator it = mSettings->mCapsules.Find(capsuleName);
            if (it == mSettings->mCapsules.End()) {
                Slogger::W(TAG, StringBuffer("Capsule named '") + capsuleName +"' doesn't exist.");
                return FALSE;
            }
            p = it->mSecond->mCap;
        }
        else {
            p = itor->mSecond;
        }
    }
    if (p == NULL) {
        Slogger::W(TAG, StringBuffer("Capsule named '") + capsuleName +"' doesn't exist.");
        return FALSE;
    }

    if (dataOnly) {
        // Delete application data first
        RemoveCapsuleDataLI(p, outInfo, flags, writeSettings);
        return TRUE;
    }
    // At this point the capsule should have ApplicationInfo associated with it
    if (p->mApplicationInfo == NULL) {
        Slogger::W(TAG, StringBuffer("Capsule ") + p->mCapsuleName + " has no applicationInfo.");
        return FALSE;
    }
    Boolean ret = FALSE;
    if (IsSystemApp(p)) {
        Logger::I(TAG, StringBuffer("Removing system capsule:") + p->mCapsuleName);
        // When an updated system application is deleted we delete the existing resources as well and
        // fall back to existing code in system partition
        ret = DeleteSystemCapsuleLI(p, flags, outInfo, writeSettings);
    } else {
        Logger::I(TAG, StringBuffer("Removing non-system capsule:") + p->mCapsuleName);
        // Kill application pre-emptively especially for apps on sd.
        KillApplication(capsuleName, p->mApplicationInfo->mUid);
        ret = DeleteInstalledCapsuleLI(p, deleteCodeAndResources, flags, outInfo, writeSettings);
    }
    return ret;
}

ECode CCapsuleManagerService::GetInstallerCapsuleName(
    /* [in] */ String capsuleName,
    /* [out] */ String* name)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::AddCapsuleToPreferred(
    /* [in] */ String capsuleName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::RemoveCapsuleFromPreferred(
    /* [in] */ String capsuleName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::GetPreferredCapsules(
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::AddPreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ const ArrayOf<IComponentName> & set,
    /* [in] */ IComponentName* activity)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::ReplacePreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ const ArrayOf<IComponentName> & set,
    /* [in] */ IComponentName* activity)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::ClearCapsulePreferredActivities(
    /* [in] */ String capsuleName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

Boolean CCapsuleManagerService::ClearCapsulePreferredActivitiesLP(
    /* [in] */ String capsuleName)
{
    Boolean changed = FALSE;
    List<PreferredActivity*>* list = mSettings->mPreferredActivities->FilterIterator();
    List<PreferredActivity*>::Iterator it;
    for (it = list->Begin(); it != list->End();) {
        PreferredActivity* pa = *it;
        AutoString name;
        pa->mActivity->GetCapsuleName(&name);
        if (name.Equals(capsuleName)) {
            List<PreferredActivity*>::Iterator iit = it++;
            list->Erase(iit);
            changed = TRUE;
        }
        else {
            it++;
        }
    }
    return changed;
}

ECode CCapsuleManagerService::GetPreferredActivities(
    /* [in] */ IObjectContainer* outFilters,
    /* [in] */ IObjectContainer* outActivities,
    /* [in] */ String capsuleName,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    Int32 num = 0;
    {
        Mutex::Autolock lock(mCapsulesLock);
        List<PreferredActivity*>* list = mSettings->mPreferredActivities->FilterIterator();
        List<PreferredActivity*>::Iterator it;
        for (it = list->Begin(); it != list->End();) {
            PreferredActivity* pa = *it; // it->Next();
            AutoString name;
            pa->mActivity->GetCapsuleName(&name);
            if (capsuleName.IsNull() || name.Equals(capsuleName)) {
                if (outFilters != NULL) {
                    AutoPtr<IIntentFilter> filter;
                    FAIL_RETURN(CIntentFilter::New(
                        (IIntentFilter*)pa->Probe(EIID_IIntentFilter),
                        (IIntentFilter**)&filter));
                    outFilters->Add(filter.Get());
                }
                if (outActivities != NULL) {
                    outActivities->Add(pa->mActivity.Get());
                }
            }
        }
    }
    *count = num;
    return NOERROR;
}

ECode CCapsuleManagerService::SetComponentEnabledSetting(
    /* [in] */ IComponentName* componentName,
    /* [in] */ Int32 newState,
    /* [in] */ Int32 flags)
{
    VALIDATE_NOT_NULL(componentName);

    AutoString capName, clsName;
    componentName->GetCapsuleName(&capName);
    componentName->GetClassName(&clsName);
    return SetEnabledSetting(capName, clsName, newState, flags);
}

ECode CCapsuleManagerService::SetEnabledSetting(
    /* [in] */ String capsuleName,
    /* [in] */ String className,
    /* [in] */ Int32 newState,
    /* [in] */ Int32 flags)
{
    if (!(newState == CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT
          || newState == CapsuleManager_COMPONENT_ENABLED_STATE_ENABLED
          || newState == CapsuleManager_COMPONENT_ENABLED_STATE_DISABLED)) {
        Logger::E(TAG, StringBuffer("Invalid new component state: ") + newState);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    CapsuleSetting* capSetting;
    const Int32 uid = -1 /*Binder::GetCallingUid()*/;
    Int32 permission = 0;
    mContext->CheckCallingPermission(
        "" /*android.Manifest.permission.CHANGE_COMPONENT_ENABLED_STATE*/,
        &permission);
    const Boolean allowedByPermission = (permission == CapsuleManager_PERMISSION_GRANTED);
    Boolean sendNow = FALSE;
    Boolean isApp = (className.IsNull());
    String componentName = isApp ? capsuleName : className;
    Int32 capsuleUid = -1;
    List<String> components;
    {
        Mutex::Autolock lock(mCapsulesLock);

        HashMap<String, CapsuleSetting*>::Iterator csIt = mSettings->mCapsules.Find(capsuleName);
        capSetting = csIt->mSecond;
        if (capSetting == NULL) {
            if (className.IsNull()) {
                Logger::E(TAG, StringBuffer("Unknown capsule: ") + capsuleName);
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            Logger::E(TAG, StringBuffer("Unknown component: ") + capsuleName + "/" + className);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        if (!allowedByPermission && (uid != capSetting->mUserId)) {
            Logger::E(TAG, StringBuffer("Permission Denial: attempt to change component state from pid=")
                + -1 /*Binder::GetCallingPid()*/
                + ", uid=" + uid + ", capsule uid=" + capSetting->mUserId);
            return E_SECURITY_EXCEPTION;
        }
        if (className.IsNull()) {
            // We're dealing with an application/capsule level state change
            if (capSetting->mEnabled == newState) {
                // Nothing to do
                return NOERROR;
            }
            capSetting->mEnabled = newState;
            capSetting->mCap->mSetEnabled = newState;
        } else {
            // We're dealing with a component level state change
            switch (newState) {
            case CapsuleManager_COMPONENT_ENABLED_STATE_ENABLED:
                if (!capSetting->EnableComponentLP(className)) {
                    return NOERROR;
                }
                break;
            case CapsuleManager_COMPONENT_ENABLED_STATE_DISABLED:
                if (!capSetting->DisableComponentLP(className)) {
                    return NOERROR;
                }
                break;
            case CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT:
                if (!capSetting->RestoreComponentLP(className)) {
                    return NOERROR;
                }
                break;
            default:
                Slogger::E(TAG, StringBuffer("Invalid new component state: ") + newState);
                return E_RUNTIME_EXCEPTION;
            }
        }
        mSettings->WriteLP();
        capsuleUid = capSetting->mUserId;
        HashMap< String, List<String> >::Iterator pbIt = mPendingBroadcasts.Find(capsuleName);
        components = pbIt->mSecond;
        Boolean newCapsule = pbIt == mPendingBroadcasts.End();
        List<String>::Iterator comIt;
        Boolean isContained = FALSE;
        for (comIt = components.Begin(); comIt != components.End(); comIt++) {
            if (componentName.Equals(*comIt)) {
                isContained = TRUE;
                break;
            }
        }
        if (!isContained) {
            components.PushBack(componentName);
        }
        if ((flags & CapsuleManager::DONT_KILL_APP) == 0) {
            sendNow = TRUE;
            // Purge entry from pending broadcast list if another one exists already
            // since we are sending one right away->
            mPendingBroadcasts.Erase(capsuleName);
        } else {
            if (newCapsule) {
                mPendingBroadcasts[capsuleName] = components;
            }
//	            if (!mHandler->HasMessages(SEND_PENDING_BROADCAST)) {
//	                // Schedule a message
//	                mHandler->SendEmptyMessageDelayed(SEND_PENDING_BROADCAST, BROADCAST_DELAY);
//	            }
        }
    }

    Int64 callingId = -1 /*Binder::ClearCallingIdentity()*/;
    (void)callingId;
    if (sendNow) {
        SendCapsuleChangedBroadcast(capsuleName,
            (flags & CapsuleManager::DONT_KILL_APP) != 0, components, capsuleUid);
    }
//	    Binder::RestoreCallingIdentity(callingId);
    return NOERROR;
}

ECode CCapsuleManagerService::GetComponentEnabledSetting(
    /* [in] */ IComponentName* componentName,
    /* [out] */ Int32* setting)
{
    VALIDATE_NOT_NULL(componentName);
    VALIDATE_NOT_NULL(setting);

    Mutex::Autolock Lock(mCapsulesLock);

    AutoString capsuleNameStr;
    componentName->GetCapsuleName(&capsuleNameStr);
    HashMap<String, CapsuleSetting*>::Iterator it = mSettings->mCapsules.Find(capsuleNameStr);
    CapsuleSetting* cap = it->mSecond;
    if (cap == NULL) {
        Logger::E(TAG, StringBuffer("Unknown component: ") + componentName);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoString classNameStr;
    componentName->GetClassName(&classNameStr);
    return cap->CurrentEnabledStateLP(classNameStr);
}

ECode CCapsuleManagerService::SetApplicationEnabledSetting(
    /* [in] */ String capsuleName,
    /* [in] */ Int32 newState,
    /* [in] */ Int32 flags)
{
    return SetEnabledSetting(capsuleName, String(NULL), newState, flags);
}

ECode CCapsuleManagerService::GetApplicationEnabledSetting(
    /* [in] */ String capsuleName,
    /* [out] */ Int32* setting)
{
    VALIDATE_NOT_NULL(setting);

    Mutex::Autolock Lock(mCapsulesLock);

    HashMap<String, CapsuleSetting*>::Iterator it = mSettings->mCapsules.Find(capsuleName);
    CapsuleSetting* cap = it->mSecond;
    if (cap == NULL) {
        Logger::E(TAG, StringBuffer("Unknown capsule: ") + capsuleName);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *setting = cap->mEnabled;
    return NOERROR;
}

void CCapsuleManagerService::HandleFreeStorageAndNotify(
    /* [in] */ Int64 freeStorageSize,
    /* [in] */ ICapsuleDataObserver* observer)
{
//	    mHandler->RemoveCallbacks(this);
    Int32 retCode = -1;
    if (mInstaller != NULL) {
        retCode = mInstaller->FreeCache(freeStorageSize);
        if (retCode < 0) {
            Slogger::W(TAG, "Couldn't clear application caches");
        }
    } //end if mInstaller
    if (observer != NULL) {
        if (FAILED(observer->OnRemoveCompleted(NULL, (retCode >= 0)))) {
            Slogger::W(TAG, "RemoveException when invoking call back");
        }
    }
}

ECode CCapsuleManagerService::FreeStorageAndNotify(
    /* [in] */ Int64 freeStorageSize,
    /* [in] */ ICapsuleDataObserver* observer)
{
    mContext->EnforceCallingOrSelfPermission(
        "" /*android.Manifest.permission.CLEAR_APP_CACHE*/, String(NULL));
    // Queue up an async operation since clearing cache may take a little while.
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(Int64, ICapsuleDataObserver*);
    pHandlerFunc = &CCapsuleManagerService::HandleFreeStorageAndNotify;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt64(freeStorageSize);
    params->WriteInt32((Handle32)observer);
    return mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params.Get());
}

void CCapsuleManagerService::HandleFreeStorage(
    /* [in] */ Int64 freeStorageSize,
    /* [in] */ IIntentSender* pi)
{
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(Int64, IIntentSender*);
    pHandlerFunc = &CCapsuleManagerService::HandleFreeStorage;
    mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    Int32 retCode = -1;
    if (mInstaller != NULL) {
        retCode = mInstaller->FreeCache(freeStorageSize);
        if (retCode < 0) {
            Slogger::W(TAG, "Couldn't clear application caches");
        }
    }

    if (pi != NULL) {
        // Callback via pending intent
        Int32 code = (retCode >= 0) ? 1 : 0;
        if (FAILED(pi->SendIntent(NULL, code, NULL, NULL, NULL))) {
            Slogger::I(TAG, "Failed to send pending intent");
        }
    }
}

ECode CCapsuleManagerService::FreeStorage(
    /* [in] */ Int64 freeStorageSize,
    /* [in] */ IIntentSender* pi)
{
    mContext->EnforceCallingOrSelfPermission(
        "" /*android.Manifest.permission.CLEAR_APP_CACHE*/, String(NULL));
    // Queue up an async operation since clearing cache may take a little while.
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(Int64, IIntentSender*);
    pHandlerFunc = &CCapsuleManagerService::HandleFreeStorage;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt64(freeStorageSize);
    params->WriteInt32((Handle32)pi);
    return mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params.Get());
}

void CCapsuleManagerService::HandleDeleteApplicationCacheFiles(
    /* [in] */ String capsuleName,
    /* [in] */ ICapsuleDataObserver* observer)
{
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(String, ICapsuleDataObserver*);
    pHandlerFunc = &CCapsuleManagerService::HandleDeleteApplicationCacheFiles;
    mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    Boolean succeded = FALSE;
    {
        Mutex::Autolock lock(mInstallLock);
        succeded = DeleteApplicationCacheFilesLI(capsuleName);
    }

    if (observer != NULL) {
        if (FAILED(observer->OnRemoveCompleted(capsuleName, succeded))) {
            Logger::I(TAG, "Observer no Int64er exists->");
        }
    }
}

ECode CCapsuleManagerService::DeleteApplicationCacheFiles(
    /* [in] */ String capsuleName,
    /* [in] */ ICapsuleDataObserver* observer)
{
    mContext->EnforceCallingOrSelfPermission(
        "" /*android.Manifest.permission.DELETE_CACHE_FILES*/, String(NULL));
    // Queue up an async operation since the package deletion may take a little while.
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(String, ICapsuleDataObserver*);
    pHandlerFunc = &CCapsuleManagerService::HandleDeleteApplicationCacheFiles;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(capsuleName);
    params->WriteInt32((Handle32)observer);
    return mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params.Get());
}

Boolean CCapsuleManagerService::DeleteApplicationCacheFilesLI(
    /* [in] */ String capsuleName)
{
    if (capsuleName.IsNull()) {
        Slogger::W(TAG, "Attempt to delete NULL capsuleName->");
        return FALSE;
    }
    CapsuleParser::Capsule* c = NULL;
    {
        Mutex::Autolock lock(mCapsulesLock);
        HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(capsuleName);
        c = it->mSecond;
    }
    if (c == NULL) {
        Slogger::W(TAG, StringBuffer("Capsule named '") + capsuleName +"' doesn't exist->");
        return FALSE;
    }
    AutoPtr<CApplicationInfo> applicationInfo = c->mApplicationInfo;
    if (applicationInfo == NULL) {
        Slogger::W(TAG, StringBuffer("Capsule ") + capsuleName + " has no applicationInfo->");
        return FALSE;
    }
    Boolean useEncryptedFSDir = UseEncryptedFilesystemForCapsule(c);
    if (mInstaller != NULL) {
        Int32 retCode = mInstaller->DeleteCacheFiles(capsuleName, useEncryptedFSDir);
        if (retCode < 0) {
            Slogger::W(TAG,
                StringBuffer("Couldn't remove cache files for capsule: ")
                + capsuleName);
            return FALSE;
        }
    }
    return TRUE;
}

void CCapsuleManagerService::HandleClearApplicationUserData(
    /* [in] */ String capsuleName,
    /* [in] */ ICapsuleDataObserver* observer)
{
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(String, ICapsuleDataObserver*);
    pHandlerFunc = &CCapsuleManagerService::HandleClearApplicationUserData;
    mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    Boolean succeeded = FALSE;
    {
        Mutex::Autolock lock(mInstallLock);
        succeeded = ClearApplicationUserDataLI(capsuleName);
    }

//	    if (succeeded) {
//	        // invoke DeviceStorageMonitor's update method to clear any notifications
//	        DeviceStorageMonitorService dsm = (DeviceStorageMonitorService)
//	                ServiceManager::GetService(DeviceStorageMonitorService::SERVICE);
//	        if (dsm != NULL) {
//	            dsm->UpdateMemory();
//	        }
//	    }

    if (observer != NULL) {
        if (FAILED(observer->OnRemoveCompleted(capsuleName, succeeded))) {
            Logger::I(TAG, "Observer no Int64er exists->");
        }
    }
}

ECode CCapsuleManagerService::ClearApplicationUserData(
    /* [in] */ String capsuleName,
    /* [in] */ ICapsuleDataObserver* observer)
{
    mContext->EnforceCallingOrSelfPermission(
        "" /*android.Manifest.permission.CLEAR_APP_USER_DATA*/, String(NULL));
    // Queue up an async operation since the package deletion may take a little while.
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(String, ICapsuleDataObserver*);
    pHandlerFunc = &CCapsuleManagerService::HandleClearApplicationUserData;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(capsuleName);
    params->WriteInt32((Handle32)observer);
    return mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params.Get());
}

Boolean CCapsuleManagerService::ClearApplicationUserDataLI(
    /* [in] */ String capsuleName)
{
    if (capsuleName.IsNull()) {
        Slogger::W(TAG, "Attempt to delete NULL capsuleName->");
        return FALSE;
    }
    CapsuleParser::Capsule* c = NULL;
    Boolean dataOnly = FALSE;
    {
        Mutex::Autolock lock(mCapsulesLock);
        HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(capsuleName);
        c = it->mSecond;
        if (c == NULL) {
            dataOnly = TRUE;
            HashMap<String, CapsuleSetting*>::Iterator csIt = mSettings->mCapsules.Find(capsuleName);
            CapsuleSetting* cs = csIt->mSecond;
            if ((cs == NULL) || (cs->mCap == NULL)) {
                Slogger::W(TAG, StringBuffer("Capsule named '") + capsuleName + "' doesn't exist->");
                return FALSE;
            }
            c = cs->mCap;
        }
    }

    Boolean useEncryptedFSDir = FALSE;

    if (!dataOnly) {
        //need to check this only for fully installed applications
        if (c == NULL) {
            Slogger::W(TAG, StringBuffer("Capsule named '") + capsuleName + "' doesn't exist->");
            return FALSE;
        }
        AutoPtr<CApplicationInfo> applicationInfo = c->mApplicationInfo;
        if (applicationInfo == NULL) {
            Slogger::W(TAG, StringBuffer("Capsule ") + capsuleName + " has no applicationInfo->");
            return FALSE;
        }
        useEncryptedFSDir = UseEncryptedFilesystemForCapsule(c);
    }
    if (mInstaller != NULL) {
        Int32 retCode = mInstaller->ClearUserData(capsuleName, useEncryptedFSDir);
        if (retCode < 0) {
            Slogger::W(TAG,
                StringBuffer("Couldn't remove cache files for capsule: ")
                + capsuleName);
            return FALSE;
        }
    }
    return TRUE;
}

void CCapsuleManagerService::HandleGetCapsuleSizeInfo(
    /* [in] */ String capsuleName,
    /* [in] */ ICapsuleStatsObserver* observer)
{
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(String, ICapsuleStatsObserver*);
    pHandlerFunc = &CCapsuleManagerService::HandleGetCapsuleSizeInfo;
    mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    AutoPtr<ICapsuleStats> lStats;
    CCapsuleStats::New(capsuleName, (ICapsuleStats**)&lStats);

    Boolean succeded = FALSE;
    {
        Mutex::Autolock lock(mInstallLock);
        succeded = GetCapsuleSizeInfoLI(capsuleName, lStats);
    }

    if (observer != NULL) {
        if (FAILED(observer->OnGetStatsCompleted(lStats, succeded))) {
            Logger::I(TAG, "Observer no Int64er exists->");
        }
    }
}

ECode CCapsuleManagerService::GetCapsuleSizeInfo(
    /* [in] */ String capsuleName,
    /* [in] */ ICapsuleStatsObserver* observer)
{
    mContext->EnforceCallingOrSelfPermission(
        "" /*android.Manifest.permission.GET_PACKAGE_SIZE*/, String(NULL));
    // Queue up an async operation since the package deletion may take a little while.
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(String, ICapsuleStatsObserver*);
    pHandlerFunc = &CCapsuleManagerService::HandleGetCapsuleSizeInfo;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(capsuleName);
    params->WriteInt32((Handle32)observer);
    return mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params.Get());
}

Boolean CCapsuleManagerService::GetCapsuleSizeInfoLI(
    /* [in] */ String capsuleName,
    /* [in] */ ICapsuleStats* stats)
{
    if (capsuleName.IsNull()) {
        Slogger::W(TAG, "Attempt to get size of NULL capsuleName->");
        return FALSE;
    }
    CapsuleParser::Capsule* c;
    Boolean dataOnly = FALSE;
    {
        Mutex::Autolock lock(mCapsulesLock);
        HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(capsuleName);
        c = it->mSecond;
        if (c == NULL) {
            dataOnly = TRUE;
            HashMap<String, CapsuleSetting*>::Iterator csIt = mSettings->mCapsules.Find(capsuleName);
            CapsuleSetting* cs = csIt->mSecond;
            if ((cs == NULL) || (cs->mCap == NULL)) {
                Slogger::W(TAG, StringBuffer("Capsule named '") + capsuleName + "' doesn't exist->");
                return FALSE;
            }
            c = cs->mCap;
        }
    }
    AutoString publicSrcDir;
    if (!dataOnly) {
        AutoPtr<CApplicationInfo> applicationInfo = c->mApplicationInfo;
        if (applicationInfo == NULL) {
            Slogger::W(TAG, StringBuffer("Capsule ") + capsuleName + " has no applicationInfo->");
            return FALSE;
        }
        publicSrcDir = IsForwardLocked(c)
            ? String::Duplicate(applicationInfo->mPublicSourceDir)
            : String(NULL);
    }
    Boolean useEncryptedFSDir = UseEncryptedFilesystemForCapsule(c);
    if (mInstaller != NULL) {
        Int32 res = mInstaller->GetSizeInfo(
            capsuleName, c->mPath, publicSrcDir, stats, useEncryptedFSDir);
        return res >= 0 ? TRUE : FALSE;
    }
    return TRUE;
}

ECode CCapsuleManagerService::GetSystemSharedLibraryNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);

    Int32 count = mSharedLibraries.GetSize();
    ArrayOf<String>* libs = ArrayOf<String>::Alloc(count);
    if (!libs) {
        return E_OUT_OF_MEMORY_ERROR;
    }

    {
        Mutex::Autolock lock(mCapsulesLock);

        Int32 i = 0;
        HashMap<String, String>::Iterator it;
        for (it = mSharedLibraries.Begin();
             it != mSharedLibraries.End(); it++) {
            (*libs)[i++] = String::Duplicate(it->mFirst);
        }
    }

    *names = libs;
    return NOERROR;
}

ECode CCapsuleManagerService::GetSystemAvailableFeatures(
    /* [out] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);

    Mutex::Autolock lock(mCapsulesLock);

    AutoPtr<IObjectContainer> featSet;
    FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&featSet));

    Int32 size = mAvailableFeatures.GetSize();
    if (size > 0) {
        HashMap<String, IFeatureInfo*>::Iterator it;
        for (it = mAvailableFeatures.Begin();
             it != mAvailableFeatures.End(); it++) {
            featSet->Add(it->mSecond);
        }
        AutoPtr<CFeatureInfo> fi;
        FAIL_RETURN(CFeatureInfo::NewByFriend((CFeatureInfo**)&fi));
        fi->mReqGlEsVersion = SystemProperties::GetInt32("ro->mOpengles->version",
                FeatureInfo_GL_ES_VERSION_UNDEFINED);
        featSet->Add((IFeatureInfo*)fi.Get());
    }

    *infos = featSet.Get();
    INTERFACE_ADDREF(*infos);
    return NOERROR;
}

ECode CCapsuleManagerService::HasSystemFeature(
    /* [in] */ String name,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mCapsulesLock);
    HashMap<String, IFeatureInfo*>::Iterator it = mAvailableFeatures.Find(name);
    *result = it != mAvailableFeatures.End();
    return NOERROR;
}

void CCapsuleManagerService::SendCapsuleChangedBroadcast(
    /* [in] */ String capsuleName,
    /* [in] */ Boolean killFlag,
    /* [in] */ List<String>& componentNames,
    /* [in] */ Int32 capsuleUid)
{
    if (FALSE) {
        Logger::V(TAG, StringBuffer("Sending capsule changed: capsule=")
            + capsuleName + " components=" /*+ componentNames*/);
    }
    AutoPtr<IBundle> extras; // = new Bundle(4);
    ASSERT_SUCCEEDED(CBundle::New((IBundle**)&extras));
    AutoStringArray nameList = ArrayOf<String>::Alloc(componentNames.GetSize());
    assert(nameList != NULL);
    AutoString first;
    Int32 i = 0;
    List<String>::Iterator it;
    for (it = componentNames.Begin(); it != componentNames.End(); it++) {
        if (i == 0) {
            first = String::Duplicate(*it);
        }
        (*nameList)[i++] = String::Duplicate(*it);
    }
    extras->PutString(Intent_EXTRA_CHANGED_COMPONENT_NAME, first);
    extras->PutStringArray(Intent_EXTRA_CHANGED_COMPONENT_NAME_LIST, *nameList);
    extras->PutBoolean(Intent_EXTRA_DONT_KILL_APP, killFlag);
    extras->PutInt32(Intent_EXTRA_UID, capsuleUid);
    SendCapsuleBroadcast(Intent_ACTION_CAPSULE_CHANGED, capsuleName, extras.Get(), NULL);
}

ECode CCapsuleManagerService::EnterSafeMode()
{
    if (!mSystemReady) {
        mSafeMode = true;
    }
    return NOERROR;
}

ECode CCapsuleManagerService::IsSafeMode(
    /* [out] */ Boolean* isSafeMode)
{
    VALIDATE_NOT_NULL(isSafeMode);
    *isSafeMode = mSafeMode;
    return NOERROR;
}

ECode CCapsuleManagerService::SystemReady()
{
    mSystemReady = TRUE;

    // Read the compatibilty setting when the system is ready->
    Boolean compatibilityModeEnabled = FALSE;
//	        android.provider.Settings.System.GetInt(
//	            mContext->GetContentResolver(),
//	            android.provider.Settings.System.COMPATIBILITY_MODE, 1) == 1;
    CapsuleParser::SetCompatibilityModeEnabled(compatibilityModeEnabled);
    if (DEBUG_SETTINGS) {
        Logger::D(TAG, StringBuffer("compatibility mode:") + compatibilityModeEnabled);
    }
    return NOERROR;
}

ECode CCapsuleManagerService::HasSystemUidErrors(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHasSystemUidErrors;
    return NOERROR;
}

void CCapsuleManagerService::HandleUpdateExternalMediaStatus(
    /* [in] */ Boolean mediaStatus,
    /* [in] */ Boolean reportStatus)
{
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(Boolean, Boolean);
    pHandlerFunc = &CCapsuleManagerService::HandleUpdateExternalMediaStatus;
    mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    UpdateExternalMediaStatusInner(mediaStatus, reportStatus);
}

ECode CCapsuleManagerService::UpdateExternalMediaStatus(
    /* [in] */ Boolean mediaStatus,
    /* [in] */ Boolean reportStatus)
{
//	    if (Binder::GetCallingUid() != Process::SYSTEM_UID) {
//	       throw new SecurityException("Media status can only be updated by the system");
//	    }
    {
        Mutex::Autolock lock(mCapsulesLock);

        Logger::I(TAG, StringBuffer("Updating external media status from ") +
               (mMediaMounted ? "mounted" : "unmounted") + " to " +
               (mediaStatus ? "mounted" : "unmounted"));
        if (DEBUG_SD_INSTALL) {
            Logger::I(TAG, StringBuffer("updateExternalMediaStatus:: mediaStatus=")
                + mediaStatus + ", mMediaMounted=" + mMediaMounted);
        }
        if (mediaStatus == mMediaMounted) {
            Message* msg = new Message();
            msg->mWhat = UPDATED_MEDIA_STATUS;
            msg->mArg1 = reportStatus ? 1 : 0;
            msg->mArg2 = -1;
            mHandler->SendMessage(msg);
            return NOERROR;
        }
        mMediaMounted = mediaStatus;
    }
    // Queue up an async operation since the capsule installation may take a little while->
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(Boolean, Boolean);
    pHandlerFunc = &CCapsuleManagerService::HandleUpdateExternalMediaStatus;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteBoolean(mediaStatus);
    params->WriteBoolean(reportStatus);
    return mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params.Get());
}

ECode CCapsuleManagerService::UpdateExternalMediaStatusInner(
    /* [in] */ Boolean mediaStatus,
    /* [in] */ Boolean reportStatus)
{
    // Collection of uids
    AutoFree< ArrayOf<Int32> > uidArr;
    // Collection of stale containers
    // HashSet<String> removeCids = new HashSet<String>();
    List<String> removeCids;
    // Collection of capsules on external media with valid containers->
    // HashMap<SdInstallArgs, String> processCids = new HashMap<SdInstallArgs, String>();
    HashMap<String, SdInstallArgs*> processCids;
    // Get list of secure containers->
    AutoStringArray list;
    FAIL_RETURN(CapsuleHelper::GetSecureContainerList((ArrayOf<String>**)&list));
    if (list == NULL || list->GetLength() == 0) {
        Logger::I(TAG, "No secure containers on sdcard");
    } else {
        // Process list of secure containers and categorize them
        // as active or stale based on their capsule internal state->
        AutoFree< ArrayOf<Int32> > uidList = ArrayOf<Int32>::Alloc(list->GetLength());
        if (uidList == NULL) return E_OUT_OF_MEMORY_ERROR;
        Int32 num = 0;
        {
            Mutex::Autolock lock(mCapsulesLock);

            for (Int32 i = 0; i < list->GetLength(); i++) {
                String cid = (*list)[i];
                SdInstallArgs* args = new SdInstallArgs(cid);
                if (!args) return E_OUT_OF_MEMORY_ERROR;
                if (DEBUG_SD_INSTALL) {
                    Logger::I(TAG, StringBuffer("Processing container ") + cid);
                }
                AutoString capName;
                args->GetCapsuleName(&capName);
                if (capName.IsNull()) {
                    if (DEBUG_SD_INSTALL) {
                        Logger::I(TAG, StringBuffer("Container : ") + cid + " stale");
                    }
                    removeCids.PushBack(cid);
                    continue;
                }
                if (DEBUG_SD_INSTALL) {
                    Logger::I(TAG, StringBuffer("Looking for cap : ") + capName);
                }
                HashMap<String, CapsuleSetting*>::Iterator it
                                        = mSettings->mCapsules.Find(capName);
                CapsuleSetting* cs = it->mSecond;
                // The capsule status is changed only if the code path
                // matches between settings and the container id.
                String path = args->GetCodePath();
                if (cs != NULL && !cs->mCodePathString.IsNull()
                    && cs->mCodePathString.Equals(path)) {
                    if (DEBUG_SD_INSTALL) {
                        Logger::I(TAG, StringBuffer("Container : ")
                            + cid + " corresponds to cap : " + capName
                            + " at code path: " + cs->mCodePathString);
                    }
                    // We do have a valid capsule installed on sdcard
                    // processCids->Put(args, cs->mCodePathString);
                    processCids[cs->mCodePathString] = args;
                    Int32 uid = cs->mUserId;
                    if (uid != -1) {
                        (*uidList)[num++] = uid;
                    }
                } else {
                    // Stale container on sdcard-> Just delete
                    if (DEBUG_SD_INSTALL) {
                        Logger::I(TAG, StringBuffer("Container : ") + cid + " stale");
                    }
                    removeCids.PushBack(cid);
                }
            }
        }

        if (num > 0) {
            // Sort uid list
//	            Arrays::Sort(uidList, 0, num);
            // Throw away duplicates
            uidArr = ArrayOf<Int32>::Alloc(num);
            if (uidArr == NULL) return E_OUT_OF_MEMORY_ERROR;
            (*uidArr)[0] = (*uidList)[0];
            Int32 di = 0;
            for (Int32 i = 1; i < num; i++) {
                if ((*uidList)[i-1] != (*uidList)[i]) {
                    (*uidArr)[di++] = (*uidList)[i];
                }
            }
        }
    }

    // Process capsules with valid entries->
    if (mediaStatus) {
        if (DEBUG_SD_INSTALL) Logger::I(TAG, "Loading capsules");
        LoadMediaCapsules(processCids, *uidArr, removeCids);
        StartCleaningCapsules();
    } else {
        if (DEBUG_SD_INSTALL) Logger::I(TAG, "Unloading capsules");
        UnloadMediaCapsules(processCids, *uidArr, reportStatus);
    }

    return NOERROR;
}

ECode CCapsuleManagerService::LoadMediaCapsules(
    /* [in] */ HashMap<String, SdInstallArgs*>& processCids,
    /* [in] */ const ArrayOf<Int32>& uidArr,
    /* [in] */ List<String>& removeCids)
{
    List<String> capList;
    HashMap<String, SdInstallArgs*>::Iterator it;
    Boolean doGc = FALSE;
    for (it = processCids.Begin(); it != processCids.End(); it++) {
        SdInstallArgs* args = it->mSecond;

        String codePath = it->mFirst;
        if (DEBUG_SD_INSTALL) {
            Logger::I(TAG, StringBuffer("Loading container : ") + args->mCid);
        }
        Int32 retCode = CapsuleManager::INSTALL_FAILED_CONTAINER_ERROR;
        // Make sure there are no container errors first.
        if (args->DoPreInstall(CapsuleManager::INSTALL_SUCCEEDED)
            != CapsuleManager::INSTALL_SUCCEEDED) {
            Slogger::E(TAG, StringBuffer("Failed to mount cid : ")
                + args->mCid + " when installing from sdcard");
           continue;
        }
        // Check code path here.
        if (codePath.IsNull() || !codePath.Equals(args->GetCodePath())) {
            Slogger::E(TAG, StringBuffer("Container ")
                + args->mCid + " cachepath " + args->GetCodePath()
                + " does not match one in settings " + codePath);
           continue;
        }
        // Parse capsule
        Int32 parseFlags = CapsuleParser::PARSE_ON_SDCARD | mDefParseFlags;
        doGc = TRUE;
        {
            Mutex::Autolock lock(mInstallLock);

            AutoPtr<IFile> file;
            FAIL_RETURN(CFile::New(codePath, (IFile**)&file));
            CapsuleParser::Capsule* cap = ScanCapsuleLI(file.Get(), parseFlags, 0, 0);
            // Scan the capsule
            if (cap != NULL) {
                Mutex::Autolock l(mCapsulesLock);

                retCode = CapsuleManager::INSTALL_SUCCEEDED;
                capList.PushBack(cap->mCapsuleName);
                // Post process args
                args->DoPostInstall(CapsuleManager::INSTALL_SUCCEEDED);
            } else {
                Slogger::I(TAG, StringBuffer("Failed to install cap from  ")
                    + codePath + " from sdcard");
            }
        }
        if (retCode != CapsuleManager::INSTALL_SUCCEEDED) {
            // Don't destroy container here-> Wait till gc clears things up.
            removeCids.PushBack(args->mCid);
        }
    }
    {
        Mutex::Autolock lock(mCapsulesLock);

        // If the platform SDK has changed since the last time we booted,
        // we need to re-grant app permission to catch any new ones that
        // appear->  This is really a hack, and means that apps can in some
        // cases get permissions that the user didn't initially explicitly
        // allow->->->  it would be nice to have some better way to handle
        // this situation->
        Boolean regrantPermissions = mSettings->mExternalSdkPlatform != mSdkVersion;
        if (regrantPermissions) {
            Slogger::I(TAG, StringBuffer("Platform changed from ")
                + mSettings->mExternalSdkPlatform + " to " + mSdkVersion
                + "; regranting permissions for external storage");
        }
        mSettings->mExternalSdkPlatform = mSdkVersion;

        // Make sure group IDs have been assigned, and any permission
        // changes in other apps are accounted for
        UpdatePermissionsLP(NULL, NULL, TRUE, regrantPermissions, regrantPermissions);
        // Persist settings
        mSettings->WriteLP();
    }
    // Send a broadcast to let everyone know we are done processing
    if (capList.GetSize() > 0) {
       SendResourcesChangedBroadcast(TRUE, capList, uidArr, NULL);
    }
    // Force gc to avoid any stale parser references that we might have->
//	    if (doGc) {
//	       Runtime::GetRuntime()->Gc();
//	    }
    // List stale containers and destroy stale temporary containers->
    List<String>::Iterator rcIt;
    for (rcIt = removeCids.Begin(); rcIt != removeCids.End(); rcIt++) {
        String cid = *rcIt;
        StringBuf_<16> subStr;
        cid.Substring(0, mTempContainerPrefix.GetLength(), subStr);
        if ((String((const char*)subStr)).Equals(mTempContainerPrefix)) {
             Logger::I(TAG, StringBuffer("Destroying stale temporary container ") + cid);
             CapsuleHelper::DestroySdDir(cid);
        } else {
             Logger::W(TAG, StringBuffer("Container ") + cid + " is stale");
        }
    }
    return NOERROR;
}

void CCapsuleManagerService::SendResourcesChangedBroadcast(
    /* [in] */ Boolean mediaStatus,
    /* [in] */ List<String>& capList,
    /* [in] */ const ArrayOf<Int32>& uidArr,
    /* [in] */ IIntentReceiver* finishedReceiver)
{
    Int32 size = capList.GetSize();
    if (size > 0) {
        AutoStringArray caps = ArrayOf<String>::Alloc(size);
        assert(caps != NULL);
        Int32 i = 0;
        List<String>::Iterator it;
        for (it = capList.Begin(); it != capList.End(); it++) {
            (*caps)[i++] = String::Duplicate(*it);
        }
        // Send broadcasts here
        AutoPtr<IBundle> extras; // = new Bundle();
        ASSERT_SUCCEEDED(CBundle::New((IBundle**)&extras));
        extras->PutStringArray(Intent_EXTRA_CHANGED_CAPSULE_LIST, *caps);
        extras->PutInt32Array(Intent_EXTRA_CHANGED_UID_LIST, uidArr);
        String action = mediaStatus
            ? Intent_ACTION_EXTERNAL_APPLICATIONS_AVAILABLE
            : Intent_ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE;
        SendCapsuleBroadcast(action, NULL, extras.Get(), finishedReceiver);
    }
}

ECode CCapsuleManagerService::UnloadMediaCapsules(
    /* [in] */ HashMap<String, SdInstallArgs*>& processCids,
    /* [in] */ const ArrayOf<Int32>& uidArr,
    /* [in] */ Boolean reportStatus)
{
    if (DEBUG_SD_INSTALL) Logger::I(TAG, "unloading media capsules");
    List<String> capList;
    List<SdInstallArgs*> failedList;
    List<SdInstallArgs*>* keys = new List<SdInstallArgs*>();
    HashMap<String, SdInstallArgs*>::Iterator it;
    for (it = processCids.Begin(); it != processCids.End(); it++) {
        SdInstallArgs* args = it->mSecond;

        keys->PushBack(args);

        String cid = args->mCid;
        AutoString capName;
        args->GetCapsuleName(&capName);
        if (DEBUG_SD_INSTALL) {
            Logger::I(TAG, StringBuffer("Trying to unload cap : ") + capName);
        }
        // Delete capsule internally
        CapsuleRemovedInfo* outInfo = new CapsuleRemovedInfo();
        assert(outInfo);
        {
            Mutex::Autolock lock(mInstallLock);
            Boolean res = DeleteCapsuleLI(
                capName, FALSE, CapsuleManager_DONT_DELETE_DATA, outInfo, FALSE);
            if (res) {
                capList.PushBack(capName);
            } else {
                Slogger::E(TAG,
                    StringBuffer("Failed to delete cap from sdcard : ")
                    + capName);
                failedList.PushBack(args);
            }
        }
    }

    {
        Mutex::Autolock lock(mCapsulesLock);
        // We didn't update the settings after removing each capsule;
        // write them now for all capsules.
        mSettings->WriteLP();
    }

    // We have to absolutely send UPDATED_MEDIA_STATUS only
    // after confirming that all the receivers processed the ordered
    // broadcast when capsules get disabled, force a gc to clean things up.
    // and unload all the containers.
    if (capList.GetSize() > 0) {
        SendResourcesChangedBroadcast(FALSE, capList, uidArr, NULL /*new IIntentReceiver::Stub() {
            public void PerformReceive(Intent intent, Int32 resultCode, String data, Bundle extras,
                    Boolean ordered, Boolean sticky) throws RemoteException {
                Message msg = mHandler->ObtainMessage(UPDATED_MEDIA_STATUS,
                        reportStatus ? 1 : 0, 1, keys);
                mHandler->SendMessage(msg);
            }
        }*/);
    } else {
        Message* msg = new Message();
        msg->mWhat = UPDATED_MEDIA_STATUS;
        msg->mArg1 = reportStatus ? 1 : 0;
        msg->mArg2 = (Int32)keys;
        mHandler->SendMessage(msg);
    }
    return NOERROR;
}

ECode CCapsuleManagerService::NextCapsuleToClean(
    /* [in] */ String lastCapsule,
    /* [out] */ String* nextCapsule)
{
    VALIDATE_NOT_NULL(nextCapsule);

    Mutex::Autolock lock(mCapsulesLock);

    if (!mMediaMounted) {
        // If the external storage is no longer mounted at this point,
        // the caller may not have been able to delete all of this
        // packages files and can not delete any more.  Bail.
        *nextCapsule = NULL;
        return E_DOES_NOT_EXIST;
    }
    if (!lastCapsule.IsNull()) {
        List<String>::Iterator it;
        for (it = mSettings->mCapsulesToBeCleaned.Begin();
            it != mSettings->mCapsulesToBeCleaned.End(); it++) {
            if ((*it).Equals(lastCapsule)) {
                mSettings->mCapsulesToBeCleaned.Erase(it);
                break;
            }
        }
    }
    if (mSettings->mCapsulesToBeCleaned.GetSize() == 0) {
        *nextCapsule = NULL;
        return E_DOES_NOT_EXIST;
    }
    *nextCapsule = String::Duplicate(*mSettings->mCapsulesToBeCleaned.Begin());
    return NOERROR;
}

void CCapsuleManagerService::UnloadAllContainers(
    /* [in] */ Set<SdInstallArgs*>& cidArgs)
{
    // Just unmount all valid containers.
    Set<SdInstallArgs*>::Iterator it;
    for (it = cidArgs.Begin(); it != cidArgs.End(); it++) {
        SdInstallArgs* arg = *it;
        Mutex::Autolock lock(mInstallLock);
        arg->DoPostDeleteLI(FALSE);
    }
}

ECode CCapsuleManagerService::MoveCapsule(
    /* [in] */ String capsuleName,
    /* [in] */ ICapsuleMoveObserver* observer,
    /* [in] */ Int32 flags)
{
    mContext->EnforceCallingOrSelfPermission(
           "" /*android.Manifest.permission.MOVE_CAPSULE*/, NULL);
    Int32 returnCode = CapsuleManager_MOVE_SUCCEEDED;
    Int32 currFlags = 0;
    Int32 newFlags = 0;
    {
        Mutex::Autolock lock(mCapsulesLock);

        HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(capsuleName);
        CapsuleParser::Capsule* cap = it->mSecond;
        if (cap == NULL) {
            returnCode =  CapsuleManager_MOVE_FAILED_DOESNT_EXIST;
        } else {
            // Disable moving fwd locked apps and system capsules
            if (cap->mApplicationInfo != NULL && IsSystemApp(cap)) {
                Slogger::W(TAG, "Cannot move system application");
                returnCode = CapsuleManager_MOVE_FAILED_SYSTEM_CAPSULE;
            } else if (cap->mApplicationInfo != NULL && IsForwardLocked(cap)) {
                Slogger::W(TAG, "Cannot move forward locked app->");
                returnCode = CapsuleManager_MOVE_FAILED_FORWARD_LOCKED;
            } else if (cap->mOperationPending) {
                Slogger::W(TAG, "Attempt to move capsule which has pending operations");
                returnCode = CapsuleManager_MOVE_FAILED_OPERATION_PENDING;
            } else {
                // Find install location first
                if ((flags & CapsuleManager_MOVE_EXTERNAL_MEDIA) != 0
                    && (flags & CapsuleManager_MOVE_INTERNAL) != 0) {
                    Slogger::W(TAG, "Ambigous flags specified for move location.");
                    returnCode = CapsuleManager_MOVE_FAILED_INVALID_LOCATION;
                } else {
                    newFlags = (flags & CapsuleManager_MOVE_EXTERNAL_MEDIA) != 0
                        ? CapsuleManager::INSTALL_EXTERNAL
                        : CapsuleManager::INSTALL_INTERNAL;
                    currFlags = IsExternal(cap)
                        ? CapsuleManager::INSTALL_EXTERNAL
                        : CapsuleManager::INSTALL_INTERNAL;
                    if (newFlags == currFlags) {
                        Slogger::W(TAG, "No move required-> Trying to move to same location");
                        returnCode = CapsuleManager_MOVE_FAILED_INVALID_LOCATION;
                    }
                }
                if (returnCode == CapsuleManager_MOVE_SUCCEEDED) {
                    cap->mOperationPending = TRUE;
                }
            }
        }
        if (returnCode != CapsuleManager_MOVE_SUCCEEDED) {
            MoveParams* params = new MoveParams(NULL, observer, 0, capsuleName, NULL);
            ProcessPendingMove(params, returnCode);
        } else {
            Message* msg = new Message(); // mHandler->ObtainMessage(INIT_COPY);
            msg->mWhat = INIT_COPY;
            InstallArgs* srcArgs;
            CreateInstallArgs(currFlags,
                cap->mApplicationInfo->mSourceDir,
                cap->mApplicationInfo->mPublicSourceDir,
                cap->mApplicationInfo->mNativeLibraryDir, &srcArgs);
            MoveParams* mp = new MoveParams(
                srcArgs, observer, newFlags, capsuleName,
                cap->mApplicationInfo->mDataDir);
            msg->mObj = mp;
            mHandler->SendMessage(msg);
        }
    }
    return NOERROR;
}

void CCapsuleManagerService::HandleProcessPendingMove(
    /* [in] */ MoveParams* mp,
    /* [in] */ Int32 currentStatus)
{
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(MoveParams*, Int32);
    pHandlerFunc = &CCapsuleManagerService::HandleProcessPendingMove;
    mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    Int32 returnCode = currentStatus;
    if (currentStatus == CapsuleManager_MOVE_SUCCEEDED) {
        AutoFree< ArrayOf<Int32> > uidArr;
        List<String> capList;

        {
            Mutex::Autolock lock(mCapsulesLock);

            HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(mp->mCapsuleName);
            CapsuleParser::Capsule* cap = it->mSecond;
            if (cap == NULL) {
                Slogger::W(TAG, StringBuffer(" Capsule ")
                    + mp->mCapsuleName + " doesn't exist-> Aborting move");
                returnCode = CapsuleManager_MOVE_FAILED_DOESNT_EXIST;
            }
            else if (!mp->mSrcArgs->GetCodePath().Equals(cap->mApplicationInfo->mSourceDir)) {
                Slogger::W(TAG, StringBuffer("Capsule ")
                    + mp->mCapsuleName + " code path changed from "
                    + mp->mSrcArgs->GetCodePath() + " to "
                    + cap->mApplicationInfo->mSourceDir
                    + " Aborting move and returning error");
                returnCode = CapsuleManager_MOVE_FAILED_INTERNAL_ERROR;
            }
            else {
                uidArr = ArrayOf<Int32>::Alloc(1);
                (*uidArr)[0] = cap->mApplicationInfo->mUid;
                capList.PushBack(mp->mCapsuleName);
            }
        }
        if (returnCode == CapsuleManager_MOVE_SUCCEEDED) {
            // Send resources unavailable broadcast
            SendResourcesChangedBroadcast(FALSE, capList, *uidArr, NULL);
            // Update capsule code and resource paths
            {
                Mutex::Autolock installLock(mInstallLock);

                {
                    Mutex::Autolock installLock(mCapsulesLock);

                    HashMap<String, CapsuleParser::Capsule*>::Iterator it =
                                            mCapsules.Find(mp->mCapsuleName);
                    CapsuleParser::Capsule* cap = it->mSecond;
                    // Recheck for capsule again.
                    if (cap == NULL) {
                         Slogger::W(TAG, StringBuffer(" Capsule ")
                            + mp->mCapsuleName + " doesn't exist-> Aborting move");
                         returnCode = CapsuleManager_MOVE_FAILED_DOESNT_EXIST;
                    }
                    else if (!mp->mSrcArgs->GetCodePath().Equals(cap->mApplicationInfo->mSourceDir)) {
                        Slogger::W(TAG, StringBuffer("Capsule ")
                            + mp->mCapsuleName + " code path changed from "
                            + mp->mSrcArgs->GetCodePath() + " to "
                            + cap->mApplicationInfo->mSourceDir
                            + " Aborting move and returning error");
                        returnCode = CapsuleManager_MOVE_FAILED_INTERNAL_ERROR;
                    }
                    else {
                        String oldCodePath = cap->mPath;
                        String newCodePath = mp->mTargetArgs->GetCodePath();
                        String newResPath = mp->mTargetArgs->GetResourcePath();
                        String newNativePath = mp->mTargetArgs->GetNativeLibraryPath();

                        if ((mp->mFlags & CapsuleManager::INSTALL_EXTERNAL) == 0) {
                            if (mInstaller->UnlinkNativeLibraryDirectory(cap->mApplicationInfo->mDataDir) < 0) {
                                returnCode = CapsuleManager_MOVE_FAILED_INSUFFICIENT_STORAGE;
                            }
                            else {
                                AutoPtr<IFile> codeFile, nativeFile;
                                CFile::New(newCodePath, (IFile**)&codeFile);
                                CFile::New(newNativePath, (IFile**)&nativeFile);
                                NativeLibraryHelper::CopyNativeBinariesLI(codeFile.Get(), nativeFile.Get());
                            }
                        }
                        else {
                            if (mInstaller->LinkNativeLibraryDirectory(
                                cap->mApplicationInfo->mDataDir, newNativePath) < 0) {
                                returnCode = CapsuleManager_MOVE_FAILED_INSUFFICIENT_STORAGE;
                            }
                        }

                        if (returnCode == CapsuleManager_MOVE_SUCCEEDED) {
                            cap->mPath = String::Duplicate(newCodePath);
                            // Move dex files around
//	                            if (MoveDexFilesLI(cap) != CapsuleManager::INSTALL_SUCCEEDED) {
//	                                // Moving of dex files failed-> Set
//	                                // error code and abort move->
//	                                cap->mPath = cap->mScanPath;
//	                                returnCode = CapsuleManager::MOVE_FAILED_INSUFFICIENT_STORAGE;
//	                            }
                        }

                        if (returnCode == CapsuleManager_MOVE_SUCCEEDED) {
                            cap->mScanPath = String::Duplicate(newCodePath);
                            cap->mApplicationInfo->mSourceDir = String::Duplicate(newCodePath);
                            cap->mApplicationInfo->mPublicSourceDir = String::Duplicate(newResPath);
                            cap->mApplicationInfo->mNativeLibraryDir = String::Duplicate(newNativePath);
                            CapsuleSetting* ps = (CapsuleSetting*)cap->mExtras;
                            CFile::New(cap->mApplicationInfo->mSourceDir, (IFile**)&ps->mCodePath);
                            ps->mCodePath->GetPath(&ps->mCodePathString);
                            CFile::New(cap->mApplicationInfo->mPublicSourceDir, (IFile**)&ps->mResourcePath);
                            ps->mResourcePath->GetPath(&ps->mResourcePathString);
                            ps->mNativeLibraryPathString = String::Duplicate(newNativePath);
                            // Set the application info flag correctly.
                            if ((mp->mFlags & CapsuleManager::INSTALL_EXTERNAL) != 0) {
                                cap->mApplicationInfo->mFlags |= CApplicationInfo::FLAG_EXTERNAL_STORAGE;
                            }
                            else {
                                cap->mApplicationInfo->mFlags &= ~CApplicationInfo::FLAG_EXTERNAL_STORAGE;
                            }
                            ps->SetFlags(cap->mApplicationInfo->mFlags);
                            mAppDirs.Erase(oldCodePath);
                            mAppDirs[newCodePath] = cap;
                            // Persist settings
                            mSettings->WriteLP();
                        }
                    }
                }
            }
            // Send resources available broadcast
            SendResourcesChangedBroadcast(TRUE, capList, *uidArr, NULL);
        }
    }
    if (returnCode != CapsuleManager_MOVE_SUCCEEDED){
        // Clean up failed installation
        if (mp->mTargetArgs != NULL) {
            mp->mTargetArgs->DoPostInstall(CapsuleManager::INSTALL_FAILED_INTERNAL_ERROR);
        }
    } else {
        // Force a gc to clear things up->
//	        Runtime::GetRuntime()->Gc();
        // Delete older code
        Mutex::Autolock lock(mInstallLock);
        mp->mSrcArgs->DoPostDeleteLI(TRUE);
    }

    // Allow more operations on this file if we didn't fail because
    // an operation was already pending for this capsule->
    if (returnCode != CapsuleManager_MOVE_FAILED_OPERATION_PENDING) {
        Mutex::Autolock lock(mCapsulesLock);
        HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(mp->mCapsuleName);
        CapsuleParser::Capsule* cap = it->mSecond;
        if (cap != NULL) {
            cap->mOperationPending = FALSE;
        }
    }

    AutoPtr<ICapsuleMoveObserver> observer = mp->mObserver;
    if (observer != NULL) {
       if (FAILED(observer->CapsuleMoved(mp->mCapsuleName, returnCode))) {
           Logger::I(TAG, "Observer no Int64er exists.");
       }
    }
}

void CCapsuleManagerService::ProcessPendingMove(
    /* [in] */ MoveParams* mp,
    /* [in] */ Int32 currentStatus)
{
    // Queue up an async operation since the package deletion may take a little while.
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(MoveParams*, Int32);
    pHandlerFunc = &CCapsuleManagerService::HandleProcessPendingMove;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)mp);
    params->WriteInt32(currentStatus);
    mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params.Get());
}

ECode CCapsuleManagerService::AddPermissionAsync(
    /* [in] */ IPermissionInfo* info,
    /* [out] */ Boolean* isAdded)
{
    VALIDATE_NOT_NULL(info);
    VALIDATE_NOT_NULL(isAdded);
    Mutex::Autolock lock(mCapsulesLock);
    return AddPermissionLocked(info, TRUE, isAdded);
}

ECode CCapsuleManagerService::AddPermissionLocked(
    /* [in] */ IPermissionInfo* info,
    /* [in] */ Boolean async,
    /* [out] */ Boolean* isAdded)
{
    assert(info);
    assert(isAdded);

    AutoPtr<CPermissionInfo> cinfo = (CPermissionInfo*)info;
    if (cinfo->mLabelRes == 0 && cinfo->mNonLocalizedLabel == NULL) {
        Logger::E(TAG, "Label must be specified in permission");
        *isAdded = FALSE;
        return E_SECURITY_EXCEPTION;
    }
    BasePermission* tree = NULL;
    CheckPermissionTreeLP(cinfo->mName, &tree);
    HashMap<String, BasePermission*>::Iterator it =
            mSettings->mPermissions.Find(cinfo->mName);
    BasePermission* bp = it->mSecond;
    Boolean added = bp == NULL;
    Boolean changed = TRUE;
    if (added) {
        bp = new BasePermission(
            cinfo->mName, tree->mSourceCapsule, BasePermission::TYPE_DYNAMIC);
    }
    else if (bp->mType != BasePermission::TYPE_DYNAMIC) {
        Logger::E(TAG,
            StringBuffer("Not allowed to modify non-dynamic permission ")
            + cinfo->mName);
        *isAdded = FALSE;
        return E_SECURITY_EXCEPTION;
    }
    else {
        if (bp->mProtectionLevel == cinfo->mProtectionLevel
            && bp->mPerm->mOwner == tree->mPerm->mOwner
            && bp->mUid == tree->mUid
            && ComparePermissionInfos(bp->mPerm->mInfo.Get(), info)) {
            changed = FALSE;
        }
    }
    bp->mProtectionLevel = cinfo->mProtectionLevel;
    AutoPtr<IPermissionInfo> pinfo;
    ASSERT_SUCCEEDED(CPermissionInfo::New(info, (IPermissionInfo**)&pinfo));
    bp->mPerm = new CapsuleParser::Permission(tree->mPerm->mOwner, pinfo.Get());
    bp->mPerm->mInfo->mCapsuleName = tree->mPerm->mInfo->mCapsuleName;
    bp->mUid = tree->mUid;
    if (added) {
        mSettings->mPermissions[cinfo->mName] = bp;
    }
    if (changed) {
        if (!async) {
            mSettings->WriteLP();
        } else {
            ScheduleWriteSettingsLocked();
        }
    }
    *isAdded = added;
    return NOERROR;
}

ECode CCapsuleManagerService::CheckPermissionTreeLP(
    /* [in] */ String permName,
    /* [out] */ BasePermission** basePerm)
{
    assert(basePerm);

    if (!permName.IsNull()) {
        BasePermission* bp = NULL;
        FAIL_RETURN(FindPermissionTreeLP(permName, &bp));
        if (bp != NULL) {
            if (TRUE /*bp->mUid == Binder::GetCallingUid()*/) {
                *basePerm = bp;
                return NOERROR;
            }
            Logger::E(TAG, StringBuffer("Calling uid ")
                /*+ Binder::GetCallingUid()*/
                + " is not allowed to add to permission tree "
                + bp->mName + " owned by uid " + bp->mUid);
        }
    }
    Logger::E(TAG, StringBuffer("No permission tree found for ") + permName);
    return E_SECURITY_EXCEPTION;
}

ECode CCapsuleManagerService::FindPermissionTreeLP(
    /* [in] */ String permName,
    /* [out] */ BasePermission** basePerm)
{
    VALIDATE_STRING_NOT_NULL(permName);
    assert(basePerm);

    StringBuf* buf = StringBuf::Alloc(permName.GetLength() + 1);
    if (!buf) return E_OUT_OF_MEMORY_ERROR;

    HashMap<String, BasePermission*>::Iterator it;
    for (it = mSettings->mPermissionTrees.Begin();
         it != mSettings->mPermissionTrees.End();
         it++) {
        BasePermission* bp = it->mSecond;

        if (bp->mName.IsNull()
            || permName.GetLength() < bp->mName.GetLength()) {
            continue;
        }

        permName.Substring(0, bp->mName.GetLength(), *buf);

        if (String((const char*)(*buf)).Equals(bp->mName)
            && permName.GetChar(bp->mName.GetLength()) == '.') {
            *basePerm = bp;
            return NOERROR;
        }
    }
    return E_DOES_NOT_EXIST;
}

Boolean CCapsuleManagerService::ComparePermissionInfos(
    /* [in] */ IPermissionInfo* pi1,
    /* [in] */ IPermissionInfo* pi2)
{
    assert(pi1);
    assert(pi2);

    AutoPtr<CPermissionInfo> cpi1 = (CPermissionInfo*)pi1;
    AutoPtr<CPermissionInfo> cpi2 = (CPermissionInfo*)pi2;

    if (cpi1->mIcon != cpi2->mIcon) return FALSE;
    if (cpi1->mLogo != cpi2->mLogo) return FALSE;
    if (cpi1->mProtectionLevel != cpi2->mProtectionLevel) return FALSE;
    if (!CompareStrings(cpi1->mName, cpi2->mName)) return FALSE;
    AutoString str1, str2;
    cpi1->mNonLocalizedLabel->ToString(&str1);
    cpi2->mNonLocalizedLabel->ToString(&str2);
    if (!CompareStrings(str1, str2)) return FALSE;
    // We'll take care of setting this one.
    if (!CompareStrings(cpi1->mCapsuleName, cpi2->mCapsuleName)) return FALSE;
    // These are not currently stored in settings.
    //if (!CompareStrings(pi1->mGroup, pi2->mGroup)) return FALSE;
    //if (!CompareStrings(pi1->mNonLocalizedDescription, pi2->mNonLocalizedDescription)) return FALSE;
    //if (pi1->mLabelRes != pi2->mLabelRes) return FALSE;
    //if (pi1->mDescriptionRes != pi2->mDescriptionRes) return FALSE;
    return TRUE;
}

Boolean CCapsuleManagerService::CompareStrings(
    /* [in] */ String s1,
    /* [in] */ String s2)
{
    if (s1.IsNull()) {
        return s2.IsNull();
    }
    if (s2.IsNull()) {
        return FALSE;
    }
    return s1.Equals(s2);
}

void CCapsuleManagerService::ScheduleWriteSettingsLocked()
{
//	    if (!mHandler.hasMessages(WRITE_SETTINGS)) {
//	        mHandler.sendEmptyMessageDelayed(WRITE_SETTINGS, WRITE_SETTINGS_DELAY);
//	    }
}

ECode CCapsuleManagerService::SetInstallLocation(
    /* [in] */ Int32 loc,
    /* [out] */ Boolean* isSetted)
{
    VALIDATE_NOT_NULL(isSetted);

    mContext->EnforceCallingOrSelfPermission(
        "" /*android.Manifest.permission.WRITE_SECURE_SETTINGS*/, NULL);
    Int32 location = 0;
    GetInstallLocation(&location);
    if (location == loc) {
        *isSetted = TRUE;
        return NOERROR;
    }
    if (loc == CapsuleHelper::APP_INSTALL_AUTO
        || loc == CapsuleHelper::APP_INSTALL_INTERNAL
        || loc == CapsuleHelper::APP_INSTALL_EXTERNAL) {
//	        android.provider.Settings.System.PutInt(mContext.GetContentResolver(),
//	                android.provider.Settings.Secure.DEFAULT_INSTALL_LOCATION, loc);
        *isSetted = TRUE;
        return NOERROR;
    }
    *isSetted = FALSE;
    return NOERROR;
}

ECode CCapsuleManagerService::GetInstallLocation(
    /* [out] */ Int32* loc)
{
//	    return android.provider.Settings.System.getInt(mContext.getContentResolver(),
//	        android.provider.Settings.Secure.DEFAULT_INSTALL_LOCATION, PackageHelper.APP_INSTALL_AUTO);
    return E_NOT_IMPLEMENTED;
}

//todo: this constructor should be removed.
ECode CCapsuleManagerService::constructor()
{
    FAIL_RETURN(CApartment::New(FALSE, (IApartment**)&mApartment));
    mApartment->Start(ApartmentAttr_New);
    mHandler = new CapsuleHandler(this);
    return NOERROR;
}

void CCapsuleManagerService::ScanDir(
    /* [in] */ String path)
{
    DIR* dir;
    struct dirent* dp;
    struct stat fs;
    char fname[512];
    CapsuleParser::Capsule* capsule;

    dir = opendir(path);
    if (dir == NULL) {
        if (DEBUG_INSTALL) {
            Slogger::E(TAG, "Fail to open directory");
        }
        return;
    }

    do {
        if ((dp = readdir(dir)) != NULL) {
            stat(dp->d_name, &fs);
            if (S_ISDIR(fs.st_mode)) {
                strcpy(fname, path);
                strcat(fname, "/");
                strcat(fname, dp->d_name);
                strcat(fname, "/data/");
                strcat(fname, (const char*)DEFAULT_RESOURCES_FILE_NAME);
                if (!access(fname, R_OK)) {
                    capsule = ScanCapsule(String(fname));
                }
            }
        }
    }while(dp != NULL);

    closedir(dir);
}

CapsuleParser::Capsule* CCapsuleManagerService::ScanCapsule(
    /* [in] */ String capfile)
{
    StringBuf_<512> buf;
    String dir;
    Int32 len;
    CapsuleParser p;
    AutoString err;
    CapsuleParser::Capsule* capsule;

    len = capfile.GetLength();
    dir = capfile.Substring(0, len - strlen(DEFAULT_RESOURCES_FILE_NAME) - 1, buf);
    capsule = new CapsuleParser::Capsule();
    assert(capsule != NULL);
    capsule->mApplicationInfo->SetSourceDir(dir);
    if (SUCCEEDED(p.ParseCapsule(capfile, capsule, &err))) {
        mCapsules[String::Duplicate(
                capsule->mApplicationInfo->mCapsuleName)] = capsule;

#ifdef _HAS_PARSED_MORE // TODO: temporary comment.
        //ContentProvider
        List<CapsuleParser::ContentProvider*>::Iterator
                cit = capsule->mProviders.Begin();
        for (; cit != capsule->mProviders.End(); ++cit) {
            CapsuleParser::ContentProvider* p = *cit;
            p->mInfo->mProcessName = FixProcessName(
                    capsule->mApplicationInfo->mProcessName,
                    (*cit)->mInfo->mProcessName, -1 /*capsule->mApplicationInfo->mUid*/);
            char* strKey = (char*)malloc(p->mInfo->mCapsuleName.GetLength() +
                    p->mInfo->mName.GetLength() + 2);
            assert(strKey != NULL);
            strcpy(strKey, (const char*)p->mInfo->mCapsuleName);
            strcat(strKey, ".");
            strcat(strKey, (const char*)p->mInfo->mName);
            mProvidersByComponent[String(strKey)] = p;
            p->mSyncable = p->mInfo->mIsSyncable;
            if (!p->mInfo->mAuthority.IsNull()) {
                String authority = p->mInfo->mAuthority;
                p->mInfo->mAuthority = NULL;
                StringTokenizer* names = new StringTokenizer(
                        authority, String(";"));
                while(names->HasMoreTokens()) {
                    String token = names->NextToken();
//                    if (j == 1 && p.syncable) {
//                        // We only want the first authority for a provider to possibly be
//                        // syncable, so if we already added this provider using a different
//                        // authority clear the syncable flag. We copy the provider before
//                        // changing it because the mProviders object contains a reference
//                        // to a provider that we don't want to change.
//                        // Only do this for the second authority since the resulting provider
//                        // object can be the same for all future authorities for this provider.
//                        p = new PackageParser.Provider(p);
//                        p.syncable = false;
//                    }

                    if (mProviders[token] == NULL) {
                        mProviders[String::Duplicate(token)] = p;
                        if (p->mInfo->mAuthority.IsNull()) {
                            p->mInfo->mAuthority = String::Duplicate(token);
                        }
                        else {
                            char* auth = (char*)malloc(
                                    p->mInfo->mAuthority.GetLength() + 1 /*";"*/ +
                                    token.GetLength() + 1);
                            strcpy(auth, (const char*)p->mInfo->mAuthority);
                            strcat(auth, ";");
                            strcat(auth, token);
                            String::Free(p->mInfo->mAuthority);
                            p->mInfo->mAuthority = String(auth);
                        }
//                        if ((parseFlags&PackageParser.PARSE_CHATTY) != 0 && Config.LOGD)
//                                Slog.d(TAG, "Registered content provider: " + names[j] +
//                                ", className = " + p.info.name +
//                                ", isSyncable = " + p.info.isSyncable);
                    }
                    else {
                        CapsuleParser::ContentProvider* other = mProviders[token];
                        UNUSED(other);
//                        Slog.w(TAG, "Skipping provider name " + names[j] +
//                                    " (in package " + pkg.applicationInfo.packageName +
//                                    "): name already used by "
//                                    + ((other != null && other.getComponentName() != null)
//                                            ? other.getComponentName().getPackageName() : "?"));
                    }
                }
            }
//            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//                if (r == null) {
//                    r = new StringBuilder(256);
//                } else {
//                    r.append(' ');
//                }
//                r.append(p.info.name);
//            }
        }
//        if (r != null) {
//            if (Config.LOGD) Slog.d(TAG, "  Providers: " + r);
//        }

        //BroadcastReceiver
        List<CapsuleParser::Activity*>::Iterator bit1 = capsule->mReceivers.Begin();
        List<CapsuleParser::Activity*>::Iterator bit2 = capsule->mReceivers.End();
//        r = null;
        for (; bit1 != bit2; ++bit1) {
            (*bit1)->mInfo->mProcessName = FixProcessName(
                    capsule->mApplicationInfo->mProcessName,
                    (*bit1)->mInfo->mProcessName, -1 /*capsule->mApplicationInfo->mUid*/);
            mReceivers->AddActivity(*bit1, "receiver");
//            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//                if (r == null) {
//                    r = new StringBuilder(256);
//                } else {
//                    r.append(' ');
//                }
//                r.append(a.info.name);
//            }
        }
//        if (r != null) {
//            if (Config.LOGD) Slog.d(TAG, "  Receivers: " + r);
//        }
#endif

        //Activity
        List<CapsuleParser::Activity*>::Iterator ait1 = capsule->mActivities.Begin();
        List<CapsuleParser::Activity*>::Iterator ait2 = capsule->mActivities.End();
//        r = null;
        for (; ait1 != ait2; ++ait1) {
            (*ait1)->mInfo->mProcessName = FixProcessName(
                    capsule->mApplicationInfo->mProcessName,
                    (*ait1)->mInfo->mProcessName, -1 /*capsule->mApplicationInfo->mUid*/);
            mActivities->AddActivity(*ait1, "activity");
//            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//                if (r == null) {
//                    r = new StringBuilder(256);
//                } else {
//                    r.append(' ');
//                }
//                r.append(a.info.name);
//            }
        }
//        if (r != null) {
//            if (Config.LOGD) Slog.d(TAG, "  Activities: " + r);
//        }

#ifdef _HAS_PARSED_MORE // TODO: temporary comment.
        //Service
        List<CapsuleParser::Service*>::Iterator sit1 = capsule->mServices.Begin();
        List<CapsuleParser::Service*>::Iterator sit2 = capsule->mServices.End();
//        r = null;
        for (; sit1 != sit2; ++sit1) {
            (*sit1)->mInfo->mProcessName = FixProcessName(
                    capsule->mApplicationInfo->mProcessName,
                    (*sit1)->mInfo->mProcessName, -1 /*capsule->mApplicationInfo->mUid*/);
            mServices->AddService(*sit1);
//            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//                if (r == null) {
//                    r = new StringBuilder(256);
//                } else {
//                    r.append(' ');
//                }
//                r.append(a.info.name);
//            }
        }
//        if (r != null) {
//            if (Config.LOGD) Log.d(TAG, "  Receivers: " + r);
//        }
#endif

        return capsule;
    }

    if (DEBUG_INSTALL) {
        Slogger::E(TAG, err);
    }

    return NULL;
}

Boolean CCapsuleManagerService::IsForwardLocked(
    /* [in] */ CapsuleParser::Capsule* cap)
{
    return (cap->mApplicationInfo->mFlags & CApplicationInfo::FLAG_FORWARD_LOCK) != 0;
}

Boolean CCapsuleManagerService::IsExternal(
    /* [in] */ CapsuleParser::Capsule* cap)
{
    return (cap->mApplicationInfo->mFlags & CApplicationInfo::FLAG_EXTERNAL_STORAGE) != 0;
}

Boolean CCapsuleManagerService::IsSystemApp(
    /* [in] */ CapsuleParser::Capsule* cap)
{
    return (cap->mApplicationInfo->mFlags & CApplicationInfo::FLAG_SYSTEM) != 0;
}

Boolean CCapsuleManagerService::IsSystemApp(
    /* [in] */ CApplicationInfo* info)
{
    return (info->mFlags & CApplicationInfo::FLAG_SYSTEM) != 0;
}

