
#include "accounts/CAccountManager.h"

const CString CAccountManager::TAG = "AccountManager";
const Int32 CAccountManager::ERROR_CODE_REMOTE_EXCEPTION;
const Int32 CAccountManager::ERROR_CODE_NETWORK_ERROR;
const Int32 CAccountManager::ERROR_CODE_CANCELED;
const Int32 CAccountManager::ERROR_CODE_INVALID_RESPONSE;
const Int32 CAccountManager::ERROR_CODE_UNSUPPORTED_OPERATION;
const Int32 CAccountManager::ERROR_CODE_BAD_ARGUMENTS;
const Int32 CAccountManager::ERROR_CODE_BAD_REQUEST;

CAccountManager::CAccountManager()
{}

ECode CAccountManager::GetPassword(
        /* [in] */ IAccount* account,
        /* [out] */ String* password)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::GetUserData(
    /* [in] */ IAccount* account,
    /* [in] */ const String& key,
    /* [out] */ String* userData)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::GetAuthenticatorTypes(
    /* [out] */ IObjectContainer** authenticators)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::GetAccounts(
    /* [out] */ IObjectContainer** accounts)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::GetAccountsByType(
    /* [in] */ const String& type,
    /* [out] */ IObjectContainer** accounts)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::HasFeatures(
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<String>& features,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::GetAccountsByTypeAndFeatures(
    /* [in] */ const String& type,
    /* [in] */ const ArrayOf<String>& features,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::AddAccountExplicitly(
    /* [in] */ IAccount* account,
    /* [in] */ const String& password,
    /* [in] */ IBundle* userdata,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::RemoveAccount(
    /* [in] */ IAccount* account,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::InvalidateAuthToken(
    /* [in] */ const String& accountType,
    /* [in] */ const String& authToken)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::PeekAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [out] */ String* token)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::SetPassword(
    /* [in] */ IAccount* account,
    /* [in] */ const String& password)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::ClearPassword(
    /* [in] */ IAccount* account)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::SetUserData(
    /* [in] */ IAccount* account,
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::SetAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ const String& authToken)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::BlockingGetAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ Boolean notifyAuthFailure,
    /* [out] */ String* token)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::GetAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ IBundle* options,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accoutManagerFuture)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::GetAuthTokenEx(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ Boolean notifyAuthFailure,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accoutManagerFuture)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::AddAccount(
    /* [in] */ const String& accountType,
    /* [in] */ const String& authTokenType,
    /* [in] */ const ArrayOf<String>& requiredFeatures,
    /* [in] */ IBundle* addAccountOptions,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accoutManagerFuture)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::ConfirmCredentials(
    /* [in] */ IAccount* account,
    /* [in] */ IBundle* options,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accoutManagerFuture)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::UpdateCredentials(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ IBundle* options,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accoutManagerFuture)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::EditProperties(
    /* [in] */ const String& accountType,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accoutManagerFuture)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::GetAuthTokenByFeatures(
    /* [in] */ const String& accountType,
    /* [in] */ const String& authTokenType,
    /* [in] */ const ArrayOf<String>& features,
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* addAccountOptions,
    /* [in] */ IBundle* getAuthTokenOptions,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accoutManagerFuture)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::AddOnAccountsUpdatedListener(
    /* [in] */ IOnAccountsUpdateListener* listener,
    /* [in] */ IApartment* handler,
    /* [in] */ Boolean updateImmediately)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::RemoveOnAccountsUpdatedListener(
    /* [in] */ IOnAccountsUpdateListener* listener)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAccountManager* service)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAccountManager* service,
    /* [in] */ IApartment* handler)
{
    return E_NOT_IMPLEMENTED;
}
