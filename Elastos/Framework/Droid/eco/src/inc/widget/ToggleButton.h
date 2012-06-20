
#ifndef __RADIOBUTTON_H__
#define __RADIOBUTTON_H__

#include "widget/CompoundButton.h"

class ToggleButton : public CompoundButton
{
public:
    ToggleButton(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x0101004b/*com.android.internal.R.attr.buttonStyleToggle*/);

    //@Override
    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    virtual CARAPI_(AutoPtr<ICharSequence>) GetTextOn();

    virtual CARAPI SetTextOn(
        /* [in] */ ICharSequence* textOn);

    virtual CARAPI_(AutoPtr<ICharSequence>) GetTextOff();

    virtual CARAPI SetTextOff(
        /* [in] */ ICharSequence* textOff);

    //@Override
    CARAPI SetBackgroundDrawable(
        /* [in] */ IDrawable* d);

protected:
    ToggleButton();

    //@Override
    CARAPI OnFinishInflate();

    //@Override
    CARAPI DrawableStateChanged();

    CARAPI Init(
        /* [in] */ IContext* context = NULL,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);

private:
    CARAPI_(void) UpdateReferenceToIndicatorDrawable(
        /* [in] */ IDrawable* backgroundDrawable);

    CARAPI_(void) SyncTextState();

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    AutoPtr<ICharSequence> mTextOn;
    AutoPtr<ICharSequence> mTextOff;

    AutoPtr<IDrawable> mIndicatorDrawable;

    static const Int32 NO_ALPHA = 0xFF;
    Float mDisabledAlpha;
};

#endif //__BUTTON_H__
