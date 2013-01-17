
#include "cmdef.h"
#include "CInteger32.h"

ECode CInteger32::constructor(
    /* [in] */ Int32 value)
{
    mValue = value;
    return NOERROR;
}

ECode CInteger32::GetValue(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    return NOERROR;
}
