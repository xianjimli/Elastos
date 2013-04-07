
#ifndef  __ABSOLUTEFILEBACKUPHELPER_H__
#define  __ABSOLUTEFILEBACKUPHELPER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include "app/backup/FileBackupHelperBase.h"

class AbsoluteFileBackupHelper
	: public ElRefBase
	, public FileBackupHelperBase
	, public IBackupHelper
{
public:
    /**
     * Construct a helper for backing up / restoring the files at the given absolute locations
     * within the file system.
     *
     * @param context
     * @param files
     */
    AbsoluteFileBackupHelper(
    	/* [in] */ IContext* context,
    	/* [in] */ ArrayOf<String>* files);

    ~AbsoluteFileBackupHelper();

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI_(PInterface) Probe(
       /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
       /* [in] */ IInterface *pObject,
       /* [out] */ InterfaceID *pIID);

    /**
     * Based on oldState, determine which of the files from the application's data directory
     * need to be backed up, write them to the data stream, and fill in newState with the
     * state as it exists now.
     */
    CARAPI PerformBackup(
        /* [in] */ IParcelFileDescriptor* oldState,
        /* [in] */ IBackupDataOutput* data,
        /* [in] */ IParcelFileDescriptor* newState);

    /**
     * Restore one absolute file entity from the restore stream
     */
    CARAPI RestoreEntity(
        /* [in] */ IBackupDataInputStream* data);

    CARAPI WriteNewStateDescription(
        /* [in] */ IParcelFileDescriptor* newState);

private:
    static CString TAG;
    static const Boolean DEBUG = FALSE;

    AutoPtr<IContext> mContext;
    ArrayOf<String>*  mFiles;
};

#endif  //__ABSOLUTEFILEBACKUPHELPER_H__
