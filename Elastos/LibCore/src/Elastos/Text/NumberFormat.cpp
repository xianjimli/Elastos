
#include "cmdef.h"
#include "NumberFormat.h"
#include <Elastos.IO.h>
#include "CNumberFormatField.h"


static AutoPtr<INumberFormatField> sInit(const String& name)
{
    // AutoPtr<CNumberFormatField> field;
    // CNumberFormatField::NewByFriend(name, (CNumberFormatField**)&field);
    // return field.Get();
    return NULL;
}

const AutoPtr<INumberFormatField> NumberFormat::Field::SIGN = sInit(String("sign"));
const AutoPtr<INumberFormatField> NumberFormat::Field::INTEGER = sInit(String("integer"));
const AutoPtr<INumberFormatField> NumberFormat::Field::FRACTION = sInit(String("fraction"));
const AutoPtr<INumberFormatField> NumberFormat::Field::EXPONENT = sInit(String("exponent"));
const AutoPtr<INumberFormatField> NumberFormat::Field::EXPONENT_SIGN = sInit(String("exponent sign"));
const AutoPtr<INumberFormatField> NumberFormat::Field::EXPONENT_SYMBOL = sInit(String("exponent symbol"));
const AutoPtr<INumberFormatField> NumberFormat::Field::DECIMAL_SEPARATOR = sInit(String("decimal separator"));
const AutoPtr<INumberFormatField> NumberFormat::Field::GROUPING_SEPARATOR = sInit(String("grouping separator"));
const AutoPtr<INumberFormatField> NumberFormat::Field::PERCENT = sInit(String("percent"));
const AutoPtr<INumberFormatField> NumberFormat::Field::PERMILLE = sInit(String("per mille"));
const AutoPtr<INumberFormatField> NumberFormat::Field::CURRENCY = sInit(String("currency"));

ECode NumberFormat::Field::Init(
   /* [in] */ const String& fn)
{
   return Format::Field::Init(fn);
}

ECode NumberFormat::Field::ReadResolve(
    /* [out] */ IInterface** object)
{
    INumberFormatField* field =
            reinterpret_cast<INumberFormatField*>(this->Probe(EIID_INumberFormatField));

    if (field == INTEGER.Get()) {
        *object = INTEGER.Get();
        (*object)->AddRef();
        return NOERROR;
    }
    if (field == FRACTION.Get()) {
        *object = FRACTION.Get();
        (*object)->AddRef();
        return NOERROR;
    }
    if (field == EXPONENT.Get()) {
        *object = EXPONENT.Get();
        (*object)->AddRef();
        return NOERROR;
    }
    if (field == EXPONENT_SIGN.Get()) {
        *object = EXPONENT_SIGN.Get();
        (*object)->AddRef();
        return NOERROR;
    }
    if (field == EXPONENT_SYMBOL.Get()) {
        *object = EXPONENT_SYMBOL.Get();
        (*object)->AddRef();
        return NOERROR;
    }
    if (field == CURRENCY.Get()) {
        *object = CURRENCY.Get();
        (*object)->AddRef();
        return NOERROR;
    }
    if (field == DECIMAL_SEPARATOR.Get()) {
        *object = DECIMAL_SEPARATOR.Get();
        (*object)->AddRef();
        return NOERROR;
    }
    if (field == GROUPING_SEPARATOR.Get()) {
        *object = GROUPING_SEPARATOR.Get();
        (*object)->AddRef();
        return NOERROR;
    }
    if (field == PERCENT.Get()) {
        *object = PERCENT.Get();
        (*object)->AddRef();
        return NOERROR;
    }
    if (field == PERMILLE.Get()) {
        *object = PERMILLE.Get();
        (*object)->AddRef();
        return NOERROR;
    }
    if (field == SIGN.Get()) {
        *object = SIGN.Get();
        (*object)->AddRef();
        return NOERROR;
    }
    // throw new InvalidObjectException("Unknown attribute");
    return E_INVALID_OBJECT_EXCEPTION;
}


NumberFormat::NumberFormat()
    : mGroupingUsed(TRUE)
    , mParseIntegerOnly(FALSE)
    , mMaximumIntegerDigits(40)
    , mMinimumIntegerDigits(1)
    , mMaximumFractionDigits(3)
    , mMinimumFractionDigits(0)
{}

ECode NumberFormat::FormatDouble(
    /* [in] */ Double value,
    /* [out] */ String* result)
{
    AutoPtr<IFieldPosition> position;
    CFieldPosition::New(0, (IFieldPosition**)&position);
    return FormatDoubleEx(value, String(NULL), position, result);
}

ECode NumberFormat::FormatInt64(
    /* [in] */ Int64 value,
    /* [out] */ String* result)
{
    AutoPtr<IFieldPosition> position;
    CFieldPosition::New(0, (IFieldPosition**)&position);
    return FormatInt64Ex(value, String(NULL), position, result);
}

ECode NumberFormat::FormatObjectEx(
    /* [in] */ IInterface* object,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* value)
{
    assert(0);
//    if (object->Probe(EIID_Byte) != NULL
//                || object->Probe(EIID_Short) != NULL
//                || object->Probe(EIID_Int32) != NULL
//                || object->Probe(EIID_Int64) != NULL
//                /*|| (object instanceof BigInteger && ((BigInteger) object).bitLength() < 64)*/) {
//        Int64 lv = (Int64)((INumber*) object)/*.longValue()*/;
//        return formatEx5(lv, buffer, field, value);
//    } else if (object->Probe(EIID_INumber) != NULL) {
//        Double dv = (Double)((INumber*) object)/*.doubleValue()*/;
//        return formatEx3(dv, buffer, field, value);
//    }
    //throw new IllegalArgumentException();
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode NumberFormat::GetAvailableLocales(
    /* [out] */ ArrayOf<ILocale*>** locales)
{
    AutoPtr<IICUHelper> ICUHelper;
    FAIL_RETURN(CICUHelper::AcquireSingleton((IICUHelper **)&ICUHelper));
    return ICUHelper->GetAvailableNumberFormatLocales(locales);
}

ECode NumberFormat::GetCurrency(
    /* [out] */ ICurrency** currency)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode NumberFormat::GetCurrencyInstance(
    /* [out] */ INumberFormat** instance)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetCurrencyInstance(locale, instance);
}

ECode NumberFormat::GetCurrencyInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    assert(0);
    //return getInstance(LocaleData.get(locale).currencyPattern, locale);
    return NOERROR;
}

ECode NumberFormat::GetIntegerInstance(
        /* [out] */ INumberFormat** instance)
{
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    return GetIntegerInstance(pLocale, instance);
}

ECode NumberFormat::GetIntegerInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    assert(0);
    //NumberFormat result = getInstance(LocaleData.get(locale).integerPattern, locale);
    //result.setParseIntegerOnly(true);
    //return result;
    return NOERROR;
}

ECode NumberFormat::GetInstance(
    /* [out] */ INumberFormat** instance)
{
    return GetNumberInstance(instance);
}

ECode NumberFormat::GetInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    return GetNumberInstance(locale, instance);
}

ECode NumberFormat::GetInstance(
    /* [in] */ const String& pattern,
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    assert(0);
//    return new DecimalFormat(pattern, locale);
    return NOERROR;
}

ECode NumberFormat::GetMaximumFractionDigits(
    /* [out] */ Int32* number)
{
    *number = mMaximumFractionDigits;
    return NOERROR;
}

ECode NumberFormat::GetMaximumIntegerDigits(
    /* [out] */ Int32* number)
{
    *number = mMaximumIntegerDigits;
    return NOERROR;
}

ECode NumberFormat::GetMinimumFractionDigits(
    /* [out] */ Int32* number)
{
    *number = mMinimumFractionDigits;
    return NOERROR;
}

ECode NumberFormat::GetMinimumIntegerDigits(
    /* [out] */ Int32* number)
{
    *number = mMinimumIntegerDigits;
    return NOERROR;
}

ECode NumberFormat::GetNumberInstance(
    /* [out] */ INumberFormat** instance)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetNumberInstance(locale, instance);
}

ECode NumberFormat::GetNumberInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    assert(0);
//    return getInstance(LocaleData.get(locale).numberPattern, locale);
    return NOERROR;
}

ECode NumberFormat::GetPercentInstance(
    /* [out] */ INumberFormat** instance)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetPercentInstance(locale, instance);
}

ECode NumberFormat::GetPercentInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    assert(0);
//    return getInstance(LocaleData.get(locale).percentPattern, locale);
    return NOERROR;
}

ECode NumberFormat::IsGroupingUsed(
    /* [out] */ Boolean* isGroupingUsed)
{
    *isGroupingUsed = mGroupingUsed;
    return NOERROR;
}

ECode NumberFormat::IsParseIntegerOnly(
    /* [out] */ Boolean* isParseIntegerOnly)
{
    *isParseIntegerOnly = mParseIntegerOnly;
    return NOERROR;
}

ECode NumberFormat::Parse(
    /* [in] */ const String& string,
    /* [out] */ INumber** number)
{
    AutoPtr<IParsePosition> pos;
    CParsePosition::New(0, (IParsePosition**)&pos);
    FAIL_RETURN(ParseEx(string, pos, number));
    Int32 index;
    pos->GetIndex(&index);
    if (index == 0) {
        //throw new ParseException("Unparseable number" + string, pos.getErrorIndex());
        return E_PARSE_EXCEPTION;
    }
    return NOERROR;
}

ECode NumberFormat::ParseObjectEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ IInterface** object)
{
    if (position == NULL) {
        //throw new NullPointerException("position is null");
        return E_NULL_POINTER_EXCEPTION;
    }
    //try {
    return ParseEx(string, position, (INumber**)object);
    //} catch (Exception e) {
    //    return null;
    //}
}

ECode NumberFormat::SetCurrency(
   /* [in] */ ICurrency* currency)
{
    //throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode NumberFormat::SetGroupingUsed(
    /* [in] */ Boolean value)
{
    mGroupingUsed = value;
    return NOERROR;
}

ECode NumberFormat::SetMaximumFractionDigits(
    /* [in] */ Int32 value)
{
    mMaximumFractionDigits = value < 0 ? 0 : value;
    if (mMaximumFractionDigits < mMinimumFractionDigits) {
        mMinimumFractionDigits = mMaximumFractionDigits;
    }
    return NOERROR;
}

ECode NumberFormat::SetMaximumIntegerDigits(
    /* [in] */ Int32 value)
{
    mMaximumIntegerDigits = value < 0 ? 0 : value;
    if (mMaximumIntegerDigits < mMinimumIntegerDigits) {
        mMinimumIntegerDigits = mMaximumIntegerDigits;
    }
    return NOERROR;
}

ECode NumberFormat::SetMinimumFractionDigits(
    /* [in] */ Int32 value)
{
    mMinimumFractionDigits = value < 0 ? 0 : value;
    if (mMaximumFractionDigits < mMinimumFractionDigits) {
        mMaximumFractionDigits = mMinimumFractionDigits;
    }
    return NOERROR;
}

ECode NumberFormat::SetMinimumIntegerDigits(
    /* [in] */ Int32 value)
{
    mMinimumIntegerDigits = value < 0 ? 0 : value;
    if (mMaximumIntegerDigits < mMinimumIntegerDigits) {
        mMaximumIntegerDigits = mMinimumIntegerDigits;
    }
    return NOERROR;
}

ECode NumberFormat::SetParseIntegerOnly(
    /* [in] */ Boolean value)
{
    mParseIntegerOnly = value;
    return NOERROR;
}

ECode NumberFormat::GetRoundingMode(
    /* [out] */ RoundingMode* roundingMode)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode NumberFormat::SetRoundingMode(
    /* [in] */ RoundingMode roundingMode)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}
