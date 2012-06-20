
#ifndef __MATH_H__
#define __MATH_H__

#include <elastos.h>

using namespace Elastos;

namespace Elastos {
namespace System {
/*!
 * @brief Provides constants and static methods for trigonometric, logarithmic,
 * and other common mathematical functions.
 */
class Math
{
public:
    static const Int32 INT32_EXPONENT_BIAS;
    static const Int32 INT32_EXPONENT_BITS;
    static const Int32 INT32_MANTISSA_BITS;
    static const Int32 INT32_NON_MANTISSA_BITS;
    static const Int32 INT32_NAN;

    static const Int32 FLOAT_EXPONENT_BIAS;
    static const Int32 FLOAT_MANTISSA_BITS;
    static const Int32 FLOAT_NON_MANTISSA_BITS;
    static const Int32 FLOAT_SIGN_MASK;
    static const Int32 FLOAT_EXPONENT_MASK;
    static const Int32 FLOAT_MANTISSA_MASK;

    static const Int64 INT64_SIGN_MASK;
    static const Int64 INT64_EXPONENT_MASK;
    static const Int64 INT64_MANTISSA_MASK;
    static const Int64 INT64_NAN;

    static const Double DOUBLE_ZERO;
    static const Double DOUBLE_E;
    static const Double DOUBLE_PI;

    /**
     * Maximum exponent that a finite value of the {@code double} type may have.
     * Equal to {@code Math.getExponent(Double.MAX_VALUE)}.
     *
     * @since 1.6
     */
    static const Int32 INT32_MAX_EXPONENT;

    /*!
     * @brief Constant for the maximum @c Int32 value, @f$ (2^31-1) @f$ .
     */
    static const Int32 INT32_MAX_VALUE;

    /**
     * Constant for the minimum {@code int} value, -2<sup>31</sup>.
     */
    static const Int32 INT32_MIN_VALUE;

    /*!
     * @brief Maximum exponent that a finite value of the @c Float type may have.
     * Equal to @c Math.getExponent(Float.MAX_VALUE) .
     */
    static const Int32 FLOAT_MAX_EXPONENT;

    /**
     * Constant for the Not-a-Number (NaN) value of the {@code double} type.
     */
    static const Double DOUBLE_NAN;
    /**
     * Constant for the maximum {@code double} value, (2 - 2<sup>-52</sup>) *
     * 2<sup>1023</sup>.
     */
    static const Double DOUBLE_MAX_VALUE;
    /**
     * Constant for the minimum {@code double} value, 2<sup>-1074</sup>.
     */
    static const Double DOUBLE_MIN_VALUE;
    /**
     * Constant for the positive infinity value of the {@code double} type.
     */
    static const Double DOUBLE_POSITIVE_INFINITY;
    /**
     * Constant for the negative infinity value of the {@code double} type.
     */
    static const Double DOUBLE_NEGATIVE_INFINITY;
    /**
     * Constant for the smallest positive normal value of the {@code double} type.
     *
     * @since 1.6
     */
    static const Double DOUBLE_MIN_NORMAL;

    /*!
     * @brief Constant for the maximum @c Float value, @f$ {(2 - 2^{-23}) * 2^127} @f$ .
     */
    static const Float FLOAT_MAX_VALUE;

    /*!
     * @brief Constant for the minimum @c Float value, @f$ {2^{-149}} @f$ .
     */
    static const Float FLOAT_MIN_VALUE;

    /*!
     * @brief Constant for the Not-a-Number (NAN) value of the @c Float type.
     */
    static const Float FLOAT_NAN;

    /*!
     * @brief Constant for the positive infinity value of the @c Float type.
     */
    static const Float FLOAT_POSITIVE_INFINITY;

    /*!
     * @brief Constant for the negative infinity value of the @c Float type.
     */
    static const Float FLOAT_NEGATIVE_INFINITY;

    /*!
     * @brief Constant for the smallest positive normal value of the @c Float type.
     */
    static const Float FLOAT_MIN_NORMAL;

private:
    /*!
     * @brief Table for Seal's algorithm for Number of Trailing Zeros. Hacker's Delight
     * online, Figure 5-18 (http://www.hackersdelight.org/revisions.pdf)
     * The entries whose value is -1 are never referenced.
     */
    static const Byte INTEGER_NTZ_TABLE[];

public:
    /*!
      * @brief Returns the absolute value of the argument.
      *
      * <p>
      * Special cases:
      * <ul>
      * <li>{Abs(-0.0) = +0.0}
      * <li>{Abs(+infinity) = +infinity}
      * <li>{Abs(-infinity) = +infinity}
      * <li>{Abs(NAN) = NAN}
      * </ul>
      *
      * @param f @b [in] the value whose absolute value has to be computed.
      * @return the argument if it is positive, otherwise the negation of the argument.
      */
    static CARAPI_(Float) Abs(
        /* [in] */ Float f);

    /*!
    * Returns the absolute value of the argument.
    * <p>
    * Special cases:
    * <ul>
    * <li>{@code abs(-0.0) = +0.0}</li>
    * <li>{@code abs(+infinity) = +infinity}</li>
    * <li>{@code abs(-infinity) = +infinity}</li>
    * <li>{@code abs(NaN) = NaN}</li>
    * </ul>
    *
    * @param d
    *            the value whose absolute value has to be computed.
    * @return the absolute value of the argument.
    */
    static CARAPI_(Double) Abs(
        /* [in] */ Double d);

    /**
     * Returns the absolute value of the argument.
     * <p>
     * If the argument is {@code Integer.MIN_VALUE}, {@code Integer.MIN_VALUE}
     * is returned.
     *
     * @param i
     *            the value whose absolute value has to be computed.
     * @return the argument if it is positive, otherwise the negation of the
     *         argument.
     */

    static CARAPI_(Int32) Abs(
        /* [in] */ Int32 i);

    /*!
     * @brief Returns the absolute value of the argument. If the argument is @c Long.MIN_VALUE,
     * @c Long.MIN_VALUE is returned.
     *
     * @param i @b [in] the value whose absolute value has to be computed.
     * @return the argument if it is positive, otherwise the negation of the argument.
     */
    static CARAPI_(Int64) Abs(
        /* [in] */ Int64 i);

    /**
     * Returns the closest double approximation of the arc cosine of the
     * argument within the range {@code [0..pi]}. The returned result is within
     * 1 ulp (unit in the last place) of the real result.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code acos((anything > 1) = NaN}</li>
     * <li>{@code acos((anything < -1) = NaN}</li>
     * <li>{@code acos(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value to compute arc cosine of.
     * @return the arc cosine of the argument.
     */
    static CARAPI_(Double) Acos(
        /* [in] */ Double d);

    /**
     * Returns the closest double approximation of the arc sine of the argument
     * within the range {@code [-pi/2..pi/2]}. The returned result is within 1
     * ulp (unit in the last place) of the real result.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code asin((anything > 1)) = NaN}</li>
     * <li>{@code asin((anything < -1)) = NaN}</li>
     * <li>{@code asin(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value whose arc sine has to be computed.
     * @return the arc sine of the argument.
     */
    static CARAPI_(Double) Asin(
        /* [in] */ Double d);

    /**
     * Returns the closest double approximation of the arc tangent of the
     * argument within the range {@code [-pi/2..pi/2]}. The returned result is
     * within 1 ulp (unit in the last place) of the real result.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code atan(+0.0) = +0.0}</li>
     * <li>{@code atan(-0.0) = -0.0}</li>
     * <li>{@code atan(+infinity) = +pi/2}</li>
     * <li>{@code atan(-infinity) = -pi/2}</li>
     * <li>{@code atan(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value whose arc tangent has to be computed.
     * @return the arc tangent of the argument.
     */
    static CARAPI_(Double) Atan(
        /* [in] */ Double d);

    /**
     * Returns the closest double approximation of the arc tangent of {@code
     * y/x} within the range {@code [-pi..pi]}. This is the angle of the polar
     * representation of the rectangular coordinates (x,y). The returned result
     * is within 2 ulps (units in the last place) of the real result.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code atan2((anything), NaN ) = NaN;}</li>
     * <li>{@code atan2(NaN , (anything) ) = NaN;}</li>
     * <li>{@code atan2(+0.0, +(anything but NaN)) = +0.0}</li>
     * <li>{@code atan2(-0.0, +(anything but NaN)) = -0.0}</li>
     * <li>{@code atan2(+0.0, -(anything but NaN)) = +pi}</li>
     * <li>{@code atan2(-0.0, -(anything but NaN)) = -pi}</li>
     * <li>{@code atan2(+(anything but 0 and NaN), 0) = +pi/2}</li>
     * <li>{@code atan2(-(anything but 0 and NaN), 0) = -pi/2}</li>
     * <li>{@code atan2(+(anything but infinity and NaN), +infinity)} {@code =}
     * {@code +0.0}</li>
     * <li>{@code atan2(-(anything but infinity and NaN), +infinity)} {@code =}
     * {@code -0.0}</li>
     * <li>{@code atan2(+(anything but infinity and NaN), -infinity) = +pi}</li>
     * <li>{@code atan2(-(anything but infinity and NaN), -infinity) = -pi}</li>
     * <li>{@code atan2(+infinity, +infinity ) = +pi/4}</li>
     * <li>{@code atan2(-infinity, +infinity ) = -pi/4}</li>
     * <li>{@code atan2(+infinity, -infinity ) = +3pi/4}</li>
     * <li>{@code atan2(-infinity, -infinity ) = -3pi/4}</li>
     * <li>{@code atan2(+infinity, (anything but,0, NaN, and infinity))} {@code
     * =} {@code +pi/2}</li>
     * <li>{@code atan2(-infinity, (anything but,0, NaN, and infinity))} {@code
     * =} {@code -pi/2}</li>
     * </ul>
     *
     * @param y
     *            the numerator of the value whose atan has to be computed.
     * @param x
     *            the denominator of the value whose atan has to be computed.
     * @return the arc tangent of {@code y/x}.
     */
    static CARAPI_(Double) Atan2(
        /* [in] */ Double y,
        /* [in] */ Double x);

    /**
     * Returns the closest double approximation of the cube root of the
     * argument.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code cbrt(+0.0) = +0.0}</li>
     * <li>{@code cbrt(-0.0) = -0.0}</li>
     * <li>{@code cbrt(+infinity) = +infinity}</li>
     * <li>{@code cbrt(-infinity) = -infinity}</li>
     * <li>{@code cbrt(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value whose cube root has to be computed.
     * @return the cube root of the argument.
     */
    static CARAPI_(Double) Cbrt(
        /* [in] */ Double d);

    /**
     * Returns the double conversion of the most negative (closest to negative
     * infinity) integer value which is greater than the argument.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code ceil(+0.0) = +0.0}</li>
     * <li>{@code ceil(-0.0) = -0.0}</li>
     * <li>{@code ceil((anything in range (-1,0)) = -0.0}</li>
     * <li>{@code ceil(+infinity) = +infinity}</li>
     * <li>{@code ceil(-infinity) = -infinity}</li>
     * <li>{@code ceil(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value whose closest integer value has to be computed.
     * @return the ceiling of the argument.
     */
    static CARAPI_(Double) Ceil(
        /* [in] */ Double d);

    /**
     * Returns the closest double approximation of the cosine of the argument.
     * The returned result is within 1 ulp (unit in the last place) of the real
     * result.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code cos(+infinity) = NaN}</li>
     * <li>{@code cos(-infinity) = NaN}</li>
     * <li>{@code cos(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the angle whose cosine has to be computed, in radians.
     * @return the cosine of the argument.
     */
    static CARAPI_(Double) Cos(
        /* [in] */ Double d);

    /**
     * Returns the closest double approximation of the hyperbolic cosine of the
     * argument. The returned result is within 2.5 ulps (units in the last
     * place) of the real result.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code cosh(+infinity) = +infinity}</li>
     * <li>{@code cosh(-infinity) = +infinity}</li>
     * <li>{@code cosh(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value whose hyperbolic cosine has to be computed.
     * @return the hyperbolic cosine of the argument.
     */
    static CARAPI_(Double) Cosh(
        /* [in] */ Double d);

    /**
     * Returns the closest double approximation of the raising "e" to the power
     * of the argument. The returned result is within 1 ulp (unit in the last
     * place) of the real result.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code exp(+infinity) = +infinity}</li>
     * <li>{@code exp(-infinity) = +0.0}</li>
     * <li>{@code exp(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value whose exponential has to be computed.
     * @return the exponential of the argument.
     */
    static CARAPI_(Double) Exp(
        /* [in] */ Double d);

    /**
     * Returns the closest double approximation of <i>{@code e}</i><sup> {@code
     * d}</sup>{@code - 1}. If the argument is very close to 0, it is much more
     * accurate to use {@code expm1(d)+1} than {@code exp(d)} (due to
     * cancellation of significant digits). The returned result is within 1 ulp
     * (unit in the last place) of the real result.
     * <p>
     * For any finite input, the result is not less than -1.0. If the real
     * result is within 0.5 ulp of -1, -1.0 is returned.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code expm1(+0.0) = +0.0}</li>
     * <li>{@code expm1(-0.0) = -0.0}</li>
     * <li>{@code expm1(+infinity) = +infinity}</li>
     * <li>{@code expm1(-infinity) = -1.0}</li>
     * <li>{@code expm1(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value to compute the <i>{@code e}</i><sup>{@code d} </sup>
     *            {@code - 1} of.
     * @return the <i>{@code e}</i><sup>{@code d}</sup>{@code - 1} value of the
     *         argument.
     */
    static CARAPI_(Double) Expm1(
        /* [in] */ Double d);

    /*!
     * @brief Returns the Double conversion of the most positive (closest to positive
     * infinity) integer value which is less than the argument.
     *
     * <p>
     * Special cases:
     * <ul>
     * <li>{Floor(+0.0) = +0.0}
     * <li>{Floor(-0.0) = -0.0}
     * <li>{Floor(+infinity) = +infinity}
     * <li>{Floor(-infinity) = -infinity}
     * <li>{Floor(NAN) = NAN}
     * </ul>
     * @param d @b [in] the value whose closest integer value has to be computed.
     * @return the floor of the argument.
     */
    static CARAPI_(Double) Floor(
        /* [in] */ Double d);

    /**
     * Returns {@code sqrt(}<i>{@code x}</i><sup>{@code 2}</sup>{@code +} <i>
     * {@code y}</i><sup>{@code 2}</sup>{@code )}. The final result is without
     * medium underflow or overflow. The returned result is within 1 ulp (unit
     * in the last place) of the real result. If one parameter remains constant,
     * the result should be semi-monotonic.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code hypot(+infinity, (anything including NaN)) = +infinity}</li>
     * <li>{@code hypot(-infinity, (anything including NaN)) = +infinity}</li>
     * <li>{@code hypot((anything including NaN), +infinity) = +infinity}</li>
     * <li>{@code hypot((anything including NaN), -infinity) = +infinity}</li>
     * <li>{@code hypot(NaN, NaN) = NaN}</li>
     * </ul>
     *
     * @param x
     *            a double number.
     * @param y
     *            a double number.
     * @return the {@code sqrt(}<i>{@code x}</i><sup>{@code 2}</sup>{@code +}
     *         <i> {@code y}</i><sup>{@code 2}</sup>{@code )} value of the
     *         arguments.
     */
    static CARAPI_(Double) Hypot(
        /* [in] */ Double x,
        /* [in] */ Double y);

    /**
     * Returns the remainder of dividing {@code x} by {@code y} using the IEEE
     * 754 rules. The result is {@code x-round(x/p)*p} where {@code round(x/p)}
     * is the nearest integer (rounded to even), but without numerical
     * cancellation problems.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code IEEEremainder((anything), 0) = NaN}</li>
     * <li>{@code IEEEremainder(+infinity, (anything)) = NaN}</li>
     * <li>{@code IEEEremainder(-infinity, (anything)) = NaN}</li>
     * <li>{@code IEEEremainder(NaN, (anything)) = NaN}</li>
     * <li>{@code IEEEremainder((anything), NaN) = NaN}</li>
     * <li>{@code IEEEremainder(x, +infinity) = x } where x is anything but
     * +/-infinity</li>
     * <li>{@code IEEEremainder(x, -infinity) = x } where x is anything but
     * +/-infinity</li>
     * </ul>
     *
     * @param x
     *            the numerator of the operation.
     * @param y
     *            the denominator of the operation.
     * @return the IEEE754 floating point reminder of of {@code x/y}.
     */
    static CARAPI_(Double) IEEEremainder(
        /* [in] */ Double x,
        /* [in] */ Double y);

    /**
     * Returns the closest double approximation of the natural logarithm of the
     * argument. The returned result is within 1 ulp (unit in the last place) of
     * the real result.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code log(+0.0) = -infinity}</li>
     * <li>{@code log(-0.0) = -infinity}</li>
     * <li>{@code log((anything < 0) = NaN}</li>
     * <li>{@code log(+infinity) = +infinity}</li>
     * <li>{@code log(-infinity) = NaN}</li>
     * <li>{@code log(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value whose log has to be computed.
     * @return the natural logarithm of the argument.
     */
    static CARAPI_(Double) Log(
        /* [in] */ Double d);

    /**
     * Returns the closest double approximation of the base 10 logarithm of the
     * argument. The returned result is within 1 ulp (unit in the last place) of
     * the real result.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code log10(+0.0) = -infinity}</li>
     * <li>{@code log10(-0.0) = -infinity}</li>
     * <li>{@code log10((anything < 0) = NaN}</li>
     * <li>{@code log10(+infinity) = +infinity}</li>
     * <li>{@code log10(-infinity) = NaN}</li>
     * <li>{@code log10(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value whose base 10 log has to be computed.
     * @return the natural logarithm of the argument.
     */
    static CARAPI_(Double) Log10(
        /* [in] */ Double d);

    /**
     * Returns the closest double approximation of the natural logarithm of the
     * sum of the argument and 1. If the argument is very close to 0, it is much
     * more accurate to use {@code log1p(d)} than {@code log(1.0+d)} (due to
     * numerical cancellation). The returned result is within 1 ulp (unit in the
     * last place) of the real result and is semi-monotonic.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code log1p(+0.0) = +0.0}</li>
     * <li>{@code log1p(-0.0) = -0.0}</li>
     * <li>{@code log1p((anything < 1)) = NaN}</li>
     * <li>{@code log1p(-1.0) = -infinity}</li>
     * <li>{@code log1p(+infinity) = +infinity}</li>
     * <li>{@code log1p(-infinity) = NaN}</li>
     * <li>{@code log1p(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value to compute the {@code ln(1+d)} of.
     * @return the natural logarithm of the sum of the argument and 1.
     */
    static CARAPI_(Double) Log1p(
        /* [in] */ Double d);

    /*!
     * @brief Returns the most positive (closest to positive infinity) of the two
     * arguments.
     *
     * <p>
     * Special cases:
     * <ul>
     * <li>{Max(NAN,(anything)) = NAN}
     * <li>{Max((anything),NaN) = NAN}
     * <li>{Max(+0.0,-0.0) = +0.0}
     * <li>{Max(-0.0,+0.0) = +0.0}
     * </ul>
     * @param f1 @b [in] the first argument.
     * @param f2 @b [in] the second argument.
     * @return the larger of @c f1 and @c f2.
     */
    static CARAPI_(Float) Max(
        /* [in] */ Float f1,
        /* [in] */ Float f2);
    /**
     * Returns the most positive (closest to positive infinity) of the two
     * arguments.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code max(NaN, (anything)) = NaN}</li>
     * <li>{@code max((anything), NaN) = NaN}</li>
     * <li>{@code max(+0.0, -0.0) = +0.0}</li>
     * <li>{@code max(-0.0, +0.0) = +0.0}</li>
     * </ul>
     *
     * @param d1
     *            the first argument.
     * @param d2
     *            the second argument.
     * @return the larger of {@code d1} and {@code d2}.
     */
    static CARAPI_(Double) Max(
        /* [in] */ Double d1,
        /* [in] */ Double d2);
    /*!
     * @brief Returns the most positive (closest to positive infinity) of the two
     * arguments.
     *
     * @param i1 @b [in] the first argument.
     * @param i2 @b [in] the second argument.
     * @return the larger of @c i1 and @c i2.
     */
    static CARAPI_(Int32) Max(
        /* [in] */ Int32 i1,
        /* [in] */ Int32 i2);

    /*!
     * @brief Returns the most positive (closest to positive infinity) of the two
     * arguments.
     *
     * @param l1 @b [in] the first argument.
     * @param l2 @b [in] the second argument.
     * @return the larger of @c l1 and @c l2.
     */
    static CARAPI_(Int64) Max(
        /* [in] */ Int64 l1,
        /* [in] */ Int64 l2);

    /*!
     * @brief Returns the most negative (closest to negative infinity) of the two
     * arguments.
     *
     * <p>
     * Special cases:
     * <ul>
     * <li>{Min(NAN,(anything)) = NAN}
     * <li>{Min((anything),NAN) = NAN}
     * <li>{Min(+0.0,-0.0) = -0.0}
     * <li>{Min(-0.0,+0.0) = -0.0}
     * </ul>
     *
     * @param f1 @b [in] the first argument.
     * @param f2 @b [in] the second argument.
     * @return the smaller of @c f1 and @c f2.
     */
    static CARAPI_(Float) Min(
        /* [in] */ Float f1,
        /* [in] */ Float f2);

    /*!
     * @brief Returns the most negative (closest to negative infinity) of the two
     * arguments.
     *
     * @param i1 @b [in] the first argument.
     * @param i2 @b [in] the second argument.
     * @return the smaller of @c i1 and @c i2.
     */
    static CARAPI_(Int32) Min(
        /* [in] */ Int32 i1,
        /* [in] */ Int32 i2);

    /*!
     * @brief Returns the most negative (closest to negative infinity) of the two
     * arguments.
     *
     * @param l1 @b [in] the first argument.
     * @param l2 @b [in] the second argument.
     * @return the smaller of @c l1 and @c l2.
     */
    static CARAPI_(Int64) Min(
        /* [in] */ Int64 l1,
        /* [in] */ Int64 l2);

    /**
     * Returns the most negative (closest to negative infinity) of the two
     * arguments.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code min(NaN, (anything)) = NaN}</li>
     * <li>{@code min((anything), NaN) = NaN}</li>
     * <li>{@code min(+0.0, -0.0) = -0.0}</li>
     * <li>{@code min(-0.0, +0.0) = -0.0}</li>
     * </ul>
     *
     * @param d1
     *            the first argument.
     * @param d2
     *            the second argument.
     * @return the smaller of {@code d1} and {@code d2}.
     */
    static CARAPI_(Double) Min(
        /* [in] */ Double d1,
        /* [in] */ Double d2);

    /**
     * Returns the closest double approximation of the result of raising {@code
     * x} to the power of {@code y}.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code pow((anything), +0.0) = 1.0}</li>
     * <li>{@code pow((anything), -0.0) = 1.0}</li>
     * <li>{@code pow(x, 1.0) = x}</li>
     * <li>{@code pow((anything), NaN) = NaN}</li>
     * <li>{@code pow(NaN, (anything except 0)) = NaN}</li>
     * <li>{@code pow(+/-(|x| > 1), +infinity) = +infinity}</li>
     * <li>{@code pow(+/-(|x| > 1), -infinity) = +0.0}</li>
     * <li>{@code pow(+/-(|x| < 1), +infinity) = +0.0}</li>
     * <li>{@code pow(+/-(|x| < 1), -infinity) = +infinity}</li>
     * <li>{@code pow(+/-1.0 , +infinity) = NaN}</li>
     * <li>{@code pow(+/-1.0 , -infinity) = NaN}</li>
     * <li>{@code pow(+0.0, (+anything except 0, NaN)) = +0.0}</li>
     * <li>{@code pow(-0.0, (+anything except 0, NaN, odd integer)) = +0.0}</li>
     * <li>{@code pow(+0.0, (-anything except 0, NaN)) = +infinity}</li>
     * <li>{@code pow(-0.0, (-anything except 0, NAN, odd integer))} {@code =}
     * {@code +infinity}</li>
     * <li>{@code pow(-0.0, (odd integer)) = -pow( +0 , (odd integer) )}</li>
     * <li>{@code pow(+infinity, (+anything except 0, NaN)) = +infinity}</li>
     * <li>{@code pow(+infinity, (-anything except 0, NaN)) = +0.0}</li>
     * <li>{@code pow(-infinity, (anything)) = -pow(0, (-anything))}</li>
     * <li>{@code pow((-anything), (integer))} {@code =} {@code
     * pow(-1,(integer))*pow(+anything,integer) }</li>
     * <li>{@code pow((-anything except 0 and inf), (non-integer)) = NAN}</li>
     * </ul>
     *
     * @param x
     *            the base of the operation.
     * @param y
     *            the exponent of the operation.
     * @return {@code x} to the power of {@code y}.
     */
    static CARAPI_(Double) Pow(
        /* [in] */ Double x,
        /* [in] */ Double y);

    /**
     * Returns the double conversion of the result of rounding the argument to
     * an integer. Tie breaks are rounded towards even.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code rint(+0.0) = +0.0}</li>
     * <li>{@code rint(-0.0) = -0.0}</li>
     * <li>{@code rint(+infinity) = +infinity}</li>
     * <li>{@code rint(-infinity) = -infinity}</li>
     * <li>{@code rint(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value to be rounded.
     * @return the closest integer to the argument (as a double).
     */
    static CARAPI_(Double) Rint(
        /* [in] */ Double d);

    /**
     * Returns the result of rounding the argument to an integer. The result is
     * equivalent to {@code (long) Math.floor(d+0.5)}.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code round(+0.0) = +0.0}</li>
     * <li>{@code round(-0.0) = +0.0}</li>
     * <li>{@code round((anything > Long.MAX_VALUE) = Long.MAX_VALUE}</li>
     * <li>{@code round((anything < Long.MIN_VALUE) = Long.MIN_VALUE}</li>
     * <li>{@code round(+infinity) = Long.MAX_VALUE}</li>
     * <li>{@code round(-infinity) = Long.MIN_VALUE}</li>
     * <li>{@code round(NaN) = +0.0}</li>
     * </ul>
     *
     * @param d
     *            the value to be rounded.
     * @return the closest integer to the argument.
     */
    static CARAPI_(Int64) Round(
        /* [in] */ Double d);

    /**
     * Returns the result of rounding the argument to an integer. The result is
     * equivalent to {@code (int) Math.floor(f+0.5)}.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code round(+0.0) = +0.0}</li>
     * <li>{@code round(-0.0) = +0.0}</li>
     * <li>{@code round((anything > Integer.MAX_VALUE) = Integer.MAX_VALUE}</li>
     * <li>{@code round((anything < Integer.MIN_VALUE) = Integer.MIN_VALUE}</li>
     * <li>{@code round(+infintiy) = Integer.MAX_VALUE}</li>
     * <li>{@code round(-infintiy) = Integer.MIN_VALUE}</li>
     * <li>{@code round(NaN) = +0.0}</li>
     * </ul>
     *
     * @param f
     *            the value to be rounded.
     * @return the closest integer to the argument.
     */
    static CARAPI_(Int32) Round(
        /* [in] */ Float f);

    /**
     * Returns the signum function of the argument. If the argument is less than
     * zero, it returns -1.0. If the argument is greater than zero, 1.0 is
     * returned. If the argument is either positive or negative zero, the
     * argument is returned as result.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code signum(+0.0) = +0.0}</li>
     * <li>{@code signum(-0.0) = -0.0}</li>
     * <li>{@code signum(+infinity) = +1.0}</li>
     * <li>{@code signum(-infinity) = -1.0}</li>
     * <li>{@code signum(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value whose signum has to be computed.
     * @return the value of the signum function.
     */
    static CARAPI_(Double) Signum(
        /* [in] */ Double d);

    /**
     * Returns the closest double approximation of the sine of the argument. The
     * returned result is within 1 ulp (unit in the last place) of the real
     * result.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code sin(+0.0) = +0.0}</li>
     * <li>{@code sin(-0.0) = -0.0}</li>
     * <li>{@code sin(+infinity) = NaN}</li>
     * <li>{@code sin(-infinity) = NaN}</li>
     * <li>{@code sin(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the angle whose sin has to be computed, in radians.
     * @return the sine of the argument.
     */
    static CARAPI_(Double) Sin(
        /* [in] */ Double d);

    /**
     * Returns the closest double approximation of the hyperbolic sine of the
     * argument. The returned result is within 2.5 ulps (units in the last
     * place) of the real result.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code sinh(+0.0) = +0.0}</li>
     * <li>{@code sinh(-0.0) = -0.0}</li>
     * <li>{@code sinh(+infinity) = +infinity}</li>
     * <li>{@code sinh(-infinity) = -infinity}</li>
     * <li>{@code sinh(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value whose hyperbolic sine has to be computed.
     * @return the hyperbolic sine of the argument.
     */
    static CARAPI_(Double) Sinh(
        /* [in] */ Double d);

    /**
     * Returns the closest double approximation of the square root of the
     * argument.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code sqrt(+0.0) = +0.0}</li>
     * <li>{@code sqrt(-0.0) = -0.0}</li>
     * <li>{@code sqrt( (anything < 0) ) = NaN}</li>
     * <li>{@code sqrt(+infinity) = +infinity}</li>
     * <li>{@code sqrt(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value whose square root has to be computed.
     * @return the square root of the argument.
     */
    static CARAPI_(Double) Sqrt(
        /* [in] */ Double d);

    /**
     * Returns the closest double approximation of the tangent of the argument.
     * The returned result is within 1 ulp (unit in the last place) of the real
     * result.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code tan(+0.0) = +0.0}</li>
     * <li>{@code tan(-0.0) = -0.0}</li>
     * <li>{@code tan(+infinity) = NaN}</li>
     * <li>{@code tan(-infinity) = NaN}</li>
     * <li>{@code tan(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the angle whose tangent has to be computed, in radians.
     * @return the tangent of the argument.
     */
    static CARAPI_(Double) Tan(
        /* [in] */ Double d);

    /**
     * Returns the closest double approximation of the hyperbolic tangent of the
     * argument. The absolute value is always less than 1. The returned result
     * is within 2.5 ulps (units in the last place) of the real result. If the
     * real result is within 0.5ulp of 1 or -1, it should return exactly +1 or
     * -1.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code tanh(+0.0) = +0.0}</li>
     * <li>{@code tanh(-0.0) = -0.0}</li>
     * <li>{@code tanh(+infinity) = +1.0}</li>
     * <li>{@code tanh(-infinity) = -1.0}</li>
     * <li>{@code tanh(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the value whose hyperbolic tangent has to be computed.
     * @return the hyperbolic tangent of the argument.
     */
    static CARAPI_(Double) Tanh(
        /* [in] */ Double d);

    /**
     * Returns a pseudo-random double {@code n}, where {@code n >= 0.0 && n < 1.0}.
     * This method reuses a single instance of {@link java.util.Random}.
     * This method is thread-safe because access to the {@code Random} is synchronized,
     * but this harms scalability. Applications may find a performance benefit from
     * allocating a {@code Random} for each of their threads.
     *
     * @return a pseudo-random number.
     */
    static CARAPI_(Double) Random();

    /**
     * Returns the measure in radians of the supplied degree angle. The result
     * is {@code angdeg / 180 * pi}.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code toRadians(+0.0) = +0.0}</li>
     * <li>{@code toRadians(-0.0) = -0.0}</li>
     * <li>{@code toRadians(+infinity) = +infinity}</li>
     * <li>{@code toRadians(-infinity) = -infinity}</li>
     * <li>{@code toRadians(NaN) = NaN}</li>
     * </ul>
     *
     * @param angdeg
     *            an angle in degrees.
     * @return the radian measure of the angle.
     */
    static CARAPI_(Double) ToRadians(
        /* [in] */ Double angdeg);

    /**
     * Returns the measure in degrees of the supplied radian angle. The result
     * is {@code angrad * 180 / pi}.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code toDegrees(+0.0) = +0.0}</li>
     * <li>{@code toDegrees(-0.0) = -0.0}</li>
     * <li>{@code toDegrees(+infinity) = +infinity}</li>
     * <li>{@code toDegrees(-infinity) = -infinity}</li>
     * <li>{@code toDegrees(NaN) = NaN}</li>
     * </ul>
     *
     * @param angrad
     *            an angle in radians.
     * @return the degree measure of the angle.
     */
    static CARAPI_(Double) ToDegrees(
        /* [in] */ Double angrad);

    /**
     * Returns the argument's ulp (unit in the last place). The size of a ulp of
     * a double value is the positive distance between this value and the double
     * value next larger in magnitude. For non-NaN {@code x}, {@code ulp(-x) ==
     * ulp(x)}.
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code ulp(+0.0) = Double.MIN_VALUE}</li>
     * <li>{@code ulp(-0.0) = Double.MIN_VALUE}</li>
     * <li>{@code ulp(+infintiy) = infinity}</li>
     * <li>{@code ulp(-infintiy) = infinity}</li>
     * <li>{@code ulp(NaN) = NaN}</li>
     * </ul>
     *
     * @param d
     *            the floating-point value to compute ulp of.
     * @return the size of a ulp of the argument.
     */
    static CARAPI_(Double) Ulp(
        /* [in] */ Double d);

    /**
     * Returns a double with the given magnitude and the sign of {@code sign}.
     * If {@code sign} is NaN, the sign of the result is arbitrary.
     * If you need a determinate sign in such cases, use {@code StrictMath.copySign}.
     * @since 1.6
     */
    static CARAPI_(Double) CopySign(
        /* [in] */ Double magnitude,
        /* [in] */ Double sign);

    /**
     * Returns the exponent of double {@code d}.
     * @since 1.6
     */
    static CARAPI_(Double) GetExponent(
        /* [in] */ Double d);

    /**
     * Returns the next double after {@code start} in the given {@code direction}.
     * @since 1.6
     */
    static CARAPI_(Double) NextAfter(
        /* [in] */ Double start,
        /* [in] */ Double direction);

    /**
     * Returns the next double larger than {@code d}.
     * @since 1.6
     */
    static CARAPI_(Double) NextUp(
        /* [in] */ Double d);

    /**
     * Returns {@code d} * 2^{@code scaleFactor}. The result may be rounded.
     * @since 1.6
     */
    static CARAPI_(Double) Scalb(
        /* [in] */ Double d,
        /* [in] */ Int32 scaleFactor);

    /*!
     * @brief Returns the signum function of the argument. If the argument is
     * less than zero, it returns -1.0. If the argument is greater than zero,
     * 1.0 is returned. If the argument is either positive or negative zero,
     * the argument is returned as result.
     *
     * <p>
     * Special cases:
     * <ul>
     * <li>{@code Signum(+0.0) = +0.0}
     * <li>{@code Signum(-0.0) = -0.0}
     * <li>{@code Signum(+infinity) = +1.0}
     * <li>{@code Signum(-infinity) = -1.0}
     * <li>{@code Signum(NAN) = NAN}
     * </ul>
     *
     * @param f @b [in] the value whose signum has to be computed.
     * @return the value of the signum function.
     */
    static CARAPI_(Float) Signum(
        /* [in] */ Float f);

    /*!
     * @brief Returns the argument's ulp (unit in the last place). The size of a ulp of
     * a float value is the positive distance between this value and the float
     * value next larger in magnitude. For non-NAN @c x, @c Ulp(-x)==Ulp(x).
     * <p>
     * Special cases:
     * <ul>
     * <li>{Ulp(+0.0) = Float.MIN_VALUE}
     * <li>{Ulp(-0.0) = Float.MIN_VALUE}
     * <li>{Ulp(+infintiy) = infinity}
     * <li>{Ulp(-infintiy) = infinity}
     * <li>{Ulp(NAN) = NAN}
     * </ul>
     *
     * @param f @b [in] the floating-point value to compute ulp of.
     * @return the size of a ulp of the argument.
     */
    static CARAPI_(Float) Ulp(
        /* [in] */ Float f);

    /*!
     * @brief Returns a Float with the given magnitude and the sign of @c sign.
     * If @c sign is NAN, the sign of the result is arbitrary.
     * If you need a determinate sign in such cases, use @c StrictMath.CopySign.
     */
    static CARAPI_(Float) CopySign(
        /* [in] */ Float magnitude,
        /* [in] */ Float sign);

    /*!
     * @brief Returns the next Float after @c start in the given @c direction.
     */
    static CARAPI_(Float) NextAfter(
        /* [in] */ Float start,
        /* [in] */ Double direction);

    /*!
     * @brief Returns the next Float larger than @c f.
     */
    static CARAPI_(Float) NextUp(
        /* [in] */ Float f);

    /*!
     * @brief Returns @f$ { @c d * 2^ @c scaleFactor } @f$ . The result may be rounded.
     */
    static CARAPI_(Float) Scalb(
        /* [in] */ Float d,
        /* [in] */ Int32 scaleFactor);

    /*!
     * @brief Determines the number of leading zeros in the specified integer prior to
     * the @link #highestOneBit(Int32) highest one bit @endlink .
     *
     * @param i @b [in] the integer to examine.
     * @return the number of leading zeros in @c i.
     */

    static CARAPI_(Int32) NumberOfLeadingZeros(
        /* [in] */ Int32 i);

    /*!
     * @brief Determines the number of trailing zeros in the specified integer after
     * the @link #lowestOneBit(Int32) lowest one bit @endlink .
     *
     * @param i @b [in] the integer to examine.
     * @return the number of trailing zeros in @c i.
     */
    static CARAPI_(Int32) NumberOfTrailingZeros(
        /* [in] */ Int32 i);

    /*!
     * @brief Determines the number of leading zeros in the specified long value prior
     * to the @link #highestOneBit(Int64) highest one bit @endlink .
     *
     * @param v @b [in] the Int64 to examine.
     * @return the number of leading zeros in @c v.
     */
    static CARAPI_(Int32) NumberOfLeadingZeros(
        /* [in] */ Int64 v);

    /**
     * @brief Determines the number of trailing zeros in the specified Int64 value after
     * the @link #lowestOneBit(Int64) lowest one bit @endlink .
     *
     * @param v @b [in] the long to examine.
     * @return the number of trailing zeros in @c v.
     */
    static CARAPI_(Int32) NumberOfTrailingZeros(
        /* [in] */Int64 v);

    /*!
     * @brief Indicates whether the specified Float is a @e Not-a-Number(NaN) value.
     *
     * @param f @b [in] the Float value to check.
     * @return @c true if @c f is @e Not-a-Number ;
     *         @c false if it is a (potentially infinite) Float number.
     */
    static CARAPI_(Boolean) IsNAN(
        /* [in] */Float f);

    /**
     * @brief Indicates whether the specified double is a @e Not-a-Number(NaN) value.
     *
     * @param d @b [in] the Double value to check.
     * @return @c true if @c d is @e Not-a-Number ;
     *         @c false if it is a (potentially infinite) Double number.
     */
    static CARAPI_(Boolean) IsNAN(
        /* [in] */Double d);

    /*!
     * @brief Indicates whether the specified Float represents an infinite value.
     *
     * @param f @b [in] the Float to check.
     * @return @c true if the value of @c f is positive or negative
     *         infinity; @c false otherwise.
     */
    static CARAPI_(Boolean) IsInfinite(
        /* [in] */ Float f);
    /**
     * Indicates whether the specified double represents an infinite value.
     *
     * @param d
     *            the double to check.
     * @return {@code true} if the value of {@code d} is positive or negative
     *         infinity; {@code false} otherwise.
     */
    static CARAPI_(Boolean) IsInfinite(
        /* [in] */ Double f);

    static CARAPI_(Int32) FloatToInt32Bits(
        /* [in] */ Float value);

    static CARAPI_(Int32) FloatToRawInt32Bits(
        /* [in] */ Float value);

    static CARAPI_(Float) Int32BitsToFloat(
        /* [in] */ Int32 value);

    static CARAPI_(Int64) DoubleToRawInt64Bits(
        /* [in] */ Double value);

    static CARAPI_(Int64) DoubleToInt64Bits(
        /* [in] */ Double d);

    static CARAPI_(Double) Int64BitsToDouble(
        /* [in] */ Int64 i);

private:
    static CARAPI_(Double) Nextafter(
        /* [in] */ Double x,
        /* [in] */ Double y);

    static CARAPI_(Float) Nextafterf(
        /* [in] */ Float x,
        /* [in] */ Float y);

    /*!
     * @brief Shifts integer bits as Float, if the digits is positive, left-shift; if
     * not, shift to right and calculate its carry.
     */
    static CARAPI_(Int32) ShiftInt32Bits(
        /* [in] */ Int32 bits,
        /* [in] */ Int32 digits);

    /*!
     * @brief Shifts Int64 bits as Double, if the digits is positive, left-shift; if
     * not, shift to right and calculate its carry.
     */
    static CARAPI_(Int64) ShiftInt64Bits(
        /* [in] */ Int64 bits,
        /* [in] */ Int64 digits);
};

} //System
} //Elastos

#endif //__MATH_H__
