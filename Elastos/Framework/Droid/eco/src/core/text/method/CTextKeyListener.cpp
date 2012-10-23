
#include "text/method/CTextKeyListener.h"

ECode CTextKeyListener::GetInputType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = TextKeyListener::GetInputType();

    return NOERROR;
}

ECode CTextKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* text,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TextKeyListener::OnKeyDown(view, text, keyCode, event);

    return NOERROR;
}

ECode CTextKeyListener::OnKeyUp(
    /* [in] */ IView* view,
    /* [in] */ IEditable* text,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TextKeyListener::OnKeyUp(view, text, keyCode, event);

    return NOERROR;
}

ECode CTextKeyListener::OnKeyOther(
    /* [in] */ IView* view,
    /* [in] */ IEditable* text,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TextKeyListener::OnKeyOther(view, text, event);

    return NOERROR;
}

ECode CTextKeyListener::ClearMetaKeyState(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 states)
{
    return TextKeyListener::ClearMetaKeyState(view, content, states);
}

ECode CTextKeyListener::ReleaseListener()
{
    return TextKeyListener::ReleaseListener();
}

ECode CTextKeyListener::GetPrefs(
    /* [in] */ IContext* context,
    /* [out] */ Int32* prefs)
{
    VALIDATE_NOT_NULL(prefs);
    *prefs = TextKeyListener::GetPrefs(context);
    return NOERROR;
}

ECode CTextKeyListener::OnSpanAdded(
    /* [in] */ ISpannable* text,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return TextKeyListener::OnSpanAdded(text, what, start, end);
}

ECode CTextKeyListener::OnSpanRemoved(
    /* [in] */ ISpannable* text,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return TextKeyListener::OnSpanRemoved(text, what, start, end);
}

ECode CTextKeyListener::OnSpanChanged(
    /* [in] */ ISpannable* text,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 ostart,
    /* [in] */ Int32 oend,
    /* [in] */ Int32 nstart,
    /* [in] */ Int32 nend)
{
    return TextKeyListener::OnSpanChanged(text, what, ostart, oend, nstart, nend);
}

ECode CTextKeyListener::constructor(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext)
{
    return TextKeyListener::Init(cap, autotext);
}
