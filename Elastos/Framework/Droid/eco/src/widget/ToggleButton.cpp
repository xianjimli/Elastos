
#include "widget/ToggleButton.h"

const Int32 ToggleButton::NO_ALPHA;

ToggleButton::ToggleButton()
{}

ToggleButton::ToggleButton(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : CompoundButton(context, attrs, defStyle)
{
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
}

ECode ToggleButton::SetChecked(
    /* [in] */ Boolean checked)
{
    CompoundButton::SetChecked(checked);

    SyncTextState();

    return NOERROR;
}

void ToggleButton::SyncTextState()
{
    Boolean checked = IsChecked();
    if (checked && mTextOn != NULL) {
        SetText(mTextOn);
    }
    else if (!checked && mTextOff != NULL) {
        SetText(mTextOff);
    }
}

/**
 * Returns the text for when the button is in the checked state.
 *
 * @return The text.
 */
AutoPtr<ICharSequence> ToggleButton::GetTextOn()
{
    return mTextOn;
}

/**
 * Sets the text for when the button is in the checked state.
 *
 * @param textOn The text.
 */
ECode ToggleButton::SetTextOn(
    /* [in] */ ICharSequence* textOn)
{
    mTextOn = textOn;

    return NOERROR;
}

/**
 * Returns the text for when the button is not in the checked state.
 *
 * @return The text.
 */
AutoPtr<ICharSequence> ToggleButton::GetTextOff()
{
    return mTextOff;
}

/**
 * Sets the text for when the button is not in the checked state.
 *
 * @param textOff The text.
 */
ECode ToggleButton::SetTextOff(
    /* [in] */ ICharSequence* textOff)
{
    mTextOff = textOff;

    return NOERROR;
}

ECode ToggleButton::OnFinishInflate()
{
    CompoundButton::OnFinishInflate();

    UpdateReferenceToIndicatorDrawable(GetBackground());

    return NOERROR;
}

ECode ToggleButton::SetBackgroundDrawable(
    /* [in] */ IDrawable* d)
{
    CompoundButton::SetBackgroundDrawable(d);

    UpdateReferenceToIndicatorDrawable(d);

    return NOERROR;
}

void ToggleButton::UpdateReferenceToIndicatorDrawable(
    /* [in] */ IDrawable* backgroundDrawable)
{
    if (ILayerDrawable::Probe(backgroundDrawable)) {
        AutoPtr<ILayerDrawable> layerDrawable =
            ILayerDrawable::Probe(backgroundDrawable);
        layerDrawable->FindDrawableByLayerId(
            0x01020017/*com.android.internal.R.id.toggle*/,
            (IDrawable**)&mIndicatorDrawable);
    }
}

ECode ToggleButton::DrawableStateChanged()
{
    CompoundButton::DrawableStateChanged();

    if (mIndicatorDrawable != NULL) {
        mIndicatorDrawable->SetAlpha(IsEnabled() ? NO_ALPHA : (Int32)(NO_ALPHA * mDisabledAlpha));
    }

    return NOERROR;
}

ECode ToggleButton::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(CompoundButton::Init(context, attrs, defStyle));
    return InitFromAttributes(context, attrs, defStyle);
}

static Int32 R_Styleable_ToggleButton[] = {
    0x01010033, 0x01010124, 0x01010125
};

ECode ToggleButton::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx3(
            attrs,
            ArrayOf<Int32>(R_Styleable_ToggleButton,
            sizeof(R_Styleable_ToggleButton) / sizeof(Int32)),/*com.android.internal.R.styleable.ToggleButton*/
            defStyle, 0, (ITypedArray**)&a));

    a->GetText(1/*com.android.internal.R.styleable.ToggleButton_textOn*/,
            (ICharSequence**)&mTextOn);
    a->GetText(2/*com.android.internal.R.styleable.ToggleButton_textOff*/,
            (ICharSequence**)&mTextOff);
    a->GetFloat(0/*com.android.internal.R.styleable.ToggleButton_disabledAlpha*/,
            0.5f, &mDisabledAlpha);
    SyncTextState();
    a->Recycle();
    return NOERROR;
}
