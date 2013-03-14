
#ifndef  __CLOCALTRANSPORT_H__
#define  __CLOCALTRANSPORT_H__

#include "_CLocalTransport.h"
#include <elastos/AutoPtr.h>

CarClass(CLocalTransport)
{
public:
	CLocalTransport();

	~CLocalTransport();

    CARAPI constructor(
    	/* [in] */ IContext* context);

    CARAPI TransportDirName(
    	/* [out] */ String* dir);

    CARAPI RequestBackupTime(
    	/* [out] */ Int64* time);

    CARAPI InitializeDevice(
    	/* [out] */ Int32* error);

    CARAPI PerformBackup(
    	/* [in] */ ICapsuleInfo* packageInfo,
    	/* [in] */ IParcelFileDescriptor* data,
    	/* [out] */ Int32* error);

    CARAPI ClearBackupData(
        /* [in] */ ICapsuleInfo* packageInfo,
        /* [out] */ Int32* error);

    CARAPI FinishBackup(
        /* [out] */ Int32* error);

    // Restore handling
    CARAPI GetAvailableRestoreSets(
        /* [out, callee] */ ArrayOf<IRestoreSet*> ** set);

    CARAPI GetCurrentRestoreSet(
        /* [out] */ Int64* set);

    CARAPI StartRestore(
        /* [in] */ Int64 token,
        /* [in] */ ArrayOf<ICapsuleInfo*>* packages,
        /* [out] */ Int32* error);

    CARAPI NextRestorePackage(
        /* [out] */ String* cap);

    CARAPI GetRestoreData(
    	/* [in] */ IParcelFileDescriptor* outFd,
        /* [out] */ Int32* data);

    CARAPI FinishRestore();

private:
    // Deletes the contents but not the given directory
    CARAPI_(void) DeleteContents(
    	/* [in] */ IFile* dirname);

private:
    static const CString TAG;
    static const Boolean DEBUG = TRUE;
    static const CString TRANSPORT_DIR_NAME;

    // The single hardcoded restore set always has the same (nonzero!) token
    static const Int64 RESTORE_TOKEN = 1;

    AutoPtr<IContext> mContext;
    AutoPtr<ICapsuleManager> mPackageManager;
    AutoPtr<IFile> mDataDir;
    ArrayOf<ICapsuleInfo*> * mRestorePackages;
    Int32 mRestorePackage;  // Index into mRestorePackages
};

#endif  //__CLOCALTRANSPORT_H__
