
#include "cmdef.h"
#include "CBoolean.h"

ECode CBoolean::constructor(
    /* [in] */ Boolean value)
{
    mValue = value;
    return NOERROR;
}

ECode CBoolean::GetValue(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    return NOERROR;
}
