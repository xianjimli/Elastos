
#ifndef __CINPUTMETHODCALLBACK_H__
#define __CINPUTMETHODCALLBACK_H__

#include "_CInputMethodCallback.h"
#include "view/ViewRoot.h"

class ViewRoot;

CarClass(CInputMethodCallback)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 viewRoot);

    CARAPI FinishedEvent(
        /* [in] */ Int32 seq,
        /* [in]*/ Boolean handled);

    CARAPI SessionCreated(
        /* [in] */ IInputMethodSession* session);

    CARAPI GetDescription(
        /* [out] */ String* description);

private:
    AutoPtr<ViewRoot> mViewRoot;
};

#endif //__CINPUTMETHODCALLBACK_H__