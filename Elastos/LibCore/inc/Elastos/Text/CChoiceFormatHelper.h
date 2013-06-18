
#ifndef __CCHOICEFORMATHELPER_H__
#define __CCHOICEFORMATHELPER_H__

#include "_CChoiceFormatHelper.h"

CarClass(CChoiceFormatHelper)
{
public:
    /**
     * Returns the double value which is closest to the specified double but
     * larger.
     *
     * @param value
     *            a double value.
     * @return the next larger double value.
     */
    CARAPI NextDouble(
        /* [in] */ Double value,
        /* [out] */ Double* nextValue);

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
    CARAPI NextDoubleEx(
        /* [in] */ Double value,
        /* [in] */ Boolean increment,
        /* [out] */ Double* nextValue);

    /**
     * Returns the double value which is closest to the specified double but
     * smaller.
     *
     * @param value
     *            a double value.
     * @return the next smaller double value.
     */
    CARAPI PreviousDouble(
        /* [in] */ Double value,
        /* [out] */ Double* preValue);
};

#endif // __CCHOICEFORMATHELPER_H__
