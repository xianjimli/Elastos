
#include "os/storage/CStorageManager.h"
ECode CStorageManager::GetNextNonce(
    /* [out] */ Int32 * pPath)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStorageManager::RegisterListener(
    /* [in] */ IStorageEventListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStorageManager::UnregisterListener(
    /* [in] */ IStorageEventListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStorageManager::EnableUsbMassStorage()
{
    mMountService->SetUsbMassStorageEnabled(TRUE);
}

ECode CStorageManager::DisableUsbMassStorage()
{
    mMountService->SetUsbMassStorageEnabled(FALSE);
}

ECode CStorageManager::IsUsbMassStorageConnected(
    /* [out] */ Boolean * pConnect)
{
//    *pConnect = mMountService->IsUsbMassStorageConnected();
}

ECode CStorageManager::IsUsbMassStorageEnabled(
    /* [out] */ Boolean * pEnabled)
{
//    *pEnabled = mMountService->IsUsbMassStorageEnabled();
}

ECode CStorageManager::IsUsbMassStorageAllEnabled(
    /* [out] */ Boolean * pEnabled)
{
//    *pEnabled = mMountService->IsUsbMassStorageAllEnabled();
}

ECode CStorageManager::MountObb(
    /* [in] */ CString filename,
    /* [in] */ CString key,
    /* [in] */ IOnObbStateChangeListener * pListener,
    /* [out] */ Boolean * pMounted)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStorageManager::UnmountObb(
    /* [in] */ CString filename,
    /* [in] */ Boolean force,
    /* [in] */ IOnObbStateChangeListener * pListener,
    /* [out] */ Boolean * pMounted)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStorageManager::IsObbMounted(
    /* [in] */ CString filename,
    /* [out] */ Boolean * pMounted)
{
//    *pMounted = mMountService->IsObbMounted(String(filename));
}

ECode CStorageManager::GetMountedObbPath(
    /* [in] */ CString filename,
    /* [out] */ CString * pObbpath)
{
//    *pObbpath = mMountService->GetMountedObbPath(String(filename));
}

ECode CStorageManager::GetVolumeState(
    /* [in] */ CString mountPoint,
    /* [out] */ CString * pState)
{
//    *pState = mMountService->GetVolumeState(String(mountPoint));
}

ECode CStorageManager::GetVolumeList(
    /* [out, callee] */ ArrayOf<IStorageVolume *> ** ppVolumelists)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStorageManager::GetVolumePaths(
    /* [out, callee] */ ArrayOf<String> ** ppVolumepaths)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStorageManager::GetVolumeFSLabel(
    /* [in] */ CString mountPoint,
    /* [out] */ CString * pLabel)
{
//    *pLabel = mMountService->GetVolumeFSLabel(String(mountPoint));
}

