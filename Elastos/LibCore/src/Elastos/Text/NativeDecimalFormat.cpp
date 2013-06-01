
#include "cmdef.h"
#include "NativeDecimalFormat.h"
#include "ElStringByteSink.h"
#include <Elastos.Utility.h>
#include <unicode/decimfmt.h>
#include <unicode/unum.h>


ArrayOf< AutoPtr<IFormatField> >* NativeDecimalFormat::FieldPositionIterator::sFields; /*= {
            // The old java field values were 0 for integer and 1 for fraction.
            // The new java field attributes are all objects.  ICU assigns the values
            // starting from 0 in the following order; note that integer and
            // fraction positions match the old field values.
            NumberFormat.Field.INTEGER,
            NumberFormat.Field.FRACTION,
            NumberFormat.Field.DECIMAL_SEPARATOR,
            NumberFormat.Field.EXPONENT_SYMBOL,
            NumberFormat.Field.EXPONENT_SIGN,
            NumberFormat.Field.EXPONENT,
            NumberFormat.Field.GROUPING_SEPARATOR,
            NumberFormat.Field.CURRENCY,
            NumberFormat.Field.PERCENT,
            NumberFormat.Field.PERMILLE,
            NumberFormat.Field.SIGN,
        };*/

NativeDecimalFormat::FieldPositionIterator::FieldPositionIterator()
    : mData(NULL)
    , mPos(-3)
{}

AutoPtr<NativeDecimalFormat::FieldPositionIterator> NativeDecimalFormat::FieldPositionIterator::ForFieldPosition(
    /* [in] */ IFieldPosition* fp)
{
    Int32 field;
    if (fp != NULL && (fp->GetField(&field), field != -1)) {
        return new FieldPositionIterator();
    }
    return NULL;
}

Int32 NativeDecimalFormat::FieldPositionIterator::GetNativeFieldPositionId(
    /* [in] */ IFieldPosition* fp)
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
            for (Int32 i = 0; i < sFields->GetLength(); ++i) {
                if ((*sFields)[i] == attr) {
                    id = i;
                    break;
                }
            }
        }
    };
    return id;
}

ECode NativeDecimalFormat::FieldPositionIterator::SetFieldPosition(
    /* [in] */ FieldPositionIterator* fpi,
    /* [in] */ IFieldPosition* fp)
{
    if (fpi != NULL && fp != NULL) {
        Int32 field = GetNativeFieldPositionId(fp);
        if (field != -1) {
            Boolean isNext;
            FAIL_RETURN(fpi->Next(&isNext));
            while (isNext) {
                Int32 fieldId;
                FAIL_RETURN(fpi->FieldId(&fieldId));
                if (fieldId == field) {
                    Int32 start, limit;
                    FAIL_RETURN(fpi->Start(&start));
                    FAIL_RETURN(fpi->Limit(&limit));
                    fp->SetBeginIndex(start);
                    fp->SetEndIndex(limit);
                    break;
                }
                FAIL_RETURN(fpi->Next(&isNext));
            }
        }
    }
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::Next(
    /* [out] */ Boolean* next)
{
    assert(next != NULL);

    // if pos == data.length, we've already returned false once
    if (mData == NULL || mPos == mData->GetLength()) {
        //throw new NoSuchElementException();
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }
    mPos += 3;
    *next = mPos < mData->GetLength();
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::CheckValid()
{
    if (mData == NULL || mPos < 0 || mPos == mData->GetLength()) {
        //throw new NoSuchElementException();
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::FieldId(
    /* [out] */ Int32* fieldId)
{
    assert(fieldId != NULL);
    *fieldId = (*mData)[mPos];
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::Field(
    /* [out] */ IFormatField** field)
{
    assert(field != NULL);
    FAIL_RETURN(CheckValid());
    *field = (*sFields)[(*mData)[mPos]];
    if (*field != NULL) (*field)->AddRef();
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::Start(
    /* [out] */ Int32* start)
{
    assert(start != NULL);
    FAIL_RETURN(CheckValid());
    *start = (*mData)[mPos + 1];
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::Limit(
    /* [out] */ Int32* limit)
{
    assert(limit != NULL);
    FAIL_RETURN(CheckValid());
    *limit = (*mData)[mPos + 2];
    return NOERROR;
}

ECode NativeDecimalFormat::FieldPositionIterator::SetData(
    /* [in] */ ArrayOf<Int32>* data)
{
    mData = data;
    mPos = -3;
    return NOERROR;
}

NativeDecimalFormat::NativeDecimalFormat()
    : mNegPrefNull(FALSE)
    , mNegSuffNull(FALSE)
    , mPosPrefNull(FALSE)
    , mPosSuffNull(FALSE)
    , mParseBigDecimal(FALSE)
{}

ECode NativeDecimalFormat::Init(
    /* [in] */ const String& pattern,
    /* [in] */ IDecimalFormatSymbols* dfs)
{
    String currencySymbol;
    dfs->GetCurrencySymbol(&currencySymbol);
    Char32 decimalSeparator;
    dfs->GetDecimalSeparator(&decimalSeparator);
    Char32 digit;
    dfs->GetDigit(&digit);
    String exponentSeparator;
    dfs->GetExponentSeparator(&exponentSeparator);
    Char32 groupingSeparator;
    dfs->GetGroupingSeparator(&groupingSeparator);
    String infinity;
    dfs->GetInfinity(&infinity);
    String internationalCurrencySymbol;
    dfs->GetInternationalCurrencySymbol(&internationalCurrencySymbol);
    Char32 minusSign;
    dfs->GetMinusSign(&minusSign);
    Char32 monetaryDecimalSeparator;
    dfs->GetMonetaryDecimalSeparator(&monetaryDecimalSeparator);
    String naN;
    dfs->GetNaN(&naN);
    Char32 patternSeparator;
    dfs->GetPatternSeparator(&patternSeparator);
    Char32 percent;
    dfs->GetPercent(&percent);
    Char32 perMill;
    dfs->GetPerMill(&perMill);
    Char32 zeroDigit;
    dfs->GetZeroDigit(&zeroDigit);
    //try {
    FAIL_RETURN(Open(pattern, currencySymbol,
            decimalSeparator, digit, exponentSeparator,
            groupingSeparator, infinity,
            internationalCurrencySymbol, minusSign,
            monetaryDecimalSeparator, naN, patternSeparator,
            percent, perMill, zeroDigit, &mAddr));
    mLastPattern = pattern;
    //} catch (NullPointerException npe) {
    //    throw npe;
    //} catch (RuntimeException re) {
    //    throw new IllegalArgumentException("syntax error: " + re.getMessage() + ": " + pattern);
    //}
    return NOERROR;
}

ECode NativeDecimalFormat::Init(
   /* [in] */ const String& pattern,
   /* [in] */ ILocaleData* data)
{
    assert(0);
   // this->addr = Open(pattern, data->mCurrencySymbol,
   //         data->mDecimalSeparator, data->mDigit, data->mEexponentSeparator, data->mGroupingSeparator,
   //         data->mInfinity, data->mInternationalCurrencySymbol, data->mMinusSign,
   //         data->mMonetarySeparator, data->mNaN, data->mPatternSeparator,
   //         data->mPercent, data->mPerMill, data->mZeroDigit);
   // this->lastPattern = new String(pattern);
   return E_NOT_IMPLEMENTED;
}

// NativeDecimalFormat::NativeDecimalFormat(
//     /* [in] */ INativeDecimalFormat* other)
// {
//     Int32 oaddr;
//     String olastPattern = String(NULL);
//     Boolean onegPrefNull, onegSuffNull, oposPrefNull, oposSuffNull;
//     other->GetAddr(&oaddr);
//     other->GetLastPattern(&olastPattern);
//     other->GetNegPrefNull(&onegPrefNull);
//     other->GetNegSuffNull(&onegSuffNull);
//     other->GetPosPrefNull(&oposPrefNull);
//     other->GetPosSuffNull(&oposSuffNull);
//     addr = CloneImpl(oaddr);
//     lastPattern = new String(olastPattern);
//     negPrefNull = onegPrefNull;
//     negSuffNull = onegSuffNull;
//     posPrefNull = oposPrefNull;
//     posSuffNull = oposSuffNull;
// }

ECode NativeDecimalFormat::Close()
{
    Mutex::Autolock lock(GetSelfLock());

    if (mAddr != 0) {
        Close(mAddr);
        mAddr = 0;
    }
    return NOERROR;
}

ECode NativeDecimalFormat::SetDecimalFormatSymbols(
    /* [in] */ IDecimalFormatSymbols* dfs)
{
    String currencySymbol;
    dfs->GetCurrencySymbol(&currencySymbol);
    Char32 decimalSeparator;
    dfs->GetDecimalSeparator(&decimalSeparator);
    Char32 digit;
    dfs->GetDigit(&digit);
    String exponentSeparator;
    dfs->GetExponentSeparator(&exponentSeparator);
    Char32 groupingSeparator;
    dfs->GetGroupingSeparator(&groupingSeparator);
    String infinity;
    dfs->GetInfinity(&infinity);
    String internationalCurrencySymbol;
    dfs->GetInternationalCurrencySymbol(&internationalCurrencySymbol);
    Char32 minusSign;
    dfs->GetMinusSign(&minusSign);
    Char32 monetaryDecimalSeparator;
    dfs->GetMonetaryDecimalSeparator(&monetaryDecimalSeparator);
    String NaN;
    dfs->GetNaN(&NaN);
    Char32 patternSeparator;
    dfs->GetPatternSeparator(&patternSeparator);
    Char32 percent;
    dfs->GetPercent(&percent);
    Char32 perMill;
    dfs->GetPerMill(&perMill);
    Char32 zeroDigit;
    dfs->GetZeroDigit(&zeroDigit);
    SetDecimalFormatSymbols(mAddr, currencySymbol, decimalSeparator,
            digit, exponentSeparator, groupingSeparator,
            infinity, internationalCurrencySymbol, minusSign,
            monetaryDecimalSeparator, NaN, patternSeparator,
            percent, perMill, zeroDigit);
    return NOERROR;
}

ECode NativeDecimalFormat::SetDecimalFormatSymbols(
    /* [in] */ ILocaleData* localeData)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
//    SetDecimalFormatSymbols(this->addr, localeData->mCurrencySymbol, localeData->mDecimalSeparator,
//            localeData->mDigit, localeData->mExponentSeparator, localeData->mGroupingSeparator,
//            localeData->mInfinity, localeData->mInternationalCurrencySymbol, localeData->mMinusSign,
//            localeData->mMonetarySeparator, localeData->mNaN, localeData->mPatternSeparator,
//            localeData->mPercent, localeData->mPerMill, localeData->mZeroDigit);
//    return NOERROR;
}

ECode NativeDecimalFormat::FormatBigDecimal(
    /* [in] */ IBigDecimal* value,
    /* [in] */ IFieldPosition* field,
    /* [out, callee] */ ArrayOf<Char32>** result)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
    // FieldPositionIterator fpi = FieldPositionIterator.forFieldPosition(field);
    // char[] result = formatDigitList(this.addr, value.toString(), fpi);
    // if (fpi != null) {
    //     FieldPositionIterator.setFieldPosition(fpi, field);
    // }
    // return result;
}

ECode NativeDecimalFormat::FormatBigInteger(
    /* [in] */ IBigInteger* value,
    /* [in] */ IFieldPosition* field,
    /* [out, callee] */ ArrayOf<Char32>** result)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
    // FieldPositionIterator fpi = FieldPositionIterator.forFieldPosition(field);
    // char[] result = formatDigitList(this.addr, value.toString(10), fpi);
    // if (fpi != null) {
    //     FieldPositionIterator.setFieldPosition(fpi, field);
    // }
    // return result;
}

ECode NativeDecimalFormat::FormatInt64(
    /* [in] */ Int64 value,
    /* [in] */ IFieldPosition* field,
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    AutoPtr<FieldPositionIterator> fpi = FieldPositionIterator::ForFieldPosition(field);
    ArrayOf<Char32>* result = FormatInt64(mAddr, value, fpi);
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
    AutoPtr<FieldPositionIterator> fpi = FieldPositionIterator::ForFieldPosition(field);
    ArrayOf<Char32>* result = FormatDouble(mAddr, value, fpi);
    if (fpi != NULL) {
        FieldPositionIterator::SetFieldPosition(fpi, field);
    }
    *array = result;
    return NOERROR;
}

ECode NativeDecimalFormat::ApplyLocalizedPattern(
    /* [in] */ const String& pattern)
{
    FAIL_RETURN(ApplyPattern(mAddr, TRUE, pattern));
    mLastPattern = NULL;
    return NOERROR;
}

ECode NativeDecimalFormat::ApplyPattern(
    /* [in] */ const String& pattern)
{
    if (!mLastPattern.IsNull() && pattern.Equals(mLastPattern)) {
        return NOERROR;
    }
    FAIL_RETURN(ApplyPattern(mAddr, FALSE, pattern));
    mLastPattern = pattern;
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
    assert(0);
    return NOERROR;
}

Int32 NativeDecimalFormat::MakeScalePositive(
    /* [in] */ Int32 scale,
    /* [in] */ StringBuffer& val)
{
    if (scale < 0) {
        scale = -scale;
        for (Int32 i = scale; i > 0; i--) {
            val += '0';
        }
        scale = 0;
    }
    return scale;
}

ECode NativeDecimalFormat::ToLocalizedPattern(
    /* [out] */ String* pattern)
{
    assert(pattern != NULL);
    *pattern = ToPatternImpl(mAddr, TRUE);
    return NOERROR;
}

ECode NativeDecimalFormat::ToPattern(
    /* [out] */ String* pattern)
{
    assert(pattern != NULL);
    *pattern = ToPatternImpl(mAddr, FALSE);
    return NOERROR;
}

ECode NativeDecimalFormat::Parse(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ INumber** number)
{
    AutoPtr<INumber> _number;
    _number = Parse(mAddr, string, position, mParseBigDecimal);
    *number = _number;
    if (*number != NULL) (*number)->AddRef();
    return NOERROR;
}

ECode NativeDecimalFormat::GetMaximumFractionDigits(
    /* [out] */ Int32* maxFractionDigits)
{
    assert(maxFractionDigits != NULL);
    *maxFractionDigits = GetAttribute(mAddr, UNUM_MAX_FRACTION_DIGITS);
    return NOERROR;
}

ECode NativeDecimalFormat::GetMaximumIntegerDigits(
    /* [out] */ Int32* maxIntegerDigits)
{
    assert(maxIntegerDigits != NULL);
    *maxIntegerDigits = GetAttribute(mAddr, UNUM_MAX_INTEGER_DIGITS);
    return NOERROR;
}

ECode NativeDecimalFormat::GetMinimumFractionDigits(
    /* [out] */ Int32* minFractionDigits)
{
    assert(minFractionDigits != NULL);
    *minFractionDigits = GetAttribute(mAddr, UNUM_MIN_FRACTION_DIGITS);
    return NOERROR;
}

ECode NativeDecimalFormat::GetMinimumIntegerDigits(
    /* [out] */ Int32* minIntegerDigits)
{
    assert(minIntegerDigits != NULL);
    *minIntegerDigits = GetAttribute(mAddr, UNUM_MIN_INTEGER_DIGITS);
    return NOERROR;
}

ECode NativeDecimalFormat::GetGroupingSize(
    /* [out] */ Int32* size)
{
    assert(size != NULL);
    *size = GetAttribute(mAddr, UNUM_GROUPING_SIZE);
    return NOERROR;
}

ECode NativeDecimalFormat::GetMultiplier(
    /* [out] */ Int32* multiplier)
{
    assert(multiplier != NULL);
    *multiplier = GetAttribute(mAddr, UNUM_MULTIPLIER);
    return NOERROR;
}

ECode NativeDecimalFormat::GetNegativePrefix(
    /* [out] */ String* prefix)
{
    assert(prefix != NULL);
    if (mNegPrefNull) {
        *prefix = NULL;
        return NOERROR;
    }
    return GetTextAttribute(mAddr, UNUM_NEGATIVE_PREFIX, prefix);
}

ECode NativeDecimalFormat::GetNegativeSuffix(
    /* [out] */ String* suffix)
{
    assert(suffix != NULL);
    if (mNegSuffNull) {
        *suffix = NULL;
        return NOERROR;
    }
    return GetTextAttribute(mAddr, UNUM_NEGATIVE_SUFFIX, suffix);
}

ECode NativeDecimalFormat::GetPositivePrefix(
    /* [out] */ String* prefix)
{
    assert(prefix != NULL);
    if (mPosPrefNull) {
        *prefix = NULL;
        return NOERROR;
    }
    return GetTextAttribute(mAddr, UNUM_POSITIVE_PREFIX, prefix);
}

ECode NativeDecimalFormat::GetPositiveSuffix(
    /* [out] */ String* suffix)
{
    assert(suffix != NULL);
    if (mPosSuffNull) {
        *suffix = NULL;
        return NOERROR;
    }
    return GetTextAttribute(mAddr, UNUM_POSITIVE_SUFFIX, suffix);
}

ECode NativeDecimalFormat::IsDecimalSeparatorAlwaysShown(
    /* [out] */ Boolean* isAlwaysShown)
{
    assert(isAlwaysShown != NULL);
    *isAlwaysShown = GetAttribute(mAddr, UNUM_DECIMAL_ALWAYS_SHOWN) != 0;
    return NOERROR;
}

ECode NativeDecimalFormat::IsParseBigDecimal(
    /* [out] */ Boolean* isParseBigDecimal)
{
    assert(isParseBigDecimal != NULL);
    *isParseBigDecimal = mParseBigDecimal;
    return NOERROR;
}

ECode NativeDecimalFormat::IsParseIntegerOnly(
    /* [out] */ Boolean* isParseIntegerOnly)
{
    assert(isParseIntegerOnly != NULL);
    *isParseIntegerOnly = GetAttribute(mAddr, UNUM_PARSE_INT_ONLY) != 0;
    return NOERROR;
}

ECode NativeDecimalFormat::IsGroupingUsed(
    /* [out] */ Boolean* isGroupingUsed)
{
    assert(isGroupingUsed != NULL);
    *isGroupingUsed = GetAttribute(mAddr, UNUM_GROUPING_USED) != 0;
    return NOERROR;
}

ECode NativeDecimalFormat::SetDecimalSeparatorAlwaysShown(
    /* [in] */ Boolean value)
{
    Int32 i = value ? -1 : 0;
    SetAttribute(mAddr, UNUM_DECIMAL_ALWAYS_SHOWN, i);
    return NOERROR;
}

ECode NativeDecimalFormat::SetCurrency(
    /* [in] */ ICurrency* currency)
{
    String symbol;
    currency->GetSymbol(&symbol);
    String currencyCode;
    currency->GetCurrencyCode(&currencyCode);
    FAIL_RETURN(SetSymbol(mAddr, UNUM_CURRENCY_SYMBOL, symbol));
    return SetSymbol(mAddr, UNUM_INTL_CURRENCY_SYMBOL, currencyCode);
}

ECode NativeDecimalFormat::SetGroupingSize(
    /* [in] */ Int32 value)
{
    SetAttribute(mAddr, UNUM_GROUPING_SIZE, value);
    return NOERROR;
}

ECode NativeDecimalFormat::SetGroupingUsed(
    /* [in] */ Boolean value)
{
    Int32 i = value ? -1 : 0;
    SetAttribute(mAddr, UNUM_GROUPING_USED, i);
    return NOERROR;
}

ECode NativeDecimalFormat::SetMaximumFractionDigits(
    /* [in] */ Int32 value)
{
    SetAttribute(mAddr, UNUM_MAX_FRACTION_DIGITS, value);
    return NOERROR;
}

ECode NativeDecimalFormat::SetMaximumIntegerDigits(
    /* [in] */ Int32 value)
{
    SetAttribute(mAddr, UNUM_MAX_INTEGER_DIGITS, value);
    return NOERROR;
}

ECode NativeDecimalFormat::SetMinimumFractionDigits(
    /* [in] */ Int32 value)
{
    SetAttribute(mAddr, UNUM_MIN_FRACTION_DIGITS, value);
    return NOERROR;
}

ECode NativeDecimalFormat::SetMinimumIntegerDigits(
    /* [in] */ Int32 value)
{
    SetAttribute(mAddr, UNUM_MIN_INTEGER_DIGITS, value);
    return NOERROR;
}

ECode NativeDecimalFormat::SetMultiplier(
    /* [in] */ Int32 value)
{
    SetAttribute(mAddr, UNUM_MULTIPLIER, value);
    // Update the cached BigDecimal for multiplier.
    assert(0);
//    multiplierBigDecimal = BigDecimal.valueOf(value);
    return NOERROR;
}

ECode NativeDecimalFormat::SetNegativePrefix(
    /* [in] */ const String& value)
{
    mNegPrefNull = value.IsNull();
    if (!mNegPrefNull) {
        return SetTextAttribute(mAddr, UNUM_NEGATIVE_PREFIX, value);
    }
    return NOERROR;
}

ECode NativeDecimalFormat::SetNegativeSuffix(
    /* [in] */ const String& value)
{
    mNegSuffNull = value.IsNull();
    if (!mNegSuffNull) {
        return SetTextAttribute(mAddr, UNUM_NEGATIVE_SUFFIX, value);
    }
    return NOERROR;
}

ECode NativeDecimalFormat::SetPositivePrefix(
    /* [in] */ const String& value)
{
    mPosPrefNull = value.IsNull();
    if (!mPosPrefNull) {
        return SetTextAttribute(mAddr, UNUM_POSITIVE_PREFIX, value);
    }
    return NOERROR;
}

ECode NativeDecimalFormat::SetPositiveSuffix(
    /* [in] */ const String& value)
{
    mPosSuffNull = value.IsNull();
    if (!mPosSuffNull) {
        return SetTextAttribute(mAddr, UNUM_POSITIVE_SUFFIX, value);
    }
    return NOERROR;
}

ECode NativeDecimalFormat::SetParseBigDecimal(
    /* [in] */ Boolean value)
{
    mParseBigDecimal = value;
    return NOERROR;
}

ECode NativeDecimalFormat::SetParseIntegerOnly(
    /* [in] */ Boolean value)
{
    Int32 i = value ? -1 : 0;
    SetAttribute(mAddr, UNUM_PARSE_INT_ONLY, i);
    return NOERROR;
}

ECode NativeDecimalFormat::ApplyPattern(
    /* [in] */ Handle32 addr,
    /* [in] */ Boolean localized,
    /* [in] */ const String& pattern)
{
    //try {
    return ApplyPatternImpl(addr, localized, pattern);
    //} catch (NullPointerException npe) {
    //    throw npe;
    //} catch (RuntimeException re) {
    //    throw new IllegalArgumentException("syntax error: " + re.getMessage() + ": " + pattern);
    //}
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
            return E_ASSERTION_ERROR;
    }
    SetRoundingMode(mAddr, nativeRoundingMode, roundingIncrement);
    return NOERROR;
}

static U_ICU_NAMESPACE::DecimalFormat* toDecimalFormat(Handle32 addr)
{
    return reinterpret_cast<U_ICU_NAMESPACE::DecimalFormat*>(static_cast<uintptr_t>(addr));
}

static UNumberFormat* toUNumberFormat(Handle32 addr)
{
    return reinterpret_cast<UNumberFormat*>(static_cast<uintptr_t>(addr));
}

static U_ICU_NAMESPACE::DecimalFormatSymbols* makeDecimalFormatSymbols(
    /* [in] */ const String& currencySymbol,
    /* [in] */ Char32 decimalSeparator,
    /* [in] */ Char32 digit,
    /* [in] */ const String& exponentSeparator,
    /* [in] */ Char32 groupingSeparator,
    /* [in] */ const String& infinity,
    /* [in] */ const String& internationalCurrencySymbol,
    /* [in] */ Char32 minusSign,
    /* [in] */ Char32 monetaryDecimalSeparator,
    /* [in] */ const String& nan,
    /* [in] */ Char32 patternSeparator,
    /* [in] */ Char32 percent,
    /* [in] */ Char32 perMill,
    /* [in] */ Char32 zeroDigit)
{
    U_ICU_NAMESPACE::DecimalFormatSymbols* result = new U_ICU_NAMESPACE::DecimalFormatSymbols;
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kCurrencySymbol,
            UnicodeString::fromUTF8(currencySymbol.string()));
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kDecimalSeparatorSymbol,
            UnicodeString((UChar32)decimalSeparator));
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kDigitSymbol,
            UnicodeString((UChar32)digit));
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kExponentialSymbol,
            UnicodeString::fromUTF8(exponentSeparator.string()));
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kGroupingSeparatorSymbol,
            UnicodeString((UChar32)groupingSeparator));
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kMonetaryGroupingSeparatorSymbol,
            UnicodeString((UChar32)groupingSeparator));
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kInfinitySymbol,
            UnicodeString::fromUTF8(infinity.string()));
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kIntlCurrencySymbol,
            UnicodeString::fromUTF8(internationalCurrencySymbol.string()));
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kMinusSignSymbol,
            UnicodeString((UChar32)minusSign));
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kMonetarySeparatorSymbol,
            UnicodeString((UChar32)monetaryDecimalSeparator));
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kNaNSymbol,
            UnicodeString::fromUTF8(nan.string()));
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kPatternSeparatorSymbol,
            UnicodeString((UChar32)patternSeparator));
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kPercentSymbol,
            UnicodeString((UChar32)percent));
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kPerMillSymbol,
            UnicodeString((UChar32)perMill));
    result->setSymbol(U_ICU_NAMESPACE::DecimalFormatSymbols::kZeroDigitSymbol,
            UnicodeString((UChar32)zeroDigit));
    return result;
}

/**
* Checks if an error has occurred, throwing a suitable exception if so.
* @param env JNI environment
* @param errorCode code to determine if it is an error
* @return 0 if errorCode is not an error, 1 if errorCode is an error, but the
*         creation of the exception to be thrown fails
 * @exception thrown if errorCode represents an error
*/
static ECode icu4_error(UErrorCode errorCode)
{
//    const char* message = u_errorName(errorCode);
    if (errorCode <= U_ZERO_ERROR || errorCode >= U_ERROR_LIMIT) {
        return NOERROR;
    }

    switch (errorCode) {
    case U_ILLEGAL_ARGUMENT_ERROR:
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    case U_INDEX_OUTOFBOUNDS_ERROR:
    case U_BUFFER_OVERFLOW_ERROR:
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    case U_UNSUPPORTED_ERROR:
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    default:
        return E_RUNTIME_EXCEPTION;
    }
}

ECode NativeDecimalFormat::ApplyPatternImpl(
    /* [in] */ Handle32 addr,
    /* [in] */ Boolean localized,
    /* [in] */ const String& pattern)
{
    if (pattern.IsNull()) {
        // jniThrowNullPointerException(env, NULL);
        return E_NULL_POINTER_EXCEPTION;
    }
    U_ICU_NAMESPACE::DecimalFormat* fmt = toDecimalFormat(addr);
    UErrorCode status = U_ZERO_ERROR;
    if (localized) {
        fmt->applyLocalizedPattern(
            UnicodeString::fromUTF8(pattern.string()), status);
    }
    else {
        fmt->applyPattern(
            UnicodeString::fromUTF8(pattern.string()), status);
    }
    return icu4_error(status);
}

Handle32 NativeDecimalFormat::CloneImpl(
    /* [in] */ Handle32 addr)
{
    U_ICU_NAMESPACE::DecimalFormat* fmt = toDecimalFormat(addr);
    return static_cast<Handle32>(reinterpret_cast<uintptr_t>(fmt->clone()));
}

void NativeDecimalFormat::Close(
    /* [in] */ Handle32 addr)
{
    delete toDecimalFormat(addr);
}

static ArrayOf<Char32>* formatResult(
    /* [in] */ const UnicodeString &str,
    /* [in] */ U_ICU_NAMESPACE::FieldPositionIterator* fpi,
    /* [in] */ NativeDecimalFormat::FieldPositionIterator* fpIter)
{
    if (fpi != NULL) {
        int len = fpi->getData(NULL, 0);
        ArrayOf<Int32>* data = NULL;
        if (len > 0) {
            data = ArrayOf<Int32>::Alloc(len);
            // ScopedIntArrayRW ints(env, data);
            // if (ints.get() == NULL) {
            //     return NULL;
            // }
            fpi->getData(data->GetPayload(), len);
        }
        fpIter->SetData(data);
    }

    int len = str.length();
    ArrayOf<Char32>* result = ArrayOf<Char32>::Alloc(len);
    if (result != NULL) {
        String s("");
        ElStringByteSink sink(&s);
        str.toUTF8(sink);
        for (Int32 i = 0; i < s.GetLength(); ++i) {
            (*result)[i] = s[i];
        }
    }
    return result;
}

ArrayOf<Char32>* NativeDecimalFormat::FormatInt64(
    /* [in] */ Handle32 addr,
    /* [in] */ Int64 value,
    /* [in] */ NativeDecimalFormat::FieldPositionIterator* fpIter)
{
    UErrorCode status = U_ZERO_ERROR;
    UnicodeString str;
    U_ICU_NAMESPACE::DecimalFormat* fmt = toDecimalFormat(addr);
    U_ICU_NAMESPACE::FieldPositionIterator fpi;
    U_ICU_NAMESPACE::FieldPositionIterator* pfpi = fpIter ? &fpi : NULL;
    fmt->format(value, str, pfpi, status);
    return formatResult(str, pfpi, fpIter);
}

ArrayOf<Char32>* NativeDecimalFormat::FormatDouble(
    /* [in] */ Handle32 addr,
    /* [in] */ Double value,
    /* [in] */ NativeDecimalFormat::FieldPositionIterator* fpIter)
{
    UErrorCode status = U_ZERO_ERROR;
    UnicodeString str;
    U_ICU_NAMESPACE::DecimalFormat* fmt = toDecimalFormat(addr);
    U_ICU_NAMESPACE::FieldPositionIterator fpi;
    U_ICU_NAMESPACE::FieldPositionIterator* pfpi = fpIter ? &fpi : NULL;
    fmt->format(value, str, pfpi, status);
    return formatResult(str, pfpi, fpIter);
}

ArrayOf<Char32>* NativeDecimalFormat::FormatDigitList(
    /* [in] */ Handle32 addr,
    /* [in] */ const String& value,
    /* [in] */ NativeDecimalFormat::FieldPositionIterator* fpIter)
{
    if (value.string() == NULL) {
        return NULL;
    }
    StringPiece sp(value.string());

    UErrorCode status = U_ZERO_ERROR;
    UnicodeString str;
    U_ICU_NAMESPACE::DecimalFormat* fmt = toDecimalFormat(addr);
    U_ICU_NAMESPACE::FieldPositionIterator fpi;
    U_ICU_NAMESPACE::FieldPositionIterator* pfpi = fpIter ? &fpi : NULL;
    fmt->format(sp, str, pfpi, status);
    return formatResult(str, pfpi, fpIter);
}

Int32 NativeDecimalFormat::GetAttribute(
    /* [in] */ Handle32 addr,
    /* [in] */ Int32 symbol)
{
    UNumberFormatAttribute attr = static_cast<UNumberFormatAttribute>(symbol);
    return unum_getAttribute(toUNumberFormat(addr), attr);
}

ECode NativeDecimalFormat::GetTextAttribute(
    /* [in] */ Handle32 addr,
    /* [in] */ Int32 symbol,
    /* [out] */ String* _attr)
{
    UErrorCode status = U_ZERO_ERROR;
    UNumberFormat* fmt = toUNumberFormat(addr);
    UNumberFormatTextAttribute attr = static_cast<UNumberFormatTextAttribute>(symbol);

    // Find out how long the result will be...
    UChar* chars;
    uint32_t charCount = 0;
    uint32_t desiredCount = unum_getTextAttribute(fmt, attr, chars, charCount, &status);
    if (status == U_BUFFER_OVERFLOW_ERROR) {
        // ...then get it.
        status = U_ZERO_ERROR;
        charCount = desiredCount + 1;
        chars = new UChar[charCount];
        charCount = unum_getTextAttribute(fmt, attr, chars, charCount, &status);
    }
    ElStringByteSink sink(_attr);
    UnicodeString(chars, charCount).toUTF8(sink);
    delete[] chars;
    return icu4_error(status);
}

ECode NativeDecimalFormat::Open(
    /* [in] */ const String& pattern,
    /* [in] */ const String& currencySymbol,
    /* [in] */ Char32 decimalSeparator,
    /* [in] */ Char32 digit,
    /* [in] */ const String& exponentSeparator,
    /* [in] */ Char32 groupingSeparator,
    /* [in] */ const String& infinity,
    /* [in] */ const String& internationalCurrencySymbol,
    /* [in] */ Char32 minusSign,
    /* [in] */ Char32 monetaryDecimalSeparator,
    /* [in] */ const String& nan,
    /* [in] */ Char32 patternSeparator,
    /* [in] */ Char32 percent,
    /* [in] */ Char32 perMill,
    /* [in] */ Char32 zeroDigit,
    /* [out] */ Handle32* addr)
{
    // if (pattern.IsNull()) {
    //     // jniThrowNullPointerException(env, NULL);
    //     *addr = 0;
    //     return E_NULL_POINTER_EXCEPTION;
    // }
    UErrorCode status = U_ZERO_ERROR;
    UParseError parseError;
    U_ICU_NAMESPACE::DecimalFormatSymbols* symbols = makeDecimalFormatSymbols(
            currencySymbol, decimalSeparator, digit, exponentSeparator, groupingSeparator,
            infinity, internationalCurrencySymbol, minusSign,
            monetaryDecimalSeparator, nan, patternSeparator, percent, perMill,
            zeroDigit);
    U_ICU_NAMESPACE::DecimalFormat* fmt = new U_ICU_NAMESPACE::DecimalFormat(
            UnicodeString::fromUTF8(pattern.string()), symbols, parseError, status);
    if (fmt == NULL) {
        delete symbols;
    }
    FAIL_RETURN(icu4_error(status));
    *addr = static_cast<Handle32>(reinterpret_cast<uintptr_t>(fmt));
    return NOERROR;
}

AutoPtr<INumber> NativeDecimalFormat::Parse(
    /* [in] */ Handle32 addr,
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [in] */ Boolean parseBigDecimal)
{
    assert(0);
    // static jmethodID gPP_getIndex = env->GetMethodID(JniConstants::parsePositionClass, "getIndex", "()I");
    // static jmethodID gPP_setIndex = env->GetMethodID(JniConstants::parsePositionClass, "setIndex", "(I)V");
    // static jmethodID gPP_setErrorIndex = env->GetMethodID(JniConstants::parsePositionClass, "setErrorIndex", "(I)V");

    // // make sure the ParsePosition is valid. Actually icu4c would parse a number
    // // correctly even if the parsePosition is set to -1, but since the RI fails
    // // for that case we have to fail too
    // int parsePos = env->CallIntMethod(position, gPP_getIndex, NULL);
    // if (parsePos < 0 || parsePos > env->GetStringLength(text)) {
    //     return NULL;
    // }

    // Formattable res;
    // ParsePosition pp(parsePos);
    // ScopedJavaUnicodeString src(env, text);
    // DecimalFormat* fmt = toDecimalFormat(addr);
    // fmt->parse(src.unicodeString(), res, pp);

    // if (pp.getErrorIndex() == -1) {
    //     env->CallVoidMethod(position, gPP_setIndex, (jint) pp.getIndex());
    // } else {
    //     env->CallVoidMethod(position, gPP_setErrorIndex, (jint) pp.getErrorIndex());
    //     return NULL;
    // }

    // if (parseBigDecimal) {
    //     UErrorCode status = U_ZERO_ERROR;
    //     StringPiece str = res.getDecimalNumber(status);
    //     if (U_SUCCESS(status)) {
    //         int len = str.length();
    //         const char* data = str.data();
    //         if (strncmp(data, "NaN", 3) == 0 ||
    //             strncmp(data, "Inf", 3) == 0 ||
    //             strncmp(data, "-Inf", 4) == 0) {
    //             double resultDouble = res.getDouble(status);
    //             return doubleValueOf(env, (jdouble) resultDouble);
    //         }
    //         return newBigDecimal(env, data, len);
    //     }
    //     return NULL;
    // }

    // Formattable::Type numType = res.getType();
    //     switch(numType) {
    //     case Formattable::kDouble: {
    //         double resultDouble = res.getDouble();
    //         return doubleValueOf(env, (jdouble) resultDouble);
    //     }
    //     case Formattable::kLong: {
    //         long resultLong = res.getLong();
    //         return longValueOf(env, (jlong) resultLong);
    //     }
    //     case Formattable::kInt64: {
    //         int64_t resultInt64 = res.getInt64();
    //         return longValueOf(env, (jlong) resultInt64);
    //     }
    //     default: {
    //         return NULL;
    //     }
    // }
    return NULL;
}

void NativeDecimalFormat::SetDecimalFormatSymbols(
    /* [in] */ Handle32 addr,
    /* [in] */ const String& currencySymbol,
    /* [in] */ Char32 decimalSeparator,
    /* [in] */ Char32 digit,
    /* [in] */ const String& exponentSeparator,
    /* [in] */ Char32 groupingSeparator,
    /* [in] */ const String& infinity,
    /* [in] */ const String& internationalCurrencySymbol,
    /* [in] */ Char32 minusSign,
    /* [in] */ Char32 monetaryDecimalSeparator,
    /* [in] */ const String& nan,
    /* [in] */ Char32 patternSeparator,
    /* [in] */ Char32 percent,
    /* [in] */ Char32 perMill,
    /* [in] */ Char32 zeroDigit)
{
    U_ICU_NAMESPACE::DecimalFormatSymbols* symbols = makeDecimalFormatSymbols(
            currencySymbol, decimalSeparator, digit, exponentSeparator, groupingSeparator,
            infinity, internationalCurrencySymbol, minusSign,
            monetaryDecimalSeparator, nan, patternSeparator, percent, perMill,
            zeroDigit);
    toDecimalFormat(addr)->adoptDecimalFormatSymbols(symbols);
}

ECode NativeDecimalFormat::SetSymbol(
    /* [in] */ Handle32 addr,
    /* [in] */ Int32 _symbol,
    /* [in] */ const String& str)
{
    UnicodeString _s = UnicodeString::fromUTF8(str.string());
    UnicodeString& s(_s);
    UErrorCode status = U_ZERO_ERROR;
    UNumberFormatSymbol symbol = static_cast<UNumberFormatSymbol>(_symbol);
    unum_setSymbol(toUNumberFormat(addr), symbol, s.getBuffer(), s.length(), &status);
    return icu4_error(status);
}

void NativeDecimalFormat::SetAttribute(
    /* [in] */ Handle32 addr,
    /* [in] */ Int32 symbol,
    /* [in] */ Int32 i)
{
    UNumberFormatAttribute attr = static_cast<UNumberFormatAttribute>(symbol);
    unum_setAttribute(toUNumberFormat(addr), attr, i);
}

void NativeDecimalFormat::SetRoundingMode(
    /* [in] */ Handle32 addr,
    /* [in] */ Int32 mode,
    /* [in] */ Double increment)
{
    U_ICU_NAMESPACE::DecimalFormat* fmt = toDecimalFormat(addr);
    fmt->setRoundingMode(static_cast<U_ICU_NAMESPACE::DecimalFormat::ERoundingMode>(mode));
    fmt->setRoundingIncrement(increment);
}

ECode NativeDecimalFormat::SetTextAttribute(
    /* [in] */ Handle32 addr,
    /* [in] */ Int32 symbol,
    /* [in] */ const String& str)
{
    UnicodeString _s = UnicodeString::fromUTF8(str.string());
    UnicodeString& s(_s);
    UErrorCode status = U_ZERO_ERROR;
    UNumberFormatTextAttribute attr = static_cast<UNumberFormatTextAttribute>(symbol);
    unum_setTextAttribute(toUNumberFormat(addr), attr, s.getBuffer(), s.length(), &status);
    return icu4_error(status);
}

String NativeDecimalFormat::ToPatternImpl(
    /* [in] */ Handle32 addr,
    /* [in] */ Boolean localized)
{
    U_ICU_NAMESPACE::DecimalFormat* fmt = toDecimalFormat(addr);
    UnicodeString pattern;
    if (localized) {
        fmt->toLocalizedPattern(pattern);
    }
    else {
        fmt->toPattern(pattern);
    }
    String s("");
    ElStringByteSink sink(&s);
    pattern.toUTF8(sink);
    return s;
}
