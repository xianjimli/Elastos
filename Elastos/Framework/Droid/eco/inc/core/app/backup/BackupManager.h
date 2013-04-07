
#ifndef  __BACKUPMANAGER_H__
#define  __BACKUPMANAGER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "app/backup/RestoreSession.h"


class BackupManager
{
public:
    /**
     * Constructs a BackupManager object through which the application can
     * communicate with the Android backup system.
     *
     * @param context The {@link android.content.Context} that was provided when
     *                one of your application's {@link android.app.Activity Activities}
     *                was created.
     */
    BackupManager(
        /* [in] */ IContext* context);

    /**
     * Notifies the Android backup system that your application wishes to back up
     * new changes to its data.  A backup operation using your application's
     * {@link android.app.backup.BackupAgent} subclass will be scheduled when you
     * call this method.
     */
    CARAPI_(void) DataChanged();

    /**
     * Convenience method for callers who need to indicate that some other package
     * needs a backup pass.  This can be useful in the case of groups of packages
     * that share a uid.
     * <p>
     * This method requires that the application hold the "android.permission.BACKUP"
     * permission if the package named in the argument does not run under the same uid
     * as the caller.
     *
     * @param capsuleName The package name identifying the application to back up.
     */
    static CARAPI_(void) DataChanged(
        /* [in] */ String capsuleName);

    /**
     * Restore the calling application from backup.  The data will be restored from the
     * current backup dataset if the application has stored data there, or from
     * the dataset used during the last full device setup operation if the current
     * backup dataset has no matching data.  If no backup data exists for this application
     * in either source, a nonzero value will be returned.
     *
     * <p>If this method returns zero (meaning success), the OS will attempt to retrieve
     * a backed-up dataset from the remote transport, instantiate the application's
     * backup agent, and pass the dataset to the agent's
     * {@link android.app.backup.BackupAgent#onRestore(BackupDataInput, Int32, android.os.ParcelFileDescriptor) onRestore()}
     * method.
     *
     * @param observer The {@link RestoreObserver} to receive callbacks during the restore
     * operation. This must not be NULL.
     *
     * @return Zero on success; nonzero on error.
     */
    CARAPI_(Int32) RequestRestore(
        /* [in] */ IRestoreObserver* observer);

    /**
     * Begin the process of restoring data from backup.  See the
     * {@link android.app.backup.RestoreSession} class for documentation on that process.
     * @hide
     */
    CARAPI_(AutoPtr<RestoreSession>) BeginRestoreSession();

private:
    static CARAPI_(void) CheckServiceBinder();

private:
    static CString TAG;

    AutoPtr<IContext> mContext;
    static AutoPtr<IBackupManager> sService;
};

#endif  //__BACKUPMANAGER_H__
