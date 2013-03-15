
#include "server/CInputMethodManagerServiceMethodCallback.h"


ECode CInputMethodManagerServiceMethodCallback::constructor(
    /* [in] */ IInputMethod* method,
    /* [in] */ Handle32 host)
{
    mMethod = method;
    mHost = (CInputMethodManagerService*)host;
    return NOERROR;
}

ECode CInputMethodManagerServiceMethodCallback::FinishedEvent(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    return NOERROR;
}

ECode CInputMethodManagerServiceMethodCallback::SessionCreated(
    /* [in] */ IInputMethodSession* session)
{
    assert(mHost != NULL);
    mHost->OnSessionCreated(mMethod, session);
    return NOERROR;
}

ECode CInputMethodManagerServiceMethodCallback::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}
