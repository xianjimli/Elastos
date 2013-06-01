#ifndef __CURRENCY_H__
#define __CURRENCY_H__

#include "cmdef.h"
#include <Elastos.Utility_server.h>
#include <elastos/HashMap.h>
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<ILocale> >
{
    size_t operator()(AutoPtr<ILocale> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

_ELASTOS_NAMESPACE_END

class Currency : public ElRefBase, public ICurrency
{
public:
    Currency();

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    /**
     * Returns the {@code Currency} instance for the given currency code.
     * <p>
     *
     * @param currencyCode
     *            the currency code.
     * @return the {@code Currency} instance for this currency code.
     *
     * @throws IllegalArgumentException
     *             if the currency code is not a supported ISO 4217 currency
     *             code.
     */
    static CARAPI_(AutoPtr<ICurrency>) GetInstance(
        /* [in] */ const String& currencyCode);

    /**
     * Returns the {@code Currency} instance for this {@code Locale}'s country.
     *
     * @param locale
     *            the {@code Locale} of a country.
     * @return the {@code Currency} used in the country defined by the locale parameter.
     *
     * @throws IllegalArgumentException
     *             if the locale's country is not a supported ISO 3166 Country.
     */
    static CARAPI_(AutoPtr<ICurrency>) GetInstance(
        /* [in] */ ILocale* locale);

    CARAPI GetCurrencyCode(
        /* [out] */ String* currencyCode);

    CARAPI GetSymbol(
        /* [out] */ String* symbol);

    CARAPI GetSymbolEx(
        /* [in] */ ILocale* locale,
        /* [out] */ String* symbol);

    CARAPI GetDefaultFractionDigits(
        /* [out] */ Int32* defaultFractionDigits);

private:
    Currency(
        /* [in] */ const String& currencyCode);

private:
    static const Int64 sSerialVersionUID;

    static HashMap<String, AutoPtr<ICurrency> > mCodesToCurrencies;
    static HashMap<AutoPtr<ILocale>, AutoPtr<ICurrency> > mLocalesToCurrencies;

    String mCurrencyCode;

    // BEGIN android-added
    // TODO: this isn't set if we're restored from serialized form,
    // so getDefaultFractionDigits always returns 0!
    Int32 mDefaultFractionDigits;
    // END android-added
};

#endif //__CURRENCY_H__
