
#include "app/backup/BackupAgentHelper.h"
#include "app/backup/BackupHelperDispatcher.h"

CString BackupAgentHelper::TAG = "BackupAgentHelper";

BackupAgentHelper::BackupAgentHelper()
{
	mDispatcher = new BackupHelperDispatcher();
}

BackupAgentHelper::~BackupAgentHelper()
{

}

void BackupAgentHelper::OnBackup(
    /* [in] */ IParcelFileDescriptor* oldState,
    /* [in] */ IBackupDataOutput* data,
    /* [in] */ IParcelFileDescriptor* newState)
{
    mDispatcher->PerformBackup(oldState, data, newState);
}

void BackupAgentHelper::OnRestore(
    /* [in] */ IBackupDataInput* data,
    /* [in] */ Int32 appVersionCode,
    /* [in] */ IParcelFileDescriptor* newState)
{
	mDispatcher->PerformRestore(data, appVersionCode, newState);
}

AutoPtr<BackupHelperDispatcher> BackupAgentHelper::GetDispatcher()
{
    return mDispatcher;
}

void BackupAgentHelper::AddHelper(
    /* [in] */ const String& keyPrefix,
    /* [in] */ IBackupHelper* helper) {
    mDispatcher->AddHelper(keyPrefix, helper);
}
