
#include "ext/frameworkdef.h"
#include "accounts/CAccountManagerAmsResponse.h"
#include "accounts/CAccountManager.h"

ECode CAccountManagerAmsResponse::OnResult(
    /* [in] */ IBundle* bundle)
{
    CAccountManager::AmsTask* task = (CAccountManager::AmsTask*)mHost;

    AutoPtr<IParcelable> p;
    FAIL_RETURN(bundle->GetParcelable(String("intent"), (IParcelable**)&p));
    AutoPtr<IIntent> intent = (IIntent*)p->Probe(EIID_IIntent);
    Boolean value;
    if (intent != NULL && task->mActivity != NULL) {
        // since the user provided an Activity we will silently start intents
        // that we see
        task->mActivity->StartActivity(intent);
        // leave the Future running to wait for the real response to this request
    }
    else if (bundle->GetBoolean(String("retry"), &value) && value) {
        //try {
        task->DoWork();
        // } catch (RemoteException e) {
        //     // this will only happen if the system process is dead, which means
        //     // we will be dying ourselves
        // }
    }
    else {
        task->Set(bundle);
    }

    return NOERROR;
}

ECode CAccountManagerAmsResponse::OnError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& errorMessage)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerAmsResponse::constructor(
    /* [in] */ Handle32 host)
{
    mHost = host;
    return NOERROR;
}
