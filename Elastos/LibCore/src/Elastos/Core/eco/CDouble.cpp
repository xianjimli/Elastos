
#include "cmdef.h"
#include "CDouble.h"


ECode CDouble::constructor(
    /* [in] */ Double value)
{
    mValue = value;
    return NOERROR;
}

ECode CDouble::GetValue(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    return NOERROR;
}
