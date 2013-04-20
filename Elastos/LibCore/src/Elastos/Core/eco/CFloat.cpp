
#include "cmdef.h"
#include "CFloat.h"


ECode CFloat::constructor(
    /* [in] */ Float value)
{
    mValue = value;
    return NOERROR;
}

ECode CFloat::GetValue(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    return NOERROR;
}
