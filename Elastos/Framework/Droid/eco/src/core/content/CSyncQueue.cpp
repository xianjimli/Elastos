
#include "content/CSyncQueue.h"

const CString CSyncQueue::TAG = "SyncManager";

CSyncQueue::CSyncQueue():
    mOperationsMap(8)
{

}

CSyncQueue::~CSyncQueue()
{
    mOperationsMap.Clear();
}

ECode CSyncQueue::Add(
    /* [in] */ ISyncOperation* operation,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncQueue::AddEx(
    /* [in] */ ISyncOperation* operation,
    /* [in] */ ISyncStorageEnginePendingOperation* pop,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncQueue::Remove(
    /* [in] */ ISyncOperation* operation)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncQueue::GetOpTime(
    /* [in] */ ISyncOperation* op,
    /* [out] */ Int64* time)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncQueue::GetIsInitial(
    /* [in] */ ISyncOperation* op,
    /* [out] */ Boolean* isInitial)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncQueue::IsOpBetter(
    /* [in] */ ISyncOperation* best,
    /* [in] */ Int64 bestRunTime,
    /* [in] */ Boolean bestIsInitial,
    /* [in] */ ISyncOperation* op,
    /* [in] */ Int64 opRunTime,
    /* [in] */ Boolean opIsInitial,
    /* [out] */ Boolean* isOpBetter)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncQueue::RemoveEx(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncQueue::Dump(
    /* [out] */ String* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncQueue::constructor(
    /* [in] */ ISyncStorageEngine* engine)
{
//    mSyncStorageEngine = engine;
//
//    ArrayList<SyncStorageEngine.PendingOperation> ops
//            = mSyncStorageEngine.getPendingOperations();
//    final int N = ops.size();
//    for (int i=0; i<N; i++) {
//        SyncStorageEngine.PendingOperation op = ops.get(i);
//        SyncOperation syncOperation = new SyncOperation(
//                op.account, op.syncSource, op.authority, op.extras, 0 /* delay */);
//        syncOperation.expedited = op.expedited;
//        syncOperation.pendingOperation = op;
//        add(syncOperation, op);
//    }

    return E_NOT_IMPLEMENTED;
}

