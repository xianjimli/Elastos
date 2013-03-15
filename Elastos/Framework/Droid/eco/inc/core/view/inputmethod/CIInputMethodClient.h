
#ifndef  __CIINPUTMETHODCLIENT_H__
#define  __CIINPUTMETHODCLIENT_H__

#include "_CIInputMethodClient.h"
#include "view/inputmethod/CLocalInputMethodManager.h"
#include <elastos/AutoPtr.h>


CarClass(CIInputMethodClient)
{
public:
    CARAPI constructor(
        /* [in] */ ILocalInputMethodManager* host);

    CARAPI SetUsingInputMethod(
        /* [in] */ Boolean state);

    CARAPI OnBindMethod(
        /* [in] */ IInputBindResult* res);

    CARAPI OnUnbindMethod(
        /* [in] */ Int32 sequence);

    CARAPI SetActive(
        /* [in] */ Boolean active);

    CARAPI GetDescription(
        /* [out] */ String* str);

private:
    AutoPtr<CLocalInputMethodManager> mHost;
};

#endif  //__CIINPUTMETHODCLIENT_H__
