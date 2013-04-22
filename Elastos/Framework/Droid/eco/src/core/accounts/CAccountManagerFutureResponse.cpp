
#include "ext/frameworkdef.h"
#include "accounts/CAccountManagerFutureResponse.h"
#include "accounts/CAccountManager.h"

ECode CAccountManagerFutureResponse::OnResult(
    /* [in] */ IBundle* bundle)
{
    CAccountManager::BaseFutureTask* task = (CAccountManager::BaseFutureTask*)mHost;

    //try {
    AutoPtr<IInterface> result;
    FAIL_RETURN(task->BundleToResult(bundle, (IInterface**)&result));

    if (result == NULL) {
        return NOERROR;
    }
    //mHost->Set(result);
    return E_NOT_IMPLEMENTED;
    // } catch (ClassCastException e) {
    //     // we will set the exception below
    // } catch (AuthenticatorException e) {
    //     // we will set the exception below
    // }
    //onError(ERROR_CODE_INVALID_RESPONSE, "no result in response");
}

ECode CAccountManagerFutureResponse::OnError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& errorMessage)
{
    // if (code == ERROR_CODE_CANCELED) {
    //     cancel(true /* mayInterruptIfRunning */);
    //     return;
    // }
    // setException(convertErrorToException(code, message));
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerFutureResponse::constructor(
    /* [in] */ Handle32 host)
{
    mHost = host;
    return NOERROR;
}
