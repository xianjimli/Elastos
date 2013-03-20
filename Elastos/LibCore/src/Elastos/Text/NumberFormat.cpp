#include "NumberFormat.h"

NumberFormat::NumberFormat() : groupingUsed(TRUE),
                               parseIntegerOnly(FALSE),
                               maximumIntegerDigits(40),
                               minimumIntegerDigits(1),
                               maximumFractionDigits(3),
                               minimumFractionDigits(0)
{
}

NumberFormat::~NumberFormat()
{
}

ECode NumberFormat::formatEx2(
        /* [in] */ Double value,
        /* [out] */ String* formattedString)
{
    VALIDATE_NOT_NULL(formattedString);
    StringBuffer* buffer = new StringBuffer("");
    StringBuffer* sb = new StringBuffer("");
    AutoPtr<IFieldPosition> position;
    CFieldPosition::New(0, (IFieldPosition**)&position);
    formatEx3(value, buffer, position, sb);
    *formattedString = sb->Substring(0, sb->GetLength());
    return NOERROR;
}

ECode NumberFormat::formatEx4(
    /* [in] */ Int64 value,
    /* [out] */ String* formattedString)
{
    VALIDATE_NOT_NULL(formattedString);
    StringBuffer* buffer = new StringBuffer("");
    StringBuffer* sb = new StringBuffer("");
    AutoPtr<IFieldPosition> position;
    CFieldPosition::New(0, (IFieldPosition**)&position);
    formatEx3(value, buffer, position, sb);
    *formattedString = sb->Substring(0, sb->GetLength());
    return NOERROR;
}

ECode NumberFormat::formatEx(
            /* [in] */ IInterface* object,
            /* [in] */ StringBuffer* buffer,
            /* [in] */ IFieldPosition* field,
            /* [out] */ StringBuffer* value)
{
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
    AutoPtr<IICUHelper> pICUHelper;
    CICUHelper::AcquireSingleton((IICUHelper **)&pICUHelper);
    return pICUHelper->GetAvailableNumberFormatLocales(locales);
}

//ECode NumberFormat::GetCurrency(
//        /* [out] */ ICurrency** currency)
//{
    //throw new UnsupportedOperationException();
//    return NOERROR;
//}

ECode NumberFormat::GetCurrencyInstance(
        /* [out] */ INumberFormat** instance)
{
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    return GetCurrencyInstance(pLocale, instance);
}

ECode NumberFormat::GetCurrencyInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance)
{
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
        /* [in] */ String pattern,
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance)
{
//    return new DecimalFormat(pattern, locale);
    return NOERROR;
}

ECode NumberFormat::GetMaximumFractionDigits(
        /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    *number = maximumFractionDigits;
    return NOERROR;
}

ECode NumberFormat::GetMaximumIntegerDigits(
        /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    *number = maximumIntegerDigits;
    return NOERROR;    
}

ECode NumberFormat::GetMinimumFractionDigits(
        /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    *number = minimumFractionDigits;
    return NOERROR;
}

ECode NumberFormat::GetMinimumIntegerDigits(
        /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    *number = minimumIntegerDigits;
    return NOERROR;    
}

ECode NumberFormat::GetNumberInstance(
        /* [out] */ INumberFormat** instance)
{
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    return GetNumberInstance(pLocale, instance);
}

ECode NumberFormat::GetNumberInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance)
{
//    return getInstance(LocaleData.get(locale).numberPattern, locale);
    return NOERROR;
}
        
ECode NumberFormat::GetPercentInstance(
    /* [out] */ INumberFormat** instance)
{
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    return GetPercentInstance(pLocale, instance);
}

ECode NumberFormat::GetPercentInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
//    return getInstance(LocaleData.get(locale).percentPattern, locale);
    return NOERROR;  
}

ECode NumberFormat::IsGroupingUsed(
        /* [out] */ Boolean* isGroupingUsed)
{
    VALIDATE_NOT_NULL(isGroupingUsed);
    *isGroupingUsed = groupingUsed;
    return NOERROR;
}

ECode NumberFormat::IsParseIntegerOnly(
        /* [out] */ Boolean* isParseIntegerOnly)
{
    VALIDATE_NOT_NULL(isParseIntegerOnly);
    *isParseIntegerOnly = parseIntegerOnly;
    return NOERROR;
}

ECode NumberFormat::Parse(
        /* [in] */ String string,
        /* [out] */ INumber** number)
{
    AutoPtr<IParsePosition> pos;
    CParsePosition::New(0, (IParsePosition**)&pos);
    ParseEx(string, pos, number);
    Int32 index;
    pos->GetIndex(&index);
    if (index == 0) {
        //throw new ParseException("Unparseable number" + string, pos.getErrorIndex());
        return NOERROR;
    }
    return NOERROR;
}

ECode NumberFormat::ParseObjectEx(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IInterface** object)
{
    if (position == NULL) {
        //throw new NullPointerException("position is null");
        return NOERROR;
    }
    //try {
        return ParseEx(string, position, (INumber**)object);
    //} catch (Exception e) {
    //    return null;
    //}
}

//ECode NumberFormat::SetCurrency(
//        /* [in] */ ICurrency* currency)
//{
    //throw new UnsupportedOperationException();
//    return NOERROR;
//}

ECode NumberFormat::SetGroupingUsed(
        /* [in] */ Boolean value)
{
    groupingUsed = value;
    return NOERROR;
}

ECode NumberFormat::SetMaximumFractionDigits(
        /* [in] */ Int32 value)
{
    maximumFractionDigits = value < 0 ? 0 : value;
    if (maximumFractionDigits < minimumFractionDigits) {
        minimumFractionDigits = maximumFractionDigits;
    }
    return NOERROR;
}

ECode NumberFormat::SetMaximumIntegerDigits(
        /* [in] */ Int32 value)
{
    maximumIntegerDigits = value < 0 ? 0 : value;
    if (maximumIntegerDigits < minimumIntegerDigits) {
        minimumIntegerDigits = maximumIntegerDigits;
    }
    return NOERROR;
}

ECode NumberFormat::SetMinimumFractionDigits(
        /* [in] */ Int32 value)
{
    minimumFractionDigits = value < 0 ? 0 : value;
    if (maximumFractionDigits < minimumFractionDigits) {
        maximumFractionDigits = minimumFractionDigits;
    }
    return NOERROR;
}

ECode NumberFormat::SetMinimumIntegerDigits(
        /* [in] */ Int32 value)
{
    minimumIntegerDigits = value < 0 ? 0 : value;
    if (maximumIntegerDigits < minimumIntegerDigits) {
        maximumIntegerDigits = minimumIntegerDigits;
    }
    return NOERROR;
}

ECode NumberFormat::SetParseIntegerOnly(
        /* [in] */ Boolean value)
{
    parseIntegerOnly = value;
    return NOERROR;
}

//ECode NumberFormat::GetRoundingMode(
//        /* [out] */ IRoundingMode** roundingMode)
//{
    //throw new UnsupportedOperationException();
//    return NOERROR;
//}

//ECode NumberFormat::SetRoundingMode(
//        /* [in] */ IRoundingMode* roundingMode)
//{
    //throw new UnsupportedOperationException();
//    return NOERROR;
//}

const AutoPtr<INumberFormat_Field> NumberFormat::NumberFormat_Field::SIGN;

const AutoPtr<INumberFormat_Field> NumberFormat::NumberFormat_Field::INTEGER;

const AutoPtr<INumberFormat_Field> NumberFormat::NumberFormat_Field::FRACTION;

const AutoPtr<INumberFormat_Field> NumberFormat::NumberFormat_Field::EXPONENT;

const AutoPtr<INumberFormat_Field> NumberFormat::NumberFormat_Field::EXPONENT_SIGN;

const AutoPtr<INumberFormat_Field> NumberFormat::NumberFormat_Field::EXPONENT_SYMBOL;

const AutoPtr<INumberFormat_Field> NumberFormat::NumberFormat_Field::DECIMAL_SEPARATOR;

const AutoPtr<INumberFormat_Field> NumberFormat::NumberFormat_Field::GROUPING_SEPARATOR;

const AutoPtr<INumberFormat_Field> NumberFormat::NumberFormat_Field::PERCENT;

const AutoPtr<INumberFormat_Field> NumberFormat::NumberFormat_Field::PERMILLE;

const AutoPtr<INumberFormat_Field> NumberFormat::NumberFormat_Field::CURRENCY;

//NumberFormat::NumberFormat_Field::NumberFormat_Field(
//    /* [in] */ String fn)
//{
//    Format::Format_Field::Format_Field(fn);
//}

ECode NumberFormat::NumberFormat_Field::ReadResolve(
            /* [out] */ IInterface** resolvedObject)
{
/*    if (this.equals(INTEGER)) {
        return INTEGER;
    }
    if (this.equals(FRACTION)) {
        return FRACTION;
    }
    if (this.equals(EXPONENT)) {
        return EXPONENT;
    }
    if (this.equals(EXPONENT_SIGN)) {
        return EXPONENT_SIGN;
    }
    if (this.equals(EXPONENT_SYMBOL)) {
        return EXPONENT_SYMBOL;
    }
    if (this.equals(CURRENCY)) {
        return CURRENCY;
    }
    if (this.equals(DECIMAL_SEPARATOR)) {
        return DECIMAL_SEPARATOR;
    }
    if (this.equals(GROUPING_SEPARATOR)) {
        return GROUPING_SEPARATOR;
    }
    if (this.equals(PERCENT)) {
        return PERCENT;
    }
    if (this.equals(PERMILLE)) {
        return PERMILLE;
    }
    if (this.equals(SIGN)) {
        return SIGN;
    }
    throw new InvalidObjectException("Unknown attribute");
*/
    return NOERROR;
}
