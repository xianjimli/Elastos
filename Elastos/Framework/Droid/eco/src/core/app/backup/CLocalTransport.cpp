
#include "app/backup/CLocalTransport.h"
#include "app/backup/BackupConstants.h"
#include "os/Environment.h"

const CString CLocalTransport::TAG = "LocalTransport";
const Boolean CLocalTransport::DEBUG;
const CString CLocalTransport::TRANSPORT_DIR_NAME
        = "com.android.internal.backup.LocalTransport";

const Int64 CLocalTransport::RESTORE_TOKEN;

CLocalTransport::CLocalTransport():
	mRestorePackages(NULL),
	mRestorePackage(-1)
{
	CFile::New(Environment::GetDownloadCacheDirectory(),
        String("backup"), (IFile**) &mDataDir);
}

CLocalTransport::~CLocalTransport()
{
	if (mRestorePackages != NULL) {
		ArrayOf<ICapsuleInfo*>::Free(mRestorePackages);
	}
}

ECode CLocalTransport::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;

    AutoPtr<IServiceManager> serviceManager;
    Elastos::GetServiceManager((IServiceManager**)&serviceManager);
    assert(serviceManager != NULL);
    serviceManager->GetService(String("capsule"),
            (IInterface**)(ICapsuleManager**)&mPackageManager);
    assert(mPackageManager != NULL);

    return NOERROR;
}


ECode CLocalTransport::TransportDirName(
    /* [out] */ String* dir)
{
    assert(dir != NULL);
    *dir = String((const char*)TRANSPORT_DIR_NAME);
    return NOERROR;
}

ECode CLocalTransport::RequestBackupTime(
    /* [out] */ Int64* time)
{
    // any time is a good time for local backup
    assert(time != NULL);
    *time = 0;
    return NOERROR;
}

ECode CLocalTransport::InitializeDevice(
    /* [out] */ Int32* error)
{
    assert(error != NULL);
    //if (DEBUG) Log.v(TAG, "wiping all data");
    DeleteContents(mDataDir);
    *error = BackupConstants::TRANSPORT_OK;
    return NOERROR;
}

ECode CLocalTransport::PerformBackup(
    /* [in] */ ICapsuleInfo* packageInfo,
    /* [in] */ IParcelFileDescriptor* data,
    /* [out] */ Int32* error)
{
    //if (DEBUG) Log.v(TAG, "performBackup() pkg=" + packageInfo.packageName);

    assert(packageInfo != NULL);
    AutoPtr<IFile> packageDir;
    String capName;
    packageInfo->GetCapsuleName(&capName);
    CFile::New(mDataDir, capName, (IFile**) &packageDir);
    assert(packageDir != NULL);

    Boolean successed = FALSE;
    packageDir->Mkdirs(&successed);

    // Each 'record' in the restore set is kept in its own file, named by
    // the record key.  Wind through the data file, extracting individual
    // record operations and building a set of all the updates to apply
    // in this update.

    //TODO
    assert(0);
    // BackupDataInput changeSet = new BackupDataInput(data.getFileDescriptor());
    // try {
    //     Int32 bufSize = 512;
    //     byte[] buf = new byte[bufSize];
    //     while (changeSet.readNextHeader()) {
    //         String key = changeSet.getKey();
    //         String base64Key = new String(Base64.encode(key.getBytes()));
    //         File entityFile = new File(packageDir, base64Key);

    //         Int32 dataSize = changeSet.getDataSize();

    //         if (DEBUG) Log.v(TAG, "Got change set key=" + key + " size=" + dataSize
    //                 + " key64=" + base64Key);

    //         if (dataSize >= 0) {
    //             if (entityFile.exists()) {
    //                 entityFile.delete();
    //             }
    //             FileOutputStream entity = new FileOutputStream(entityFile);

    //             if (dataSize > bufSize) {
    //                 bufSize = dataSize;
    //                 buf = new byte[bufSize];
    //             }
    //             changeSet.readEntityData(buf, 0, dataSize);
    //             if (DEBUG) Log.v(TAG, "  data size " + dataSize);

    //             try {
    //                 entity.write(buf, 0, dataSize);
    //             } catch (IOException e) {
    //                 Log.e(TAG, "Unable to update key file " + entityFile.getAbsolutePath());
    //                 return BackupConstants.TRANSPORT_ERROR;
    //             } finally {
    //                 entity.close();
    //             }
    //         } else {
    //             entityFile.delete();
    //         }
    //     }
    //     return BackupConstants.TRANSPORT_OK;
    // } catch (IOException e) {
    //     // oops, something went wrong.  abort the operation and return error.
    //     Log.v(TAG, "Exception reading backup input:", e);
    //     return BackupConstants.TRANSPORT_ERROR;
    // }
    return NOERROR;
}

ECode CLocalTransport::ClearBackupData(
    /* [in] */ ICapsuleInfo* packageInfo,
    /* [out] */ Int32* error)
{
    //if (DEBUG) Log.v(TAG, "clearBackupData() pkg=" + packageInfo.packageName);

    AutoPtr<IFile> packageDir;
    String capName;
    packageInfo->GetCapsuleName(&capName);

    CFile::New(mDataDir, capName, (IFile**) &packageDir);
    assert(packageDir != NULL);

    AutoPtr<IObjectContainer> files;
    AutoPtr<IObjectEnumerator> filesEmu;
    packageDir->ListFiles((IObjectContainer**)&files);
    files->GetObjectEnumerator((IObjectEnumerator**)&filesEmu);
    Boolean hasNext = FALSE, successed = FALSE;;
    while (filesEmu->MoveNext(&hasNext), hasNext) {
        AutoPtr<IFile> f;
        filesEmu->Current((IInterface**)&f);
        assert(f != NULL);

        f->Delete(&successed);
    }

    packageDir->Delete(&successed);
    assert(error != NULL);
    *error = BackupConstants::TRANSPORT_OK;
    return NOERROR;
}

ECode CLocalTransport::FinishBackup(
    /* [out] */ Int32* error)
{
    //if (DEBUG) Log.v(TAG, "finishBackup()");
    assert(error != NULL);
    *error = BackupConstants::TRANSPORT_OK;
    return NOERROR;
}

// Restore handling
ECode CLocalTransport::GetAvailableRestoreSets(
    /* [out, callee] */ ArrayOf<IRestoreSet*> ** set)
{
    // one hardcoded restore set

    //TODO
    assert(0);
    // RestoreSet set = new RestoreSet("Local disk image", "flash", RESTORE_TOKEN);
    // RestoreSet[] array = { set };
    // return array;
    return NOERROR;
}

ECode CLocalTransport::GetCurrentRestoreSet(
    /* [out] */ Int64* set)
{
    // The hardcoded restore set always has the same token
    assert(set != NULL);
    *set = RESTORE_TOKEN;
    return NOERROR;
}

ECode CLocalTransport::StartRestore(
    /* [in] */ Int64 token,
    /* [in] */ ArrayOf<ICapsuleInfo*>* packages,
    /* [out] */ Int32* error)
{
    //if (DEBUG) Log.v(TAG, "start restore " + token);
    mRestorePackages = packages;
    mRestorePackage = -1;

    assert(error != NULL);
    *error = BackupConstants::TRANSPORT_OK;
    return NOERROR;
}

ECode CLocalTransport::NextRestorePackage(
    /* [out] */ String* capsuleName) {
    assert(capsuleName != NULL);

    if (mRestorePackages == NULL) {
        assert(0);
        //throw new IllegalStateException("startRestore not called");
    }

    while (++mRestorePackage < mRestorePackages->GetLength()) {
        AutoPtr<ICapsuleInfo> cap = (*mRestorePackages)[mRestorePackage];
        assert(cap != NULL);

        String name;
        cap->GetCapsuleName(&name);

        AutoPtr<IFile> tmp;
        CFile::New(mDataDir, name, (IFile**) &tmp);
        assert(tmp != NULL);

        Boolean isDir = FALSE;
        tmp->IsDirectory(&isDir);
        if (isDir) {
            //if (DEBUG) Log.v(TAG, "  nextRestorePackage() = " + name);
            *capsuleName = name;
            return NOERROR;
        }
    }

    //if (DEBUG) Log.v(TAG, "  no more packages to restore");
    *capsuleName = String("");
    return NOERROR;
}

ECode CLocalTransport::GetRestoreData(
    /* [in] */ IParcelFileDescriptor* outFd,
    /* [out] */ Int32* data)
{
    assert(data != NULL);

    if (mRestorePackages == NULL) {
        assert(0);
        //throw new IllegalStateException("startRestore not called");
    }

    if (mRestorePackage < 0) {
        assert(0);
        //throw new IllegalStateException("nextRestorePackage not called");
    }

    //TODO
    assert(0);
    // File packageDir = new File(mDataDir, mRestorePackages[mRestorePackage].packageName);

    // // The restore set is the concatenation of the individual record blobs,
    // // each of which is a file in the package's directory
    // File[] blobs = packageDir.listFiles();
    // if (blobs == NULL) {  // nextRestorePackage() ensures the dir exists, so this is an error
    //     Log.e(TAG, "Error listing directory: " + packageDir);
    //     return BackupConstants.TRANSPORT_ERROR;
    // }

    // // We expect at least some data if the directory exists in the first place
    // if (DEBUG) Log.v(TAG, "  getRestoreData() found " + blobs.length + " key files");
    // BackupDataOutput out = new BackupDataOutput(outFd.getFileDescriptor());
    // try {
    //     for (File f : blobs) {
    //         FileInputStream in = new FileInputStream(f);
    //         try {
    //             Int32 size = (Int32) f.length();
    //             byte[] buf = new byte[size];
    //             in.read(buf);
    //             String key = new String(Base64.decode(f.getName()));
    //             if (DEBUG) Log.v(TAG, "    ... key=" + key + " size=" + size);
    //             out.writeEntityHeader(key, size);
    //             out.writeEntityData(buf, size);
    //         } finally {
    //             in.close();
    //         }
    //     }
    //     return BackupConstants.TRANSPORT_OK;
    // } catch (IOException e) {
    //     Log.e(TAG, "Unable to read backup records", e);
    //     return BackupConstants.TRANSPORT_ERROR;
    // }

    return NOERROR;
}

ECode CLocalTransport::FinishRestore()
{
    //if (DEBUG) Log.v(TAG, "finishRestore()");
    return NOERROR;
}

void CLocalTransport::DeleteContents(
	/* [in] */ IFile* dirname)
{
    assert(dirname != NULL);
    AutoPtr<IObjectContainer> files;
    AutoPtr<IObjectEnumerator> filesEmu;
    dirname->ListFiles((IObjectContainer**)&files);
    files->GetObjectEnumerator((IObjectEnumerator**)&filesEmu);

    if (filesEmu != NULL) {
        Boolean hasNext = FALSE, successed = FALSE;;
        while (filesEmu->MoveNext(&hasNext), hasNext) {
            AutoPtr<IFile> f;
            filesEmu->Current((IInterface**)&f);
            assert(f != NULL);

            Boolean isDir = FALSE;
            f->IsDirectory(&isDir);
            if (isDir) {
                DeleteContents(f);
            }

            f->Delete(&successed);
        }
    }
}
