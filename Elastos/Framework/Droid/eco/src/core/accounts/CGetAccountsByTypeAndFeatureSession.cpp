
#include "accounts/CGetAccountsByTypeAndFeatureSession.h"

CGetAccountsByTypeAndFeatureSession::CGetAccountsByTypeAndFeatureSession()
    : AccountManagerServiceSession()
    , mCurrentAccount(0)
{}

ECode CGetAccountsByTypeAndFeatureSession::OnResult(
    /* [in] */ IBundle* value)
{
    return AccountManagerServiceSession::OnResult(value);
}

ECode CGetAccountsByTypeAndFeatureSession::OnRequestContinued()
{
    return AccountManagerServiceSession::OnRequestContinued();
}

ECode CGetAccountsByTypeAndFeatureSession::OnError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& errorMessage)
{
    return AccountManagerServiceSession::OnError(errorCode, errorMessage);
}

ECode CGetAccountsByTypeAndFeatureSession::BinderDied()
{
    return AccountManagerServiceSession::BinderDied();
}

ECode CGetAccountsByTypeAndFeatureSession::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    return AccountManagerServiceSession::OnServiceConnected(name, service);
}

ECode CGetAccountsByTypeAndFeatureSession::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    return AccountManagerServiceSession::OnServiceDisconnected(name);
}

ECode CGetAccountsByTypeAndFeatureSession::constructor(
    /* [in] */ IAccountManagerResponse* response,
    /* [in] */ const String& type,
    /* [in] */ const ArrayOf<String>& features)
{
    AccountManagerServiceSession::Init(response, type,
            FALSE /* expectActivityLaunch */, TRUE /* stripAuthTokenFromResult */);
    mFeatures = features.Clone();
    return NOERROR;
}
