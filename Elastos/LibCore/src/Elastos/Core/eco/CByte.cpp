
#include "cmdef.h"
#include "CByte.h"

ECode CByte::constructor(
    /* [in] */ Byte value)
{
    mValue = value;
    return NOERROR;
}

ECode CByte::GetValue(
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    return NOERROR;
}
