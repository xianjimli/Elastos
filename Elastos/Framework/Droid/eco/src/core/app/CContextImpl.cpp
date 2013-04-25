
#include "app/CContextImpl.h"
#include "app/ActivityManagerNative.h"
#include "content/CIntent.h"
#include "content/CResourcesFactory.h"
#include "content/CApplicationInfo.h"
#include "impl/CPolicyManager.h"
#include "view/CWindowManagerImpl.h"
#include "view/inputmethod/CLocalInputMethodManager.h"
#include "os/Process.h"
#include "os/FileUtils.h"
#include "os/Environment.h"
#include "os/Binder.h"
#include "text/CClipboardManager.h"
#include <unistd.h>
#include <assert.h>
#include <Slogger.h>

#include <stdio.h>

using namespace Elastos::Utility::Logging;

const char* CContextImpl::TAG  = "CContextImpl";

ICONTEXT_METHODS_IMPL(ReceiverRestrictedContext, ContextWrapper, ContextWrapper);

ReceiverRestrictedContext::ReceiverRestrictedContext(
    /* [in] */ IContext* base)
    : ContextWrapper(base)
{}

IInterface* ReceiverRestrictedContext::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IContext) {
        return (IContext*)this;
    }
    else if (riid == EIID_IContextWrapper) {
        return (IContextWrapper*)this;
    }

    return NULL;
}

UInt32 ReceiverRestrictedContext::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReceiverRestrictedContext::Release()
{
    return ElRefBase::Release();
}

ECode ReceiverRestrictedContext::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(IContextWrapper*)this) {
        *pIID = EIID_IContextWrapper;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode ReceiverRestrictedContext::GetBaseContext(
    /* [out] */ IContext** context)
{
    return ContextWrapper::GetBaseContext(context);
}


CContextImpl::ApplicationCapsuleManager::ResourceName::ResourceName(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 iconId)
    : mCapsuleName(capsuleName)
    , mIconId(iconId)
{}

CContextImpl::ApplicationCapsuleManager::ResourceName::ResourceName(
    /* [in] */ CApplicationInfo* aInfo,
    /* [in] */ Int32 iconId)
    : mCapsuleName(aInfo->mCapsuleName)
    , mIconId(iconId)
{}

CContextImpl::ApplicationCapsuleManager::ResourceName::ResourceName(
    /* [in] */ ComponentInfo* cInfo,
    /* [in] */ Int32 iconId)
    : mCapsuleName(cInfo->mApplicationInfo->mCapsuleName)
    , mIconId(iconId)
{}

CContextImpl::ApplicationCapsuleManager::ResourceName::ResourceName(
    /* [in] */ CResolveInfo* rInfo,
    /* [in] */ Int32 iconId)
    : mCapsuleName(rInfo->mActivityInfo->mApplicationInfo->mCapsuleName)
    , mIconId(iconId)
{}

UInt32 CContextImpl::ApplicationCapsuleManager::ResourceName::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CContextImpl::ApplicationCapsuleManager::ResourceName::Release()
{
    return ElRefBase::Release();
}


CContextImpl::ApplicationCapsuleManager::ApplicationCapsuleManager(
    /* [in] */ CContextImpl* context,
    /* [in] */ ICapsuleManager* pm)
    : mCachedSafeMode(-1)
    , mContext(context)
    , mPM(pm)
{}

CContextImpl::ApplicationCapsuleManager::~ApplicationCapsuleManager()
{
    mContext = NULL;
}

PInterface CContextImpl::ApplicationCapsuleManager::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ILocalCapsuleManager*)this;
    }
    else if (riid == EIID_ILocalCapsuleManager) {
        return (ILocalCapsuleManager*)this;
    }

    return NULL;
}

UInt32 CContextImpl::ApplicationCapsuleManager::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CContextImpl::ApplicationCapsuleManager::Release()
{
    return ElRefBase::Release();
}

ECode CContextImpl::ApplicationCapsuleManager::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(ILocalCapsuleManager*)this) {
        *pIID = EIID_ILocalCapsuleManager;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CContextImpl::ApplicationCapsuleManager::GetCapsuleInfo(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ ICapsuleInfo** capInfo)
{
    VALIDATE_NOT_NULL(capInfo);

    // try{
    ECode ec = mPM->GetCapsuleInfo(capsuleName, flags, capInfo);
    if (ec == E_REMOTE_EXCEPTION) {
        *capInfo = NULL;
        return E_RUNTIME_EXCEPTION;
    }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    if (*capInfo != NULL) return NOERROR;
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode CContextImpl::ApplicationCapsuleManager::CurrentToCanonicalCapsuleNames(
    /* [in] */ const ArrayOf<String>& names,
    /* [out, callee] */ ArrayOf<String>** cnames)
{
    VALIDATE_NOT_NULL(cnames);

    // try {
    ECode ec = mPM->CurrentToCanonicalCapsuleNames(names, cnames);
    if (ec == E_REMOTE_EXCEPTION) {
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
}

ECode CContextImpl::ApplicationCapsuleManager::CanonicalToCurrentCapsuleNames(
    /* [in] */ const ArrayOf<String>& names,
    /* [out, callee] */ ArrayOf<String>** cnames)
{
    VALIDATE_NOT_NULL(cnames);

    // try {
    ECode ec = mPM->CanonicalToCurrentCapsuleNames(names, cnames);
    if (ec == E_REMOTE_EXCEPTION) {
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
}

ECode CContextImpl::ApplicationCapsuleManager::GetLaunchIntentForCapsuel(
    /* [in] */ const String& capsuleName,
    /* [out] */ IIntent** intent)
{
    // // First see if the package has an INFO activity; the existence of
    // // such an activity is implied to be the desired front-door for the
    // // overall package (such as if it has multiple launcher entries).
    // Intent intentToResolve = new Intent(Intent.ACTION_MAIN);
    // intentToResolve.addCategory(Intent.CATEGORY_INFO);
    // intentToResolve.setPackage(packageName);
    // ResolveInfo resolveInfo = resolveActivity(intentToResolve, 0);

    // // Otherwise, try to find a main launcher activity.
    // if (resolveInfo == null) {
    //     // reuse the intent instance
    //     intentToResolve.removeCategory(Intent.CATEGORY_INFO);
    //     intentToResolve.addCategory(Intent.CATEGORY_LAUNCHER);
    //     intentToResolve.setPackage(packageName);
    //     resolveInfo = resolveActivity(intentToResolve, 0);
    // }
    // if (resolveInfo == null) {
    //     return null;
    // }
    // Intent intent = new Intent(Intent.ACTION_MAIN);
    // intent.setClassName(packageName, resolveInfo.activityInfo.name);
    // intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    // return intent;
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetCapsuleGids(
    /* [in] */ const String& capsuleName,
    /* [out, callee] */ ArrayOf<Int32>** gids)
{
    // try {
    //     int[] gids = mPM.getPackageGids(packageName);
    //     if (gids == null || gids.length > 0) {
    //         return gids;
    //     }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    // throw new NameNotFoundException(packageName);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetPermissionInfo(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [out] */ IPermissionInfo** info)
{
    // try {
    AutoPtr<IPermissionInfo> pi;
    mPM->GetPermissionInfo(name, flags, (IPermissionInfo**)&pi);
    if (pi != NULL) {
        *info = pi.Get();
        return NOERROR;
    }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    // throw new NameNotFoundException(name);
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode CContextImpl::ApplicationCapsuleManager::QueryPermissionsByGroup(
    /* [in] */ const String& group,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** infos)
{
    // try {
    AutoPtr<IObjectContainer> pi;
    mPM->QueryPermissionsByGroup(group, flags, (IObjectContainer**)&pi);
    if (pi != NULL) {
        *infos = pi.Get();
        return NOERROR;
    }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    // throw new NameNotFoundException(group);
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode CContextImpl::ApplicationCapsuleManager::GetPermissionGroupInfo(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [out] */ IPermissionGroupInfo** info)
{
    // try {
    AutoPtr<IPermissionGroupInfo> pgi;
    mPM->GetPermissionGroupInfo(name, flags, (IPermissionGroupInfo**)&pgi);
    if (pgi != NULL) {
        *info = pgi.Get();
        return NOERROR;
    }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    // throw new NameNotFoundException(name);
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode CContextImpl::ApplicationCapsuleManager::GetAllPermissionGroups(
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** infos)
{
    // try {
    return mPM->GetAllPermissionGroups(flags, infos);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    //return NOERROR;
}

ECode CContextImpl::ApplicationCapsuleManager::GetApplicationInfo(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IApplicationInfo** appInfo)
{
    VALIDATE_NOT_NULL(appInfo);

    // try {
    ECode ec = mPM->GetApplicationInfo(capsuleName, flags, appInfo);
    if (ec == E_REMOTE_EXCEPTION) {
        return E_RUNTIME_EXCEPTION;
    }

    if (*appInfo != NULL) return NOERROR;
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    // throw new NameNotFoundException(packageName);
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode CContextImpl::ApplicationCapsuleManager::GetActivityInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IActivityInfo** activityInfo)
{
    // try {
    //     ActivityInfo ai = mPM.getActivityInfo(className, flags);
    //     if (ai != null) {
    //         return ai;
    //     }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    // throw new NameNotFoundException(className.toString());
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetReceiverInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IActivityInfo** info)
{
    // try {
    //     ActivityInfo ai = mPM.getReceiverInfo(className, flags);
    //     if (ai != null) {
    //         return ai;
    //     }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    // throw new NameNotFoundException(className.toString());
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetServiceInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IServiceInfo** info)
{
    // try {
    //     ServiceInfo si = mPM.getServiceInfo(className, flags);
    //     if (si != null) {
    //         return si;
    //     }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    // throw new NameNotFoundException(className.toString());
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetContentProviderInfo(
    /* [in] */ IComponentName* className,
    /* [in] */ Int32 flags,
    /* [out] */ IContentProviderInfo** info)
{
    // try {
    //     ProviderInfo pi = mPM.getProviderInfo(className, flags);
    //     if (pi != null) {
    //         return pi;
    //     }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    // throw new NameNotFoundException(className.toString());
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetSystemSharedLibraryNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    // try {
    //      return mPM.getSystemSharedLibraryNames();
    // } catch (RemoteException e) {
    //  throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetSystemAvailableFeatures(
    /* [out] */ IObjectContainer** infos)
{
    // try {
    //     return mPM.getSystemAvailableFeatures();
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::HasSystemFeature(
    /* [in] */ const String& name,
    /* [out] */ Boolean* result)
{
    // try {
    //     return mPM.hasSystemFeature(name);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::CheckPermission(
    /* [in] */ const String& permName,
    /* [in] */ const String& capName,
    /* [out] */ Int32* perm)
{
    // try {
    return mPM->CheckPermission(permName, capName, perm);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    //return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::AddPermission(
    /* [in] */ IPermissionInfo* info,
    /* [out] */ Boolean* isAdded)
{
    // try {
    return mPM->AddPermission(info, isAdded);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    //return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::AddPermissionAsync(
    /* [in] */ IPermissionInfo* info,
    /* [out] */ Boolean* isAdded)
{
    // try {
    return mPM->AddPermissionAsync(info, isAdded);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    // return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::RemovePermission(
    /* [in] */ const String& name)
{
    // try {
    return mPM->RemovePermission(name);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    // return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::CheckSignatures(
    /* [in] */ const String& cap1,
    /* [in] */ const String& cap2,
    /* [out] */ Int32* sig)
{
    // try {
    //     return mPM.checkSignatures(pkg1, pkg2);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::CheckUidSignatures(
    /* [in] */ Int32 uid1,
    /* [in] */ Int32 uid2,
    /* [out] */ Int32* sig)
{
    // try {
    //     return mPM.checkUidSignatures(uid1, uid2);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetCapsulesForUid(
    /* [in] */ Int32 uid,
    /* [out, callee] */ ArrayOf<String>** capsules)
{
    // try {
    //     return mPM.getPackagesForUid(uid);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetNameForUid(
    /* [in] */ Int32 uid,
    /* [out] */ String* name)
{
    // try {
    //     return mPM.getNameForUid(uid);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetUidForSharedUser(
    /* [in] */ const String& sharedUserName,
    /* [out] */ Int32* uid)
{
    // try {
    //     int uid = mPM.getUidForSharedUser(sharedUserName);
    //     if(uid != -1) {
    //         return uid;
    //     }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    // throw new NameNotFoundException("No shared userid for user:"+sharedUserName);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetInstalledCapsules(
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** caps)
{
    // try {
    //     final List<PackageInfo> packageInfos = new ArrayList<PackageInfo>();
    //     PackageInfo lastItem = null;
    //     ParceledListSlice<PackageInfo> slice;

    //     do {
    //         final String lastKey = lastItem != null ? lastItem.packageName : null;
    //         slice = mPM.getInstalledPackages(flags, lastKey);
    //         lastItem = slice.populateList(packageInfos, PackageInfo.CREATOR);
    //     } while (!slice.isLastSlice());

    //     return packageInfos;
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetInstalledApplications(
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** apps)
{
    // try {
    //     final List<ApplicationInfo> applicationInfos = new ArrayList<ApplicationInfo>();
    //     ApplicationInfo lastItem = null;
    //     ParceledListSlice<ApplicationInfo> slice;

    //     do {
    //         final String lastKey = lastItem != null ? lastItem.packageName : null;
    //         slice = mPM.getInstalledApplications(flags, lastKey);
    //         lastItem = slice.populateList(applicationInfos, ApplicationInfo.CREATOR);
    //     } while (!slice.isLastSlice());

    //     return applicationInfos;
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::ResolveActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IResolveInfo** resolveInfo)
{
    // try {
    //     return mPM.resolveIntent(
    //         intent,
    //         intent.resolveTypeIfNeeded(mContext.getContentResolver()),
    //         flags);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::QueryIntentActivities(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** infos)
{
    // try {
    //     return mPM.queryIntentActivities(
    //         intent,
    //         intent.resolveTypeIfNeeded(mContext.getContentResolver()),
    //         flags);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::QueryIntentActivityOptions(
    /* [in] */ IComponentName* caller,
    /* [in] */ ArrayOf<IIntent*>* specifics,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** infos)
{
    // final ContentResolver resolver = mContext.getContentResolver();

    // String[] specificTypes = null;
    // if (specifics != null) {
    //     final int N = specifics.length;
    //     for (int i=0; i<N; i++) {
    //         Intent sp = specifics[i];
    //         if (sp != null) {
    //             String t = sp.resolveTypeIfNeeded(resolver);
    //             if (t != null) {
    //                 if (specificTypes == null) {
    //                     specificTypes = new String[N];
    //                 }
    //                 specificTypes[i] = t;
    //             }
    //         }
    //     }
    // }

    // try {
    //     return mPM.queryIntentActivityOptions(caller, specifics,
    //         specificTypes, intent, intent.resolveTypeIfNeeded(resolver),
    //         flags);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::QueryBroadcastReceivers(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** infos)
{
    // try {
    //     return mPM.queryIntentReceivers(
    //         intent,
    //         intent.resolveTypeIfNeeded(mContext.getContentResolver()),
    //         flags);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::ResolveService(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IResolveInfo** resolveInfo)
{
    // try {
    //     return mPM.resolveService(
    //         intent,
    //         intent.resolveTypeIfNeeded(mContext.getContentResolver()),
    //         flags);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::QueryIntentServices(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** infos)
{
    // try {
    AutoPtr<IContentResolver> cr;
    FAIL_RETURN(mContext->GetContentResolver((IContentResolver**)&cr));
    String type;
    FAIL_RETURN(intent->ResolveTypeIfNeeded(cr, &type));
    ECode ec = mPM->QueryIntentServices(intent, type, flags, infos);
    if (ec == E_REMOTE_EXCEPTION) {
        return E_RUNTIME_EXCEPTION;
    }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    return NOERROR;
}

ECode CContextImpl::ApplicationCapsuleManager::ResolveContentProvider(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [out] */ IContentProviderInfo** info)
{
    // try {
    //     return mPM.resolveContentProvider(name, flags);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::QueryContentProviders(
    /* [in] */ const String& processName,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** providers)
{
    // try {
    //     return mPM.queryContentProviders(processName, uid, flags);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetInstrumentationInfo(
    /* [in] */ IComponentName* className,
    /* [in] */ Int32 flags,
    /* [out] */ IInstrumentationInfo** info)
{
    // try {
    //     InstrumentationInfo ii = mPM.getInstrumentationInfo(
    //             className, flags);
    //     if (ii != null) {
    //         return ii;
    //     }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    // throw new NameNotFoundException(className.toString());
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::QueryInstrumentation(
    /* [in] */ const String& targetCapsule,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** infos)
{
    // try {
    //     return mPM.queryInstrumentation(targetPackage, flags);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetDrawable(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 resid,
    /* [in] */ IApplicationInfo* appInfo,
    /* [out] */ IDrawable** drawable)
{
    // ResourceName name = new ResourceName(packageName, resid);
    // Drawable dr = getCachedIcon(name);
    // if (dr != null) {
    //     return dr;
    // }
    // if (appInfo == null) {
    //     try {
    //         appInfo = getApplicationInfo(packageName, 0);
    //     } catch (NameNotFoundException e) {
    //         return null;
    //     }
    // }
    // try {
    //     Resources r = getResourcesForApplication(appInfo);
    //     dr = r.getDrawable(resid);
    //     if (false) {
    //         RuntimeException e = new RuntimeException("here");
    //         e.fillInStackTrace();
    //         Log.w(TAG, "Getting drawable 0x" + Integer.toHexString(resid)
    //                 + " from package " + packageName
    //                 + ": app scale=" + r.getCompatibilityInfo().applicationScale
    //                 + ", caller scale=" + mContext.getResources().getCompatibilityInfo().applicationScale,
    //                 e);
    //     }
    //     if (DEBUG_ICONS) Log.v(TAG, "Getting drawable 0x"
    //             + Integer.toHexString(resid) + " from " + r
    //             + ": " + dr);
    //     putCachedIcon(name, dr);
    //     return dr;
    // } catch (NameNotFoundException e) {
    //     Log.w("CapsuleManager_, "Failure retrieving resources for"
    //             + appInfo.packageName);
    // } catch (RuntimeException e) {
    //     // If an exception was thrown, fall through to return
    //     // default icon.
    //     Log.w("CapsuleManager_, "Failure retrieving icon 0x"
    //             + Integer.toHexString(resid) + " in package "
    //             + packageName, e);
    // }
    // return null;
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetActivityIcon(
    /* [in] */ IComponentName* activityName,
    /* [out] */ IDrawable** icon)
{
    // return getActivityInfo(activityName, 0).loadIcon(this);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetActivityIconEx(
    /* [in] */ IIntent* intent,
    /* [out] */ IDrawable** icon)
{
    // if (intent.getComponent() != null) {
    //     return getActivityIcon(intent.getComponent());
    // }

    // ResolveInfo info = resolveActivity(
    //     intent, CapsuleManager_MATCH_DEFAULT_ONLY);
    // if (info != null) {
    //     return info.activityInfo.loadIcon(this);
    // }

    // throw new NameNotFoundException(intent.toURI());
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetDefaultActivityIcon(
    /* [out] */ IDrawable** icon)
{
    // return Resources.getSystem().getDrawable(
    //         com.android.internal.R.drawable.sym_def_app_icon);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetApplicationIcon(
    /* [in] */ IApplicationInfo* info,
    /* [out] */ IDrawable** icon)
{
    // return info.loadIcon(this);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetApplicationIconEx(
    /* [in] */ const String& capsuleName,
    /* [out] */ IDrawable** icon)
{
    // return getApplicationIcon(getApplicationInfo(packageName, 0));
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetActivityLogo(
    /* [in] */ IComponentName* activityName,
    /* [out] */ IDrawable** logo)
{
    // return getActivityInfo(activityName, 0).loadLogo(this);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetActivityLogoEx(
    /* [in] */ IIntent* intent,
    /* [out] */ IDrawable** logo)
{
    // if (intent.getComponent() != null) {
    //     return getActivityLogo(intent.getComponent());
    // }

    // ResolveInfo info = resolveActivity(
    //         intent, CapsuleManager_MATCH_DEFAULT_ONLY);
    // if (info != null) {
    //     return info.activityInfo.loadLogo(this);
    // }

    // throw new NameNotFoundException(intent.toUri(0));
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetApplicationLogo(
    /* [in] */ IApplicationInfo* info,
    /* [out] */ IDrawable** logo)
{
    // return info.loadLogo(this);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetApplicationLogoEx(
    /* [in] */ const String& capsuleName,
    /* [out] */ IDrawable** logo)
{
    // return getApplicationLogo(getApplicationInfo(packageName, 0));
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetResourcesForActivity(
    /* [in] */ IComponentName* activityName,
    /* [out] */ IResources** res)
{
     // return getResourcesForApplication(
     //        getActivityInfo(activityName, 0).applicationInfo);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetResourcesForApplication(
    /* [in] */ IApplicationInfo* app,
    /* [out] */ IResources** res)
{
    VALIDATE_NOT_NULL(res);

    CApplicationInfo* appObj = (CApplicationInfo*)app;
    if (appObj->mCapsuleName.Equals("system")) {
        assert(0);
        return E_NOT_IMPLEMENTED;
        // return mContext.mMainThread.getSystemContext().getResources();
    }
    AutoPtr<CResources> r;
    mContext->mApartment->GetTopLevelResources(
            appObj->mUid == Process::MyUid() ? appObj->mSourceDir
            : appObj->mPublicSourceDir, mContext->mCapsuleInfo, (CResources**)&r);
    if (r != NULL) {
        *res = (IResources*)r.Get();
        (*res)->AddRef();
        return NOERROR;
    }
    // throw new NameNotFoundException("Unable to open " + app.publicSourceDir);
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode CContextImpl::ApplicationCapsuleManager::GetResourcesForApplicationEx(
    /* [in] */ const String& appCapsuleName,
    /* [out] */ IResources** res)
{
    // return getResourcesForApplication(
    //         getApplicationInfo(appPackageName, 0));
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::IsSafeMode(
    /* [out] */ Boolean* isSafeMode)
{
    // try {
    //     if (mCachedSafeMode < 0) {
    //         mCachedSafeMode = mPM.isSafeMode() ? 1 : 0;
    //     }
    //     return mCachedSafeMode != 0;
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

void CContextImpl::ApplicationCapsuleManager::ConfigurationChanged()
{
    // synchronized (sSync) {
    //     sIconCache.clear();
    //     sStringCache.clear();
    // }
    assert(0);
}

AutoPtr<IDrawable> CContextImpl::ApplicationCapsuleManager::GetCachedIcon(
    /* [in] */ ResourceName* name)
{
    // synchronized (sSync) {
    //     WeakReference<Drawable> wr = sIconCache.get(name);
    //     if (DEBUG_ICONS) Log.v(TAG, "Get cached weak drawable ref for "
    //             + name + ": " + wr);
    //     if (wr != null) {   // we have the activity
    //         Drawable dr = wr.get();
    //         if (dr != null) {
    //             if (DEBUG_ICONS) Log.v(TAG, "Get cached drawable for "
    //                     + name + ": " + dr);
    //             return dr;
    //         }
    //         // our entry has been purged
    //         sIconCache.remove(name);
    //     }
    // }
    // return null;
    assert(0);
    return NULL;
}

void CContextImpl::ApplicationCapsuleManager::PutCachedIcon(
    /* [in] */ ResourceName* name,
    /* [in] */ IDrawable* dr)
{
    // synchronized (sSync) {
    //     sIconCache.put(name, new WeakReference<Drawable>(dr));
    //     if (DEBUG_ICONS) Log.v(TAG, "Added cached drawable for "
    //             + name + ": " + dr);
    // }
    assert(0);
}

void CContextImpl::ApplicationCapsuleManager::HandleCapsuleBroadcast(
    /* [in] */ Int32 cmd,
    /* [in] */ ArrayOf<String>* capList,
    /* [in] */ Boolean hasCapInfo)
{
    // boolean immediateGc = false;
    // if (cmd == IApplicationThread.EXTERNAL_STORAGE_UNAVAILABLE) {
    //     immediateGc = true;
    // }
    // if (pkgList != null && (pkgList.length > 0)) {
    //     boolean needCleanup = false;
    //     for (String ssp : pkgList) {
    //         synchronized (sSync) {
    //             if (sIconCache.size() > 0) {
    //                 Iterator<ResourceName> it = sIconCache.keySet().iterator();
    //                 while (it.hasNext()) {
    //                     ResourceName nm = it.next();
    //                     if (nm.packageName.equals(ssp)) {
    //                         //Log.i(TAG, "Removing cached drawable for " + nm);
    //                         it.remove();
    //                         needCleanup = true;
    //                     }
    //                 }
    //             }
    //             if (sStringCache.size() > 0) {
    //                 Iterator<ResourceName> it = sStringCache.keySet().iterator();
    //                 while (it.hasNext()) {
    //                     ResourceName nm = it.next();
    //                     if (nm.packageName.equals(ssp)) {
    //                         //Log.i(TAG, "Removing cached string for " + nm);
    //                         it.remove();
    //                         needCleanup = true;
    //                     }
    //                 }
    //             }
    //         }
    //     }
    //     if (needCleanup || hasPkgInfo) {
    //         if (immediateGc) {
    //             // Schedule an immediate gc.
    //             Runtime.getRuntime().gc();
    //         } else {
    //             ActivityThread.currentActivityThread().scheduleGcIdler();
    //         }
    //     }
    // }
}

AutoPtr<ICharSequence> CContextImpl::ApplicationCapsuleManager::GetCachedString(
    /* [in] */ ResourceName* name)
{
    // synchronized (sSync) {
    //     WeakReference<CharSequence> wr = sStringCache.get(name);
    //     if (wr != null) {   // we have the activity
    //         CharSequence cs = wr.get();
    //         if (cs != null) {
    //             return cs;
    //         }
    //         // our entry has been purged
    //         sStringCache.remove(name);
    //     }
    // }
    // return null;
    assert(0);
    return NULL;
}

void CContextImpl::ApplicationCapsuleManager::PutCachedString(
    /* [in] */ ResourceName* name,
    /* [in] */ ICharSequence* cs)
{
    // synchronized (sSync) {
    //     sStringCache.put(name, new WeakReference<CharSequence>(cs));
    // }
    assert(0);
}

ECode CContextImpl::ApplicationCapsuleManager::GetText(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 resid,
    /* [in] */ IApplicationInfo* appInfo,
    /* [out] */ ICharSequence** text)
{
    // ResourceName name = new ResourceName(packageName, resid);
    // CharSequence text = getCachedString(name);
    // if (text != null) {
    //     return text;
    // }
    // if (appInfo == null) {
    //     try {
    //         appInfo = getApplicationInfo(packageName, 0);
    //     } catch (NameNotFoundException e) {
    //         return null;
    //     }
    // }
    // try {
    //     Resources r = getResourcesForApplication(appInfo);
    //     text = r.getText(resid);
    //     putCachedString(name, text);
    //     return text;
    // } catch (NameNotFoundException e) {
    //     Log.w("CapsuleManager_, "Failure retrieving resources for"
    //             + appInfo.packageName);
    // } catch (RuntimeException e) {
    //     // If an exception was thrown, fall through to return
    //     // default icon.
    //     Log.w("CapsuleManager_, "Failure retrieving text 0x"
    //             + Integer.toHexString(resid) + " in package "
    //             + packageName, e);
    // }
    // return null;
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetXml(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 resid,
    /* [in] */ IApplicationInfo* appInfo,
    /* [out] */ IXmlResourceParser** parser)
{
    VALIDATE_NOT_NULL(parser);

    AutoPtr<IApplicationInfo> _appInfo;
    if (appInfo == NULL) {
        // try {
        ECode ec = GetApplicationInfo(capsuleName, 0, (IApplicationInfo**)&_appInfo);
        if (ec == E_NAME_NOT_FOUND_EXCEPTION) {
            *parser = NULL;
            return NOERROR;
        }
        appInfo = _appInfo.Get();
        // } catch (NameNotFoundException e) {
        //     return null;
        // }
    }
    // try {
    AutoPtr<IResources> r;
    GetResourcesForApplication(appInfo, (IResources**)&r);
    return r->GetXml(resid, parser);
    // } catch (RuntimeException e) {
    //     // If an exception was thrown, fall through to return
    //     // default icon.
    //     Log.w("CapsuleManager_, "Failure retrieving xml 0x"
    //             + Integer.toHexString(resid) + " in package "
    //             + packageName, e);
    // } catch (NameNotFoundException e) {
    //     Log.w("CapsuleManager_, "Failure retrieving resources for"
    //             + appInfo.packageName);
    // }
    // return null;
}

ECode CContextImpl::ApplicationCapsuleManager::GetApplicationLabel(
    /* [in] */ IApplicationInfo* info,
    /* [out] */ ICharSequence** label)
{
    // return info.loadLabel(this);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::InstallCapsule(
    /* [in] */ IUri* capsuleURI,
    /* [in] */ ICapsuleInstallObserver* observer,
    /* [in] */ Int32 flags,
    /* [in] */ const String& installerCapsuleName)
{
    // try {
    //     mPM.installPackage(packageURI, observer, flags, installerPackageName);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::MoveCapsule(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleMoveObserver* observer,
    /* [in] */ Int32 flags)
{
    // try {
    //     mPM.movePackage(packageName, observer, flags);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetInstallerCapsuleName(
    /* [in] */ const String& capsuleName,
    /* [out] */ String* name)
{
    // try {
    //     return mPM.getInstallerPackageName(packageName);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    // return null;
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::DeleteCapsule(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleDeleteObserver* observer,
    /* [in] */ Int32 flags)
{
    // try {
    //     mPM.deletePackage(packageName, observer, flags);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::ClearApplicationUserData(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleDataObserver* observer)
{
    // try {
    //     mPM.clearApplicationUserData(packageName, observer);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::DeleteApplicationCacheFiles(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleDataObserver* observer)
{
    // try {
    //     mPM.deleteApplicationCacheFiles(packageName, observer);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::FreeStorageAndNotify(
    /* [in] */ Int64 freeStorageSize,
    /* [in] */ ICapsuleDataObserver* observer)
{
    // try {
    //     mPM.freeStorageAndNotify(idealStorageSize, observer);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::FreeStorage(
    /* [in] */ Int64 freeStorageSize,
    /* [in] */ IIntentSender* pi)
{
    // try {
    //     mPM.freeStorage(freeStorageSize, pi);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetCapsuleSizeInfo(
    /* [in] */ const String& capsuleName,
    /* [in, out] */ ICapsuleStatsObserver* observer)
{
    // try {
    //     mPM.getPackageSizeInfo(packageName, observer);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::AddCapsuleToPreferred(
    /* [in] */ const String& capsuleName)
{
    // try {
    //     mPM.addPackageToPreferred(packageName);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::RemoveCapsuleFromPreferred(
    /* [in] */ const String& capsuleName)
{
    // try {
    //     mPM.removePackageFromPreferred(packageName);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetPreferredCapsules(
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** infos)
{
    // try {
    //     return mPM.getPreferredPackages(flags);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    // return new ArrayList<PackageInfo>();
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::AddPreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ const ArrayOf<IComponentName*>& set,
    /* [in] */ IComponentName* activity)
{
    // try {
    //     mPM.addPreferredActivity(filter, match, set, activity);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::ReplacePreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ const ArrayOf<IComponentName*>& set,
    /* [in] */ IComponentName* activity)
{
    // try {
    //     mPM.replacePreferredActivity(filter, match, set, activity);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::ClearCapsulePreferredActivities(
    /* [in] */ const String& capsuleName)
{
    // try {
    //     mPM.clearPackagePreferredActivities(packageName);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetPreferredActivities(
    /* [in] */ IObjectContainer* outFilters,
    /* [in] */ IObjectContainer* outActivities,
    /* [in] */ const String& capsuleName,
    /* [out] */ Int32* count)
{
    // try {
    //     return mPM.getPreferredActivities(outFilters, outActivities, packageName);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    // return 0;
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::SetComponentEnabledSetting(
    /* [in] */ IComponentName* componentName,
    /* [in] */ Int32 newState,
    /* [in] */ Int32 flags)
{
    // try {
    //     mPM.setComponentEnabledSetting(componentName, newState, flags);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetComponentEnabledSetting(
    /* [in] */ IComponentName* componentName,
    /* [out] */ Int32* setting)
{
    // try {
    //     return mPM.getComponentEnabledSetting(componentName);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    // return CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT;
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::SetApplicationEnabledSetting(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 newState,
    /* [in] */ Int32 flags)
{
    // try {
    //     mPM.setApplicationEnabledSetting(packageName, newState, flags);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetApplicationEnabledSetting(
    /* [in] */ const String& capsuleName,
    /* [out] */ Int32* setting)
{
    // try {
    //     return mPM.getApplicationEnabledSetting(packageName);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
    // return CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT;
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContextImpl::ApplicationCapsuleManager::GetCapsuleArchiveInfo(
        /* [in] */ const String& archiveFilePath,
        /* [in] */ Int32 flags,
        /* [out] */ ICapsuleInfo** info)
{
    VALIDATE_NOT_NULL(info);

    return CapsuleManager::GetCapsuleArchiveInfo(archiveFilePath, flags, info);
}


CContextImpl::CContextImpl()
    : mCapsuleInfo(NULL)
    , mThemeResource(0)
{}

CContextImpl::~CContextImpl()
{}

ECode CContextImpl::constructor()
{
    return NOERROR;
}

ECode CContextImpl::constructor(
    /* [in] */ Boolean isSysCtx)
{
    if (!isSysCtx) return NOERROR;

    AutoPtr<IResourcesFactory> factory;
    FAIL_RETURN(CResourcesFactory::AcquireSingleton((IResourcesFactory**)&factory));
    FAIL_RETURN(factory->GetSystem((IResources**)&mResources));

    CApplicationApartment::NewByFriend((CApplicationApartment**)&mApartment);
    LoadedCap* info = new LoadedCap(mApartment, String("android"), this, NULL);
    Init(info, NULL, mApartment, mResources);
    AutoPtr<IConfiguration> config = mApartment->GetConfiguration();
    AutoPtr<IDisplayMetrics> dis;
    mApartment->GetDisplayMetricsLocked(FALSE, (IDisplayMetrics**) &dis);
    return mResources->UpdateConfiguration(config, dis);
}

ECode CContextImpl::constructor(
    /* [in] */ IResources* res,
    /* [in] */ IBinder* binder)
{
    mResources = res;
    mApartment = (CApplicationApartment*)binder;

    return CApplicationContentResolver::NewByFriend(this, mApartment,
            (CApplicationContentResolver**)&mContentResolver);
}

PInterface CContextImpl::Probe(
    /* [in] */ REIID riid)
{
    return _CContextImpl::Probe(riid);
}

ECode CContextImpl::GetAssets(
    /* [out] */ IAssetManager** assetManager)
{
    return mResources->GetAssets(assetManager);
}

ECode CContextImpl::GetResources(
    /* [out] */ IResources** resources)
{
    if (resources == NULL) return E_INVALID_ARGUMENT;

    *resources = mResources.Get();
    if (*resources != NULL) (*resources)->AddRef();
    return NOERROR;
}

ECode CContextImpl::GetCapsuleManager(
    /* [out] */ ILocalCapsuleManager** capsuleManager)
{
    VALIDATE_NOT_NULL(capsuleManager);

    if (mCapsuleManager != NULL) {
        *capsuleManager = mCapsuleManager;
        (*capsuleManager)->AddRef();
        return NOERROR;
    }

    AutoPtr<ICapsuleManager> pm = CApplicationApartment::GetCapsuleManager();
    if (pm != NULL) {
        // Doesn't matter if we make more than one instance.
        mCapsuleManager = new ApplicationCapsuleManager(this, pm);
        *capsuleManager = mCapsuleManager;
        (*capsuleManager)->AddRef();
        return NOERROR;
    }

    *capsuleManager = NULL;
    return NOERROR;
}

ECode CContextImpl::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    if (resolver == NULL) return E_INVALID_ARGUMENT;

    *resolver = (IContentResolver*)
            (CApplicationContentResolver*)mContentResolver;
    (*resolver)->AddRef();

    return NOERROR;
}

ECode CContextImpl::GetApplicationContext(
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx);

    *ctx = NULL;
    if (mCapsuleInfo != NULL) {
        *ctx = mCapsuleInfo->GetApplication();
        if (*ctx != NULL) {
            (*ctx)->AddRef();
        }
    }
    else {
        assert(0);
        //TODO
        //ActivityThread::GetApplication();
    }

    return NOERROR;
}

ECode CContextImpl::GetText(
    /* [in] */ Int32 resId,
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    return Context::GetText(resId, text);
}

ECode CContextImpl::GetString(
    /* [in] */ Int32 resId,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    return Context::GetString(resId, str);
}

ECode CContextImpl::SetTheme(
    /* [in] */ Int32 resid)
{
    mThemeResource = resid;
    return NOERROR;
}

ECode CContextImpl::GetTheme(
    /* [out] */ ITheme** theme)
{
    VALIDATE_NOT_NULL(theme);

    if (mTheme == NULL) {
        if (mThemeResource == 0) {
            //todo:
            mThemeResource = 0x01030005/*com.android.internal.R.style.Theme*/;
        }
        ASSERT_SUCCEEDED(mResources->NewTheme((ITheme**)&mTheme));
        mTheme->ApplyStyle(mThemeResource, TRUE);
    }
    *theme = mTheme.Get();
    (*theme)->AddRef();
    return NOERROR;
}

ECode CContextImpl::ObtainStyledAttributes(
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributes(attrs, styles);
}

ECode CContextImpl::ObtainStyledAttributesEx(
    /* [in] */ Int32 resid,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributesEx(resid, attrs, styles);
}

ECode CContextImpl::ObtainStyledAttributesEx2(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributesEx2(set, attrs, styles);
}

ECode CContextImpl::ObtainStyledAttributesEx3(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributesEx3(set, attrs, defStyleAttr, defStyleRes, styles);
}

ECode CContextImpl::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    VALIDATE_NOT_NULL(loader);

    if (mCapsuleInfo != NULL) {
        return mCapsuleInfo->GetClassLoader(loader);
    }
    else {
//        return ClassLoader::GetSystemClassLoader(loader);
        assert(0);
        return E_NOT_IMPLEMENTED;
    }
}

ECode CContextImpl::GetCapsuleName(
    /* [out] */ String* capsuleName)
{
    if (mCapsuleInfo != NULL) {
        return mCapsuleInfo->GetCapsuleName(capsuleName);
    }
    return E_DOES_NOT_EXIST;

//    throw new RuntimeException("Not supported in system context");
}

ECode CContextImpl::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    if (mCapsuleInfo != NULL) {
        return mCapsuleInfo->GetApplicationInfo(info);
    }
//    throw new RuntimeException("Not supported in system context");
    return E_RUNTIME_EXCEPTION;
}

ECode CContextImpl::GetCapsuleResourcePath(
    /* [out] */ String* path)
{
    if (mCapsuleInfo != NULL) {
        return mCapsuleInfo->GetResDir(path);
    }
    //throw new RuntimeException("Not supported in system context");
    return E_RUNTIME_EXCEPTION;
}

ECode CContextImpl::GetFilesDir(
    /* [out] */ IFile** filesDir)
{
    VALIDATE_NOT_NULL(filesDir);

    Mutex::Autolock lock(mSync);
    if (mFilesDir == NULL) {
        AutoPtr<IFile> dirFile;
        GetDataDirFile((IFile**)&dirFile);
        CFile::New(dirFile, String("files"), (IFile**)&mFilesDir);
    }
    Boolean isExists;
    mFilesDir->Exists(&isExists);
    if (!isExists) {
        Boolean succeeded;
        mFilesDir->Mkdir(&succeeded);
        if(!succeeded) {
            //Log.w(TAG, "Unable to create files directory");
            *filesDir = NULL;
            return NOERROR;
        }
        String path;
        mFilesDir->GetPath(&path);
        FileUtils::SetPermissions(
                path, FileUtils::IRWXU | FileUtils::IRWXG | FileUtils::IXOTH,
                -1, -1);
    }
    *filesDir = mFilesDir;
    return NOERROR;
}

ECode CContextImpl::GetDataDirFile(
    /* [out] */ IFile** dirFile)
{
    if (mCapsuleInfo != NULL) {
        return mCapsuleInfo->GetDataDirFile(dirFile);
    }
    return E_RUNTIME_EXCEPTION;
    //throw new RuntimeException("Not supported in system context");
}

ECode CContextImpl::GetExternalFilesDir(
    /* [in] */ const String& type,
    /* [out] */ IFile** filesDir)
{
    VALIDATE_NOT_NULL(filesDir);

    Mutex::Autolock lock(mSync);
    if (mExternalFilesDir == NULL) {
        String name;
        GetCapsuleName(&name);
        mExternalFilesDir = Environment::GetExternalStorageAppFilesDirectory(name);
    }
    Boolean isExists;
    mExternalFilesDir->Exists(&isExists);
    if (!isExists) {
        //try {
        AutoPtr<IFile> f;
        CFile::New(Environment::GetExternalStorageElastosDataDir()
                , String(".nomedia"), (IFile**)&f);
        Boolean result;
        f->CreateNewFile(&result);
        // } catch (IOException e) {
        // }
        Boolean succeeded;
        mExternalFilesDir->Mkdirs(&succeeded);
        if (!succeeded) {
            //Log.w(TAG, "Unable to create external files directory");
            *filesDir = NULL;
            return NOERROR;
        }
    }
    if (type.IsNull()) {
        *filesDir = mExternalFilesDir;
        return NOERROR;
    }
    AutoPtr<IFile> dir;
    CFile::New(mExternalFilesDir, type, (IFile**)&dir);
    dir->Exists(&isExists);
    if (!isExists) {
        Boolean succeeded;
        dir->Mkdirs(&succeeded);
        if (!succeeded) {
            //Log.w(TAG, "Unable to create external media directory " + dir);
            *filesDir = NULL;
            return NOERROR;
        }
    }
    *filesDir = dir;
    return NOERROR;
}

ECode CContextImpl::GetCacheDir(
    /* [out] */ IFile** cacheDir)
{
    VALIDATE_NOT_NULL(cacheDir);

    Mutex::Autolock lock(mSync);
    if (mCacheDir == NULL) {
        AutoPtr<IFile> dirFile;
        GetDataDirFile((IFile**)&dirFile);
        CFile::New(dirFile, String("cache"), (IFile**)&mCacheDir);
    }
    Boolean isExists;
    mCacheDir->Exists(&isExists);
    if (!isExists) {
        Boolean succeeded;
        mCacheDir->Mkdirs(&succeeded);
        if(!succeeded) {
            //Log.w(TAG, "Unable to create cache directory");
            *cacheDir = NULL;
            return NOERROR;
        }
        String path;
        mCacheDir->GetPath(&path);
        FileUtils::SetPermissions(path,
                FileUtils::IRWXU | FileUtils::IRWXG | FileUtils::IXOTH,
                -1, -1);
    }
    *cacheDir = mCacheDir;
    return NOERROR;
}

ECode CContextImpl::GetDir(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir);

    String s = String("app_") + name;
    AutoPtr<IFile> dirFile;
    GetDataDirFile((IFile**)&dirFile);
    AutoPtr<IFile> file;
    MakeFilename(dirFile, name, (IFile**)&file);
    Boolean isExists;
    file->Exists(&isExists);
    if (!isExists) {
        Boolean succeeded;
        file->Mkdir(&succeeded);
        String path;
        file->GetPath(&path);
        SetFilePermissionsFromMode(path, mode,
                FileUtils::IRWXU | FileUtils::IRWXG | FileUtils::IXOTH);
    }
    *dir = file;
    return NOERROR;
}

void CContextImpl::SetFilePermissionsFromMode(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ Int32 extraPermissions)
{
    Int32 perms = FileUtils::IRUSR | FileUtils::IWUSR
        | FileUtils::IRGRP | FileUtils::IWGRP
        | extraPermissions;
    if ((mode & Context_MODE_WORLD_READABLE) != 0) {
        perms |= FileUtils::IROTH;
    }
    if ((mode & Context_MODE_WORLD_WRITEABLE) != 0) {
        perms |= FileUtils::IWOTH;
    }
    // if (DEBUG) {
    //     Log.i(TAG, "File " + name + ": mode=0x" + Integer.toHexString(mode)
    //           + ", perms=0x" + Integer.toHexString(perms));
    // }
    FileUtils::SetPermissions(name, perms, -1, -1);
}

ECode CContextImpl::MakeFilename(
    /* [in] */ IFile* base,
    /* [in] */ const String& name,
    /* [out] */ IFile** file)
{
    if (name.IndexOf('/'/*File.separatorChar*/) < 0) {
        return CFile::New(base, name, file);
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
    // throw new IllegalArgumentException(
    //         "File " + name + " contains a path separator");
}

ECode CContextImpl::StartActivity(
    /* [in] */ IIntent *intent)
{
    Int32 flags;
    intent->GetFlags(&flags);
    if ((flags & Intent_FLAG_ACTIVITY_NEW_TASK) == 0) {
//            throw new AndroidRuntimeException(
//                  "Calling startActivity() from outside of an Activity "
//                  + " context requires the FLAG_ACTIVITY_NEW_TASK flag."
//                  + " Is this really what you want?");
    }
    AutoPtr<IActivityResult> result;
    mApartment->GetInstrumentation()->ExecStartActivity(
            GetOuterContext(), (IApplicationApartment*)(CApplicationApartment*)mApartment,
            NULL, NULL, intent, -1, (IActivityResult**)&result);
    return NOERROR;
}

ECode CContextImpl::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags)
{
    // try {
        String resolvedType;
        if (fillInIntent != NULL) {
            AutoPtr<IContentResolver> contentResolver;
            FAIL_RETURN(GetContentResolver(
                (IContentResolver**)&contentResolver));
            FAIL_RETURN(fillInIntent->ResolveTypeIfNeeded(
                contentResolver, &resolvedType));
        }

        AutoPtr<IActivityManager> activityManager;
        FAIL_RETURN(ActivityManagerNative::GetDefault(
            (IActivityManager**)&activityManager));

        // TODO: ALEX need IActivityManager::StartActivityIntentSender
        return E_NOT_IMPLEMENTED;
        /*
        Int32 result;
        FAIL_RETURN(activityManager->StartActivityIntentSender(
            mMainThread.getApplicationThread(), intent,
                fillInIntent, resolvedType, null, null,
                0, flagsMask, flagsValues, &result));
        if (result == IActivityManager_START_CANCELED) {
            throw new IntentSender.SendIntentException();
        }
        Instrumentation.checkStartActivityResult(result, null);
        */
    // } catch (RemoteException e) {
    // }
}

ECode CContextImpl::SendBroadcast(
    /* [in] */ IIntent *intent)
{
    AutoPtr<IContentResolver> resolver;
    GetContentResolver((IContentResolver**)&resolver);
    String resolvedType;
    intent->ResolveTypeIfNeeded(resolver, &resolvedType);

//    try {
    AutoPtr<IActivityManager> mgr;
    ECode ec = ActivityManagerNative::GetDefault(
            (IActivityManager**)&mgr);
    if (FAILED(ec)) return ec;
    Int32 result;
    return mgr->BroadcastIntent(
            (IApplicationApartment*)mApartment, intent, resolvedType, NULL,
            Activity_RESULT_OK, String(NULL), NULL, String(NULL), FALSE, FALSE, getpid(), getuid(), &result);
//    } catch (RemoteException e) {
//    }
}

ECode CContextImpl::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    if (name == NULL) return E_INVALID_ARGUMENT;

    AutoPtr<IActivityManager> activityManager;
    ActivityManagerNative::GetDefault((IActivityManager**)&activityManager);
    AutoPtr<IContentResolver> resolver;
    GetContentResolver((IContentResolver**)&resolver);
    String type;
    service->ResolveTypeIfNeeded(resolver, &type);
    ECode ec = activityManager->StartService(
            (IApplicationApartment*)(CApplicationApartment*)mApartment,
            service, type, name);
    if (FAILED(ec)) {
        *name = NULL;
        return ec;
    }
    if (*name != NULL) {
        String capsuleName;
        (*name)->GetCapsuleName(&capsuleName);
        if (!capsuleName.Compare("!")) {
//            throw new SecurityException(
//                    "Not allowed to start service " + service
//                    + " without permission " + cn.getClassName());
            return E_SECURITY_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CContextImpl::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    if (succeeded == NULL) return E_INVALID_ARGUMENT;

    AutoPtr<IActivityManager> activityManager;
    ActivityManagerNative::GetDefault((IActivityManager**)&activityManager);
    AutoPtr<IContentResolver> resolver;
    GetContentResolver((IContentResolver**)&resolver);
    String type;
    service->ResolveTypeIfNeeded(resolver, &type);
    Int32 res;
    ECode ec = activityManager->StopService(
            (IApplicationApartment*)(CApplicationApartment*)mApartment,
            service, type, &res);
    if (FAILED(ec)) {
        *succeeded = FALSE;
        return NOERROR;
    }

    if ((res < 0)) {
//        throw new SecurityException(
//                    "Not allowed to stop service " + service);
        return E_SECURITY_EXCEPTION;
    }
    else {
        *succeeded = res != 0;
        return NOERROR;
    }
}

ECode CContextImpl::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    if (succeeded == NULL) return E_INVALID_ARGUMENT;

    AutoPtr<IServiceConnectionInner> sd;
    if (mCapsuleInfo != NULL) {
        sd = mCapsuleInfo->GetServiceDispatcher(conn, GetOuterContext(),
                (IApplicationApartment*)(CApplicationApartment*)mApartment,
                flags);
    }
    else {
//        throw new RuntimeException("Not supported in system context");
        return E_RUNTIME_EXCEPTION;
    }
    AutoPtr<IActivityManager> activityManager;
    ActivityManagerNative::GetDefault((IActivityManager**)&activityManager);
    AutoPtr<IContentResolver> resolver;
    GetContentResolver((IContentResolver**)&resolver);
    String type;
    service->ResolveTypeIfNeeded(resolver, &type);
    Int32 res;
    ECode ec = activityManager->BindService(
        (IApplicationApartment*)(CApplicationApartment*)mApartment,
        GetActivityToken(), service, type, sd, flags, getpid(), getuid(), &res);
    if (FAILED(ec)) {
        *succeeded = FALSE;
        return NOERROR;
    }

    if (res < 0) {
//        throw new SecurityException(
//                "Not allowed to bind to service " + service);
        return E_SECURITY_EXCEPTION;
    }
    else {
        *succeeded = res != 0;
        return NOERROR;
    }
}

ECode CContextImpl::UnbindService(
    /* [in] */ IServiceConnection* conn)
{
    if (mCapsuleInfo != NULL) {
        IServiceConnectionInner* sd = mCapsuleInfo->ForgetServiceDispatcher(
                GetOuterContext(), conn);
//        try {
        AutoPtr<IActivityManager> activityManager;
        ActivityManagerNative::GetDefault((IActivityManager**)&activityManager);
        Boolean succeeded;
        activityManager->UnbindService(sd, &succeeded);
//        } catch (RemoteException e) {
//        }
        return NOERROR;
    } else {
//        throw new RuntimeException("Not supported in system context");
        return E_RUNTIME_EXCEPTION;
    }
}

ECode CContextImpl::GetSystemService(
    /* [in] */ CString name,
    /* [out] */ IInterface** object)
{
    ECode ec = NOERROR;
    if (!CString(Context_WINDOW_SERVICE).Compare(name)) {
        AutoPtr<ILocalWindowManager> wm;
        CWindowManagerImpl::AcquireSingleton((ILocalWindowManager**)&wm);
        *object = (IInterface*)wm.Get();
        (*object)->AddRef();
        return NOERROR;
    }
    else if (!CString(Context_LAYOUT_INFLATER_SERVICE).Compare(name)) {
        Mutex::Autolock lock(mSync);

        if (mLayoutInflater != NULL) {
            *object = (IInterface*)mLayoutInflater.Get();
            (*object)->AddRef();
            return NOERROR;
        }

        AutoPtr<IPolicyManager> pm;
        ec = CPolicyManager::AcquireSingleton((IPolicyManager**)&pm);
        if (FAILED(ec)) {
            return ec;
        }

        ec = pm->MakeNewLayoutInflater(GetOuterContext(), (ILayoutInflater**)&mLayoutInflater);
        if (FAILED(ec)) {
            return ec;
        }

        *object = (IInterface*)mLayoutInflater.Get();
        (*object)->AddRef();
        return NOERROR;
    }
    else if (!CString(Context_ACTIVITY_SERVICE).Compare(name)) {
        assert(0);
//        return getActivityManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_INPUT_METHOD_SERVICE).Compare(name)) {
        *object = (IInterface*)CLocalInputMethodManager::GetInstance(
                (IContext*)this->Probe(EIID_IContext));
        (*object)->AddRef();

        return NOERROR;
    }
    else if (!CString(Context_ALARM_SERVICE).Compare(name)) {
//        return getAlarmManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_ACCOUNT_SERVICE).Compare(name)) {
//        return getAccountManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_POWER_SERVICE).Compare(name)) {
//        return getPowerManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_CONNECTIVITY_SERVICE).Compare(name)) {
//        return getConnectivityManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_THROTTLE_SERVICE).Compare(name)) {
//        return getThrottleManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_WIFI_SERVICE).Compare(name)) {
//        return getWifiManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_NOTIFICATION_SERVICE).Compare(name)) {
//        return getNotificationManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_KEYGUARD_SERVICE).Compare(name)) {
//        return new KeyguardManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_ACCESSIBILITY_SERVICE).Compare(name)) {
//        return AccessibilityManager.getInstance(this);
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_LOCATION_SERVICE).Compare(name)) {
//        return getLocationManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_SEARCH_SERVICE).Compare(name)) {
//        return getSearchManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_SENSOR_SERVICE).Compare(name)) {
//        return getSensorManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_STORAGE_SERVICE).Compare(name)) {
//        return getStorageManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_USB_SERVICE).Compare(name)) {
//        return getUsbManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_VIBRATOR_SERVICE).Compare(name)) {
//        return getVibrator();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_STATUS_BAR_SERVICE).Compare(name)) {
//        synchronized (mSync) {
//            if (mStatusBarManager == null) {
//                mStatusBarManager = new StatusBarManager(getOuterContext());
//            }
//            return mStatusBarManager;
//        }
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_AUDIO_SERVICE).Compare(name)) {
//        return getAudioManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_TELEPHONY_SERVICE).Compare(name)) {
//        return getTelephonyManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_CLIPBOARD_SERVICE).Compare(name)) {
        AutoPtr<IClipboardManager> clipboardManager = GetClipboardManager();
        *object = clipboardManager;
        (*object)->AddRef();
        return NOERROR;
    }
    else if (!CString(Context_WALLPAPER_SERVICE).Compare(name)) {
//        return getWallpaperManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_DROPBOX_SERVICE).Compare(name)) {
//        return getDropBoxManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_DEVICE_POLICY_SERVICE).Compare(name)) {
//        return getDevicePolicyManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_UI_MODE_SERVICE).Compare(name)) {
//        return getUiModeManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_DOWNLOAD_SERVICE).Compare(name)) {
//        return getDownloadManager();
        return E_NOT_IMPLEMENTED;
    }
    else if (!CString(Context_NFC_SERVICE).Compare(name)) {
//        return getNfcManager();
        return E_NOT_IMPLEMENTED;
    }

    return E_DOES_NOT_EXIST;
}

AutoPtr<IClipboardManager> CContextImpl::GetClipboardManager()
{
    Mutex::Autolock lock(mSync);
    if (mClipboardManager == NULL) {
        ASSERT_SUCCEEDED(CClipboardManager::New(GetOuterContext(),
                (IClipboardManager**)&mClipboardManager));
    }
    return mClipboardManager;
}

ECode CContextImpl::Init(
    /* [in] */ LoadedCap* capsuleInfo,
    /* [in] */ IBinder* activityToken,
    /* [in] */ CApplicationApartment* apartment)
{
    return Init(capsuleInfo, activityToken, apartment, NULL);
}

ECode CContextImpl::Init(
    /* [in] */ LoadedCap* capsuleInfo,
    /* [in] */ IBinder* activityToken,
    /* [in] */ CApplicationApartment* apartment,
    /* [in] */ IResources* container)
{
    assert(capsuleInfo);

    mCapsuleInfo = capsuleInfo;
    AutoPtr<CResources> res;
    ECode ec = mCapsuleInfo->GetResources(apartment, (CResources**)&res);
    if (FAILED(ec)) return ec;
    mResources = res.Get();

//    if (mResources != null && container != null
//            && container.getCompatibilityInfo().applicationScale !=
//                    mResources.getCompatibilityInfo().applicationScale) {
//        if (DEBUG) {
//            Log.d(TAG, "loaded context has different scaling. Using container's" +
//                    " compatiblity info:" + container.getDisplayMetrics());
//        }
//        mResources = mainThread.getTopLevelResources(
//                mPackageInfo.getResDir(), container.getCompatibilityInfo().copy());
//    }
    mApartment = apartment;
    CApplicationContentResolver::NewByFriend(this, apartment,
            (CApplicationContentResolver**)&mContentResolver);
    SetActivityToken(activityToken);
    return NOERROR;
}

ECode CContextImpl::CreateCapsuleContext(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** context)
{
//    if (packageName.equals("system") || packageName.equals("android")) {
//        return new ContextImpl(mMainThread.getSystemContext());
//    }

    LoadedCap* pi = mApartment->GetCapsuleInfo(capsuleName, flags);
    if (pi != NULL) {
        AutoPtr<CContextImpl> c;
        CContextImpl::NewByFriend((CContextImpl**)&c);
        //c.mRestricted = (flags & CONTEXT_RESTRICTED) == CONTEXT_RESTRICTED;
        c->Init(pi, NULL, mApartment, mResources);
        if (c->mResources != NULL) {
            *context = (IContext*)(CContextImpl*)c;
            if (*context) (*context)->AddRef();
            return NOERROR;
        }
    }

//    // Should be a better exception.
//        throw new CapsuleManager_NameNotFoundException(
//            "Application package " + packageName + " not found");

    *context = NULL;
    return E_DOES_NOT_EXIST;
}

ECode CContextImpl::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* value)
{
    if (permission.IsNull()) {
        // throw new IllegalArgumentException("permission is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (!Process::SupportsProcesses()) {
        *value = CapsuleManager_PERMISSION_GRANTED;
    }

    Int32 pid = Binder::GetCallingPid();
    if (pid != Process::MyPid()) {
        return CheckPermission(permission, pid,
                Binder::GetCallingUid(), value);
    }

    *value = CapsuleManager_PERMISSION_DENIED;
    return NOERROR;
}

ECode CContextImpl::Enforce(
    /* [in] */ CString permission,
    /* [in] */ Int32 resultOfCheck,
    /* [in] */ Boolean selfToo,
    /* [in] */ Int32 uid,
    /* [in] */ CString message)
{
    if (resultOfCheck != CapsuleManager_PERMISSION_GRANTED) {
        // throw new SecurityException(
        //         (message != null ? (message + ": ") : "") +
        //         (selfToo
        //          ? "Neither user " + uid + " nor current process has "
        //          : "User " + uid + " does not have ") +
        //         permission +
        //         ".");
        return E_SECURITY_EXCEPTION;
    }

    return NOERROR;
}

ECode CContextImpl::EnforceCallingOrSelfPermission(
    /* [in] */ CString permission,
    /* [in] */ CString message)
{
    Int32 result;
    CheckCallingOrSelfPermission(permission, &result);
    return Enforce(permission,
            result,
            TRUE,
            Binder::GetCallingUid(),
            message);
}

ECode CContextImpl::CheckPermission(
    /* [in] */ CString permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [out] */ Int32* result)
{
    if (permission.IsNull()) {
        // throw new IllegalArgumentException("permission is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (!Process::SupportsProcesses()) {
        *result = CapsuleManager_PERMISSION_GRANTED;
        return NOERROR;
    }
    // try {
    AutoPtr<IActivityManager> activityManager;
    FAIL_RETURN(ActivityManagerNative::GetDefault((IActivityManager**)&activityManager));
    return activityManager->CheckPermission(String(permission), pid, uid, result);
    // } catch (RemoteException e) {
    //     return CapsuleManager_PERMISSION_DENIED;
    // }
}

ECode CContextImpl::CheckUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* result)
{
    // if (DEBUG) {
    //     Log.i("foo", "checkUriPermission: uri=" + uri + "readPermission="
    //             + readPermission + " writePermission=" + writePermission
    //             + " pid=" + pid + " uid=" + uid + " mode" + modeFlags);
    // }
    if ((modeFlags & Intent_FLAG_GRANT_READ_URI_PERMISSION) != 0) {
        if (readPermission == NULL
                || CheckPermission(readPermission, pid, uid, result)
                == CapsuleManager_PERMISSION_GRANTED) {
            *result = CapsuleManager_PERMISSION_GRANTED;
            return NOERROR;
        }
    }
    if ((modeFlags & Intent_FLAG_GRANT_WRITE_URI_PERMISSION) != 0) {
        if (writePermission == NULL
                || CheckPermission(writePermission, pid, uid, result)
                == CapsuleManager_PERMISSION_GRANTED) {
            *result = CapsuleManager_PERMISSION_GRANTED;
            return NOERROR;
        }
    }

    if (uri != NULL) {
        return CheckUriPermissionEx(uri, pid, uid, modeFlags, result);
    }
    else {
        *result = CapsuleManager_PERMISSION_DENIED;
        return NOERROR;
    }
}

ECode CContextImpl::CheckUriPermissionEx(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* result)
{
    if (!Process::SupportsProcesses()) {
        *result = CapsuleManager_PERMISSION_GRANTED;
        return NOERROR;
    }
    // try {
    AutoPtr<IActivityManager> activityManager;
    FAIL_RETURN(ActivityManagerNative::GetDefault((IActivityManager**)&activityManager));
    return activityManager->CheckUriPermission(uri, pid, uid, modeFlags, result);
    // } catch (RemoteException e) {
    //     return CapsuleManager_PERMISSION_DENIED;
    // }
    return NOERROR;
}

ECode CContextImpl::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    // try {
    AutoPtr<IActivityManager> activityManager;
    FAIL_RETURN(ActivityManagerNative::GetDefault((IActivityManager**)&activityManager));
    return activityManager->RevokeUriPermission(
            (IApplicationApartment*)mApartment, uri,
            modeFlags);
    // } catch (RemoteException e) {
    // }
}

ECode CContextImpl::CheckCallingOrSelfPermission(
    /* [in] */ CString permission,
    /* [out] */ Int32* perm)
{
    if (permission.IsNull()) {
        // throw new IllegalArgumentException("permission is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return CheckPermission(permission, Binder::GetCallingPid(),
            Binder::GetCallingUid(), perm);
}

ECode CContextImpl::GrantUriPermission(
    /* [in] */ const String& toCapsule,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    // try {
    AutoPtr<IActivityManager> activityManager;
    FAIL_RETURN(ActivityManagerNative::GetDefault((IActivityManager**)&activityManager));
    return activityManager->GrantUriPermission(
            (IApplicationApartment*)mApartment, toCapsule, uri,
            modeFlags);
    // } catch (RemoteException e) {
    // }
}

ECode CContextImpl::GetApplicationApartment(
    /* [out] */ IApplicationApartment** apartment)
{
    VALIDATE_NOT_NULL(apartment);
    assert(mApartment != NULL);
    *apartment = (IApplicationApartment*)mApartment;
    (*apartment)->AddRef();

    return NOERROR;
}

void CContextImpl::ScheduleFinalCleanup(
    /* [in] */ const String& who,
    /* [in] */ const String& what)
{
     mApartment->ScheduleContextCleanup(this, who, what);
}

ECode CContextImpl::PerformFinalCleanup(
    /* [in] */ const String& who,
    /* [in] */ const String& what)
{
    //Slog.i(TAG, "Cleanup up context: " + this);
    ECode ec = mCapsuleInfo->RemoveContextRegistrations(GetOuterContext(), who, what);
    return ec;
}

IContext* CContextImpl::GetReceiverRestrictedContext()
{
    if (mReceiverRestrictedContext != NULL) {
        return (IContext*)mReceiverRestrictedContext;
    }
    return mReceiverRestrictedContext = new ReceiverRestrictedContext(GetOuterContext());
}

void CContextImpl::SetActivityToken(
    /* [in] */ IBinder* token)
{
    mActivityToken = token;
}

void CContextImpl::SetOuterContext(
    /* [in] */ IContext* context)
{
    mOuterContext = context;
}

IContext* CContextImpl::GetOuterContext()
{
    return (IContext*)mOuterContext;
}

IBinder* CContextImpl::GetActivityToken()
{
    return mActivityToken;
}
