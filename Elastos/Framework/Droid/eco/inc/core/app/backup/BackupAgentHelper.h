
#ifndef  __BACKUPAGENTHELPER_H__
#define  __BACKUPAGENTHELPER_H__

#include "app/backup/BackupAgent.h"
#include "app/backup/BackupHelperDispatcher.h"

/**
 * A convenient {@link BackupAgent} wrapper class that automatically manages
 * heterogeneous data sets within the backup data, each identified by a unique
 * key prefix.  When processing a backup or restore operation, the BackupAgentHelper
 * dispatches to one or more installed {@link BackupHelper} objects, each
 * of which is responsible for a defined subset of the data being processed.
 * <p>
 * An application will typically extend this class in its own
 * backup agent. Then, within the agent's {@link BackupAgent#onCreate() onCreate()}
 * method, it will call {@link #addHelper(String, BackupHelper) addHelper()} one or more times to
 * install the handlers for each kind of data it wishes to manage within its backups.
 * <p>
 * The Android framework currently provides two predefined {@link BackupHelper} classes:</p>
 * <ul><li>{@link FileBackupHelper} - Manages the backup and restore of entire files
 * within an application's data directory hierarchy.</li>
 * <li>{@link SharedPreferencesBackupHelper} - Manages the backup and restore of an
 * application's {@link android.content.SharedPreferences} data.</li></ul>
 * <p>
 * An application can also implement its own helper classes to work within the
 * {@link BackupAgentHelper} framework.  See the {@link BackupHelper} interface
 * documentation for details.
 *
 * @see BackupHelper
 * @see FileBackupHelper
 * @see SharedPreferencesBackupHelper
 */
class BackupAgentHelper:
    public BackupAgent
{
public:
    BackupAgentHelper();

    ~BackupAgentHelper();

    /**
     * Run the backup process on each of the configured handlers.
     */
    //@Override
    CARAPI_(void) OnBackup(
        /* [in] */ IParcelFileDescriptor* oldState,
        /* [in] */ IBackupDataOutput* data,
        /* [in] */ IParcelFileDescriptor* newState);
    /**
     * Run the restore process on each of the configured handlers.
     */
    //@Override
    CARAPI_(void) OnRestore(
        /* [in] */ IBackupDataInput* data,
        /* [in] */ Int32 appVersionCode,
        /* [in] */ IParcelFileDescriptor* newState);

    /** @hide */
    CARAPI_(AutoPtr<BackupHelperDispatcher>) GetDispatcher();

    /**
     * Add a helper for a given data subset to the agent's configuration.  Each helper
     * must have a prefix string that is unique within this backup agent's set of
     * helpers.
     *
     * @param keyPrefix A string used to disambiguate the various helpers within this agent
     * @param helper A backup/restore helper object to be invoked during backup and restore
     *    operations.
     */
    CARAPI_(void) AddHelper(
        /* [in] */ const String& keyPrefix,
        /* [in] */ IBackupHelper* helper);

private:
    static CString TAG;

    AutoPtr<BackupHelperDispatcher> mDispatcher;
};

#endif  //__BACKUPAGENTHELPER_H__
