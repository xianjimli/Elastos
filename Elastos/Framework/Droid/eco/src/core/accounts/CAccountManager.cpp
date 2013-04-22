
#include "ext/frameworkdef.h"
#include "accounts/CAccountManager.h"
#include "accounts/CAccountManagerResponse.h"
#include "accounts/CChooseResponse.h"
#include "accounts/CAccountManagerAmsResponse.h"
#include "accounts/CAccountManagerFutureResponse.h"
#include "accounts/CAccount.h"
#include "os/CApartment.h"
#include "os/CBundle.h"
#include "text/TextUtils.h"
#include "content/CIntent.h"
#include "content/CIntentFilter.h"

const CString CAccountManager::TAG = "AccountManager";

CAccountManager::CAccountManager()
{
    //mAccountsChangedBroadcastReceiver = new AccountsChangedBroadcastReceiver();
}

ECode CAccountManager::SanitizeResult(
    /* [in] */ IBundle* result,
    /* [out] */ IBundle** newResult)
{
    VALIDATE_NOT_NULL(newResult);
    if (result != NULL) {
        Boolean isContains;
        result->ContainsKey(String(AccountManager_KEY_AUTHTOKEN), &isContains);
        String value;
        result->GetString(String(AccountManager_KEY_AUTHTOKEN), &value);
        AutoPtr<ICharSequence> cs;
        FAIL_RETURN(CStringWrapper::New(value, (ICharSequence**)&cs));
        if (isContains && !TextUtils::IsEmpty(cs)) {
            CBundle::New(newResult);
            (*newResult)->PutString(String(AccountManager_KEY_AUTHTOKEN),
                    String("<omitted for logging purposes>"));
            return NOERROR;
        }
    }
    *newResult = result;
    return NOERROR;
}

ECode CAccountManager::Get(
    /* [in] */ IContext* context,
    /* [out] */ ILocalAccountManager** accountManager)
{
    VALIDATE_NOT_NULL(accountManager);
    if (context == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        //throw new IllegalArgumentException("context is null");
    }
    return context->GetSystemService(Context_ACCOUNT_SERVICE,
            (IInterface**)accountManager);
}

ECode CAccountManager::GetPassword(
        /* [in] */ IAccount* account,
        /* [out] */ String* password)
{
    VALIDATE_NOT_NULL(password);
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    //try {
    return mService->GetPassword(account, password);
    // } catch (RemoteException e) {
    //     // will never happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::GetUserData(
    /* [in] */ IAccount* account,
    /* [in] */ const String& key,
    /* [out] */ String* userData)
{
    VALIDATE_NOT_NULL(userData);
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    if (key.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("key is null");
    }
    //try {
    return mService->GetUserData(account, key, userData);
    // } catch (RemoteException e) {
    //     // will never happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::GetAuthenticatorTypes(
    /* [out] */ IObjectContainer** authenticators)
{
    VALIDATE_NOT_NULL(authenticators);
    //try {
    return mService->GetAuthenticatorTypes(authenticators);
    // } catch (RemoteException e) {
    //     // will never happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::GetAccounts(
    /* [out] */ IObjectContainer** accounts)
{
    VALIDATE_NOT_NULL(accounts);
    //try {
    return mService->GetAccounts(String(NULL), accounts);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::GetAccountsByType(
    /* [in] */ const String& type,
    /* [out] */ IObjectContainer** accounts)
{
    VALIDATE_NOT_NULL(accounts);
    //try {
    return mService->GetAccounts(type, accounts);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::HasFeatures(
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<String>& features,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    //if (features == null) throw new IllegalArgumentException("features is null");
    Future2Task_HasFeatures* future = new Future2Task_HasFeatures(
            handler, accountManagerCb, this, account, features);
    return future->Start(accountManagerFuture);
}

ECode CAccountManager::GetAccountsByTypeAndFeatures(
    /* [in] */ const String& type,
    /* [in] */ const ArrayOf<String>& features,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (type.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("type is null");
    }
    Future2Task_GetAccounts* future = new Future2Task_GetAccounts(
            handler, accountManagerCb, this, type, features);
    return future->Start(accountManagerFuture);
}

ECode CAccountManager::AddAccountExplicitly(
    /* [in] */ IAccount* account,
    /* [in] */ const String& password,
    /* [in] */ IBundle* userdata,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    //try {
    return mService->AddAccount(account, password, userdata, result);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::RemoveAccount(
    /* [in] */ IAccount* account,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    Future2Task_RemoveAccount* future = new Future2Task_RemoveAccount(
            handler, accountManagerCb, this, account);
    return future->Start(accountManagerFuture);
}

ECode CAccountManager::InvalidateAuthToken(
    /* [in] */ const String& accountType,
    /* [in] */ const String& authToken)
{
    if (accountType.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("accountType is null");
    }
    //try {
    if (!authToken.IsNull()) {
        return mService->InvalidateAuthToken(accountType, authToken);
    }

    return NOERROR;
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::PeekAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [out] */ String* token)
{
    VALIDATE_NOT_NULL(token);
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    if (authTokenType.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("authTokenType is null");
    }
    //try {
    return mService->PeekAuthToken(account, authTokenType, token);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::SetPassword(
    /* [in] */ IAccount* account,
    /* [in] */ const String& password)
{
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    //try {
    return mService->SetPassword(account, password);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::ClearPassword(
    /* [in] */ IAccount* account)
{
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    //try {
    return mService->ClearPassword(account);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::SetUserData(
    /* [in] */ IAccount* account,
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    if (key.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("key is null");
    }
    //try {
    return mService->SetUserData(account, key, value);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::SetAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ const String& authToken)
{
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    if (authTokenType.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("authTokenType is null");
    }
    //try {
    return mService->SetAuthToken(account, authTokenType, authToken);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::BlockingGetAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ Boolean notifyAuthFailure,
    /* [out] */ String* token)
{
    VALIDATE_NOT_NULL(token);
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    if (authTokenType.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("authTokenType is null");
    }

    AutoPtr<IAccountManagerFuture> accountManagerFuture;
    GetAuthTokenEx(account, authTokenType, notifyAuthFailure, NULL /* callback */,
            NULL /* handler */, (IAccountManagerFuture**)&accountManagerFuture);
    AutoPtr<IBundle> bundle;
    accountManagerFuture->GetResult((IInterface**)&bundle);
    if (bundle == NULL) {
        // This should never happen, but it does, occasionally. If it does return null to
        // signify that we were not able to get the authtoken.
        // TODO: remove this when the bug is found that sometimes causes a null bundle to be
        // returned
        // Log.e(TAG, "blockingGetAuthToken: null was returned from getResult() for "
        //         + account + ", authTokenType " + authTokenType);
        *token = String(NULL);
        return NOERROR;
    }
    return bundle->GetString(String(AccountManager_KEY_AUTHTOKEN), token);
}

ECode CAccountManager::GetAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ IBundle* options,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    if (authTokenType.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("authTokenType is null");
    }
    AmsTask_GetAuthToken* task = new AmsTask_GetAuthToken(
            activity, handler, accountManagerCb, this, account, authTokenType,
            FALSE /* notifyOnAuthFailure */, TRUE /* expectActivityLaunch */, options);
    return task->Start(accountManagerFuture);
}

ECode CAccountManager::GetAuthTokenEx(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ Boolean notifyAuthFailure,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    if (authTokenType.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("authTokenType is null");
    }
    AmsTask_GetAuthToken* task = new AmsTask_GetAuthToken(
            NULL, handler, accountManagerCb, this, account,
            authTokenType, notifyAuthFailure, FALSE /* expectActivityLaunch */,
            NULL /* options */);
    return task->Start(accountManagerFuture);
}

ECode CAccountManager::AddAccount(
    /* [in] */ const String& accountType,
    /* [in] */ const String& authTokenType,
    /* [in] */ const ArrayOf<String>& requiredFeatures,
    /* [in] */ IBundle* addAccountOptions,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (accountType.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("accountType is null");
    }
    AmsTask_AddAccount* task = new AmsTask_AddAccount(
            activity, handler, accountManagerCb, this, accountType, authTokenType,
            requiredFeatures, addAccountOptions);
    return task->Start(accountManagerFuture);
}

ECode CAccountManager::ConfirmCredentials(
    /* [in] */ IAccount* account,
    /* [in] */ IBundle* options,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    AmsTask_ConfirmCredentials* task = new AmsTask_ConfirmCredentials(
            activity, handler, accountManagerCb, this, account, options);
    return task->Start(accountManagerFuture);
}

ECode CAccountManager::UpdateCredentials(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ IBundle* options,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (account == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    AmsTask_UpdateCredentials* task = new AmsTask_UpdateCredentials(
            activity, handler, accountManagerCb, this, account, authTokenType, options);
    return task->Start(accountManagerFuture);
}

ECode CAccountManager::EditProperties(
    /* [in] */ const String& accountType,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IApartment* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (accountType.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("accountType is null");
    }
    AmsTask_EditProperties* task = new AmsTask_EditProperties(
            activity, handler, accountManagerCb, this, accountType);
    return task->Start(accountManagerFuture);
}

ECode CAccountManager::EnsureNotOnMainThread()
{
    return NOERROR;
    AutoPtr<IApartment> myApartment;
    ASSERT_SUCCEEDED(CApartment::GetMyApartment((IApartment**)&myApartment));
    if (myApartment != NULL && myApartment == mMainHandler) {
        // final IllegalStateException exception = new IllegalStateException(
        //         "calling this from your main thread can lead to deadlock");
        // Log.e(TAG, "calling this from your main thread can lead to deadlock and/or ANRs",
        //         exception);
        AutoPtr<IApplicationInfo> info;
        mContext->GetApplicationInfo((IApplicationInfo**)&info);
        Int32 version;
        info->GetTargetSdkVersion(&version);
        if (version >= 8/*Build.VERSION_CODES.FROYO*/) {
            return E_ILLEGAL_STATE_EXCEPTION;
//            throw exception;
        }
    }
    return NOERROR;
}

void CAccountManager::PostToHandler(
    /* [in] */ IApartment* handler,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IAccountManagerFuture* future)
{
    handler = handler == NULL ? mMainHandler.Get() : handler;
    AutoPtr<IRunnable> action =
            (IRunnable*)new CallbackAction(accountManagerCb, future);
    ECode (STDCALL IRunnable::*pHandlerFunc)();
    pHandlerFunc = &IRunnable::Run;

    handler->PostCppCallback(
            (Handle32)action.Get(), *(Handle32*)&pHandlerFunc, NULL, 0);
}

void CAccountManager::PostToHandler(
    /* [in] */ IApartment* handler,
    /* [in] */ IOnAccountsUpdateListener* listener,
    /* [in] */ IObjectContainer* accounts)
{
    //final Account[] accountsCopy = new Account[accounts.length];
    // send a copy to make sure that one doesn't
    // change what another sees
    //System.arraycopy(accounts, 0, accountsCopy, 0, accountsCopy.length);
    handler = (handler == NULL) ? mMainHandler.Get() : handler;
    AutoPtr<IRunnable> action =
            (IRunnable*)new AccountUpdateAction(listener, accounts);
    ECode (STDCALL IRunnable::*pHandlerFunc)();
    pHandlerFunc = &IRunnable::Run;

    handler->PostCppCallback(
        (Handle32)action.Get(), *(Handle32*)&pHandlerFunc, NULL, 0);
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
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (accountType.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account type is null");
    }
    if (authTokenType.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("authTokenType is null");
    }
    GetAuthTokenByTypeAndFeaturesTask* task =
            new GetAuthTokenByTypeAndFeaturesTask(accountType, authTokenType, features,
            activity, addAccountOptions, getAuthTokenOptions, accountManagerCb, handler, this);
    return task->Start(accountManagerFuture);
}

ECode CAccountManager::AddOnAccountsUpdatedListener(
    /* [in] */ IOnAccountsUpdateListener* listener,
    /* [in] */ IApartment* handler,
    /* [in] */ Boolean updateImmediately)
{
    if (listener == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("the listener is null");
    }

    {
        Mutex::Autolock lock(mAccountsUpdatedListenersLock);
        HashMap<AutoPtr<IOnAccountsUpdateListener>, AutoPtr<IApartment> >::Iterator it;
        AutoPtr<IOnAccountsUpdateListener> key;
        for (it = mAccountsUpdatedListeners.Begin(); it != mAccountsUpdatedListeners.End(); ++it) {
            key = it->mFirst;
            if (key.Get() == listener) {
                return E_ILLEGAL_STATE_EXCEPTION;
                //throw new IllegalStateException("this listener is already added");
            }
        }

        Boolean wasEmpty = mAccountsUpdatedListeners.Begin() == mAccountsUpdatedListeners.End();

        mAccountsUpdatedListeners[listener] = handler;

        if (wasEmpty) {
            // Register a broadcast receiver to monitor account changes
            AutoPtr<IIntentFilter> intentFilter;
            CIntentFilter::New((IIntentFilter**)&intentFilter);
            intentFilter->AddAction(String(AccountManager_LOGIN_ACCOUNTS_CHANGED_ACTION));
            // To recover from disk-full.
            intentFilter->AddAction(String(Intent_ACTION_DEVICE_STORAGE_OK));
            //mContext.registerReceiver(mAccountsChangedBroadcastReceiver, intentFilter);
        }
    }

    if (updateImmediately) {
        AutoPtr<IObjectContainer> accounts;
        GetAccounts((IObjectContainer**)&accounts);
        PostToHandler(handler, listener, accounts);
    }
    return NOERROR;
}

ECode CAccountManager::RemoveOnAccountsUpdatedListener(
    /* [in] */ IOnAccountsUpdateListener* listener)
{
    if (listener == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("listener is null");
    }
    Mutex::Autolock lock(mAccountsUpdatedListenersLock);
    HashMap<AutoPtr<IOnAccountsUpdateListener>, AutoPtr<IApartment> >::Iterator it
            = mAccountsUpdatedListeners.Find(listener);
    if (it == mAccountsUpdatedListeners.End()) {
        //Log.e(TAG, "Listener was not previously added");
        return NOERROR;
    }

    mAccountsUpdatedListeners.Erase(it);
    if (mAccountsUpdatedListeners.Begin() == mAccountsUpdatedListeners.End()) {
        //mContext.unregisterReceiver(mAccountsChangedBroadcastReceiver);
    }

    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAccountManager* service)
{
    mContext = context;
    mService = service;
    ASSERT_SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mMainHandler));
    return NOERROR;
}

ECode CAccountManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAccountManager* service,
    /* [in] */ IApartment* handler)
{
    mContext = context;
    mService = service;
    mMainHandler = handler;
    return NOERROR;
}

CAccountManager::AmsTask::AmsTask(
    /* [in] */ IActivity* activity,
    /* [in] */ IApartment* handler,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ CAccountManager* host)
    : mHandler(handler)
    , mCallback(accountManagerCb)
    , mActivity(activity)
    , mHost(host)
{
    // super(new Callable<Bundle>() {
    //     public Bundle call() throws Exception {
    //         throw new IllegalStateException("this should never be called");
    //     }
    // });
    ASSERT_SUCCEEDED(CAccountManagerAmsResponse::New((Handle32)this,
            (IAccountManagerResponse**)&mResponse));
}

PInterface CAccountManager::AmsTask::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IAccountManagerFuture*)this;
    }
    else if (riid == EIID_IAccountManagerFuture) {
        return (IAccountManagerFuture*)this;
    }

    return NULL;
}

UInt32 CAccountManager::AmsTask::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAccountManager::AmsTask::Release()
{
    return ElRefBase::Release();
}

ECode CAccountManager::AmsTask::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::AmsTask::Cancel(
    /* [in] */ Boolean mayInterruptIfRunning,
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

ECode CAccountManager::AmsTask::IsCancelled(
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

ECode CAccountManager::AmsTask::IsDone(
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

ECode CAccountManager::AmsTask::GetResult(
    /* [out] */ IInterface** result)
{
    return InternalGetResult(-1/*null*/, /*NULL,*/ (IBundle**)result);
}

ECode CAccountManager::AmsTask::GetResultEx(
    /* [in] */ Int64 timeout,
    //[in] TimeUnit unit,
    /* [out] */ IInterface** result)
{
    return InternalGetResult(timeout, /*unit,*/ (IBundle**)result);
}

ECode CAccountManager::AmsTask::Start(
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    //try {
    FAIL_RETURN(DoWork());
    // } catch (RemoteException e) {
    //     setException(e);
    // }
    *accountManagerFuture = (IAccountManagerFuture*)Probe(EIID_IAccountManagerFuture);
    return NOERROR;
}

ECode CAccountManager::AmsTask::Set(
    /* [in] */ IBundle* bundle)
{
    //TODO: somehow a null is being set as the result of the Future. Log this
    // case to help debug where this is occurring. When this bug is fixed this
    // condition statement should be removed.
    if (bundle == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        //Log.e(TAG, "the bundle must not be null", new Exception());
    }
    //super.set(bundle);
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::AmsTask::InternalGetResult(
    /* [in] */ Int64 timeout,
    /*TimeUnit unit*/
    /* [out] */ IBundle** result)
{
    Boolean isDone;
    IsDone(&isDone);
    if (!isDone) {
        mHost->EnsureNotOnMainThread();
    }
    //try {
    // if (timeout == null) {
    //     return get();
    // } else {
    //     return get(timeout, unit);
    // }
    // } catch (CancellationException e) {
    //     throw new OperationCanceledException();
    // } catch (TimeoutException e) {
    //     // fall through and cancel
    // } catch (InterruptedException e) {
    //     // fall through and cancel
    // } catch (ExecutionException e) {
    //     final Throwable cause = e.getCause();
    //     if (cause instanceof IOException) {
    //         throw (IOException) cause;
    //     } else if (cause instanceof UnsupportedOperationException) {
    //         throw new AuthenticatorException(cause);
    //     } else if (cause instanceof AuthenticatorException) {
    //         throw (AuthenticatorException) cause;
    //     } else if (cause instanceof RuntimeException) {
    //         throw (RuntimeException) cause;
    //     } else if (cause instanceof Error) {
    //         throw (Error) cause;
    //     } else {
    //         throw new IllegalStateException(cause);
    //     }
    // } finally {
    //     cancel(true /* interrupt if running */);
    // }
    return E_NOT_IMPLEMENTED;
}

void CAccountManager::AmsTask::Done()
{
    if (mCallback != NULL) {
        mHost->PostToHandler(mHandler, mCallback, (IAccountManagerFuture*)this);
    }
}

CAccountManager::AmsTask_GetAuthToken::AmsTask_GetAuthToken(
    /* [in] */ IActivity* activity,
    /* [in] */ IApartment* handler,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ CAccountManager* host,
    /* [in] */ IAccount* account,
    /* [in] */ String authTokenType,
    /* [in] */ Boolean notifyAuthFailure,
    /* [in] */ Boolean expectActivityLaunch,
    /* [in] */ IBundle* options)
    : AmsTask(activity, handler, accountManagerCb, host)
    , mAccount(account)
    , mAuthTokenType(authTokenType)
    , mNotifyAuthFailure(notifyAuthFailure)
    , mExpectActivityLaunch(expectActivityLaunch)
    , mOptions(options)
{}

CAccountManager::AmsTask_GetAuthToken::~AmsTask_GetAuthToken()
{}

ECode CAccountManager::AmsTask_GetAuthToken::DoWork()
{
    return mHost->mService->GetAuthToken(mResponse, mAccount, mAuthTokenType,
            mNotifyAuthFailure, mExpectActivityLaunch, mOptions);
}

CAccountManager::AmsTask_AddAccount::AmsTask_AddAccount(
    /* [in] */ IActivity* activity,
    /* [in] */ IApartment* handler,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ CAccountManager* host,
    /* [in] */ String accountType,
    /* [in] */ String authTokenType,
    /* [in] */ const ArrayOf<String>& requiredFeatures,
    /* [in] */ IBundle* options)
    : AmsTask(activity, handler, accountManagerCb, host)
    , mAccountType(accountType)
    , mAuthTokenType(authTokenType)
    , mOptions(options)
{
    mRequiredFeatures = requiredFeatures.Clone();
}

CAccountManager::AmsTask_AddAccount::~AmsTask_AddAccount()
{
    ArrayOf<String>::Free(mRequiredFeatures);
}

ECode CAccountManager::AmsTask_AddAccount::DoWork()
{
    return mHost->mService->AddAcount(mResponse, mAccountType, mAuthTokenType,
            *mRequiredFeatures, mActivity != NULL, mOptions);
}

CAccountManager::AmsTask_ConfirmCredentials::AmsTask_ConfirmCredentials(
    /* [in] */ IActivity* activity,
    /* [in] */ IApartment* handler,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ CAccountManager* host,
    /* [in] */ IAccount* account,
    /* [in] */ IBundle* options)
    : AmsTask(activity, handler, accountManagerCb, host)
    , mAccount(account)
    , mOptions(options)
{}

CAccountManager::AmsTask_ConfirmCredentials::~AmsTask_ConfirmCredentials()
{}

ECode CAccountManager::AmsTask_ConfirmCredentials::DoWork()
{
    return mHost->mService->ConfirmCredentials(mResponse, mAccount,
            mOptions, mActivity != NULL);
}

CAccountManager::AmsTask_UpdateCredentials::AmsTask_UpdateCredentials(
    /* [in] */ IActivity* activity,
    /* [in] */ IApartment* handler,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ CAccountManager* host,
    /* [in] */ IAccount* account,
    /* [in] */ String authTokenType,
    /* [in] */ IBundle* options)
    : AmsTask(activity, handler, accountManagerCb, host)
    , mAccount(account)
    , mAuthTokenType(authTokenType)
    , mOptions(options)
{}

CAccountManager::AmsTask_UpdateCredentials::~AmsTask_UpdateCredentials()
{}

ECode CAccountManager::AmsTask_UpdateCredentials::DoWork()
{
    return mHost->mService->UpdateCredentials(mResponse, mAccount,
            mAuthTokenType, mActivity != NULL, mOptions);
}

CAccountManager::AmsTask_EditProperties::AmsTask_EditProperties(
    /* [in] */ IActivity* activity,
    /* [in] */ IApartment* handler,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ CAccountManager* host,
    /* [in] */ String authTokenType)
    : AmsTask(activity, handler, accountManagerCb, host)
    , mAuthTokenType(authTokenType)
{}

CAccountManager::AmsTask_EditProperties::~AmsTask_EditProperties()
{}

ECode CAccountManager::AmsTask_EditProperties::DoWork()
{
    return mHost->mService->EditProperties(mResponse,
            mAuthTokenType, mActivity != NULL);
}

CAccountManager::BaseFutureTask::BaseFutureTask(
    /* [in] */ IApartment* handler,
    /* [in] */ CAccountManager* host)
    : mHandler(handler)
    , mHost(host)
{
    // super(new Callable<T>() {
    //     public T call() throws Exception {
    //         throw new IllegalStateException("this should never be called");
    //     }
    // });
    ASSERT_SUCCEEDED(CAccountManagerFutureResponse::New((Handle32)this,
            (IAccountManagerResponse**)&mResponse));
}

void CAccountManager::BaseFutureTask::PostRunnableToHandler(
    /* [in] */ IRunnable* runnable)
{
    AutoPtr<IApartment> handler = (mHandler == NULL) ? mHost->mMainHandler : mHandler;
    ECode (STDCALL IRunnable::*pHandlerFunc)();
    pHandlerFunc = &IRunnable::Run;
    handler->PostCppCallback(
            (Handle32)runnable, *(Handle32*)&pHandlerFunc, NULL, 0);
}

ECode CAccountManager::BaseFutureTask::StartTask()
{
    //try {
    return DoWork();
    // } catch (RemoteException e) {
    //     setException(e);
    // }
}

CAccountManager::Future2Task::Future2Task(
    /* [in] */ IApartment* handler,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ CAccountManager* host)
    : BaseFutureTask(handler, host)
    , mCallback(accountManagerCb)
{}

PInterface CAccountManager::Future2Task::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IAccountManagerFuture*)this;
    }
    else if (riid == EIID_IAccountManagerFuture) {
        return (IAccountManagerFuture*)this;
    }

    return NULL;
}

UInt32 CAccountManager::Future2Task::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAccountManager::Future2Task::Release()
{
    return ElRefBase::Release();
}

ECode CAccountManager::Future2Task::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::Future2Task::Cancel(
    /* [in] */ Boolean mayInterruptIfRunning,
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

ECode CAccountManager::Future2Task::IsCancelled(
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

ECode CAccountManager::Future2Task::IsDone(
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

ECode CAccountManager::Future2Task::GetResult(
    /* [out] */ IInterface** result)
{
    return InternalGetResult(-1/*null*/, /*null,*/ result);
}

ECode CAccountManager::Future2Task::GetResultEx(
    /* [in] */ Int64 timeout,
    //[in] TimeUnit unit,
    /* [out] */ IInterface** result)
{
    return InternalGetResult(timeout, /*null,*/ result);
}

void CAccountManager::Future2Task::Done()
{
    if (mCallback != NULL) {
        AutoPtr<IRunnable> action =
                (IRunnable*)new CAccountManager::CallbackAction(mCallback,
                        (IAccountManagerFuture*)this);
        PostRunnableToHandler(action);
    }
}

ECode CAccountManager::Future2Task::Start(
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    StartTask();
    *accountManagerFuture = (IAccountManagerFuture*)Probe(EIID_IAccountManagerFuture);
    return NOERROR;
}

ECode CAccountManager::Future2Task::InternalGetResult(
    /* [in] */ Int64 timeout,
    /*TimeUnit unit*/
    /* [out] */ IInterface** result)
{
    Boolean isDone;
    IsDone(&isDone);
    if (!isDone) {
        mHost->EnsureNotOnMainThread();
    }
    //try {
    // if (timeout == null) {
    //     return get();
    // } else {
    //     return get(timeout, unit);
    // }
    // } catch (InterruptedException e) {
    //     // fall through and cancel
    // } catch (TimeoutException e) {
    //     // fall through and cancel
    // } catch (CancellationException e) {
    //     // fall through and cancel
    // } catch (ExecutionException e) {
    //     final Throwable cause = e.getCause();
    //     if (cause instanceof IOException) {
    //         throw (IOException) cause;
    //     } else if (cause instanceof UnsupportedOperationException) {
    //         throw new AuthenticatorException(cause);
    //     } else if (cause instanceof AuthenticatorException) {
    //         throw (AuthenticatorException) cause;
    //     } else if (cause instanceof RuntimeException) {
    //         throw (RuntimeException) cause;
    //     } else if (cause instanceof Error) {
    //         throw (Error) cause;
    //     } else {
    //         throw new IllegalStateException(cause);
    //     }
    // } finally {
    //     cancel(true /* interrupt if running */);
    // }
    //throw new OperationCanceledException();

    return E_NOT_IMPLEMENTED;
}

CAccountManager::Future2Task_HasFeatures::Future2Task_HasFeatures(
    /* [in] */ IApartment* handler,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ CAccountManager* host,
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<String>& features)
    : Future2Task(handler, accountManagerCb, host)
    , mAccount(account)
{
    mFeatures = features.Clone();
}

CAccountManager::Future2Task_HasFeatures::~Future2Task_HasFeatures()
{
    ArrayOf<String>::Free(mFeatures);
}

ECode CAccountManager::Future2Task_HasFeatures::DoWork()
{
    return mHost->mService->HasFeatures(mResponse, mAccount, *mFeatures);
}

ECode CAccountManager::Future2Task_HasFeatures::BundleToResult(
    /* [in] */ IBundle* bundle,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);

    Boolean isContains;
    bundle->ContainsKey(String(AccountManager_KEY_BOOLEAN_RESULT), &isContains);
    if (!isContains) {
        return E_AUTHENTICATOR_EXCEPTION;
//        throw new AuthenticatorException("no result in response");
    }
    Boolean res;
    bundle->GetBoolean(String(AccountManager_KEY_BOOLEAN_RESULT), &res);
    return CBoolean::New(res, (IBoolean**)result);
}

CAccountManager::Future2Task_GetAccounts::Future2Task_GetAccounts(
    /* [in] */ IApartment* handler,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ CAccountManager* host,
    /* [in] */ String type,
    /* [in] */ const ArrayOf<String>& features)
    : Future2Task(handler, accountManagerCb, host)
    , mType(type)
{
    mFeatures = features.Clone();
}

CAccountManager::Future2Task_GetAccounts::~Future2Task_GetAccounts()
{
    ArrayOf<String>::Free(mFeatures);
}

ECode CAccountManager::Future2Task_GetAccounts::DoWork()
{
    return mHost->mService->GetAccountsByFeatures(mResponse, mType, *mFeatures);
}

ECode CAccountManager::Future2Task_GetAccounts::BundleToResult(
    /* [in] */ IBundle* bundle,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);

    Boolean isContains;
    bundle->ContainsKey(String(AccountManager_KEY_BOOLEAN_RESULT), &isContains);
    if (!isContains) {
        return E_AUTHENTICATOR_EXCEPTION;
//        throw new AuthenticatorException("no result in response");
    }
    ArrayOf<IParcelable*>* parcelables;
    bundle->GetParcelableArray(String(AccountManager_KEY_ACCOUNTS), &parcelables);
    AutoPtr<IObjectContainer> descs;
    CObjectContainer::New((IObjectContainer**)&descs);
    for (Int32 i = 0; i < parcelables->GetLength(); i++) {
        AutoPtr<IAccount> account;
        if ((*parcelables)[i] != NULL &&
                (*parcelables)[i]->Probe(EIID_IAccount) != NULL){
            account = (IAccount*)(*parcelables)[i]->Probe(EIID_IAccount);
        }
        descs->Add(account);
    }
    *result = (IInterface*)descs;
    return NOERROR;
}

CAccountManager::Future2Task_RemoveAccount::Future2Task_RemoveAccount(
    /* [in] */ IApartment* handler,
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ CAccountManager* host,
    /* [in] */ IAccount* account)
    : Future2Task(handler, accountManagerCb, host)
    , mAccount(account)
{}

CAccountManager::Future2Task_RemoveAccount::~Future2Task_RemoveAccount()
{}

ECode CAccountManager::Future2Task_RemoveAccount::DoWork()
{
    return mHost->mService->RemoveAccount(mResponse, mAccount);
}

ECode CAccountManager::Future2Task_RemoveAccount::BundleToResult(
    /* [in] */ IBundle* bundle,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);

    Boolean isContains;
    bundle->ContainsKey(String(AccountManager_KEY_BOOLEAN_RESULT), &isContains);
    if (!isContains) {
        return E_AUTHENTICATOR_EXCEPTION;
//        throw new AuthenticatorException("no result in response");
    }
    Boolean res;
    bundle->GetBoolean(String(AccountManager_KEY_BOOLEAN_RESULT), &res);
    return CBoolean::New(res, (IBoolean**)result);
}

ECode CAccountManager::ConvertErrorToException(
    /* [in] */ Int32 code
    /*String message*/)
{
    if (code == AccountManager_ERROR_CODE_NETWORK_ERROR) {
        return E_IO_EXCEPTION;
//        return new IOException(message);
    }

    if (code == AccountManager_ERROR_CODE_UNSUPPORTED_OPERATION) {
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
//        return new UnsupportedOperationException(message);
    }

    if (code == AccountManager_ERROR_CODE_INVALID_RESPONSE) {
        return E_AUTHENTICATOR_EXCEPTION;
//        return new AuthenticatorException(message);
    }

    if (code == AccountManager_ERROR_CODE_BAD_ARGUMENTS) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        return new IllegalArgumentException(message);
    }

    return E_AUTHENTICATOR_EXCEPTION;
//    return new AuthenticatorException(message);
}

CAccountManager::CallbackAction::CallbackAction(
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IAccountManagerFuture* future)
    : mCallback(accountManagerCb)
    , mFuture(future)
{}

ECode CAccountManager::CallbackAction::Run()
{
    return mCallback->Run(mFuture);
}

CAccountManager::AccountUpdateAction::AccountUpdateAction(
    /* [in] */ IOnAccountsUpdateListener* listener,
    /* [in] */ IObjectContainer* accounts)
    : mListener(listener)
    , mAccounts(accounts)
{}

ECode CAccountManager::AccountUpdateAction::Run()
{
    //try {
    return mListener->OnAccountsUpdated(mAccounts);
    // } catch (SQLException e) {
    //     // Better luck next time.  If the problem was disk-full,
    //     // the STORAGE_OK intent will re-trigger the update.
    //     Log.e(TAG, "Can't update accounts", e);
    // }
}

CAccountManager::GetAuthTokenByTypeAndFeaturesTask::GetAuthTokenByTypeAndFeaturesTask(
    /* [in] */ const String& accountType,
    /* [in] */ const String& authTokenType,
    /* [in] */ const ArrayOf<String>& features,
    /* [in] */ IActivity* activityForPrompting,
    /* [in] */ IBundle* addAccountOptions,
    /* [in] */ IBundle* loginOptions,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IApartment* handler,
    /* [in] */ CAccountManager* host)
    : AmsTask(activityForPrompting, handler, cb, host)
    , mNumAccounts(0)
{}

PInterface CAccountManager::GetAuthTokenByTypeAndFeaturesTask::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IAccountManagerCallback*)this;
    }
    else if (riid == EIID_IAccountManagerCallback) {
        return (IAccountManagerCallback*)this;
    }
    else {
        return AmsTask::Probe(riid);
    }
}

UInt32 CAccountManager::GetAuthTokenByTypeAndFeaturesTask::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAccountManager::GetAuthTokenByTypeAndFeaturesTask::Release()
{
    return ElRefBase::Release();
}

ECode CAccountManager::GetAuthTokenByTypeAndFeaturesTask::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::GetAuthTokenByTypeAndFeaturesTask::Run(
    /* [in] */ IAccountManagerFuture* future)
{
    //try {
    AutoPtr<IBundle> result;
    future->GetResult((IInterface**)&result);
    if (mNumAccounts == 0) {
        String accountName, accountType;
        result->GetString(String(AccountManager_KEY_ACCOUNT_NAME), &accountName);
        result->GetString(String(AccountManager_KEY_ACCOUNT_TYPE), &accountType);
        AutoPtr<ICharSequence> cs1, cs2;
        CStringWrapper::New(accountName, (ICharSequence**)&cs1);
        CStringWrapper::New(accountType, (ICharSequence**)&cs2);
        if (TextUtils::IsEmpty(cs1) || TextUtils::IsEmpty(cs2)) {
            return E_AUTHENTICATOR_EXCEPTION;
//            setException(new AuthenticatorException("account not in result"));
        }
        AutoPtr<IAccount> account;
        CAccount::New(accountName, accountType, (IAccount**)&account);
        mNumAccounts = 1;
        AutoPtr<IAccountManagerFuture> f;
        return mHost->GetAuthToken(account, mAuthTokenType, NULL /* options */, mActivity,
                mMyCallback, mHandler, (IAccountManagerFuture**)&f);
    }
    return Set(result);
    // } catch (OperationCanceledException e) {
    //     cancel(true /* mayInterruptIfRUnning */);
    // } catch (IOException e) {
    //     setException(e);
    // } catch (AuthenticatorException e) {
    //     setException(e);
    // }
}

ECode CAccountManager::GetAuthTokenByTypeAndFeaturesTask::DoWork()
{
    AutoPtr<IAccountManagerFuture> future;
    return mHost->GetAccountsByTypeAndFeatures(mAccountType, *mFeatures,
            (IAccountManagerCallback*)new GetAccountsCallback(this), mHandler,
            (IAccountManagerFuture**)&future);
}

CAccountManager::GetAuthTokenByTypeAndFeaturesTask::GetAccountsCallback::GetAccountsCallback(
    /* [in] */ GetAuthTokenByTypeAndFeaturesTask* host)
    : mHost(host)
{}

PInterface CAccountManager::GetAuthTokenByTypeAndFeaturesTask::GetAccountsCallback::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IAccountManagerCallback*)this;
    }
    else if (riid == EIID_IAccountManagerCallback) {
        return (IAccountManagerCallback*)this;
    }
    return NULL;
}

UInt32 CAccountManager::GetAuthTokenByTypeAndFeaturesTask::GetAccountsCallback::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAccountManager::GetAuthTokenByTypeAndFeaturesTask::GetAccountsCallback::Release()
{
    return ElRefBase::Release();
}

ECode CAccountManager::GetAuthTokenByTypeAndFeaturesTask::GetAccountsCallback::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::GetAuthTokenByTypeAndFeaturesTask::GetAccountsCallback::Run(
    /* [in] */ IAccountManagerFuture* future)
{
    AutoPtr<IObjectContainer> accounts;//Account[] accounts;
    //try {
    FAIL_RETURN(future->GetResult((IInterface**)&accounts));
    // } catch (OperationCanceledException e) {
    //     setException(e);
    //     return;
    // } catch (IOException e) {
    //     setException(e);
    //     return;
    // } catch (AuthenticatorException e) {
    //     setException(e);
    //     return;
    // }

    //mNumAccounts = accounts.length;
    AutoPtr<IObjectEnumerator> enumerator;
    accounts->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext = FALSE;
    enumerator->MoveNext(&hasNext);
    AutoPtr<IAccount> account;
    if (!hasNext) {
        mHost->mNumAccounts = 0;
        if (mHost->mActivity != NULL) {
            // no accounts, add one now. pretend that the user directly
            // made this request
            mHost->mHost->AddAccount(mHost->mAccountType, mHost->mAuthTokenType,
                    *(mHost->mFeatures), mHost->mAddAccountOptions,
                    mHost->mActivity, mHost->mMyCallback, mHost->mHandler,
                    (IAccountManagerFuture**)&(mHost->mFuture));
        }
        else {
            // send result since we can't prompt to add an account
            AutoPtr<IBundle> result;
            CBundle::New((IBundle**)&result);
            result->PutString(String(AccountManager_KEY_ACCOUNT_NAME), String(NULL));
            result->PutString(String(AccountManager_KEY_ACCOUNT_TYPE), String(NULL));
            result->PutString(String(AccountManager_KEY_AUTHTOKEN), String(NULL));
            //try {
            mHost->mResponse->OnResult(result);
            // } catch (RemoteException e) {
            //     // this will never happen
            // }
            // we are done
        }
    }
    else {
        AutoPtr<IAccount> account;
        enumerator->Current((IInterface**)&account);
        enumerator->MoveNext(&hasNext);
        if (!hasNext) {
            mHost->mNumAccounts = 1;
            // have a single account, return an authtoken for it
            if (mHost->mActivity == NULL) {
                mHost->mHost->GetAuthTokenEx(account, mHost->mAuthTokenType,
                        FALSE /* notifyAuthFailure */, mHost->mMyCallback,
                        mHost->mHandler, (IAccountManagerFuture**)&(mHost->mFuture));
            }
        }
        else {
            mHost->mNumAccounts = 2;
            if (mHost->mActivity != NULL) {
                AutoPtr<IAccountManagerResponse> chooseResponse;
                CChooseResponse::New((Handle32)this,
                        (IAccountManagerResponse**)&chooseResponse);
                // have many accounts, launch the chooser
                AutoPtr<IIntent> intent;
                CIntent::New((IIntent**)&intent);
                intent->SetClassNameEx(String("elastos"),
                        String("elastos.accounts.ChooseAccountActivity"), NULL);
                intent->PutInt32Extra(
                        String(AccountManager_KEY_ACCOUNTS), (Int32)accounts.Get());
                AutoPtr<IAccountManagerResponse> response;
                FAIL_RETURN(CAccountManagerResponse::New(chooseResponse,
                        (IAccountManagerResponse**)&response));
                intent->PutParcelableExtra(String(AccountManager_KEY_ACCOUNT_MANAGER_RESPONSE),
                        (IParcelable*)response->Probe(EIID_IParcelable));
                mHost->mActivity->StartActivity(intent);
                // the result will arrive via the IAccountManagerResponse
            }
            else {
                // send result since we can't prompt to select an account
                AutoPtr<IBundle> result;
                CBundle::New((IBundle**)&result);
                result->PutString(String(AccountManager_KEY_ACCOUNTS), String(NULL));
                //try {
                mHost->mResponse->OnResult(result);
                // } catch (RemoteException e) {
                //     // this will never happen
                // }
                // we are done
            }
        }
    }

    return NOERROR;
}
