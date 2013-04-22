
#ifndef __ACCOUNTMANAGERSERVICESESSION_H__
#define __ACCOUNTMANAGERSERVICESESSION_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

class AccountManagerServiceSession
{
public:
    AccountManagerServiceSession();

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

    CARAPI Init(
        /* [in] */ IAccountManagerResponse* response,
        /* [in] */ const String& accountType,
        /* [in] */ Boolean expectActivityLaunch,
        /* [in] */ Boolean stripAuthTokenFromResult);

public:
    AutoPtr<IAccountManagerResponse> mResponse;
    String mAccountType;
    Boolean mExpectActivityLaunch;
    Int64 mCreationTime;

    Int32 mNumResults;

    AutoPtr<IAccountAuthenticator> mAuthenticator;

private:
    Int32 mNumRequestContinued;
    Int32 mNumErrors;

    Boolean mStripAuthTokenFromResult;
};

#endif //__ACCOUNTMANAGERSERVICESESSION_H__
