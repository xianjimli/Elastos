
#include "view/CInputMethodCallback.h"

ECode CInputMethodCallback::constructor(
    /* [in] */ Handle32 viewRoot)
{
    mViewRoot = (ViewRoot*)viewRoot;
    return NOERROR;
}

ECode CInputMethodCallback::FinishedEvent(
    /* [in] */ Int32 seq,
    /* [in]*/ Boolean handled)
{
    if (mViewRoot != NULL) {
        mViewRoot->DispatchFinishedEvent(seq, handled);
    }
    return NOERROR;
}

//ECode CInputMethodCallback::SessionCreated(
//    /* [in] */ IInputMethodSession* session)
//{
//    return E_NOT_IMPLEMENTED;
//}
