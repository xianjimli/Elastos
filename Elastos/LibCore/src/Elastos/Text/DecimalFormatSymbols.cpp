
#include "cmdef.h"
#include "DecimalFormatSymbols.h"
#include "CDecimalFormatSymbols.h"


ECode DecimalFormatSymbols::Init()
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return Init(locale);
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
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode DecimalFormatSymbols::Clone(
    /* [out] */ IDecimalFormatSymbols** object)
{
    assert(0);
   // try {
   //     return super.clone();
   // } catch (CloneNotSupportedException e) {
   //     throw new AssertionError(e);
   // }
    return E_NOT_IMPLEMENTED;
}

ECode DecimalFormatSymbols::GetCurrency(
    /* [out] */ ICurrency** currency)
{
    *currency = mCurrency;
    if (*currency != NULL) (*currency)->AddRef();
    return NOERROR;
}

ECode DecimalFormatSymbols::GetInternationalCurrencySymbol(
    /* [out] */ String* symbol)
{
    *symbol = mIntlCurrencySymbol;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetCurrencySymbol(
    /* [out] */ String* symbol)
{
    *symbol = mCurrencySymbol;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetDecimalSeparator(
    /* [out] */ Char32* separator)
{
    *separator = mDecimalSeparator;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetDigit(
    /* [out] */ Char32* digit)
{
    *digit = mDigit;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetGroupingSeparator(
    /* [out] */ Char32* separator)
{
    *separator = mGroupingSeparator;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetInfinity(
    /* [out] */ String* infinity)
{
    *infinity = mInfinity;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetMinusSign(
    /* [out] */ Char32* minusSign)
{
    *minusSign = mMinusSign;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetMonetaryDecimalSeparator(
    /* [out] */ Char32* separator)
{
    *separator = mMonetarySeparator;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetNaN(
    /* [out] */ String* infinity)
{
    *infinity = mNaN;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetPatternSeparator(
    /* [out] */ Char32* separator)
{
    *separator = mPatternSeparator;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetPercent(
    /* [out] */ Char32* percent)
{
    *percent = mPercent;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetPerMill(
    /* [out] */ Char32* perMill)
{
    *perMill = mPerMill;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetZeroDigit(
    /* [out] */ Char32* zeroDigit)
{
    *zeroDigit = mZeroDigit;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetExponentSeparator(
    /* [out] */ String* separator)
{
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
        return NOERROR;
    }
    mCurrency = currency;
    currency->GetCurrencyCode(&mIntlCurrencySymbol);
    currency->GetSymbolEx(mLocale, &mCurrencySymbol);
    return NOERROR;
}

ECode DecimalFormatSymbols::SetInternationalCurrencySymbol(
    /* [in] */ const String& value)
{
    if (value.IsNull()) {
        mCurrency = NULL;
        mIntlCurrencySymbol = NULL;
        return NOERROR;
    }

    if (value.Equals(mIntlCurrencySymbol)) {
        return NOERROR;
    }

    //try {
    assert(0);
//        Currency::GetInstance(value, (ICurrency**)&mCurrency);
    mCurrency->GetSymbolEx(mLocale, &mCurrencySymbol);
    //} catch (IllegalArgumentException e) {
    //    currency = null;
    //}
    mIntlCurrencySymbol = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetCurrencySymbol(
    /* [in] */ const String& value)
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
    /* [in] */ const String& value)
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
    /* [in] */ const String& value)
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
    /* [in] */ const String& value)
{
    if (value.IsNull()) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    mExponentSeparator = value;
    return NOERROR;
}
