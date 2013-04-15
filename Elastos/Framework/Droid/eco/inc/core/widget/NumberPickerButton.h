
#ifndef __NUMBERPICKERBUTTON_H__
#define __NUMBERPICKERBUTTON_H__

#include "widget/ImageButton.h"

/**
 * This class exists purely to cancel long click events, that got
 * started in NumberPicker
 */
class NumberPickerButton : public ImageButton
{
public:
    NumberPickerButton();

    NumberPickerButton(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    NumberPickerButton(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    NumberPickerButton(
        /* [in] */ IContext* context);

    virtual CARAPI SetNumberPicker(
        /* [in] */ INumberPicker* picker);

    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

private:
    CARAPI_(void) CancelLongpressIfRequired(
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) CancelLongpress();

    AutoPtr<INumberPicker> mNumberPicker;
};

#endif
