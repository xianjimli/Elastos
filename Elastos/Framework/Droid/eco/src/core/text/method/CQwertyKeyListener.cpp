
#include "text/method/CQwertyKeyListener.h"

ECode CQwertyKeyListener::GetInputType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = QwertyKeyListener::GetInputType();

    return NOERROR;
}

ECode CQwertyKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* text,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = QwertyKeyListener::OnKeyDown(view, text, keyCode, event);

    return NOERROR;
}

ECode CQwertyKeyListener::OnKeyUp(
    /* [in] */ IView* view,
    /* [in] */ IEditable* text,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = QwertyKeyListener::OnKeyUp(view, text, keyCode, event);

    return NOERROR;
}

ECode CQwertyKeyListener::OnKeyOther(
    /* [in] */ IView* view,
    /* [in] */ IEditable* text,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = QwertyKeyListener::OnKeyOther(view, text, event);

    return NOERROR;
}

ECode CQwertyKeyListener::ClearMetaKeyState(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 states)
{
    return QwertyKeyListener::ClearMetaKeyState(view, content, states);
}

ECode CQwertyKeyListener::constructor(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext)
{
    return QwertyKeyListener::Init(cap, autotext);
}
