
#include "cmdef.h"
#include "ICUCollator.h"
#include "ICURuleBasedCollator.h"

AutoPtr<IICUCollator> ICUCollator::GetInstance(
    /* [in] */ ILocale* locale)
{
    return (IICUCollator*)new ICURuleBasedCollator(locale);
}

ECode ICUCollator::Equals(
    /* [in] */ const String& source,
    /* [in] */ const String& target,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 value;
    Compare(source, target, &value);
    *result = value == 0;
    return NOERROR;
}