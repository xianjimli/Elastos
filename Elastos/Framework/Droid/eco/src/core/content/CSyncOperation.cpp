
#include "content/CSyncOperation.h"
#include "os/SystemClock.h"
#include "os/CBundle.h"

ECode CSyncOperation::GetDescription(
    /* [out] */ String* description)
{
    VALIDATE_NOT_NULL(description);

    *description = String("");
    return NOERROR;
}

ECode CSyncOperation::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);

    return E_NOT_IMPLEMENTED;
}

ECode CSyncOperation::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuffer sb("");

    sb += "authority: ";
    sb += authority;

    // Append account info
    String accountInfo;
    sb += " account: Account {name=";
    account->GetName(&accountInfo);
    sb += accountInfo;
    sb += ", type=";
    account->GetType(&accountInfo);
    sb += accountInfo;
    sb += "}";

    sb += " extras: ";

    ExtrasToStringBuilder(extras, &sb, FALSE /* asKey */);

    sb += " syncSource: ";
    sb += syncSource;
    sb += " when: ";
    sb += earliestRunTime;

    sb += " expedited: ";
    sb += expedited;

    *str = String(sb);

    return NOERROR;
}

ECode CSyncOperation::GetAccount(
    /* [out] */ IAccount** account)
{
    VALIDATE_NOT_NULL(account);

    *account = this->account;
    return NOERROR;
}

ECode CSyncOperation::GetSyncSource(
    /* [out] */ Int32* syncSource)
{
    VALIDATE_NOT_NULL(syncSource);

    *syncSource = this->syncSource;
    return NOERROR;
}

ECode CSyncOperation::SetSyncSource(
    /* [in] */ Int32 syncSource)
{
    this->syncSource = syncSource;
    return NOERROR;
}

ECode CSyncOperation::GetPendingOperation(
    /* [out] */ ISyncStorageEnginePendingOperation** pendingOperation)
{
    VALIDATE_NOT_NULL(pendingOperation);

    *pendingOperation = this->pendingOperation;
    return NOERROR;
}

ECode CSyncOperation::SetPendingOperation(
    /* [in] */ ISyncStorageEnginePendingOperation* pendingOperation)
{
    this->pendingOperation = pendingOperation;
    return NOERROR;
}

ECode CSyncOperation::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);

    *authority = this->authority;
    return NOERROR;
}

ECode CSyncOperation::SetAuthority(
    /* [in] */ const String& authority)
{
    this->authority = authority;
    return NOERROR;
}

ECode CSyncOperation::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras);

    *extras = this->extras;
    return NOERROR;
}

ECode CSyncOperation::SetExtras(
    /* [in] */ IBundle* extras)
{
    this->extras = extras;
    return NOERROR;
}

ECode CSyncOperation::GetKey(
    /* [out] */ String* key)
{
    VALIDATE_NOT_NULL(key);

    *key = this->key;
    return NOERROR;
}

ECode CSyncOperation::GetEearliestRunTime(
    /* [out] */ Int64* earliestRunTime)
{
    VALIDATE_NOT_NULL(earliestRunTime);

    *earliestRunTime = this->earliestRunTime;
    return NOERROR;
}

ECode CSyncOperation::SetEearliestRunTime(
    /* [in] */ Int64 earliestRunTime)
{
    this->earliestRunTime = earliestRunTime;
    return NOERROR;
}

ECode CSyncOperation::GetExpedited(
    /* [out] */ Boolean* expedited)
{
    VALIDATE_NOT_NULL(expedited);
    *expedited = this->expedited;
    return NOERROR;
}

ECode CSyncOperation::SetExpedited(
    /* [in] */ Boolean expedited)
{
    this->expedited = expedited;
    return NOERROR;
}

ECode CSyncOperation::constructor(
    /* [in] */ ISyncOperation* other)
{
    if(other == NULL){
        return E_INVALID_ARGUMENT;
    }

    other->GetAccount((IAccount**)&account);
    other->GetSyncSource(&syncSource);
    other->GetAuthority(&authority);

//    this->extras = new Bundle(other->extras);
    AutoPtr<IBundle> otherExtras;
    other->GetExtras((IBundle**)&otherExtras);

    CBundle::New((IBundle*)otherExtras, (IBundle**)&extras);

    other->GetExpedited(&expedited);
    earliestRunTime = SystemClock::GetElapsedRealtime();

    this->key = ToKey();

    return NOERROR;
}

ECode CSyncOperation::constructor(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 source,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* otherExtras,
    /* [in] */ Int64 delayInMs)
{
    this->account = account;
    this->syncSource = source;
    this->authority = authority;

    CBundle::New((IBundle*)otherExtras, (IBundle**)&extras);

    RemoveFalseExtra(String(ContentResolver_SYNC_EXTRAS_UPLOAD));
    RemoveFalseExtra(String(ContentResolver_SYNC_EXTRAS_MANUAL));
    RemoveFalseExtra(String(ContentResolver_SYNC_EXTRAS_IGNORE_SETTINGS));
    RemoveFalseExtra(String(ContentResolver_SYNC_EXTRAS_IGNORE_BACKOFF));
    RemoveFalseExtra(String(ContentResolver_SYNC_EXTRAS_DO_NOT_RETRY));
    RemoveFalseExtra(String(ContentResolver_SYNC_EXTRAS_DISCARD_LOCAL_DELETIONS));
    RemoveFalseExtra(String(ContentResolver_SYNC_EXTRAS_EXPEDITED));
    RemoveFalseExtra(String(ContentResolver_SYNC_EXTRAS_OVERRIDE_TOO_MANY_DELETIONS));

    /*final*/ Int64 now = SystemClock::GetElapsedRealtime();

    if (delayInMs < 0) {
        this->expedited = TRUE;
        this->earliestRunTime = now;
    } else {
        this->expedited = FALSE;
        this->earliestRunTime = now + delayInMs;
    }

    this->key = ToKey();

    return NOERROR;
}

ECode CSyncOperation::CompareTo(
    /* [in] */ IInterface* obj,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return E_NOT_IMPLEMENTED;
}

void CSyncOperation::ExtrasToStringBuilder(IBundle* bundle, StringBuffer* sb, Boolean asKey)
{
    if(bundle == NULL || sb == NULL){
        return;
    }

    (*sb) += "[";

    IObjectContainer* keySetContainer;
    IObjectEnumerator* emuKeys;

    bundle->KeySet(&keySetContainer);

    keySetContainer->GetObjectEnumerator(&emuKeys);

    Boolean hasNext = FALSE;
    String syncExtraInitialize(ContentResolver_SYNC_EXTRAS_INITIALIZE);

    while (emuKeys->MoveNext(&hasNext), hasNext) {
        String key;
        emuKeys->Current((PInterface*)&key);

        // if we are writing this as a key don't consider whether this
        // is an initialization sync or not when computing the key since
        // we set this flag appropriately when dispatching the sync request.
        if (asKey && syncExtraInitialize.Equals(key)) {
            continue;
        }

        // TODO: Check the value type, maybe boolean type of other else
//        String value;
//        bundle->GetString(key, &value);
//        (*sb) += (key + String("=") + value + String(" "));
    }

    (*sb) += "]";

    if(keySetContainer != NULL){
        keySetContainer->Release();
        keySetContainer = NULL;
    }

    if(emuKeys != NULL){
        emuKeys->Release();
        emuKeys = NULL;
    }
}

void CSyncOperation::RemoveFalseExtra(String extraName)
{
    Boolean result = FALSE;
    extras->GetBoolean(extraName, &result);

    if (result == FALSE) {
        extras->Remove(extraName);
    }
}

String CSyncOperation::ToKey()
{
    StringBuffer sb("");
    sb += "authority: ";
    sb += authority;

    String accountProperty;

    // Append account name
    sb += " account {name=";
    account->GetName(&accountProperty);
    sb += accountProperty;

    // Append account type
    sb += ", type=";
    account->GetType(&accountProperty);
    sb += accountProperty;
    sb += "}";

    sb += " extras: ";

    ExtrasToStringBuilder(extras, &sb, TRUE /* asKey */);

    return String(sb);
}
