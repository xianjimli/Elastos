
#ifndef __CARROWKEYMOVEMENTMETHOD_H__
#define __CARROWKEYMOVEMENTMETHOD_H__

#include "_CArrowKeyMovementMethod.h"
#include "text/method/ArrowKeyMovementMethod.h"

CarClass(CArrowKeyMovementMethod), public ArrowKeyMovementMethod
{
public:
    CARAPI OnKeyDown(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* res);

    CARAPI OnKeyUp(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* res);

    CARAPI OnKeyOther(
        /* [in] */ ITextView* view,
        /* [in] */ ISpannable* text,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* res);

    CARAPI OnTrackballEvent(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* text,
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* res);

    CARAPI OnTouchEvent(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* res);

    CARAPI CanSelectArbitrarily(
        /* [out] */ Boolean* res);

    CARAPI Initialize(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* text);

    CARAPI OnTakeFocus(
        /* [in] */ ITextView* view,
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 dir);

    static CARAPI_(AutoPtr<IMovementMethod>) GetInstance();

private:
    static AutoPtr<CArrowKeyMovementMethod> sInstance;
};
#endif //__CARROWKEYMOVEMENTMETHOD_H__
