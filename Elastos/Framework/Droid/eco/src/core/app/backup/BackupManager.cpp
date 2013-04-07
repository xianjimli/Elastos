
#include "app/backup/BackupManager.h"
#include "os/CServiceManager.h"

CString BackupManager::TAG = "BackupManager";
AutoPtr<IBackupManager> BackupManager::sService;

BackupManager::BackupManager(
    /* [in] */ IContext* context)
    : mContext(context)
{
}

void BackupManager::DataChanged()
{
    CheckServiceBinder();
    if (sService != NULL) {
        assert(mContext != NULL);
        String name;
        mContext->GetCapsuleName(&name);
        // try {
        sService->DataChanged(name);
        // } catch (RemoteException e) {
            // Log.d(TAG, "dataChanged() couldn't connect");
        // }
    }
}

void BackupManager::DataChanged(
    /* [in] */ String capsuleName)
{
    CheckServiceBinder();
    if (sService != NULL) {
        // try {
        sService->DataChanged(capsuleName);
        // } catch (RemoteException e) {
            // Log.d(TAG, "dataChanged(pkg) couldn't connect");
        // }
    }
}

Int32 BackupManager::RequestRestore(
    /* [in] */ IRestoreObserver* observer)
{
    Int32 result = -1;
    CheckServiceBinder();
    if (sService != NULL) {
        AutoPtr<RestoreSession> session;
        // try {
        AutoPtr<IRestoreSessionStub> binder;
        String name;
        mContext->GetCapsuleName(&name);

        sService->BeginRestoreSession(name,
                String(""), (IRestoreSessionStub**) &binder);


        session = new RestoreSession(mContext, binder);
        session->RestoreCapsule(name, observer, &result);
        // } catch (RemoteException e) {
            // Log.w(TAG, "restoreSelf() unable to contact service");
        // } finally {
        if (session != NULL) {
            session->EndRestoreSession();
        }
        // }
    }

    return result;
}

AutoPtr<RestoreSession> BackupManager::BeginRestoreSession()
{
    AutoPtr<RestoreSession> session;
    CheckServiceBinder();
    if (sService != NULL) {
        // try {
        // All packages, current transport
        AutoPtr<IRestoreSessionStub> binder;
        sService->BeginRestoreSession(String(""),
                String(""), (IRestoreSessionStub**) &binder);

        session = new RestoreSession(mContext, binder);
        // } catch (RemoteException e) {
            // Log.w(TAG, "beginRestoreSession() couldn't connect");
        // }
    }

    return session;
}

void BackupManager::CheckServiceBinder()
{
    if (sService == NULL) {
        AutoPtr<IServiceManager> sm;
        CServiceManager::AcquireSingleton((IServiceManager**)&sm);
        sm->GetService(String(Context_BACKUP_SERVICE), (IInterface**)&sService);
        assert(sService != NULL);
    }
}
