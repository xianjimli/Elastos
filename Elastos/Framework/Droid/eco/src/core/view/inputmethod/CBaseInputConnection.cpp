
#include "view/inputmethod/CBaseInputConnection.h"

BASEINPUTCONNECTION_METHODS_IMPL(CBaseInputConnection, BaseInputConnection, BaseInputConnection);

ECode CBaseInputConnection::constructor(
    /* [in] */ ILocalInputMethodManager* mgr,
    /* [in] */ Boolean fullEditor)
{
    return BaseInputConnection::Init(mgr, fullEditor);
}

ECode CBaseInputConnection::constructor(
    /* [in] */ IView* targetView,
    /* [in] */ Boolean fullEditor)
{
    return BaseInputConnection::Init(targetView, fullEditor);
}
