
#include "ext/frameworkdef.h"
#include "utils/CTypedValue.h"
#include "utils/CDisplayMetrics.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

const Float CTypedValue::MANTISSA_MULT =
    1.0f / (1<<TypedValue_COMPLEX_MANTISSA_SHIFT);
const Float CTypedValue::RADIX_MULTS[] = {
    1.0f*MANTISSA_MULT, 1.0f/(1<<7)*MANTISSA_MULT,
    1.0f/(1<<15)*MANTISSA_MULT, 1.0f/(1<<23)*MANTISSA_MULT
};
const String CTypedValue::DIMENSION_UNIT_STRS[] = {
    String("px"), String("dip"), String("sp"), String("pt"), String("in"), String("mm")
};
const String CTypedValue::FRACTION_UNIT_STRS[] = {
    String("%"), String("%p")
};

CTypedValue::CTypedValue()
    : mType(0)
    , mString(NULL)
    , mData(0)
    , mAssetCookie(0)
    , mResourceId(0)
    , mChangingConfigurations(-1)
    , mDensity(0)
{
}

CTypedValue::~CTypedValue()
{
}

ECode CTypedValue::GetFloat(
    /* [out] */ Float* f)
{
    VALIDATE_NOT_NULL(f);
    *f = Math::Int32BitsToFloat(mData);
    return NOERROR;
}

Float CTypedValue::ComplexToFloat(Int32 complex)
{
    return (complex&(TypedValue_COMPLEX_MANTISSA_MASK
                   <<TypedValue_COMPLEX_MANTISSA_SHIFT))
            * RADIX_MULTS[(complex>>TypedValue_COMPLEX_RADIX_SHIFT)
                            & TypedValue_COMPLEX_RADIX_MASK];
}

Float CTypedValue::ComplexToDimension(
    /* [in] */ Int32 data,
    /* [in] */ IDisplayMetrics* metrics)
{
    assert(metrics);

    return ApplyDimension(
            (data>>TypedValue_COMPLEX_UNIT_SHIFT)&TypedValue_COMPLEX_UNIT_MASK,
            ComplexToFloat(data),
            metrics);
}

Int32 CTypedValue::ComplexToDimensionPixelOffset(
    /* [in] */ Int32 data,
    /* [in] */ IDisplayMetrics* metrics)
{
    assert(metrics);

    return (Int32)ApplyDimension(
                (data>>TypedValue_COMPLEX_UNIT_SHIFT)&TypedValue_COMPLEX_UNIT_MASK,
                ComplexToFloat(data),
                metrics);
}

Int32 CTypedValue::ComplexToDimensionPixelSize(
    /* [in] */ Int32 data,
    /* [in] */ IDisplayMetrics* metrics)
{
    assert(metrics);

    const Float value = ComplexToFloat(data);
    const Float f = ApplyDimension(
            (data>>TypedValue_COMPLEX_UNIT_SHIFT)&TypedValue_COMPLEX_UNIT_MASK,
            value,
            metrics);
    const Int32 res = (Int32)(f+0.5f);
    if (res != 0) return res;
    if (value == 0) return 0;
    if (value > 0) return 1;
    return -1;
}

Float CTypedValue::ComplexToDimensionNoisy(
    /* [in] */ Int32 data,
    /* [in] */ IDisplayMetrics* metrics)
{
    assert(metrics);

    Float res = ComplexToDimension(data, metrics);
//	    System.out.println(
//	        "Dimension (0x" + ((data>>TypedValue.COMPLEX_MANTISSA_SHIFT)
//	                           & TypedValue.COMPLEX_MANTISSA_MASK)
//	        + "*" + (RADIX_MULTS[(data>>TypedValue.COMPLEX_RADIX_SHIFT)
//	                            & TypedValue.COMPLEX_RADIX_MASK] / MANTISSA_MULT)
//	        + ")" + DIMENSION_UNIT_STRS[(data>>COMPLEX_UNIT_SHIFT)
//	                            & COMPLEX_UNIT_MASK]
//	        + " = " + res);
    return res;
}

Float CTypedValue::ApplyDimension(
        /* [in] */ Int32 unit,
        /* [in] */ Float value,
        /* [in] */ IDisplayMetrics* metrics)
{
    assert(metrics);

    switch (unit) {
    case TypedValue_COMPLEX_UNIT_PX:
        return value;
    case TypedValue_COMPLEX_UNIT_DIP:
        return value * ((CDisplayMetrics*)metrics)->mDensity;
    case TypedValue_COMPLEX_UNIT_SP:
        return value * ((CDisplayMetrics*)metrics)->mScaledDensity;
    case TypedValue_COMPLEX_UNIT_PT:
        return value * ((CDisplayMetrics*)metrics)->mXdpi * (1.0f/72);
    case TypedValue_COMPLEX_UNIT_IN:
        return value * ((CDisplayMetrics*)metrics)->mXdpi;
    case TypedValue_COMPLEX_UNIT_MM:
        return value * ((CDisplayMetrics*)metrics)->mXdpi * (1.0f/25.4f);
    }
    return 0;
}

ECode CTypedValue::GetDimension(
    /* [in] */ IDisplayMetrics* metrics,
    /* [out] */ Float* dimension)
{
    VALIDATE_NOT_NULL(metrics);
    VALIDATE_NOT_NULL(dimension);

    *dimension = ComplexToDimension(mData, metrics);
    return NOERROR;
}

Float CTypedValue::ComplexToFraction(
    /* [in] */ Int32 data,
    /* [in] */ Float base,
    /* [in] */ Float pbase)
{
    switch ((data>>TypedValue_COMPLEX_UNIT_SHIFT)&TypedValue_COMPLEX_UNIT_MASK) {
    case TypedValue_COMPLEX_UNIT_FRACTION:
        return ComplexToFloat(data) * base;
    case TypedValue_COMPLEX_UNIT_FRACTION_PARENT:
        return ComplexToFloat(data) * pbase;
    }
    return 0;
}

ECode CTypedValue::GetFraction(
    /* [in] */ Float base,
    /* [in] */ Float pbase,
    /* [out] */ Float* fraction)
{
    VALIDATE_NOT_NULL(fraction);

    *fraction = ComplexToFraction(mData, base, pbase);
    return NOERROR;
}

ECode CTypedValue::CoerceToString(
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);

    Int32 t = mType;
    if (t == TypedValue_TYPE_STRING) {
        *csq = mString.Get();
        if (*csq) (*csq)->AddRef();
        return NOERROR;
    }
    String s;
    CoerceToStringEx(t, mData, &s);
    return CStringWrapper::New(s, csq);
}

ECode CTypedValue::CoerceToStringEx(
    /* [in] */ Int32 type,
    /* [in] */ Int32 data,
    /* [out] */ String* str)
{
    assert(str);

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
            s.Append((Double)ComplexToFloat(data));
            s.Append(DIMENSION_UNIT_STRS[
                (data>>TypedValue_COMPLEX_UNIT_SHIFT)&TypedValue_COMPLEX_UNIT_MASK]);
            *str = (const char*)s;
            return NOERROR;
        }
        case TypedValue_TYPE_FRACTION: {
            StringBuf_<32> s;
            s.Append((Double)(ComplexToFloat(data)*100));
            s.Append(FRACTION_UNIT_STRS[
                (data>>TypedValue_COMPLEX_UNIT_SHIFT)&TypedValue_COMPLEX_UNIT_MASK]);
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

ECode CTypedValue::SetTo(
    /* [in] */ ITypedValue* other)
{
    VALIDATE_NOT_NULL(other);

    mType = ((CTypedValue*)other)->mType;
    mString = ((CTypedValue*)other)->mString;
    mData = ((CTypedValue*)other)->mData;
    mAssetCookie = ((CTypedValue*)other)->mAssetCookie;
    mResourceId = ((CTypedValue*)other)->mResourceId;
    mDensity = ((CTypedValue*)other)->mDensity;
    return NOERROR;
}

ECode CTypedValue::GetDescription(
    /* [out] */ String* str)
{
//	    StringBuilder sb = new StringBuilder();
//	    sb.append("TypedValue{t=0x").append(Integer.toHexString(type));
//	    sb.append("/d=0x").append(Integer.toHexString(data));
//	    if (type == TYPE_STRING) {
//	        sb.append(" \"").append(string != null ? string : "<null>").append("\"");
//	    }
//	    if (assetCookie != 0) {
//	        sb.append(" a=").append(assetCookie);
//	    }
//	    if (resourceId != 0) {
//	        sb.append(" r=0x").append(Integer.toHexString(resourceId));
//	    }
//	    sb.append("}");
//	    return sb.toString();
    return E_NOT_IMPLEMENTED;
}

ECode CTypedValue::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;
    return NOERROR;
}

ECode CTypedValue::SetType(
    /* [in] */ Int32 type)
{
    mType = type;
    return NOERROR;
}

ECode CTypedValue::GetString(
    /* [out] */ ICharSequence** string)
{
    VALIDATE_NOT_NULL(string);
    *string = mString;
    if (*string != NULL) (*string)->AddRef();
    return NOERROR;
}

ECode CTypedValue::SetString(
    /* [in] */ ICharSequence* string)
{
    mString = string;
    return NOERROR;
}

ECode CTypedValue::GetData(
    /* [out] */ Int32* data)
{
    VALIDATE_NOT_NULL(data);
    *data = mData;
    return NOERROR;
}

ECode CTypedValue::SetData(
    /* [in] */ Int32 data)
{
    mData = data;
    return NOERROR;
}

ECode CTypedValue::GetResourceId(
    /* [out] */ Int32* resId)
{
    VALIDATE_NOT_NULL(resId);
    *resId = mResourceId;
    return NOERROR;
}

ECode CTypedValue::SetResourceId(
    /* [in] */ Int32 resId)
{
    mResourceId = resId;
    return NOERROR;
}
