
#include "CChoiceFormatHelper.h"
#include "ChoiceFormat.h"
#include "cmdef.h"

ECode CChoiceFormatHelper::NextDouble(
    /* [in] */ Double value,
    /* [out] */ Double* nextValue)
{
    VALIDATE_NOT_NULL(nextValue);
    *nextValue = ChoiceFormat::NextDouble(value);
    return NOERROR;
}

/**
 * Returns the double value which is closest to the specified double but
 * either larger or smaller as specified.
 *
 * @param value
 *            a double value.
 * @param increment
 *            {@code true} to get the next larger value, {@code false} to
 *            get the previous smaller value.
 * @return the next larger or smaller double value.
 */
ECode CChoiceFormatHelper::NextDoubleEx(
    /* [in] */ Double value,
    /* [in] */ Boolean increment,
    /* [out] */ Double* nextValue)
{
    VALIDATE_NOT_NULL(nextValue);
    *nextValue = ChoiceFormat::NextDouble(value, increment);
    return NOERROR;
}

/**
 * Returns the double value which is closest to the specified double but
 * smaller.
 *
 * @param value
 *            a double value.
 * @return the next smaller double value.
 */
ECode CChoiceFormatHelper::PreviousDouble(
    /* [in] */ Double value,
    /* [out] */ Double* preValue)
{
    VALIDATE_NOT_NULL(preValue);
    *preValue = ChoiceFormat::PreviousDouble(value);
    return NOERROR;
}
