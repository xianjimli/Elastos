
#include "ext/frameworkdef.h"
#include "accounts/CRemoveAccountSession.h"

ECode CRemoveAccountSession::OnResult(
    /* [in] */ IBundle* value)
{
    return AccountManagerServiceSession::OnResult(value);
}

ECode CRemoveAccountSession::OnRequestContinued()
{
    return AccountManagerServiceSession::OnRequestContinued();
}

ECode CRemoveAccountSession::OnError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& errorMessage)
{
    return AccountManagerServiceSession::OnError(errorCode, errorMessage);
}

ECode CRemoveAccountSession::BinderDied()
{
    return AccountManagerServiceSession::BinderDied();
}

ECode CRemoveAccountSession::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    return AccountManagerServiceSession::OnServiceConnected(name, service);
}

ECode CRemoveAccountSession::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    return AccountManagerServiceSession::OnServiceDisconnected(name);
}

ECode CRemoveAccountSession::constructor(
    /* [in] */ IAccountManagerResponse* response,
    /* [in] */ IAccount* account)
{
    String type;
    account->GetType(&type);
    AccountManagerServiceSession::Init(response, type,
            FALSE /* expectActivityLaunch */, TRUE /* stripAuthTokenFromResult */);
    mAccount = account;
    return NOERROR;
}
