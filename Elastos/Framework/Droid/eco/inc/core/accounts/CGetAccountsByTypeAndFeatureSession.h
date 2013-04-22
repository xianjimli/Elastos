
#ifndef __CGETACCOUNTSBYTYPEANDFEATURESESSION_H__
#define __CGETACCOUNTSBYTYPEANDFEATURESESSION_H__

#include "_CGetAccountsByTypeAndFeatureSession.h"
#include "accounts/AccountManagerServiceSession.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>

CarClass(CGetAccountsByTypeAndFeatureSession), public AccountManagerServiceSession
{
public:
    CGetAccountsByTypeAndFeatureSession();

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
        /* [in] */ const String& type,
        /* [in] */ const ArrayOf<String>& features);

public:
    ArrayOf<String>* mFeatures;
    AutoPtr<IAccount> mAccount;
    ArrayOf<AutoPtr<IAccount> >* mAccountsOfType;
    List<AutoPtr<IAccount> >* mAccountsWithFeatures;
    Int32 mCurrentAccount;
};

#endif //__CGETACCOUNTSBYTYPEANDFEATURESESSION_H__
