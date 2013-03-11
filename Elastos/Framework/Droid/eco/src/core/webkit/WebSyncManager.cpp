
#include "webkit/WebSyncManager.h"

const char* WebSyncManager::LOGTAG = "websync";

CARAPI_(void) WebSyncManager::Run()
{}

/**
 * sync() forces sync manager to sync now
 */
CARAPI_(void) WebSyncManager::Sync()
{}

/**
 * resetSync() resets sync manager's timer
 */
CARAPI_(void) WebSyncManager::ResetSync()
{}

/**
 * startSync() requests sync manager to start sync
 */
CARAPI_(void) WebSyncManager::StartSync()
{}

/**
 * stopSync() requests sync manager to stop sync. remove any SYNC_MESSAGE in
 * the queue to break the sync loop
 */
CARAPI_(void) WebSyncManager::StopSync()
{}

WebSyncManager::WebSyncManager(
	/* [in] */ IContext* context, 
	/* [in] */ const String& name)
{}

CARAPI_(void) WebSyncManager::onSyncInit()
{}

CARAPI_(IInterface*) WebSyncManager::Clone()
{
	return NULL;
}

//@Override
CARAPI_(void) WebSyncManager::SyncHandler::HandleMessage(
	/* [in] */ IMessage* msg)
{}
