
#ifndef __CHECKEDTEXTVIEW_H__
#define __CHECKEDTEXTVIEW_H__

#include "widget/TextView.h"

/**
 * An extension to TextView that supports the {@link android.widget.Checkable} interface.
 * This is useful when used in a {@link android.widget.ListView ListView} where the it's 
 * {@link android.widget.ListView#setChoiceMode(Int32) setChoiceMode} has been set to
 * something other than {@link android.widget.ListView#CHOICE_MODE_NONE CHOICE_MODE_NONE}.
 *
 */
class CheckedTextView : public TextView
{
public:
    CheckedTextView();

    CheckedTextView(
        /* [in] */ IContext* context);

    CheckedTextView(
        /* [in] */ IContext* context, 
        /* [in] */ IAttributeSet* attrs);

    CheckedTextView(
        /* [in] */ IContext* context, 
        /* [in] */ IAttributeSet* attrs, 
        /* [in] */ Int32 defStyle);

    virtual CARAPI Toggle();

    virtual CARAPI_(Boolean) IsChecked();

    /**
     * <p>Changes the checked state of this text view.</p>
     *
     * @param checked true to check the text, false to uncheck it
     */
    virtual CARAPI SetChecked(
        /* [in] */ Boolean checked);


    /**
     * Set the checkmark to a given Drawable, identified by its resourece id. This will be drawn
     * when {@link #isChecked()} is true.
     * 
     * @param resid The Drawable to use for the checkmark.
     */
    virtual CARAPI SetCheckMarkDrawable(
        /* [in] */ Int32 resid);

    /**
     * Set the checkmark to a given Drawable. This will be drawn when {@link #isChecked()} is true.
     *
     * @param d The Drawable to use for the checkmark.
     */
    virtual CARAPI SetCheckMarkDrawable(
        /* [in] */ IDrawable* d);
    
    virtual CARAPI SetPadding(
        /* [in] */ Int32 left, 
        /* [in] */ Int32 top, 
        /* [in] */ Int32 right, 
        /* [in] */ Int32 bottom);

    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);
    
    virtual CARAPI OnCreateDrawableState(
        /* [in] */ Int32 extraSpace,
        /* [out] */ ArrayOf<Int32>** drawableState);

    virtual CARAPI DrawableStateChanged();

    virtual CARAPI_(Boolean) DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI_(void) Init(
        /* [in] */ IContext* context, 
        /* [in] */ IAttributeSet* attrs = NULL, 
        /* [in] */ Int32 defStyle = 0);

private:
    Boolean mChecked;
    Int32 mCheckMarkResource;
    AutoPtr<IDrawable> mCheckMarkDrawable;
    Int32 mBasePaddingRight;
    Int32 mCheckMarkWidth;

    static Int32 CHECKED_STATE_SET[1];// = {
       // /*R.attr.state_checked*/0x010100a0
    //};
};

#endif
