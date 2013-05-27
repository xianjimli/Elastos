
#include "content/CSyncQueue.h"
#include "content/CSyncOperation.h"
#include "content/CSyncStorageEnginePendingOperation.h"
#include "elastos/Math.h"

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
    AddEx(operation, NULL, result);

    return NOERROR;
}

ECode CSyncQueue::AddEx(
    /* [in] */ ISyncOperation* operation,
    /* [in] */ ISyncStorageEnginePendingOperation* pop,
    /* [out] */ Boolean* result)
{

    if(operation == NULL){
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // - if an operation with the same key exists and this one should run earlier,
    //   update the earliestRunTime of the existing to the new time
    // - if an operation with the same key exists and if this one should run
    //   later, ignore it
    // - if no operation exists then add the new one

    String operationKey;
    operation->GetKey(&operationKey);

    ISyncOperation* existingOperation = NULL;

    HashMap<String, ISyncOperation*>::Iterator it = mOperationsMap.Find((operationKey));

    if (it != mOperationsMap.End()) {
        existingOperation = it->mSecond;
        existingOperation->AddRef();
    }

    if (existingOperation != NULL) {
        Boolean changed = FALSE;

        Boolean existingOperationExpedited;
        Boolean operationExpedited;

        existingOperation->GetExpedited(&existingOperationExpedited);
        operation->GetExpedited(&operationExpedited);

        if (operationExpedited == existingOperationExpedited) {
            Int64 existingOperationEarliestRunTime;
            Int64 operationEarliestRunTime;

            existingOperation->GetEarliestRunTime(&existingOperationEarliestRunTime);
            operation->GetEarliestRunTime(&operationEarliestRunTime);

            Int64 newRunTime = Math::Min(existingOperationEarliestRunTime, operationEarliestRunTime);

            if (existingOperationEarliestRunTime != newRunTime) {
                existingOperation->SetEarliestRunTime(newRunTime);
                changed = TRUE;
            }
        } else {
            if (operationExpedited) {
                existingOperation->SetExpedited(TRUE);
                changed = TRUE;
            }
        }

        *result = changed;

        // Release reference
        existingOperation->Release();
        existingOperation = NULL;
    }

    operation->SetPendingOperation(pop);

    if (pop == NULL) {

        AutoPtr<IAccount> account;
        operation->GetAccount((IAccount**)&account);

        Int32 syncSource;
        operation->GetSyncSource(&syncSource);

        String authority;
        operation->GetAuthority(&authority);

        AutoPtr<IBundle> extra;
        operation->GetExtras((IBundle**)&extra);

        Boolean expedited;
        operation->GetExpedited(&expedited);

        AutoPtr<ISyncOperation> syncOperation;

        FAIL_RETURN(CSyncStorageEnginePendingOperation::New(account, syncSource, authority, extra, expedited, &pop));

        ISyncStorageEnginePendingOperation* newPop;
        mSyncStorageEngine->InsertIntoPending(pop, &newPop);
        pop = newPop;

        if (pop == NULL) {
//            throw new IllegalStateException("error adding pending sync operation " + operation);
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        operation->SetPendingOperation(pop);
    }

    mOperationsMap[operationKey] = operation;
    *result = TRUE;

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
    mSyncStorageEngine = engine;

    AutoPtr<IObjectContainer> pendingOperations;

    mSyncStorageEngine->GetPendingOperations((IObjectContainer**)&pendingOperations);


    AutoPtr<IObjectEnumerator> opsIt;
    pendingOperations->GetObjectEnumerator((IObjectEnumerator**) &opsIt);

    Boolean hasNext;
    while (opsIt->MoveNext(&hasNext), hasNext)
    {
        AutoPtr<ISyncStorageEnginePendingOperation> pendingOperation;
        opsIt->Current((IInterface**)&pendingOperation);


        AutoPtr<IAccount> account;
        pendingOperation->GetAccount((IAccount**)&account);

        Int32 syncSource;
        pendingOperation->GetSyncSource(&syncSource);

        String authority;
        pendingOperation->GetAuthority(&authority);

        AutoPtr<IBundle> extra;
        pendingOperation->GetExtras((IBundle**)&extra);

        Boolean expedited;
        pendingOperation->GetExpedited(&expedited);

        AutoPtr<ISyncOperation> syncOperation;

        FAIL_RETURN(CSyncOperation::New(account, syncSource, authority, extra, 0, (ISyncOperation**)&syncOperation));

        syncOperation->SetExpedited(expedited);
        syncOperation->SetPendingOperation(pendingOperation);

        Boolean result = FALSE;
        AddEx(syncOperation, pendingOperation, &result);
    }

    return NOERROR;
}
