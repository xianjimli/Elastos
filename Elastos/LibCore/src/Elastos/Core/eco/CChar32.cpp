
#include "cmdef.h"
#include "CChar32.h"


ECode CChar32::constructor(
    /* [in] */ Char32 value)
{
    mValue = value;
    return NOERROR;
}

ECode CChar32::GetValue(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    return NOERROR;
}
