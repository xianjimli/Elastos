
#ifndef __HH__PLURALRULES_H
#define __HH__PLURALRULES_H

#include <elastos/Locale.h>

using namespace Elastos::Utility;

class PluralRules
{
public:
    static const Int32 QUANTITY_OTHER = 0x0000;
    static const Int32 QUANTITY_ZERO  = 0x0001;
    static const Int32 QUANTITY_ONE   = 0x0002;
    static const Int32 QUANTITY_TWO   = 0x0004;
    static const Int32 QUANTITY_FEW   = 0x0008;
    static const Int32 QUANTITY_MANY  = 0x0010;

    static const Int32 ID_OTHER = 0x01000004;

public:
    virtual CARAPI_(Int32) QuantityForNumber(
        /* [in] */ Int32 n) = 0;

    CARAPI_(Int32) AttrForNumber(
        /* [in] */ Int32 n);

    static CARAPI_(String) StringForQuantity(
        /* [in] */ Int32 quantity);

    static CARAPI_(PluralRules*) RuleForLocale(
        /* [in] */ Locale* locale);

private:
};

#endif // __HH__PLURALRULES_H
