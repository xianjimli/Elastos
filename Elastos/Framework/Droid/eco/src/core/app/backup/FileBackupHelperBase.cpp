
#include "app/backup/FileBackupHelperBase.h"
#include "app/backup/CBackupDataOutput.h"
//#include "utils/log.h"
#include "app/backup/CBackupDataInput.h"
#include "app/backup/CBackupDataInputStream.h"


CString FileBackupHelperBase::TAG = "FileBackupHelperBase";


FileBackupHelperBase::FileBackupHelperBase(
	/* [in] */ IContext* context)
	: mPtr(NULL)
	, mContext(context)
	, mExceptionLogged(FALSE)
{
    mPtr = ctor_native();
}

FileBackupHelperBase::~FileBackupHelperBase()
{
	assert(mPtr == NULL);
}

void FileBackupHelperBase::WriteNewStateDescription(
	/* [in] */ IParcelFileDescriptor* fd)
{
	assert(fd != NULL);
	AutoPtr<IFileDescriptor> des;
	fd->GetFileDescriptor((IFileDescriptor**) &des);
    writeSnapshot_native(mPtr, des);
    // TODO: Do something with the error.
}

void FileBackupHelperBase::Finalize()
{
    dtor_native();
}

void FileBackupHelperBase::PerformBackup_checked(
	/* [in] */ IParcelFileDescriptor* oldState,
	/* [in] */ IBackupDataOutput* data,
    /* [in] */ IParcelFileDescriptor* newState,
    /* [in] */ ArrayOf<String>* files,
    /* [in] */ ArrayOf<String>* keys)
{
	assert(files != NULL);
    if (files->GetLength() == 0) {
        return;
    }

    // files must be all absolute paths
    for (Int32 i = 0; i < files->GetLength(); i ++) {
        if ((*files)[i].GetChar(0) != '/') {
        	assert(0);
            //throw new RuntimeException("files must have all absolute paths: " + f);
        }
    }

    // the length of files and keys must be the same
    assert(keys != NULL);
    if (files->GetLength() != keys->GetLength()) {
    	assert(0);
        // throw new RuntimeException("files.length=" + files.length
        //         + " keys.length=" + keys.length);
    }

    // oldStateFd can be NULL
    AutoPtr<IFileDescriptor> oldStateFd;
    if (oldState != NULL) {
    	oldState->GetFileDescriptor((IFileDescriptor**) &oldStateFd);
    }

    AutoPtr<IFileDescriptor> newStateFd;
    newState->GetFileDescriptor((IFileDescriptor**) &newStateFd);

    if (newStateFd == NULL) {
    	assert(0);
        //throw new NullPointerException();
    }

    assert(data != NULL);
    Int32 err = performBackup_native(oldStateFd, ((CBackupDataOutput*)data)->mBackupWriter,
    		newStateFd, files, keys);

    if (err != 0) {
    	assert(0);
        // TODO: more here
        //throw new RuntimeException("Backup failed 0x" + Integer.toHexString(err));
    }
}

void FileBackupHelperBase::WriteFile(
	/* [in] */ IFile* f,
	/* [in] */ IBackupDataInputStream* in)
{
	assert(f != NULL);
    Int32 result = -1;

    // Create the enclosing directory.
    AutoPtr<IFile> parent;
    f->GetParentFile((IFile**) &parent);
    assert(parent != NULL);

    Boolean successed = FALSE;
    parent->Mkdirs(&successed);

    String path;
    f->GetAbsolutePath(&path);
    result = writeFile_native(mPtr, path,
    	((CBackupDataInput*) (((CBackupDataInputStream*)in)->mData).Get())->mBackupReader);

    if (result != 0) {
        // Bail on this entity.  Only log one failure per helper object.
        if (!mExceptionLogged) {
            // LOGE(TAG, "Failed restoring file '" + f + "' for app '"
            //         + mContext.getPackageName() + "\' result=0x"
            //         + Integer.toHexString(result));
            mExceptionLogged = TRUE;
        }
    }
}

Boolean FileBackupHelperBase::IsKeyInList(
	/* [in] */ const String& key,
	/* [in] */ ArrayOf<String>* list)
{
	assert(list != NULL);
	for (Int32 i = 0; i < list->GetLength(); i ++) {
		if ((*list)[i] == key) {
			return TRUE;
		}
	}

    return FALSE;
}

android::RestoreHelperBase* FileBackupHelperBase::ctor_native()
{
	return new android::RestoreHelperBase();
}

void FileBackupHelperBase::dtor_native()
{
	assert(mPtr != NULL);
	delete mPtr;
	mPtr = NULL;
}

Int32 FileBackupHelperBase::performBackup_native(
	/* [in] */ IFileDescriptor* oldState,
    /* [in] */ android::BackupDataWriter* dataStream,
    /* [in] */ IFileDescriptor* newState,
    /* [in] */ ArrayOf<String>* files,
    /* [in] */ ArrayOf<String>* keys)
{
    Int32 err = 0;

    // all parameters have already been checked against NULL
    Int32 oldStateFD = -1;
    if (oldState != NULL) {
    	oldState->GetDescriptor(&oldStateFD);
    }

    assert(newState != NULL);
    Int32 newStateFD = 0;
    newState->GetDescriptor(&newStateFD);

    assert(files != NULL);
    const Int32 fileCount = files->GetLength();
    char const** filesUTF = (char const**)malloc(sizeof(char*)*fileCount);
    for (Int32 i=0; i<fileCount; i++) {
        filesUTF[i] = (*files)[i];
    }

    assert(keys != NULL);
    const Int32 keyCount = keys->GetLength();
    char const** keysUTF = (char const**)malloc(sizeof(char*)*keyCount);
    for (Int32 i=0; i<keyCount; i++) {
        keysUTF[i] = (*keys)[i];;
    }

    err = back_up_files(oldStateFD, dataStream, newStateFD, filesUTF, keysUTF, fileCount);

    free(filesUTF);
    free(keysUTF);

    return err;
}

Int32 FileBackupHelperBase::writeFile_native(
	/* [in] */ android::RestoreHelperBase* restore,
	/* [in] */ const String& filename,
	/* [in] */ android::BackupDataReader* reader)
{
	assert(restore != NULL);
    return restore->WriteFile(android::String8((const char*) filename), reader);
}

Int32 FileBackupHelperBase::writeSnapshot_native(
	/* [in] */ android::RestoreHelperBase* restore,
	/* [in] */ IFileDescriptor* fdes)
{
	assert(fdes != NULL);
    Int32 fd = 0;
    fdes->GetDescriptor(&fd);
    return restore->WriteSnapshot(fd);
}
