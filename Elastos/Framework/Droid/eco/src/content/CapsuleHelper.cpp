
#include "content/CapsuleHelper.h"
#include "os/ServiceManager.h"
#include "os/StorageResultCode.h"

const Int32 CapsuleHelper::RECOMMEND_INSTALL_INTERNAL;
const Int32 CapsuleHelper::RECOMMEND_INSTALL_EXTERNAL;
const Int32 CapsuleHelper::RECOMMEND_FAILED_INSUFFICIENT_STORAGE;
const Int32 CapsuleHelper::RECOMMEND_FAILED_INVALID_APK;
const Int32 CapsuleHelper::RECOMMEND_FAILED_INVALID_LOCATION;
const Int32 CapsuleHelper::RECOMMEND_FAILED_ALREADY_EXISTS;
const Int32 CapsuleHelper::RECOMMEND_MEDIA_UNAVAILABLE;

const Int32 CapsuleHelper::APP_INSTALL_AUTO;
const Int32 CapsuleHelper::APP_INSTALL_INTERNAL;
const Int32 CapsuleHelper::APP_INSTALL_EXTERNAL;

const Boolean CapsuleHelper::mLocalLOGV;
CString CapsuleHelper::TAG = "CapsuleHelper";

AutoPtr<IMountService> CapsuleHelper::GetMountService()
{
    AutoPtr<IMountService> service;
    service = IMountService::Probe(ServiceManager::GetService(String("mount")).Get());
    if (service != NULL) {
        return service;
    }
    else {
//        Log.e(TAG, "Can't get mount service");
    }
    return NULL;
}

String CapsuleHelper::CreateSdDir(
    /* [in] */ Int64 sizeBytes,
    /* [in] */ const String& cid,
    /* [in] */ const String& sdEncKey,
    /* [in] */ Int32 uid)
{
    // Create mount point via MountService
    AutoPtr<IMountService> mountService = GetMountService();
    Int32 sizeMb = (Int32)(sizeBytes >> 20);
    if ((sizeBytes - (sizeMb * 1024 * 1024)) > 0) {
        sizeMb++;
    }
    // Add buffer size
    sizeMb++;
//    if (localLOGV)
//        Log.i(TAG, "Size of container " + sizeMb + " MB " + sizeBytes + " bytes");

//    try {
    Int32 rc;
    mountService->CreateSecureContainer(
            cid, sizeMb, String("fat"), sdEncKey, uid, &rc);
    if (rc != StorageResultCode::OperationSucceeded) {
//        Log.e(TAG, "Failed to create secure container " + cid);
        return String(NULL);
    }
    String cachePath;
    mountService->GetSecureContainerPath(cid, &cachePath);
//    if (localLOGV) Log.i(TAG, "Created secure container " + cid +
//            " at " + cachePath);
    return cachePath;
//    } catch (RemoteException e) {
//        Log.e(TAG, "MountService running?");
//    }
//    return null;
}

String CapsuleHelper::MountSdDir(
    /* [in] */ const String& cid,
    /* [in] */ const String& key,
    /* [in] */ Int32 ownerUid)
{
//    try {
    Int32 rc;
    GetMountService()->MountSecureContainer(cid, key, ownerUid, &rc);
    if (rc != StorageResultCode::OperationSucceeded) {
//        Log.i(TAG, "Failed to mount container " + cid + " rc : " + rc);
        return String(NULL);
    }
    String path;
    GetMountService()->GetSecureContainerPath(cid, &path);
    return path;
//    } catch (RemoteException e) {
//        Log.e(TAG, "MountService running?");
//    }
//    return null;
}

Boolean CapsuleHelper::UnMountSdDir(
    /* [in] */ const String& cid)
{
//    try {
    Int32 rc;
    GetMountService()->UnmountSecureContainer(cid, TRUE, &rc);
    if (rc != StorageResultCode::OperationSucceeded) {
//        Log.e(TAG, "Failed to unmount " + cid + " with rc " + rc);
        return FALSE;
    }
    return TRUE;
//    } catch (RemoteException e) {
//        Log.e(TAG, "MountService running?");
//    }
//    return false;
}

Boolean CapsuleHelper::RenameSdDir(
    /* [in] */ const String& oldId,
    /* [in] */ const String& newId)
{
//    try {
    Int32 rc;
    GetMountService()->RenameSecureContainer(oldId, newId, &rc);
    if (rc != StorageResultCode::OperationSucceeded) {
//        Log.e(TAG, "Failed to rename " + oldId + " to " +
//                newId + "with rc " + rc);
        return FALSE;
    }
    return TRUE;
//    } catch (RemoteException e) {
//        Log.i(TAG, "Failed ot rename  " + oldId + " to " + newId +
//                " with exception : " + e);
//    }
//    return false;
}

String CapsuleHelper::GetSdDir(
    /* [in] */ const String& cid)
{
//    try {
    String path;
    GetMountService()->GetSecureContainerPath(cid, &path);
    return path;
//    } catch (RemoteException e) {
//        Log.e(TAG, "Failed to get container path for " + cid +
//            " with exception " + e);
//    }
//    return null;
}

Boolean CapsuleHelper::FinalizeSdDir(
    /* [in] */ const String& cid)
{
//    try {
    Int32 rc;
    GetMountService()->FinalizeSecureContainer(cid, &rc);
    if (rc != StorageResultCode::OperationSucceeded) {
//        Log.i(TAG, "Failed to finalize container " + cid);
        return FALSE;
    }
    return TRUE;
//    } catch (RemoteException e) {
//        Log.e(TAG, "Failed to finalize container " + cid +
//                " with exception " + e);
//    }
//    return false;
}

Boolean CapsuleHelper::DestroySdDir(
    /* [in] */ const String& cid)
{
//    try {
//    if (localLOGV) Log.i(TAG, "Forcibly destroying container " + cid);
    Int32 rc;
    GetMountService()->DestroySecureContainer(cid, TRUE, &rc);
    if (rc != StorageResultCode::OperationSucceeded) {
//        Log.i(TAG, "Failed to destroy container " + cid);
        return FALSE;
    }
    return TRUE;
//    } catch (RemoteException e) {
//        Log.e(TAG, "Failed to destroy container " + cid +
//                " with exception " + e);
//    }
//    return false;
}

ECode CapsuleHelper::GetSecureContainerList(
    /* [out, callee] */ ArrayOf<String>** list)
{
    VALIDATE_NOT_NULL(list);

//    try {
    return GetMountService()->GetSecureContainerList(list);
//    } catch (RemoteException e) {
//        Log.e(TAG, "Failed to get secure container list with exception" +
//                e);
//    }
//    return null;
}

Boolean CapsuleHelper::IsContainerMounted(
    /* [in] */ const String& cid)
{
//    try {
    Boolean isMounted;
    GetMountService()->IsSecureContainerMounted(cid, &isMounted);
    return isMounted;
//    } catch (RemoteException e) {
//        Log.e(TAG, "Failed to find out if container " + cid + " mounted");
//    }
//    return false;
}
