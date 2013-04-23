
#include "content/CIntegerMapWrapper.h"
#include "ext/frameworkdef.h"

ECode CIntegerMapWrapper::GetKey(
    /* [out] */ Int32* key)
{
    VALIDATE_NOT_NULL(key);
    *key = mKey;
    return NOERROR;
}

ECode CIntegerMapWrapper::GetValue(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    return NOERROR;
}

ECode CIntegerMapWrapper::constructor(
    /* [in] */ Int32 key,
    /* [in] */ Int32 value)
{
    mKey = key;
    mValue = value;
    return NOERROR;
}

