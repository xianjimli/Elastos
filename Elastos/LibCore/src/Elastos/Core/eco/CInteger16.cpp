
#include "cmdef.h"
#include "CInteger16.h"


ECode CInteger16::constructor(
    /* [in] */ Int16 value)
{
    mValue = value;
    return NOERROR;
}

ECode CInteger16::GetValue(
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    return NOERROR;
}
