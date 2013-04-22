
#ifndef __CREMOVEACCOUNTSESSION_H__
#define __CREMOVEACCOUNTSESSION_H__

#include "_CRemoveAccountSession.h"
#include "accounts/AccountManagerServiceSession.h"
#include <elastos/AutoPtr.h>

CarClass(CRemoveAccountSession), public AccountManagerServiceSession
{
public:
    CARAPI OnResult(
        /* [in] */ IBundle* value);

    CARAPI OnRequestContinued();

    CARAPI OnError(
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& errorMessage);

    CARAPI BinderDied();

    CARAPI OnServiceConnected(
        /* [in] */ IComponentName* name,
        /* [in] */ IBinder* service);

    CARAPI OnServiceDisconnected(
        /* [in] */ IComponentName* name);

    CARAPI constructor(
        /* [in] */ IAccountManagerResponse* response,
        /* [in] */ IAccount* account);

public:
    AutoPtr<IAccount> mAccount;
};

#endif //__CREMOVEACCOUNTSESSION_H__
