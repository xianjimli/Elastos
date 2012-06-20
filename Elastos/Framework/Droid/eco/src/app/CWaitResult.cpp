
#include "app/CWaitResult.h"

ECode CWaitResult::constructor()
{
    return NOERROR;
}

ECode CWaitResult::constructor(
    /* [in] */ IParcel* source)
{
    return NOERROR;
}

ECode CWaitResult::DescribeContents(
    /* [out] */ Int32* result)
{
    if (result == NULL) return E_NOT_IMPLEMENTED;

    *result = 0;
    return NOERROR;
}

ECode CWaitResult::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWaitResult::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return E_NOT_IMPLEMENTED;
}

