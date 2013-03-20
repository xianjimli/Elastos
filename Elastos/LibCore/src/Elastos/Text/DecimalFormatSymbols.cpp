#include "DecimalFormatSymbols.h"

DecimalFormatSymbols::~DecimalFormatSymbols()
{}

ECode DecimalFormatSymbols::Init()
{
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    return Init((ILocale*)pLocale);
}

ECode DecimalFormatSymbols::Init(
        /* [in] */ ILocale* locale)
{
/*
    LocaleData *localeData = LocaleData::Get(locale);
    mZeroDigit = localeData.mZeroDigit;
    mDigit = localeData.mDigit;
    mDecimalSeparator = localeData.mDecimalSeparator;
    mGroupingSeparator = localeData.mGroupingSeparator;
    mPatternSeparator = localeData.mPatternSeparator;
    mPercent = localeData.mPercent;
    mPerMill = localeData.mPerMill;
    mMonetarySeparator = localeData.mMonetarySeparator;
    mMinusSign = localeData.mMinusSign;
    mInfinity = localeData.mInfinity;
    mNaN = localeData.mNaN;
    mExponentSeparator = localeData.mExponentSeparator;
    mLocale = locale;
    //try {
        Currency::GetInstance(locale, (ICurrency**)&mCurrency);
        mCurrency->GetSymbolEx(locale, &mCurrencySymbol);
        mCurrency->GetCurrencyCode(&mIntlCurrencySymbol);
    //} catch (IllegalArgumentException e) {
    //    currency = Currency.getInstance("XXX");
    //    currencySymbol = localeData.currencySymbol;
    //    intlCurrencySymbol = localeData.internationalCurrencySymbol;
    //}
*/
    return NOERROR;
}

ECode DecimalFormatSymbols::GetInstance(
        /* [out] */ IDecimalFormatSymbols** instance)
{
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    return GetInstance((ILocale*)pLocale, instance);
}

ECode DecimalFormatSymbols::GetInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ IDecimalFormatSymbols** instance)
{
    if (locale == NULL) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
//    CDecimalFormatSymbols::New(locale, instance);
    return NOERROR;
}

ECode DecimalFormatSymbols::GetAvailableLocales(
        /* [out] */ ArrayOf<ILocale*>** locales)
{
    AutoPtr<IICUHelper> pICUHelper;
    CICUHelper::AcquireSingleton((IICUHelper**)&pICUHelper);
    return pICUHelper->GetAvailableDecimalFormatSymbolsLocales(locales);
}

ECode DecimalFormatSymbols::GetCurrency(
        /* [out] */ ICurrency** currency)
{
    *currency = (ICurrency*)mCurrency;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetInternationalCurrencySymbol(
        /* [out] */ String* symbol)
{
    VALIDATE_NOT_NULL(symbol);
    *symbol = mIntlCurrencySymbol;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetCurrencySymbol(
        /* [out] */ String* symbol)
{
    VALIDATE_NOT_NULL(symbol);
    *symbol = mCurrencySymbol;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetDecimalSeparator(
        /* [out] */ Char32* separator)
{
    VALIDATE_NOT_NULL(separator);
    *separator = mDecimalSeparator;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetDigit(
        /* [out] */ Char32* digit)
{
    VALIDATE_NOT_NULL(digit);
    *digit = mDigit;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetGroupingSeparator(
        /* [out] */ Char32* separator)
{
    VALIDATE_NOT_NULL(separator);
    *separator = mGroupingSeparator;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetInfinity(
        /* [out] */ String* infinity)
{
    VALIDATE_NOT_NULL(infinity);
    *infinity = mInfinity;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetMinusSign(
        /* [out] */ Char32* minusSign)
{
    VALIDATE_NOT_NULL(minusSign);
    *minusSign = mMinusSign;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetMonetaryDecimalSeparator(
        /* [out] */ Char32* separator)
{
    VALIDATE_NOT_NULL(separator);
    *separator = mMonetarySeparator;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetNaN(
            /* [out] */ String* infinity)
{
    VALIDATE_NOT_NULL(infinity);
    *infinity = mNaN;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetPatternSeparator(
        /* [out] */ Char32* separator)
{
    VALIDATE_NOT_NULL(separator);
    *separator = mPatternSeparator;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetPercent(
            /* [out] */ Char32* percent)
{
    VALIDATE_NOT_NULL(percent);
    *percent = mPercent;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetPerMill(
        /* [out] */ Char32* perMill)
{
    VALIDATE_NOT_NULL(perMill);
    *perMill = mPerMill;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetZeroDigit(
        /* [out] */ Char32* zeroDigit)
{
    VALIDATE_NOT_NULL(zeroDigit);
    *zeroDigit = mZeroDigit;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetExponentSeparator(
        /* [out] */ String* separator)
{
    VALIDATE_NOT_NULL(separator);
    *separator = mExponentSeparator;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetCurrency(
        /* [in] */ ICurrency* currency)
{
    if (currency == NULL) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (currency == mCurrency) {
        return NULL;
    }
    mCurrency = currency;
    currency->GetCurrencyCode(&mIntlCurrencySymbol);
    currency->GetSymbolEx(mLocale, &mCurrencySymbol);
    return NOERROR;
}

ECode DecimalFormatSymbols::SetInternationalCurrencySymbol(
        /* [in] */ String value)
{
    if (value.IsNull()) {
        mCurrency = NULL;
        mIntlCurrencySymbol = String(NULL);
        return NULL;
    }

    if (value.Equals(mIntlCurrencySymbol)) {
        return NULL;
    }

    //try {
//        Currency::GetInstance(value, (ICurrency**)&mCurrency);
        mCurrency->GetSymbolEx(mLocale, &mCurrencySymbol);
    //} catch (IllegalArgumentException e) {
    //    currency = null;
    //}
    mIntlCurrencySymbol = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetCurrencySymbol(
        /* [in] */ String value)
{
    mCurrencySymbol = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetDecimalSeparator(
        /* [in] */ Char32 value)
{
    mDecimalSeparator = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetDigit(
        /* [in] */ Char32 value)
{
    mDigit = value;
    return NOERROR;
}
ECode DecimalFormatSymbols::SetGroupingSeparator(
            /* [in] */ Char32 value)
{
    mGroupingSeparator = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetInfinity(
        /* [in] */ String value)
{
    mInfinity = value;
    return NOERROR;
}
ECode DecimalFormatSymbols::SetMinusSign(
                /* [in] */ Char32 value)
{
    mMinusSign = value;
    return NOERROR;
}
ECode DecimalFormatSymbols::SetMonetaryDecimalSeparator(
                    /* [in] */ Char32 value)
{
    mMonetarySeparator = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetNaN(
        /* [in] */ String value)
{
    mNaN = value;
    return NOERROR;
}
ECode DecimalFormatSymbols::SetPatternSeparator(
    /* [in] */ Char32 value)
{
    mPatternSeparator = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetPercent(
    /* [in] */ Char32 value)
{
    mPercent = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetPerMill(
    /* [in] */ Char32 value)
{
    mPerMill = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetZeroDigit(
    /* [in] */ Char32 value)
{
    mZeroDigit = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetExponentSeparator(
    /* [in] */ String value)
{
    if (value.IsNull()) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    mExponentSeparator = value;
    return NOERROR;
}