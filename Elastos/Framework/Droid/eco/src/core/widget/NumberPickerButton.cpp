
#include "widget/NumberPickerButton.h"

static const Int32 R_Id_Increment=0x010201fa;
static const Int32 R_Id_Decrement=0x010201fc;

NumberPickerButton::NumberPickerButton()
{

}

NumberPickerButton::NumberPickerButton(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle) : ImageButton(context, attrs, defStyle)
{

}

NumberPickerButton::NumberPickerButton(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs)  : ImageButton(context, attrs)
{

}

NumberPickerButton::NumberPickerButton(
    /* [in] */ IContext* context) : ImageButton(context)
{

}

ECode NumberPickerButton::SetNumberPicker(
    /* [in] */ INumberPicker* picker)
{
    mNumberPicker = picker;

    return NOERROR;
}

Boolean NumberPickerButton::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    CancelLongpressIfRequired(event);
    return ImageButton::OnTouchEvent(event);
}

Boolean NumberPickerButton::OnTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    CancelLongpressIfRequired(event);
    return ImageButton::OnTrackballEvent(event);
}

Boolean NumberPickerButton::OnKeyUp(
    /* [in] */ Int32 keyCode, 
    /* [in] */ IKeyEvent* event)
{
    if ((keyCode == KeyEvent_KEYCODE_DPAD_CENTER)
        || (keyCode == KeyEvent_KEYCODE_ENTER)) {
            CancelLongpress();
    }
    return ImageButton::OnKeyUp(keyCode, event);
}

void NumberPickerButton::CancelLongpressIfRequired(
    /* [in] */ IMotionEvent* event)
{
    Int32 action;
    event->GetAction(&action);
    if ((action == MotionEvent_ACTION_CANCEL)
        || (action == MotionEvent_ACTION_UP)) {
            CancelLongpress();
    }
}

void NumberPickerButton::CancelLongpress() 
{
    if (R_Id_Increment == GetId()) {
        mNumberPicker->CancelIncrement();
    } else if (R_Id_Decrement == GetId()) {
        mNumberPicker->CancelDecrement();
    }
}
