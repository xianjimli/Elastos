
#include "widget/CompoundButton.h"

const Int32 CompoundButton::CHECKED_STATE_SET[1] =
    {0x010100a0/*R.attr.state_checked*/};

CompoundButton::CompoundButton()
{}

CompoundButton::CompoundButton(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : Button(context, attrs, defStyle)
{
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
}

ECode CompoundButton::Toggle()
{
    return SetChecked(!mChecked);
}

Boolean CompoundButton::PerformClick()
{
    /*
     * XXX: These are tiny, need some surrounding 'expanded touch area',
     * which will need to be implemented in Button if we only override
     * performClick()
     */

    /* When clicked, toggle the state */
    Toggle();
    return Button::PerformClick();
}

Boolean CompoundButton::IsChecked()
{
    return mChecked;
}

/**
 * <p>Changes the checked state of this button.</p>
 *
 * @param checked TRUE to check the button, FALSE to uncheck it
 */
ECode CompoundButton::SetChecked(
    /* [in] */ Boolean checked)
{
    if (mChecked != checked) {
        mChecked = checked;
        RefreshDrawableState();

        // Avoid infinite recursions if setChecked() is called from a listener
        if (mBroadcasting) {
            return NOERROR;
        }

        mBroadcasting = TRUE;
        if (mOnCheckedChangeListener != NULL) {
            mOnCheckedChangeListener->OnCheckedChanged(
                    (ICompoundButton*)this->Probe(EIID_ICompoundButton), mChecked);
        }
        if (mOnCheckedChangeWidgetListener != NULL) {
            mOnCheckedChangeWidgetListener->OnCheckedChanged(
                    (ICompoundButton*)this->Probe(EIID_ICompoundButton), mChecked);
        }

        mBroadcasting = FALSE;
    }

    return NOERROR;
}

/**
 * Register a callback to be invoked when the checked state of this button
 * changes.
 *
 * @param listener the callback to call on checked state change
 */
ECode CompoundButton::SetOnCheckedChangeListener(
    /* [in] */ IOnCheckedChangeListener* listener)
{
    mOnCheckedChangeListener = listener;

    return NOERROR;
}

/**
 * Register a callback to be invoked when the checked state of this button
 * changes. This callback is used for internal purpose only.
 *
 * @param listener the callback to call on checked state change
 * @hide
 */
ECode CompoundButton::SetOnCheckedChangeWidgetListener(
    /* [in] */ IOnCheckedChangeListener* listener)
{
    mOnCheckedChangeWidgetListener = listener;

    return NOERROR;
}

/**
 * Set the background to a given Drawable, identified by its resource id.
 *
 * @param resid the resource id of the drawable to use as the background
 */
ECode CompoundButton::SetButtonDrawable(
    /* [in] */ Int32 resid)
{
    if (resid != 0 && resid == mButtonResource) {
        return NOERROR;
    }

    mButtonResource = resid;

    AutoPtr<IDrawable> d;
    if (mButtonResource != 0) {
        GetResources()->GetDrawable(mButtonResource, (IDrawable**)&d);
    }
    return SetButtonDrawable(d);
}

/**
 * Set the background to a given Drawable
 *
 * @param d The Drawable to use as the background
 */
ECode CompoundButton::SetButtonDrawable(
    /* [in] */ IDrawable* d)
{
    if (d != NULL) {
        if (mButtonDrawable != NULL) {
            mButtonDrawable->SetCallback(NULL);
            UnscheduleDrawable(mButtonDrawable);
        }
        d->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
        Boolean stateful;
        d->SetState(GetDrawableState(), &stateful);
        d->SetVisible(GetVisibility() == VISIBLE, FALSE, &stateful);
        mButtonDrawable = d;
        mButtonDrawable->SetState(NULL, &stateful);
        Int32 height;
        mButtonDrawable->GetIntrinsicHeight(&height);
        SetMinHeight(height);
    }

    RefreshDrawableState();

    return NOERROR;
}

Boolean CompoundButton::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    Boolean populated = Button::DispatchPopulateAccessibilityEvent(event);

    if (!populated) {
        Int32 resourceId = 0;
        if (mChecked) {
            resourceId = 0x01040364;/*R.string.accessibility_compound_button_selected*/
        }
        else {
            resourceId = 0x01040365;/*R.string.accessibility_compound_button_unselected*/
        }
        String state;
        GetResources()->GetString(resourceId, &state);
        //event.getText().add(state);
        //event.setChecked(mChecked);
    }

    return populated;
}

void CompoundButton::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    Button::OnDraw(canvas);

    AutoPtr<IDrawable> buttonDrawable = mButtonDrawable;
    if (buttonDrawable != NULL) {
        Int32 verticalGravity = GetGravity() & Gravity_VERTICAL_GRAVITY_MASK;
        Int32 height;
        buttonDrawable->GetIntrinsicHeight(&height);

        Int32 y = 0;

        switch (verticalGravity) {
            case Gravity_BOTTOM:
                y = GetHeight() - height;
                break;
            case Gravity_CENTER_VERTICAL:
                y = (GetHeight() - height) / 2;
                break;
        }

        Int32 width;
        buttonDrawable->GetIntrinsicWidth(&width);

        buttonDrawable->SetBounds(0, y, width, y + height);
        buttonDrawable->Draw(canvas);
    }
}

ECode CompoundButton::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace,
    /* [out] */ ArrayOf<Int32>** drawableState)
{
    Button::OnCreateDrawableState(extraSpace + 1, drawableState);
    if (IsChecked()) {
        ArrayOf_<Int32, 1> state;
        state[0] = CHECKED_STATE_SET[0];
        MergeDrawableStates(*drawableState, &state);
    }
    return NOERROR;
}

ECode CompoundButton::DrawableStateChanged()
{
    Button::DrawableStateChanged();

    if (mButtonDrawable != NULL) {
        ArrayOf<Int32>* myDrawableState = GetDrawableState();

        Boolean state;
        // Set the state of the Drawable
        mButtonDrawable->SetState(myDrawableState, &state);

        Invalidate();
    }

    return NOERROR;
}

Boolean CompoundButton::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    return Button::VerifyDrawable(who) || who == mButtonDrawable.Get();
}

AutoPtr<IParcelable> CompoundButton::OnSaveInstanceState()
{
    // Force our ancestor class to save its state
    SetFreezesText(TRUE);
    AutoPtr<IParcelable> superState = Button::OnSaveInstanceState();

    /*SavedState ss = new SavedState(superState);

    ss.checked = IsChecked();
    return ss;*/

    return NULL;
}

void CompoundButton::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    /*SavedState ss = (SavedState) state;

    Button::OnRestoreInstanceState(ss.getSuperState());
    SetChecked(ss.checked);*/
    RequestLayout();
}

ECode CompoundButton::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(Button::Init(context, attrs, defStyle));
    return InitFromAttributes(context, attrs, defStyle);
}

static Int32 R_Styleable_CompoundButton[] = {
    0x01010106, 0x01010107
};

ECode CompoundButton::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx3(
            attrs,
            ArrayOf<Int32>(R_Styleable_CompoundButton,
            sizeof(R_Styleable_CompoundButton) / sizeof(Int32)),/*com.android.internal.R.styleable.CompoundButton*/
            defStyle, 0, (ITypedArray**)&a));

    AutoPtr<IDrawable> d;
    a->GetDrawable(1/*com.android.internal.R.styleable.CompoundButton_button*/,
            (IDrawable**)&d);
    if (d != NULL) {
        SetButtonDrawable(d);
    }

    Boolean checked;
    a->GetBoolean(0/*com.android.internal.R.styleable.CompoundButton_checked*/,
            FALSE, &checked);
    SetChecked(checked);

    a->Recycle();
    return NOERROR;
}
