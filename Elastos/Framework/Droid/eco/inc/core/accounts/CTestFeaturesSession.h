
#ifndef __CTESTFEATURESSESSION_H__
#define __CTESTFEATURESSESSION_H__

#include "_CTestFeaturesSession.h"
#include "accounts/AccountManagerServiceSession.h"
#include <elastos/AutoPtr.h>

CarClass(CTestFeaturesSession), public AccountManagerServiceSession
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
        /* [in] */ IAccount* account,
        /* [in] */ const ArrayOf<String>& features);

private:
    ArrayOf<String>* mFeatures;
    AutoPtr<IAccount> mAccount;
};

#endif //__CTESTFEATURESSESSION_H__
