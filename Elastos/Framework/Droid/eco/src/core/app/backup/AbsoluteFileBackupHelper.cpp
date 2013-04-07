
#include "app/backup/AbsoluteFileBackupHelper.h"


CString AbsoluteFileBackupHelper::TAG = "AbsoluteFileBackupHelper";
const Boolean AbsoluteFileBackupHelper::DEBUG;

AbsoluteFileBackupHelper::AbsoluteFileBackupHelper(
	/* [in] */ IContext* context,
	/* [in] */ ArrayOf<String>* files)
	: FileBackupHelperBase(context)
	, mContext(context)
	, mFiles(NULL)
{
    if (files != NULL) {
    	mFiles = files->Clone();
    }
}

AbsoluteFileBackupHelper::~AbsoluteFileBackupHelper()
{
	if (mFiles != NULL) {
		ArrayOf<String>::Free(mFiles);
		mFiles = NULL;
	}
}

UInt32 AbsoluteFileBackupHelper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AbsoluteFileBackupHelper::Release()
{
    return ElRefBase::Release();
}

PInterface AbsoluteFileBackupHelper::Probe(
   /* [in] */ REIID riid)
{
   if (riid == EIID_IBackupHelper) {
       return (IBackupHelper*)this;
   }

   return NULL;
}

ECode AbsoluteFileBackupHelper::GetInterfaceID(
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

ECode AbsoluteFileBackupHelper::PerformBackup(
    /* [in] */ IParcelFileDescriptor* oldState,
    /* [in] */ IBackupDataOutput* data,
    /* [in] */ IParcelFileDescriptor* newState)
{
    // use the file paths as the keys, too
    PerformBackup_checked(oldState, data, newState, mFiles, mFiles);

    return NOERROR;
}

ECode AbsoluteFileBackupHelper::RestoreEntity(
    /* [in] */ IBackupDataInputStream* data)
{
    //if (DEBUG) Log.d(TAG, "got entity '" + data.getKey() + "' size=" + data.size());
    assert(data != NULL);
    String key;
    data->GetKey(&key);

    if (IsKeyInList(key, mFiles)) {
        AutoPtr<IFile> f;
        CFile::New(key, (IFile**) &f);
        WriteFile(f, data);
    }

    return NOERROR;
}

ECode AbsoluteFileBackupHelper::WriteNewStateDescription(
    /* [in] */ IParcelFileDescriptor* newState)
{
    return NOERROR;
}

