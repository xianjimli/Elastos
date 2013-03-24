#include "DecimalFormat.h"

ECode DecimalFormat::Init()
{
    mRoundingMode = RoundingMode_HALF_EVEN;
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    CDecimalFormatSymbols::New(pLocale, (IDecimalFormatSymbols**)&mSymbols);
//    InitNative((LocaleData::Get(pLocale))->mNumberPattern);
    return NOERROR;
}

ECode DecimalFormat::Init(
        /* [in] */ String pattern)
{
    mRoundingMode = RoundingMode_HALF_EVEN;
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    return Init(pattern, pLocale);
}

ECode DecimalFormat::Init(
        /* [in] */ String pattern,
        /* [in] */ IDecimalFormatSymbols* value)
{
    mRoundingMode = RoundingMode_HALF_EVEN;
    mSymbols = value;
    InitNative(pattern);
    return NOERROR;
}

ECode DecimalFormat::Init(
        /* [in] */ String pattern,
        /* [in] */ ILocale* locale)
{
    CDecimalFormatSymbols::New(locale, (IDecimalFormatSymbols**)&mSymbols);
    InitNative(pattern);
    return NOERROR;
}

void DecimalFormat::InitNative(
        /* [in] */ String pattern)
{
    //try {
        CNativeDecimalFormat::New(pattern, mSymbols, (INativeDecimalFormat**)&mDform);
    //} catch (IllegalArgumentException ex) {
    //    throw new IllegalArgumentException(pattern);
    //}
    Int32 maximumFractionDigits, maximumIntegerDigits, minimumFractionDigits,
    minimumIntegerDigits;
    mDform->GetMaximumFractionDigits(&maximumFractionDigits);
    mDform->GetMaximumIntegerDigits(&maximumIntegerDigits);
    mDform->GetMinimumFractionDigits(&minimumFractionDigits);
    mDform->GetMinimumIntegerDigits(&minimumIntegerDigits);
    NumberFormat::SetMaximumFractionDigits(maximumFractionDigits);
    NumberFormat::SetMaximumIntegerDigits(maximumIntegerDigits);
    NumberFormat::SetMinimumFractionDigits(minimumFractionDigits);
    NumberFormat::SetMinimumIntegerDigits(minimumIntegerDigits);
}

ECode DecimalFormat::ApplyLocalizedPattern(
        /* [in] */ String pattern)
{
   return mDform->ApplyLocalizedPattern(pattern);
}

ECode DecimalFormat::ApplyPattern(
        /* [in] */ String pattern)
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
        /* [in] */ StringBuffer buffer,
        /* [in] */ IFieldPosition* position)
{
    if (buffer == NULL) {
        //throw new NullPointerException("buffer == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (position == NULL) {
        //throw new NullPointerException("position == null");
        return E_NULL_POINTER_EXCEPTION;
    }
}

ECode DecimalFormat::formatEx3(
        /* [in] */ Double value,
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ StringBuffer* result)
{
    VALIDATE_NOT_NULL(result);

    CheckBufferAndFieldPosition(*buffer, field);
    // All float/double/Float/Double formatting ends up here...
    if (mRoundingMode == RoundingMode_UNNECESSARY) {
        // ICU4C doesn't support this rounding mode, so we have to fake it.
        //try {
            SetRoundingMode(RoundingMode_UP);

            AutoPtr<IFieldPosition> fp;
            CFieldPosition::New(0, (IFieldPosition**)&fp);
            formatEx3(value, new StringBuffer(NULL), (IFieldPosition*)fp, result);
            String upResult = String(result->Substring(0, result->GetLength()));
            SetRoundingMode(RoundingMode_DOWN);


            AutoPtr<IFieldPosition> fpx;
            CFieldPosition::New(0, (IFieldPosition**)&fpx);
            formatEx3(value, new StringBuffer(NULL), (IFieldPosition*)fp, result);
            String downResult = String(result->Substring(0, result->GetLength()));
            if (!upResult.Equals(downResult)) {
                //throw new ArithmeticException("rounding mode UNNECESSARY but rounding required");
                return NOERROR;
            }
        //} finally {
            SetRoundingMode(RoundingMode_UNNECESSARY);
        //}
    }
    ArrayOf<Char32>* v;
    mDform->FormatDouble(value, field, &v);
    (*buffer) += *v;
    *result = StringBuffer(*buffer);
    return NOERROR;
}

ECode DecimalFormat::formatEx5(
        /* [in] */ Int64 value,
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ StringBuffer* formattedString)
{
    VALIDATE_NOT_NULL(formattedString);
    CheckBufferAndFieldPosition(*buffer, field);
    ArrayOf<Char32>* v;
    mDform->FormatLong(value, field, &v);
    (*buffer) += *v;
    *formattedString = StringBuffer(*buffer);
    return NOERROR;
}

ECode DecimalFormat::formatEx(
            /* [in] */ IInterface* object,
            /* [in] */ StringBuffer* buffer,
            /* [in] */ IFieldPosition* field,
            /* [out] */ StringBuffer* value)
{
    VALIDATE_NOT_NULL(value);
    CheckBufferAndFieldPosition(*buffer, field);
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
    return NOERROR;
}

ECode DecimalFormat::GetDecimalFormatSymbols(
        /* [out] */ IDecimalFormatSymbols** decimalFormatSymbols)
{
//    return (DecimalFormatSymbols) symbols.clone();
    return NOERROR;
}

ECode DecimalFormat::GetGroupingSize(
        /* [out] */ Int32* groupingSize)
{
    VALIDATE_NOT_NULL(groupingSize);
    return mDform->GetGroupingSize(groupingSize);
}

ECode DecimalFormat::GetMultiplier(
        /* [out] */ Int32* multiplier)
{
    VALIDATE_NOT_NULL(multiplier);
    return mDform->GetMultiplier(multiplier);
}

ECode DecimalFormat::GetNegativePrefix(
        /* [out] */ String* negativePrefix)
{
    VALIDATE_NOT_NULL(negativePrefix);
    return mDform->GetNegativePrefix(negativePrefix);
}

ECode DecimalFormat::GetNegativeSuffix(
        /* [out] */ String* negativeSuffix)
{
    VALIDATE_NOT_NULL(negativeSuffix);
    return mDform->GetNegativeSuffix(negativeSuffix);
}

ECode DecimalFormat::GetPositivePrefix(
        /* [out] */ String* positivePrefix)
{
    VALIDATE_NOT_NULL(positivePrefix);
    return mDform->GetPositivePrefix(positivePrefix);
}

ECode DecimalFormat::GetPositiveSuffix(
        /* [out] */ String* positiveSuffix)
{
    VALIDATE_NOT_NULL(positiveSuffix);
    return mDform->GetPositiveSuffix(positiveSuffix);
}

ECode DecimalFormat::IsDecimalSeparatorAlwaysShown(
        /* [out] */ Boolean* isDecimalSeparatorAlwaysShown)
{
    VALIDATE_NOT_NULL(isDecimalSeparatorAlwaysShown);
    return mDform->IsDecimalSeparatorAlwaysShown(isDecimalSeparatorAlwaysShown);
}

ECode DecimalFormat::IsParseBigDecimal(
        /* [out] */ Boolean* isParseBigDecimal)
{
    VALIDATE_NOT_NULL(isParseBigDecimal);
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
    VALIDATE_NOT_NULL(isParseIntegerOnly);
    return mDform->IsParseIntegerOnly(isParseIntegerOnly);
}

ECode DecimalFormat::ParseEx(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** value)
{
    AutoPtr<INumber> number;
    mDform->Parse(string, position, (INumber**)&number);
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
    Boolean isParseBigDecimal;
    this->IsParseBigDecimal(&isParseBigDecimal);
    if (isParseBigDecimal) {
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
//    }
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
}

ECode DecimalFormat::SetDecimalFormatSymbols(
        /* [in] */ IDecimalFormatSymbols* value)
{
    if (value != NULL) {
        // BEGIN android-changed: the Java object is canonical, and we copy down to native code.
//        this.symbols = (DecimalFormatSymbols) value.clone();
        mDform->SetDecimalFormatSymbols(this->mSymbols);
        // END android-changed
    }
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
    VALIDATE_NOT_NULL(isGroupingUsed);
    return mDform->IsGroupingUsed(isGroupingUsed);
}

ECode DecimalFormat::SetMaximumFractionDigits(
        /* [in] */ Int32 value)
{
    NumberFormat::SetMaximumFractionDigits(value);
    Int32 maximumFractionDigits;
    this->GetMaximumFractionDigits(&maximumFractionDigits);
    mDform->SetMaximumFractionDigits(maximumFractionDigits);
    // Changing the maximum fraction digits needs to update ICU4C's rounding configuration.
    SetRoundingMode(mRoundingMode);
    return NOERROR;
}

ECode DecimalFormat::SetMaximumIntegerDigits(
        /* [in] */ Int32 value)
{
    NumberFormat::SetMaximumIntegerDigits(value);
    Int32 maximumIntegerDigits;
    this->GetMaximumIntegerDigits(&maximumIntegerDigits);
    mDform->GetMaximumIntegerDigits(&maximumIntegerDigits);
    return NOERROR;
}

ECode DecimalFormat::SetMinimumFractionDigits(
        /* [in] */ Int32 value)
{
    NumberFormat::SetMinimumFractionDigits(value);
    Int32 minimumFractionDigits;
    this->GetMinimumFractionDigits(&minimumFractionDigits);
    mDform->SetMinimumFractionDigits(minimumFractionDigits);
    return NOERROR;
}

ECode DecimalFormat::GetMinimumIntegerDigits(
        /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    Int32 minimumIntegerDigits;
    this->GetMinimumIntegerDigits(&minimumIntegerDigits);
    mDform->SetMinimumIntegerDigits(minimumIntegerDigits);
    return NOERROR;
}

ECode DecimalFormat::SetMultiplier(
        /* [in] */ Int32 value)
{
    return mDform->SetMultiplier(value);
}

ECode DecimalFormat::SetNegativePrefix(
        /* [in] */ String value)
{
    return mDform->SetNegativePrefix(value);
}

ECode DecimalFormat::SetNegativeSuffix(
        /* [in] */ String value)
{
    return mDform->SetNegativeSuffix(value);
}

ECode DecimalFormat::SetPositivePrefix(
        /* [in] */ String value)
{
    return mDform->SetPositivePrefix(value);
}

ECode DecimalFormat::SetPositiveSuffix(
        /* [in] */ String value)
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
    VALIDATE_NOT_NULL(pattern);
    return mDform->ToLocalizedPattern(pattern);
}

ECode DecimalFormat::ToPattern(
        /* [out] */ String* pattern)
{
    VALIDATE_NOT_NULL(pattern);
    return mDform->ToPattern(pattern);
}

ECode DecimalFormat::GetRoundingMode(
        /* [out] */ RoundingMode* roundingMode)
{
    *roundingMode = mRoundingMode;
    return NOERROR;
}

ECode DecimalFormat::SetRoundingMode(
        /* [in] */ RoundingMode roundingMode)
{
    if (roundingMode == NULL) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    mRoundingMode = roundingMode;
    if (mRoundingMode != RoundingMode_UNNECESSARY) { // ICU4C doesn't support UNNECESSARY.
        Int32 maximumFractionDigits;
        GetMaximumFractionDigits(&maximumFractionDigits);
        Double roundingIncrement = 1.0 / Math::Pow(10, Math::Max(0, maximumFractionDigits));
        mDform->SetRoundingMode(roundingMode, roundingIncrement);
    }
    return NOERROR;
}