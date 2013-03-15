
#include "app/backup/FileBackupHelper.h"

CString FileBackupHelper::TAG = "FileBackupHelper";
const Boolean FileBackupHelper::DEBUG;


FileBackupHelper::FileBackupHelper(
	/* [in] */ IContext* context,
	/* [in] */ ArrayOf<String>* files)
	: FileBackupHelperBase(context)
	, mContext(context)
	, mFiles(NULL)
{
	assert(context != NULL);

	//TODO: has no member named 'GetFilesDir'
	assert(0);
    //context->GetFilesDir((IFile**) &mFilesDir);

    if (files != NULL) {
    	mFiles = files->Clone();
    }
}

FileBackupHelper::~FileBackupHelper()
{
	if (mFiles != NULL) {
		ArrayOf<String>::Free(mFiles);
		mFiles = NULL;
	}
}

UInt32 FileBackupHelper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 FileBackupHelper::Release()
{
    return ElRefBase::Release();
}

PInterface FileBackupHelper::Probe(
   /* [in] */ REIID riid)
{
   if (riid == EIID_IBackupHelper) {
       return (IBackupHelper*)this;
   }

   return NULL;
}

ECode FileBackupHelper::GetInterfaceID(
   /* [in] */ IInterface *pObject,
   /* [out] */ InterfaceID *pIID)
{
   VALIDATE_NOT_NULL(pIID);

   if (pObject == (IInterface*)(IBackupHelper*)this) {
       *pIID = EIID_IBackupHelper;
   }
   else {
       return E_INVALID_ARGUMENT;
   }

   return NOERROR;
}

ECode FileBackupHelper::PerformBackup(
    /* [in] */ IParcelFileDescriptor* oldState,
    /* [in] */ IBackupDataOutput* data,
    /* [in] */ IParcelFileDescriptor* newState)
{
    AutoPtr<IFile> base;

	//TODO: has no member named 'GetFilesDir'
	assert(0);
    //mContext->GetFilesDir((IFile**) &base);

    const Int32 N = mFiles->GetLength();
    ArrayOf<String>* fullPaths = ArrayOf<String>::Alloc(N);

    AutoPtr<IFile> tmp;
    for (Int32 i=0; i<N; i++) {
    	CFile::New(base, (*mFiles)[i], (IFile**) &tmp);
    	assert(tmp != NULL);

    	tmp->GetAbsolutePath(&((*fullPaths)[i]));
    }

    // go
    PerformBackup_checked(oldState, data, newState, fullPaths, mFiles);

    return NOERROR;
}

ECode FileBackupHelper::RestoreEntity(
    /* [in] */ IBackupDataInputStream* data)
{
    //if (DEBUG) Log.d(TAG, "got entity '" + data.getKey() + "' size=" + data.size());
    assert(data != NULL);
    String key;
    data->GetKey(&key);
    if (IsKeyInList(key, mFiles)) {
        AutoPtr<IFile> f;
        CFile::New(mFilesDir, key, (IFile**) &f);
        WriteFile(f, data);
    }

    return NOERROR;
}

ECode FileBackupHelper::WriteNewStateDescription(
    /* [in] */ IParcelFileDescriptor* newState)
{
	return NOERROR;
}
