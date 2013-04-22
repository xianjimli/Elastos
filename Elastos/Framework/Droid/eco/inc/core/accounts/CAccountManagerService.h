
#ifndef __CACCOUNTMANAGERSERVICE_H__
#define __CACCOUNTMANAGERSERVICE_H__

#include "_CAccountManagerService.h"
#include "accounts/AccountAuthenticatorCache.h"
#include "os/HandlerThread.h"
//#include "database/sqlite/SQLiteOpenHelper.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IAccount> >
{
    size_t operator()(AutoPtr<IAccount> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

_ELASTOS_NAMESPACE_END

using namespace Elastos::Core::Threading;

CarClass(CAccountManagerService)
{
public:
    class AuthTokenKey {
    public:
        AuthTokenKey(
            /* [in] */ IAccount* account,
            /* [in] */ const String& authTokenType);

        CARAPI_(Boolean) Equals(
            /* [in] */ AuthTokenKey* authTokenKey);

        CARAPI_(Int32) HashCode();

    private:
        CARAPI_(Int32) ComputeHashCode();

    public:
        AutoPtr<IAccount> mAccount;
        String mAuthTokenType;

    private:
        Int32 mHashCode;
    };

private:
    class DatabaseHelper
            : public ISQLiteOpenHelper
            , public ElRefBase
            //, public SQLiteOpenHelper
    {
    public:
        DatabaseHelper(
            /* [in] */ IContext* context);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetWritableDatabase(
            /* [out] */ ISQLiteDatabase **database);

        CARAPI GetReadableDatabase(
            /* [out] */ ISQLiteDatabase **database);

        CARAPI Close();

        CARAPI OnCreate(
            /* [in] */ ISQLiteDatabase *database);

        CARAPI OnUpgrade(
            /* [in] */ ISQLiteDatabase *db,
            /* [in] */ Int32 oldVersion,
            /* [in] */ Int32 newVersion);

        CARAPI OnOpen(
            /* [in] */ ISQLiteDatabase *db);

    private:
        CARAPI_(void) CreateAccountsDeletionTrigger(
            /* [in] */ ISQLiteDatabase* db);

        CARAPI_(void) CreateGrantsTable(
            /* [in] */ ISQLiteDatabase* db);
    };

public:
    /**
     * This should only be called by system code. One should only call this after the service
     * has started.
     * @return a reference to the AccountManagerService instance
     * @hide
     */
    static CARAPI GetSingleton(
        /* [out] */ IAccountManager** service);

    CARAPI GetPassword(
        /* [in] */ IAccount* account,
        /* [out] */ String* password);

    CARAPI GetUserData(
        /* [in] */ IAccount* account,
        /* [in] */ const String& key,
        /* [out] */ String* userData);

    CARAPI GetAuthenticatorTypes(
        /* [out] */ IObjectContainer** types);

    CARAPI GetAccounts(
        /* [in] */ const String& accountType,
        /* [out] */ IObjectContainer** accounts);

    CARAPI HasFeatures(
        /* [in] */ IAccountManagerResponse* response,
        /* [in] */ IAccount* account,
        /* [in] */ const ArrayOf<String>& features);

    CARAPI GetAccountsByFeatures(
        /* [in] */ IAccountManagerResponse* response,
        /* [in] */ const String& accountType,
        /* [in] */ const ArrayOf<String>& features);

    CARAPI AddAccount(
        /* [in] */ IAccount* account,
        /* [in] */ const String& password,
        /* [in] */ IBundle* extras,
        /* [out] */ Boolean* result);

    CARAPI RemoveAccount(
        /* [in] */ IAccountManagerResponse* response,
        /* [in] */ IAccount* account);

    CARAPI InvalidateAuthToken(
        /* [in] */ const String& accountType,
        /* [in] */ const String& authToken);

    CARAPI PeekAuthToken(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authTokenType,
        /* [out] */ String* token);

    CARAPI SetAuthToken(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authTokenType,
        /* [in] */ const String& authToken);

    CARAPI SetPassword(
        /* [in] */ IAccount* account,
        /* [in] */ const String& password);

    CARAPI ClearPassword(
        /* [in] */ IAccount* account);

    CARAPI SetUserData(
        /* [in] */ IAccount* account,
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    CARAPI GetAuthToken(
        /* [in] */ IAccountManagerResponse* response,
        /* [in] */ IAccount* account,
        /* [in] */ const String& authTokenType,
        /* [in] */ Boolean notifyOnAuthFailure,
        /* [in] */ Boolean expectActivityLaunch,
        /* [in] */ IBundle* options);

    CARAPI AddAcount(
        /* [in] */ IAccountManagerResponse* response,
        /* [in] */ const String& accountType,
        /* [in] */ const String& authTokenType,
        /* [in] */ const ArrayOf<String>& requiredFeatures,
        /* [in] */ Boolean expectActivityLaunch,
        /* [in] */ IBundle* options);

    CARAPI UpdateCredentials(
        /* [in] */ IAccountManagerResponse* response,
        /* [in] */ IAccount* account,
        /* [in] */ const String& authTokenType,
        /* [in] */ Boolean expectActivityLaunch,
        /* [in] */ IBundle* options);

    CARAPI EditProperties(
        /* [in] */ IAccountManagerResponse* response,
        /* [in] */ const String& accountType,
        /* [in] */ Boolean expectActivityLaunch);

    CARAPI ConfirmCredentials(
        /* [in] */ IAccountManagerResponse* response,
        /* [in] */ IAccount* account,
        /* [in] */ IBundle* options,
        /* [in] */ Boolean expectActivityLaunch);

    CARAPI constructor(
        /* [in] */ IContext* context);

private:
    static CARAPI_(AutoPtr<IIntent>) InitAccountsChangedIntent();

    static CARAPI_(String) GetDatabaseName();

private:
    static const CString GOOGLE_ACCOUNT_TYPE;

    static const CString NO_BROADCAST_FLAG;

    static const CString TAG;

    static const Int32 TIMEOUT_DELAY_MS;
    static const CString DATABASE_NAME;
    static const Int32 DATABASE_VERSION;

    // Messages that can be sent on mHandler
    static const Int32 MESSAGE_TIMED_OUT;

    static const CString TABLE_ACCOUNTS;
    static const CString ACCOUNTS_ID;
    static const CString ACCOUNTS_NAME;
    static const CString ACCOUNTS_TYPE;
    static const CString ACCOUNTS_TYPE_COUNT;
    static const CString ACCOUNTS_PASSWORD;

    static const CString TABLE_AUTHTOKENS;
    static const CString AUTHTOKENS_ID;
    static const CString AUTHTOKENS_ACCOUNTS_ID;
    static const CString AUTHTOKENS_TYPE;
    static const CString AUTHTOKENS_AUTHTOKEN;

    static const CString TABLE_GRANTS;
    static const CString GRANTS_ACCOUNTS_ID;
    static const CString GRANTS_AUTH_TOKEN_TYPE;
    static const CString GRANTS_GRANTEE_UID;

    static const CString TABLE_EXTRAS;
    static const CString EXTRAS_ID;
    static const CString EXTRAS_ACCOUNTS_ID;
    static const CString EXTRAS_KEY;
    static const CString EXTRAS_VALUE;

    static const CString TABLE_META;
    static const CString META_KEY;
    static const CString META_VALUE;

    static const CString ACCOUNT_NAME_TYPE_PROJECTION[];
    static const CString ACCOUNT_TYPE_COUNT_PROJECTION[];
    static const AutoPtr<IIntent> ACCOUNTS_CHANGED_INTENT;

    static const String COUNT_OF_MATCHING_GRANTS;

    static Boolean SDebuggableMonkeyBuild;
    static ArrayOf<AutoPtr<IAccount> >* EMPTY_ACCOUNT_ARRAY;

private:
    AutoPtr<IContext> mContext;

    AutoPtr<ICapsuleManager> mCapsuleManager;

    HandlerThread* mMessageThread;
    //MessageHandler mMessageHandler;
    AccountAuthenticatorCache* mAuthenticatorCache;
    DatabaseHelper* mOpenHelper;

    HashMap<String, AutoPtr<IAccountAuthenticatorResponse> > mSessions;
    Mutex mSessionsLock;
    //AtomicInteger mNotificationIds = new AtomicInteger(1);

    HashMap<HashMap<HashMap<AutoPtr<IAccount> , String>, Int32>, Int32>
            mCredentialsPermissionNotificationIds;
    HashMap<AutoPtr<IAccount>, Int32> mSigninRequiredNotificationIds;
    //static AtomicReference<AccountManagerService> sThis =
    //         new AtomicReference<AccountManagerService>();
    static AutoPtr<CAccountManagerService> sThis;
};

#endif //__CACCOUNTMANAGERSERVICE_H__
