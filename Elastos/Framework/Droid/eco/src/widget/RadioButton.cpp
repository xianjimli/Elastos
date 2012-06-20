
#include "widget/RadioButton.h"

RadioButton::RadioButton()
{}

RadioButton::RadioButton(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : CompoundButton(context, attrs, defStyle)
{}

/**
 * {@inheritDoc}
 * <p>
 * If the radio button is already checked, this method will not toggle the radio button.
 */
ECode RadioButton::Toggle()
{
    // we override to prevent toggle when the radio is already
    // checked (as opposed to check boxes widgets)
    if (!IsChecked()) {
        return CompoundButton::Toggle();
    }

    return NOERROR;
}
