
#ifndef __CSYNCOPERATION_H__
#define __CSYNCOPERATION_H__

#include "_CSyncOperation.h"
#include "StringBuffer.h"
#include <ext/frameworkdef.h>
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

CarClass(CSyncOperation)
{
public:
    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI GetAccount(
        /* [out] */ IAccount** account);

    CARAPI GetSyncSource(
        /* [out] */ Int32* syncSource);

    CARAPI SetSyncSource(
        /* [in] */ Int32 syncSource);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI SetAuthority(
        /* [in] */ const String& authority);

    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI SetExtras(
        /* [in] */ IBundle* extras);

    CARAPI GetKey(
        /* [out] */ String* key);

    CARAPI GetEearliestRunTime(
        /* [out] */ Int64* earliestRunTime);

    CARAPI SetEearliestRunTime(
        /* [in] */ Int64 earliestRunTime);

    CARAPI GetExpedited(
        /* [out] */ Boolean* expedited);

    CARAPI SetExpedited(
        /* [in] */ Boolean expedited);

    CARAPI constructor(
        /* [in] */ ISyncOperation* other);

    CARAPI constructor(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 source,
        /* [in] */ const String& authority,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 delayInMs);

    CARAPI CompareTo(
        /* [in] */ IInterface* obj,
        /* [out] */ Int32* result);

public:
    static void ExtrasToStringBuilder(IBundle* bundle, StringBuffer* sb, Boolean asKey);

public:
    /*const*/ AutoPtr<IAccount> account;
    Int32 syncSource;
    String authority;
    AutoPtr<IBundle> extras;
    /*const*/ String key;
    Int64 earliestRunTime;
    Boolean expedited;
//    AutoPtr<ISyncStorageEnginePendingOperation> pendingOperation;

private:
    void RemoveFalseExtra(String extraName);
    String ToKey();
};

#endif // __CSYNCOPERATION_H__
