
#ifndef  __FILEBACKUPHELPERBASE_H__
#define  __FILEBACKUPHELPERBASE_H__

#include "ext/frameworkext.h"
#include <utils/BackupHelpers.h>
#include <elastos/AutoPtr.h>

class FileBackupHelperBase
{
public:
    FileBackupHelperBase(
    	/* [in] */ IContext* context);

    ~FileBackupHelperBase();

    CARAPI_(void) WriteNewStateDescription(
    	/* [in] */ IParcelFileDescriptor* fd);

protected:
    CARAPI_(void) Finalize();

    /**
     * Check the parameters so the native code doesn't have to throw all the exceptions
     * since it's easier to do that from Java.
     */
    static CARAPI_(void) PerformBackup_checked(
    	/* [in] */ IParcelFileDescriptor* oldState,
    	/* [in] */ IBackupDataOutput* data,
        /* [in] */ IParcelFileDescriptor* newState,
        /* [in] */ ArrayOf<String>* files,
        /* [in] */ ArrayOf<String>* keys);

    CARAPI_(void) WriteFile(
    	/* [in] */ IFile* f,
    	/* [in] */ IBackupDataInputStream* in);

    CARAPI_(Boolean) IsKeyInList(
    	/* [in] */ const String& key,
    	/* [in] */ ArrayOf<String>* list);

private:
    static CARAPI_(android::RestoreHelperBase*) ctor_native();

    CARAPI_(void) dtor_native();

    static CARAPI_(Int32) performBackup_native(
    	/* [in] */ IFileDescriptor* oldState,
        /* [in] */ android::BackupDataWriter* data,
        /* [in] */ IFileDescriptor* newState,
        /* [in] */ ArrayOf<String>* files,
        /* [in] */ ArrayOf<String>* keys);

    static CARAPI_(Int32) writeFile_native(
    	/* [in] */ android::RestoreHelperBase* restore,
    	/* [in] */ const String& filename,
    	/* [in] */ android::BackupDataReader* reader);

    static CARAPI_(Int32) writeSnapshot_native(
    	/* [in] */ android::RestoreHelperBase* restore,
    	/* [in] */ IFileDescriptor* fd);

private:
	static CString TAG;
    android::RestoreHelperBase* mPtr;
    AutoPtr<IContext> mContext;
    Boolean mExceptionLogged;
};

#endif  //__FILEBACKUPHELPERBASE_H__
