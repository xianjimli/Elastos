
#include "cmdef.h"
#include "CInteger64.h"


ECode CInteger64::constructor(
    /* [in] */ Int64 value)
{
    mValue = value;
    return NOERROR;
}

ECode CInteger64::GetValue(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    return NOERROR;
}
