
#include "app/backup/CRestoreObserverWrapper.h"

const Int32 CRestoreObserverWrapper::MSG_RESTORE_STARTING;
const Int32 CRestoreObserverWrapper::MSG_UPDATE;
const Int32 CRestoreObserverWrapper::MSG_RESTORE_FINISHED;
const Int32 CRestoreObserverWrapper::MSG_RESTORE_SETS_AVAILABLE;

CRestoreObserverWrapper::CRestoreObserverWrapper()
{

}

CRestoreObserverWrapper::~CRestoreObserverWrapper()
{

}

ECode CRestoreObserverWrapper::constructor(
	/* [in] */ IContext* context,
	/* [in] */ IRestoreObserver* appObserver)
{
	mAppObserver = appObserver;
    // mHandler = new Handler(context.getMainLooper()) {
    //     @Override
    //     public void handleMessage(Message msg) {
    //         switch (msg.what) {
    //         case MSG_RESTORE_STARTING:
    //             mAppObserver.restoreStarting(msg.arg1);
    //             break;
    //         case MSG_UPDATE:
    //             mAppObserver.onUpdate(msg.arg1, (String)msg.obj);
    //             break;
    //         case MSG_RESTORE_FINISHED:
    //             mAppObserver.restoreFinished(msg.arg1);
    //             break;
    //         case MSG_RESTORE_SETS_AVAILABLE:
    //             mAppObserver.restoreSetsAvailable((RestoreSet[])msg.obj);
    //             break;
    //         }
    //     }
    // };
    // mAppObserver = appObserver;

    return NOERROR;
}

ECode CRestoreObserverWrapper::RestoreSetsAvailable(
	/* [in] */ ArrayOf<IRestoreSet*>* result)
{
    // mHandler.sendMessage(
    //         mHandler.obtainMessage(MSG_RESTORE_SETS_AVAILABLE, result));
    return mAppObserver->RestoreSetsAvailable(result);
}

ECode CRestoreObserverWrapper::RestoreStarting(
	/* [in] */ Int32 numPackages)
{
    // mHandler.sendMessage(
    //         mHandler.obtainMessage(MSG_RESTORE_STARTING, numPackages, 0));
    return mAppObserver->RestoreStarting(numPackages);
}

ECode CRestoreObserverWrapper::OnUpdate(
	/* [in] */ Int32 nowBeingRestored,
	/* [in] */ const String& currentPackage)
{
    // mHandler.sendMessage(
    //         mHandler.obtainMessage(MSG_UPDATE, nowBeingRestored, 0, currentPackage));
    return mAppObserver->OnUpdate(nowBeingRestored, currentPackage);
}

ECode CRestoreObserverWrapper::RestoreFinished(
	/* [in] */ Int32 error)
{
    // mHandler.sendMessage(
    //         mHandler.obtainMessage(MSG_RESTORE_FINISHED, error, 0));
    return mAppObserver->RestoreFinished(error);
}
