
#ifndef __ARROWKEYMOVEMENTMETHOD_H__
#define __ARROWKEYMOVEMENTMETHOD_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

class ArrowKeyMovementMethod
{
public:
    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyOther(
        /* [in] */ ITextView* view,
        /* [in] */ ISpannable* text,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* text,
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) CanSelectArbitrarily();

    virtual CARAPI Initialize(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* text);

    virtual CARAPI OnTakeFocus(
        /* [in] */ ITextView* view,
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 dir);

private:
    CARAPI_(Boolean) IsCap(
        /* [in] */ ISpannable* buffer);

    CARAPI_(Boolean) IsAlt(
        /* [in] */ ISpannable* buffer);

    CARAPI_(Boolean) Up(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    CARAPI_(Boolean) Down(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    CARAPI_(Boolean) Left(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    CARAPI_(Boolean) Right(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    CARAPI_(Boolean) ExecuteDown(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ Int32 keyCode);

private:
    static const AutoPtr<IInterface> LAST_TAP_DOWN;
};
#endif //__ARROWKEYMOVEMENTMETHOD_H__
