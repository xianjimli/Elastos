
#include "cmdef.h"
#include "DecimalFormat.h"
#include "CDecimalFormatSymbols.h"
#include "CNativeDecimalFormat.h"
#include <elastos/Math.h>
#include <elastos/AutoFree.h>


using namespace Elastos::Core;

const Double DecimalFormat::NEGATIVE_ZERO_DOUBLE;

DecimalFormat::DecimalFormat()
    : mRoundingMode(RoundingMode_HALF_EVEN)
{}

ECode DecimalFormat::Init()
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    CDecimalFormatSymbols::New(locale, (IDecimalFormatSymbols**)&mSymbols);
    assert(0);
//    InitNative((LocaleData::Get(pLocale))->mNumberPattern);
    return E_NOT_IMPLEMENTED;
}

ECode DecimalFormat::Init(
    /* [in] */ const String& pattern)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return Init(pattern, locale);
}

ECode DecimalFormat::Init(
    /* [in] */ const String& pattern,
    /* [in] */ IDecimalFormatSymbols* value)
{
    value->Clone((IDecimalFormatSymbols**)&mSymbols);
    return InitNative(pattern);
}

ECode DecimalFormat::Init(
    /* [in] */ const String& pattern,
    /* [in] */ ILocale* locale)
{
    CDecimalFormatSymbols::New(locale, (IDecimalFormatSymbols**)&mSymbols);
    return InitNative(pattern);
}

ECode DecimalFormat::InitNative(
    /* [in] */ const String& pattern)
{
    //try {
    FAIL_RETURN(CNativeDecimalFormat::New(pattern, mSymbols, (INativeDecimalFormat**)&mDform));
    //} catch (IllegalArgumentException ex) {
    //    throw new IllegalArgumentException(pattern);
    //}
    Int32 maxFractionDigits;
    Int32 maxIntegerDigits;
    Int32 minFractionDigits;
    Int32 minIntegerDigits;
    mDform->GetMaximumFractionDigits(&maxFractionDigits);
    mDform->GetMaximumIntegerDigits(&maxIntegerDigits);
    mDform->GetMinimumFractionDigits(&minFractionDigits);
    mDform->GetMinimumIntegerDigits(&minIntegerDigits);
    NumberFormat::SetMaximumFractionDigits(maxFractionDigits);
    NumberFormat::SetMaximumIntegerDigits(maxIntegerDigits);
    NumberFormat::SetMinimumFractionDigits(minFractionDigits);
    NumberFormat::SetMinimumIntegerDigits(minIntegerDigits);
    return NOERROR;
}

ECode DecimalFormat::ApplyLocalizedPattern(
    /* [in] */ const String& pattern)
{
   return mDform->ApplyLocalizedPattern(pattern);
}

ECode DecimalFormat::ApplyPattern(
    /* [in] */ const String& pattern)
{
    return mDform->ApplyPattern(pattern);
}

ECode DecimalFormat::FormatToCharacterIterator(
    /* [in] */ IInterface* object,
    /* [out] */ IAttributedCharacterIterator** characterIterator)
{
    if (object == NULL) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    return mDform->FormatToCharacterIterator(object, characterIterator);
}

ECode DecimalFormat::CheckBufferAndFieldPosition(
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* position)
{
    // if (buffer == NULL) {
    //     //throw new NullPointerException("buffer == null");
    //     return E_NULL_POINTER_EXCEPTION;
    // }
    if (position == NULL) {
        //throw new NullPointerException("position == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    return NOERROR;
}

ECode DecimalFormat::FormatDoubleEx(
    /* [in] */ Double value,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* result)
{
    FAIL_RETURN(CheckBufferAndFieldPosition(buffer, field));
    // All float/double/Float/Double formatting ends up here...
    if (mRoundingMode == RoundingMode_UNNECESSARY) {
        // ICU4C doesn't support this rounding mode, so we have to fake it.
        //try {
        SetRoundingMode(RoundingMode_UP);
        AutoPtr<IFieldPosition> fp;
        CFieldPosition::New(0, (IFieldPosition**)&fp);
        String upResult;
        FormatDoubleEx(value, String(NULL), fp, &upResult);

        SetRoundingMode(RoundingMode_DOWN);
        AutoPtr<IFieldPosition> fpx;
        CFieldPosition::New(0, (IFieldPosition**)&fpx);
        String downResult;
        FormatDoubleEx(value, String(NULL), fp, &downResult);
        if (!upResult.Equals(downResult)) {
            //throw new ArithmeticException("rounding mode UNNECESSARY but rounding required");
            SetRoundingMode(RoundingMode_UNNECESSARY);
            return E_ARITHMETIC_EXCEPTION;
        }
        //} finally {
        SetRoundingMode(RoundingMode_UNNECESSARY);
        //}
    }
    AutoFree< ArrayOf<Char32> > v;
    FAIL_RETURN(mDform->FormatDouble(value, field, (ArrayOf<Char32>**)&v));
    StringBuffer sb(buffer.string());
    for (Int32 i = 0; i < v->GetLength(); ++i) {
        sb += (*v.Get())[i];
    }
    *result = sb;
    return NOERROR;
}

ECode DecimalFormat::FormatInt64Ex(
    /* [in] */ Int64 value,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* result)
{
    FAIL_RETURN(CheckBufferAndFieldPosition(buffer, field));
    AutoFree< ArrayOf<Char32> > v;
    FAIL_RETURN(mDform->FormatInt64(value, field, (ArrayOf<Char32>**)&v));
    StringBuffer sb(buffer.string());
    for (Int32 i = 0; i < v->GetLength(); ++i) {
        sb += (*v.Get())[i];
    }
    *result = sb;
    return NOERROR;
}

ECode DecimalFormat::FormatObjectEx(
    /* [in] */ IInterface* object,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* value)
{
    FAIL_RETURN(CheckBufferAndFieldPosition(buffer, field));
    assert(0);
/*
    if (number instanceof BigInteger) {
        BigInteger bigInteger = (BigInteger) number;
        char[] chars = (bigInteger.bitLength() < 64)
                ? dform.formatLong(bigInteger.longValue(), position)
                : dform.formatBigInteger(bigInteger, position);
        buffer.append(chars);
        return buffer;
    } else if (number instanceof BigDecimal) {
        buffer.append(dform.formatBigDecimal((BigDecimal) number, position));
        return buffer;
    }
    return super.format(number, buffer, position);
*/
    return E_NOT_IMPLEMENTED;
}

ECode DecimalFormat::GetDecimalFormatSymbols(
    /* [out] */ IDecimalFormatSymbols** symbols)
{
    return mSymbols->Clone(symbols);
}

ECode DecimalFormat::GetCurrency(
    /* [out] */ ICurrency** currency)
{
    return mSymbols->GetCurrency(currency);
}

ECode DecimalFormat::GetGroupingSize(
    /* [out] */ Int32* size)
{
    return mDform->GetGroupingSize(size);
}

ECode DecimalFormat::GetMultiplier(
    /* [out] */ Int32* multiplier)
{
    return mDform->GetMultiplier(multiplier);
}

ECode DecimalFormat::GetNegativePrefix(
    /* [out] */ String* prefix)
{
    return mDform->GetNegativePrefix(prefix);
}

ECode DecimalFormat::GetNegativeSuffix(
    /* [out] */ String* suffix)
{
    return mDform->GetNegativeSuffix(suffix);
}

ECode DecimalFormat::GetPositivePrefix(
    /* [out] */ String* prefix)
{
    return mDform->GetPositivePrefix(prefix);
}

ECode DecimalFormat::GetPositiveSuffix(
    /* [out] */ String* suffix)
{
    return mDform->GetPositiveSuffix(suffix);
}

ECode DecimalFormat::IsDecimalSeparatorAlwaysShown(
    /* [out] */ Boolean* isAlwaysShown)
{
    return mDform->IsDecimalSeparatorAlwaysShown(isAlwaysShown);
}

ECode DecimalFormat::IsParseBigDecimal(
    /* [out] */ Boolean* isParseBigDecimal)
{
    return mDform->IsParseBigDecimal(isParseBigDecimal);
}

ECode DecimalFormat::SetParseIntegerOnly(
    /* [in] */ Boolean value)
{
    // In this implementation, NativeDecimalFormat is wrapped to
    // fulfill most of the format and parse feature. And this method is
    // delegated to the wrapped instance of NativeDecimalFormat.
    return mDform->SetParseIntegerOnly(value);
}

ECode DecimalFormat::IsParseIntegerOnly(
    /* [out] */ Boolean* isParseIntegerOnly)
{
    return mDform->IsParseIntegerOnly(isParseIntegerOnly);
}

ECode DecimalFormat::ParseEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ INumber** value)
{
    AutoPtr<INumber> number;
    FAIL_RETURN(mDform->Parse(string, position, (INumber**)&number));
    if (NULL == number) {
        *value = NULL;
        return NOERROR;
    }
    // BEGIN android-removed
    // if (this.isParseBigDecimal()) {
    //     if (number instanceof Long) {
    //         return new BigDecimal(number.longValue());
    //     }
    //     if ((number instanceof Double) && !((Double) number).isInfinite()
    //             && !((Double) number).isNaN()) {
    //
    //         return new BigDecimal(number.doubleValue());
    //     }
    //     if (number instanceof BigInteger) {
    //         return new BigDecimal(number.doubleValue());
    //     }
    //     if (number instanceof com.ibm.icu.math.BigDecimal) {
    //         return new BigDecimal(number.toString());
    //     }
    //     return number;
    // }
    // if ((number instanceof com.ibm.icu.math.BigDecimal)
    //         || (number instanceof BigInteger)) {
    //     return new Double(number.doubleValue());
    // }
    // END android-removed
    // BEGIN android-added
    assert(0);
    Boolean isParseBigDecimal;
    if (IsParseBigDecimal(&isParseBigDecimal), isParseBigDecimal) {
//        if (number instanceof Long) {
//            return new BigDecimal(number.longValue());
//        }
//        if ((number instanceof Double) && !((Double) number).isInfinite()
//                && !((Double) number).isNaN()) {

//            return new BigDecimal(number.toString());
//        }
//        if (number instanceof BigInteger) {
//            return new BigDecimal(number.toString());
//        }
//        return number;
   }
//    if ((number instanceof BigDecimal) || (number instanceof BigInteger)) {
//        return new Double(number.doubleValue());
//    }
    // END android-added

    Boolean isParseIntegerOnly;
    this->IsParseIntegerOnly(&isParseIntegerOnly);
//    if (isParseIntegerOnly && number.equals(NEGATIVE_ZERO_DOUBLE)) {
//        return Long.valueOf(0); // android-changed
//    }
//    return number;

    return E_NOT_IMPLEMENTED;
}

ECode DecimalFormat::SetDecimalFormatSymbols(
    /* [in] */ IDecimalFormatSymbols* value)
{
    if (value != NULL) {
        // BEGIN android-changed: the Java object is canonical, and we copy down to native code.
        mSymbols = NULL;
        value->Clone((IDecimalFormatSymbols**)&mSymbols);
        mDform->SetDecimalFormatSymbols(mSymbols);
        // END android-changed
    }
    return NOERROR;
}

ECode DecimalFormat::SetCurrency(
    /* [in] */ ICurrency* currency)
{
    // // BEGIN android-changed
    // dform.setCurrency(Currency.getInstance(currency.getCurrencyCode()));
    // // END android-changed
    // symbols.setCurrency(currency);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode DecimalFormat::SetDecimalSeparatorAlwaysShown(
    /* [in] */ Boolean value)
{
    return mDform->SetDecimalSeparatorAlwaysShown(value);
}

ECode DecimalFormat::SetGroupingSize(
    /* [in] */ Int32 value)
{
    return mDform->SetGroupingSize(value);
}

ECode DecimalFormat::SetGroupingUsed(
    /* [in] */ Boolean value)
{
    return mDform->SetGroupingUsed(value);
}

ECode DecimalFormat::IsGroupingUsed(
    /* [out] */ Boolean* isGroupingUsed)
{
    return mDform->IsGroupingUsed(isGroupingUsed);
}

ECode DecimalFormat::SetMaximumFractionDigits(
    /* [in] */ Int32 value)
{
    NumberFormat::SetMaximumFractionDigits(value);
    Int32 maxFractionDigits;
    GetMaximumFractionDigits(&maxFractionDigits);
    mDform->SetMaximumFractionDigits(maxFractionDigits);
    // Changing the maximum fraction digits needs to update ICU4C's rounding configuration.
    SetRoundingMode(mRoundingMode);
    return NOERROR;
}

ECode DecimalFormat::SetMaximumIntegerDigits(
    /* [in] */ Int32 value)
{
    NumberFormat::SetMaximumIntegerDigits(value);
    Int32 maxIntegerDigits;
    GetMaximumIntegerDigits(&maxIntegerDigits);
    mDform->GetMaximumIntegerDigits(&maxIntegerDigits);
    return NOERROR;
}

ECode DecimalFormat::SetMinimumFractionDigits(
    /* [in] */ Int32 value)
{
    NumberFormat::SetMinimumFractionDigits(value);
    Int32 minFractionDigits;
    GetMinimumFractionDigits(&minFractionDigits);
    mDform->SetMinimumFractionDigits(minFractionDigits);
    return NOERROR;
}

ECode DecimalFormat::SetMinimumIntegerDigits(
    /* [in] */ Int32 value)
{
    NumberFormat::SetMinimumIntegerDigits(value);
    Int32 minIntegerDigits;
    GetMinimumIntegerDigits(&minIntegerDigits);
    mDform->SetMinimumIntegerDigits(minIntegerDigits);
    return NOERROR;
}

ECode DecimalFormat::SetMultiplier(
    /* [in] */ Int32 value)
{
    return mDform->SetMultiplier(value);
}

ECode DecimalFormat::SetNegativePrefix(
    /* [in] */ const String& value)
{
    return mDform->SetNegativePrefix(value);
}

ECode DecimalFormat::SetNegativeSuffix(
    /* [in] */ const String& value)
{
    return mDform->SetNegativeSuffix(value);
}

ECode DecimalFormat::SetPositivePrefix(
    /* [in] */ const String& value)
{
    return mDform->SetPositivePrefix(value);
}

ECode DecimalFormat::SetPositiveSuffix(
    /* [in] */ const String& value)
{
    return mDform->SetPositiveSuffix(value);
}

ECode DecimalFormat::SetParseBigDecimal(
    /* [in] */ Boolean newValue)
{
    return mDform->SetParseBigDecimal(newValue);
}

ECode DecimalFormat::ToLocalizedPattern(
    /* [out] */ String* pattern)
{
    return mDform->ToLocalizedPattern(pattern);
}

ECode DecimalFormat::ToPattern(
    /* [out] */ String* pattern)
{
    return mDform->ToPattern(pattern);
}

ECode DecimalFormat::GetRoundingMode(
    /* [out] */ RoundingMode* mode)
{
    *mode = mRoundingMode;
    return NOERROR;
}

ECode DecimalFormat::SetRoundingMode(
    /* [in] */ RoundingMode mode)
{
    // if (mode == NULL) {
    //     //throw new NullPointerException();
    //     return E_NULL_POINTER_EXCEPTION;
    // }
    mRoundingMode = mode;
    if (mRoundingMode != RoundingMode_UNNECESSARY) { // ICU4C doesn't support UNNECESSARY.
        Int32 maxFractionDigits;
        GetMaximumFractionDigits(&maxFractionDigits);
        Double roundingIncrement = 1.0 / Math::Pow(10, Math::Max(0, maxFractionDigits));
        mDform->SetRoundingMode(mode, roundingIncrement);
    }
    return NOERROR;
}
