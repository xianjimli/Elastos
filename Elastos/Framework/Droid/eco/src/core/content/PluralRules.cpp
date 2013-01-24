
#include "content/PluralRules.h"

const Int32 PluralRules::QUANTITY_OTHER;
const Int32 PluralRules::QUANTITY_ZERO;
const Int32 PluralRules::QUANTITY_ONE;
const Int32 PluralRules::QUANTITY_TWO;
const Int32 PluralRules::QUANTITY_FEW;
const Int32 PluralRules::QUANTITY_MANY;
const Int32 PluralRules::ID_OTHER;

Int32 PluralRules::AttrForNumber(
    /* [in] */ Int32 n)
{
    return -1; // E_NOT_IMPLEMENTED
}

String PluralRules::StringForQuantity(
    /* [in] */ Int32 quantity)
{
    return String(""); // E_NOT_IMPLEMENTED
}

PluralRules* PluralRules::RuleForLocale(
    /* [in] */ ILocale* locale)
{
    return NULL; // E_NOT_IMPLEMENTED
}

