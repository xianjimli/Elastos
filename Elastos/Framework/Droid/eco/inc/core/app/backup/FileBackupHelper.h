
#ifndef  __FILEBACKUPHELPER_H__
#define  __FILEBACKUPHELPER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include "app/backup/FileBackupHelperBase.h"

class FileBackupHelper
	: public ElRefBase
	, public FileBackupHelperBase
	, public IBackupHelper
{
public:
    /**
     * Construct a helper to manage backup/restore of entire files within the
     * application's data directory hierarchy.
     *
     * @param context The backup agent's Context object
     * @param files A list of the files to be backed up or restored.
     */
    FileBackupHelper(
    	/* [in] */ IContext* context,
    	/* [in] */ ArrayOf<String>* files);

    ~FileBackupHelper();

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI_(PInterface) Probe(
       /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
       /* [in] */ IInterface *pObject,
       /* [out] */ InterfaceID *pIID);

    /**
     * Based on <code>oldState</code>, determine which of the files from the
     * application's data directory need to be backed up, write them to the data
     * stream, and fill in <code>newState</code> with the state as it exists
     * now. When <code>oldState</code> is <code>null</code>, all the files will
     * be backed up.
     * <p>
     * This should only be called directly from within the {@link BackupAgentHelper}
     * implementation. See
     * {@link android.app.backup.BackupAgent#onBackup(ParcelFileDescriptor, BackupDataOutput, ParcelFileDescriptor)}
     * for a description of parameter meanings.
     */
    CARAPI PerformBackup(
        /* [in] */ IParcelFileDescriptor* oldState,
        /* [in] */ IBackupDataOutput* data,
        /* [in] */ IParcelFileDescriptor* newState);

    /**
     * Restore one record [representing a single file] from the restore dataset.
     * <p>
     * This should only be called directly from within the {@link BackupAgentHelper}
     * implementation.
     */
    CARAPI RestoreEntity(
        /* [in] */ IBackupDataInputStream* data);

    CARAPI WriteNewStateDescription(
        /* [in] */ IParcelFileDescriptor* newState);

private:
    static CString TAG;
    static const Boolean DEBUG = FALSE;

    AutoPtr<IContext> mContext;
    AutoPtr<IFile> mFilesDir;
    ArrayOf<String>* mFiles;
};

#endif  //__FILEBACKUPHELPER_H__
