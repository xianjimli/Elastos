
#ifndef  __SHAREDPREFERENCESBACKUPHELPER_H__
#define  __SHAREDPREFERENCESBACKUPHELPER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include "app/backup/FileBackupHelperBase.h"

class SharedPreferencesBackupHelper
	: public ElRefBase
	, public FileBackupHelperBase
	, public IBackupHelper
{
public:
    /**
     * Construct a helper for backing up and restoring the
     * {@link android.content.SharedPreferences} under the given names.
     *
     * @param context The application {@link android.content.Context}
     * @param prefGroups The names of each {@link android.content.SharedPreferences} file to
     * back up
     */
    SharedPreferencesBackupHelper(
    	/* [in] */ IContext* context,
    	/* [in] */ ArrayOf<String>* prefGroups);

    ~SharedPreferencesBackupHelper();

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI_(PInterface) Probe(
       /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
       /* [in] */ IInterface *pObject,
       /* [out] */ InterfaceID *pIID);

    /**
     * Backs up the configured {@link android.content.SharedPreferences} groups.
     */
    CARAPI PerformBackup(
        /* [in] */ IParcelFileDescriptor* oldState,
        /* [in] */ IBackupDataOutput* data,
        /* [in] */ IParcelFileDescriptor* newState);

    /**
     * Restores one entity from the restore data stream to its proper shared
     * preferences file store.
     */
    CARAPI RestoreEntity(
        /* [in] */ IBackupDataInputStream* data);

    CARAPI WriteNewStateDescription(
        /* [in] */ IParcelFileDescriptor* newState);

private:
    static CString TAG;
    static const Boolean DEBUG = FALSE;

    AutoPtr<IContext> mContext;
    ArrayOf<String>* mPrefGroups;
};

#endif  //__SHAREDPREFERENCESBACKUPHELPER_H__
