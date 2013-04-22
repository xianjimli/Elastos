
#include "ext/frameworkdef.h"
#include "accounts/CChooseResponse.h"
#include "accounts/CAccountManager.h"
#include "accounts/CAccount.h"

ECode CChooseResponse::OnResult(
    /* [in] */ IBundle* bundle)
{
    AutoPtr<IAccount> account;
    String name, type;
    bundle->GetString(String(AccountManager_KEY_ACCOUNT_NAME), &name);
    bundle->GetString(String(AccountManager_KEY_ACCOUNT_TYPE), &type);
    CAccount::New(name, type, (IAccount**)&account);
    CAccountManager::GetAuthTokenByTypeAndFeaturesTask* task
            = (CAccountManager::GetAuthTokenByTypeAndFeaturesTask*)mHost;
    return task->mHost->GetAuthToken(account, task->mAuthTokenType, task->mLoginOptions,
            task->mActivity, task->mMyCallback, task->mHandler,
            (IAccountManagerFuture**)&(task->mFuture));
}

ECode CChooseResponse::OnError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& errorMessage)
{
    // if (code == ERROR_CODE_CANCELED) {
    //     cancel(true /* mayInterruptIfRunning */);
    //     return;
    // }
    // setException(convertErrorToException(code, message));
    CAccountManager::GetAuthTokenByTypeAndFeaturesTask* task
            = (CAccountManager::GetAuthTokenByTypeAndFeaturesTask*)mHost;
    return task->mResponse->OnError(errorCode, errorMessage);
}

ECode CChooseResponse::constructor(
    /* [in] */ Handle32 host)
{
    mHost = host;
    return NOERROR;
}
