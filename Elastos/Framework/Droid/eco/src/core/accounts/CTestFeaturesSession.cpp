
#include "ext/frameworkdef.h"
#include "accounts/CTestFeaturesSession.h"

ECode CTestFeaturesSession::OnResult(
    /* [in] */ IBundle* value)
{
    return AccountManagerServiceSession::OnResult(value);
}

ECode CTestFeaturesSession::OnRequestContinued()
{
    return AccountManagerServiceSession::OnRequestContinued();
}

ECode CTestFeaturesSession::OnError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& errorMessage)
{
    return AccountManagerServiceSession::OnError(errorCode, errorMessage);
}

ECode CTestFeaturesSession::BinderDied()
{
    return AccountManagerServiceSession::BinderDied();
}

ECode CTestFeaturesSession::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    return AccountManagerServiceSession::OnServiceConnected(name, service);
}

ECode CTestFeaturesSession::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    return AccountManagerServiceSession::OnServiceDisconnected(name);
}

ECode CTestFeaturesSession::constructor(
    /* [in] */ IAccountManagerResponse* response,
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<String>& features)
{
    String type;
    account->GetType(&type);
    AccountManagerServiceSession::Init(response, type,
            FALSE /* expectActivityLaunch */, TRUE /* stripAuthTokenFromResult */);
    mFeatures = features.Clone();
    mAccount = account;
    return NOERROR;
}
