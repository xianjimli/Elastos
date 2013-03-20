
#ifndef __CDECIMALFORMATSYMBOLS_H__
#define __CDECIMALFORMATSYMBOLS_H__

#include "_CDecimalFormatSymbols.h"
#include "DecimalFormatSymbols.h"
CarClass(CDecimalFormatSymbols), public DecimalFormatSymbols
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ILocale * pLocale);

    CARAPI GetCurrency(
        /* [out] */ ICurrency ** ppCurrency);

    CARAPI GetInternationalCurrencySymbol(
        /* [out] */ String * pInternationalCurrencySymbol);

    CARAPI GetCurrencySymbol(
        /* [out] */ String * pCurrencySymbol);

    CARAPI GetDecimalSeparator(
        /* [out] */ Char32 * pDecimalSeparator);

    CARAPI GetDigit(
        /* [out] */ Char32 * pDigit);

    CARAPI GetGroupingSeparator(
        /* [out] */ Char32 * pGroupingSeparator);

    CARAPI GetInfinity(
        /* [out] */ String * pInfinity);

    CARAPI GetMinusSign(
        /* [out] */ Char32 * pMinusSign);

    CARAPI GetMonetaryDecimalSeparator(
        /* [out] */ Char32 * pMonetaryDecimalSeparator);

    CARAPI GetNaN(
        /* [out] */ String * pNaN);

    CARAPI GetPatternSeparator(
        /* [out] */ Char32 * pPatternSeparator);

    CARAPI GetPercent(
        /* [out] */ Char32 * pPercent);

    CARAPI GetPerMill(
        /* [out] */ Char32 * pPerMill);

    CARAPI GetZeroDigit(
        /* [out] */ Char32 * pZeroDigit);

    CARAPI GetExponentSeparator(
        /* [out] */ String * pExponentSeparator);

    CARAPI SetCurrency(
        /* [in] */ ICurrency * pCurrency);

    CARAPI SetInternationalCurrencySymbol(
        /* [in] */ const String& value);

    CARAPI SetCurrencySymbol(
        /* [in] */ const String& value);

    CARAPI SetDecimalSeparator(
        /* [in] */ Char32 value);

    CARAPI SetDigit(
        /* [in] */ Char32 value);

    CARAPI SetGroupingSeparator(
        /* [in] */ Char32 value);

    CARAPI SetInfinity(
        /* [in] */ const String& value);

    CARAPI SetMinusSign(
        /* [in] */ Char32 value);

    CARAPI SetMonetaryDecimalSeparator(
        /* [in] */ Char32 value);

    CARAPI SetNaN(
        /* [in] */ const String& value);

    CARAPI SetPatternSeparator(
        /* [in] */ Char32 value);

    CARAPI SetPercent(
        /* [in] */ Char32 value);

    CARAPI SetPerMill(
        /* [in] */ Char32 value);

    CARAPI SetZeroDigit(
        /* [in] */ Char32 value);

    CARAPI SetExponentSeparator(
        /* [in] */ const String& value);

private:
    // TODO: Add your private member variables here.
};

#endif // __CDECIMALFORMATSYMBOLS_H__
