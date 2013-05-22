
#ifndef __CSTORAGEVOLUME_H__
#define __CSTORAGEVOLUME_H__

#include "_CStorageVolume.h"

CarClass(CStorageVolume)
{
public:
    CStorageVolume();

    CARAPI constructor(
        /* [in] */ CString path,
        /* [in] */ Int32 descriptionId,
        /* [in] */ Boolean removable,
        /* [in] */ Boolean emulated,
        /* [in] */ Int32 mtpReserveSpace,
        /* [in] */ Boolean allowMassStorage,
        /* [in] */ Int64 maxFileSize);

    CARAPI constructor(
        /* [in] */ CString path,
        /* [in] */ Int32 descriptionId,
        /* [in] */ Boolean removable,
        /* [in] */ Boolean emulated,
        /* [in] */ Int32 mtpReserveSpace,
        /* [in] */ Int32 storageId,
        /* [in] */ Boolean allowMassStorage,
        /* [in] */ Int64 maxFileSize);

    CARAPI GetPath(
        /* [out] */ CString * pMPath);

    CARAPI SetPath(
        /* [in] */ CString path,
        /* [out] */ CString * pMPath);

    CARAPI GetDescription(
        /* [in] */ IContext * pContext,
        /* [out] */ CString * pDesc);

    CARAPI GetDescriptionId(
        /* [out] */ Int32 * pMDescriptionId);

    CARAPI IsRemovable(
        /* [out] */ Boolean * pMRemovable);

    CARAPI IsEmulated(
        /* [out] */ Boolean * pMEmulated);

    CARAPI GetStorageId(
        /* [out] */ Int32 * pMStorageId);

    CARAPI SetStorageId(
        /* [in] */ Int32 index);

    CARAPI GetMtpReserveSpace(
        /* [out] */ Int32 * pMMtpReserveSpace);

    CARAPI AllowMassStorage(
        /* [out] */ Boolean * pMAllowMassStorage);

    CARAPI GetMaxFileSize(
        /* [out] */ Int64 * pMMaxFileSize);

    CARAPI Equals(
        /* [in] */ IObjectContainer * pObj,
        /* [out] */ Boolean * pEqu);

    CARAPI HashCode(
        /* [out] */ Int32 * pCode);

    CARAPI ToString(
        /* [out] */ CString * pStr);

    CARAPI DescribeContents(
        /* [out] */ Int32 * pContents);

    CARAPI WriteToParcel(
        /* [in] */ IParcel * pParcel,
        /* [in] */ Int32 flags);

    static const String EXTRA_STORAGE_VOLUME;// = "storage_volume";

private:
    static const char* TAG;// = "StorageVolume";
    CString mPath;
    Int32 mDescriptionId;
    Boolean mRemovable;
    Boolean mEmulated;
    Int32 mMtpReserveSpace;
    Boolean mAllowMassStorage;
    Int32 mStorageId;
    // maximum file size for the storage, or zero for no limit
    Int64 mMaxFileSize;

    // StorageVolume extra for ACTION_MEDIA_REMOVED, ACTION_MEDIA_UNMOUNTED, ACTION_MEDIA_CHECKING,
    // ACTION_MEDIA_NOFS, ACTION_MEDIA_MOUNTED, ACTION_MEDIA_SHARED, ACTION_MEDIA_UNSHARED,
    // ACTION_MEDIA_BAD_REMOVAL, ACTION_MEDIA_UNMOUNTABLE and ACTION_MEDIA_EJECT broadcasts.
};

#endif // __CSTORAGEVOLUME_H__
