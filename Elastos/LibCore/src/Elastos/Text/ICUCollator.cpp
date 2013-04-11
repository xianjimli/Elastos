#include "ICUCollator.h"
#include "CICURuleBasedCollator.h"

IICUCollator* ICUCollator::GetInstance(
    /* [in] */ ILocale* locale)
{
    AutoPtr<IICURuleBasedCollator> icurbc;
    CICURuleBasedCollator::New(locale, (IICURuleBasedCollator**)&icurbc);
    IICUCollator * icuc = reinterpret_cast<IICUCollator*>(icurbc->Probe(EIID_IICUCollator));
    return icuc;
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