
#include "os/storage/CStorageVolume.h"

CStorageVolume::CStorageVolume(){}

ECode CStorageVolume::constructor(
    /* [in] */ CString path,
    /* [in] */ Int32 descriptionId,
    /* [in] */ Boolean removable,
    /* [in] */ Boolean emulated,
    /* [in] */ Int32 mtpReserveSpace,
    /* [in] */ Boolean allowMassStorage,
    /* [in] */ Int64 maxFileSize)
{
    mPath = path;
    mDescriptionId = descriptionId;
    mRemovable = removable;
    mEmulated = emulated;
    mMtpReserveSpace = mtpReserveSpace;
    mAllowMassStorage = allowMassStorage;
    mMaxFileSize = maxFileSize;
}

ECode CStorageVolume::constructor(
    /* [in] */ CString path,
    /* [in] */ Int32 descriptionId,
    /* [in] */ Boolean removable,
    /* [in] */ Boolean emulated,
    /* [in] */ Int32 mtpReserveSpace,
    /* [in] */ Int32 storageId,
    /* [in] */ Boolean allowMassStorage,
    /* [in] */ Int64 maxFileSize)
{
    mPath = path;
    mDescriptionId = descriptionId;
    mRemovable = removable;
    mEmulated = emulated;
    mMtpReserveSpace = mtpReserveSpace;
    mAllowMassStorage = allowMassStorage;
    mStorageId = storageId;
    mMaxFileSize = maxFileSize;
}

/**
* Returns the mount path for the volume.
*
* @return the mount path
*/
ECode CStorageVolume::GetPath(
    /* [out] */ CString * pMPath)
{
    *pMPath = mPath;
}

ECode CStorageVolume::SetPath(
    /* [in] */ CString path,
    /* [out] */ CString * pMPath)
{
    mPath = path;
    *pMPath = mPath;
}

/**
* Returns a user visible description of the volume.
*
* @return the volume description
*/
ECode CStorageVolume::GetDescription(
    /* [in] */ IContext * pContext,
    /* [out] */ CString * pDesc)
{
//    *pDesc = pContext->GetResources().GetString(mDescriptionId);
}

ECode CStorageVolume::GetDescriptionId(
    /* [out] */ Int32 * pMDescriptionId)
{
    *pMDescriptionId = mDescriptionId;
}

/**
* Returns true if the volume is removable.
*
* @return is removable
*/
ECode CStorageVolume::IsRemovable(
    /* [out] */ Boolean * pMRemovable)
{
    *pMRemovable = mRemovable;
}

/**
* Returns true if the volume is emulated.
*
* @return is removable
*/
ECode CStorageVolume::IsEmulated(
    /* [out] */ Boolean * pMEmulated)
{
    *pMEmulated = mEmulated;
}

/**
* Returns the MTP storage ID for the volume.
* this is also used for the storage_id column in the media provider.
*
* @return MTP storage ID
*/
ECode CStorageVolume::GetStorageId(
    /* [out] */ Int32 * pMStorageId)
{
    *pMStorageId = mStorageId;
}

/**
* Do not call this unless you are MountService
*/
ECode CStorageVolume::SetStorageId(
    /* [in] */ Int32 index)
{
// storage ID is 0x00010001 for primary storage,
// then 0x00020001, 0x00030001, etc. for secondary storages
    mStorageId = ((index + 1) << 16) + 1;
}

/**
* Number of megabytes of space to leave unallocated by MTP.
* MTP will subtract this value from the free space it reports back
* to the host via GetStorageInfo, and will not allow new files to
* be added via MTP if there is less than this amount left free in the storage.
* If MTP has dedicated storage this value should be zero, but if MTP is
* sharing storage with the rest of the system, set this to a positive value
* to ensure that MTP activity does not result in the storage being
* too close to full.
*
* @return MTP reserve space
*/
ECode CStorageVolume::GetMtpReserveSpace(
    /* [out] */ Int32 * pMMtpReserveSpace)
{
    *pMMtpReserveSpace = mMtpReserveSpace;
}

/**
* Returns true if this volume can be shared via USB mass storage.
*
* @return whether mass storage is allowed
*/
ECode CStorageVolume::AllowMassStorage(
    /* [out] */ Boolean * pMAllowMassStorage)
{
    *pMAllowMassStorage = mAllowMassStorage;
}

/**
* Returns maximum file size for the volume, or zero if it is unbounded.
*
* @return maximum file size
*/
ECode CStorageVolume::GetMaxFileSize(
    /* [out] */ Int64 * pMMaxFileSize)
{
    *pMMaxFileSize = mMaxFileSize;
}

ECode CStorageVolume::Equals(
    /* [in] */ IObjectContainer * pObj,
    /* [out] */ Boolean * pEqu)
{
//    if (obj instanceof StorageVolume && mPath != null) {
//    	StorageVolume volume = (StorageVolume)obj;
//	return (mPath.equals(volume.mPath));
//    }
    *pEqu = FALSE;
}

ECode CStorageVolume::HashCode(
    /* [out] */ Int32 * pCode)
{
//    return mPath.hashCode();
}

ECode CStorageVolume::ToString(
    /* [out] */ CString * pStr)
{
//    return "StorageVolume [mAllowMassStorage=" + mAllowMassStorage + ", mDescriptionId="
//	+ mDescriptionId + ", mEmulated=" + mEmulated + ", mMaxFileSize=" + mMaxFileSize
//	+ ", mMtpReserveSpace=" + mMtpReserveSpace + ", mPath=" + mPath + ", mRemovable="
//	+ mRemovable + ", mStorageId=" + mStorageId + "]";
}

ECode CStorageVolume::DescribeContents(
    /* [out] */ Int32 * pContents)
{
    *pContents = 0;
}

ECode CStorageVolume::WriteToParcel(
    /* [in] */ IParcel * pParcel,
    /* [in] */ Int32 flags)
{
    pParcel->WriteString(String(mPath));
    pParcel->WriteInt32(mDescriptionId);
    pParcel->WriteInt32(mRemovable ? 1 : 0);
    pParcel->WriteInt32(mEmulated ? 1 : 0);
    pParcel->WriteInt32(mStorageId);
    pParcel->WriteInt32(mMtpReserveSpace);
    pParcel->WriteInt32(mAllowMassStorage ? 1 : 0);
    pParcel->WriteInt64(mMaxFileSize);
}

