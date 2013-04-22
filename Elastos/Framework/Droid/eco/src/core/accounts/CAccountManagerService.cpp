
#include "ext/frameworkdef.h"
#include "accounts/CAccountManagerService.h"
#include "os/SystemProperties.h"
#include "content/CIntent.h"

const CString CAccountManagerService::GOOGLE_ACCOUNT_TYPE = "com.google";

const CString CAccountManagerService::NO_BROADCAST_FLAG = "nobroadcast";

const CString CAccountManagerService::TAG = "AccountManagerService";

const Int32 CAccountManagerService::TIMEOUT_DELAY_MS = 1000 * 60;
const CString CAccountManagerService::DATABASE_NAME = "accounts.db";
const Int32 CAccountManagerService::DATABASE_VERSION = 4;

// Messages that can be sent on mHandler
const Int32 CAccountManagerService::MESSAGE_TIMED_OUT = 3;

const CString CAccountManagerService::TABLE_ACCOUNTS = "accounts";
const CString CAccountManagerService::ACCOUNTS_ID = "_id";
const CString CAccountManagerService::ACCOUNTS_NAME = "name";
const CString CAccountManagerService::ACCOUNTS_TYPE = "type";
const CString CAccountManagerService::ACCOUNTS_TYPE_COUNT = "count(type)";
const CString CAccountManagerService::ACCOUNTS_PASSWORD = "password";

const CString CAccountManagerService::TABLE_AUTHTOKENS = "authtokens";
const CString CAccountManagerService::AUTHTOKENS_ID = "_id";
const CString CAccountManagerService::AUTHTOKENS_ACCOUNTS_ID = "accounts_id";
const CString CAccountManagerService::AUTHTOKENS_TYPE = "type";
const CString CAccountManagerService::AUTHTOKENS_AUTHTOKEN = "authtoken";

const CString CAccountManagerService::TABLE_GRANTS = "grants";
const CString CAccountManagerService::GRANTS_ACCOUNTS_ID = "accounts_id";
const CString CAccountManagerService::GRANTS_AUTH_TOKEN_TYPE = "auth_token_type";
const CString CAccountManagerService::GRANTS_GRANTEE_UID = "uid";

const CString CAccountManagerService::TABLE_EXTRAS = "extras";
const CString CAccountManagerService::EXTRAS_ID = "_id";
const CString CAccountManagerService::EXTRAS_ACCOUNTS_ID = "accounts_id";
const CString CAccountManagerService::EXTRAS_KEY = "key";
const CString CAccountManagerService::EXTRAS_VALUE = "value";

const CString CAccountManagerService::TABLE_META = "meta";
const CString CAccountManagerService::META_KEY = "key";
const CString CAccountManagerService::META_VALUE = "value";

const CString CAccountManagerService::ACCOUNT_NAME_TYPE_PROJECTION[] =
        { ACCOUNTS_ID, ACCOUNTS_NAME, ACCOUNTS_TYPE };
const CString CAccountManagerService::ACCOUNT_TYPE_COUNT_PROJECTION[] =
        { ACCOUNTS_TYPE, ACCOUNTS_TYPE_COUNT };
const AutoPtr<IIntent> CAccountManagerService::ACCOUNTS_CHANGED_INTENT =
        InitAccountsChangedIntent();

const String CAccountManagerService::COUNT_OF_MATCHING_GRANTS = String("")
        + String("SELECT COUNT(*) FROM ") + String(TABLE_GRANTS) + String(", ")
        + String(TABLE_ACCOUNTS) + String(" WHERE ")
        + String(GRANTS_ACCOUNTS_ID) + String("=") + String(ACCOUNTS_ID)
        + String(" AND ") + String(GRANTS_GRANTEE_UID) + String("=?")
        + String(" AND ") + String(GRANTS_AUTH_TOKEN_TYPE) + String("=?")
        + String(" AND ") + String(ACCOUNTS_NAME) + String("=?")
        + String(" AND ") + String(ACCOUNTS_TYPE) + String("=?");

Boolean CAccountManagerService::SDebuggableMonkeyBuild =
        SystemProperties::GetBoolean(String("ro.monkey"), FALSE);

ArrayOf<AutoPtr<IAccount> >* CAccountManagerService::EMPTY_ACCOUNT_ARRAY =
        ArrayOf<AutoPtr<IAccount> >::Alloc(0);

AutoPtr<CAccountManagerService> CAccountManagerService::sThis;

AutoPtr<IIntent> CAccountManagerService::InitAccountsChangedIntent()
{
    AutoPtr<CIntent> i;
    ASSERT_SUCCEEDED(CIntent::NewByFriend(String(AccountManager_LOGIN_ACCOUNTS_CHANGED_ACTION),
            (CIntent**)&i));
    i->SetFlags(Intent_FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);

    return (IIntent*)i.Get();
}

CAccountManagerService::GetSingleton(
    /* [out] */ IAccountManager** service)
{
    VALIDATE_NOT_NULL(service);
    *service = (IAccountManager*)sThis;
    return NOERROR;
}

CAccountManagerService::AuthTokenKey::AuthTokenKey(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType)
    : mHashCode(0)
{}

Boolean CAccountManagerService::AuthTokenKey::Equals(
    /* [in] */ AuthTokenKey* authTokenKey)
{
    return FALSE;
}

Int32 CAccountManagerService::AuthTokenKey::ComputeHashCode()
{
    return -1;
}

Int32 CAccountManagerService::AuthTokenKey::HashCode()
{
    return mHashCode;
}

ECode CAccountManagerService::GetPassword(
    /* [in] */ IAccount* account,
    /* [out] */ String* password)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::GetUserData(
    /* [in] */ IAccount* account,
    /* [in] */ const String& key,
    /* [out] */ String* userData)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::GetAuthenticatorTypes(
    /* [out] */ IObjectContainer** types)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::GetAccounts(
    /* [in] */ const String& accountType,
    /* [out] */ IObjectContainer** accounts)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::HasFeatures(
    /* [in] */ IAccountManagerResponse* response,
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<String>& features)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::GetAccountsByFeatures(
    /* [in] */ IAccountManagerResponse* response,
    /* [in] */ const String& accountType,
    /* [in] */ const ArrayOf<String>& features)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::AddAccount(
    /* [in] */ IAccount* account,
    /* [in] */ const String& password,
    /* [in] */ IBundle* extras,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::RemoveAccount(
    /* [in] */ IAccountManagerResponse* response,
    /* [in] */ IAccount* account)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::InvalidateAuthToken(
    /* [in] */ const String& accountType,
    /* [in] */ const String& authToken)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::PeekAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [out] */ String* token)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::SetAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ const String& authToken)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::SetPassword(
    /* [in] */ IAccount* account,
    /* [in] */ const String& password)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::ClearPassword(
    /* [in] */ IAccount* account)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::SetUserData(
    /* [in] */ IAccount* account,
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::GetAuthToken(
    /* [in] */ IAccountManagerResponse* response,
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ Boolean notifyOnAuthFailure,
    /* [in] */ Boolean expectActivityLaunch,
    /* [in] */ IBundle* options)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::AddAcount(
    /* [in] */ IAccountManagerResponse* response,
    /* [in] */ const String& accountType,
    /* [in] */ const String& authTokenType,
    /* [in] */ const ArrayOf<String>& requiredFeatures,
    /* [in] */ Boolean expectActivityLaunch,
    /* [in] */ IBundle* options)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::UpdateCredentials(
    /* [in] */ IAccountManagerResponse* response,
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ Boolean expectActivityLaunch,
    /* [in] */ IBundle* options)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::EditProperties(
    /* [in] */ IAccountManagerResponse* response,
    /* [in] */ const String& accountType,
    /* [in] */ Boolean expectActivityLaunch)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::ConfirmCredentials(
    /* [in] */ IAccountManagerResponse* response,
    /* [in] */ IAccount* account,
    /* [in] */ IBundle* options,
    /* [in] */ Boolean expectActivityLaunch)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::constructor(
    /* [in] */ IContext* context)
{
    return E_NOT_IMPLEMENTED;
}

String CAccountManagerService::GetDatabaseName()
{
    return String(NULL);
}

PInterface CAccountManagerService::DatabaseHelper::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ISQLiteOpenHelper*)this;
    }
    else if (riid == EIID_ISQLiteOpenHelper) {
        return (ISQLiteOpenHelper*)this;
    }

    return NULL;
}

UInt32 CAccountManagerService::DatabaseHelper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAccountManagerService::DatabaseHelper::Release()
{
    return ElRefBase::Release();
}

ECode CAccountManagerService::DatabaseHelper::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

CAccountManagerService::DatabaseHelper::DatabaseHelper(
    /* [in] */ IContext* context)
    // : SQLiteOpenHelper(context, CAccountManagerService::GetDatabaseName(),
    //         NULL, DATABASE_VERSION)
{}

ECode CAccountManagerService::DatabaseHelper::GetWritableDatabase(
    /* [out] */ ISQLiteDatabase **database)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::DatabaseHelper::GetReadableDatabase(
    /* [out] */ ISQLiteDatabase **database)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::DatabaseHelper::Close()
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::DatabaseHelper::OnCreate(
    /*[in]*/ ISQLiteDatabase* db)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::DatabaseHelper::OnUpgrade(
    /*[in]*/ ISQLiteDatabase* db,
    /*[in]*/ Int32 oldVersion,
    /*[in]*/ Int32 newVersion)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerService::DatabaseHelper::OnOpen(
    /*[in]*/ ISQLiteDatabase* db)
{
    return E_NOT_IMPLEMENTED;
}

void CAccountManagerService::DatabaseHelper::CreateAccountsDeletionTrigger(
    /* [in] */ ISQLiteDatabase* db)
{}

void CAccountManagerService::DatabaseHelper::CreateGrantsTable(
    /* [in] */ ISQLiteDatabase* db)
{}
