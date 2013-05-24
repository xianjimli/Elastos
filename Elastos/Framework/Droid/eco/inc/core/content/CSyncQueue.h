
#ifndef __CSYNCQUEUE_H__
#define __CSYNCQUEUE_H__

#include "_CSyncQueue.h"

#include <elastos/AutoPtr.h>
#include <ext/frameworkdef.h>
#include <elastos/HashMap.h>

using namespace Elastos;
using namespace Elastos::Core;

CarClass(CSyncQueue)
{
public:
    CSyncQueue();
    ~CSyncQueue();

public:
    CARAPI Add(
        /* [in] */ ISyncOperation* operation,
        /* [out] */ Boolean* result);

    CARAPI AddEx(
        /* [in] */ ISyncOperation* operation,
        /* [in] */ ISyncStorageEnginePendingOperation* pop,
        /* [out] */ Boolean* result);

    CARAPI Remove(
        /* [in] */ ISyncOperation* operation);

    CARAPI GetOpTime(
        /* [in] */ ISyncOperation* op,
        /* [out] */ Int64* time);

    CARAPI GetIsInitial(
        /* [in] */ ISyncOperation* op,
        /* [out] */ Boolean* isInitial);

    CARAPI IsOpBetter(
        /* [in] */ ISyncOperation* best,
        /* [in] */ Int64 bestRunTime,
        /* [in] */ Boolean bestIsInitial,
        /* [in] */ ISyncOperation* op,
        /* [in] */ Int64 opRunTime,
        /* [in] */ Boolean opIsInitial,
        /* [out] */ Boolean* isOpBetter);

    CARAPI RemoveEx(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority);

    CARAPI Dump(
        /* [out] */ String* result);

    CARAPI constructor(
        /* [in] */ ISyncStorageEngine* engine);


private:
    const static CString TAG; // = "SyncManager";
    AutoPtr<ISyncStorageEngine> mSyncStorageEngine;

    // A Map of SyncOperations operationKey -> SyncOperation that is designed for
    // quick lookup of an enqueued SyncOperation.
    /*final*/ HashMap<String, ISyncOperation*> mOperationsMap;// = Maps.newHashMap();
};

#endif // __CSYNCQUEUE_H__
