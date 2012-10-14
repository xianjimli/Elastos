
#ifndef __CTYPEDVALUEHELPER_H__
#define __CTYPEDVALUEHELPER_H__

#include "_CTypedValueHelper.h"

CarClass(CTypedValueHelper)
{
public:
    CARAPI ComplexToFloat(
        /* [in] */ Int32 complex,
        /* [out] */ Float* value);

    CARAPI ComplexToDimension(
        /* [in] */ Int32 data,
        /* [in] */ IDisplayMetrics* metrics,
        /* [out] */ Float* dimension);

    CARAPI ComplexToDimensionPixelOffset(
        /* [in] */ Int32 data,
        /* [in] */ IDisplayMetrics* metrics,
        /* [out] */ Int32* pixelOffset);

    CARAPI ComplexToDimensionPixelSize(
        /* [in] */ Int32 data,
        /* [in] */ IDisplayMetrics* metrics,
        /* [out] */ Int32* pixelSize);

    CARAPI ComplexToDimensionNoisy(
        /* [in] */ Int32 data,
        /* [in] */ IDisplayMetrics* metrics,
        /* [out] */ Float* noisy);

    CARAPI ApplyDimension(
        /* [in] */ Int32 unit,
        /* [in] */ Float value,
        /* [in] */ IDisplayMetrics* metrics,
        /* [out] */ Float* result);

    CARAPI ComplexToFraction(
        /* [in] */ Int32 data,
        /* [in] */ Float base,
        /* [in] */ Float pbase,
        /* [out] */ Float* fraction);

    CARAPI CoerceToString(
        /* [in] */ Int32 type,
        /* [in] */ Int32 data,
        /* [out] */ String* str);
};


#endif //__CTYPEDVALUEHELPER_H__
