
#include "app/backup/SharedPreferencesBackupHelper.h"

CString SharedPreferencesBackupHelper::TAG = "SharedPreferencesBackupHelper";
const Boolean SharedPreferencesBackupHelper::DEBUG;


SharedPreferencesBackupHelper::SharedPreferencesBackupHelper(
	/* [in] */ IContext* context,
	/* [in] */ ArrayOf<String>* prefGroups)
	: FileBackupHelperBase(context)
	, mContext(context)
	, mPrefGroups(NULL)
{
    if (prefGroups != NULL) {
    	mPrefGroups = prefGroups->Clone();
    }
}

SharedPreferencesBackupHelper::~SharedPreferencesBackupHelper()
{
	if (mPrefGroups != NULL) {
		ArrayOf<String>::Free(mPrefGroups);
		mPrefGroups = NULL;
	}
}

UInt32 SharedPreferencesBackupHelper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SharedPreferencesBackupHelper::Release()
{
    return ElRefBase::Release();
}

PInterface SharedPreferencesBackupHelper::Probe(
   /* [in] */ REIID riid)
{
   if (riid == EIID_IBackupHelper) {
       return (IBackupHelper*)this;
   }

   return NULL;
}

ECode SharedPreferencesBackupHelper::GetInterfaceID(
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

ECode SharedPreferencesBackupHelper::PerformBackup(
    /* [in] */ IParcelFileDescriptor* oldState,
    /* [in] */ IBackupDataOutput* data,
    /* [in] */ IParcelFileDescriptor* newState)
{
    // If a SharedPreference has an outstanding write in flight,
    // wait for it to finish flushing to disk.
    //TODO
    //QueuedWork.waitToFinish();

    // make filenames for the mPrefGroups
    assert(mPrefGroups != NULL && mContext != NULL);
    const Int32 N = mPrefGroups->GetLength();
    ArrayOf<String>* files = ArrayOf<String>::Alloc(N);

    //TODO: Func GetSharedPrefsFile doesn't exist.
    assert(0);
    for (Int32 i=0; i<N; i++) {
    	//TODO
    	// AutoPtr<IFile> f;
    	// mContext.GetSharedPrefsFile((*mPrefGroups)[i], (IFile**) &f);
    	// assert(f != NULL);

    	// f->GetAbsolutePath(& ((*files)[i]));
    }

    // go
    PerformBackup_checked(oldState, data, newState, files, mPrefGroups);

    return NOERROR;
}

ECode SharedPreferencesBackupHelper::RestoreEntity(
    /* [in] */ IBackupDataInputStream* data)
{
    assert(data != NULL);
    String key;
    data->GetKey(&key);
    //if (DEBUG) Log.d(TAG, "got entity '" + key + "' size=" + data.size());

    //TODO: Func GetSharedPrefsFile doesn't exist.
    assert(0);
    assert(mContext != NULL);
    if (IsKeyInList(key, mPrefGroups)) {
    	//TODO
    	// AutoPtr<IFile> tmp;
    	// mContext.GetSharedPrefsFile(key, (IFile**) &tmp);
    	// assert(tmp != NULL);

    	// AutoPtr<IFile> f;
    	// tmp->GetAbsoluteFile((IFile**) &f);
     	// WriteFile(f, data);
    }

    return NOERROR;
}

ECode SharedPreferencesBackupHelper::WriteNewStateDescription(
    /* [in] */ IParcelFileDescriptor* newState)
{
    return NOERROR;
}
