
#include "text/method/CArrowKeyMovementMethod.h"

AutoPtr<CArrowKeyMovementMethod> CArrowKeyMovementMethod::sInstance;

ECode CArrowKeyMovementMethod::OnKeyDown(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ArrowKeyMovementMethod::OnKeyDown(widget, buffer, keyCode, event);

    return NOERROR;
}

ECode CArrowKeyMovementMethod::OnKeyUp(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ArrowKeyMovementMethod::OnKeyUp(widget, buffer, keyCode, event);

    return NOERROR;
}

ECode CArrowKeyMovementMethod::OnKeyOther(
    /* [in] */ ITextView* view,
    /* [in] */ ISpannable* text,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ArrowKeyMovementMethod::OnKeyOther(view, text, event);

    return NOERROR;
}

ECode CArrowKeyMovementMethod::OnTrackballEvent(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* text,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ArrowKeyMovementMethod::OnTrackballEvent(widget, text, event);

    return NOERROR;
}

ECode CArrowKeyMovementMethod::OnTouchEvent(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ArrowKeyMovementMethod::OnTouchEvent(widget, buffer, event);

    return NOERROR;
}

ECode CArrowKeyMovementMethod::CanSelectArbitrarily(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ArrowKeyMovementMethod::CanSelectArbitrarily();

    return NOERROR;
}

ECode CArrowKeyMovementMethod::Initialize(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* text)
{
    return ArrowKeyMovementMethod::Initialize(widget, text);
}

ECode CArrowKeyMovementMethod::OnTakeFocus(
    /* [in] */ ITextView* view,
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 dir)
{
    return ArrowKeyMovementMethod::OnTakeFocus(view, text, dir);
}

AutoPtr<IMovementMethod> CArrowKeyMovementMethod::GetInstance()
{
    if (sInstance == NULL) {
        CArrowKeyMovementMethod::NewByFriend((CArrowKeyMovementMethod**)&sInstance);
    }

    return sInstance;
}
