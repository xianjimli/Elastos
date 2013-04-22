
#ifndef __CACCOUNTMANAGER_H__
#define __CACCOUNTMANAGER_H__

#include "_CAccountManager.h"
#include "os/Runnable.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IOnAccountsUpdateListener> >
{
    size_t operator()(AutoPtr<IOnAccountsUpdateListener> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

_ELASTOS_NAMESPACE_END

using namespace Elastos::Core::Threading;

class CAccountManagerAmsResponse;
class CAccountManagerFutureResponse;
class CChooseResponse;

CarClass(CAccountManager)
{
public:
    friend class CAccountManagerAmsResponse;
    friend class CAccountManagerFutureResponse;
    friend class CChooseResponse;

private:
    class AmsTask
            : public IAccountManagerFuture
            , public ElRefBase
    {
    public:
        AmsTask(
            /* [in] */ IActivity* activity,
            /* [in] */ IApartment* handler,
            /* [in] */ IAccountManagerCallback* accountManagerCb,
            /* [in] */ CAccountManager* host);

        virtual ~AmsTask() {}

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Cancel(
            /* [in] */ Boolean mayInterruptIfRunning,
            /* [out] */ Boolean* result);

        CARAPI IsCancelled(
            /* [out] */ Boolean* result);

        CARAPI IsDone(
            /* [out] */ Boolean* result);

        CARAPI GetResult(
            /* [out] */ IInterface** result);

        CARAPI GetResultEx(
            /* [in] */ Int64 timeout,
            //[in] TimeUnit unit,
            /* [out] */ IInterface** result);

        CARAPI Start(
            /* [out] */ IAccountManagerFuture** accountManagerFuture);

        virtual CARAPI DoWork() = 0;

        CARAPI Set(
            /* [in] */ IBundle* bundle);

    protected:
        CARAPI_(void) Done();

    private:
        CARAPI InternalGetResult(
            /* [in] */ Int64 timeout,
            /*TimeUnit unit*/
            /* [out] */ IBundle** result);

    public:
        AutoPtr<IAccountManagerResponse> mResponse;
        AutoPtr<IApartment> mHandler;
        AutoPtr<IAccountManagerCallback> mCallback;
        AutoPtr<IActivity> mActivity;
        AutoPtr<CAccountManager> mHost;
    };

    class AmsTask_GetAuthToken : public AmsTask
    {
    public:
        AmsTask_GetAuthToken(
            /* [in] */ IActivity* activity,
            /* [in] */ IApartment* handler,
            /* [in] */ IAccountManagerCallback* accountManagerCb,
            /* [in] */ CAccountManager* host,
            /* [in] */ IAccount* account,
            /* [in] */ String authTokenType,
            /* [in] */ Boolean notifyAuthFailure,
            /* [in] */ Boolean expectActivityLaunch,
            /* [in] */ IBundle* options);

        ~AmsTask_GetAuthToken();

        CARAPI DoWork();

    private:
        AutoPtr<IAccount> mAccount;
        String mAuthTokenType;
        Boolean mNotifyAuthFailure;
        Boolean mExpectActivityLaunch;
        AutoPtr<IBundle> mOptions;
    };

    class AmsTask_AddAccount : public AmsTask
    {
    public:
        AmsTask_AddAccount(
            /* [in] */ IActivity* activity,
            /* [in] */ IApartment* handler,
            /* [in] */ IAccountManagerCallback* accountManagerCb,
            /* [in] */ CAccountManager* host,
            /* [in] */ String accountType,
            /* [in] */ String authTokenType,
            /* [in] */ const ArrayOf<String>& requiredFeatures,
            /* [in] */ IBundle* options);

        ~AmsTask_AddAccount();

        CARAPI DoWork();

    private:
        String mAccountType;
        String mAuthTokenType;
        ArrayOf<String>* mRequiredFeatures;
        AutoPtr<IBundle> mOptions;
    };

    class AmsTask_ConfirmCredentials : public AmsTask
    {
    public:
        AmsTask_ConfirmCredentials(
            /* [in] */ IActivity* activity,
            /* [in] */ IApartment* handler,
            /* [in] */ IAccountManagerCallback* accountManagerCb,
            /* [in] */ CAccountManager* host,
            /* [in] */ IAccount* account,
            /* [in] */ IBundle* options);

        ~AmsTask_ConfirmCredentials();

        CARAPI DoWork();

    private:
        AutoPtr<IAccount> mAccount;
        AutoPtr<IBundle> mOptions;
    };

    class AmsTask_UpdateCredentials : public AmsTask
    {
    public:
        AmsTask_UpdateCredentials(
            /* [in] */ IActivity* activity,
            /* [in] */ IApartment* handler,
            /* [in] */ IAccountManagerCallback* accountManagerCb,
            /* [in] */ CAccountManager* host,
            /* [in] */ IAccount* account,
            /* [in] */ String authTokenType,
            /* [in] */ IBundle* options);

        ~AmsTask_UpdateCredentials();

        CARAPI DoWork();

    private:
        AutoPtr<IAccount> mAccount;
        String mAuthTokenType;
        AutoPtr<IBundle> mOptions;
    };

    class AmsTask_EditProperties : public AmsTask
    {
    public:
        AmsTask_EditProperties(
            /* [in] */ IActivity* activity,
            /* [in] */ IApartment* handler,
            /* [in] */ IAccountManagerCallback* accountManagerCb,
            /* [in] */ CAccountManager* host,
            /* [in] */ String authTokenType);

        ~AmsTask_EditProperties();

        CARAPI DoWork();

    private:
        String mAuthTokenType;
    };

    class BaseFutureTask/* extends FutureTask<T>*/
    {
    public:
        BaseFutureTask(
            /* [in] */ IApartment* handler,
            /* [in] */ CAccountManager* host);

        virtual CARAPI DoWork() = 0;

        virtual CARAPI BundleToResult(
            /* [in] */ IBundle* bundle,
            /* [out] */ IInterface** result) = 0;

    protected:
        CARAPI_(void) PostRunnableToHandler(
            /* [in] */ IRunnable* runnable);

        CARAPI StartTask();

    public:
        AutoPtr<IAccountManagerResponse> mResponse;
        AutoPtr<IApartment> mHandler;

    protected:
        AutoPtr<CAccountManager> mHost;
    };

    class Future2Task
            : public BaseFutureTask
            , public IAccountManagerFuture
            , public ElRefBase
    {
    public:
        Future2Task(
            /* [in] */ IApartment* handler,
            /* [in] */ IAccountManagerCallback* accountManagerCb,
            /* [in] */ CAccountManager* host);

        virtual ~Future2Task() {}

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Cancel(
            /* [in] */ Boolean mayInterruptIfRunning,
            /* [out] */ Boolean* result);

        CARAPI IsCancelled(
            /* [out] */ Boolean* result);

        CARAPI IsDone(
            /* [out] */ Boolean* result);

        CARAPI GetResult(
            /* [out] */ IInterface** result);

        CARAPI GetResultEx(
            /* [in] */ Int64 timeout,
            //[in] TimeUnit unit,
            /* [out] */ IInterface** result);

        CARAPI Start(
            /* [out] */ IAccountManagerFuture** accountManagerFuture);

    protected:
        CARAPI_(void) Done();

    private:
        CARAPI InternalGetResult(
            /* [in] */ Int64 timeout,
            /*TimeUnit unit*/
            /* [out] */ IInterface** result);

    private:
        AutoPtr<IAccountManagerCallback> mCallback;
    };

    class Future2Task_HasFeatures : public Future2Task
    {
    public:
        Future2Task_HasFeatures(
            /* [in] */ IApartment* handler,
            /* [in] */ IAccountManagerCallback* accountManagerCb,
            /* [in] */ CAccountManager* host,
            /* [in] */ IAccount* account,
            /* [in] */ const ArrayOf<String>& features);

        ~Future2Task_HasFeatures();

        CARAPI DoWork();

        CARAPI BundleToResult(
            /* [in] */ IBundle* bundle,
            /* [out] */ IInterface** result);

    private:
        AutoPtr<IAccount> mAccount;
        ArrayOf<String>* mFeatures;
    };

    class Future2Task_GetAccounts : public Future2Task
    {
    public:
        Future2Task_GetAccounts(
            /* [in] */ IApartment* handler,
            /* [in] */ IAccountManagerCallback* accountManagerCb,
            /* [in] */ CAccountManager* host,
            /* [in] */ String type,
            /* [in] */ const ArrayOf<String>& features);

        ~Future2Task_GetAccounts();

        CARAPI DoWork();

        CARAPI BundleToResult(
            /* [in] */ IBundle* bundle,
            /* [out] */ IInterface** result);

    private:
        String mType;
        ArrayOf<String>* mFeatures;
    };

    class Future2Task_RemoveAccount : public Future2Task
    {
    public:
        Future2Task_RemoveAccount(
            /* [in] */ IApartment* handler,
            /* [in] */ IAccountManagerCallback* accountManagerCb,
            /* [in] */ CAccountManager* host,
            /* [in] */ IAccount* account);

        ~Future2Task_RemoveAccount();

        CARAPI DoWork();

        CARAPI BundleToResult(
            /* [in] */ IBundle* bundle,
            /* [out] */ IInterface** result);

    private:
        AutoPtr<IAccount> mAccount;
    };

    class GetAuthTokenByTypeAndFeaturesTask
            : public AmsTask
            , public IAccountManagerCallback
    {
    private:
        class GetAccountsCallback
                : public IAccountManagerCallback
                , public ElRefBase
        {
        public:
            GetAccountsCallback(
                /* [in] */ GetAuthTokenByTypeAndFeaturesTask* host);

            CARAPI_(PInterface) Probe(
                /* [in]  */ REIID riid);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            CARAPI GetInterfaceID(
                /* [in] */ IInterface *pObject,
                /* [out] */ InterfaceID *pIID);

            CARAPI Run(
                /* [in] */ IAccountManagerFuture* future);

        private:
            GetAuthTokenByTypeAndFeaturesTask* mHost;
        };

    public:
        GetAuthTokenByTypeAndFeaturesTask(
            /* [in] */ const String& accountType,
            /* [in] */ const String& authTokenType,
            /* [in] */ const ArrayOf<String>& features,
            /* [in] */ IActivity* activityForPrompting,
            /* [in] */ IBundle* addAccountOptions,
            /* [in] */ IBundle* loginOptions,
            /* [in] */ IAccountManagerCallback* cb,
            /* [in] */ IApartment* handler,
            /* [in] */ CAccountManager* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Run(
            /* [in] */ IAccountManagerFuture* future);

        CARAPI DoWork();

    public:
        AutoPtr<IAccountManagerFuture> mFuture;
        String mAccountType;
        String mAuthTokenType;
        ArrayOf<String>* mFeatures;
        AutoPtr<IBundle> mAddAccountOptions;
        AutoPtr<IBundle> mLoginOptions;
        AutoPtr<IAccountManagerCallback> mMyCallback;

    private:
        Int32 mNumAccounts;//todo: if the value is equal to 2, this mean the numberf of Accounts is above 2;
    };

    class CallbackAction : public Runnable
    {
    public:
        CallbackAction(
            /* [in] */ IAccountManagerCallback* accountManagerCb,
            /* [in] */ IAccountManagerFuture* future);

        CARAPI Run();

    private:
        AutoPtr<IAccountManagerCallback> mCallback;
        AutoPtr<IAccountManagerFuture> mFuture;
    };

    class AccountUpdateAction : public Runnable
    {
    public:
        AccountUpdateAction(
            /* [in] */ IOnAccountsUpdateListener* listener,
            /* [in] */ IObjectContainer* accounts);

        CARAPI Run();

    private:
        AutoPtr<IOnAccountsUpdateListener> mListener;
        AutoPtr<IObjectContainer> mAccounts;
    };

    /**
     * BroadcastReceiver that listens for the LOGIN_ACCOUNTS_CHANGED_ACTION intent
     * so that it can read the updated list of accounts and send them to the listener
     * in mAccountsUpdatedListeners.
     */
    // class AccountsChangedBroadcastReceiver
    //     : public ElRefBase
    //     , public IBroadcastReceiver
    // {
    // public:
    //     CARAPI_(PInterface) Probe(
    //         /* [in]  */ REIID riid);

    //     CARAPI_(UInt32) AddRef();

    //     CARAPI_(UInt32) Release();

    //     CARAPI GetInterfaceID(
    //         /* [in] */ IInterface *pObject,
    //         /* [out] */ InterfaceID *pIID);
    // };

public:
    CAccountManager();

    /**
     * @hide for internal use only
     */
    static CARAPI SanitizeResult(
        /* [in] */ IBundle* result,
        /* [out] */ IBundle** newResult);

    /**
     * Gets an AccountManager instance associated with a Context.
     * The {@link Context} will be used as long as the AccountManager is
     * active, so make sure to use a {@link Context} whose lifetime is
     * commensurate with any listeners registered to
     * {@link #addOnAccountsUpdatedListener} or similar methods.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>No permission is required to call this method.
     *
     * @param context The {@link Context} to use when necessary
     * @return An {@link AccountManager} instance
     */
    static CARAPI Get(
        /* [in] */ IContext* context,
        /* [out] */ ILocalAccountManager** accountManager);

    /**
     * Gets the saved password associated with the account.
     * This is intended for authenticators and related code; applications
     * should get an auth token instead.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#AUTHENTICATE_ACCOUNTS}
     * and to have the same UID as the account's authenticator.
     *
     * @param account The account to query for a password
     * @return The account's password, null if none or if the account doesn't exist
     */
    CARAPI GetPassword(
        /* [in] */ IAccount* account,
        /* [out] */ String* password);

    /**
     * Gets the user data named by "key" associated with the account.
     * This is intended for authenticators and related code to store
     * arbitrary metadata along with accounts.  The meaning of the keys
     * and values is up to the authenticator for the account.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#AUTHENTICATE_ACCOUNTS}
     * and to have the same UID as the account's authenticator.
     *
     * @param account The account to query for user data
     * @return The user data, null if the account or key doesn't exist
     */
    CARAPI GetUserData(
        /* [in] */ IAccount* account,
        /* [in] */ const String& key,
        /* [out] */ String* userData);

    /**
     * Lists the currently registered authenticators.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>No permission is required to call this method.
     *
     * @return An array of {@link AuthenticatorDescription} for every
     *     authenticator known to the AccountManager service.  Empty (never
     *     null) if no authenticators are known.
     */
    CARAPI GetAuthenticatorTypes(
        /* [out] */ IObjectContainer** authenticators);

    /**
     * Lists all accounts of any type registered on the device.
     * Equivalent to getAccountsByType(null).
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#GET_ACCOUNTS}.
     *
     * @return An array of {@link Account}, one for each account.  Empty
     *     (never null) if no accounts have been added.
     */
    CARAPI GetAccounts(
        /* [out] */ IObjectContainer** accounts);

    /**
     * Lists all accounts of a particular type.  The account type is a
     * string token corresponding to the authenticator and useful domain
     * of the account.  For example, there are types corresponding to Google
     * and Facebook.  The exact string token to use will be published somewhere
     * associated with the authenticator in question.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#GET_ACCOUNTS}.
     *
     * @param type The type of accounts to return, null to retrieve all accounts
     * @return An array of {@link Account}, one per matching account.  Empty
     *     (never null) if no accounts of the specified type have been added.
     */
    CARAPI GetAccountsByType(
        /* [in] */ const String& type,
        /* [out] */ IObjectContainer** accounts);

    /**
     * Finds out whether a particular account has all the specified features.
     * Account features are authenticator-specific string tokens identifying
     * boolean account properties.  For example, features are used to tell
     * whether Google accounts have a particular service (such as Google
     * Calendar or Google Talk) enabled.  The feature names and their meanings
     * are published somewhere associated with the authenticator in question.
     *
     * <p>This method may be called from any thread, but the returned
     * {@link AccountManagerFuture} must not be used on the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#GET_ACCOUNTS}.
     *
     * @param account The {@link Account} to test
     * @param features An array of the account features to check
     * @param callback Callback to invoke when the request completes,
     *     null for no callback
     * @param handler {@link Handler} identifying the callback thread,
     *     null for the main thread
     * @return An {@link AccountManagerFuture} which resolves to a Boolean,
     * true if the account exists and has all of the specified features.
     */
    CARAPI HasFeatures(
        /* [in] */ IAccount* account,
        /* [in] */ const ArrayOf<String>& features,
        /* [in] */ IAccountManagerCallback* accountManagerCb,
        /* [in] */ IApartment* handler,
        /* [out] */ IAccountManagerFuture** accountManagerFuture);

    /**
     * Lists all accounts of a type which have certain features.  The account
     * type identifies the authenticator (see {@link #getAccountsByType}).
     * Account features are authenticator-specific string tokens identifying
     * boolean account properties (see {@link #hasFeatures}).
     *
     * <p>Unlike {@link #getAccountsByType}, this method calls the authenticator,
     * which may contact the server or do other work to check account features,
     * so the method returns an {@link AccountManagerFuture}.
     *
     * <p>This method may be called from any thread, but the returned
     * {@link AccountManagerFuture} must not be used on the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#GET_ACCOUNTS}.
     *
     * @param type The type of accounts to return, must not be null
     * @param features An array of the account features to require,
     *     may be null or empty
     * @param callback Callback to invoke when the request completes,
     *     null for no callback
     * @param handler {@link Handler} identifying the callback thread,
     *     null for the main thread
     * @return An {@link AccountManagerFuture} which resolves to an array of
     *     {@link Account}, one per account of the specified type which
     *     matches the requested features.
     */
    CARAPI GetAccountsByTypeAndFeatures(
        /* [in] */ const String& type,
        /* [in] */ const ArrayOf<String>& features,
        /* [in] */ IAccountManagerCallback* accountManagerCb,
        /* [in] */ IApartment* handler,
        /* [out] */ IAccountManagerFuture** accountManagerFuture);

    /**
     * Adds an account directly to the AccountManager.  Normally used by sign-up
     * wizards associated with authenticators, not directly by applications.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#AUTHENTICATE_ACCOUNTS}
     * and to have the same UID as the added account's authenticator.
     *
     * @param account The {@link Account} to add
     * @param password The password to associate with the account, null for none
     * @param userdata String values to use for the account's userdata, null for none
     * @return True if the account was successfully added, false if the account
     *     already exists, the account is null, or another error occurs.
     */
    CARAPI AddAccountExplicitly(
        /* [in] */ IAccount* account,
        /* [in] */ const String& password,
        /* [in] */ IBundle* userdata,
        /* [out] */ Boolean* result);

    /**
     * Removes an account from the AccountManager.  Does nothing if the account
     * does not exist.  Does not delete the account from the server.
     * The authenticator may have its own policies preventing account
     * deletion, in which case the account will not be deleted.
     *
     * <p>This method may be called from any thread, but the returned
     * {@link AccountManagerFuture} must not be used on the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#MANAGE_ACCOUNTS}.
     *
     * @param account The {@link Account} to remove
     * @param callback Callback to invoke when the request completes,
     *     null for no callback
     * @param handler {@link Handler} identifying the callback thread,
     *     null for the main thread
     * @return An {@link AccountManagerFuture} which resolves to a Boolean,
     *     true if the account has been successfully removed,
     *     false if the authenticator forbids deleting this account.
     */
    CARAPI RemoveAccount(
        /* [in] */ IAccount* account,
        /* [in] */ IAccountManagerCallback* accountManagerCb,
        /* [in] */ IApartment* handler,
        /* [out] */ IAccountManagerFuture** accountManagerFuture);

    /**
     * Removes an auth token from the AccountManager's cache.  Does nothing if
     * the auth token is not currently in the cache.  Applications must call this
     * method when the auth token is found to have expired or otherwise become
     * invalid for authenticating requests.  The AccountManager does not validate
     * or expire cached auth tokens otherwise.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#MANAGE_ACCOUNTS} or
     * {@link android.Manifest.permission#USE_CREDENTIALS}
     *
     * @param accountType The account type of the auth token to invalidate, must not be null
     * @param authToken The auth token to invalidate, may be null
     */
    CARAPI InvalidateAuthToken(
        /* [in] */ const String& accountType,
        /* [in] */ const String& authToken);

    /**
     * Gets an auth token from the AccountManager's cache.  If no auth
     * token is cached for this account, null will be returned -- a new
     * auth token will not be generated, and the server will not be contacted.
     * Intended for use by the authenticator, not directly by applications.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#AUTHENTICATE_ACCOUNTS}
     * and to have the same UID as the account's authenticator.
     *
     * @param account The account to fetch an auth token for
     * @param authTokenType The type of auth token to fetch, see {#getAuthToken}
     * @return The cached auth token for this account and type, or null if
     *     no auth token is cached or the account does not exist.
     */
    CARAPI PeekAuthToken(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authTokenType,
        /* [out] */ String* token);

    /**
     * Sets or forgets a saved password.  This modifies the local copy of the
     * password used to automatically authenticate the user; it does
     * not change the user's account password on the server.  Intended for use
     * by the authenticator, not directly by applications.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#AUTHENTICATE_ACCOUNTS}
     * and have the same UID as the account's authenticator.
     *
     * @param account The account to set a password for
     * @param password The password to set, null to clear the password
     */
    CARAPI SetPassword(
        /* [in] */ IAccount* account,
        /* [in] */ const String& password);

    /**
     * Forgets a saved password.  This erases the local copy of the password;
     * it does not change the user's account password on the server.
     * Has the same effect as setPassword(account, null) but requires fewer
     * permissions, and may be used by applications or management interfaces
     * to "sign out" from an account.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#MANAGE_ACCOUNTS}
     *
     * @param account The account whose password to clear
     */
    CARAPI ClearPassword(
        /* [in] */ IAccount* account);

    /**
     * Sets one userdata key for an account.  Intended by use for the
     * authenticator to stash state for itself, not directly by applications.
     * The meaning of the keys and values is up to the authenticator.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#AUTHENTICATE_ACCOUNTS}
     * and to have the same UID as the account's authenticator.
     *
     * @param account The account to set the userdata for
     * @param key The userdata key to set.  Must not be null
     * @param value The value to set, null to clear this userdata key
     */
    CARAPI SetUserData(
        /* [in] */ IAccount* account,
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    /**
     * Adds an auth token to the AccountManager cache for an account.
     * If the account does not exist then this call has no effect.
     * Replaces any previous auth token for this account and auth token type.
     * Intended for use by the authenticator, not directly by applications.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#AUTHENTICATE_ACCOUNTS}
     * and to have the same UID as the account's authenticator.
     *
     * @param account The account to set an auth token for
     * @param authTokenType The type of the auth token, see {#getAuthToken}
     * @param authToken The auth token to add to the cache
     */
    CARAPI SetAuthToken(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authTokenType,
        /* [in] */ const String& authToken);

    /**
     * This convenience helper synchronously gets an auth token with
     * {@link #getAuthToken(Account, String, boolean, AccountManagerCallback, Handler)}.
     *
     * <p>This method may block while a network request completes, and must
     * never be made from the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#USE_CREDENTIALS}.
     *
     * @param account The account to fetch an auth token for
     * @param authTokenType The auth token type, see {#link getAuthToken}
     * @param notifyAuthFailure If true, display a notification and return null
     *     if authentication fails; if false, prompt and wait for the user to
     *     re-enter correct credentials before returning
     * @return An auth token of the specified type for this account, or null
     *     if authentication fails or none can be fetched.
     * @throws AuthenticatorException if the authenticator failed to respond
     * @throws OperationCanceledException if the request was canceled for any
     *     reason, including the user canceling a credential request
     * @throws java.io.IOException if the authenticator experienced an I/O problem
     *     creating a new auth token, usually because of network trouble
     */
    CARAPI BlockingGetAuthToken(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authTokenType,
        /* [in] */ Boolean notifyAuthFailure,
        /* [out] */ String* token);

    /**
     * Gets an auth token of the specified type for a particular account,
     * prompting the user for credentials if necessary.  This method is
     * intended for applications running in the foreground where it makes
     * sense to ask the user directly for a password.
     *
     * <p>If a previously generated auth token is cached for this account and
     * type, then it is returned.  Otherwise, if a saved password is
     * available, it is sent to the server to generate a new auth token.
     * Otherwise, the user is prompted to enter a password.
     *
     * <p>Some authenticators have auth token <em>types</em>, whose value
     * is authenticator-dependent.  Some services use different token types to
     * access different functionality -- for example, Google uses different auth
     * tokens to access Gmail and Google Calendar for the same account.
     *
     * <p>This method may be called from any thread, but the returned
     * {@link AccountManagerFuture} must not be used on the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#USE_CREDENTIALS}.
     *
     * @param account The account to fetch an auth token for
     * @param authTokenType The auth token type, an authenticator-dependent
     *     string token, must not be null
     * @param options Authenticator-specific options for the request,
     *     may be null or empty
     * @param activity The {@link Activity} context to use for launching a new
     *     authenticator-defined sub-Activity to prompt the user for a password
     *     if necessary; used only to call startActivity(); must not be null.
     * @param callback Callback to invoke when the request completes,
     *     null for no callback
     * @param handler {@link Handler} identifying the callback thread,
     *     null for the main thread
     * @return An {@link AccountManagerFuture} which resolves to a Bundle with
     *     at least the following fields:
     * <ul>
     * <li> {@link #KEY_ACCOUNT_NAME} - the name of the account you supplied
     * <li> {@link #KEY_ACCOUNT_TYPE} - the type of the account
     * <li> {@link #KEY_AUTHTOKEN} - the auth token you wanted
     * </ul>
     *
     * (Other authenticator-specific values may be returned.)  If an auth token
     * could not be fetched, {@link AccountManagerFuture#getResult()} throws:
     * <ul>
     * <li> {@link AuthenticatorException} if the authenticator failed to respond
     * <li> {@link OperationCanceledException} if the operation is canceled for
     *      any reason, incluidng the user canceling a credential request
     * <li> {@link IOException} if the authenticator experienced an I/O problem
     *      creating a new auth token, usually because of network trouble
     * </ul>
     * If the account is no longer present on the device, the return value is
     * authenticator-dependent.  The caller should verify the validity of the
     * account before requesting an auth token.
     */
    CARAPI GetAuthToken(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authTokenType,
        /* [in] */ IBundle* options,
        /* [in] */ IActivity* activity,
        /* [in] */ IAccountManagerCallback* accountManagerCb,
        /* [in] */ IApartment* handler,
        /* [out] */ IAccountManagerFuture** accountManagerFuture);

    /**
     * Gets an auth token of the specified type for a particular account,
     * optionally raising a notification if the user must enter credentials.
     * This method is intended for background tasks and services where the
     * user should not be immediately interrupted with a password prompt.
     *
     * <p>If a previously generated auth token is cached for this account and
     * type, then it is returned.  Otherwise, if a saved password is
     * available, it is sent to the server to generate a new auth token.
     * Otherwise, an {@link Intent} is returned which, when started, will
     * prompt the user for a password.  If the notifyAuthFailure parameter is
     * set, a status bar notification is also created with the same Intent,
     * alerting the user that they need to enter a password at some point.
     *
     * <p>In that case, you may need to wait until the user responds, which
     * could take hours or days or forever.  When the user does respond and
     * supply a new password, the account manager will broadcast the
     * {@link #LOGIN_ACCOUNTS_CHANGED_ACTION} Intent, which applications can
     * use to try again.
     *
     * <p>If notifyAuthFailure is not set, it is the application's
     * responsibility to launch the returned Intent at some point.
     * Either way, the result from this call will not wait for user action.
     *
     * <p>Some authenticators have auth token <em>types</em>, whose value
     * is authenticator-dependent.  Some services use different token types to
     * access different functionality -- for example, Google uses different auth
     * tokens to access Gmail and Google Calendar for the same account.
     *
     * <p>This method may be called from any thread, but the returned
     * {@link AccountManagerFuture} must not be used on the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#USE_CREDENTIALS}.
     *
     * @param account The account to fetch an auth token for
     * @param authTokenType The auth token type, an authenticator-dependent
     *     string token, must not be null
     * @param notifyAuthFailure True to add a notification to prompt the
     *     user for a password if necessary, false to leave that to the caller
     * @param callback Callback to invoke when the request completes,
     *     null for no callback
     * @param handler {@link Handler} identifying the callback thread,
     *     null for the main thread
     * @return An {@link AccountManagerFuture} which resolves to a Bundle with
     *     at least the following fields on success:
     * <ul>
     * <li> {@link #KEY_ACCOUNT_NAME} - the name of the account you supplied
     * <li> {@link #KEY_ACCOUNT_TYPE} - the type of the account
     * <li> {@link #KEY_AUTHTOKEN} - the auth token you wanted
     * </ul>
     *
     * (Other authenticator-specific values may be returned.)  If the user
     * must enter credentials, the returned Bundle contains only
     * {@link #KEY_INTENT} with the {@link Intent} needed to launch a prompt.
     *
     * If an error occurred, {@link AccountManagerFuture#getResult()} throws:
     * <ul>
     * <li> {@link AuthenticatorException} if the authenticator failed to respond
     * <li> {@link OperationCanceledException} if the operation is canceled for
     *      any reason, incluidng the user canceling a credential request
     * <li> {@link IOException} if the authenticator experienced an I/O problem
     *      creating a new auth token, usually because of network trouble
     * </ul>
     * If the account is no longer present on the device, the return value is
     * authenticator-dependent.  The caller should verify the validity of the
     * account before requesting an auth token.
     */
    CARAPI GetAuthTokenEx(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authTokenType,
        /* [in] */ Boolean notifyAuthFailure,
        /* [in] */ IAccountManagerCallback* accountManagerCb,
        /* [in] */ IApartment* handler,
        /* [out] */ IAccountManagerFuture** accountManagerFuture);

    /**
     * Asks the user to add an account of a specified type.  The authenticator
     * for this account type processes this request with the appropriate user
     * interface.  If the user does elect to create a new account, the account
     * name is returned.
     *
     * <p>This method may be called from any thread, but the returned
     * {@link AccountManagerFuture} must not be used on the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#MANAGE_ACCOUNTS}.
     *
     * @param accountType The type of account to add; must not be null
     * @param authTokenType The type of auth token (see {@link #getAuthToken})
     *     this account will need to be able to generate, null for none
     * @param requiredFeatures The features (see {@link #hasFeatures}) this
     *     account must have, null for none
     * @param addAccountOptions Authenticator-specific options for the request,
     *     may be null or empty
     * @param activity The {@link Activity} context to use for launching a new
     *     authenticator-defined sub-Activity to prompt the user to create an
     *     account; used only to call startActivity(); if null, the prompt
     *     will not be launched directly, but the necessary {@link Intent}
     *     will be returned to the caller instead
     * @param callback Callback to invoke when the request completes,
     *     null for no callback
     * @param handler {@link Handler} identifying the callback thread,
     *     null for the main thread
     * @return An {@link AccountManagerFuture} which resolves to a Bundle with
     *     these fields if activity was specified and an account was created:
     * <ul>
     * <li> {@link #KEY_ACCOUNT_NAME} - the name of the account created
     * <li> {@link #KEY_ACCOUNT_TYPE} - the type of the account
     * </ul>
     *
     * If no activity was specified, the returned Bundle contains only
     * {@link #KEY_INTENT} with the {@link Intent} needed to launch the
     * actual account creation process.  If an error occurred,
     * {@link AccountManagerFuture#getResult()} throws:
     * <ul>
     * <li> {@link AuthenticatorException} if no authenticator was registered for
     *      this account type or the authenticator failed to respond
     * <li> {@link OperationCanceledException} if the operation was canceled for
     *      any reason, including the user canceling the creation process
     * <li> {@link IOException} if the authenticator experienced an I/O problem
     *      creating a new account, usually because of network trouble
     * </ul>
     */
    CARAPI AddAccount(
        /* [in] */ const String& accountType,
        /* [in] */ const String& authTokenType,
        /* [in] */ const ArrayOf<String>& requiredFeatures,
        /* [in] */ IBundle* addAccountOptions,
        /* [in] */ IActivity* activity,
        /* [in] */ IAccountManagerCallback* accountManagerCb,
        /* [in] */ IApartment* handler,
        /* [out] */ IAccountManagerFuture** accountManagerFuture);

    /**
     * Confirms that the user knows the password for an account to make extra
     * sure they are the owner of the account.  The user-entered password can
     * be supplied directly, otherwise the authenticator for this account type
     * prompts the user with the appropriate interface.  This method is
     * intended for applications which want extra assurance; for example, the
     * phone lock screen uses this to let the user unlock the phone with an
     * account password if they forget the lock pattern.
     *
     * <p>If the user-entered password matches a saved password for this
     * account, the request is considered valid; otherwise the authenticator
     * verifies the password (usually by contacting the server).
     *
     * <p>This method may be called from any thread, but the returned
     * {@link AccountManagerFuture} must not be used on the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#MANAGE_ACCOUNTS}.
     *
     * @param account The account to confirm password knowledge for
     * @param options Authenticator-specific options for the request;
     *     if the {@link #KEY_PASSWORD} string field is present, the
     *     authenticator may use it directly rather than prompting the user;
     *     may be null or empty
     * @param activity The {@link Activity} context to use for launching a new
     *     authenticator-defined sub-Activity to prompt the user to enter a
     *     password; used only to call startActivity(); if null, the prompt
     *     will not be launched directly, but the necessary {@link Intent}
     *     will be returned to the caller instead
     * @param callback Callback to invoke when the request completes,
     *     null for no callback
     * @param handler {@link Handler} identifying the callback thread,
     *     null for the main thread
     * @return An {@link AccountManagerFuture} which resolves to a Bundle
     *     with these fields if activity or password was supplied and
     *     the account was successfully verified:
     * <ul>
     * <li> {@link #KEY_ACCOUNT_NAME} - the name of the account created
     * <li> {@link #KEY_ACCOUNT_TYPE} - the type of the account
     * <li> {@link #KEY_BOOLEAN_RESULT} - true to indicate success
     * </ul>
     *
     * If no activity or password was specified, the returned Bundle contains
     * only {@link #KEY_INTENT} with the {@link Intent} needed to launch the
     * password prompt.  If an error occurred,
     * {@link AccountManagerFuture#getResult()} throws:
     * <ul>
     * <li> {@link AuthenticatorException} if the authenticator failed to respond
     * <li> {@link OperationCanceledException} if the operation was canceled for
     *      any reason, including the user canceling the password prompt
     * <li> {@link IOException} if the authenticator experienced an I/O problem
     *      verifying the password, usually because of network trouble
     * </ul>
     */
    CARAPI ConfirmCredentials(
        /* [in] */ IAccount* account,
        /* [in] */ IBundle* options,
        /* [in] */ IActivity* activity,
        /* [in] */ IAccountManagerCallback* accountManagerCb,
        /* [in] */ IApartment* handler,
        /* [out] */ IAccountManagerFuture** accountManagerFuture);

    /**
     * Asks the user to enter a new password for an account, updating the
     * saved credentials for the account.  Normally this happens automatically
     * when the server rejects credentials during an auth token fetch, but this
     * can be invoked directly to ensure we have the correct credentials stored.
     *
     * <p>This method may be called from any thread, but the returned
     * {@link AccountManagerFuture} must not be used on the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#MANAGE_ACCOUNTS}.
     *
     * @param account The account to update credentials for
     * @param authTokenType The credentials entered must allow an auth token
     *     of this type to be created (but no actual auth token is returned);
     *     may be null
     * @param options Authenticator-specific options for the request;
     *     may be null or empty
     * @param activity The {@link Activity} context to use for launching a new
     *     authenticator-defined sub-Activity to prompt the user to enter a
     *     password; used only to call startActivity(); if null, the prompt
     *     will not be launched directly, but the necessary {@link Intent}
     *     will be returned to the caller instead
     * @param callback Callback to invoke when the request completes,
     *     null for no callback
     * @param handler {@link Handler} identifying the callback thread,
     *     null for the main thread
     * @return An {@link AccountManagerFuture} which resolves to a Bundle
     *     with these fields if an activity was supplied and the account
     *     credentials were successfully updated:
     * <ul>
     * <li> {@link #KEY_ACCOUNT_NAME} - the name of the account created
     * <li> {@link #KEY_ACCOUNT_TYPE} - the type of the account
     * </ul>
     *
     * If no activity was specified, the returned Bundle contains only
     * {@link #KEY_INTENT} with the {@link Intent} needed to launch the
     * password prompt.  If an error occurred,
     * {@link AccountManagerFuture#getResult()} throws:
     * <ul>
     * <li> {@link AuthenticatorException} if the authenticator failed to respond
     * <li> {@link OperationCanceledException} if the operation was canceled for
     *      any reason, including the user canceling the password prompt
     * <li> {@link IOException} if the authenticator experienced an I/O problem
     *      verifying the password, usually because of network trouble
     * </ul>
     */
    CARAPI UpdateCredentials(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authTokenType,
        /* [in] */ IBundle* options,
        /* [in] */ IActivity* activity,
        /* [in] */ IAccountManagerCallback* accountManagerCb,
        /* [in] */ IApartment* handler,
        /* [out] */ IAccountManagerFuture** accountManagerFuture);

    /**
     * Offers the user an opportunity to change an authenticator's settings.
     * These properties are for the authenticator in general, not a particular
     * account.  Not all authenticators support this method.
     *
     * <p>This method may be called from any thread, but the returned
     * {@link AccountManagerFuture} must not be used on the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#MANAGE_ACCOUNTS}.
     *
     * @param accountType The account type associated with the authenticator
     *     to adjust
     * @param activity The {@link Activity} context to use for launching a new
     *     authenticator-defined sub-Activity to adjust authenticator settings;
     *     used only to call startActivity(); if null, the settings dialog will
     *     not be launched directly, but the necessary {@link Intent} will be
     *     returned to the caller instead
     * @param callback Callback to invoke when the request completes,
     *     null for no callback
     * @param handler {@link Handler} identifying the callback thread,
     *     null for the main thread
     * @return An {@link AccountManagerFuture} which resolves to a Bundle
     *     which is empty if properties were edited successfully, or
     *     if no activity was specified, contains only {@link #KEY_INTENT}
     *     needed to launch the authenticator's settings dialog.
     *     If an error occurred, {@link AccountManagerFuture#getResult()}
     *     throws:
     * <ul>
     * <li> {@link AuthenticatorException} if no authenticator was registered for
     *      this account type or the authenticator failed to respond
     * <li> {@link OperationCanceledException} if the operation was canceled for
     *      any reason, including the user canceling the settings dialog
     * <li> {@link IOException} if the authenticator experienced an I/O problem
     *      updating settings, usually because of network trouble
     * </ul>
     */
    CARAPI EditProperties(
        /* [in] */ const String& accountType,
        /* [in] */ IActivity* activity,
        /* [in] */ IAccountManagerCallback* accountManagerCb,
        /* [in] */ IApartment* handler,
        /* [out] */ IAccountManagerFuture** accountManagerFuture);

    /**
     * This convenience helper combines the functionality of
     * {@link #getAccountsByTypeAndFeatures}, {@link #getAuthToken}, and
     * {@link #addAccount}.
     *
     * <p>This method gets a list of the accounts matching the
     * specified type and feature set; if there is exactly one, it is
     * used; if there are more than one, the user is prompted to pick one;
     * if there are none, the user is prompted to add one.  Finally,
     * an auth token is acquired for the chosen account.
     *
     * <p>This method may be called from any thread, but the returned
     * {@link AccountManagerFuture} must not be used on the main thread.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#MANAGE_ACCOUNTS}.
     *
     * @param accountType The account type required
     *     (see {@link #getAccountsByType}), must not be null
     * @param authTokenType The desired auth token type
     *     (see {@link #getAuthToken}), must not be null
     * @param features Required features for the account
     *     (see {@link #getAccountsByTypeAndFeatures}), may be null or empty
     * @param activity The {@link Activity} context to use for launching new
     *     sub-Activities to prompt to add an account, select an account,
     *     and/or enter a password, as necessary; used only to call
     *     startActivity(); should not be null
     * @param addAccountOptions Authenticator-specific options to use for
     *     adding new accounts; may be null or empty
     * @param getAuthTokenOptions Authenticator-specific options to use for
     *     CARAPI Getting auth tokens; may be null or empty
     * @param callback Callback to invoke when the request completes,
     *     null for no callback
     * @param handler {@link Handler} identifying the callback thread,
     *     null for the main thread
     * @return An {@link AccountManagerFuture} which resolves to a Bundle with
     *     at least the following fields:
     * <ul>
     * <li> {@link #KEY_ACCOUNT_NAME} - the name of the account
     * <li> {@link #KEY_ACCOUNT_TYPE} - the type of the account
     * <li> {@link #KEY_AUTHTOKEN} - the auth token you wanted
     * </ul>
     *
     * If an error occurred, {@link AccountManagerFuture#getResult()} throws:
     * <ul>
     * <li> {@link AuthenticatorException} if no authenticator was registered for
     *      this account type or the authenticator failed to respond
     * <li> {@link OperationCanceledException} if the operation was canceled for
     *      any reason, including the user canceling any operation
     * <li> {@link IOException} if the authenticator experienced an I/O problem
     *      updating settings, usually because of network trouble
     * </ul>
     */
    CARAPI GetAuthTokenByFeatures(
        /* [in] */ const String& accountType,
        /* [in] */ const String& authTokenType,
        /* [in] */ const ArrayOf<String>& features,
        /* [in] */ IActivity* activity,
        /* [in] */ IBundle* addAccountOptions,
        /* [in] */ IBundle* getAuthTokenOptions,
        /* [in] */ IAccountManagerCallback* accountManagerCb,
        /* [in] */ IApartment* handler,
        /* [out] */ IAccountManagerFuture** accountManagerFuture);

    /**
     * Adds an {@link OnAccountsUpdateListener} to this instance of the
     * {@link AccountManager}.  This listener will be notified whenever the
     * list of accounts on the device changes.
     *
     * <p>As long as this listener is present, the AccountManager instance
     * will not be garbage-collected, and neither will the {@link Context}
     * used to retrieve it, which may be a large Activity instance.  To avoid
     * memory leaks, you must remove this listener before then.  Normally
     * listeners are added in an Activity or Service's {@link Activity#onCreate}
     * and removed in {@link Activity#onDestroy}.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>No permission is required to call this method.
     *
     * @param listener The listener to send notifications to
     * @param handler {@link Handler} identifying the thread to use
     *     for notifications, null for the main thread
     * @param updateImmediately If true, the listener will be invoked
     *     (on the handler thread) right away with the current account list
     * @throws IllegalArgumentException if listener is null
     * @throws IllegalStateException if listener was already added
     */
    CARAPI AddOnAccountsUpdatedListener(
        /* [in] */ IOnAccountsUpdateListener* listener,
        /* [in] */ IApartment* handler,
        /* [in] */ Boolean updateImmediately);

    /**
     * Removes an {@link OnAccountsUpdateListener} previously registered with
     * {@link #addOnAccountsUpdatedListener}.  The listener will no longer
     * receive notifications of account changes.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>No permission is required to call this method.
     *
     * @param listener The previously added listener to remove
     * @throws IllegalArgumentException if listener is null
     * @throws IllegalStateException if listener was not already added
     */
    CARAPI RemoveOnAccountsUpdatedListener(
        /* [in] */ IOnAccountsUpdateListener* listener);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAccountManager* service);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAccountManager* service,
        /* [in] */ IApartment* handler);

private:
    CARAPI EnsureNotOnMainThread();

    CARAPI_(void) PostToHandler(
        /* [in] */ IApartment* handler,
        /* [in] */ IAccountManagerCallback* accountManagerCb,
        /* [in] */ IAccountManagerFuture* future);

    CARAPI_(void) PostToHandler(
        /* [in] */ IApartment* handler,
        /* [in] */ IOnAccountsUpdateListener* listener,
        /* [in] */ IObjectContainer* accounts);

    CARAPI ConvertErrorToException(
        /* [in] */ Int32 code
        /*String message*/);

private:
    static const CString TAG;

private:
    AutoPtr<IContext> mContext;
    AutoPtr<IAccountManager> mService;
    AutoPtr<IApartment> mMainHandler;
    AutoPtr<IRunnable> mCallbackAction;

    HashMap<AutoPtr<IOnAccountsUpdateListener>, AutoPtr<IApartment> >
            mAccountsUpdatedListeners;
    Mutex mAccountsUpdatedListenersLock;

    //AccountsChangedBroadcastReceiver* mAccountsChangedBroadcastReceiver;
};

#endif // __CACCOUNTMANAGER_H__
