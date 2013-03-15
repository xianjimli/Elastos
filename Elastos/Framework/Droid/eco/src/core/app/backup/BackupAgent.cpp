
#include "app/backup/BackupAgent.h"
#include "os/Binder.h"
#include "utils/log.h"
#include "app/backup/CBackupDataOutput.h"
#include "app/backup/CBackupDataInput.h"

CString BackupAgent::BackupServiceBinder::TAG = "BackupServiceBinder";
CString BackupAgent::TAG = "BackupAgent";
const Boolean BackupAgent::DEBUG;

BackupAgent::BackupServiceBinder::BackupServiceBinder(
    /* [in] */ BackupAgent* host)
    : mHost(host)
{
}

BackupAgent::BackupServiceBinder::~BackupServiceBinder()
{

}

UInt32 BackupAgent::BackupServiceBinder::AddRef()
{
	return ElRefBase::AddRef();
}

UInt32 BackupAgent::BackupServiceBinder::Release()
{
	return ElRefBase::Release();
}

PInterface BackupAgent::BackupServiceBinder::Probe(
    /* [in]  */ REIID riid)
{
	if (riid == EIID_IBackupAgent) {
		return (IInterface*)this;
	}

	return NULL;
}

ECode BackupAgent::BackupServiceBinder::GetInterfaceID(
    /* [in]  */ IInterface* pObject,
    /* [out]  */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IBackupAgent*)this) {
        *pIID = EIID_IBackupAgent;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode BackupAgent::BackupServiceBinder::DoBackup(
    /* [in] */ IParcelFileDescriptor* oldState,
    /* [in] */ IParcelFileDescriptor* data,
    /* [in] */ IParcelFileDescriptor* newState,
    /* [in] */ Int32 token,
    /* [in] */ IBackupManager* callbackBinder)
{
    // Ensure that we're running with the app's normal permission level
    Int64 ident = Binder::ClearCallingIdentity();

    if (DEBUG) LOGW(TAG, "doBackup() invoked");
    AutoPtr<IBackupDataOutput> output;
    AutoPtr<IFileDescriptor> fdes;
    assert(data != NULL);
    data->GetFileDescriptor((IFileDescriptor**) &fdes);

    CBackupDataOutput::New(fdes.Get(), (IBackupDataOutput**) &output);
    mHost->OnBackup(oldState, output, newState);
    Binder::RestoreCallingIdentity(ident);
    return callbackBinder->OpComplete(token);
}

ECode BackupAgent::BackupServiceBinder::DoRestore(
    /* [in] */ IParcelFileDescriptor* data,
    /* [in] */ Int32 appVersionCode,
    /* [in] */ IParcelFileDescriptor* newState,
    /* [in] */ Int32 token,
    /* [in] */ IBackupManager* callbackBinder)
{
    // Ensure that we're running with the app's normal permission level
    Int64 ident = Binder::ClearCallingIdentity();

    if (DEBUG) LOGW(TAG, "doRestore() invoked");
    AutoPtr<IBackupDataInput> input;
    AutoPtr<IFileDescriptor> fdes;
    assert(data != NULL);
    data->GetFileDescriptor((IFileDescriptor**) &fdes);

    CBackupDataInput::New(fdes, (IBackupDataInput**) &input);

    mHost->OnRestore(input, appVersionCode, newState);
    Binder::RestoreCallingIdentity(ident);
    return callbackBinder->OpComplete(token);
}

ECode BackupAgent::BackupServiceBinder::GetDescription(
    /* [in] */ String* des)
{
    return E_NOT_IMPLEMENTED;
}

BackupAgent::BackupAgent():
	ContextWrapper(NULL)
{
    mBinder = new BackupServiceBinder(this);
}

BackupAgent::~BackupAgent()
{

}

void BackupAgent::OnCreate()
{
}

void BackupAgent::OnDestroy()
{
}

AutoPtr<IBinder> BackupAgent::OnBind()
{
    return mBinder;
}

void BackupAgent::Attach(
    /* [in] */ IContext* context)
{
    AttachBaseContext(context);
}
