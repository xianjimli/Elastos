
#ifndef __COMPOUNDBUTTON_H__
#define __COMPOUNDBUTTON_H__

#include "widget/Button.h"

/**
 * <p>
 * A button with two states, checked and unchecked. When the button is pressed
 * or clicked, the state changes automatically.
 * </p>
 *
 * <p><strong>XML attributes</strong></p>
 * <p>
 * See {@link android.R.styleable#CompoundButton
 * CompoundButton Attributes}, {@link android.R.styleable#Button Button
 * Attributes}, {@link android.R.styleable#TextView TextView Attributes}, {@link
 * android.R.styleable#View View Attributes}
 * </p>
 */
class CompoundButton : public Button
{
public:
        //static class SavedState extends BaseSavedState {
    //    Boolean checked;

    //    /**
    //     * Constructor called from {@link CompoundButton#onSaveInstanceState()}
    //     */
    //    SavedState(Parcelable superState) {
    //        super(superState);
    //    }
    //
    //    /**
    //     * Constructor called from {@link #CREATOR}
    //     */
    //    private SavedState(Parcel in) {
    //        super(in);
    //        checked = (Boolean)in.readValue(NULL);
    //    }

    //    @Override
    //    public void writeToParcel(Parcel out, Int32 flags) {
    //        super.writeToParcel(out, flags);
    //        out.writeValue(checked);
    //    }

    //    @Override
    //    public String toString() {
    //        return "CompoundButton.SavedState{"
    //                + Integer.toHexString(System.identityHashCode(this))
    //                + " checked=" + checked + "}";
    //    }

    //    public static final Parcelable.Creator<SavedState> CREATOR
    //            = new Parcelable.Creator<SavedState>() {
    //        public SavedState createFromParcel(Parcel in) {
    //            return new SavedState(in);
    //        }

    //        public SavedState[] newArray(Int32 size) {
    //            return new SavedState[size];
    //        }
    //    };
    //}

public:
    CompoundButton();

    CompoundButton(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);

    virtual CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid) = 0;

    virtual CARAPI Toggle();

    //@Override
    CARAPI_(Boolean) PerformClick();

    virtual CARAPI_(Boolean) IsChecked();

    /**
     * <p>Changes the checked state of this button.</p>
     *
     * @param checked TRUE to check the button, FALSE to uncheck it
     */
    virtual CARAPI SetChecked(
        /* [in] */ Boolean checked);

    /**
     * Register a callback to be invoked when the checked state of this button
     * changes.
     *
     * @param listener the callback to call on checked state change
     */
    virtual CARAPI SetOnCheckedChangeListener(
        /* [in] */ IOnCheckedChangeListener* listener);

    /**
     * Register a callback to be invoked when the checked state of this button
     * changes. This callback is used for internal purpose only.
     *
     * @param listener the callback to call on checked state change
     * @hide
     */
    virtual CARAPI SetOnCheckedChangeWidgetListener(
        /* [in] */ IOnCheckedChangeListener* listener);

    /**
     * Set the background to a given Drawable, identified by its resource id.
     *
     * @param resid the resource id of the drawable to use as the background
     */
    virtual CARAPI SetButtonDrawable(
        /* [in] */ Int32 resid);

    /**
     * Set the background to a given Drawable
     *
     * @param d The Drawable to use as the background
     */
    virtual CARAPI SetButtonDrawable(
        /* [in] */ IDrawable* d);

    //@Override
    CARAPI_(Boolean) DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    //@Override
    CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    //@Override
    CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

protected:
    //@Override
    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI OnCreateDrawableState(
        /* [in] */ Int32 extraSpace,
        /* [out] */ ArrayOf<Int32>** drawableState);

    //@Override
    CARAPI DrawableStateChanged();

    //@Override
    CARAPI_(Boolean) VerifyDrawable(
        /* [in] */ IDrawable* who);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);

private:
    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    Boolean mChecked;
    Int32 mButtonResource;
    Boolean mBroadcasting;
    AutoPtr<IDrawable> mButtonDrawable;
    AutoPtr<IOnCheckedChangeListener> mOnCheckedChangeListener;
    AutoPtr<IOnCheckedChangeListener> mOnCheckedChangeWidgetListener;

    static const Int32 CHECKED_STATE_SET[1];
};

#endif //__COMPOUNDBUTTON_H__
