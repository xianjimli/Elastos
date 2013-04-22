
#include "accounts/AccountManagerServiceSession.h"

AccountManagerServiceSession::AccountManagerServiceSession()
    : mExpectActivityLaunch(FALSE)
    , mCreationTime(0)
    , mNumResults(0)
    , mNumRequestContinued(0)
    , mNumErrors(0)
    , mStripAuthTokenFromResult(FALSE)
{}

ECode AccountManagerServiceSession::OnResult(
    /* [in] */ IBundle* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode AccountManagerServiceSession::OnRequestContinued()
{
    return E_NOT_IMPLEMENTED;
}

ECode AccountManagerServiceSession::OnError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& errorMessage)
{
    return E_NOT_IMPLEMENTED;
}

ECode AccountManagerServiceSession::BinderDied()
{
    return E_NOT_IMPLEMENTED;
}

ECode AccountManagerServiceSession::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    return E_NOT_IMPLEMENTED;
}

ECode AccountManagerServiceSession::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    return E_NOT_IMPLEMENTED;
}

ECode AccountManagerServiceSession::Init(
    /* [in] */ IAccountManagerResponse* response,
    /* [in] */ const String& accountType,
    /* [in] */ Boolean expectActivityLaunch,
    /* [in] */ Boolean stripAuthTokenFromResult)
{
    return E_NOT_IMPLEMENTED;
}
