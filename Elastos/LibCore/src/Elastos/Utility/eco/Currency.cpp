
#include "Currency.h"
#include "LocaleData.h"
#include "CLocaleHelper.h"

HashMap<String, AutoPtr<ICurrency> > Currency::sCodesToCurrencies;
HashMap<AutoPtr<ILocale>, AutoPtr<ICurrency> > Currency::sLocalesToCurrencies;

Currency::Currency()
    : mDefaultFractionDigits(0)
{}

Currency::Currency(
    /* [in] */ const String& currencyCode)
    : mDefaultFractionDigits(0)
{
    // BEGIN android-changed
    mCurrencyCode = currencyCode;

    // In some places the code XXX is used as the fall back currency.
    // The RI returns -1, but ICU defaults to 2 for unknown currencies.
    if (currencyCode.Equals(String("XXX"))) {
        mDefaultFractionDigits = -1;
        return;
    }

    // Ensure that we throw if the our currency code isn't an ISO currency code.
    // String symbol = ICU.getCurrencySymbolNative(Locale.US.toString(), currencyCode);
    // if (symbol == null) {
    //     throw badCurrency(currencyCode);
    // }

    // this.defaultFractionDigits = ICU.getCurrencyFractionDigitsNative(currencyCode);
    // if (defaultFractionDigits < 0) {
    //     // In practice, I don't think this can fail because ICU doesn't care whether you give
    //     // it a valid country code, and will just return a sensible default for the default
    //     // locale's currency.
    //     throw badCurrency(currencyCode);
    // }
    // END android-changed
}

PInterface Currency::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ICurrency*)this;
    }
    else if (riid == EIID_ICurrency) {
        return (ICurrency*)this;
    }

    return NULL;
}

UInt32 Currency::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Currency::Release()
{
    return ElRefBase::Release();
}

ECode Currency::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(ICurrency*)this) {
        *pIID = EIID_ICurrency;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

AutoPtr<ICurrency> Currency::GetInstance(
    /* [in] */ const String& currencyCode)
{
    // BEGIN android-changed
    HashMap<String, AutoPtr<ICurrency> >::Iterator it =
            sCodesToCurrencies.Find(currencyCode);
    if (it != sCodesToCurrencies.End()) {
        return it->mSecond;
    }

    Currency* currency = new Currency(currencyCode);
    sCodesToCurrencies[currencyCode] = (ICurrency*)currency;
    return (ICurrency*)currency;
    // END android-changed
}

AutoPtr<ICurrency> Currency::GetInstance(
    /* [in] */ ILocale* locale)
{
    // BEGIN android-changed
    HashMap<AutoPtr<ILocale>, AutoPtr<ICurrency> >::Iterator it =
            sLocalesToCurrencies.Find(locale);
    if (it != sLocalesToCurrencies.End()) {
        return it->mSecond;
    }

    String country, variant;
    locale->GetCountry(&country);
    locale->GetVariant(&variant);
    if (variant.GetLength() > 0 && (variant.Equals(String("EURO"))
            || variant.Equals(String("HK")) || variant.Equals(String("PREEURO")))) {
        country = country + String("_") + variant;
    }

    // String currencyCode = ICU.getCurrencyCodeNative(country);
    // if (currencyCode == null) {
    //     throw new IllegalArgumentException("Not a supported ISO 3166 country: " + locale);
    // } else if (currencyCode.equals("None")) {
    //     return null;
    // }
    // AutoPtr<ICurrency> result = GetInstance(currencyCode);
    // sLocalesToCurrencies[locale] = result;
    // return result;
    // END android-changed

    return NULL;
}

ECode Currency::GetCurrencyCode(
    /* [out] */ String* currencyCode)
{
    VALIDATE_NOT_NULL(currencyCode);

    *currencyCode = mCurrencyCode;
    return NOERROR;
}

ECode Currency::GetSymbol(
    /* [out] */ String* symbol)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));

    return GetSymbolEx(locale, symbol);
}

ECode Currency::GetSymbolEx(
    /* [in] */ ILocale* locale,
    /* [out] */ String* symbol)
{
    VALIDATE_NOT_NULL(symbol);

    String country;
    locale->GetCountry(&country);
    if (country.GetLength() == 0) {
        *symbol = mCurrencyCode;
        return NOERROR;
    }

    // Check the locale first, in case the locale has the same currency.
    AutoPtr<ILocaleData> localeData = LocaleData::Get(locale);
    if (((LocaleData*)localeData.Get())->mInternationalCurrencySymbol.Equals(mCurrencyCode)) {
        *symbol = ((LocaleData*)localeData.Get())->mCurrencySymbol;
        return NOERROR;
    }

    // Try ICU, and fall back to the currency code if ICU has nothing.
    //String symbol = ICU.getCurrencySymbolNative(locale.toString(), currencyCode);
    //return symbol != null ? symbol : currencyCode;

    *symbol = mCurrencyCode;
    return NOERROR;
}

ECode Currency::GetDefaultFractionDigits(
    /* [out] */ Int32* defaultFractionDigits)
{
    // BEGIN android-changed
    // return com.ibm.icu.util.Currency.getInstance(currencyCode).getDefaultFractionDigits();
    VALIDATE_NOT_NULL(defaultFractionDigits);

    *defaultFractionDigits = mDefaultFractionDigits;
    // END android-changed
    return NOERROR;
}