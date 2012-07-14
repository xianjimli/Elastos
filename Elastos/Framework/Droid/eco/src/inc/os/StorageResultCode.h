
#ifndef __STORAGERESULTCODE_H__
#define __STORAGERESULTCODE_H__

#include "ext/frameworkext.h"

/**
 * Class that provides access to constants returned from StorageManager
 * and lower level MountService APIs.
 *
 * @hide
 */
class StorageResultCode
{
public:
    /**
     * Operation succeeded.
     * @see android.os.storage.StorageManager
     */
    static const Int32 OperationSucceeded               =  0;

    /**
     * Operation failed: Internal error.
     * @see android.os.storage.StorageManager
     */
    static const Int32 OperationFailedInternalError     = -1;

    /**
     * Operation failed: Missing media.
     * @see android.os.storage.StorageManager
     */
    static const Int32 OperationFailedNoMedia           = -2;

    /**
     * Operation failed: Media is blank.
     * @see android.os.storage.StorageManager
     */
    static const Int32 OperationFailedMediaBlank        = -3;

    /**
     * Operation failed: Media is corrupt.
     * @see android.os.storage.StorageManager
     */
    static const Int32 OperationFailedMediaCorrupt      = -4;

    /**
     * Operation failed: Storage not mounted.
     * @see android.os.storage.StorageManager
     */
    static const Int32 OperationFailedStorageNotMounted  = -5;

    /**
     * Operation failed: Storage is mounted.
     * @see android.os.storage.StorageManager
     */
    static const Int32 OperationFailedStorageMounted     = -6;

    /**
     * Operation failed: Storage is busy.
     * @see android.os.storage.StorageManager
     */
    static const Int32 OperationFailedStorageBusy        = -7;
};

#endif //__STORAGERESULTCODE_H__
