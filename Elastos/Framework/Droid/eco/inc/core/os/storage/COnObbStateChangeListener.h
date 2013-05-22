
#ifndef __CONOBBSTATECHANGELISTENER_H__
#define __CONOBBSTATECHANGELISTENER_H__

#include "_COnObbStateChangeListener.h"

CarClass(COnObbStateChangeListener)
{
public:
    /**
     * The OBB container is now mounted and ready for use. Returned in status
     * messages from calls made via {@link StorageManager}
     */
    static const Int32 MOUNTED = 1;

    /**
     * The OBB container is now unmounted and not usable. Returned in status
     * messages from calls made via {@link StorageManager}
     */
    static const Int32 UNMOUNTED = 2;

    /**
     * There was an internal system error encountered while trying to mount the
     * OBB. Returned in status messages from calls made via
     * {@link StorageManager}
     */
    static const Int32 ERROR_INTERNAL = 20;

    /**
     * The OBB could not be mounted by the system. Returned in status messages
     * from calls made via {@link StorageManager}
     */
    static const Int32 ERROR_COULD_NOT_MOUNT = 21;

    /**
     * The OBB could not be unmounted. This most likely indicates that a file is
     * in use on the OBB. Returned in status messages from calls made via
     * {@link StorageManager}
     */
    static const Int32 ERROR_COULD_NOT_UNMOUNT = 22;

    /**
     * A call was made to unmount the OBB when it was not mounted. Returned in
     * status messages from calls made via {@link StorageManager}
     */
    static const Int32 ERROR_NOT_MOUNTED = 23;

    /**
     * The OBB has already been mounted. Returned in status messages from calls
     * made via {@link StorageManager}
     */
    static const Int32 ERROR_ALREADY_MOUNTED = 24;

    /**
     * The current application does not have permission to use this OBB. This
     * could be because the OBB indicates it's owned by a different package or
     * some other error. Returned in status messages from calls made via
     * {@link StorageManager}
     */
    static const Int32 ERROR_PERMISSION_DENIED = 25;

public:

    /**
     * Called when an OBB has changed states.
     * 
     * @param path path to the OBB file the state change has happened on
     * @param state the current state of the OBB
     */
    CARAPI OnObbStateChange(
        /* [in] */ CString path,
        /* [in] */ Int32 state);

private:
    // TODO: Add your private member variables here.
};

#endif // __CONOBBSTATECHANGELISTENER_H__
