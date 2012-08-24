
#include <elastos/Math.h>
#include <math.h>
#include <stdlib.h>

namespace Elastos {
namespace Core {

#ifdef _win32
#define isnanf __isnanf
#endif


const Int32 Math::INT32_EXPONENT_BIAS = 1023;
const Int32 Math::INT32_EXPONENT_BITS = 12;
const Int32 Math::INT32_MANTISSA_BITS = 52;
const Int32 Math::INT32_NON_MANTISSA_BITS = 12;
const Int32 Math::INT32_MAX_EXPONENT = 1023;
const Int32 Math::INT32_MAX_VALUE = 0X7FFFFFFF;
const Int32 Math::INT32_MIN_VALUE = 0x80000000;
const Int32 Math::INT32_NAN = 0X7FC00000;
const Int32 Math::FLOAT_EXPONENT_BIAS = 127;
const Int32 Math::FLOAT_MANTISSA_BITS = 23;
const Int32 Math::FLOAT_NON_MANTISSA_BITS = 9;
const Int32 Math::FLOAT_SIGN_MASK = 0X80000000;
const Int32 Math::FLOAT_EXPONENT_MASK = 0X7F800000;
const Int32 Math::FLOAT_MANTISSA_MASK = 0X007FFFFF;
const Int32 Math::FLOAT_MAX_EXPONENT = 127;

const Int64 Math::INT64_MAX_VALUE = 0x7FFFFFFFFFFFFFFFLL;
const Int64 Math::INT64_MIN_VALUE = 0x8000000000000000LL;

const Double Math::DOUBLE_ZERO = 0.0;
const Double Math::DOUBLE_E = 2.718281828459045;
const Double Math::DOUBLE_PI = 3.141592653589793;
const Double Math::DOUBLE_NAN = 0.0 / DOUBLE_ZERO;
const Double Math::DOUBLE_MAX_VALUE = 1.79769313486231570E+308;
const Double Math::DOUBLE_MIN_VALUE = 5E-324;
const Double Math::DOUBLE_POSITIVE_INFINITY = 1.0/DOUBLE_ZERO;
const Double Math::DOUBLE_NEGATIVE_INFINITY = -1.0/DOUBLE_ZERO;
const Double Math::DOUBLE_MIN_NORMAL = 2.2250738585072014E-308;

const Int64 Math::INT64_SIGN_MASK = 0X8000000000000000LL;
const Int64 Math::INT64_EXPONENT_MASK = 0X7FF0000000000000LL;
const Int64 Math::INT64_MANTISSA_MASK= 0X000FFFFFFFFFFFFFLL;
const Int64 Math::INT64_NAN = 0X7FF8000000000000ULL;

const Float Math::FLOAT_MAX_VALUE = 3.40282346638528860e+38F;
const Float Math::FLOAT_MIN_VALUE = 1.40129846432481707e-45F;
const Float FLOAT_ZERO = 0.0F;
const Float Math::FLOAT_NAN = 0.0F/FLOAT_ZERO;
const Float Math::FLOAT_POSITIVE_INFINITY = 1.0F/FLOAT_ZERO;
const Float Math::FLOAT_NEGATIVE_INFINITY = -1.0F/FLOAT_ZERO;
const Float Math::FLOAT_MIN_NORMAL = 1.1754943508222875E-38F;

const Byte Math::INTEGER_NTZ_TABLE[] = {
    32,  0,  1, 12,  2,  6, -1, 13,   3, -1,  7, -1, -1, -1, -1, 14,
    10,  4, -1, -1,  8, -1, -1, 25,  -1, -1, -1, -1, -1, 21, 27, 15,
    31, 11,  5, -1, -1, -1, -1, -1,   9, -1, -1, 24, -1, -1, 20, 26,
    30, -1, -1, -1, -1, 23, -1, 19,  29, -1, 22, 18, 28, 17, 16, -1
};

#define CAST_PTR(type, val) ((type *)(&(val)))
#define CAST(type, val) (*CAST_PTR(type, val))

Double Math::Abs(
    /* [in] */ Double d)
{
    Int64 i = DoubleToInt64Bits(d);
    i &= 0X7FFFFFFFFFFFFFFFLL;
    return Int64BitsToDouble(i);
}

Float Math::Abs(
    /* [in] */ Float f)
{
    Int32 bits = FloatToInt32Bits(f);
    bits &= 0X7FFFFFFF;
    return Int32BitsToFloat(bits);
}

Int32 Math::Abs(
    /* [in] */ Int32 i)
{
    return i >= 0 ? i : -i;
}

Double Math::Acos(
    /* [in] */ Double d)
{
    return acos(d);
}

Double Math::Asin(
    /* [in] */ Double d)
{
    return asin(d);
}

Double Math:: Atan(
    /* [in] */ Double d)
{
    return atan(d);
}

Double Math::Atan2(
    /* [in] */ Double y,
    /* [in] */ Double x)
{
    return atan2(y, x);
}

Double Math:: Cbrt(
    /* [in] */ Double d)
{
    return cbrt(d);
}

Double Math:: Ceil(
    /* [in] */ Double d)
{
    return ceil(d);
}

Double Math::Cos(
    /* [in] */ Double d)
{
    return cos(d);
}

Double Math::Cosh(
    /* [in] */ Double d)
{
    return cosh(d);
}

Double Math:: Exp(
    /* [in] */ Double d)
{
    return exp(d);
}

Double Math::Expm1(
    /* [in] */ Double d)
{
    return expm1(d);
}

Int64 Math::Abs(
    /* [in] */Int64 i)
{
    return (i >= 0) ? i : -i;
}

Double Math::Floor(
    /* [in] */ Double d)
{
    return floor(d);
}

Double Math::Hypot(
    /* [in] */ Double x,
    /* [in] */ Double y)
{
    return hypot(x, y);
}

Double Math::IEEEremainder(
    /* [in] */ Double x,
    /* [in] */ Double y)
{
    return remainder(x, y);
}

Double Math::Log(
    /* [in] */ Double d)
{
    return log(d);
}

Double Math::Log10(
    /* [in] */ Double d)
{
    return log10(d);
}

Double Math::Log1p(
    /* [in] */ Double d)
{
    return log1p(d);
}

Double Math::Max(
    /* [in] */ Double d1,
    /* [in] */ Double d2)
{
    if (d1 > d2) {
        return d1;
    }
    if (d1 < d2) {
        return d2;
    }
    /* if either arg is DOUBLE_NAN, return DOUBLE_NAN */
    if (d1 != d2) {
        return DOUBLE_NAN;
    }
    /* max(+0.0,-0.0) == +0.0 */
    if(DoubleToRawInt64Bits(d1) != 0) {
        return d2;
    }
    return 0.0;
}

Float Math::Max(
    /* [in] */ Float f1,
    /* [in] */ Float f2)
{
    if (f1 > f2) {
        return f1;
    }
    if (f1 < f2) {
        return f2;
    }
    /* if either arg is NAN, return NAN */
    if (f1 != f2) {
        return FLOAT_NAN;
    }
    /* Max(+0.0,-0.0) == +0.0 */
    /* FloatToRawInt32Bits(0.0F) == 0*/
    if (FloatToRawInt32Bits(f1) != 0) {
        return f2;
    }
    return 0.0F;
}

Int32 Math::Max(
    /* [in] */ Int32 i1,
    /* [in] */ Int32 i2)
{
    return (i1>i2)?i1:i2;
}

Int64 Math::Max(
        /* [in] */ Int64 l1,
        /* [in] */ Int64 l2)
{
    return l1 > l2 ? l1 : l2;
}

Double Math::Min(
    /* [in] */ Double d1,
    /* [in] */ Double d2)
{
    if (d1 > d2) {
        return d2;
    }
    if (d1 < d2) {
        return d1;
    }
    /* if either arg is DOUBLE_NAN, return DOUBLE_NAN */
    if (d1 != d2) {
        return DOUBLE_NAN;
    }
    /* min(+0.0,-0.0) == -0.0 */
    /* 0X8000000000000000L == Double.doubleToRawLongBits(-0.0d) */
    if (DoubleToRawInt64Bits(d1) == (Int64)0X8000000000000000LL) {
        return -0.0;
    }
    return d2;
}

Float Math::Min(
    /* [in] */ Float f1,
    /* [in] */ Float f2)
{
    if (f1 > f2) {
        return f2;
    }
    if (f1 < f2) {
        return f1;
    }
    /* if either arg is NAN, return NAN */
    if (f1 != f2) {
        return FLOAT_NAN;
    }
    /* Min(+0.0,-0.0) == -0.0 */
    /* 0X80000000 == FloatToRawInt32Bits(-0.0F) */
    if (FloatToRawInt32Bits(f1) == 0X80000000) {
        return -0.0F;
    }
    return f2;
}

Int32 Math::Min(
    /* [in] */ Int32 i1,
    /* [in] */ Int32 i2)
{
    return (i1<i2)?i1:i2;
}

Int64 Math::Min(
        /* [in] */ Int64 l1,
        /* [in] */ Int64 l2)
{
    return l1 < l2 ? l1 : l2;
}

Double Math::Pow(
    /* [in] */ Double x,
    /* [in] */ Double y)
{
    return pow(x, y);
}

Double Math::Rint(
    /* [in] */ Double d)
{
    return rint(d);
}

Int64 Math::Round(
    /* [in] */ Double d)
{
    // check for NAN
    if (IsNAN(d)) {
        return 0;
    }
    return (Int64)Floor(d + 0.5);
}

Int32 Math::Round(
    /* [in] */ Float f)
{
    // check for NAN
    if (f != f) {
        return 0;
    }
    return (int)Floor(f + 0.5F);
}

Double Math::Signum(
    /* [in] */ Double d)
{
    if (IsNAN(d)) {
        return DOUBLE_NAN;
    }
    Double sig = d;
    if (d > 0) {
        sig = 1.0;
    } else if (d < 0) {
        sig = -1.0;
    }
    return sig;
}

Double Math::Sin(
    /* [in] */ Double d)
{
    return sin(d);
}

Double Math:: Sinh(
    /* [in] */ Double d)
{
    return sinh(d);
}

Double Math:: Sqrt(
    /* [in] */ Double d)
{
    return sqrt(d);
}

Double Math:: Tan(
    /* [in] */ Double d)
{
    return tan(d);
}

Double Math::Tanh(
    /* [in] */ Double d)
{
    return tanh(d);
}

Double Math::Random()
{
    srand((Int32)time(0));

    Int64 i = rand();

    i <<= 32;
    i += rand();

    Double d = CAST(Double, i);

    if(IsNAN(d)) return Random();

    return d;
}

Double Math::ToRadians(
    /* [in] */ Double angdeg)
{
    return angdeg / 180 * DOUBLE_PI;
}

Double Math::ToDegrees(
    /* [in] */ Double angrad)
{
    return angrad * 180 / DOUBLE_PI;
}

Double Math::Ulp(
    /* [in] */ Double d)
{
    if (IsInfinite(d)) {
        return DOUBLE_POSITIVE_INFINITY;
    } else if (d == INT64_MANTISSA_MASK || d == -INT64_MANTISSA_MASK) {

        return pow(2.0, 971);
    }
    d = Abs(d);
    return Nextafter(d, (Double)INT64_MANTISSA_MASK) - d;
}

Double Math::Nextafter(
    /* [in] */ Double x,
    /* [in] */ Double y)
{
    return nextafter(x, y);
}

Double Math::CopySign(
    /* [in] */ Double magnitude,
    /* [in] */ Double sign)
{
    Int64 magnitudeBits = DoubleToRawInt64Bits(magnitude);
    Int64 signBits = DoubleToRawInt64Bits(sign);
    magnitudeBits = (magnitudeBits & ~INT64_SIGN_MASK) | (signBits & INT64_SIGN_MASK);
    return Int64BitsToDouble(magnitudeBits);
}

Double Math::GetExponent(
    /* [in] */ Double d)
{
    Int64 bits = DoubleToRawInt64Bits(d);
    bits = (bits & INT64_EXPONENT_MASK) >> INT32_MANTISSA_BITS;
    return (Int32) bits - INT32_EXPONENT_BIAS;
}

Double Math::NextAfter(
    /* [in] */ Double start,
    /* [in] */ Double direction)
{
    if (start == 0 && direction == 0) {
        return direction;
    }
    return nextafter(start, direction);
}

Double Math::NextUp(
    /* [in] */ Double d)
{
    if (IsNAN(d)) {
        return DOUBLE_NAN;
    }
    if (d == DOUBLE_POSITIVE_INFINITY) {
        return d;
    }
    if (d == 0) {
        return DOUBLE_MIN_VALUE;
    } else if (d > 0) {
        return Int64BitsToDouble(DoubleToInt64Bits(d) + 1);
    } else {
        return Int64BitsToDouble(DoubleToInt64Bits(d) - 1);
    }
}

Double Math::Scalb(
    /* [in] */ Double d,
    /* [in] */ Int32 scaleFactor)
{
    if (IsNAN(d) || IsInfinite(d) || d == 0) {
        return d;
    }
    // change double to long for calculation
    Int64 bits = DoubleToInt64Bits(d);
    // the sign of the results must be the same of given d
    Int64 sign = bits & INT64_SIGN_MASK;
    // calculates the factor of the result
    Int64 factor = ((bits & INT64_EXPONENT_MASK) >> INT32_EXPONENT_BITS)
        - INT32_EXPONENT_BIAS + scaleFactor;

    // calculates the factor of sub-normal values
    Int64 subNormalFactor = NumberOfLeadingZeros(bits & ~INT64_SIGN_MASK)
        - INT32_NON_MANTISSA_BITS;
    if (subNormalFactor < 0) {
        // not sub-normal values
        subNormalFactor = 0;
    } else {
        factor = factor - subNormalFactor;
    }
    if (factor > INT32_MAX_EXPONENT) {
        return (d > 0 ? DOUBLE_POSITIVE_INFINITY : DOUBLE_NEGATIVE_INFINITY);
    }

    Int64 result;
    // if result is a sub-normal
    if (factor <= -INT32_EXPONENT_BIAS) {
        // the number of digits that shifts
        Int64 digits = factor + INT32_EXPONENT_BIAS + subNormalFactor;
        if (Abs(d) < DOUBLE_MIN_NORMAL) {
            // origin d is already sub-normal
            result = ShiftInt64Bits(bits & INT64_MANTISSA_MASK, digits);
        } else {
            // origin d is not sub-normal, change mantissa to sub-normal
            result = ShiftInt64Bits((bits & INT64_MANTISSA_MASK) | 0X0010000000000000LL, digits - 1);
        }
    } else {
        if (Abs(d) >= DOUBLE_MIN_NORMAL) {
            // common situation
            result = ((factor + INT32_EXPONENT_BIAS) << INT32_EXPONENT_BITS)
                | (bits & INT64_MANTISSA_MASK);
        } else {
            // origin d is sub-normal, change mantissa to normal style
            result = ((factor + INT32_EXPONENT_BIAS) << INT32_EXPONENT_BITS)
                | ((bits << (subNormalFactor + 1)) & INT64_MANTISSA_MASK);
        }
    }
    return Int64BitsToDouble(result | sign);
}

Int32 Math::ShiftInt32Bits(
    /* [in] */ Int32 bits,
    /* [in] */ Int32 digits)
{
    if (digits > 0) {
        return bits << digits;
    }
    // change it to positive
    Int32 absdigits = -digits;
    if (!(NumberOfLeadingZeros(bits & ~INT64_SIGN_MASK) <= (32 - absdigits))) {
        return 0;
    }
    Int32 ret = bits >> absdigits;
    Boolean halfbit = ((bits >> (absdigits - 1)) & 0X1) == 1;
    if (halfbit) {
        if (NumberOfTrailingZeros(bits) < (absdigits - 1)) {
            ret = ret + 1;
        }
        if (NumberOfTrailingZeros(bits) == (absdigits - 1)) {
            if ((ret & 0X1) == 1) {
                ret = ret + 1;
            }
        }
    }
    return ret;
}

Int64 Math::DoubleToRawInt64Bits(
    /* [in] */ Double d)
{
    return CAST(Int64, d);
}

Int64 Math::DoubleToInt64Bits(
    /* [in] */ Double d)
{
    return (IsNAN(d)) ? INT64_NAN : CAST(Int64, d);
}

Double Math::Int64BitsToDouble(
    /* [in] */ Int64 i)
{
    return CAST(Double, i);
}

Int32 Math::NumberOfTrailingZeros(
    /* [in] */ Int32 i)
{
    // Seal's algorithm - Hacker's Delight 5-18
    // BEGIN android-changed - Harmony version should be one-liner in comment below
    i &= -i;
    i = (i <<  4) + i;    // x *= 17
    i = (i <<  6) + i;    // x *= 65
    i = (i << 16) - i;    // x *= 65535
    return INTEGER_NTZ_TABLE[(Int32)i >> 26]; // END android-changed
}

Int32 Math::NumberOfTrailingZeros(
    /* [in] */ Int64 v)
{
    Int32 low = (Int32)v;
    return low !=0 ? NumberOfTrailingZeros(low)
        : 32 + NumberOfTrailingZeros((Int32)((UInt64)v >> 32));
}

Int32 Math::NumberOfLeadingZeros(
    /* [in] */ Int64 v)
{
    // After Hacker's Delight, Figure 5-6
    if (v < 0) {
        return 0;
    }
    if (v == 0) {
        return 64;
    }
    // On a 64-bit VM, the two previous tests should probably be replaced by
    // if (v <= 0) return ((Int32) (~v >> 57)) & 64;

    Int32 n = 1;
    Int32 i = (Int32)((UInt64)v >> 32);
    if (i == 0) {
        n +=  32;
        i = (Int32) v;
    }
    if ((UInt32)i >> 16 == 0) {
        n +=  16;
        i <<= 16;
    }
    if ((Int32)i >> 24 == 0) {
        n +=  8;
        i <<= 8;
    }
    if ((Int32)i >> 28 == 0) {
        n +=  4;
        i <<= 4;
    }
    if ((Int32)i >> 30 == 0) {
        n +=  2;
        i <<= 2;
    }
    return n - ((Int32)i >> 31);
}

Int32 Math::NumberOfLeadingZeros(
    /* [in] */ Int32 i)
{
    // Hacker's Delight, Figure 5-6
    if (i <= 0) {
        return (~i >> 26) & 32;
    }
    Int32 n = 1;
    if (i >> 16 == 0) {
        n +=  16;
        i <<= 16;
    }
    if (i >> 24 == 0) {
        n +=  8;
        i <<= 8;
    }
    if (i >> 28 == 0) {
        n +=  4;
        i <<= 4;
    }
    if (i >> 30 == 0) {
        n +=  2;
        i <<= 2;
    }
    //return n - (i >>> 31);
    return n - ((Int32)i >> 31);
}

Float Math::Signum(
    /* [in] */ Float f)
{
    //check for NAN
    if(IsNAN(f)){
        return FLOAT_NAN;
    }
    Float sig = f;
    if(f > 0){
        sig = 1.0F;
    }else if(f < 0){
        sig = -1.0F;
    }
    return sig;
}

Float Math::Ulp(
   /* [in] */ Float f)
{
    //special cases
    if(IsNAN(f)){
        return FLOAT_NAN;
    }else if(IsInfinite(f)){
        return FLOAT_POSITIVE_INFINITY;
    }else if (f == FLOAT_MAX_VALUE || f == -FLOAT_MAX_VALUE) {
        return (Float) pow(2, 104);
    }
    f = Abs(f);
    return Nextafterf(f, FLOAT_MAX_VALUE) - f;
}

Float Math::CopySign(
    /* [in] */ Float magnitude,
    /* [in] */ Float sign)
{
    Int32 magnitudeBits = FloatToRawInt32Bits(magnitude);
    Int32 signBits = FloatToRawInt32Bits(sign);
    magnitudeBits = (magnitudeBits & ~FLOAT_SIGN_MASK) | (signBits & FLOAT_SIGN_MASK);
    return Int32BitsToFloat(magnitudeBits);
}

Float Math::NextAfter(
    /* [in] */ Float start,
    /* [in] */ Double direction)
{
    if(IsNAN(start) || IsNAN(direction)){
        return FLOAT_NAN;
    }
    if(start == 0 && direction == 0){
        return (Float)direction;
    }
    if ((start == FLOAT_MIN_VALUE && direction < start)
                 || (start == -FLOAT_MIN_VALUE && direction > start)) {
            return ((start > 0) ? 0.0F : -0.0F);
    }
    if (IsInfinite(start) && (direction != start)) {
          return (start > 0 ? FLOAT_MAX_VALUE : -FLOAT_MAX_VALUE);
    }
    if ((start == FLOAT_MAX_VALUE && direction > start)
                 || (start == -FLOAT_MAX_VALUE && direction < start)) {
            return (start > 0 ? FLOAT_POSITIVE_INFINITY : FLOAT_NEGATIVE_INFINITY);
    }
    if (direction > start) {
        if (start > 0) {
            return Int32BitsToFloat(FloatToInt32Bits(start) + 1);
        }
        if (start < 0) {
            return Int32BitsToFloat(FloatToInt32Bits(start) - 1);
        }
        return +FLOAT_MIN_VALUE;
    }
    if (direction < start) {
        if (start > 0) {
            return Int32BitsToFloat(FloatToInt32Bits(start) - 1);
        }
        if (start < 0) {
            return Int32BitsToFloat(FloatToInt32Bits(start) + 1);
        }
        return -FLOAT_MIN_VALUE;
    }
    return (Float)direction;
}

Float Math::NextUp(
    /* [in] */ Float f)
{
    if(IsNAN(f)){
        return FLOAT_NAN;
    }
    if(f == FLOAT_POSITIVE_INFINITY){
        return FLOAT_POSITIVE_INFINITY;
    }
    if(f == 0){
        return FLOAT_MIN_VALUE;
    }else if(f > 0){
        return Int32BitsToFloat(FloatToInt32Bits(f) + 1);
    }else{
        return Int32BitsToFloat(FloatToInt32Bits(f) - 1);
    }
}

Float Math::Scalb(
    /* [in] */ Float d,
    /* [in] */ Int32 scaleFactor)
{
    if ((IsNAN(d)) || IsInfinite(d) || d == 0) {
        return d;
    }
    Int32 bits = FloatToInt32Bits(d);
    Int32 sign = bits & FLOAT_SIGN_MASK;
    Int32 factor = ((bits & FLOAT_EXPONENT_MASK) >> FLOAT_MANTISSA_BITS)
                - FLOAT_EXPONENT_BIAS + scaleFactor;
    // calcutes the factor of sub-normal values
    Int32 subNormalFactor = NumberOfLeadingZeros((Int32)(bits & ~FLOAT_SIGN_MASK))
                - FLOAT_NON_MANTISSA_BITS;
    if (subNormalFactor < 0) {
    // not sub-normal values
        subNormalFactor = 0;
    } else {
           factor = factor - subNormalFactor;
    }
    if (factor > FLOAT_MAX_EXPONENT) {
           return (d > 0 ? FLOAT_POSITIVE_INFINITY : FLOAT_NEGATIVE_INFINITY);
    }

    Int32 result;
    // if result is a sub-normal
    if (factor <= -FLOAT_EXPONENT_BIAS) {
        // the number of digits that shifts
        Int32 digits = factor + FLOAT_EXPONENT_BIAS + subNormalFactor;
        if (Abs(d) < FLOAT_MIN_NORMAL) {
            // origin d is already sub-normal
            result = ShiftInt32Bits(bits & FLOAT_MANTISSA_MASK, digits);
        } else {
            // origin d is not sub-normal, change mantissa to sub-normal
            result = ShiftInt32Bits((bits & FLOAT_MANTISSA_MASK) | 0X00800000, digits - 1);
        }
    } else {
        if (Abs(d) >= FLOAT_MIN_NORMAL) {
            // common situation
            result = ((factor + FLOAT_EXPONENT_BIAS) << FLOAT_MANTISSA_BITS)
                        | (bits & FLOAT_MANTISSA_MASK);
        } else {
            // origin d is sub-normal, change mantissa to normal style
            result = ((factor + FLOAT_EXPONENT_BIAS) << FLOAT_MANTISSA_BITS)
                        | ((bits << (subNormalFactor + 1)) & FLOAT_MANTISSA_MASK);
        }
    }
    return Int32BitsToFloat(result | sign);

}

Boolean Math::IsNAN(
    /* [in] */ Float f)
{
    return f != f;
}

Boolean Math::IsNAN(
    /* [in] */ Double d)
{
    return d != d;
}

Boolean Math::IsInfinite(
    /* [in] */ Float f)
{
    return (f == FLOAT_POSITIVE_INFINITY) || (f == FLOAT_NEGATIVE_INFINITY);
}

Boolean Math::IsInfinite(
    /* [in] */ Double d)
{
    return (d == DOUBLE_POSITIVE_INFINITY) || (d == DOUBLE_NEGATIVE_INFINITY);
}

Int32 Math::FloatToRawInt32Bits(
    /* [in] */ Float value)
{
    return CAST(Int32, value);
}

Int32 Math::FloatToInt32Bits(
    /* [in] */ Float value)
{
    return IsNAN(value) ? INT32_NAN : CAST(Int32,value);
}

Float Math::Int32BitsToFloat(
    /* [in] */ Int32 value)
{
    return CAST(Float, value);
}

Float Math::Nextafterf(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    return nextafterf(x,y);
}

Int64 Math::ShiftInt64Bits(
    /* [in] */ Int64 bits,
    /* [in] */ Int64 digits)
{
    if (digits > 0) {
        return bits << digits;
    }
    // change it to positive
    Int64 absdigits = -digits;
    if (!(NumberOfLeadingZeros((Int64)(bits & ~INT64_SIGN_MASK)) <= (64 - absdigits))) {
        return 0;
    }
    Int64 ret = bits >> absdigits;
    Boolean halfbit = ((bits >> (absdigits - 1)) & 0X1) == 1;
    if (halfbit) {
        // some bits will remain after shifting, calculates its carry
        // subnormal
        if (NumberOfTrailingZeros((Int64)bits) < (absdigits - 1)) {
             ret = ret + 1;
        }
        if (NumberOfTrailingZeros((Int64)bits) == (absdigits - 1)) {
             if ((ret & 0X1) == 1) {
                 ret = ret + 1;
             }
        }
    }
    return ret;
}
} //Core
} //Elastos
