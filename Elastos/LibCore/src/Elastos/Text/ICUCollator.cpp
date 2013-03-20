#include "ICUCollator.h"

IICUCollator* ICUCollator::GetInstance(
    /* [in] */ ILocale* locale)
{
//    return new RuleBasedCollator(locale);
    return NULL;
}

ECode ICUCollator::Equals(
        /* [in] */ String source,
        /* [in] */ String target,
        /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 value;
    Compare(source, target, &value);
    *result = (value == 0) ? TRUE : FALSE;
    return NOERROR;
}