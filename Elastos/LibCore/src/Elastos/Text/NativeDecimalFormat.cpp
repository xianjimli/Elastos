#include "NativeDecimalFormat.h"

ArrayOf<IFormatField*>* NativeDecimalFormat::FieldPositionIterator::fields;

NativeDecimalFormat::NativeDecimalFormat(){}

ECode NativeDecimalFormat::Init(
        /* [in] */ String pattern,
        /* [in] */ IDecimalFormatSymbols* dfs)
{
    String* currencySymbol = new String("");
    dfs->GetCurrencySymbol(currencySymbol);
    Char32 decimalSeparator;
    dfs->GetDecimalSeparator(&decimalSeparator);
    Char32 digit;
    dfs->GetDigit(&digit);
    String* exponentSeparator = new String("");
    dfs->GetExponentSeparator(exponentSeparator);
    Char32 groupingSeparator;
    dfs->GetGroupingSeparator(&groupingSeparator);
    String* infinity = new String("");
    dfs->GetInfinity(infinity);
    String* internationalCurrencySymbol = new String("");
    dfs->GetInternationalCurrencySymbol(internationalCurrencySymbol);
    Char32 minusSign;
    dfs->GetMinusSign(&minusSign);
    Char32 monetaryDecimalSeparator;
    dfs->GetMonetaryDecimalSeparator(&monetaryDecimalSeparator);
    String* naN = new String("");
    dfs->GetNaN(naN);
    Char32 patternSeparator;
    dfs->GetPatternSeparator(&patternSeparator);
    Char32 percent;
    dfs->GetPercent(&percent);
    Char32 perMill;
    dfs->GetPerMill(&perMill);
    Char32 zeroDigit;
    dfs->GetZeroDigit(&zeroDigit);
    //try {
        this->addr = Open(pattern, *currencySymbol,
                decimalSeparator, digit, *exponentSeparator,
                groupingSeparator, *infinity,
                *internationalCurrencySymbol, minusSign,
                monetaryDecimalSeparator, *naN, patternSeparator,
                percent, perMill, zeroDigit);
        this->lastPattern = new String(pattern);
    //} catch (NullPointerException npe) {
    //    throw npe;
    //} catch (RuntimeException re) {
    //    throw new IllegalArgumentException("syntax error: " + re.getMessage() + ": " + pattern);
    //}
    return NOERROR;
}

//NativeDecimalFormat::NativeDecimalFormat(
//        /* [in] */ String pattern,
//        /* [in] */ LocaleData* data)
//{
//    this->addr = Open(pattern, data->mCurrencySymbol,
//            data->mDecimalSeparator, data->mDigit, data->mEexponentSeparator, data->mGroupingSeparator,
//            data->mInfinity, data->mInternationalCurrencySymbol, data->mMinusSign,
//            data->mMonetarySeparator, data->mNaN, data->mPatternSeparator,
//            data->mPercent, data->mPerMill, data->mZeroDigit);
//    this->lastPattern = new String(pattern);
//    return NOERROR;
//}

NativeDecimalFormat::NativeDecimalFormat(
    /* [in] */ INativeDecimalFormat* other)
{
    Int32 oaddr;
    String olastPattern = String(NULL);
    Boolean onegPrefNull, onegSuffNull, oposPrefNull, oposSuffNull;
    other->GetAddr(&oaddr);
    other->GetLastPattern(&olastPattern);
    other->GetNegPrefNull(&onegPrefNull);
    other->GetNegSuffNull(&onegSuffNull);
    other->GetPosPrefNull(&oposPrefNull);
    other->GetPosSuffNull(&oposSuffNull);
    addr = CloneImpl(oaddr);
    lastPattern = new String(olastPattern);
    negPrefNull = onegPrefNull;
    negSuffNull = onegSuffNull;
    posPrefNull = oposPrefNull;
    posSuffNull = oposSuffNull;
}

ECode NativeDecimalFormat::Close()
{
    if (addr != 0) {
        Close(addr);
        addr = 0;
    }
    return NOERROR;
}

ECode NativeDecimalFormat::SetDecimalFormatSymbols(
    /* [in] */ IDecimalFormatSymbols* dfs)
{
    String* currencySymbol = new String("");
    dfs->GetCurrencySymbol(currencySymbol);
    Char32 decimalSeparator;
    dfs->GetDecimalSeparator(&decimalSeparator);
    Char32 digit;
    dfs->GetDigit(&digit);
    String* exponentSeparator = new String("");
    dfs->GetExponentSeparator(exponentSeparator);
    Char32 groupingSeparator;
    dfs->GetGroupingSeparator(&groupingSeparator);
    String* infinity = new String("");
    dfs->GetInfinity(infinity);
    String* internationalCurrencySymbol = new String("");
    dfs->GetInternationalCurrencySymbol(internationalCurrencySymbol);
    Char32 minusSign;
    dfs->GetMinusSign(&minusSign);
    Char32 monetaryDecimalSeparator;
    dfs->GetMonetaryDecimalSeparator(&monetaryDecimalSeparator);
    String* naN = new String("");
    dfs->GetNaN(naN);
    Char32 patternSeparator;
    dfs->GetPatternSeparator(&patternSeparator);
    Char32 percent;
    dfs->GetPercent(&percent);
    Char32 perMill;
    dfs->GetPerMill(&perMill);
    Char32 zeroDigit;
    dfs->GetZeroDigit(&zeroDigit);
    SetDecimalFormatSymbols(this->addr, *currencySymbol, decimalSeparator,
            digit, *exponentSeparator, groupingSeparator,
            *infinity, *internationalCurrencySymbol, minusSign,
            monetaryDecimalSeparator, *naN, patternSeparator,
            percent, perMill, zeroDigit);
    return NOERROR;
}

//ECode NativeDecimalFormat::SetDecimalFormatSymbols(
//        /* [in] */ const LocaleData* localeData)
//{
//    SetDecimalFormatSymbols(this->addr, localeData->mCurrencySymbol, localeData->mDecimalSeparator,
//            localeData->mDigit, localeData->mExponentSeparator, localeData->mGroupingSeparator,
//            localeData->mInfinity, localeData->mInternationalCurrencySymbol, localeData->mMinusSign,
//            localeData->mMonetarySeparator, localeData->mNaN, localeData->mPatternSeparator,
//            localeData->mPercent, localeData->mPerMill, localeData->mZeroDigit);
//    return NOERROR;
//}

ECode NativeDecimalFormat::FormatLong(
        /* [in] */ Int64 value,
        /* [in] */ IFieldPosition* field,
        /* [out, callee] */ ArrayOf<Char32>** array)
{
    FieldPositionIterator* fpi;
    FieldPositionIterator::ForFieldPosition(field, &fpi);
    ArrayOf<Char32>* result = FormatLong(this->addr, value, fpi);
    if (fpi != NULL) {
        FieldPositionIterator::SetFieldPosition(fpi, field);
    }
    *array = result;
    return NOERROR;
}

ECode NativeDecimalFormat::FormatDouble(
        /* [in] */ Double value,
        /* [in] */ IFieldPosition* field,
        /* [out, callee] */ ArrayOf<Char32>** array)
{
    FieldPositionIterator* fpi;
    FieldPositionIterator::ForFieldPosition(field, &fpi);
    ArrayOf<Char32>* result = FormatDouble(addr, value, fpi);
    if (fpi != NULL) {
        FieldPositionIterator::SetFieldPosition(fpi, field);
    }
    *array = result;
    return NOERROR;
}

ECode NativeDecimalFormat::ApplyLocalizedPattern(
        /* [in] */ String pattern)
{
    ApplyPattern(this->addr, TRUE, pattern);
    lastPattern = new String(NULL);
    return NOERROR;
}

ECode NativeDecimalFormat::ApplyPattern(
        /* [in] */ String pattern)
{
    if (lastPattern->IsNull() && pattern.Equals(*lastPattern)) {
        return E_NULL_POINTER_EXCEPTION;
    }
    ApplyPattern(addr, FALSE, pattern);
    lastPattern = new String(pattern);
    return NOERROR;
}

ECode NativeDecimalFormat::FormatToCharacterIterator(
        /* [in] */ IInterface* object,
        /* [out] */ IAttributedCharacterIterator** iter)
{
/*
    if (!(object instanceof Number)) {
        throw new IllegalArgumentException();
    }
    Number number = (Number) object;
    FieldPositionIterator fpIter = new FieldPositionIterator();
    String text;
    if (number instanceof BigInteger || number instanceof BigDecimal) {
        text = new String(formatDigitList(this.addr, number.toString(), fpIter));
    } else if (number instanceof Double || number instanceof Float) {
        double dv = number.doubleValue();
        text = new String(formatDouble(this.addr, dv, fpIter));
    } else {
        long lv = number.longValue();
        text = new String(formatLong(this.addr, lv, fpIter));
    }

    AttributedString as = new AttributedString(text);

    while (fpIter.next()) {
        Format.Field field = fpIter.field();
        as.addAttribute(field, field, fpIter.start(), fpIter.limit());
    }

    // return the CharacterIterator from AttributedString
    return as.getIterator();
*/
    return NOERROR;
}

ECode NativeDecimalFormat::MakeScalePositive(
        /* [in] */ Int32 scale,
        /* [in] */ StringBuffer val,
        /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    if (scale < 0) {
        scale = -scale;
        for (Int32 i = scale; i > 0; i--) {
            val += '0';
        }
        scale = 0;
    }
    *value = scale;
    return NOERROR;
}

ECode NativeDecimalFormat::ToLocalizedPattern(
        /* [out] */ String* localizedPattern)
{
    VALIDATE_NOT_NULL(localizedPattern);
    *localizedPattern = String(ToPatternImpl(this->addr, TRUE));
    return NOERROR;
}

ECode NativeDecimalFormat::ToPattern(
        /* [out] */ String* pattern)
{
    *pattern = String(ToPatternImpl(this->addr, FALSE));
    return NOERROR;
}

ECode NativeDecimalFormat::Parse(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** number)
{
    *number = Parse(addr, string, position, parseBigDecimal);
    return NOERROR;
}

ECode NativeDecimalFormat::GetMaximumFractionDigits(
        /* [out] */ Int32* maximumFractionDigits)
{
    VALIDATE_NOT_NULL(maximumFractionDigits);
    *maximumFractionDigits = GetAttribute(this->addr, UNUM_MAX_FRACTION_DIGITS);
    return NOERROR;
}

ECode NativeDecimalFormat::GetMaximumIntegerDigits(
        /* [out] */ Int32* maximumIntegerDigits)
{
    VALIDATE_NOT_NULL(maximumIntegerDigits);
    *maximumIntegerDigits = GetAttribute(this->addr, UNUM_MAX_INTEGER_DIGITS);
    return NOERROR;
}

ECode NativeDecimalFormat::GetMinimumFractionDigits(
        /* [out] */ Int32* minimumFractionDigits)
{
    VALIDATE_NOT_NULL(minimumFractionDigits);
    *minimumFractionDigits = GetAttribute(this->addr, UNUM_MIN_FRACTION_DIGITS);
    return NOERROR;
}

ECode NativeDecimalFormat::GetMinimumIntegerDigits(
        /* [out] */ Int32* minimumIntegerDigits)
{
    VALIDATE_NOT_NULL(minimumIntegerDigits);
    *minimumIntegerDigits = GetAttribute(this->addr, UNUM_MIN_INTEGER_DIGITS);
    return NOERROR;
}

ECode NativeDecimalFormat::GetGroupingSize(
        /* [out] */ Int32* groupingSize)
{
    VALIDATE_NOT_NULL(groupingSize);
    *groupingSize = GetAttribute(this->addr, UNUM_GROUPING_SIZE);
    return NOERROR;
}

ECode NativeDecimalFormat::GetMultiplier(
        /* [out] */ Int32* multiplier)
{
    VALIDATE_NOT_NULL(multiplier);
    *multiplier = GetAttribute(this->addr, UNUM_MULTIPLIER);
    return NOERROR;
}

ECode NativeDecimalFormat::GetNegativePrefix(
        /* [out] */ String* negativePrefix)
{
    VALIDATE_NOT_NULL(negativePrefix);
    if (negPrefNull) {
        *negativePrefix = String(NULL);
        return NOERROR;
    }
    *negativePrefix = String(GetTextAttribute(this->addr, UNUM_NEGATIVE_PREFIX));
    return NOERROR;
}

ECode NativeDecimalFormat::GetNegativeSuffix(
        /* [out] */ String* negativeSuffix)
{
    VALIDATE_NOT_NULL(negativeSuffix);
    if (negSuffNull) {
        *negativeSuffix = String(NULL);
    }
    *negativeSuffix = String(GetTextAttribute(this->addr, UNUM_NEGATIVE_SUFFIX));
    return NOERROR;
}

ECode NativeDecimalFormat::GetPositivePrefix(
        /* [out] */ String* positivePrefix)
{
    VALIDATE_NOT_NULL(positivePrefix);
    if (posPrefNull) {
        *positivePrefix = String(NULL);
        return NOERROR;
    }
    *positivePrefix = String(GetTextAttribute(this->addr, UNUM_POSITIVE_PREFIX));
    return NOERROR;
}

ECode NativeDecimalFormat::GetPositiveSuffix(
        /* [out] */ String* positiveSuffix)
{
    VALIDATE_NOT_NULL(positiveSuffix)
    if (posSuffNull) {
        *positiveSuffix = String(NULL);
        return NOERROR;
    }
    *positiveSuffix = String(GetTextAttribute(this->addr, UNUM_POSITIVE_SUFFIX));
    return NOERROR;
}

ECode NativeDecimalFormat::IsDecimalSeparatorAlwaysShown(
        /* [out] */ Boolean* isDecimalSeparatorAlwaysShown)
{
    VALIDATE_NOT_NULL(isDecimalSeparatorAlwaysShown);
    *isDecimalSeparatorAlwaysShown = (GetAttribute(this->addr, UNUM_DECIMAL_ALWAYS_SHOWN) != 0) ? TRUE : FALSE;
    return NOERROR;
}

ECode NativeDecimalFormat::IsParseBigDecimal(
        /* [out] */ Boolean* isParseBigDecimal)
{
    VALIDATE_NOT_NULL(isParseBigDecimal);
    *isParseBigDecimal = parseBigDecimal;
    return NOERROR;
}

ECode NativeDecimalFormat::IsParseIntegerOnly(
        /* [out] */ Boolean* isParseIntegerOnly)
{
    VALIDATE_NOT_NULL(isParseIntegerOnly);
    *isParseIntegerOnly = (GetAttribute(this->addr, UNUM_PARSE_INT_ONLY) != 0) ? TRUE : FALSE;
    return NOERROR;
}

ECode NativeDecimalFormat::IsGroupingUsed(
        /* [out] */ Boolean* isGroupingUsed)
{
    VALIDATE_NOT_NULL(isGroupingUsed);
    *isGroupingUsed = (GetAttribute(this->addr, UNUM_GROUPING_USED) != 0) ? TRUE : FALSE;
    return NOERROR;
}

ECode NativeDecimalFormat::SetDecimalSeparatorAlwaysShown(
        /* [in] */ Boolean value)
{
    Int32 i = value ? -1 : 0;
    SetAttribute(this->addr, UNUM_DECIMAL_ALWAYS_SHOWN, i);
    return NOERROR;
}

ECode NativeDecimalFormat::SetCurrency(
    /* [in] */ ICurrency* currency)
{
    String* symbol = new String("");
    currency->GetSymbol(symbol);
    String* currencyCode = new String("");
    currency->GetCurrencyCode(currencyCode);
    SetSymbol(this->addr, UNUM_CURRENCY_SYMBOL, *symbol);
    SetSymbol(this->addr, UNUM_INTL_CURRENCY_SYMBOL, *currencyCode);
    return NOERROR;
}

ECode NativeDecimalFormat::SetGroupingSize(
        /* [in] */ Int32 value)
{
    SetAttribute(this->addr, UNUM_GROUPING_SIZE, value);
    return NOERROR;
}

ECode NativeDecimalFormat::SetGroupingUsed(
        /* [in] */ Boolean value)
{
    Int32 i = value ? -1 : 0;
    SetAttribute(this->addr, UNUM_GROUPING_USED, i);
    return NOERROR;
}

ECode NativeDecimalFormat::SetMaximumFractionDigits(
        /* [in] */ Int32 value)
{
    SetAttribute(this->addr, UNUM_MAX_FRACTION_DIGITS, value);
    return NOERROR;
}

ECode NativeDecimalFormat::SetMaximumIntegerDigits(
        /* [in] */ Int32 value)
{
    SetAttribute(this->addr, UNUM_MAX_INTEGER_DIGITS, value);
    return NOERROR;
}

ECode NativeDecimalFormat::SetMinimumFractionDigits(
        /* [in] */ Int32 value)
{
    SetAttribute(this->addr, UNUM_MIN_FRACTION_DIGITS, value);
    return NOERROR;
}

ECode NativeDecimalFormat::SetMinimumIntegerDigits(
        /* [in] */ Int32 value)
{
    SetAttribute(this->addr, UNUM_MIN_INTEGER_DIGITS, value);
    return NOERROR;
}

ECode NativeDecimalFormat::SetMultiplier(
        /* [in] */ Int32 value)
{
    SetAttribute(this->addr, UNUM_MULTIPLIER, value);
    // Update the cached BigDecimal for multiplier.
//    multiplierBigDecimal = BigDecimal.valueOf(value);
    return NOERROR;
}

ECode NativeDecimalFormat::SetNegativePrefix(
        /* [in] */ String value)
{
    negPrefNull = (value == NULL);
    if (!negPrefNull) {
        SetTextAttribute(this->addr, UNUM_NEGATIVE_PREFIX, value);
    }
    return NOERROR;
}

ECode NativeDecimalFormat::SetNegativeSuffix(
        /* [in] */ String value)
{
    negSuffNull = (value == NULL);
    if (!negSuffNull) {
        SetTextAttribute(this->addr, UNUM_NEGATIVE_SUFFIX, value);
    }
    return NOERROR;
}

ECode NativeDecimalFormat::SetPositivePrefix(
        /* [in] */ String value)
{
    posPrefNull = (value == NULL);
    if (!posPrefNull) {
        SetTextAttribute(this->addr, UNUM_POSITIVE_PREFIX, value);
    }
    return NOERROR;
}

ECode NativeDecimalFormat::SetPositiveSuffix(
        /* [in] */ String value)
{
    posSuffNull = (value == NULL);
    if (!posSuffNull) {
        SetTextAttribute(this->addr, UNUM_POSITIVE_SUFFIX, value);
    }
    return NOERROR;
}

ECode NativeDecimalFormat::SetParseBigDecimal(
        /* [in] */ Boolean value)
{
    parseBigDecimal = value;
    return NOERROR;
}

ECode NativeDecimalFormat::SetParseIntegerOnly(
        /* [in] */ Boolean value)
{
    Int32 i = value ? -1 : 0;
    SetAttribute(this->addr, UNUM_PARSE_INT_ONLY, i);
    return NOERROR;
}

ECode NativeDecimalFormat::SetRoundingMode(
        /* [in] */ RoundingMode roundingMode,
        /* [in] */ Double roundingIncrement)
{
    Int32 nativeRoundingMode;
    switch (roundingMode) {
        case RoundingMode_CEILING: nativeRoundingMode = 0; break;
        case RoundingMode_FLOOR: nativeRoundingMode = 1; break;
        case RoundingMode_DOWN: nativeRoundingMode = 2; break;
        case RoundingMode_UP: nativeRoundingMode = 3; break;
        case RoundingMode_HALF_EVEN: nativeRoundingMode = 4; break;
        case RoundingMode_HALF_DOWN: nativeRoundingMode = 5; break;
        case RoundingMode_HALF_UP: nativeRoundingMode = 6; break;
        default: //throw new AssertionError();
            return NOERROR;
    }
    SetRoundingMode(addr, nativeRoundingMode, roundingIncrement);
    return NOERROR;
}

ECode NativeDecimalFormat::ApplyPattern(
        /* [in] */ Int32 addr,
        /* [in] */ Boolean localized,
        /* [in] */ String pattern)
{
    //try {
        ApplyPatternImpl(addr, localized, pattern);
    //} catch (NullPointerException npe) {
    //    throw npe;
    //} catch (RuntimeException re) {
    //    throw new IllegalArgumentException("syntax error: " + re.getMessage() + ": " + pattern);
    //}
    return NOERROR;
}

ECode NativeDecimalFormat::GetAddr(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = addr;
    return NOERROR;
}

ECode NativeDecimalFormat::GetLastPattern(
            /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    *value = String(*lastPattern);
    return NOERROR;
}

ECode NativeDecimalFormat::GetNegPrefNull(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = negPrefNull;
    return NOERROR;
}

ECode NativeDecimalFormat::GetNegSuffNull(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = negSuffNull;
    return NOERROR;
}

ECode NativeDecimalFormat::GetPosPrefNull(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = posPrefNull;
    return NOERROR;
}

ECode NativeDecimalFormat::GetPosSuffNull(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = posSuffNull;
    return NOERROR;
}

void NativeDecimalFormat::ApplyPatternImpl(
    /* [in] */ Int32 addr,
    /* [in] */ Boolean localized,
    /* [in] */ String pattern)
{
    return ;
}

Int32 NativeDecimalFormat::CloneImpl(
    /* [in] */ Int32 addr)
{
    return 0;
}

void NativeDecimalFormat::Close(
    /* [in] */ Int32 addr)
{
    return ;
}

ArrayOf<Char32>* NativeDecimalFormat::FormatLong(
    /* [in] */ Int32 addr,
    /* [in] */ Int64 value,
    /* [in] */ FieldPositionIterator* iter)
{
    return NULL;
}

ArrayOf<Char32>* NativeDecimalFormat::FormatDouble(
    /* [in] */ Int32 addr,
    /* [in] */ Double value,
    /* [in] */ FieldPositionIterator* iter)
{
    return NULL;
}

ArrayOf<Char32>* NativeDecimalFormat::FormatDigitList(
    /* [in] */ Int32 addr,
    /* [in] */ String value,
    /* [in] */ FieldPositionIterator* iter)
{
    return NULL;
}

Int32 NativeDecimalFormat::GetAttribute(
    /* [in] */ Int32 addr,
    /* [in] */ Int32 symbol)
{
    return 0;
}

String NativeDecimalFormat::GetTextAttribute(
    /* [in] */ Int32 addr,
    /* [in] */ Int32 symbol)
{
    return String(NULL);
}

Int32 NativeDecimalFormat::Open(
    /* [in] */ String pattern,
    /* [in] */ String currencySymbol,
    /* [in] */ Char32 decimalSeparator,
    /* [in] */ Char32 digit,
    /* [in] */ String exponentSeparator,
    /* [in] */ Char32 groupingSeparator,
    /* [in] */ String infinity,
    /* [in] */ String internationalCurrencySymbol,
    /* [in] */ Char32 minusSign,
    /* [in] */ Char32 monetaryDecimalSeparator,
    /* [in] */ String nan,
    /* [in] */ Char32 patternSeparator,
    /* [in] */ Char32 percent,
    /* [in] */ Char32 perMill,
    /* [in] */ Char32 zeroDigit)
{
    return 0;
}

INumber* NativeDecimalFormat::Parse(
    /* [in] */ Int32 addr,
    /* [in] */ String string,
    /* [in] */ IParsePosition* position,
    /* [in] */ Boolean parseBigDecimal)
{
    return NULL;
}

void NativeDecimalFormat::SetDecimalFormatSymbols(
    /* [in] */ Int32 addr,
    /* [in] */ String currencySymbol,
    /* [in] */ Char32 decimalSeparator,
    /* [in] */ Char32 digit,
    /* [in] */ String exponentSeparator,
    /* [in] */ Char32 groupingSeparator,
    /* [in] */ String infinity,
    /* [in] */ String internationalCurrencySymbol,
    /* [in] */ Char32 minusSign,
    /* [in] */ Char32 monetaryDecimalSeparator,
    /* [in] */ String nan,
    /* [in] */ Char32 patternSeparator,
    /* [in] */ Char32 percent,
    /* [in] */ Char32 perMill,
    /* [in] */ Char32 zeroDigit)
{
    return;
}

void NativeDecimalFormat::SetSymbol(
    /* [in] */ Int32 addr,
    /* [in] */ Int32 symbol,
    /* [in] */ String str)
{
    return;
}

void NativeDecimalFormat::SetAttribute(
    /* [in] */ Int32 addr,
    /* [in] */ Int32 symbol,
    /* [in] */ Int32 i)
{
    return;
}

void NativeDecimalFormat::SetRoundingMode(
    /* [in] */ Int32 addr,
    /* [in] */ Int32 roundingMode,
    /* [in] */ Double roundingIncrement)
{
    return;
}

void NativeDecimalFormat::SetTextAttribute(
    /* [in] */ Int32 addr,
    /* [in] */ Int32 symbol,
    /* [in] */ String str)
{
    return;
}

String NativeDecimalFormat::ToPatternImpl(
    /* [in] */ Int32 addr,
    /* [in] */ Boolean localized)
{
    return String(NULL);
}

NativeDecimalFormat::FieldPositionIterator::FieldPositionIterator() : pos(-3)
{
}

ECode NativeDecimalFormat::FieldPositionIterator::ForFieldPosition(
        /* [in] */ IFieldPosition* fp,
        /* [out] */ FieldPositionIterator** fpi)
{
    Int32 field;
    fp->GetField(&field);
    if (fp != NULL && field != -1) {
        *fpi = new FieldPositionIterator();
        return NOERROR;
    }
    *fpi = NULL;
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::GetNativeFieldPositionId(
            /* [in] */ IFieldPosition* fp,
            /* [out] */ Int32* nfpid)
{
    // NOTE: -1, 0, and 1 were the only valid original java field values
    // for NumberFormat.  They take precedence.  This assumes any other
    // value is a mistake and the actual value is in the attribute.
    // Clients can construct FieldPosition combining any attribute with any field
    // value, which is just wrong, but there you go.

    Int32 id;
    fp->GetField(&id);
    if (id < -1 || id > 1) {
        id = -1;
    }
    if (id == -1) {
        AutoPtr<IFormatField> attr;
        fp->GetFieldAttribute((IFormatField**)&attr);
        if (attr != NULL) {
            for (Int32 i = 0; i < fields->GetLength(); ++i) {
                if ((*fields)[i] == attr) {
                    id = i;
                    break;
                }
            }
        }
    }
    *nfpid = id;
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::SetFieldPosition(
        /* [in] */ FieldPositionIterator* fpi,
        /* [in] */ IFieldPosition* fp)
{
    if (fpi != NULL && fp != NULL) {
        Int32 field;
        GetNativeFieldPositionId(fp, &field);
        if (field != -1) {
            Boolean isNext;
            fpi->Next(&isNext);
            while (isNext) {
                Int32 fieldId;
                fpi->FieldId(&fieldId);
                if (fieldId == field) {
                    Int32 start, limit;
                    fp->SetBeginIndex(fpi->Start(&start));
                    fp->SetEndIndex(fpi->Limit(&limit));
                    break;
                }
            }
        }
    }
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::Next(
        /* [out] */ Boolean* next) {
    VALIDATE_NOT_NULL(next);
    // if pos == data.length, we've already returned false once
    if (data == NULL || pos == data->GetLength()) {
        //throw new NoSuchElementException();
        return NOERROR;
    }
    pos += 3;
    *next = (pos < data->GetLength()) ? TRUE : FALSE;
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::CheckValid()
{
    if (data == NULL || pos < 0 || pos == data->GetLength()) {
        //throw new NoSuchElementException();
        return NOERROR;
    }
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::FieldId(
        /* [out] */ Int32* fieldId)
{
    VALIDATE_NOT_NULL(fieldId);
    *fieldId = (*data)[pos];
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::Field(
        /* [out] */ IFormatField** ff)
{
    CheckValid();
    *ff = (*fields)[(*data)[pos]];
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::Start(
        /* [out] */ Int32* start)
{
    VALIDATE_NOT_NULL(start);
    CheckValid();
    *start = (*data)[pos + 1];
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::Limit(
        /* [out] */ Int32* limit)
{
    VALIDATE_NOT_NULL(limit);
    CheckValid();
    *limit = (*data)[pos + 2];
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::SetData(
        /* [in] */ ArrayOf<Int32>* data)
{
    this->data = data;
    this->pos = -3;
    return NOERROR;
}