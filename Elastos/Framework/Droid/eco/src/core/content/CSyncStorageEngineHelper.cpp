
#include "content/CSyncStorageEngineHelper.h"
#include "content/CSyncStorageEngine.h"
#include "ext/frameworkdef.h"
#include "os/Environment.h"

ECode CSyncStorageEngineHelper::NewTestInstance(
    /* [in] */ IContext* context,
    /* [out] */ ISyncStorageEngine** newEngine)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(newEngine);
    AutoPtr<IFile> file;
    FAIL_RETURN(context->GetFilesDir((IFile**) &file));
    return CSyncStorageEngine::New(context, file, newEngine);
}

ECode CSyncStorageEngineHelper::Init(
    /* [in] */ IContext* context)
{
    if (NULL != CSyncStorageEngine::sSyncStorageEngine) {
        return NOERROR;
    }
    // This call will return the correct directory whether Encrypted File Systems is
    // enabled or not.
    AutoPtr<IFile> dataDir = Environment::GetSystemSecureDirectory();
    return CSyncStorageEngine::New(context, dataDir, (ISyncStorageEngine**)&CSyncStorageEngine::sSyncStorageEngine);
}

ECode CSyncStorageEngineHelper::GetSingleton(
    /* [out] */ ISyncStorageEngine** syncStoregeEngine)
{
    VALIDATE_NOT_NULL(syncStoregeEngine);

    if (NULL == CSyncStorageEngine::sSyncStorageEngine) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *syncStoregeEngine = (ISyncStorageEngine*)CSyncStorageEngine::sSyncStorageEngine;
    return NOERROR;
}

ECode CSyncStorageEngineHelper::Equals(
    /* [in] */ IBundle* bundleOne,
    /* [in] */ IBundle* bundleTwo,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(bundleOne);
    VALIDATE_NOT_NULL(bundleTwo);
    VALIDATE_NOT_NULL(isEqual);
    Int32 size1;
    Int32 size2;
    Boolean isEmpty;
    FAIL_RETURN(bundleOne->GetSize(&size1));
    FAIL_RETURN(bundleTwo->GetSize(&size2));

    if (size1 != size2) {
        *isEqual = FALSE;
        return NOERROR;
    }

    FAIL_RETURN(bundleOne->IsEmpty(&isEmpty));

    if (isEmpty) {
        *isEqual = TRUE;
        return NOERROR;
    }

    AutoPtr<IObjectContainer> stringSet;
    AutoPtr<IObjectEnumerator> ObjEnumerator;
    FAIL_RETURN(bundleOne->KeySet((IObjectContainer**)&stringSet));

    if (NULL != stringSet) {
        AutoPtr<ICharSequence> stringKey;
        AutoPtr<IInterface> compare1;
        AutoPtr<IInterface> conpare2;
        String key;
        Boolean hasNext;
        Boolean isContain;
        FAIL_RETURN(stringSet->GetObjectEnumerator((IObjectEnumerator**)&ObjEnumerator));

        while ((ObjEnumerator->MoveNext(&hasNext), hasNext)) {
            FAIL_RETURN(ObjEnumerator->Current((IInterface**) &stringKey));
            FAIL_RETURN(stringKey->ToString(&key));
            FAIL_RETURN(bundleTwo->ContainsKey(key, &isContain));
            if(!isContain){
                *isEqual = FALSE;
                return NOERROR;
            }

            FAIL_RETURN(bundleOne->Get(key, (IInterface**)&compare1));
            FAIL_RETURN(bundleTwo->Get(key, (IInterface**)&conpare2));

            if(compare1 != conpare2){
                *isEqual = FALSE;
                return NOERROR;
            }
        }

    }

    *isEqual = TRUE;
    return NOERROR;
}

