
#include "ext/frameworkdef.h"
#include "utils/CTypedValueHelper.h"
#include "utils/CTypedValue.h"
#include "utils/CDisplayMetrics.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

ECode CTypedValueHelper::ComplexToFloat(
    /* [in] */ Int32 complex,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    *value = (complex & (TypedValue_COMPLEX_MANTISSA_MASK
                   << TypedValue_COMPLEX_MANTISSA_SHIFT))
            * CTypedValue::RADIX_MULTS[(complex >> TypedValue_COMPLEX_RADIX_SHIFT)
                            & TypedValue_COMPLEX_RADIX_MASK];
    return NOERROR;
}

ECode CTypedValueHelper::ComplexToDimension(
    /* [in] */ Int32 data,
    /* [in] */ IDisplayMetrics* metrics,
    /* [out] */ Float* dimension)
{
    VALIDATE_NOT_NULL(dimension);
    assert(metrics);

    Float value;
    ComplexToFloat(data, &value);
    return ApplyDimension(
            (data >> TypedValue_COMPLEX_UNIT_SHIFT) & TypedValue_COMPLEX_UNIT_MASK,
            value, metrics, dimension);
}

ECode CTypedValueHelper::ComplexToDimensionPixelOffset(
    /* [in] */ Int32 data,
    /* [in] */ IDisplayMetrics* metrics,
    /* [out] */ Int32* pixelOffset)
{
    VALIDATE_NOT_NULL(pixelOffset);
    assert(metrics);

    Float value;
    ComplexToFloat(data, &value);
    ApplyDimension(
                (data >> TypedValue_COMPLEX_UNIT_SHIFT) & TypedValue_COMPLEX_UNIT_MASK,
                value, metrics, &value);
    *pixelOffset = (Int32)value;
    return NOERROR;
}

ECode CTypedValueHelper::ComplexToDimensionPixelSize(
    /* [in] */ Int32 data,
    /* [in] */ IDisplayMetrics* metrics,
    /* [out] */ Int32* pixelSize)
{
    VALIDATE_NOT_NULL(pixelSize);
    assert(metrics);

    Float value, f;
    ComplexToFloat(data, &value);
    ApplyDimension(
            (data >> TypedValue_COMPLEX_UNIT_SHIFT) & TypedValue_COMPLEX_UNIT_MASK,
            value, metrics, &f);
    *pixelSize = (Int32)(f + 0.5f);
    if (*pixelSize != 0) return NOERROR;
    if (value == 0) {
        *pixelSize = 0;
        return NOERROR;
    }
    if (value > 0) {
        *pixelSize = 1;
        return NOERROR;
    }
    *pixelSize = -1;
    return NOERROR;
}

ECode CTypedValueHelper::ComplexToDimensionNoisy(
    /* [in] */ Int32 data,
    /* [in] */ IDisplayMetrics* metrics,
    /* [out] */ Float* noisy)
{
    VALIDATE_NOT_NULL(noisy);
    assert(metrics);

    ComplexToDimension(data, metrics, noisy);
//      System.out.println(
//          "Dimension (0x" + ((data>>TypedValue.COMPLEX_MANTISSA_SHIFT)
//                             & TypedValue.COMPLEX_MANTISSA_MASK)
//          + "*" + (RADIX_MULTS[(data>>TypedValue.COMPLEX_RADIX_SHIFT)
//                              & TypedValue.COMPLEX_RADIX_MASK] / MANTISSA_MULT)
//          + ")" + DIMENSION_UNIT_STRS[(data>>COMPLEX_UNIT_SHIFT)
//                              & COMPLEX_UNIT_MASK]
//          + " = " + res);
    return NOERROR;
}

ECode CTypedValueHelper::ApplyDimension(
    /* [in] */ Int32 unit,
    /* [in] */ Float value,
    /* [in] */ IDisplayMetrics* metrics,
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);
    VALIDATE_NOT_NULL(metrics);

    switch (unit) {
    case TypedValue_COMPLEX_UNIT_PX:
        *result = value;
        return NOERROR;
    case TypedValue_COMPLEX_UNIT_DIP:
        *result = value * ((CDisplayMetrics*)metrics)->mDensity;
        return NOERROR;
    case TypedValue_COMPLEX_UNIT_SP:
        *result = value * ((CDisplayMetrics*)metrics)->mScaledDensity;
        return NOERROR;
    case TypedValue_COMPLEX_UNIT_PT:
        *result = value * ((CDisplayMetrics*)metrics)->mXdpi * (1.0f/72);
        return NOERROR;
    case TypedValue_COMPLEX_UNIT_IN:
        *result = value * ((CDisplayMetrics*)metrics)->mXdpi;
        return NOERROR;
    case TypedValue_COMPLEX_UNIT_MM:
        *result = value * ((CDisplayMetrics*)metrics)->mXdpi * (1.0f/25.4f);
        return NOERROR;
    }
    *result = 0;
    return NOERROR;
}

ECode CTypedValueHelper::ComplexToFraction(
    /* [in] */ Int32 data,
    /* [in] */ Float base,
    /* [in] */ Float pbase,
    /* [out] */ Float* fraction)
{
    VALIDATE_NOT_NULL(fraction);
    switch ((data>>TypedValue_COMPLEX_UNIT_SHIFT)&TypedValue_COMPLEX_UNIT_MASK) {
        case TypedValue_COMPLEX_UNIT_FRACTION: {
            ComplexToFloat(data, fraction);
            *fraction = *fraction * base;
            return NOERROR;
        }
        case TypedValue_COMPLEX_UNIT_FRACTION_PARENT: {
            ComplexToFloat(data, fraction);
            *fraction = *fraction * pbase;
            return NOERROR;
        }
    }
    *fraction = 0;
    return NOERROR;
}

ECode CTypedValueHelper::CoerceToString(
    /* [in] */ Int32 type,
    /* [in] */ Int32 data,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    switch (type) {
        case TypedValue_TYPE_NULL: {
            *str = NULL;
            return NOERROR;
        }
        case TypedValue_TYPE_REFERENCE: {
            StringBuf_<16> s;
            s.Append("@");
            s.Append(data);
            *str = (const char*)s;
            return NOERROR;
        }
        case TypedValue_TYPE_ATTRIBUTE: {
            StringBuf_<16> s;
            s.Append("?");
            s.Append(data);
            *str = (const char*)s;
            return NOERROR;
        }
        case TypedValue_TYPE_FLOAT: {
            Float f = Math::Int32BitsToFloat(data);
            StringBuf_<32> s;
            s.Append((Double)f);
            *str = (const char*)s;
            return NOERROR;
        }
        case TypedValue_TYPE_DIMENSION: {
            StringBuf_<32> s;
            Float value;
            ComplexToFloat(data, &value);
            s.Append((Double)value);
            s.Append(CTypedValue::DIMENSION_UNIT_STRS[
                (data >> TypedValue_COMPLEX_UNIT_SHIFT) & TypedValue_COMPLEX_UNIT_MASK]);
            *str = (const char*)s;
            return NOERROR;
        }
        case TypedValue_TYPE_FRACTION: {
            StringBuf_<32> s;
            Float value;
            ComplexToFloat(data, &value);
            s.Append((Double)(value * 100));
            s.Append(CTypedValue::FRACTION_UNIT_STRS[
                (data >> TypedValue_COMPLEX_UNIT_SHIFT)&TypedValue_COMPLEX_UNIT_MASK]);
            *str = (const char*)s;
            return NOERROR;
        }
        case TypedValue_TYPE_INT_HEX: {
            StringBuf_<32> s;
            s.Append("0x");
            s.Append(data, NumberFormat_Hex);
            *str = (const char*)s;
            return NOERROR;
        }
        case TypedValue_TYPE_INT_BOOLEAN: {
            StringBuf_<32> s;
            s.Copy(data != 0 ? "TRUE" : "FALSE");
            *str = (const char*)s;
            return NOERROR;
        }
    }

    if (type >= TypedValue_TYPE_FIRST_COLOR_INT && type <= TypedValue_TYPE_LAST_COLOR_INT) {
        StringBuf_<16> s;
        s.Append("#");
        s.Append(data, NumberFormat_Hex);
        *str = (const char*)s;
        return NOERROR;
    }
    else if (type >= TypedValue_TYPE_FIRST_INT && type <= TypedValue_TYPE_LAST_INT) {
        StringBuf_<16> s;
        s.Append(data);
        *str = (const char*)s;
        return NOERROR;
    }

    *str = NULL;
    return NOERROR;
}
